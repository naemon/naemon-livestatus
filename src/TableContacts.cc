// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "nagios.h"
#include "TableContacts.h"
#include "Query.h"
#include "OffsetStringColumn.h"
#include "OffsetIntColumn.h"
#include "OffsetTimeperiodColumn.h"
#include "AttributelistColumn.h"
#include "CustomVarsColumn.h"
#include "ContactgroupsObjectlistColumn.h"
#include "CommandsMemberColumn.h"

TableContacts::TableContacts()
{
    addColumns(this, "", -1);
}

void TableContacts::addColumns(Table *table, string prefix, int indirect_offset)
{
    contact ctc;
    char *ref = (char *)&ctc;
    table->addColumn(new OffsetIntColumn(prefix + "id",
                "Contact id", (char *)(&ctc.id) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "name",
                "The login name of the contact person", (char *)(&ctc.name) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "alias",
                "The full name of the contact", (char *)(&ctc.alias) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "email",
                "The email address of the contact", (char *)(&ctc.email) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "pager",
                "The pager address of the contact", (char *)(&ctc.pager) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "host_notification_period",
                "The time period in which the contact will be notified about host problems", (char *)(&ctc.host_notification_period) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "service_notification_period",
                "The time period in which the contact will be notified about service problems", (char *)(&ctc.service_notification_period) - ref, indirect_offset));
    for (int i=0; i<MAX_CONTACT_ADDRESSES; i++) {
        char b[32];
        snprintf(b, sizeof(b), "address%d", i+1);
        table->addColumn(new OffsetStringColumn(prefix + b,
                    (string("The additional field ") + b), (char *)(&ctc.address[i]) - ref, indirect_offset));
    }

    table->addColumn(new OffsetIntColumn(prefix + "can_submit_commands",
                "Whether the contact is allowed to submit commands (0/1)", (char *)&ctc.can_submit_commands - ref, indirect_offset));
    table->addColumn(new OffsetIntColumn(prefix + "host_notifications_enabled",
                "Whether the contact will be notified about host problems in general (0/1)", (char *)&ctc.host_notifications_enabled - ref, indirect_offset));
    table->addColumn(new OffsetIntColumn(prefix + "service_notifications_enabled",
                "Whether the contact will be notified about service problems in general (0/1)", (char *)&ctc.service_notifications_enabled - ref, indirect_offset));

    table->addColumn(new OffsetTimeperiodColumn(prefix + "in_host_notification_period",
                "Whether the contact is currently in his/her host notification period (0/1)", (char *)&ctc.host_notification_period_ptr - ref, indirect_offset));
    table->addColumn(new OffsetTimeperiodColumn(prefix + "in_service_notification_period",
                "Whether the contact is currently in his/her service notification period (0/1)", (char *)&ctc.service_notification_period_ptr - ref, indirect_offset));

    table->addColumn(new CustomVarsColumn(prefix + "custom_variable_names",
                "A list of all custom variables of the contact", (char *)(&ctc.custom_variables) - ref, indirect_offset, CVT_VARNAMES));
    table->addColumn(new CustomVarsColumn(prefix + "custom_variable_values",
                "A list of the values of all custom variables of the contact", (char *)(&ctc.custom_variables) - ref, indirect_offset, CVT_VALUES));
    table->addColumn(new CustomVarsColumn(prefix + "custom_variables",
                "A dictionary of the custom variables", (char *)(&ctc.custom_variables) - ref, indirect_offset, CVT_DICT));
    table->addColumn(new AttributelistColumn(prefix + "modified_attributes",
                "A bitmask specifying which attributes have been modified", (char *)(&ctc.modified_attributes) - ref, indirect_offset, false));
    table->addColumn(new AttributelistColumn(prefix + "modified_attributes_list",
                "A list of all modified attributes", (char *)(&ctc.modified_attributes) - ref, indirect_offset, true));
    table->addColumn(new ContactgroupsObjectlistColumn(prefix + "groups",
                "A list of all contact groups this contact is in", (char *)(&ctc.contactgroups_ptr) - ref, indirect_offset));
    table->addColumn(new CommandsMemberColumn(prefix + "host_notification_commands",
                "A list of all host notifications commands for this contact", (char *)(&ctc.host_notification_commands) - ref, indirect_offset));
    table->addColumn(new CommandsMemberColumn(prefix + "service_notification_commands",
                "A list of all service notifications commands for this contact", (char *)(&ctc.service_notification_commands) - ref, indirect_offset));

    table->clearNatSort();
    table->addNatSort( prefix + "name" );
}


void TableContacts::answerQuery(Query *query)
{
    contact *ct = contact_list;
    while (ct) {
        if (!query->processDataset(ct))
            break;
        ct = ct->next;
    }
}


void *TableContacts::findObject(char *objectspec)
{
    return find_contact(objectspec);
}
