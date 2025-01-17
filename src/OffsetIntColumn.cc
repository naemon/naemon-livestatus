// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "nagios.h"
#include <stdint.h>
#include "OffsetIntColumn.h"

int32_t OffsetIntColumn::getValue(void *data, Query *)
{
    if (!data)
        return 0;

    char *p = (char *)shiftPointer(data);
    if (p)
        return *(int *)(p + _offset);
    else
        return 0;
}
