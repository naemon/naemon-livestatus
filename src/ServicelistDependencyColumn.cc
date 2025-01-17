// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ServicelistDependencyColumn.h"
#include "ServicelistDependencyColumnFilter.h"
#include "Query.h"
#include "TableServices.h"

extern TableServices *g_table_services;

objectlist *ServicelistDependencyColumn::getList(void *data)
{
    data = shiftPointer(data);
    if (!data) return 0;

    return *(objectlist **)((char *)data + _offset);
}

void ServicelistDependencyColumn::output(void *data, Query *query)
{
    bool first = true;
    query->outputBeginList();
    objectlist *list = getList(data);
    contact *auth_user = query->authUser();
    while(list) {
        servicedependency *dependency = (servicedependency *)list->object_ptr;
        service *svc = dependency->master_service_ptr;
        if(!auth_user || g_table_services->isAuthorized(auth_user, svc)) {
            if (!first)
                query->outputListSeparator();
            else
                first = false;
            query->outputBeginSublist();
            query->outputString(svc->host_name);
            query->outputSublistSeparator();
            query->outputString(svc->description);
            if(_with_info) {
                query->outputSublistSeparator();
                query->outputInteger64(dependency->failure_options);
                query->outputSublistSeparator();
                query->outputString(dependency->dependency_period);
                query->outputSublistSeparator();
                query->outputInteger64(dependency->inherits_parent);
            }
            query->outputEndSublist();
        }
        list = list->next;
    }
    query->outputEndList();
}

Filter *ServicelistDependencyColumn::createFilter(int opid, char *value)
{
    return new ServicelistDependencyColumnFilter(this, opid, value, _with_info);
}