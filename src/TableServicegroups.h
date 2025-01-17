// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableServicegroups_h
#define TableServicegroups_h

#include "config.h"

#include <set>
#include "Table.h"
#include "nagios.h"

class TableServicegroups : public Table
{
public:
    TableServicegroups();
    const char *name() { return "servicegroups"; }
    void *findObject(char *objectspec);
    bool isAuthorized(contact *, void *);
    void addColumns(Table *, string prefix, int indirect_offset);
    void add(servicegroup *);
    void answerQuery(Query *query);
};

#endif // TableServicegroups_h
