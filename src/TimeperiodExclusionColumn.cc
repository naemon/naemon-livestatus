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

#include "TimeperiodExclusionColumn.h"
#include "nagios.h"
#include "Query.h"
#include "TableServices.h"

timeperiodexclusion *TimeperiodExclusionColumn::getData(void *data)
{
    data = shiftPointer(data);
    if (!data) return 0;

    return *(timeperiodexclusion **)((char *)data + _offset);
}

void TimeperiodExclusionColumn::output(void *data, Query *query)
{
    query->outputBeginList();
    timeperiodexclusion *mem = getData(data);

    bool first = true;
    while (mem) {
        timeperiod *tp = mem->timeperiod_ptr;
        if (!first)
            query->outputListSeparator();
        else
            first = false;

        query->outputString(tp->name);
        mem = mem->next;
    }
    query->outputEndList();
}

void *TimeperiodExclusionColumn::getNagiosObject(char *name)
{
    return find_timeperiod(name);
}

bool TimeperiodExclusionColumn::isNagiosMember(void *data, void *nagobject)
{
    if (!nagobject || !data)
        return false;

    // data is already shifted (_indirect_offset is taken into account)
    // But _offset needs still to be accounted for
    timeperiodexclusion *list = *(timeperiodexclusion **)((char *)data + _offset);

    while (list) {
        if (list->timeperiod_ptr == nagobject)
            return true;
        list = list->next;
    }
    return false;
}

bool TimeperiodExclusionColumn::isEmpty(void *data)
{
    timeperiodexclusion *list = *(timeperiodexclusion **)((char *)data + _offset);
    return list == 0;
}
