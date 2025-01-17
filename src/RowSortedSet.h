// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef RowSortedSet_h
#define RowSortedSet_h

#include <vector>

using namespace std;

class Column;
class Query;

class RowSortedSet
{

    struct _sort_col_t {
        Column *col;
        bool desc;
        _sort_col_t( Column *in_col, bool in_desc ) : col(in_col), desc(in_desc) {}
    };

    typedef vector<_sort_col_t> _columns_t;
    typedef vector<void*> rowset_t;

    rowset_t    _heap;
    _columns_t  _sort_cols;

    Query      *_query;

    int  compare( void *dataa, void *datab );
    void bubbleDown();

public:

    RowSortedSet();

    void addSortColumn( Column *col, bool desc );
    void setQuery( Query *query );

    void insert( void *data, int limit );
    void *extract();

    int  size();
};


#endif // RowStorage_h
