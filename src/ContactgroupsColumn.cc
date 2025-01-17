// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ContactgroupsColumn.h"
#include "nagios.h"
#include "Query.h"

contactgroupsmember *ContactgroupsColumn::getData(void *data)
{
    if (data) {
        data = shiftPointer(data);
        if (data)
            return *(contactgroupsmember **)((char *)data + _offset);
    }
    return 0;
}

void ContactgroupsColumn::output(void *data, Query *query)
{
    query->outputBeginList();
    contactgroupsmember *cgm = getData(data);
    if (cgm) {
        bool first = true;
        while (cgm) {
            contactgroup *cg = (contactgroup *)cgm->group_ptr;
            if (!first)
                query->outputListSeparator();
            else
                first = false;
            query->outputString(cg->group_name);
            cgm = cgm->next;
        }
    }
    query->outputEndList();
}

void *ContactgroupsColumn::getNagiosObject(char *name)
{
    return find_contactgroup(name);
}

bool ContactgroupsColumn::isNagiosMember(void *data, void *nagobject)
{
    if (!nagobject || !data)
        return false;

    // data is already shifted (_indirect_offset is taken into account)
    // But _offset needs still to be accounted for
    contactgroupsmember *cgm = *(contactgroupsmember **)((char *)data + _offset);

    while (cgm) {
        if (cgm->group_ptr == nagobject)
            return true;
        cgm = cgm->next;
    }
    return false;
}

bool ContactgroupsColumn::isEmpty(void *data)
{
    contactgroupsmember *cgm = *(contactgroupsmember **)((char *)data + _offset);
    return cgm == 0;
}
