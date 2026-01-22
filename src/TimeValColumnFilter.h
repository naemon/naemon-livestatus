// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TimeValColumnFilter_h
#define TimeValColumnFilter_h

#include "TimeValColumn.h"
#include "Filter.h"

class TimeValColumnFilter : public Filter
{
    TimeValColumn *_column;
    int _opid;
    bool _negate;
    struct timeval _ref_value;
public:
    TimeValColumnFilter(TimeValColumn *column, int opid, char *value);
    struct timeval convertRefValue(const char *value);
    bool accepts(void *data);
    int str2timeval(const char *str, struct timeval *tv);
};

#endif // TimeValColumnFilter_h
