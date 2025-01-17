// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableHostgroups_h
#define TableHostgroups_h

#include "config.h"

#include <set>
#include "Table.h"
#include "nagios.h"

class TableHostgroups : public Table
{
public:
    TableHostgroups();
    const char *name() { return "hostgroups"; }
    void *findObject(char *objectspec);
    bool isAuthorized(contact *, void *);
    void addColumns(Table *, string prefix, int indirect_offset);
    void add(hostgroup *ctc);
    void answerQuery(Query *query);
};

#endif // TableHostgroups_h
