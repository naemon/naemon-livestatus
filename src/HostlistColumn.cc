// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <glib.h>
#include "HostlistColumn.h"
#include "HostlistColumnFilter.h"
#include "nagios.h"
#include "logger.h"
#include "Query.h"
#include "TableHosts.h"

extern TableHosts *g_table_hosts;

GTree *HostlistColumn::getMembers(gpointer data)
{
    data = shiftPointer(data);
    if (!data) return 0;

    return *(GTree **)((char *)data + _offset);
}

struct output_parameters {
    Query *query;
    bool first;
    bool show_state;
};

static gboolean output_host(gpointer _name, gpointer _hst, gpointer user_data)
{
    host *hst = (host *)_hst;
    output_parameters *params = (output_parameters *)user_data;
    contact *auth_user = params->query->authUser();
    if (!auth_user || g_table_hosts->isAuthorized(auth_user, hst)) {
        if (!params->first)
            params->query->outputListSeparator();
        else
            params->first = false;
        if (!params->show_state)
            params->query->outputString(hst->name);
        else {
            params->query->outputBeginSublist();
            params->query->outputString(hst->name);
            params->query->outputSublistSeparator();
            params->query->outputInteger(hst->current_state);
            params->query->outputSublistSeparator();
            params->query->outputInteger(hst->has_been_checked);
            params->query->outputEndSublist();
        }
    }
    return FALSE;
}

void HostlistColumn::output(void *data, Query *query)
{
    query->outputBeginList();
    GTree *mem = getMembers(data);
    if(mem == NULL) {
        query->outputEndList();
        return;
    }
    output_parameters params;
    params.query = query;
    params.first = true;
    params.show_state = _show_state;
    g_tree_foreach(mem, output_host, &params);
    query->outputEndList();
}

Filter *HostlistColumn::createFilter(int opid, char *value)
{
    return new HostlistColumnFilter(this, opid, value);
}
