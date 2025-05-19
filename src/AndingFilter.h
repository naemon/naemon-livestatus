// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef AndingFilter_h
#define AndingFilter_h

#include "config.h"

#include "Filter.h"
#include <deque>

class AndingFilter : public Filter
{
protected:
    typedef deque<Filter *> _subfilters_t;
    _subfilters_t _subfilters;
public:
    AndingFilter() {}
    ~AndingFilter();
    void addSubfilter(Filter *);
    Filter *stealLastSubfilter();
    bool accepts(void *data);
    void combineFilters(int count, int andor);
    unsigned numFilters() { return _subfilters.size(); }
    _subfilters_t::iterator begin() { return _subfilters.begin(); }
    _subfilters_t::iterator end() { return _subfilters.end(); }
    void *findIndexFilter(const char *columnname);
    void findTimeLimits(const char *columnname, time_t *lower, time_t *upper);
    bool optimizeBitmask(const char *columnname, uint32_t *mask);
};


#endif // AndingFilter_h
