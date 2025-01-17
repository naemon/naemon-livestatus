// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef HostgroupsColumn_h
#define HostgroupsColumn_h

#include "config.h"

#include "ListColumn.h"
#include "nagios.h"

class HostgroupsColumn : public ListColumn
{
    int _offset;
public:
    HostgroupsColumn(string name, string description, int offset, int indirect_offset)
        : ListColumn(name, description, indirect_offset), _offset(offset) {}
    int type() { return COLTYPE_LIST; }
    void output(void *, Query *);
    void *getNagiosObject(char *name); // return pointer to host group
    bool isNagiosMember(void *data, void *nagobject);
    bool isEmpty(void *data);
private:
    objectlist *getData(void *);
};



#endif // HostgroupsColumn_h
