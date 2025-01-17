// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef HostlistColumn_h
#define HostlistColumn_h

#include <glib.h>
#include "config.h"

#include "Column.h"
#include "nagios.h"

class HostlistColumn : public Column
{
    int  _offset;
    bool _show_state;
public:
    HostlistColumn(string name, string description, int offset, int indirect_offset, bool show_state)
        : Column(name, description, indirect_offset), _offset(offset), _show_state(show_state) {}
    int type() { return COLTYPE_LIST; }
    void output(void *, Query *);
    Filter *createFilter(int opid, char *value);
    GTree *getMembers(gpointer data);
};



#endif // HostlistColumn_h
