// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ServicelistColumn_h
#define ServicelistColumn_h

#include "config.h"

#include "Column.h"
#include "nagios.h"

class ServicelistColumn : public Column
{
    int _offset;
    bool _show_host;
    int _info_depth;
public:
    ServicelistColumn(string name, string description, int offset, int indirect_offset, bool show_host, int info_depth)
        : Column(name, description, indirect_offset), _offset(offset), _show_host(show_host), _info_depth(info_depth) {}
    int type() { return COLTYPE_LIST; };
    void output(void *, Query *);
    Filter *createFilter(int opid, char *value);
    servicesmember *getMembers(void *data);
};



#endif // ServicelistColumn_h
