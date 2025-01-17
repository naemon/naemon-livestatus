// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef CustomVarsColumn_h
#define CustomVarsColumn_h

#include "config.h"

#include "Column.h"
#include <string>
#include "nagios.h"

using namespace std;

#define CVT_VARNAMES 0
#define CVT_VALUES   1
#define CVT_DICT     2

class CustomVarsColumn : public Column
{
    int _offset; // within data structure (differs from host/service)
    int _what;

public:
    CustomVarsColumn(string name, string description, int offset, int indirect_offset, int what)
        : Column(name, description, indirect_offset),  _offset(offset), _what(what) {}
    int type() { return _what == CVT_DICT ? COLTYPE_DICT : COLTYPE_LIST; }
    void output(void *, Query *);
    Filter *createFilter(int opid, char *value);
    bool contains(void *data, const char *value);
    char *getVariable(void *data, const char *varname);
private:
    customvariablesmember *getCVM(void *data);
};


#endif // CustomVarsColumn_h
