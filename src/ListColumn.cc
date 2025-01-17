// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ListColumn.h"
#include "ListColumnFilter.h"


Filter *ListColumn::createFilter(int opid, char *value)
{
    return new ListColumnFilter(this, opid, value);
}
