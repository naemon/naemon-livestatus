// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "OutputBuffer.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

#include "logger.h"
#include "Query.h"

#define WRITE_TIMEOUT_USEC 100000


OutputBuffer::OutputBuffer(int *termination_flag) :
  _termination_flag(termination_flag),
  _max_size(INITIAL_OUTPUT_BUFFER_SIZE)
{
    _buffer = (char *)malloc(_max_size);
    _end = _buffer + _max_size;
    reset();
}

OutputBuffer::~OutputBuffer()
{
    free(_buffer);
}

void OutputBuffer::reset()
{
    _writepos = _buffer;
    _response_header = RESPONSE_HEADER_OFF;
    _response_code = RESPONSE_CODE_OK;
    _do_keepalive = false;
    _error_message = "";
}

void OutputBuffer::addChar(char c)
{
    needSpace(1);
    *_writepos++ = c;
}

void OutputBuffer::addString(const char *s)
{
    int l = strlen(s);
    addBuffer(s, l);
}

void OutputBuffer::addBuffer(const char *buf, unsigned len)
{
    needSpace(len);
    memcpy(_writepos, buf, len);
    _writepos += len;
}

void OutputBuffer::needSpace(unsigned len)
{
    if (_writepos + len > _end)
    {
        unsigned s = size();
        unsigned needed = s + len;
        while (_max_size < needed) {
            // increase until enough space
            // double untill 500MB
            if(_max_size < 500 * 1024 * 1024) {
                _max_size *= 2;
            } else {
                // increase by 25% afterwards
                _max_size += (_max_size / 4);
            }
        }

        _buffer = (char *)realloc(_buffer, _max_size);
        _writepos = _buffer + s;
        _end = _buffer + _max_size;
    }
}

void OutputBuffer::flush(int fd)
{
    const char *buffer = _buffer;
    int s = size();
    // if response code is not OK, output error
    // message instead of data
    if (_response_code != RESPONSE_CODE_OK)
    {
        buffer = _error_message.c_str();
        s = _error_message.size();
    }
    if (_response_header == RESPONSE_HEADER_FIXED16)
    {
        char header[17];
        snprintf(header, sizeof(header), "%03d %11d\n", _response_code, s);
        writeData(fd, header, 16);
        writeData(fd, buffer, s);
    }
    else
        writeData(fd, buffer, s);
    reset();
}


void OutputBuffer::writeData(int fd, const char *write_from, int to_write)
{
    struct timeval tv;
    while (!*_termination_flag && to_write > 0)
    {
        tv.tv_sec  = WRITE_TIMEOUT_USEC / 1000000;
        tv.tv_usec = WRITE_TIMEOUT_USEC % 1000000;

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        int retval = select(fd + 1, NULL, &fds, NULL, &tv);
        if (retval > 0 && FD_ISSET(fd, &fds)) {
            ssize_t w = write(fd, write_from, to_write);
            if (w < 0) {
                logger(LG_INFO, "Couldn't write %d bytes to client socket: %s", to_write, strerror(errno));
                break;
            }
            else if (w == 0)
                logger(LG_INFO, "Strange: wrote 0 bytes inspite of positive select()");
            else {
                to_write -= w;
            }
        }
    }
}

bool OutputBuffer::isAlive(int fd)
{
    struct timeval tv = {0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    int retval = select(fd + 1, NULL, &fds, NULL, &tv);
    if (retval > 0 && FD_ISSET(fd, &fds)) {
        ssize_t w = write(fd, "", 0);
        if (w < 0) {
            // select returned file handle, but write failed -> client is gone
            return false;
        }
        // select returned file handle and write succeeded -> client is alive
        return true;
    }

    // select returned no file handle -> client is gone
    return false;
}

void OutputBuffer::setError(unsigned code, const char *format, ...)
{
    // only the first error is being returned
    if (_error_message == "")
    {
        char buffer[8192];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, sizeof(buffer) - 1, format, ap);
        logger(LG_INFO, "error: %s", buffer);
        strcat(buffer, "\n");
        va_end(ap);
        _error_message = buffer;
        _response_code = code;
    }
}

bool OutputBuffer::shouldTerminate()
{
    if(*_termination_flag) {
        return true;
    }
    return false;
}
