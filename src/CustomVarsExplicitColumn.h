// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef CustomVarsExplicitColumn_h
#define CustomVarsExplicitColumn_h

#include "StringColumn.h"
#include "nagios.h"

using namespace std;


class CustomVarsExplicitColumn : public StringColumn
{
    int _offset; // within data structure (differs from host/service)
    std::string _varname;

public:
    CustomVarsExplicitColumn(string name, string description, int offset, int indirect_offset, const char *varname)
        : StringColumn(name, description, indirect_offset),  _offset(offset), _varname(varname) {}
    const char *getValue(void *data);
private:
    customvariablesmember *getCVM(void *data);
};


#endif // CustomVarsExplicitColumn_h
