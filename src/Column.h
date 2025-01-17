// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef Column_h
#define Column_h

#include "config.h"

#include <stdio.h>
#include <string>
using namespace std;

#define COLTYPE_INT     0
#define COLTYPE_DOUBLE  1
#define COLTYPE_STRING  2
#define COLTYPE_LIST    3
#define COLTYPE_TIME    4
#define COLTYPE_DICT    5

class Filter;
class Query;

class Column
{
    string _name;
    string _description;
public:
    int _indirect_offset;

public:
    Column(string name, string description, int indirect_offset);
    virtual ~Column() {}
    const char *name() const { return _name.c_str(); }
    const char *description() const { return _description.c_str(); }
    virtual string valueAsString(void *data __attribute__ ((__unused__)), Query *)
        { return "invalid"; }
    virtual int type() = 0;
    virtual void output(void *data, Query *) = 0;
    virtual Filter *createFilter(int opid __attribute__ ((__unused__)), char *value __attribute__ ((__unused__))) { return 0; }
    void *shiftPointer(void *data);
    virtual int compare(void *dataa, void *datab, Query *query);
};

#endif // Column_h
