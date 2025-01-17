// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <time.h>
#include <pthread.h>
#include <syslog.h>

#include "TimeperiodsCache.h"
#include "nagios.h"
#include "logger.h"

TimeperiodsCache::TimeperiodsCache()
{
    pthread_mutex_init(&_cache_lock, 0);
    _cache_time = 0;
}


TimeperiodsCache::~TimeperiodsCache()
{
    pthread_mutex_destroy(&_cache_lock);
}


void TimeperiodsCache::update(time_t now)
{
    pthread_mutex_lock(&_cache_lock);

    // update cache only once a minute. The timeperiod
    // definitions have 1 minute as granularity, so a
    // 1sec resultion is not needed.
    int minutes = now / 60;
    if (minutes == _cache_time) {
        pthread_mutex_unlock(&_cache_lock);
        return;
    }

    // Loop over all timeperiods and compute if we are
    // currently in. Detect the case where no time periods
    // are known (yet!). This might be the case when a timed
    // event broker message arrives *before* the start of the
    // event loop.
    timeperiod *tp = timeperiod_list;
    int num_periods = 0;
    while (tp) {
        bool is_in = 0 == check_time_against_period(now, tp);

        // check previous state and log transition if state has changed
        _cache_t::iterator it = _cache.find(tp);
        if (it == _cache.end()) { // first entry
            logTransition(tp->name, -1, is_in ? 1 : 0);
            _cache.insert(std::make_pair(tp, is_in));
        }
        else if (it->second != is_in) {
            logTransition(tp->name, it->second ? 1 : 0, is_in ? 1 : 0);
            it->second = is_in;
        }

        tp = tp->next;
        num_periods++;
    }
    if (num_periods > 0)
        _cache_time = minutes;
    else
        logger(LG_INFO, "Timeperiod cache not updated, there are no timeperiods (yet)");

    pthread_mutex_unlock(&_cache_lock);
}


bool TimeperiodsCache::inTimeperiod(timeperiod *tp)
{
    bool is_in;
    pthread_mutex_lock(&_cache_lock);
    _cache_t::iterator it = _cache.find(tp);
    if (it != _cache.end())
        is_in = it->second;
    else {
        logger(LG_INFO, "No timeperiod information available for %s. Assuming out of period.", tp->name);
        is_in = false;
        // Problem: The method check_time_against_period is to a high
        // degree not thread safe. In the current situation Icinga is
        // very probable to hang up forever.
        // time_t now = time(0);
        // is_in = 0 == check_time_against_period(now, tp);
    }
    pthread_mutex_unlock(&_cache_lock);
    return is_in;
}


void TimeperiodsCache::logTransition(char *name, int from, int to)
{
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "TIMEPERIOD TRANSITION: %s;%d;%d", name, from, to);
    nm_log(NSLOG_INFO_MESSAGE, "%s", buffer);
}
