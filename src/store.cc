// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <strings.h>
#include <map>
#include <set>

#include "nagios.h"
#include "store.h"
#include "Store.h"
#include "Query.h"
#include "InputBuffer.h"
#include "OutputBuffer.h"
#include "logger.h"
#include "TimeperiodsCache.h"

using namespace std;

Store *g_store = 0;
TimeperiodsCache *g_timeperiods_cache = 0;

/* API functions for event broker module (with C linkage) */

void store_init()
{
    g_store = new Store();
    g_timeperiods_cache = new TimeperiodsCache();
}


void store_deinit()
{
    if (g_store) {
        delete g_store;
        g_store = 0;
    }
    if (g_timeperiods_cache) {
        delete g_timeperiods_cache;
        g_timeperiods_cache = 0;
    }
}

void store_register_comment(nebstruct_comment_data *d)
{
    g_store->registerComment(d);
}

void store_register_downtime(nebstruct_downtime_data *d)
{
    g_store->registerDowntime(d);
}

int store_answer_request(void *ib, void *ob, int fd)
{
    return g_store->answerRequest((InputBuffer *)ib, (OutputBuffer *)ob, fd);
}

void *create_outputbuffer(int *termination_flag)
{
    return new OutputBuffer(termination_flag);
}

void flush_output_buffer(void *ob, int fd)
{
    ((OutputBuffer *)ob)->flush(fd);
}

void delete_outputbuffer(void *ob)
{
    delete (OutputBuffer *)ob;
}

void *create_inputbuffer(int *termination_flag)
{
    return new InputBuffer(termination_flag);
}

void set_inputbuffer_fd(void *ib, int fd)
{
    ((InputBuffer *)ib)->setFd(fd);
}

void delete_inputbuffer(void *ib)
{
    delete (InputBuffer *)ib;
}

void update_timeperiods_cache(time_t now)
{
    g_timeperiods_cache->update(now);
}
