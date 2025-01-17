// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "CommandsMemberColumn.h"
#include "nagios.h"
#include "Query.h"

commandsmember *CommandsMemberColumn::getData(void *data)
{
    if (data) {
        data = shiftPointer(data);
        if (data)
            return *(commandsmember **)((char *)data + _offset);
    }
    return 0;
}

void CommandsMemberColumn::output(void *data, Query *query)
{
    query->outputBeginList();
    commandsmember *list = getData(data);
    if (list) {
        bool first = true;
        while (list) {
            command *co = (command *)list->command_ptr;
            if (!first)
                query->outputListSeparator();
            else
                first = false;
            query->outputString(co->name);
            list = list->next;
        }
    }
    query->outputEndList();
}

void *CommandsMemberColumn::getNagiosObject(char *name)
{
    return find_command(name);
}

bool CommandsMemberColumn::isNagiosMember(void *data, void *nagobject)
{
    if (!nagobject || !data)
        return false;

    // data is already shifted (_indirect_offset is taken into account)
    // But _offset needs still to be accounted for
    commandsmember *list = *(commandsmember **)((char *)data + _offset);

    while (list) {
        if (list->command_ptr == nagobject)
            return true;
        list = list->next;
    }
    return false;
}

bool CommandsMemberColumn::isEmpty(void *data)
{
    commandsmember *list = *(commandsmember **)((char *)data + _offset);
    return list == 0;
}
