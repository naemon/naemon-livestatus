// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ContactgroupsObjectlistColumn.h"
#include "nagios.h"
#include "Query.h"

objectlist *ContactgroupsObjectlistColumn::getData(void *data)
{
    if (data) {
        data = shiftPointer(data);
        if (data)
            return *(objectlist **)((char *)data + _offset);
    }
    return 0;
}

void ContactgroupsObjectlistColumn::output(void *data, Query *query)
{
    query->outputBeginList();
    objectlist *list = getData(data);
    if (list) {
        bool first = true;
        while (list) {
            contactgroup *sg = (contactgroup *)list->object_ptr;
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

void *ContactgroupsObjectlistColumn::getNagiosObject(char *name)
{
    return find_contactgroup(name);
}

bool ContactgroupsObjectlistColumn::isNagiosMember(void *data, void *nagobject)
{
    if (!nagobject || !data)
        return false;

    // data is already shifted (_indirect_offset is taken into account)
    // But _offset needs still to be accounted for
    objectlist *list = *(objectlist **)((char *)data + _offset);

    while (list) {
        if (list->object_ptr == nagobject)
            return true;
        list = list->next;
    }
    return false;
}

bool ContactgroupsObjectlistColumn::isEmpty(void *data)
{
    objectlist *list = *(objectlist **)((char *)data + _offset);
    return list == 0;
}
