// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ServiceSpecialIntColumn_h
#define ServiceSpecialIntColumn_h

#include "config.h"
#include "IntColumn.h"

#define SSIC_PNP_GRAPH_PRESENT 1
#define SSIC_SHOULD_BE_SCHEDULED 2

class ServiceSpecialIntColumn : public IntColumn
{
    int _type;

public:
    ServiceSpecialIntColumn(string name, string description, int ssic_type, int indirect)
        : IntColumn(name, description, indirect) , _type(ssic_type) {}
    int32_t getValue(void *data, Query *);
};

#endif // ServiceSpecialIntColumn_h
