// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableStatus_h
#define TableStatus_h

#include "config.h"

#include "Table.h"
class Query;

class TableStatus : public Table
{
public:
    TableStatus();
    const char *name() { return "status"; }
    void answerQuery(Query *query);
};


#endif // TableStatus_h
