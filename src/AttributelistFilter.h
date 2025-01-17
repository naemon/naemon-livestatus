// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef AttributelistFilter_h
#define AttributelistFilter_h

#include "config.h"
#include "Filter.h"
#include <stdlib.h>

class AttributelistColumn;

class AttributelistFilter : public Filter
{
    AttributelistColumn *_column;
    int _opid;
    bool _negate;
    unsigned long _ref;

public:
    AttributelistFilter(AttributelistColumn *column, int opid, unsigned long ref) :
        _column(column), _opid(abs(opid)), _negate(opid < 0), _ref(ref) {}
    bool accepts(void *data);
};


#endif // AttributelistFilter_h
