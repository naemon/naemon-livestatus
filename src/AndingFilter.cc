// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "AndingFilter.h"
#include "OringFilter.h"
#include "logger.h"
#include "Query.h"

AndingFilter::~AndingFilter()
{
    for (_subfilters_t::iterator it = _subfilters.begin();
            it != _subfilters.end();
            ++it)
    {
        delete *it;
    }
}

void AndingFilter::addSubfilter(Filter *f)
{
    _subfilters.push_back(f);
}


Filter *AndingFilter::stealLastSubfiler()
{
    if (_subfilters.size() == 0)
        return 0;
    else {
        Filter *l = _subfilters.back();
        _subfilters.pop_back();
        return l;
    }
}


bool AndingFilter::accepts(void *data)
{
    for (_subfilters_t::iterator it = _subfilters.begin();
            it != _subfilters.end();
            ++it)
    {
        Filter *filter = *it;
        if (!filter->accepts(data))
            return false;
    }
    return true;
}

void *AndingFilter::findIndexFilter(const char *columnname)
{
    for (_subfilters_t::iterator it = _subfilters.begin();
            it != _subfilters.end();
            ++it)
    {
        Filter *filter = *it;
        void *refvalue = filter->indexFilter(columnname);
        if (refvalue)
            return refvalue;
    }
    return 0;
}

void AndingFilter::findTimeLimits(const char *columnname, time_t *lower, time_t *upper)
{
    for (_subfilters_t::iterator it = _subfilters.begin();
            it != _subfilters.end();
            ++it)
    {
        Filter *filter = *it;
        filter->findTimeLimits(columnname, lower, upper);
    }
}

bool AndingFilter::optimizeBitmask(const char *columnname, uint32_t *mask)
{
    bool optimized = false;
    for (_subfilters_t::iterator it = _subfilters.begin();
            it != _subfilters.end();
            ++it)
    {
        Filter *filter = *it;
        if (filter->optimizeBitmask(columnname, mask))
            optimized = true;
    }
    return optimized;
}

void AndingFilter::combineFilters(int count, int andor)
{
    if (count > (int)_subfilters.size()) {
        logger(LG_INFO, "Cannot combine %d filters with '%s': only %d are on stack",
                count, andor == ANDOR_AND ? "AND" : "OR", _subfilters.size());
        this->setError(400, "Cannot combine %d filters with '%s': only %d are on stack",
                count, andor == ANDOR_AND ? "AND" : "OR", _subfilters.size());
        return;
    }

    AndingFilter *andorfilter; // OringFilter is subclassed from AndingFilter
    if (andor == ANDOR_AND)
        andorfilter = new AndingFilter();
    else
        andorfilter = new OringFilter();
    while (count--) {
        andorfilter->addSubfilter(_subfilters.back());
        _subfilters.pop_back();
    }
    addSubfilter(andorfilter);
}
