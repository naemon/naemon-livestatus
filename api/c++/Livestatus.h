// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef Livestatus_h
#define Livestatus_h

#include <stdio.h>
#include <vector>
#include <string>

// simple C++ API for accessing Livestatus from C++,
// currently supports only UNIX sockets, no TCP. But
// this is only a simple enhancement.

class Livestatus
{
    int _connection;
    FILE *_file;

public:
    Livestatus() : _connection(-1), _file(0) {};
    ~Livestatus();
    void connectUNIX(const char *socketpath);
    bool isConnected() const { return _connection >= 0; };
    void disconnect();
    void sendQuery(const char *query);
    std::vector<std::string> *nextRow();
};



#endif // Livestatus_h
