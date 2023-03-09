#ifndef ContactgroupsObjectlistColumn_h
#define ContactgroupsObjectlistColumn_h

#include "config.h"

#include "ListColumn.h"
#include "nagios.h"

class ContactgroupsObjectlistColumn : public ListColumn
{
    int _offset;
public:
    ContactgroupsObjectlistColumn(string name, string description, int offset, int indirect_offset)
        : ListColumn(name, description, indirect_offset), _offset(offset) {}
    int type() { return COLTYPE_LIST; }
    void output(void *, Query *);
    void *getNagiosObject(char *name); // return pointer to host group
    bool isNagiosMember(void *data, void *nagobject);
    bool isEmpty(void *data);
private:
    objectlist *getData(void *);
};



#endif // ContactgroupsObjectlistColumn_h
