// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ColumnsColumn_h
#define ColumnsColumn_h

#include "config.h"

#include "StringColumn.h"

#define COLCOL_TABLE 1
#define COLCOL_NAME  2
#define COLCOL_DESCR 3
#define COLCOL_TYPE  4

class TableColumns;

class ColumnsColumn : public StringColumn
{
    int _colcol;
    TableColumns *_table_columns;

public:
    ColumnsColumn(string name, string description, int colcol, TableColumns *tablecols) :
        StringColumn(name, description, -1), _colcol(colcol), _table_columns(tablecols) {}
    const char *getValue(void *data);
};


#endif // ColumnsColumn_h
