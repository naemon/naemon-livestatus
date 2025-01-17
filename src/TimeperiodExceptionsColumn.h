// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TimeperiodExceptionsColumn_h
#define TimeperiodExceptionsColumn_h

#include "config.h"

#include "Column.h"
#include "nagios.h"

class TimeperiodExceptionsColumn : public Column
{
    int _daterange;
public:
    TimeperiodExceptionsColumn(string name, string description, int indirect_offset, int daterange)
        : Column(name, description, indirect_offset), _daterange(daterange) {}
    int type() { return COLTYPE_LIST; };
    void output(void *, Query *);
    Filter *createFilter(int opid, char *value);
};



#endif // TimeperiodDaysColumn_h
