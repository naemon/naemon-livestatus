// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef PerfdataAggregator_h
#define PerfdataAggregator_h

#include <map>
#include <string>
#include "Aggregator.h"

class StringColumn;

struct perf_aggr {
    double _aggr;
    double _count;
    double _sumq;
};

class PerfdataAggregator : public Aggregator
{
    StringColumn *_column;
    typedef std::map<std::string, perf_aggr> _aggr_t;
    _aggr_t _aggr;

public:
    PerfdataAggregator(StringColumn *c, int o) : Aggregator(o), _column(c) {}
    void consume(void *data, Query *);
    void output(Query *);

private:
    void consumeVariable(const char *varname, double value);
};

#endif // PerfdataAggregator_h
