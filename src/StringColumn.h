// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef StringColumn_h
#define StringColumn_h

#include "config.h"

#include "Column.h"

class StringColumn : public Column
{
public:
    StringColumn(string name, string description, int indirect_offset) :
        Column(name, description, indirect_offset) {}
    virtual const char *getValue(void *data) = 0;
    string valueAsString(void *data, Query *) { return getValue(data); }
    void output(void *, Query *);
    int type() { return COLTYPE_STRING; }
    Filter *createFilter(int operator_id, char *value);
    int compare(void *dataa, void*datab, Query *query);
};

#endif // StringColumn_h
