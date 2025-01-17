// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableCommands_h
#define TableCommands_h

#include "config.h"

#include <set>
#include "Table.h"
#include "nagios.h"

class TableCommands : public Table
{
public:
    TableCommands();
    const char *name() { return "commands"; }
    void answerQuery(Query *query);
    void addColumns(Table *table, string prefix, int indirect_offset);
};

#endif // TableCommands_h
