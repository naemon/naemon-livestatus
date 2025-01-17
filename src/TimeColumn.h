// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TimeColumn_h
#define TimeColumn_h

#include "config.h"

#include <stdint.h>

#include "Column.h"

class TimeColumn : public Column
{
public:
    TimeColumn(string name, string description, int indirect_offset)
        : Column(name, description, indirect_offset) {}
    virtual time_t getValue(void *data, Query *) = 0;
    void output(void *, Query *);
    int type() { return COLTYPE_INT; }
    string valueAsString(void *data, Query *);
    Filter *createFilter(int operator_id, char *value);
    int compare(void *dataa, void*datab, Query *query);
};

#endif // TimeColumn_h
