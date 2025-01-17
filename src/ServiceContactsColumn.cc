// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "nagios.h"
#include "ServiceContactsColumn.h"
#include "Query.h"
#include <list>

bool ServiceContactsColumn::isNagiosMember(void *svc, void *ctc)
{
    return is_contact_for_service((service *)svc, (contact *)ctc);
}

void ServiceContactsColumn::output(void *data, Query *query)
{
    service *svc;
    data = shiftPointer(data);

    // create list of contacts by merging contacts and contact group members
    std::list<contactsmember*> result;
    if(data) {
        svc = (service *)data;
        contactsmember *cm = svc->contacts;
        while(cm) {
            contact *ctc = cm->contact_ptr;
            result.push_back(cm);
            cm = cm->next;
        }

        contactgroupsmember *cgm = svc->contact_groups;
        while(cgm) {
            contactsmember *cm = cgm->group_ptr->members;
            while(cm) {
                result.push_back(cm);
                cm = cm->next;
            }
            cgm = cgm->next;
        }
        result.sort(compare_contactsmember);
        result.unique(equals_contactsmember);
    }

    query->outputBeginList();
    bool first = true;
    for (std::list<contactsmember*>::iterator it = result.begin(); it != result.end(); ++it) {
        if (first)
            first = false;
        else
            query->outputListSeparator();
        query->outputString(((contactsmember*)*it)->contact_name);
    }
    query->outputEndList();
}

bool ServiceContactsColumn::isEmpty(void *data)
{
    service *svc;
    data = shiftPointer(data);
    if(data) {
        svc = (service *)data;
        if(svc->contacts != NULL || svc->contact_groups != NULL) {
            return false;
        }
    }
    return true;
}
