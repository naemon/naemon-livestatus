// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#define EXTERN /* */
#include "global_counters.h"
#include "time.h"

time_t last_statistics_update = 0;
#define STATISTICS_INTERVAL    5
#define RATING_WEIGHT          0.25

void do_statistics()
{
    if (last_statistics_update == 0) {
        last_statistics_update = time(0);
        unsigned i;
        for (i=0; i<NUM_COUNTERS; i++) {
            g_counters[i] = 0;
            g_last_counter[i] = 0;
            g_counter_rate[i] = 0.0;
        }
        return;
    }
    time_t now = time(0);
    time_t delta_time = now - last_statistics_update;
    if (delta_time >= STATISTICS_INTERVAL)
    {
        last_statistics_update = now;
        unsigned i;
        for (i=0; i<NUM_COUNTERS; i++)
        {
            counter_t delta_value = g_counters[i] - g_last_counter[i];
            double new_rate = (double)delta_value / (double)delta_time;
            double old_rate = g_counter_rate[i];
            double avg_rate;
            if (old_rate == 0)
                avg_rate = new_rate;
            else
                avg_rate = old_rate * (1.0 - RATING_WEIGHT) + new_rate * RATING_WEIGHT;
            g_counter_rate[i] = avg_rate;
            g_last_counter[i] = g_counters[i];
        }
    }
}
