// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef Table_h
#define Table_h

#include "config.h"
#include "nagios.h"

#include <map>
#include <vector>
#include <vector>
#include <string>

using namespace std;

class Column;
class Query;

class Table
{
public:
    typedef map<string, Column *> _columns_t;
    typedef vector<Column *> _nat_sort_t;
private:
    _columns_t _columns;
    _nat_sort_t _nat_sort; /* Natural sort order columns */

public:
    Table() {}
    virtual ~Table();
    virtual Column *column(const char *name);
    virtual void answerQuery(Query *) = 0;
    virtual void cleanupQuery(Query * __attribute__ ((__unused__))) {}
    virtual const char *name() = 0;
    virtual const char *prefixname() { return name(); }
    virtual bool isAuthorized(contact *, void *) { return true; }
    virtual void *findObject(char *objectspec __attribute__ ((__unused__))) { return 0; } // objectspec may be modified while parsing
    virtual bool hasLogcache() { return false; };
    void clearNatSort();
    void addNatSort(string);
    void addColumn(Column *);
    bool hasColumn(Column *);
    void addAllColumnsToQuery(Query *);
    _columns_t *columns() { return &_columns; }
    _nat_sort_t *natSortColumns() { return &_nat_sort; }
};


#endif // Table_h
