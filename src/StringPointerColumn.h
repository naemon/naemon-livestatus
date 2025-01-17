// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef StringPointerColumn_h
#define StringPointerColumn_h

#include "StringColumn.h"

class StringPointerColumn : public StringColumn
{
    const char *_string;
public:
    StringPointerColumn(string name, string description, const char *string)
        : StringColumn(name, description, -1), _string(string) {}
    const char* getValue(void* ) { return _string; }
};


#endif // StringPointerColumn_h
