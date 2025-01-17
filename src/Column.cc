// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "Column.h"
#include "logger.h"


Column::Column(string name, string description, int indirect_offset)
  : _name(name)
  , _description(description)
  , _indirect_offset(indirect_offset)
{
}

void *Column::shiftPointer(void *data)
{
    if (!data)
        return 0;

    else if (_indirect_offset >= 0) {
        // add one indirection level
        // indirect_offset is place in structure, where
        // pointer to real object is
        return *((void **)((char *)data + _indirect_offset));
    }
    else // no indirection
        return data;
}

int Column::compare(void *dataa, void *datab, Query *query) {
    /* Column cant be compared. Assume everything is equal. Makes column
     * unsorted. Override this function for comparable types
     */
    return 0;
}
