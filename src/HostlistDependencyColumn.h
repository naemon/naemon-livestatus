#ifndef HostlistDependencyColumn_h
#define HostlistDependencyColumn_h

#include "nagios.h"
#include "config.h"
#include "Column.h"

class HostlistDependencyColumn : public Column
{
    int  _offset;
    bool  _with_info;
public:
    HostlistDependencyColumn(string name, string description, int offset, int indirect_offset, bool with_info)
        : Column(name, description, indirect_offset), _offset(offset), _with_info(with_info) {}
    int type() { return COLTYPE_LIST; }
    void output(void *, Query *);
    objectlist *getList(void *data);
    Filter *createFilter(int opid, char *value);
};

#endif // HostlistDependencyColumn_h
