// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef ContactsColumn_h
#define ContactsColumn_h

#include "config.h"
#include "nagios.h"

bool compare_contactsmember(const contactsmember *m1, const contactsmember *m2);
bool equals_contactsmember(const contactsmember *m1, const contactsmember *m2);

#include "ListColumn.h"
class TableContacts;

class ContactsColumn : public ListColumn
{
public:
    ContactsColumn(string name, string description, int indirect_offset)
        : ListColumn(name, description, indirect_offset)  {}
    int type() { return COLTYPE_LIST; }
    void *getNagiosObject(char *name);
    bool isEmpty(void *data);
    void output(void *, Query *);
};

#endif // ContactsColumn_h
