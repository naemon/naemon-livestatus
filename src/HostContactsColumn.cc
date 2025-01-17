// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "nagios.h"
#include "HostContactsColumn.h"
#include "Query.h"
#include <list>

bool HostContactsColumn::isNagiosMember(void *hst, void *ctc)
{
    return is_contact_for_host((host *)hst, (contact *)ctc);
}

void HostContactsColumn::output(void *data, Query *query)
{
    data = shiftPointer(data);
    host *hst = (host *)data;

    // create list of contacts by merging contacts and contact group members
    std::list<contactsmember*> result;
    if(hst) {
        contactsmember *cm = hst->contacts;
        while(cm) {
            contact *ctc = cm->contact_ptr;
            result.push_back(cm);
            cm = cm->next;
        }

        contactgroupsmember *cgm = hst->contact_groups;
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

bool HostContactsColumn::isEmpty(void *data)
{
    host *hst;
    data = shiftPointer(data);
    if(data) {
        hst = (host *)data;
        if(hst->contacts != NULL || hst->contact_groups != NULL) {
            return false;
        }
    }
    return true;
}
