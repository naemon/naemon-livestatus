// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <glib.h>
#include "HostlistStateColumn.h"
#include "ServicelistStateColumn.h"
#include "nagios.h"
#include "TableServices.h"
#include "Query.h"

extern TableServices *g_table_hosts;


inline bool hst_state_is_worse(int32_t state1, int32_t state2)
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
    int32_t result;
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

int32_t HostlistStateColumn::getValue(void *data, Query *query)
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
