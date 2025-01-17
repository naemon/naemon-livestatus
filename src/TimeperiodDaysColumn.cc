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

#include "TimeperiodDaysColumn.h"
#include "OringFilter.h"
#include "nagios.h"
#include "Query.h"

void TimeperiodDaysColumn::output(void *data, Query *query) {
    timeperiod *tp;
    timerange *range;
    int day;

    data = shiftPointer(data);
    if (!data)
        return;
    tp = (timeperiod*)data;

    query->outputBeginList();
    for (day = 0; day < 7; day++) {
        if(day>0)
            query->outputListSeparator();
        range = tp->days[day];
        bool first = true;
        query->outputBeginSublist();
        while (range) {
            if(!first)
                query->outputSublistSeparator();
            first = false;
            query->outputInteger64(range->range_start);
            query->outputSublistSeparator();
            query->outputInteger64(range->range_end);
            range = range->next;
        }
        query->outputEndSublist();
    }
    query->outputEndList();
}

Filter *TimeperiodDaysColumn::createFilter(int opid, char *value) {
    // Nothing matches (except if negated)
    if(opid<0)
        return new AndingFilter();
    return new OringFilter();
}
