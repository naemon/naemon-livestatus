// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "waittriggers.h"

const char *wt_names[WT_NUM_TRIGGERS] =
{
    "all",
    "check",
    "state",
    "log",
    "downtime",
    "comment",
    "command",
    "program",
};

pthread_cond_t g_wait_cond[] = {
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
};

pthread_mutex_t g_wait_mutex = PTHREAD_MUTEX_INITIALIZER;
