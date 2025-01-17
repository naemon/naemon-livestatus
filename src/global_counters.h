// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef global_counters_h
#define global_counters_h

#include "config.h"

#include <stdint.h>

#ifndef EXTERN
# define EXTERN extern
#endif

typedef uint64_t counter_t;

#define COUNTER_NEB_CALLBACKS        0
#define COUNTER_REQUESTS             1
#define COUNTER_CONNECTIONS          2
#define COUNTER_SERVICE_CHECKS       3
#define COUNTER_HOST_CHECKS          4
#define COUNTER_FORKS                5
#define COUNTER_LOG_MESSAGES         6
#define COUNTER_COMMANDS             7
#define COUNTER_LIVECHECKS           8
#define COUNTER_LIVECHECK_OVERFLOWS  9
#define NUM_COUNTERS                10

EXTERN counter_t g_counters[NUM_COUNTERS];
EXTERN counter_t g_last_counter[NUM_COUNTERS];
EXTERN double g_counter_rate[NUM_COUNTERS];

void do_statistics();

#endif // global_counters_h
