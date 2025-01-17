// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef EmptyFilter_h
#define EmptyFilter_h

#include "config.h"

using namespace std;

#include "Filter.h"
class EmptyColumn;

class EmptyFilter : public Filter
{
    Column *_column;
    int _opid;
    bool _negate;
public:
    EmptyFilter(Column *_column, int opid, char *value);
    ~EmptyFilter();
    bool accepts(void *data);
    void *indexFilter(const char *column);
};


#endif // EmptyFilter_h
