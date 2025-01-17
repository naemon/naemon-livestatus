// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "nagios.h"
#include "Query.h"
#include "OffsetStringColumn.h"
#include "OffsetIntColumn.h"
#include "OffsetTimeperiodColumn.h"
#include "TimeperiodDaysColumn.h"
#include "TimeperiodExclusionColumn.h"
#include "TimeperiodExceptionsColumn.h"
#include "TableTimeperiods.h"

TableTimeperiods::TableTimeperiods() {
    addColumns(this, "", -1);
}

void TableTimeperiods::addColumns(Table *table, string prefix,
        int indirect_offset) {
    timeperiod tp;
    char *ref = (char *) &tp;
    table->addColumn(
            new OffsetIntColumn(prefix + "id", "Timeperiod id",
                    (char *) (&tp.id) - ref, indirect_offset));
    table->addColumn(
            new OffsetStringColumn(prefix + "name",
                    "The name of the timeperiod", (char *) (&tp.name) - ref,
                    indirect_offset));
    table->addColumn(
            new OffsetStringColumn(prefix + "alias",
                    "The alias of the timeperiod", (char *) (&tp.alias) - ref,
                    indirect_offset));
    table->addColumn(
            new TimeperiodDaysColumn(prefix + "days",
                    "Time per days, as 7 lists (sun-sat) of a list of even number of elements, representing start,stop,start,stop... in seconds since midnight",
                    indirect_offset));

    table->addColumn(
            new TimeperiodExceptionsColumn(prefix + "exceptions_calendar_dates",
                    "List of objects representing exceptions for DATERANGE_CALENDAR_DATE. For time ranges, see \"days\" column.",
                    indirect_offset, DATERANGE_CALENDAR_DATE));
    table->addColumn(
            new TimeperiodExceptionsColumn(prefix + "exceptions_month_date",
                    "List of objects representing exceptions for DATERANGE_MONTH_DATE. For time ranges, see \"days\" column.",
                    indirect_offset, DATERANGE_MONTH_DATE));
    table->addColumn(
            new TimeperiodExceptionsColumn(prefix + "exceptions_month_day",
                    "List of objects representing exceptions for DATERANGE_MONTH_DAY. For time ranges, see \"days\" column.",
                    indirect_offset, DATERANGE_MONTH_DAY));
    table->addColumn(
            new TimeperiodExceptionsColumn(prefix + "exceptions_month_week_day",
                    "List of objects representing exceptions for DATERANGE_MONTH_WEEK_DAY. For time ranges, see \"days\" column.",
                    indirect_offset, DATERANGE_MONTH_WEEK_DAY));
    table->addColumn(
            new TimeperiodExceptionsColumn(prefix + "exceptions_week_day",
                    "List of objects representing exceptions for DATERANGE_WEEK_DAY. For time ranges, see \"days\" column.",
                    indirect_offset, DATERANGE_WEEK_DAY));

    table->addColumn(
            new TimeperiodExclusionColumn(prefix + "exclusions",
                    "Exclusions for this timeperiod",
                    (char *) (&tp.exclusions) - ref, indirect_offset));
    table->addColumn(
            new OffsetTimeperiodColumn(prefix + "in",
                    "Whether we are currently in this period (0/1)", -1,
                    indirect_offset));
}

void TableTimeperiods::answerQuery(Query *query) {
    timeperiod *tp = timeperiod_list;
    while (tp) {
        if (!query->processDataset(tp))
            break;
        tp = tp->next;
    }
}
