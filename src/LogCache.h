// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef LogCache_h
#define LogCache_h

#include <map>
#include <time.h>
#include "config.h"
#include "Table.h"

class Logfile;

typedef map<time_t, Logfile *> _logfiles_t;

class LogCache
{
    pthread_mutex_t _lock;
    unsigned long   _max_cached_messages;
    unsigned long   _num_at_last_check;
    _logfiles_t     _logfiles;

public:
    LogCache(unsigned long max_cached_messages);
    ~LogCache();
    void setMaxCachedMessages(unsigned long m);
    time_t _last_index_update;

    const char *name() { return "log"; }
    const char *prefixname() { return "logs"; }
    bool isAuthorized(contact *ctc, void *data);
    void handleNewMessage(Logfile *logfile, time_t since, time_t until, unsigned logclasses);
    Column *column(const char *colname); // override in order to handle current_
    _logfiles_t *logfiles() { return &_logfiles; };
    void forgetLogfiles();
    void updateLogfileIndex();

    bool logCachePreChecks();
    void lockLogCache();
    void unlockLogCache();

private:
    void scanLogfile(char *path, bool watch);
    _logfiles_t::iterator findLogfileStartingBefore(time_t);
};

#endif // LogCache_h
