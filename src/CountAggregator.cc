// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "CountAggregator.h"
#include "Query.h"
#include "Filter.h"

void CountAggregator::consume(void *data, Query *)
{
    // _filter is 0 --> no filter, accept all data
    if (!_filter || _filter->accepts(data))
        _count++;
}

void CountAggregator::output(Query *q)
{
    q->outputInteger(_count);
}
