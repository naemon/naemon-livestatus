// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "OffsetStringServiceMacroColumn.h"
#include "nagios.h"

host *OffsetStringServiceMacroColumn::getHost(void *data)
{
    service *svc = getService(data);
    if (svc)
        return svc->host_ptr;
    else
        return 0;
}

service *OffsetStringServiceMacroColumn::getService(void *data)
{
    data = shiftPointer(data);
    return (service *)data;
}
