#include <strings.h>

#include "HostlistDependencyColumnFilter.h"
#include "HostlistDependencyColumn.h"
#include "nagios.h"
#include "opids.h"
#include "logger.h"


    HostlistDependencyColumnFilter::HostlistDependencyColumnFilter(HostlistDependencyColumn *column, int opid, char *refvalue, bool with_info)
: _hostlist_dependency_column(column), _opid(opid), _with_info(with_info)
{
    if (abs(_opid) == OP_EQUAL && !refvalue[0])
        return; // test for emptiness is allowed

    _ref_host = refvalue;
}

bool HostlistDependencyColumnFilter::accepts(void *data)
{
    // data points to a primary object list containing pointer to host dependencies
    objectlist *list = _hostlist_dependency_column->getList(data);

    // test for empty list
    if(abs(_opid) == OP_EQUAL && _ref_host == "") // bugfix (found by clang warning -Wabsolute-value)
        return (list == 0) == (_opid == OP_EQUAL);

    bool is_member = false;
    while (list) {
        hostdependency *dependency = (hostdependency *)list->object_ptr;
        host *hst = dependency->master_host_ptr;
        if(hst->name == _ref_host) {
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
            logger(LG_INFO, "Sorry, Operator %s for host dependecy lists lists not implemented.", op_names_plus_8[_opid]);
            return true;
    }
}
