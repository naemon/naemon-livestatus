// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "nagios.h"
#include "OffsetDoubleColumn.h"

double OffsetDoubleColumn::getValue(void *data)
{
    if (!data)
        return 0;

    char *p = (char *)shiftPointer(data);
    if (p)
        return *(double *)(p + _offset);
    else
        return 0;
}
