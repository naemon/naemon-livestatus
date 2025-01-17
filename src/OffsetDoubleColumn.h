// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OffsetDoubleColumn_h
#define OffsetDoubleColumn_h

#include "config.h"

#include <stdlib.h>
#include "DoubleColumn.h"

class OffsetDoubleColumn : public DoubleColumn
{
    int _offset;
public:
    OffsetDoubleColumn(string name, string description,
                       int offset, int indirect_offset = -1)
        : DoubleColumn(name, description, indirect_offset), _offset(offset) {}
    double getValue(void *data);
protected:
    int offset() { return _offset; }
};


#endif // OffsetDoubleColumn_h
