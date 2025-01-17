// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "OffsetStringColumn.h"
#include "logger.h"

const char *OffsetStringColumn::getValue(void *data)
{
    if (!data)
        return (char *)"";

    char *p = (char *)shiftPointer(data);
    if (p) {
        const char *s = *(char **)(p + _offset);
        if (s)
            return (char *)s;
        else
            return (char *)"";
    }
    else
        return (char *)"";
}
