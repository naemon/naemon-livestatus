#ifndef HostlistDependencyColumnFilter_h
#define HostlistDependencyColumnFilter_h

#include "config.h"

#include "Filter.h"
#include <string>
using namespace std;

class HostlistDependencyColumn;

class HostlistDependencyColumnFilter : public Filter
{
    HostlistDependencyColumn *_hostlist_dependency_column;
    int _opid;
    string _ref_host;
    bool _with_info;

public:
    HostlistDependencyColumnFilter(HostlistDependencyColumn *column, int opid, char *refvalue, bool with_info);
    bool accepts(void *data);
};


#endif // HostlistDependencyColumnFilter_h
