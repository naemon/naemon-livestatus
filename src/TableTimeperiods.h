// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableTimeperiods_h
#define TableTimeperiods_h

#include "Table.h"

class TableTimeperiods : public Table
{
public:
    TableTimeperiods();
    const char *name() { return "timeperiods"; }
    void answerQuery(Query *query);
    void addColumns(Table *table, string prefix, int indirect_offset);
};

#endif // TableTimeperiods_h
