// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef IntPointerColumn_h
#define IntPointerColumn_h

#include "IntColumn.h"

class IntPointerColumn : public IntColumn
{
    int *_number;
public:
    IntPointerColumn(string name, string description, int* number)
        : IntColumn(name, description, -1), _number(number) {}
    int32_t getValue(void *, Query *) { return *_number; }
};


#endif // IntPointerColumn_h
