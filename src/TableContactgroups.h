// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableContactgroups_h
#define TableContactgroups_h

#include <set>
#include "config.h"
#include "Table.h"
#include "nagios.h"

class TableContactgroups : public Table
{
public:
    TableContactgroups();
    const char *name() { return "contactgroups"; }
    void *findObject(char *objectspec);
    void answerQuery(Query *query);
    void addColumns(Table *table, string prefix, int indirect_offset);
};

#endif // TableContactgroups_h
