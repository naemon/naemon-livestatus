// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "DownCommColumn.h"
#include "DowntimeOrComment.h"
#include "TableDownComm.h"
#include "logger.h"
#include "Query.h"
#include "tables.h"

void DownCommColumn::output(void *data, Query *query)
{
    TableDownComm *table = _is_downtime ? g_table_downtimes : g_table_comments;
    table->lock();
    query->outputBeginList();
    data = shiftPointer(data); // points to host or service
    if (data)
    {
        bool first = true;

        for (map<unsigned long, DowntimeOrComment *>::iterator it = table->entriesIteratorBegin();
                it != table->entriesIteratorEnd();
                ++it)
        {
            unsigned long id = it->first;
            DowntimeOrComment *dt = it->second;
            if ((void *)dt->_service == data ||
                    (dt->_service == 0 && dt->_host == data))
            {
                if (first)
                    first = false;
                else
                    query->outputListSeparator();
                if (_with_info)
                {
                    query->outputBeginSublist();
                    query->outputUnsignedLong(id);
                    query->outputSublistSeparator();
                    query->outputString(dt->_author_name);
                    query->outputSublistSeparator();
                    query->outputString(dt->_comment);
                    query->outputSublistSeparator();
                    query->outputTime(dt->_entry_time);
                    if(_is_downtime) {
                        Downtime *d = (Downtime*) dt;
                        query->outputSublistSeparator();
                        query->outputTime(d->_start_time);
                        query->outputSublistSeparator();
                        query->outputTime(d->_end_time);
                        query->outputSublistSeparator();
                        query->outputInteger(d->_fixed);
                        query->outputSublistSeparator();
                        query->outputInteger64(d->_duration);
                        query->outputSublistSeparator();
                        query->outputInteger(d->_triggered_by);
                    } else {
                        Comment *c = (Comment*) dt;
                        query->outputSublistSeparator();
                        query->outputInteger(c->_entry_type);
                        query->outputSublistSeparator();
                        query->outputInteger(c->_expires);
                        query->outputSublistSeparator();
                        query->outputTime(c->_expire_time);
                    }
                    query->outputEndSublist();
                }
                else
                    query->outputUnsignedLong(id);
            }
        }
    }
    table->unlock();
    query->outputEndList();
}

void *DownCommColumn::getNagiosObject(char *name)
{
    unsigned int id = strtoul(name, 0, 10);
    return (void *)(uintptr_t)id; // Hack. Convert number into pointer.
}

bool DownCommColumn::isNagiosMember(void *data, void *member)
{
    TableDownComm *table = _is_downtime ? g_table_downtimes : g_table_comments;
    // data points to a host or service
    // member is not a pointer, but an unsigned int (hack)
    int64_t id = (int64_t)(uintptr_t)member; // Hack. Convert it back.
    DowntimeOrComment *dt = table->findEntry(id);
    return dt != 0 &&
        ( dt->_service == (service *)data
          || (dt->_service == 0 && dt->_host == (host *)data));
}

bool DownCommColumn::isEmpty(void *data)
{
    if (!data) return true;

    TableDownComm *table = _is_downtime ? g_table_downtimes : g_table_comments;
    table->lock();
    for (map<unsigned long, DowntimeOrComment *>::iterator it = table->entriesIteratorBegin();
            it != table->entriesIteratorEnd();
            ++it)
    {
        DowntimeOrComment *dt = it->second;
        if ((void *)dt->_service == data ||
                (dt->_service == 0 && dt->_host == data))
        {
            table->unlock();
            return false;
        }
    }
    table->unlock();
    return true; // empty
}
