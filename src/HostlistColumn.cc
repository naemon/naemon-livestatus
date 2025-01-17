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
            params->query->outputInteger64(hst->current_state);
            params->query->outputSublistSeparator();
            params->query->outputInteger64(hst->has_been_checked);
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
