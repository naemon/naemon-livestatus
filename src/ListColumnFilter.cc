// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "opids.h"
#include "ListColumnFilter.h"
#include "ListColumn.h"

ListColumnFilter::ListColumnFilter(ListColumn *column, int opid, char *value)
    : _column(column)
    , _opid(opid)
    , _empty_ref(!value[0])
{
    _ref_member = _column->getNagiosObject(value);
}

bool ListColumnFilter::accepts(void *data)
{
    data = _column->shiftPointer(data);
    if (!data)
        return false;
    bool is_member = _column->isNagiosMember(data, _ref_member);
    switch (_opid) {
        case -OP_LESS: /* !< means >= means 'contains' */
            return is_member;
        case OP_LESS:
            return !is_member;
        case OP_EQUAL:
        case -OP_EQUAL:
            if (_empty_ref)
                return _column->isEmpty(data) == (_opid == OP_EQUAL);
            logger(LG_INFO, "Sorry, equality for lists implemented only for emptyness");
            return false;

        default:
            logger(LG_INFO, "Sorry, Operator %s for lists not implemented.", op_names_plus_8[_opid]);
            return true;
    }
}

void *ListColumnFilter::indexFilter(const char *columnname)
{
    if (_opid == -OP_LESS && !strcmp(columnname, _column->name()))
        return _ref_member;
    else
        return 0;
}
