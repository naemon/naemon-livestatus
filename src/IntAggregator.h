// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef IntAggregator_h
#define IntAggregator_h

#include "Aggregator.h"
class IntColumn;

class IntAggregator : public Aggregator
{
    IntColumn *_column;
    int64_t _aggr;
    double _sumq;
public:
    IntAggregator(IntColumn *c, int o) :
        Aggregator(o), _column(c), _aggr(0), _sumq(0) {}
    void consume(void *data, Query *);
    void output(Query *);
};

#endif // IntAggregator_h
