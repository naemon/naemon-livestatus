// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ServiceSpecialDoubleColumn_h
#define ServiceSpecialDoubleColumn_h

#include "config.h"
#include "DoubleColumn.h"

#define HSDC_STALENESS 1

class HostSpecialDoubleColumn : public DoubleColumn
{
    int _type;

public:
    HostSpecialDoubleColumn(string name, string description, int hsdc_type, int indirect)
        : DoubleColumn(name, description, indirect) , _type(hsdc_type) {}
    double getValue(void *data);
};

#endif // ServiceSpecialDoubleColumn_h
