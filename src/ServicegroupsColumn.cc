// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ServicegroupsColumn.h"
#include "nagios.h"
#include "Query.h"

objectlist *ServicegroupsColumn::getData(void *data)
{
    if (data) {
        data = shiftPointer(data);
        if (data)
            return *(objectlist **)((char *)data + _offset);
    }
    return 0;
}

void ServicegroupsColumn::output(void *data, Query *query)
{
    query->outputBeginList();
    objectlist *list = getData(data);
    if (list) {
        bool first = true;
        while (list) {
            servicegroup *sg = (servicegroup *)list->object_ptr;
            if (!first)
                query->outputListSeparator();
            else
                first = false;
            query->outputString(sg->group_name);
            list = list->next;
        }
    }
    query->outputEndList();
}

void *ServicegroupsColumn::getNagiosObject(char *name)
{
    return find_servicegroup(name);
}

bool ServicegroupsColumn::isNagiosMember(void *data, void *nagobject)
{
    // data is already shifted
    objectlist *list = *(objectlist **)((char *)data + _offset);
    while (list) {
        if (list->object_ptr == nagobject)
            return true;
        list = list->next;
    }
    return false;
}

bool ServicegroupsColumn::isEmpty(void *data)
{
    objectlist *list = *(objectlist **)((char *)data + _offset);
    return list == 0;
}
