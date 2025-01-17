// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef InputBuffer_h
#define InputBuffer_h

#include "config.h"

#define IB_REQUEST_READ               0
#define IB_DATA_READ                  1
#define IB_NO_MORE_REQUEST            2
#define IB_UNEXPECTED_END_OF_FILE     3
#define IB_SHOULD_TERMINATE           4
#define IB_LINE_TOO_LONG              5
#define IB_END_OF_FILE                6
#define IB_EMPTY_REQUEST              7
#define IB_TIMEOUT                    8

#define IB_BUFFER_SIZE            65536

#include <string>
#include <deque>
using namespace std;

class InputBuffer
{
    int _fd;
    int *_termination_flag;
    typedef deque<string> _requestlines_t;
    _requestlines_t _requestlines;
    char _readahead_buffer[IB_BUFFER_SIZE];
    char *_read_pointer;
    char *_write_pointer;
    char *_end_pointer;

    // some buffer
public:
    InputBuffer(int *termination_flag);
    void setFd(int fd);
    int readRequest();
    bool moreLines() { return !_requestlines.empty(); }
    string nextLine();

private:
    void storeRequestLine(char *line, int length);
    int readData();
};


#endif // InputBuffer_h
