// +------------------------------------------------------------------+
// |             ____ _               _        __  __ _  __           |
// |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
// |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
// |           | |___| | | |  __/ (__|   <    | |  | | . \            |
// |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
// |                                                                  |
// | Copyright Mathias Kettner 2012             mk@mathias-kettner.de |
// +------------------------------------------------------------------+
//
// This file is part of Check_MK.
// The official homepage is at http://mathias-kettner.de/check_mk.
//
// check_mk is free software;  you can redistribute it and/or modify it
// under the  terms of the  GNU General Public License  as published by
// the Free Software Foundation in version 2.  check_mk is  distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY;  with-
// out even the implied warranty of  MERCHANTABILITY  or  FITNESS FOR A
// PARTICULAR PURPOSE. See the  GNU General Public License for more de-
// ails.  You should have  received  a copy of the  GNU  General Public
// License along with GNU Make; see the file  COPYING.  If  not,  write
// to the Free Software Foundation, Inc., 51 Franklin St,  Fifth Floor,
// Boston, MA 02110-1301 USA.

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
