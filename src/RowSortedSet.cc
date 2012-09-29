// This is a file made for Check_MK by Mathias Kettner 2012
//
// This file is written by Max Sikström to a branch for OP5 Montior
// http://www.op5.com
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
