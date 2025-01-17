// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef _TimeperiodsCache_h
#define _TimeperiodsCache_h

#include <map>
#include "nagios.h"

class TimeperiodsCache
{
    time_t _cache_time;
    typedef std::map<timeperiod *, bool> _cache_t;
    _cache_t _cache;
    pthread_mutex_t _cache_lock;

public:
    TimeperiodsCache();
    ~TimeperiodsCache();
    void update(time_t now);
    bool inTimeperiod(timeperiod *tp);
private:
    void logTransition(char *name, int from, int to);
};

#endif // _TimeperiodsCache_h
