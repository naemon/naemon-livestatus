// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "DoubleColumn.h"
#include "DoubleColumnFilter.h"
#include "Query.h"

void DoubleColumn::output(void *data, Query *query)
{
    query->outputDouble(getValue(data));
}

Filter *DoubleColumn::createFilter(int operator_id, char *value)
{
    return new DoubleColumnFilter(this, operator_id, value);
}

string DoubleColumn::valueAsString(void *data, Query *query)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%.10e", getValue(data));
    return buf;
}

int DoubleColumn::compare(void *dataa, void *datab, Query *query)
{
    double a = getValue(dataa);
    double b = getValue(datab);
    if( a>b ) return 1;
    if( a<b ) return -1;
    return 0;
}
