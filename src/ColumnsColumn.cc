// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ColumnsColumn.h"
#include "Column.h"
#include "TableColumns.h"

const char *ColumnsColumn::getValue(void *data)
{
    Column *column = (Column *)data;
    return (char *)_table_columns->getValue(column, _colcol);
}
