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

// A host-state aggregate column (as opposed to a hostgroup service column, which keeps the
// per-host ServicelistStateColumn walk and is served from #145's per-host service cache).
static bool isHostStateLogicType(int logictype)
{
    switch (logictype) {
    case HLSC_NUM_HST:
    case HLSC_NUM_HST_PENDING:
    case HLSC_NUM_HST_UP:
    case HLSC_NUM_HST_DOWN:
    case HLSC_NUM_HST_UNREACH:
    case HLSC_WORST_HST_STATE:
        return true;
    default:
        return false;
    }
}

struct host_aggregate_ctx {
    contact *auth_user;
    HostAggregates *aggregates;
};

// One traversal over a host list (hostgroup) computing every host-state aggregate.
static gboolean accumulate_host(gpointer _name, gpointer _hst, gpointer user_data)
{
    host *hst = (host *)_hst;
    host_aggregate_ctx *ctx = (host_aggregate_ctx *)user_data;

    if (ctx->auth_user && !g_table_hosts->isAuthorized(ctx->auth_user, hst))
        return FALSE;

    HostAggregates &a = *ctx->aggregates;
    a.num++;
    if (!hst->has_been_checked) {
        a.num_pending++;
    } else {
        switch (hst->current_state) {
            case 0: a.up++;      break;
            case 1: a.down++;    break;
            case 2: a.unreach++; break;
        }
    }
    if (hst_state_is_worse(hst->current_state, a.worst_host))
        a.worst_host = hst->current_state;
    return FALSE;
}

HostAggregates HostlistStateColumn::computeAggregates(GTree *members, Query *query)
{
    HostAggregates aggregates;
    if (members == NULL)
        return aggregates;

    host_aggregate_ctx ctx;
    ctx.auth_user = query->authUser();
    ctx.aggregates = &aggregates;
    g_tree_foreach(members, accumulate_host, &ctx);
    return aggregates;
}

int32_t HostlistStateColumn::getValue(void *data, Query *query)
{
    // Host-state columns are cached per host list so a query asking for several of them
    // walks the host tree once instead of once per column. Service columns keep the
    // per-host walk (served from #145's per-host service cache).
    if (isHostStateLogicType(_logictype))
        return query->hostAggregates(getMembers(data)).get(_logictype);

    GTree *mem = getMembers(data);
    output_parameters params;
    params.query = query;
    params.logictype = _logictype;
    params.result = 0;
    if (mem == NULL)
        return params.result;
    g_tree_foreach(mem, get_subvalue, &params);
    return params.result;
}
