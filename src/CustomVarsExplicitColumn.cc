// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "CustomVarsExplicitColumn.h"

const char *CustomVarsExplicitColumn::getValue(void *data)
{
    customvariablesmember *cvm = getCVM(data);
    while (cvm) {
        if (cvm->variable_name == _varname)
            return cvm->variable_value;
        cvm = cvm->next;
    }
    return (char *)"";
}

customvariablesmember *CustomVarsExplicitColumn::getCVM(void *data)
{
    if (!data) return 0;
    data = shiftPointer(data);
    if (!data) return 0;
    return *(customvariablesmember **)((char *)data + _offset);
}
