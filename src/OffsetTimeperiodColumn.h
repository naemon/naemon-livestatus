// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OffsetTimeperiodColumn_h
#define OffsetTimeperiodColumn_h

#include "config.h"

#include <stdlib.h>
#include <map>
#include "nagios.h"
#include "OffsetIntColumn.h"

class OffsetTimeperiodColumn : public OffsetIntColumn
{
public:
    OffsetTimeperiodColumn(string, string, int, int indirect_offset = -1);
    int32_t getValue(void *data, Query *);
};


#endif // OffsetTimeperiodColumn_h
