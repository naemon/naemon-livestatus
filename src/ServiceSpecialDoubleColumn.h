// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ServiceSpecialDoubleColumn_h
#define ServiceSpecialDoubleColumn_h

#include "config.h"
#include "DoubleColumn.h"

#define SSDC_STALENESS 1

class ServiceSpecialDoubleColumn : public DoubleColumn
{
    int _type;

public:
    ServiceSpecialDoubleColumn(string name, string description, int ssdc_type, int indirect)
        : DoubleColumn(name, description, indirect) , _type(ssdc_type) {}
    double getValue(void *data);
};

#endif // ServiceSpecialDoubleColumn_h
