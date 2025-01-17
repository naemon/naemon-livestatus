// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef CountAggregator_h
#define CountAggregator_h

#include "Aggregator.h"
#include "StatsColumn.h"

class Filter;

class CountAggregator : public Aggregator
{
    Filter *_filter;
public:
    CountAggregator(Filter *f) : Aggregator(STATS_OP_COUNT), _filter(f) {}
    void consume(void *data, Query *);
    void output(Query *);
};

#endif // CountAggregator_h
