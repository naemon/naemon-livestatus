// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef AttributelistColumn_h
#define AttributelistColumn_h

#include "config.h"

#include "IntColumn.h"
#include "nagios.h"

/* Since this column can be of type COLTYPE_INT, it must
   be a subclass of IntColumn, since StatsColumn assumes
   Columns of the type COLTYPE_INT to be of that type.
 */

class AttributelistColumn : public IntColumn
{
    int _offset;
    bool _show_list;
public:
    AttributelistColumn(string name, string description, int offset, int indirect_offset, bool show_list)
        : IntColumn(name, description, indirect_offset), _offset(offset), _show_list(show_list) {}

    /* API of Column */
    int type() { return _show_list ? COLTYPE_LIST : COLTYPE_INT; }
    virtual string valueAsString(void *data, Query *);
    void output(void *, Query *);
    Filter *createFilter(int opid, char *value);

    /* API of IntColumn */
    virtual int32_t getValue(void *data, Query *) { return getValue(data); }

    unsigned long getValue(void *data);
};



#endif // AttributelistColumn_h
