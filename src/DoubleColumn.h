// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef DoubleColumn_h
#define DoubleColumn_h

#include "config.h"

#include "Column.h"

class DoubleColumn : public Column
{
public:
    DoubleColumn(string name, string description, int indirect_offset)
        : Column(name, description, indirect_offset) {}
    virtual double getValue(void *data) = 0;
    void output(void *, Query *);
    int type() { return COLTYPE_DOUBLE; }
    string valueAsString(void *data, Query *);
    Filter *createFilter(int operator_id, char *value);
    int compare(void *dataa, void*datab, Query *);
};

#endif // DoubleColumn_h
