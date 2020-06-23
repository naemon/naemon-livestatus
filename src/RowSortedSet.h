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

    void insert( void *data, std::vector<void*>::size_type limit ); // satisfy -Wsign-compare
    void *extract();

    int  size();
};


#endif // RowStorage_h

