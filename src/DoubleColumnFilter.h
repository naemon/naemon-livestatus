// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef DoubleColumnFilter_h
#define DoubleColumnFilter_h

#include "config.h"

#include <string>
using namespace std;

#include "Filter.h"
class DoubleColumn;

class DoubleColumnFilter : public Filter
{
    DoubleColumn *_column;
    double _ref_value;
    int _opid;
    bool _negate;

public:
    DoubleColumnFilter(DoubleColumn *_column, int opid, char *value);
    bool accepts(void *data);
};


#endif // DoubleColumnFilter_h
