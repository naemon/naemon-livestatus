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
    contact *auth_user = query->authUser();
    int32_t result = 0;
    int lt;

    while (mem) {
        service *svc = mem->service_ptr;
        if (!auth_user || g_table_services->isAuthorized(auth_user, svc)) {
            int lt = logictype;
            int state;
            int has_been_checked;
            if (logictype >= 60) {
                state = svc->last_hard_state;
                lt -= 64;
            }
            else
                state = svc->current_state;

            has_been_checked = svc->has_been_checked;

            switch (lt) {
                case SLSC_WORST_STATE:
                    if (svcStateIsWorse(state, result))
                        result = state;
                    break;
                case SLSC_NUM:
                    result++;
                    break;
                case SLSC_NUM_PENDING:
                    if (!has_been_checked)
                        result++;
                    break;
                default:
                    if (has_been_checked && state == lt)
                        result++;
                    break;
            }
        }
        mem = mem->next;
    }
    return result;
}


int32_t ServicelistStateColumn::getValue(void *data, Query *query)
{
    servicesmember *mem = getMembers(data);
    return getValue(_logictype, mem, query);
}
