// +------------------------------------------------------------------+
// |             ____ _               _        __  __ _  __           |
// |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
// |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
// |           | |___| | | |  __/ (__|   <    | |  | | . \            |
// |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
// |                                                                  |
// | Copyright Mathias Kettner 2012             mk@mathias-kettner.de |
// +------------------------------------------------------------------+
//
// This file is part of Check_MK.
// The official homepage is at http://mathias-kettner.de/check_mk.
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
