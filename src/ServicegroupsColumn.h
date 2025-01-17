// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ServicegroupsColumn_h
#define ServicegroupsColumn_h

#include "config.h"

#include "ListColumn.h"
#include "nagios.h"

class ServicegroupsColumn : public ListColumn
{
    int _offset;
public:
    ServicegroupsColumn(string name, string description, int offset, int indirect_offset)
        : ListColumn(name, description, indirect_offset), _offset(offset) {}
    int type() { return COLTYPE_LIST; }
    void output(void *, Query *);
    void *getNagiosObject(char *name); // return pointer to service group
    bool isEmpty(void *data);
    bool isNagiosMember(void *data, void *nagobject);
private:
    objectlist *getData(void *);
};



#endif // ServicegroupsColumn_h
