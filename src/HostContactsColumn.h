// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef HostContactsColumn_h
#define HostContactsColumn_h

#include "config.h"

#include "ContactsColumn.h"

class HostContactsColumn : public ContactsColumn
{
public:
    HostContactsColumn(string name, string description, int indirect_offset)
        : ContactsColumn(name, description, indirect_offset) {}
    int type() { return COLTYPE_LIST; }
    bool isNagiosMember(void *data, void *member);
    bool isEmpty(void *data);
    void output(void *, Query *);
};

#endif // HostContactsColumn_h
