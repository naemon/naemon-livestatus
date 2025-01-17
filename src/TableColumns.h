// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableColumns_h
#define TableColumns_h

#include "config.h"

#include <vector>
#include "Table.h"

class TableColumns : public Table
{
    typedef vector<Table *> _tables_t;
    _tables_t _tables;
public:
    TableColumns();
    const char *name() { return "columns"; }
    void addTable(Table *);
    void answerQuery(Query *query);
    const char *getValue(Column *column, int colcol);
    const char *tableNameOf(Column *column);
};


#endif // TableColumns_h
