// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ServiceSpecialIntColumn.h"
#include "nagios.h"
#include "pnp4nagios.h"

int32_t ServiceSpecialIntColumn::getValue(void *data, Query *)
{
    data = shiftPointer(data);
    if (!data) return 0;

    service *svc = (service *)data;
    switch (_type) {
        case SSIC_PNP_GRAPH_PRESENT:
            return pnpgraph_present(svc->host_ptr->name, svc->description);
        case SSIC_SHOULD_BE_SCHEDULED:
            return svc->check_interval > 0 && svc->checks_enabled;
    }
    return -1; // never reached
}
