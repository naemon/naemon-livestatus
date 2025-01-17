// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "IntColumn.h"
#include "IntColumnFilter.h"
#include "Query.h"

void IntColumn::output(void *data, Query *query)
{
    query->outputInteger(getValue(data, query));
}

Filter *IntColumn::createFilter(int operator_id, char *value)
{
    return new IntColumnFilter(this, operator_id, value);
}


string IntColumn::valueAsString(void *data, Query *query)
{
    char i[16];
    snprintf(i, sizeof(i), "%d", getValue(data, query));
    return i;
}

int IntColumn::compare(void *dataa, void*datab, Query *query) {
    /* return a-b; would work, but isn't overflow-safe */
    int32_t a = getValue(dataa, query);
    int32_t b = getValue(datab, query);
    if( a==b ) return 0;
    return (a>b)?(1):(-1);
}
