// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "HostSpecialIntColumn.h"
#include "nagios.h"
#include "pnp4nagios.h"

int32_t HostSpecialIntColumn::getValue(void *data, Query *)
{
    data = shiftPointer(data);
    if (!data) return 0;

    host *hst = (host *)data;
    switch (_type) {
        case HSIC_REAL_HARD_STATE:
            if (hst->current_state == 0)
                return 0;
            else if (hst->state_type == 1)
                return hst->current_state; // we have reached a hard state
            else
                return hst->last_hard_state;
        case HSIC_PNP_GRAPH_PRESENT:
            return pnpgraph_present(hst->name, 0);
        case HSIC_SHOULD_BE_SCHEDULED:
            return hst->check_interval > 0 && hst->checks_enabled;
    }
    return -1; // never reached
}
