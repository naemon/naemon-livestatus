// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <stdlib.h>
#include <string.h>
#include "TimeColumnFilter.h"
#include "Query.h"
#include "logger.h"
#include "opids.h"

TimeColumnFilter::TimeColumnFilter(TimeColumn *column, int opid, char *value)
    : _column(column)
    , _opid(abs(opid))
    , _negate(opid < 0)
    , _ref_value(convertRefValue(value))
{
}

// offset from Localtime: header
time_t TimeColumnFilter::convertRefValue(const char *value)
{
    time_t ref_remote = atoi(value);
    if (_query) {
        time_t timezone_offset = _query->timezoneOffset();
        return ref_remote - timezone_offset;
    }
    else
        return ref_remote; // should never happen
}

bool TimeColumnFilter::accepts(void *data)
{
    bool pass = true;
    time_t act_value = _column->getValue(data, _query);
    switch (_opid) {
        case OP_EQUAL:
            pass = act_value == _ref_value; break;
        case OP_GREATER:
            pass = act_value > _ref_value; break;
        case OP_LESS:
            pass = act_value < _ref_value; break;
        default:
            logger(LG_INFO, "Sorry. Operator %s for time_t not implemented.", op_names_plus_8[_opid]);
            break;
    }
    return pass != _negate;
}

void TimeColumnFilter::findTimeLimits(const char *columnname, time_t *lower, time_t *upper)
{
    if (strcmp(columnname, _column->name())) {
        return; // wrong column
    }
    if (*lower >= *upper) {
        return; // already empty interval
    }

    /* [lower, upper[ is some interval. This filter might restrict
       that interval to a smaller interval.
     */
    int opref = _opid * (_negate != false ? -1 : 1);
    switch (opref) {
        case OP_EQUAL:
            if (_ref_value >= *lower && _ref_value < *upper) {
                *lower = _ref_value;
                *upper = _ref_value + 1;
            }
            else
                *lower = *upper;
            return;

        case -OP_EQUAL:
            if (_ref_value == *lower)
                *lower = *lower + 1;
            else if (_ref_value == *upper - 1)
                *upper = *upper - 1;
            return;

        case OP_GREATER:
            if (_ref_value >= *lower) {
                *lower = _ref_value + 1;
            }

            return;

        case OP_LESS:
            if (_ref_value < *upper)
                *upper = _ref_value;
            return;

        case -OP_GREATER: // LESS OR EQUAL
            if (_ref_value < *upper - 1)
                *upper = _ref_value + 1;
            return;

        case -OP_LESS: // GREATER OR EQUAL
            if (_ref_value > *lower)
                *lower = _ref_value;
            return;
    }
}
