// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TimeColumnFilter_h
#define TimeColumnFilter_h

#include "TimeColumn.h"
#include "Filter.h"

class TimeColumnFilter : public Filter
{
    TimeColumn *_column;
    int _opid;
    bool _negate;
    string _ref_string;
public:
    TimeColumnFilter(TimeColumn *column, int opid, char *value);
    time_t convertRefValue();
    bool accepts(void *data);
    void findTimeLimits(const char *columnname, time_t *lower, time_t *upper);
};

#endif // TimeColumnFilter_h
