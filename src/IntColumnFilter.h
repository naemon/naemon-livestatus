// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef IntColumnFilter_h
#define IntColumnFilter_h

#include "config.h"

#include <string>
using namespace std;

#include "Filter.h"
class IntColumn;

class IntColumnFilter : public Filter
{
    IntColumn *_column;
    int _opid;
    bool _negate;
    string _ref_string;

public:
    IntColumnFilter(IntColumn *column, int opid, char *value);
    virtual int32_t convertRefValue();
    bool accepts(void *data);
    bool optimizeBitmask(const char *columnname, uint32_t *mask);
};


#endif // IntColumnFilter_h
