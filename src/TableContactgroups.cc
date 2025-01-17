// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "nagios.h"
#include "Query.h"
#include "OffsetStringColumn.h"
#include "OffsetIntColumn.h"
#include "TableContactgroups.h"
#include "ContactgroupsMemberColumn.h"

TableContactgroups::TableContactgroups()
{
    addColumns(this, "", -1);
}


void TableContactgroups::addColumns(Table *table, string prefix, int indirect_offset)
{
    contactgroup cg;
    char *ref = (char *)&cg;
    table->addColumn(new OffsetIntColumn(prefix + "id",
                "Contactgroup id", (char *)(&cg.id) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "name",
                "The name of the contactgroup", (char *)(&cg.group_name) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "alias",
                "The alias of the contactgroup", (char *)(&cg.alias) - ref, indirect_offset));
    table->addColumn(new ContactgroupsMemberColumn(prefix + "members",
                "A list of all members of this contactgroup", indirect_offset));
}


void TableContactgroups::answerQuery(Query *query)
{
    contactgroup *cg = contactgroup_list;
    while (cg) {
        if (!query->processDataset(cg)) break;
        cg = cg->next;
    }
}

void *TableContactgroups::findObject(char *objectspec)
{
    return find_contactgroup(objectspec);
}
