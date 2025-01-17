// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ServicelistColumn.h"
#include "ServicelistColumnFilter.h"
#include "nagios.h"
#include "Query.h"
#include "TableServices.h"

extern TableServices *g_table_services;

servicesmember *ServicelistColumn::getMembers(void *data)
{
    data = shiftPointer(data);
    if (!data) return 0;

    return *(servicesmember **)((char *)data + _offset);
}

void ServicelistColumn::output(void *data, Query *query)
{
    query->outputBeginList();
    contact *auth_user = query->authUser();
    servicesmember *mem = getMembers(data);

    bool first = true;
    while (mem) {
        service *svc = mem->service_ptr;
        if (!auth_user || g_table_services->isAuthorized(auth_user, svc)) {
            if (!first)
                query->outputListSeparator();
            else
                first = false;
            // show only service name => no sublist
            if (!_show_host && _info_depth == 0)
                query->outputString(svc->description);
            else
            {
                query->outputBeginSublist();
                if (_show_host) {
                    query->outputString(svc->host_name);
                    query->outputSublistSeparator();
                }
                query->outputString(svc->description);
                if (_info_depth >= 1) {
                    query->outputSublistSeparator();
                    query->outputInteger(svc->current_state);
                    query->outputSublistSeparator();
                    query->outputInteger(svc->has_been_checked);
                }
                if (_info_depth >= 2) {
                    query->outputSublistSeparator();
                    query->outputString(svc->plugin_output);
                }
                query->outputEndSublist();
            }
        }
        mem = mem->next;
    }
    query->outputEndList();
}

Filter *ServicelistColumn::createFilter(int opid, char *value)
{
    return new ServicelistColumnFilter(this, opid, value, _show_host);
}
