// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <stdlib.h>
#include <string.h>
#include "IntColumnFilter.h"
#include "IntColumn.h"
#include "logger.h"
#include "opids.h"

    IntColumnFilter::IntColumnFilter(IntColumn *column, int opid, char *value)
    : _column(column)
    , _opid(abs(opid))
    , _negate(opid < 0)
      , _ref_string(value)
{
}

int32_t IntColumnFilter::convertRefValue()
{
    return atoi(_ref_string.c_str());
}

bool IntColumnFilter::accepts(void *data)
{
    bool pass = true;
    int32_t act_value = _column->getValue(data, _query);
    int32_t ref_value = convertRefValue();
    switch (_opid) {
        case OP_EQUAL:
            pass = act_value == ref_value; break;
        case OP_GREATER:
            pass = act_value > ref_value; break;
        case OP_LESS:
            pass = act_value < ref_value; break;
        default:
            logger(LG_INFO, "Sorry. Operator %s for integers not implemented.", op_names_plus_8[_opid]);
            break;
    }
    return pass != _negate;
}

bool IntColumnFilter::optimizeBitmask(const char *columnname, uint32_t *mask)
{
    int32_t ref_value = convertRefValue();

    if (strcmp(columnname, _column->name())) {
        return false; // wrong column
    }

    if (ref_value < 0 || ref_value > 31)
        return true; // not optimizable by 32bit bit mask

    // Our task is to remove those bits from mask that are deselected
    // by the filter.
    uint32_t bit = 1 << ref_value;

    int opref = _opid * (_negate != false ? -1 : 1);
    switch (opref) {
        case OP_EQUAL:
            *mask &= bit; // bit must be set
            return true;

        case -OP_EQUAL:
            *mask &= ~bit; // bit must not be set
            return true;

        case -OP_LESS: // >=
            bit >>= 1;
        case OP_GREATER:
            while (bit) {
                *mask &= ~bit;
                bit >>= 1;
            }
            return true;

        case -OP_GREATER: // <=
            if (ref_value == 31)
                return true;
            bit <<= 1;
        case OP_LESS:
            while (true) {
                *mask &= ~bit;
                if (bit == 0x80000000)
                    return true;
                bit <<= 1;
            }
            return true;
    }
    return false; // should not be reached
}
