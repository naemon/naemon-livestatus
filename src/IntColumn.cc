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
// Updated 2012 by Max Sikström - op5: Added compare interface
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

#include "IntColumn.h"
#include "IntColumnFilter.h"
#include "Query.h"

void IntColumn::output(void *data, Query *query)
{
    query->outputInteger64(getValue(data, query));
}

Filter *IntColumn::createFilter(int operator_id, char *value)
{
    return new IntColumnFilter(this, operator_id, value);
}


string IntColumn::valueAsString(void *data, Query *query)
{
    char i[16];
    snprintf(i, sizeof(i), "%d", getValue(data, query));
    return i;
}

int IntColumn::compare(void *dataa, void*datab, Query *query) {
    /* return a-b; would work, but isn't overflow-safe */
    int64_t a = getValue(dataa, query);
    int64_t b = getValue(datab, query);
    if( a==b ) return 0;
    return (a>b)?(1):(-1);
}
