// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ServicelistDependencyColumn_h
#define ServicelistDependencyColumn_h

#include "nagios.h"
#include "config.h"
#include "Column.h"

class ServicelistDependencyColumn : public Column
{
    int  _offset;
    bool  _with_info;
public:
    ServicelistDependencyColumn(string name, string description, int offset, int indirect_offset, bool with_info)
        : Column(name, description, indirect_offset), _offset(offset), _with_info(with_info) {}
    int type() { return COLTYPE_LIST; }
    void output(void *, Query *);
    objectlist *getList(void *data);
    Filter *createFilter(int opid, char *value);
};

#endif // ServicelistDependencyColumn_h
