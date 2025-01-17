// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef DoubleAggregator_h
#define DoubleAggregator_h

#include "Aggregator.h"
class DoubleColumn;

class DoubleAggregator : public Aggregator
{
    DoubleColumn *_column;
    double _aggr;
    double _sumq;
public:
    DoubleAggregator(DoubleColumn *c, int o) :
        Aggregator(o), _column(c), _aggr(0), _sumq(0) {}
    void consume(void *data, Query *);
    void output(Query *);
};

#endif // DoubleAggregator_h
