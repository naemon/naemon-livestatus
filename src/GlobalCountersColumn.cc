// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "GlobalCountersColumn.h"
#include "Query.h"

void GlobalCountersColumn::output(void *, Query *query)
{
    if (_do_average)
        query->outputDouble(g_counter_rate[_counter_index]);
    else
        query->outputCounter(g_counters[_counter_index]);
}
