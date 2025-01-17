// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef HostlistColumnFilter_h
#define HostlistColumnFilter_h

#include "config.h"

#include "Filter.h"
#include <string>
using namespace std;

class HostlistColumn;

class HostlistColumnFilter : public Filter
{
    HostlistColumn *_hostlist_column;
    int _opid;
    string _ref_value;

public:
    HostlistColumnFilter(HostlistColumn *column, int opid, char *refvalue)
        : _hostlist_column(column), _opid(opid), _ref_value(refvalue) {}
    bool accepts(void *data);
};


#endif // HostlistColumnFilter_h
