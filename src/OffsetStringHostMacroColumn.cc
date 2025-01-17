// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "OffsetStringHostMacroColumn.h"
#include "nagios.h"

host *OffsetStringHostMacroColumn::getHost(void *data)
{
    data = shiftPointer(data);
    return (host *)data;
}

service *OffsetStringHostMacroColumn::getService(void *data __attribute__ ((__unused__)))
{
    return 0;
}
