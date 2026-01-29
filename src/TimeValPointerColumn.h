// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TimeValPointerColumn_h
#define TimeValPointerColumn_h

#include "TimeValColumn.h"

class TimeValPointerColumn : public TimeValColumn
{
    struct timeval *_time;
public:
    TimeValPointerColumn(string name, string description, struct timeval* time)
        : TimeValColumn(name, description, -1), _time(time) {}
    struct timeval getValue(void *, Query *) { return *_time; }
};


#endif // TimeValPointerColumn_h
