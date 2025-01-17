// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef HostlistStateColumn_h
#define HostlistStateColumn_h

#include <glib.h>
#include "config.h"

#include "IntColumn.h"
#include "ServicelistStateColumn.h"
#include "nagios.h"

#define HLSC_NUM_SVC               SLSC_NUM
#define HLSC_NUM_SVC_PENDING       SLSC_NUM_PENDING
#define HLSC_NUM_SVC_OK            SLSC_NUM_OK
#define HLSC_NUM_SVC_WARN          SLSC_NUM_WARN
#define HLSC_NUM_SVC_CRIT          SLSC_NUM_CRIT
#define HLSC_NUM_SVC_UNKNOWN       SLSC_NUM_UNKNOWN
#define HLSC_WORST_SVC_STATE       SLSC_WORST_STATE
#define HLSC_NUM_SVC_HARD_OK       SLSC_NUM_HARD_OK
#define HLSC_NUM_SVC_HARD_WARN     SLSC_NUM_HARD_WARN
#define HLSC_NUM_SVC_HARD_CRIT     SLSC_NUM_HARD_CRIT
#define HLSC_NUM_SVC_HARD_UNKNOWN  SLSC_NUM_HARD_UNKNOWN
#define HLSC_WORST_SVC_HARD_STATE  SLSC_WORST_HARD_STATE

#define HLSC_NUM_HST_UP       10
#define HLSC_NUM_HST_DOWN     11
#define HLSC_NUM_HST_UNREACH  12
#define HLSC_NUM_HST_PENDING  13
#define HLSC_NUM_HST          -11
#define HLSC_WORST_HST_STATE  -12


class HostlistStateColumn : public IntColumn
{
    int _offset;
    int _logictype;

public:
    HostlistStateColumn(string name, string description, int logictype, int offset, int indirect_offset)
        : IntColumn(name, description, indirect_offset), _offset(offset), _logictype(logictype) {}
    int32_t getValue(void *data, Query *);
    GTree *getMembers(gpointer data);
};


#endif // HostlistStateColumn_h
