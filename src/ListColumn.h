// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ListColumn_h
#define ListColumn_h

#include "config.h"

#include "Column.h"

class ListColumn : public Column
{
public:
    ListColumn(string name, string description, int indirect_offset) :
        Column(name, description, indirect_offset) {}
    int type() { return COLTYPE_LIST; }
    virtual void *getNagiosObject(char *name) = 0;
    virtual bool isNagiosMember(void *data, void *member) = 0;
    virtual bool isEmpty(void *data) = 0;
    Filter *createFilter(int opid, char *value);
};

#endif // ListColumn_h
