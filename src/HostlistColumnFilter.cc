// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <glib.h>
#include "HostlistColumnFilter.h"
#include "HostlistColumn.h"
#include "nagios.h"
#include "opids.h"
#include "logger.h"

bool HostlistColumnFilter::accepts(void *data)
{
    // data points to a primary data object. We need to extract
    // a pointer to a host list
    GTree *mem = _hostlist_column->getMembers(data);

    // test for empty list
    if (abs(_opid) == OP_EQUAL && _ref_value == "") {
        bool is_empty = false;
        if(mem == 0) {
            is_empty = true;
        } else if(g_tree_nnodes(mem) == 0) {
            is_empty = true;
        }
        return is_empty == (_opid == OP_EQUAL);
    }

    bool is_member;

    switch (_opid) {
        case -OP_LESS: /* !< means >= means 'contains' */
            is_member = true;
            break;
        case OP_LESS:
            is_member = false;
            break;
        default:
            logger(LG_INFO, "Sorry, Operator %s for host lists lists not implemented.", op_names_plus_8[_opid]);
            return true;
    }
    if (g_tree_lookup(mem, _ref_value.c_str()))
        return is_member;
    return !is_member;
}
