// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "TimeColumn.h"
#include "TimeColumnFilter.h"
#include "IntFormat.h"
#include "Query.h"

void TimeColumn::output(void *data, Query *query)
{
    query->outputTime(getValue(data, query));
}

Filter *TimeColumn::createFilter(int operator_id, char *value)
{
    return new TimeColumnFilter(this, operator_id, value);
}


string TimeColumn::valueAsString(void *data, Query *query)
{
    char buf[24];
    char *end = buf + sizeof(buf);
    char *p = formatIntegerBackward(end, (int64_t)getValue(data, query));
    return string(p, (size_t)(end - p));
}

int TimeColumn::compare(void *dataa, void*datab, Query *query) {
    /* return a-b; would work, but isn't overflow-safe */
    time_t a = getValue(dataa, query);
    time_t b = getValue(datab, query);
    if( a==b ) return 0;
    return (a>b)?(1):(-1);
}
