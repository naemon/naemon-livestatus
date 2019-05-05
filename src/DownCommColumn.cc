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
