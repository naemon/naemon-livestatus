// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef EmptyColumn_h
#define EmptyColumn_h

#include "config.h"

#include "Column.h"

class EmptyColumn : public Column
{
public:
    EmptyColumn(string name, string description) :
        Column(name, description, -1) {}
    int type() { return COLTYPE_STRING; }
    void output(void *data, Query *);
    Filter *createFilter(int operator_id, char *value);
};

#endif // EmptyColumn_h
