#ifndef ServicelistDependencyColumnFilter_h
#define ServicelistDependencyColumnFilter_h

#include "config.h"

#include "Filter.h"
#include <string>
using namespace std;

class ServicelistDependencyColumn;

class ServicelistDependencyColumnFilter : public Filter
{
    ServicelistDependencyColumn *_servicelist_dependency_column;
    int _opid;
    string _ref_host;
    string _ref_service;
    bool _with_info;

public:
    ServicelistDependencyColumnFilter(ServicelistDependencyColumn *column, int opid, char *refvalue, bool with_info);
    bool accepts(void *data);
};


#endif // ServicelistDependencyColumnFilter_h
