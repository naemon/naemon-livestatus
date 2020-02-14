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
// Updated 2012 by Max Sikström - op5: Added compare interface
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

#ifndef StringColumn_h
#define StringColumn_h

#include "config.h"

#include "Column.h"

class StringColumn : public Column
{
public:
    StringColumn(string name, string description, int indirect_offset) :
        Column(name, description, indirect_offset) {}
    virtual const char *getValue(void *data) = 0;
    string valueAsString(void *data, Query *) { return getValue(data); }
    void output(void *, Query *);
    int type() { return COLTYPE_STRING; }
    Filter *createFilter(int operator_id, char *value);
    int compare(void *dataa, void*datab, Query *query);
};

#endif // StringColumn_h
