// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef GlobalCountersColumn_h
#define GlobalCountersColumn_h

#include "config.h"

#include "Column.h"
#include "global_counters.h"

class GlobalCountersColumn : public Column
{
    unsigned _counter_index;
    bool _do_average;

public:
    GlobalCountersColumn(string name, string description, unsigned counter_index, bool do_average)
        : Column(name, description, -1), _counter_index(counter_index), _do_average(do_average) {}
    int type() { return _do_average ? COLTYPE_DOUBLE : COLTYPE_INT; }
    void output(void *, Query *);
    Filter *createFilter(int operator_id __attribute__ ((__unused__)), char *value __attribute__ ((__unused__))) { return 0; }
};


#endif // GlobalCountersColumn_h
