// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <cstddef>

#define NSCORE
#include "nagios.h"
#include "Query.h"
#include "OffsetStringColumn.h"
#include "OffsetIntColumn.h"
#include "TableCommands.h"


TableCommands::TableCommands()
{
    addColumns(this, "", -1);
}


void TableCommands::addColumns(Table *table, string prefix, int indirect_offset)
{
    command cmd;
    char *ref = (char *)&cmd;
    table->addColumn(new OffsetIntColumn(prefix + "id",
                "Command id", (char *)(&cmd.id) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "name",
                "The name of the command", (char *)(&cmd.name) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "line",
                "The shell command line", (char *)(&cmd.command_line) - ref, indirect_offset));
}


void TableCommands::answerQuery(Query *query)
{
    command *cmd = command_list;
    while (cmd) {
        if (!query->processDataset(cmd)) break;
        cmd = cmd->next;
    }
}
