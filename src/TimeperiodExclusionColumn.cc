// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

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
