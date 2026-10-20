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


// Everything the host-state HostlistStateColumn values can be asked about one list of hosts,
// computed in a single traversal. Query keeps one of these per host list (hostgroup) for the
// duration of a query, so a query that asks for several host columns (num_hosts, num_hosts_up,
// worst_host_state, ...) only walks the host tree once instead of once per column. Mirrors the
// ServiceAggregates cache added in #145. (The hostgroup service columns reuse the per-host
// ServiceAggregates cache and keep their own per-host walk, so they are not part of this.)
struct HostAggregates {
    int32_t num;          // HLSC_NUM_HST
    int32_t num_pending;  // HLSC_NUM_HST_PENDING
    int32_t up;           // HLSC_NUM_HST_UP
    int32_t down;         // HLSC_NUM_HST_DOWN
    int32_t unreach;      // HLSC_NUM_HST_UNREACH
    int32_t worst_host;   // HLSC_WORST_HST_STATE

    HostAggregates()
        : num(0), num_pending(0), up(0), down(0), unreach(0), worst_host(0)
    {
    }

    int32_t get(int logictype) const {
        switch (logictype) {
        case HLSC_NUM_HST:         return num;
        case HLSC_NUM_HST_PENDING: return num_pending;
        case HLSC_NUM_HST_UP:      return up;
        case HLSC_NUM_HST_DOWN:    return down;
        case HLSC_NUM_HST_UNREACH: return unreach;
        case HLSC_WORST_HST_STATE: return worst_host;
        default:                   return 0;
        }
    }
};


class HostlistStateColumn : public IntColumn
{
    int _offset;
    int _logictype;

public:
    HostlistStateColumn(string name, string description, int logictype, int offset, int indirect_offset)
        : IntColumn(name, description, indirect_offset), _offset(offset), _logictype(logictype) {}
    int32_t getValue(void *data, Query *);
    GTree *getMembers(gpointer data);

    // Single pass over a host list producing all aggregate values.
    static HostAggregates computeAggregates(GTree *members, Query *query);
};


#endif // HostlistStateColumn_h
