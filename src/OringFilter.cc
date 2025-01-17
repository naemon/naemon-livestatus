// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "OringFilter.h"


bool OringFilter::accepts(void *data)
{
    for (_subfilters_t::iterator it = _subfilters.begin();
            it != _subfilters.end();
            ++it)
    {
        Filter *filter = *it;
        if (filter->accepts(data))
            return true;
    }
    return false;
}

bool OringFilter::optimizeBitmask(const char *columnname, uint32_t *mask)
{
    // We can only optimize, if *all* subfilters are filters for the
    // same column.
    uint32_t m = 0;

    for (_subfilters_t::iterator it = _subfilters.begin();
            it != _subfilters.end();
            ++it)
    {
        Filter *filter = *it;
        uint32_t mm = 0xffffffff;
        if (!filter->optimizeBitmask(columnname, &mm))
            return false; // wrong column
        m |= mm;
    }
    *mask &= m;
    return true;
}
