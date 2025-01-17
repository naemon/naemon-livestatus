// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableStateHistory_h
#define TableStateHistory_h

#include <map>
#include <time.h>
#include "config.h"
#include "string.h"
#include "logger.h"
#include "nagios.h"
#include "Logfile.h"
#include "LogCache.h"
#include "HostServiceState.h"
#include "Query.h"

#define CLASSMASK_STATEHIST 0xC6

class HostServiceState;

typedef map<HostServiceKey, HostServiceState*> state_info_t;
class TableStateHistory : public Table
{
    int      _query_timeframe;
    Query   *_query;
    time_t   _since;
    time_t   _until;

    // Notification periods information, name: active(1)/inactive(0)
    typedef map<string, int> _notification_periods_t;
    _notification_periods_t  _notification_periods;

    // Helper functions to traverse through logfiles
    _logfiles_t::iterator         _it_logs;
    logfile_entries_t            *_entries;
    logfile_entries_t::iterator   _it_entries;
    LogEntry                     *_current_entry;
    state_info_t                  _state_info;

protected:
    bool     _abort_query;

public:
    TableStateHistory();
    const char *name() { return "statehist"; }
    const char *prefixname() { return "statehist_"; }
    bool isAuthorized(contact *ctc, void *data);
    void answerQuery(Query *query);
    void cleanupQuery(Query *query);
    Column *column(const char *colname); // override in order to handle current_
    int updateHostServiceState(Query *query, const LogEntry *entry, HostServiceState *state, const bool only_update);
    static void addColumns(Table *);
    bool hasLogcache() { return true; };

private:
    LogEntry* getPreviousLogentry();
    LogEntry* getNextLogentry();
    void      process(Query *query, HostServiceState *hs_state);
    bool      objectFilteredOut(Query *, void *entry);
};


#endif // TableStateHistory_h
