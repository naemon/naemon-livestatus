// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <string.h>
#include "Table.h"
#include "Column.h"
#include "Query.h"
#include "logger.h"

void Table::clearNatSort()
{
    _nat_sort.clear();
}

void Table::addNatSort(string colname)
{
    _columns_t::iterator it = _columns.find(colname);
    if (it != _columns.end())
        _nat_sort.push_back( it->second );
}

void Table::addColumn(Column *col)
{
    // do not insert column if one with that name
    // already exists. Delete that column in that
    // case. (For example needed for TableLog->TableHosts,
    // which both define host_name.
    if (column(col->name())) {
        delete col;
    }
    else
        _columns.insert(make_pair(col->name(), col));
}



Table::~Table()
{
    for (_columns_t::iterator it = _columns.begin();
            it != _columns.end();
            ++it)
    {
        delete it->second;
    }
}


void Table::addAllColumnsToQuery(Query *q)
{
    for (_columns_t::iterator it = _columns.begin();
            it != _columns.end();
            ++it)
    {
        q->addColumn(it->second);
    }
}


Column *Table::column(const char *colname)
{
    // First try exact match
    _columns_t::iterator it = _columns.find(string(colname));
    if (it != _columns.end())
        return it->second;

    // Second allow column names to bear prefix like
    // the tablename (e.g. service_ for table services, or log_ for table log)
    int prefix_len = strlen(prefixname()); // replace 's' with '_'
    if (!strncmp(colname, prefixname(), prefix_len - 1) && \
            colname[prefix_len - 1] == '_')
    {
        return column(colname + prefix_len);
    }
    else
        return 0;
}


bool Table::hasColumn(Column *col)
{
    // this is not very efficient but seldomly used
    for (_columns_t::iterator it = _columns.begin();
            it != _columns.end();
            ++it)
    {
        if (col == it->second)
            return true;
    }
    return false;
}
