// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ServicelistColumnFilter_h
#define ServicelistColumnFilter_h

#include "config.h"

#include "Filter.h"
#include <string>
using namespace std;

class ServicelistColumn;

class ServicelistColumnFilter : public Filter
{
    ServicelistColumn *_servicelist_column;
    int _opid;
    string _ref_host;
    string _ref_service;
    bool _show_host;

public:
    ServicelistColumnFilter(ServicelistColumn *column, int opid, char *refvalue, bool show_host);
    bool accepts(void *data);
};


#endif // ServicelistColumnFilter_h
