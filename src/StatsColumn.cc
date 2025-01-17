// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "StatsColumn.h"
#include "Column.h"
#include "Filter.h"
#include "CountAggregator.h"
#include "IntAggregator.h"
#include "DoubleAggregator.h"
#include "PerfdataAggregator.h"
#include "strutil.h"

StatsColumn::~StatsColumn()
{
    if (_filter)
        delete _filter;
}


Aggregator *StatsColumn::createAggregator()
{
    if (_operation == STATS_OP_COUNT)
        return new CountAggregator(_filter);
    else if (_column->type() == COLTYPE_INT || _column->type() == COLTYPE_TIME)
        return new IntAggregator((IntColumn *)_column, _operation);
    else if (_column->type() == COLTYPE_DOUBLE)
        return new DoubleAggregator((DoubleColumn *)_column, _operation);
    else if (_column->type() == COLTYPE_STRING and ends_with(_column->name(), "perf_data"))
        return new PerfdataAggregator((StringColumn *)_column, _operation);
    else  // unaggregateble column
        return new CountAggregator(_filter);
}
