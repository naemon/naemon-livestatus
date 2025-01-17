// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OffsetTimeColumn_h
#define OffsetTimeColumn_h

#include "config.h"

#include <stdlib.h>
#include "TimeColumn.h"


/* This does almost the same as the time column,
   but applies a timezone offset stored in the Query. */

class OffsetTimeColumn : public TimeColumn
{
    int _offset;
public:
    OffsetTimeColumn(string name, string description, int offset, int indirect_offset = -1)
        : TimeColumn(name, description, indirect_offset), _offset(offset) {}
    time_t getValue(void *data, Query *);
protected:
    int offset() { return _offset; }
};


#endif // OffsetTimeColumn_h
