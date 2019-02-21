#include "HostlistDependencyColumn.h"
#include "HostlistDependencyColumnFilter.h"
#include "Query.h"
#include "TableHosts.h"

extern TableHosts *g_table_hosts;

objectlist *HostlistDependencyColumn::getList(void *data)
{
    data = shiftPointer(data);
    if (!data) return 0;

    return *(objectlist **)((char *)data + _offset);
}

void HostlistDependencyColumn::output(void *data, Query *query)
{
    bool first = true;
    query->outputBeginList();
    objectlist *list = getList(data);
    contact *auth_user = query->authUser();
    while(list) {
        hostdependency *dependency = (hostdependency *)list->object_ptr;
        host *hst = dependency->master_host_ptr;
        if(!auth_user || g_table_hosts->isAuthorized(auth_user, hst)) {
            if (!first)
                query->outputListSeparator();
            else
                first = false;
            if(_with_info)
                query->outputBeginSublist();
            query->outputString(hst->name);
            if(_with_info) {
                query->outputSublistSeparator();
                query->outputInteger64(dependency->failure_options);
                query->outputSublistSeparator();
                query->outputString(dependency->dependency_period);
                query->outputSublistSeparator();
                query->outputInteger64(dependency->inherits_parent);
                query->outputEndSublist();
            }
        }
        list = list->next;
    }
    query->outputEndList();
}

Filter *HostlistDependencyColumn::createFilter(int opid, char *value)
{
    return new HostlistDependencyColumnFilter(this, opid, value, _with_info);
}