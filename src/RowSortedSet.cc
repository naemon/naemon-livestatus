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
    _sort_cols.push_back( col );
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
        diff = _sort_cols[i]->compare( dataa, datab, _query );
        if( diff != 0 ) {
            return diff;
        }
    }
    return 0;
}

void RowSortedSet::bubbleUp()
{
    int child;
    int parent;
    int diff;
    void *tmp;

    child = _heap.size()-1;
    while( child != 0 ) {
        parent = (child-1)>>1;
        diff = compare( _heap[child], _heap[parent] );

        if( diff > 0 ) {
            tmp           = _heap[parent];
            _heap[parent] = _heap[child];
            _heap[child]  = tmp;
        } else {
            /* No need to continue... */
            break;
        }
        child=parent;
    }
}

void RowSortedSet::bubbleDown()
{
    int parent;
    int child;
    int diff;
    void *tmp;

    parent = 0;
    for(;;) {
        child = (parent<<1) + 1;

        if( child >= _heap.size() ) {
            break;
        }
        child = child;
        if( child+1 < _heap.size() ) {
            /* Two children exists */
            diff = compare( _heap[child+1], _heap[child] );
            if( diff > 0 ) {
                /* Second child is bigger */
                child++;
            }
        }
        diff = compare( _heap[child], _heap[parent] );
        if( diff > 0 ) {
            tmp           = _heap[parent];
            _heap[parent] = _heap[child];
            _heap[child]  = tmp;
        } else {
            /* No need to continue */
            break;
        }
        parent = child;
    };
}

void RowSortedSet::insert( void *data, int limit )
{
    _heap.push_back( data ); /* Add to last */
    bubbleUp(); /* Ensure heap consistancy */

    while( _heap.size() > limit ) {
        extract();
    }
}

void *RowSortedSet::extract()
{
    void *data;

    if( _heap.empty() )
        return 0;

    data = _heap[0];
    _heap[0] = _heap.back();
    _heap.pop_back();
    bubbleDown(); /* Enforce heap consistancy */

    return data;
}

int RowSortedSet::size()
{
    return _heap.size();
}
