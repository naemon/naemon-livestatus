// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef NegatingFilter_h
#define NegatingFilter_h

#include "config.h"
#include "Filter.h"

class NegatingFilter : public Filter
{
    Filter *_filter;
public:
    NegatingFilter(Filter *filter) : _filter(filter) {}
    ~NegatingFilter() { delete _filter; }
    bool accepts(void *data) { return !_filter->accepts(data); }
};


#endif // NegatingFilter_h
