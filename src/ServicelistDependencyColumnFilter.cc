// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <strings.h>

#include "ServicelistDependencyColumnFilter.h"
#include "ServicelistDependencyColumn.h"
#include "ServicelistColumnFilter.h"
#include "nagios.h"
#include "opids.h"
#include "logger.h"


    ServicelistDependencyColumnFilter::ServicelistDependencyColumnFilter(ServicelistDependencyColumn *column, int opid, char *refvalue, bool with_info)
: _servicelist_dependency_column(column), _opid(opid), _with_info(with_info)
{
    if (abs(_opid) == OP_EQUAL && !refvalue[0])
        return; // test for emptiness is allowed

    // ref_value must be of from hostname HOSTSERVICE_SEPARATOR service_description
    char *sep = index(refvalue, HOSTSERVICE_SEPARATOR);
    if (!sep) {
        logger(LG_INFO, "Invalid reference value for service dependency list membership. Must be 'hostname%cservicename'", HOSTSERVICE_SEPARATOR);
        _ref_host = "";
        _ref_service = "";
    }
    else {
        _ref_host = string(refvalue, sep - refvalue);
        _ref_service = sep + 1;
    }
}

bool ServicelistDependencyColumnFilter::accepts(void *data)
{
    // data points to a primary object list containing pointer to service dependencies
    objectlist *list = _servicelist_dependency_column->getList(data);

    // test for empty list
    if(abs(_opid) == OP_EQUAL && _ref_service == "")
        return (list == 0) == (_opid == OP_EQUAL);

    bool is_member = false;
    while (list) {
        servicedependency *dependency = (servicedependency *)list->object_ptr;
        service *svc = dependency->master_service_ptr;
        if(svc->host_name == _ref_host && svc->description == _ref_service) {
            is_member = true;
            break;
        }
        list = list->next;
    }
    switch (_opid) {
        case -OP_LESS: // !< means >= means 'contains'
            return is_member;
        case OP_LESS:
            return !is_member;
        default:
            logger(LG_INFO, "Sorry, Operator %s for service dependecy lists lists not implemented.", op_names_plus_8[_opid]);
            return true;
    }
}
