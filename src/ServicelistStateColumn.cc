// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ServicelistStateColumn.h"
#include "nagios.h"
#include "TableServices.h"
#include "Query.h"

extern TableServices *g_table_services;

// return true if state1 is worse than state2
bool ServicelistStateColumn::svcStateIsWorse(int32_t state1, int32_t state2)
{
    if (state1 == 0) return false;        // OK is worse than nothing
    else if (state2 == 0) return true;    // everything else is worse then OK
    else if (state2 == 2) return false;   // nothing is worse than CRIT
    else if (state1 == 2) return true;    // state1 is CRIT, state2 not
    else return (state1 > state2);        // both or WARN or UNKNOWN
}

servicesmember *ServicelistStateColumn::getMembers(void *data)
{
    data = shiftPointer(data);
    if (!data) return 0;

    return *(servicesmember **)((char *)data + _offset);
}

int32_t ServicelistStateColumn::getValue(int logictype, servicesmember *mem, Query *query)
{
    return query->serviceAggregates(mem).get(logictype);
}


int32_t ServicelistStateColumn::getValue(void *data, Query *query)
{
    return getValue(_logictype, getMembers(data), query);
}


// Performs one traversal calculating every value of the ServiceAggregate
// Getters then use saved of ServiceAggregate
ServiceAggregates ServicelistStateColumn::computeAggregates(servicesmember *mem, Query *query)
{
    ServiceAggregates aggregates;
    contact *auth_user = query->authUser();

    while (mem) {
        service *svc = mem->service_ptr;
        if (!auth_user || g_table_services->isAuthorized(auth_user, svc)) {
            aggregates.num++;

            if (!svc->has_been_checked) {
                aggregates.num_pending++;
            } else {
                int soft_state = svc->current_state;
                if (soft_state >= 0 && soft_state <= 3)
                    aggregates.soft[soft_state]++;
                int hard_state = svc->last_hard_state;
                if (hard_state >= 0 && hard_state <= 3)
                    aggregates.hard[hard_state]++;
            }

            if (svcStateIsWorse(svc->current_state, aggregates.worst_soft))
                aggregates.worst_soft = svc->current_state;
            if (svcStateIsWorse(svc->last_hard_state, aggregates.worst_hard))
                aggregates.worst_hard = svc->last_hard_state;
        }
        mem = mem->next;
    }

    return aggregates;
}
