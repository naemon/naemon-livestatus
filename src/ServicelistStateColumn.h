// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ServicelistStateColumn_h
#define ServicelistStateColumn_h

#include "config.h"

#include "IntColumn.h"
#include "nagios.h"

#define SLSC_NUM_OK             0
#define SLSC_NUM_WARN           1
#define SLSC_NUM_CRIT           2
#define SLSC_NUM_UNKNOWN        3
#define SLSC_NUM_PENDING        4
#define SLSC_WORST_STATE       -2

#define SLSC_NUM_HARD_OK       ( 0 + 64)
#define SLSC_NUM_HARD_WARN     ( 1 + 64)
#define SLSC_NUM_HARD_CRIT     ( 2 + 64)
#define SLSC_NUM_HARD_UNKNOWN  ( 3 + 64)
#define SLSC_WORST_HARD_STATE  (-2 + 64)

#define SLSC_NUM               -1


class ServicelistStateColumn : public IntColumn
{
    int _offset;
    int _logictype;

public:
    ServicelistStateColumn(string name, string description, int logictype, int offset, int indirect_offset)
        : IntColumn(name, description, indirect_offset), _offset(offset), _logictype(logictype) {}
    int32_t getValue(void *data, Query *);
    servicesmember *getMembers(void *data);
    static int32_t getValue(int logictype, servicesmember *services, Query *);
    static bool svcStateIsWorse(int32_t state1, int32_t state2);
};


#endif // ServicelistStateColumn_h
