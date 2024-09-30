#ifndef CommandsMemberColumn_h
#define CommandsMemberColumn_h

#include "config.h"

#include "ListColumn.h"
#include "nagios.h"


class CommandsMemberColumn : public ListColumn
{
    int _offset;
public:
    CommandsMemberColumn(string name, string description, int offset, int indirect_offset)
        : ListColumn(name, description, indirect_offset), _offset(offset) {}
    int type() { return COLTYPE_LIST; }
    void output(void *, Query *);
    void *getNagiosObject(char *name); // return pointer to host group
    bool isNagiosMember(void *data, void *nagobject);
    bool isEmpty(void *data);
private:
    commandsmember *getData(void *);
};

#endif // CommandsMemberColumn_h
