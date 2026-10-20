// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OutputBuffer_h
#define OutputBuffer_h

#include "config.h"

#include <string>
#include <string.h>
using namespace std;

#define INITIAL_OUTPUT_BUFFER_SIZE 1024

#define RESPONSE_CODE_OK                 200
#define RESPONSE_CODE_INVALID_HEADER     400
#define RESPONSE_CODE_UNAUTHORIZED       403
#define RESPONSE_CODE_NOT_FOUND          404
#define RESPONSE_CODE_LIMIT_EXCEEDED     413
#define RESPONSE_CODE_INCOMPLETE_REQUEST 451
#define RESPONSE_CODE_INVALID_REQUEST    452
#define RESPONSE_CODE_UNKNOWN_COLUMN     450

class OutputBuffer
{
    int *_termination_flag;
    char *_buffer;
    char *_writepos;
    char *_end;
    size_t _max_size;
    int _response_header;
    unsigned _response_code;
    string _error_message;
    bool _do_keepalive;

public:
    OutputBuffer(int *termination_flag);
    ~OutputBuffer();
    const char *buffer() { return _buffer; }
    size_t size() { return _writepos - _buffer; }

    // The append helpers addChar, addString and addBuffer are defined here rather than in OutputBuffer.cc so
    // that callers in other translation units (Query.cc) can inline them.

    inline void addChar(char c)
    {
        if (_writepos + 1 > _end)
            needSpace(1);
        *_writepos++ = c;
    }

    inline void addString(const char *s)
    {
        addBuffer(s, strlen(s));
    }

    inline void addBuffer(const char *buf, size_t len)
    {
        if (_writepos + len > _end)
            needSpace(len);
        memcpy(_writepos, buf, len);
        _writepos += len;
    }

    // Appends the run of bytes starting at *src up to (but not including) the first byte for which is_significant[*src] != 0,
    // then advances *src to that byte so the caller can handle it.
    inline void addUntilNextSignificantChar(const char **src,
                                            const unsigned char *is_significant)
    {
        const char *start = *src;
        const char *p = start;
        while (is_significant[(unsigned char)*p] == 0)
            p++;
        if (p != start) {
            addBuffer(start, (size_t)(p - start));
            *src = p;
        }
    }

    void reset();
    void flush(int fd);
    bool shouldTerminate();
    void setResponseHeader(int r) { _response_header = r; }
    int responseHeader() { return _response_header; }
    void setDoKeepalive(bool d) { _do_keepalive = d; }
    bool doKeepalive() { return _do_keepalive; }
    void setError(unsigned code, const char *format, ...);
    bool hasError() { return _error_message != ""; }
    bool isAlive(int fd);

private:
    void needSpace(size_t);
    void writeData(int fd, const char *, size_t);
};


#endif // OutputBuffer_h
