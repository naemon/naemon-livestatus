// +------------------------------------------------------------------+
// |             ____ _               _        __  __ _  __           |
// |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
// |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
// |           | |___| | | |  __/ (__|   <    | |  | | . \            |
// |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
// |                                                                  |
// | Copyright Mathias Kettner 2012             mk@mathias-kettner.de |
// +------------------------------------------------------------------+
//
// This file is part of Check_MK.
// The official homepage is at http://mathias-kettner.de/check_mk.
//
// check_mk is free software;  you can redistribute it and/or modify it
// under the  terms of the  GNU General Public License  as published by
// the Free Software Foundation in version 2.  check_mk is  distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY;  with-
// out even the implied warranty of  MERCHANTABILITY  or  FITNESS FOR A
// PARTICULAR PURPOSE. See the  GNU General Public License for more de-
// ails.  You should have  received  a copy of the  GNU  General Public
// License along with GNU Make; see the file  COPYING.  If  not,  write
// to the Free Software Foundation, Inc., 51 Franklin St,  Fifth Floor,
// Boston, MA 02110-1301 USA.

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

bool OringFilter::optimizeBitmask(const char *columnname, uint64_t *mask)
{
    // We can only optimize, if *all* subfilters are filters for the
    // same column.
    uint64_t m = 0;

    for (_subfilters_t::iterator it = _subfilters.begin();
            it != _subfilters.end();
            ++it)
    {
        Filter *filter = *it;
        uint64_t mm = 0xffffffff;
        if (!filter->optimizeBitmask(columnname, &mm))
            return false; // wrong column
        m |= mm;
    }
    *mask &= m;
    return true;
}
