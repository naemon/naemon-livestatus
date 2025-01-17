// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OffsetIntColumn_h
#define OffsetIntColumn_h

#include "config.h"

#include <stdlib.h>
#include "IntColumn.h"

class OffsetIntColumn : public IntColumn
{
    int _offset;
public:
    OffsetIntColumn(string name, string description, int offset, int indirect_offset = -1)
        : IntColumn(name, description, indirect_offset), _offset(offset) {}
    int32_t getValue(void *data, Query *);
protected:
    int offset() { return _offset; }
};


#endif // OffsetIntColumn_h
