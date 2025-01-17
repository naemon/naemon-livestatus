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
#include "HostlistStateColumn.h"
#include "ServicelistStateColumn.h"
#include "nagios.h"
#include "TableServices.h"
#include "Query.h"

extern TableServices *g_table_hosts;


inline bool hst_state_is_worse(int64_t state1, int64_t state2)
{
    if (state1 == 0) return false;        // UP is worse than nothing
    else if (state2 == 0) return true;    // everything else is worse then UP
    else if (state2 == 1) return false;   // nothing is worse than DOWN
    else if (state1 == 1) return true;    // state1 is DOWN, state2 not
    else return false;                    // both are UNREACHABLE
}

GTree *HostlistStateColumn::getMembers(gpointer data)
{
    data = shiftPointer(data);
    if (!data) return 0;

    return *(GTree **)((char *)data + _offset);
}

struct output_parameters {
    Query *query;
    int logictype;
    int64_t result;
};

static gboolean get_subvalue(gpointer _name, gpointer _hst, gpointer user_data)
{
    int state;
    host *hst = (host *)_hst;
    output_parameters *params = (output_parameters *)user_data;
    contact *auth_user = params->query->authUser();

    if (!auth_user || g_table_hosts->isAuthorized(auth_user, hst)) {
        switch (params->logictype) {
            case HLSC_NUM_SVC_PENDING:
            case HLSC_NUM_SVC_OK:
            case HLSC_NUM_SVC_WARN:
            case HLSC_NUM_SVC_CRIT:
            case HLSC_NUM_SVC_UNKNOWN:
            case HLSC_NUM_SVC:
                params->result += ServicelistStateColumn::getValue(params->logictype, hst->services, params->query);
                break;

            case HLSC_WORST_SVC_STATE:
                state = ServicelistStateColumn::getValue(params->logictype, hst->services, params->query);
                if (ServicelistStateColumn::svcStateIsWorse(state, params->result))
                    params->result = state;
                break;

            case HLSC_NUM_HST_UP:
            case HLSC_NUM_HST_DOWN:
            case HLSC_NUM_HST_UNREACH:
                if (hst->has_been_checked && hst->current_state == params->logictype - HLSC_NUM_HST_UP)
                    params->result ++;
                break;

            case HLSC_NUM_HST_PENDING:
                if (!hst->has_been_checked)
                    params->result ++;
                break;

            case HLSC_NUM_HST:
                params->result ++;
                break;

            case HLSC_WORST_HST_STATE:
                if (hst_state_is_worse(hst->current_state, params->result))
                    params->result = hst->current_state;
                break;
        }
    }
    return FALSE;
}

int64_t HostlistStateColumn::getValue(void *data, Query *query)
{
    GTree *mem = getMembers(data);
    output_parameters params;
    params.query = query;
    params.logictype = _logictype;
    params.result = 0;
    if(mem == NULL)
        return params.result;
    g_tree_foreach(mem, get_subvalue, &params);
    return params.result;
}
