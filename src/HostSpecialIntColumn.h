// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef HostSpecialIntColumn_h
#define HostSpecialIntColumn_h

#include "config.h"

#include "IntColumn.h"

#define HSIC_REAL_HARD_STATE   0
#define HSIC_PNP_GRAPH_PRESENT 1
#define HSIC_SHOULD_BE_SCHEDULED 2

class HostSpecialIntColumn : public IntColumn
{
    int _type;

public:
    HostSpecialIntColumn(string name, string description, int hsic_type, int indirect)
        : IntColumn(name, description, indirect) , _type(hsic_type) {}
    int32_t getValue(void *data, Query *);
};

#endif // HostSpecialIntColumn_h
