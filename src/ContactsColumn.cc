// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "ContactsColumn.h"
#include "nagios.h"
#include "TableContacts.h"
#include "logger.h"
#include "Query.h"
#include "tables.h"
#include "nagios.h"

bool compare_contactsmember(const contactsmember *m1, const contactsmember *m2)
{
    return(m1->contact_name < m2->contact_name);
}

bool equals_contactsmember(const contactsmember *m1, const contactsmember *m2)
{
    return(m1->contact_ptr == m2->contact_ptr);
}

void *ContactsColumn::getNagiosObject(char *name)
{
    return (void *)find_contact(name);
}

void ContactsColumn::output(void *data, Query *query)
{
    query->outputBeginList();
    data = shiftPointer(data);

    if (data) {
        bool first = true;

        contact *ctc = contact_list;
        while (ctc) {
            if (isNagiosMember(data, ctc)) {
                if (first)
                    first = false;
                else
                    query->outputListSeparator();
                query->outputString(ctc->name);
            }
            ctc = ctc->next;
        }
    }
    query->outputEndList();
}


bool ContactsColumn::isEmpty(void *svc)
{
    contact *ct = contact_list;
    while (ct) {
        if (isNagiosMember(svc, ct))
            return false;
        ct = ct->next;
    }
    return true;
}
