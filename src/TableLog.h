// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableLog_h
#define TableLog_h

#include <map>
#include <time.h>
#include "config.h"
#include "Table.h"

class Logfile;

class TableLog : public Table
{

public:
    TableLog();
    ~TableLog();
    const char *name() { return "log"; }
    const char *prefixname() { return "logs"; }
    bool isAuthorized(contact *ctc, void *data);
    void addColumns(Table *, string prefix, int indirect_offset, bool add_host = true, bool add_service = true);
    void answerQuery(Query *query);
    Column *column(const char *colname); // override in order to handle current_
    bool hasLogcache() { return true; };

private:
    bool answerQuery(Query *, Logfile *, time_t, time_t);
};

#endif // TableLog_h
