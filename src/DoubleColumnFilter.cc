// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <stdlib.h>
#include "DoubleColumnFilter.h"
#include "DoubleColumn.h"
#include "logger.h"
#include "opids.h"

DoubleColumnFilter::DoubleColumnFilter(DoubleColumn *column, int opid, char *value)
    : _column(column)
    , _ref_value(atof(value))
    , _opid(abs(opid))
    , _negate(opid < 0)
{
}

bool DoubleColumnFilter::accepts(void *data)
{
    bool pass = true;
    double act_value = _column->getValue(data);
    switch (_opid) {
        case OP_EQUAL:
            pass = act_value == _ref_value; break;
        case OP_GREATER:
            pass = act_value > _ref_value; break;
        case OP_LESS:
            pass = act_value < _ref_value; break;
        default:
            logger(LG_INFO, "Sorry. Operator %s for float columns not implemented.", op_names_plus_8[_opid]);
            break;
    }
    return pass != _negate;
}
