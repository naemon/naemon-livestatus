// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "EmptyColumn.h"
#include "EmptyFilter.h"
#include "Query.h"

void EmptyColumn::output(void *data __attribute__ ((__unused__)), Query *query)
{
    query->outputString("");
}

Filter *EmptyColumn::createFilter(int operator_id, char *value)
{
    return new EmptyFilter(this, operator_id, value);
}
