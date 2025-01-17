// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <strings.h>

#include "ServicelistColumnFilter.h"
#include "ServicelistColumn.h"
#include "nagios.h"
#include "opids.h"
#include "logger.h"

    ServicelistColumnFilter::ServicelistColumnFilter(ServicelistColumn *column, int opid, char *refvalue, bool show_host)
: _servicelist_column(column), _opid(opid), _show_host(show_host)
{
    if (abs(_opid) == OP_EQUAL && !refvalue[0])
        return; // test for emptiness is allowed

    if(!_show_host) {
        _ref_service = refvalue;
    } else {
        // ref_value must be of from hostname HOSTSERVICE_SEPARATOR service_description
        char *sep = index(refvalue, HOSTSERVICE_SEPARATOR);
        if (!sep) {
            logger(LG_INFO, "Invalid reference value for service list membership. Must be 'hostname%cservicename'", HOSTSERVICE_SEPARATOR);
            _ref_host = "";
            _ref_service = "";
        }
        else {
            _ref_host = string(refvalue, sep - refvalue);
            _ref_service = sep + 1;
        }
    }
}

bool ServicelistColumnFilter::accepts(void *data)
{
    // data points to a primary data object. We need to extract
    // a pointer to a service list
    servicesmember *mem = _servicelist_column->getMembers(data);

    // test for empty list
    if (abs(_opid) == OP_EQUAL && _ref_host == "")
        return (mem == 0) == (_opid == OP_EQUAL);

    bool is_member = false;
    while (mem) {
        service *svc = mem->service_ptr;
        if ((!_show_host || svc->host_name == _ref_host) && svc->description == _ref_service) {
            is_member = true;
            break;
        }
        mem = mem->next;
    }
    switch (_opid) {
        case -OP_LESS: /* !< means >= means 'contains' */
            return is_member;
        case OP_LESS:
            return !is_member;
        default:
            logger(LG_INFO, "Sorry, Operator %s for service lists lists not implemented.", op_names_plus_8[_opid]);
            return true;
    }
}
