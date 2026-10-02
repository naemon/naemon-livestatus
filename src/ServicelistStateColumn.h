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

// +64 for hard variants

#define SLSC_NUM_HARD_OK       ( 0 + 64)
#define SLSC_NUM_HARD_WARN     ( 1 + 64)
#define SLSC_NUM_HARD_CRIT     ( 2 + 64)
#define SLSC_NUM_HARD_UNKNOWN  ( 3 + 64)
#define SLSC_WORST_HARD_STATE  (-2 + 64)

#define SLSC_NUM               -1


// Everything a ServicelistStateColumn can be asked about one list of services,computed in a single traversal.
// Query keeps one of these per service list for the duration of a query, so a query that asks for several aggregate
// columns (num_services, num_services_ok, worst_service_state, ...) only walks the list once instead of once per column.
struct ServiceAggregates {
    // all services that authenticated user sees
    int32_t num;
    // of those, that are not checked yet
    int32_t num_pending;

    //  checked services by current_state
    int32_t soft[4];
    // checked services by last_hard_state (SLSC_NUM_HARD_*)
    int32_t hard[4];

    // worst current_state (SLSC_WORST_STATE)
    int32_t worst_soft;
    // worst last_hard_state (SLSC_WORST_HARD_STATE)
    int32_t worst_hard;

    ServiceAggregates()
        : num(0), num_pending(0), worst_soft(0), worst_hard(0)
    {
        for (int i = 0; i < 4; i++) {
            soft[i] = 0;
            hard[i] = 0;
        }
    }

    // a state number 0..3 is a count, -1 is the total, 4 is pending, -2 is the worst state,
    // +64 selects the hard instead of the soft variant.

    int32_t get(int logictype) const {
        int lt = logictype;
        bool use_hard = false;
        if (logictype >= 60) {
            lt = logictype - 64;
            use_hard = true;
        }
        switch (lt) {
        case SLSC_NUM:
            return num;
        case SLSC_NUM_PENDING:
            return num_pending;
        case SLSC_WORST_STATE:
            return use_hard ? worst_hard : worst_soft;
        default:
            if (lt >= 0 && lt <= 3)
                return use_hard ? hard[lt] : soft[lt];
            return 0;
        }
    }
};


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

    // Single pass over a service list producing all aggregate values.
    static ServiceAggregates computeAggregates(servicesmember *services, Query *query);
};


#endif // ServicelistStateColumn_h
