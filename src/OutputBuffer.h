// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OutputBuffer_h
#define OutputBuffer_h

#include "config.h"

#include <string>
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
    unsigned _max_size;
    int _response_header;
    unsigned _response_code;
    string _error_message;
    bool _do_keepalive;

public:
    OutputBuffer(int *termination_flag);
    ~OutputBuffer();
    const char *buffer() { return _buffer; }
    unsigned size() { return _writepos - _buffer; }
    void addChar(char c);
    void addString(const char *);
    void addBuffer(const char *, unsigned);
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
    void needSpace(unsigned);
    void writeData(int fd, const char *, int);
};


#endif // OutputBuffer_h
