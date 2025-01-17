// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ListColumnFilter_h
#define ListColumnFilter_h

#include "config.h"

#include "Filter.h"
class ListColumn;

class ListColumnFilter : public Filter
{
    ListColumn *_column;
    void *_ref_member;
    int _opid;
    bool _empty_ref; // distinct from unknown ref

public:
    ListColumnFilter(ListColumn *column, int opid, char *value);
    bool accepts(void *data);
    void *indexFilter(const char *columnname);
};


#endif // ListColumnFilter_h
