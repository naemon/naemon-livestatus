// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TimePointerColumn_h
#define TimePointerColumn_h

#include "TimeColumn.h"

class TimePointerColumn : public TimeColumn
{
    time_t *_number;
public:
    TimePointerColumn(string name, string description, time_t* number)
        : TimeColumn(name, description, -1), _number(number) {}
    time_t getValue(void *, Query *) { return *_number; }
};


#endif // TimePointerColumn_h
