// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableServices_h
#define TableServices_h

#include "config.h"

#include <set>
#include "Table.h"
#include "nagios.h"

using namespace std;
class TableHosts;
class TableContacts;
class TableDowntimes;

struct servicebygroup {
    service               *_service;
    host                  *_host;
    servicegroup          *_servicegroup;
    struct servicebygroup *_next;
};

struct servicebyhostgroup {
    service                   *_service;
    host                  *_host;
    hostgroup                 *_hostgroup;
    struct servicebyhostgroup *_next;
};

class TableServices : public Table
{
    bool _by_group;
    bool _by_hostgroup; // alternative to _by_group

public:
    TableServices(bool by_group, bool by_hostgroup);
    const char *name() { return _by_group ? "servicesbygroup" : \
        (_by_hostgroup ? "servicesbyhostgroup" : "services"); }
    const char *prefixname() { return "services"; }
    bool isAuthorized(contact *, void *);
    void *findObject(char *objectspec);
    void add(service *svc);
    void answerQuery(Query *);
    void cleanupQuery(Query *);
    void addColumns(Table *, string prefix, int indirect_offset, bool add_hosts);
};


#endif // TableServices_h
