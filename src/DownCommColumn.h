// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef DownCommColumn_h
#define DownCommColumn_h

#include "config.h"

#include "ListColumn.h"
#include "TableContacts.h"

class TableDownComm;

class DownCommColumn : public ListColumn
{
    bool _is_downtime;
    bool _with_info;
public:
    DownCommColumn(string name, string description, int indirect_offset, bool is_downtime, bool with_info)
        : ListColumn(name, description, indirect_offset), _is_downtime(is_downtime), _with_info(with_info) {}
    int type() { return COLTYPE_LIST; }
    void output(void *, Query *);
    void *getNagiosObject(char *name);
    bool isEmpty(void *data);
    bool isNagiosMember(void *data, void *member);
};


#endif // DownCommColumn_h
