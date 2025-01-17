// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ContactgroupsColumn_h
#define ContactgroupsColumn_h

#include "config.h"

#include "ListColumn.h"
#include "nagios.h"

class ContactgroupsColumn : public ListColumn
{
    int _offset;
public:
    ContactgroupsColumn(string name, string description, int offset, int indirect_offset)
        : ListColumn(name, description, indirect_offset), _offset(offset) {}
    int type() { return COLTYPE_LIST; }
    void output(void *, Query *);
    void *getNagiosObject(char *name); // return pointer to contact group
    bool isNagiosMember(void *data, void *nagobject);
    bool isEmpty(void *data);
private:
    contactgroupsmember *getData(void *);
};



#endif // ContactgroupsColumn_h
