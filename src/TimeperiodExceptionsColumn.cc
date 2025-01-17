// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "TimeperiodExceptionsColumn.h"
#include "OringFilter.h"
#include "nagios.h"
#include "Query.h"

#define OUTPUT_DICT_ENTRY(_Q, _O, _F) do { \
        (_Q)->outputString(#_F); \
        (_Q)->outputDictValueSeparator(); \
        (_Q)->outputInteger((_O)->_F); \
        (_Q)->outputDictSeparator(); \
    } while(0)

void TimeperiodExceptionsColumn::output(void *data, Query *query) {
    timeperiod *tp;

    daterange *dr;
    bool first_dr;

    timerange *tr;
    bool first_tr;

    data = shiftPointer(data);
    if (!data)
        return;
    tp = (timeperiod*) data;

    query->outputBeginList();
    first_dr = true;
    for (dr = tp->exceptions[_daterange]; dr; dr = dr->next) {
        if (!first_dr)
            query->outputListSeparator();
        first_dr = false;

        query->outputBeginDict();

        /* Output of variables */
        switch(_daterange) {
        case DATERANGE_CALENDAR_DATE:
            OUTPUT_DICT_ENTRY(query, dr, syear);
            OUTPUT_DICT_ENTRY(query, dr, smon);
            OUTPUT_DICT_ENTRY(query, dr, smday);
            OUTPUT_DICT_ENTRY(query, dr, eyear);
            OUTPUT_DICT_ENTRY(query, dr, emon);
            OUTPUT_DICT_ENTRY(query, dr, emday);
            break;
        case DATERANGE_MONTH_DATE:
            OUTPUT_DICT_ENTRY(query, dr, smon);
            OUTPUT_DICT_ENTRY(query, dr, smday);
            OUTPUT_DICT_ENTRY(query, dr, emon);
            OUTPUT_DICT_ENTRY(query, dr, emday);
            break;
        case DATERANGE_MONTH_DAY:
            OUTPUT_DICT_ENTRY(query, dr, smday);
            OUTPUT_DICT_ENTRY(query, dr, emday);
            break;
        case DATERANGE_MONTH_WEEK_DAY:
            OUTPUT_DICT_ENTRY(query, dr, smon);
            OUTPUT_DICT_ENTRY(query, dr, swday);
            OUTPUT_DICT_ENTRY(query, dr, swday_offset);
            OUTPUT_DICT_ENTRY(query, dr, emon);
            OUTPUT_DICT_ENTRY(query, dr, ewday);
            OUTPUT_DICT_ENTRY(query, dr, ewday_offset);
            break;
        case DATERANGE_WEEK_DAY:
            OUTPUT_DICT_ENTRY(query, dr, swday);
            OUTPUT_DICT_ENTRY(query, dr, swday_offset);
            OUTPUT_DICT_ENTRY(query, dr, ewday);
            OUTPUT_DICT_ENTRY(query, dr, ewday_offset);
            break;
        }
        OUTPUT_DICT_ENTRY(query, dr, skip_interval);

        /* Output time ranges */
        query->outputString("times");
        query->outputDictValueSeparator();
        first_tr = true;
        query->outputBeginSublist();
        for (tr = dr->times; tr; tr = tr->next) {
            if (!first_tr)
                query->outputSublistSeparator();
            first_tr = false;

            query->outputInteger(tr->range_start);
            query->outputSublistSeparator();
            query->outputInteger(tr->range_end);
        }
        query->outputEndSublist();

        query->outputEndDict();
    }
    query->outputEndList();
}

Filter *TimeperiodExceptionsColumn::createFilter(int opid, char *value) {
    // Nothing matches (except if negated)
    if (opid < 0)
        return new AndingFilter();
    return new OringFilter();
}
