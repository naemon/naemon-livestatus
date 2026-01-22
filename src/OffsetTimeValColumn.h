// Copyright (c) 2026 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OffsetTimeValColumn_h
#define OffsetTimeValColumn_h

#include "config.h"

#include <stdlib.h>
#include "TimeValColumn.h"


/* This does almost the same as the time column,
   but applies a timezone offset stored in the Query. */

class OffsetTimeValColumn : public TimeValColumn
{
    int _offset;
public:
    OffsetTimeValColumn(string name, string description, int offset, int indirect_offset = -1)
        : TimeValColumn(name, description, indirect_offset), _offset(offset) {}
    struct timeval getValue(void *data, Query *);
protected:
    int offset() { return _offset; }
};


#endif // OffsetTimeValColumn_h
