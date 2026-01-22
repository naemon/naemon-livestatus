// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "TimeValColumn.h"
#include "TimeValColumnFilter.h"
#include "Query.h"

void TimeValColumn::output(void *data, Query *query)
{
    query->outputTimeVal(getValue(data, query));
}

Filter *TimeValColumn::createFilter(int operator_id, char *value)
{
    return new TimeValColumnFilter(this, operator_id, value);
}


string TimeValColumn::valueAsString(void *data, Query *query)
{
    char i[64];
    struct timeval value = (struct timeval)getValue(data, query);
    snprintf(i, sizeof(i), "%lu.%06lu", value.tv_sec, value.tv_usec);
    return i;
}

int TimeValColumn::compare(void *dataa, void*datab, Query *query) {
    struct timeval a = getValue(dataa, query);
    struct timeval b = getValue(datab, query);
    if( a.tv_sec==b.tv_sec && a.tv_usec==b.tv_usec ) return 0;
    return (a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_usec > b.tv_usec)) ? 1 : -1;
}
