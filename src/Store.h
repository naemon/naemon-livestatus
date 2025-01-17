// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef Store_h
#define Store_h

#include "config.h"

#include "TableServices.h"
#include "TableHosts.h"
#include "TableHostgroups.h"
#include "TableServicegroups.h"
#include "TableContacts.h"
#include "TableCommands.h"
#include "TableTimeperiods.h"
#include "TableContactgroups.h"
#include "TableDownComm.h"
#include "TableStatus.h"
#include "TableLog.h"
#include "TableStateHistory.h"
#include "TableColumns.h"
#include "OutputBuffer.h"
#include "InputBuffer.h"
#include "LogCache.h"

class Store
{
    LogCache           _log_cache;
    TableContacts      _table_contacts;
    TableCommands      _table_commands;
    TableHostgroups    _table_hostgroups;
    TableHosts         _table_hosts;
    TableHosts         _table_hostsbygroup;
    TableServicegroups _table_servicegroups;
    TableServices      _table_services;
    TableServices      _table_servicesbygroup;
    TableServices      _table_servicesbyhostgroup;
    TableTimeperiods   _table_timeperiods;
    TableContactgroups _table_contactgroups;
    TableDownComm      _table_downtimes;
    TableDownComm      _table_comments;
    TableStatus        _table_status;
    TableLog           _table_log;
    TableStateHistory  _table_statehistory;
    TableColumns       _table_columns;

    typedef map<string, Table *> _tables_t;
    _tables_t _tables;

public:
    Store();
    LogCache* logCache(){ return &_log_cache; };
    void registerHostgroup(hostgroup *);
    void registerComment(nebstruct_comment_data *);
    void registerDowntime(nebstruct_downtime_data *);
    bool answerRequest(InputBuffer *, OutputBuffer *, int);

private:
    Table *findTable(string name);
    void answerGetRequest(InputBuffer *, OutputBuffer *, const char *, int fd);
    void answerCommandRequest(const char *, OutputBuffer *);
};

#endif // Store_h
