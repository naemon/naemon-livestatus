// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OffsetStringColumn_h
#define OffsetStringColumn_h

#include "config.h"

#include "StringColumn.h"

class OffsetStringColumn : public StringColumn
{
    int _offset;
public:
    OffsetStringColumn(string name, string description, int offset, int indirect_offset = -1)
        : StringColumn(name, description, indirect_offset), _offset(offset) {}
    const char *getValue(void *data);
};


#endif // OffsetStringColumn_h
