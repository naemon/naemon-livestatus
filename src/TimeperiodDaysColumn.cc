// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "TimeperiodDaysColumn.h"
#include "OringFilter.h"
#include "nagios.h"
#include "Query.h"

void TimeperiodDaysColumn::output(void *data, Query *query) {
    timeperiod *tp;
    timerange *range;
    int day;

    data = shiftPointer(data);
    if (!data)
        return;
    tp = (timeperiod*)data;

    query->outputBeginList();
    for (day = 0; day < 7; day++) {
        if(day>0)
            query->outputListSeparator();
        range = tp->days[day];
        bool first = true;
        query->outputBeginSublist();
        while (range) {
            if(!first)
                query->outputSublistSeparator();
            first = false;
            query->outputInteger(range->range_start);
            query->outputSublistSeparator();
            query->outputInteger(range->range_end);
            range = range->next;
        }
        query->outputEndSublist();
    }
    query->outputEndList();
}

Filter *TimeperiodDaysColumn::createFilter(int opid, char *value) {
    // Nothing matches (except if negated)
    if(opid<0)
        return new AndingFilter();
    return new OringFilter();
}
