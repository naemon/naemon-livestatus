// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableHosts_h
#define TableHosts_h

#include <set>
#include "config.h"
#include "Table.h"
#include "nagios.h"

using namespace std;

class TableContacts;
class TableDownComm;

struct hostbygroup {
    host      *_host;
    hostgroup *_hostgroup;
    struct hostbygroup *_next;
};

class TableHosts : public Table
{
    bool _by_group;

public:
    TableHosts(bool by_group);
    const char *name() { return _by_group ? "hostsbygroup" : "hosts"; }
    const char *prefixname() { return "hosts"; }
    bool isAuthorized(contact *ctc, void *data);
    void *findObject(char *objectspec);
    void addColumns(Table *, string prefix, int indirect_offset);
    void answerQuery(Query *query);
    void cleanupQuery(Query *query);
};

#endif // TableHosts_h
