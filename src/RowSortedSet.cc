// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "RowSortedSet.h"
#include "Column.h"


RowSortedSet::RowSortedSet()
    :_query(0)
{
}


void RowSortedSet::addSortColumn( Column *col, bool desc )
{
    _sort_cols.push_back( _sort_col_t( col, desc ) );
}

void RowSortedSet::setQuery( Query *query )
{
    _query = query;
}

/* Direction should be as dataa-datab; positive if dataa>datab */
int RowSortedSet::compare( void *dataa, void *datab )
{
    int i;
    int diff;
    for (i=0; i<_sort_cols.size(); i++) {
        diff = _sort_cols[i].col->compare( dataa, datab, _query );
        if( _sort_cols[i].desc ) {
            diff = -diff;
        }
        if( diff != 0 ) {
            return diff;
        }
    }
    return 0;
}

void RowSortedSet::insert( void *data, int limit )
{
    int cur;
    _heap.push_back( data ); /* Make space for object */

    for( cur=_heap.size()-1;
         cur != 0 && compare( _heap[(cur-1)>>1], data ) < 0;
         cur=(cur-1)>>1 )
    {
        _heap[cur] = _heap[(cur-1)>>1];
    }
    _heap[cur] = data;


    if( limit > 0 && _heap.size() > limit ) {
        extract();
    }
}

void *RowSortedSet::extract()
{
    void *data;
    void *last;
    int   cur;
    int   child;

    if( _heap.empty() )
        return 0;

    data = _heap[0];

    /* return early if there is just one item */
    if (_heap.size() == 1) {
        _heap.pop_back();
        return data;
    };

    last = _heap.back();
    _heap.pop_back();

    for( cur=0; (cur<<1)+1 < _heap.size(); cur=child ) {
        child = (cur<<1)+1;

        if( child+1 < _heap.size() )
            if( compare( _heap[child+1], _heap[child] ) > 0 )
                child++;

        if( compare( _heap[child], last ) > 0 ) {
            _heap[cur] = _heap[child];
        } else {
            break;
        }
    }

    _heap[cur] = last;

    return data;
}

int RowSortedSet::size()
{
    return _heap.size();
}
