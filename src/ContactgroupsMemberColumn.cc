// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "nagios.h"
#include "ContactgroupsMemberColumn.h"
#include "logger.h"

bool ContactgroupsMemberColumn::isNagiosMember(void *cg, void *ctc)
{
    contactsmember *mem;
    for (mem = ((contactgroup *)cg)->members; mem != NULL; mem = mem->next)
    {
        if (mem->contact_ptr == ctc)
            return true;
    }
    return false;
}
