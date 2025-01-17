// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableContacts_h
#define TableContacts_h

#include "config.h"
#include "Table.h"
#include "nagios.h"

class TableContacts : public Table
{
public:
    TableContacts();
    const char *name() { return "contacts"; }
    void *findObject(char *objectspec);
    void addColumns(Table *, string prefix, int indirect_offset);
    void answerQuery(Query *query);
};

#endif // TableContacts_h
