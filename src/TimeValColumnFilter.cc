// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <stdlib.h>
#include <string.h>
#include "TimeValColumnFilter.h"
#include "Query.h"
#include "logger.h"
#include "opids.h"

TimeValColumnFilter::TimeValColumnFilter(TimeValColumn *column, int opid, char *value)
    : _column(column)
    , _opid(abs(opid))
    , _negate(opid < 0)
    , _ref_value(convertRefValue(value))
{
}

// offset from Localtime: header
struct timeval TimeValColumnFilter::convertRefValue(const char *value)
{
    struct timeval ref_remote = {0, 0};
    str2timeval(value, &ref_remote);
    if (_query) {
        time_t timezone_offset = _query->timezoneOffset();
        return { ref_remote.tv_sec - timezone_offset, ref_remote.tv_usec };
    }
    else
        return ref_remote; // should never happen
}

bool TimeValColumnFilter::accepts(void *data)
{
    bool pass = true;
    struct timeval act_value = _column->getValue(data, _query);
    switch (_opid) {
        case OP_EQUAL:
            pass = act_value.tv_sec == _ref_value.tv_sec && act_value.tv_usec == _ref_value.tv_usec; break;
        case OP_GREATER:
            pass = act_value.tv_sec > _ref_value.tv_sec || (act_value.tv_sec == _ref_value.tv_sec && act_value.tv_usec > _ref_value.tv_usec); break;
        case OP_LESS:
            pass = act_value.tv_sec < _ref_value.tv_sec || (act_value.tv_sec == _ref_value.tv_sec && act_value.tv_usec < _ref_value.tv_usec); break;
        default:
            logger(LG_INFO, "Sorry. Operator %s for timeval not implemented.", op_names_plus_8[_opid]);
            break;
    }
    return pass != _negate;
}

/* Convert string to timeval */
int TimeValColumnFilter::str2timeval(const char *str, struct timeval *tv)
{
	char *ptr, *ptr2;

	tv->tv_sec = strtoul(str, &ptr, 10);
	if (ptr == str) {
		tv->tv_sec = tv->tv_usec = 0;
		return -1;
	}
	if (*ptr == '.' || *ptr == ',') {
		ptr2 = ptr + 1;
		tv->tv_usec = strtoul(ptr2, &ptr, 10);
	}
	return 0;
}
