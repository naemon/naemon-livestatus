// +------------------------------------------------------------------+
// |             ____ _               _        __  __ _  __           |
// |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
// |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
// |           | |___| | | |  __/ (__|   <    | |  | | . \            |
// |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
// |                                                                  |
// | Copyright Mathias Kettner 2012             mk@mathias-kettner.de |
// +------------------------------------------------------------------+
//
// This file is part of Check_MK.
// The official homepage is at http://mathias-kettner.de/check_mk.
//
// check_mk is free software;  you can redistribute it and/or modify it
// under the  terms of the  GNU General Public License  as published by
// the Free Software Foundation in version 2.  check_mk is  distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY;  with-
// out even the implied warranty of  MERCHANTABILITY  or  FITNESS FOR A
// PARTICULAR PURPOSE. See the  GNU General Public License for more de-
// ails.  You should have  received  a copy of the  GNU  General Public
// License along with GNU Make; see the file  COPYING.  If  not,  write
// to the Free Software Foundation, Inc., 51 Franklin St,  Fifth Floor,
// Boston, MA 02110-1301 USA.

#include "OutputBuffer.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

#include "logger.h"
#include "Query.h"

#define WRITE_TIMEOUT_USEC 100000


OutputBuffer::OutputBuffer()
{
    _buffer.reserve(INITIAL_OUTPUT_BUFFER_SIZE);
    reset();
}

OutputBuffer::~OutputBuffer()
{
}

void OutputBuffer::reset()
{
    _buffer.clear();
    _response_header = RESPONSE_HEADER_OFF;
    _response_code = RESPONSE_CODE_OK;
    _do_keepalive = false;
    _error_message = "";
}

void OutputBuffer::addChar(char c)
{
    _buffer.push_back(c);
}

void OutputBuffer::addString(const char *s)
{
    _buffer.append(s);
}

void OutputBuffer::addBuffer(const char *buf, unsigned len)
{
    _buffer.append(buf, len);
}

void OutputBuffer::flush(int fd, int *termination_flag)
{
    if (_response_header == RESPONSE_HEADER_FIXED16)
    {
        int s = size();
        const char *buffer = NULL;
        // if response code is not OK, output error
        // message instead of data
        if (_response_code != RESPONSE_CODE_OK)
        {
            buffer = _error_message.c_str();
            s = _error_message.size();
        }
        else {
            buffer = _buffer.c_str();
        }

        char header[17];
        snprintf(header, sizeof(header), "%03d %11d\n", _response_code, s);
        writeData(fd, termination_flag, header, 16);
        writeData(fd, termination_flag, buffer, s);
    }
    else
        writeData(fd, termination_flag, _buffer.c_str(), size());
    reset();
}


void OutputBuffer::writeData(int fd, int *termination_flag, const char *write_from, int to_write)
{
    struct timeval tv;
    while (!*termination_flag && to_write > 0)
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

