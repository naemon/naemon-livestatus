// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "HostSpecialDoubleColumn.h"
#include "nagios.h"
#include "logger.h"
#include "time.h"

extern int      interval_length;

double HostSpecialDoubleColumn::getValue(void *data)
{
    data = shiftPointer(data);
    if (!data) return 0;

    host *hst  = (host *)data;

    switch (_type) {
        case HSDC_STALENESS:
        {
            return (time(0) - hst->last_check) / ((hst->check_interval == 0 ? 1 : hst->check_interval) * interval_length);
        }
    }
    return -1; // Never reached
}
