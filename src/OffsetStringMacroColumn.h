// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OffsetStringMacroColumn_h
#define OffsetStringMacroColumn_h

#include "nagios.h"
#include "OffsetStringColumn.h"

class OffsetStringMacroColumn : public OffsetStringColumn
{
    int _offset;
public:
    OffsetStringMacroColumn(string name, string description, int offset, int indirect_offset = -1) :
        OffsetStringColumn(name, description, offset, indirect_offset) {}
    // reimplement several functions from StringColumn

    string valueAsString(void *data, Query *);
    void output(void *data, Query *);

    // overriden by host and service macro columns
    virtual host *getHost(void *) = 0;
    virtual service *getService(void *) = 0;
private:
    const char *expandMacro(const char *macroname, host *hst, service *svc);
    const char *expandCustomVariables(const char *varname, customvariablesmember *custvars);
};

#endif // OffsetStringMacroColumn_h
