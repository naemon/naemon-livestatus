// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "StringColumn.h"
#include "StringColumnFilter.h"
#include "Query.h"
#include <string.h>

void StringColumn::output(void *data, Query *query)
{
    if (data)
        query->outputString(getValue(data));
    else
        query->outputString(0);
}

Filter *StringColumn::createFilter(int operator_id, char *value)
{
    return new StringColumnFilter(this, operator_id, value);
}


int StringColumn::compare(void *dataa, void*datab, Query *query) {
    return strcmp( getValue(dataa), getValue(datab) );
}
