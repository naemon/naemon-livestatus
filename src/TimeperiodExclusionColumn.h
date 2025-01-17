// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ServicelistColumn_h
#define ServicelistColumn_h

#include "config.h"

#include "ListColumn.h"
#include "nagios.h"

class TimeperiodExclusionColumn : public ListColumn
{
    int _offset;
    timeperiodexclusion *getData(void *data);
public:
    TimeperiodExclusionColumn(string name, string description, int offset, int indirect_offset)
        : ListColumn(name, description, indirect_offset), _offset(offset) {}
    int type() { return COLTYPE_LIST; };
    void output(void *data, Query *query);
    void *getNagiosObject(char *name);
    bool isNagiosMember(void *data, void *member);
    bool isEmpty(void *data);
};



#endif // ServicelistColumn_h
