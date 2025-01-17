// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "nagios.h"
#include <stdint.h>
#include "OffsetTimeperiodColumn.h"
#include "logger.h"
#include "TimeperiodsCache.h"

extern TimeperiodsCache *g_timeperiods_cache;


    OffsetTimeperiodColumn::OffsetTimeperiodColumn(string name, string description, int offset, int indirect_offset)
: OffsetIntColumn(name, description, offset, indirect_offset)
{
}


int32_t OffsetTimeperiodColumn::getValue(void *data, Query *)
{
    data = shiftPointer(data);
    if (!data)
        return 0;

    timeperiod *tp;
    if (offset() == -1)
        tp = (timeperiod *)data;
    else
        tp = *(timeperiod **)((char *)data + offset());


    if (!tp)
        return 1; // no timeperiod set -> Nagios assumes 7x24
    else if (g_timeperiods_cache->inTimeperiod(tp))
        return 1;
    else
        return 0;
}
