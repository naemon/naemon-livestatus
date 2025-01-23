// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "strutil.h"
#include "logger.h"
#include "Query.h"
#include "PerfdataAggregator.h"
#include "StringColumn.h"
#include "Aggregator.h"
#include "StatsColumn.h"

void PerfdataAggregator::consume(void *data, Query *query)
{
    char *perf_data = strdup(_column->getValue(data));
    char *scan = perf_data;

    char *entry;
    while (0 != (entry = next_field(&scan)))
    {
        char *start_of_varname = entry;
        char *place_of_equal = entry;
        while (*place_of_equal && *place_of_equal != '=')
            place_of_equal++;
        if (!*place_of_equal)
            continue; // ignore invalid perfdata
        *place_of_equal = 0; // terminate varname
        char *start_of_number = place_of_equal + 1;
        char *end_of_number = start_of_number;
        while (*end_of_number && (isdigit(*end_of_number) || *end_of_number == '.'))
            end_of_number ++;
        if (start_of_number == end_of_number)
            continue; // empty number
        *end_of_number = 0; // terminate number
        double value = strtod(start_of_number, 0);
        consumeVariable(start_of_varname, value);
    }

    free(perf_data);
}


void PerfdataAggregator::consumeVariable(const char *varname, double value)
{
    _aggr_t::iterator it = _aggr.find(varname);
    if (it == _aggr.end()) { // first entry
        perf_aggr new_entry;
        new_entry._aggr = value;
        new_entry._count = 1;
        new_entry._sumq = value * value;
        _aggr.insert(make_pair(std::string(varname), new_entry));
    }
    else {
        it->second._count ++;
        switch (_operation) {
            case STATS_OP_SUM:
            case STATS_OP_AVG:
                it->second._aggr += value;
                break;

            case STATS_OP_SUMINV:
            case STATS_OP_AVGINV:
                it->second._aggr += 1.0 / value;
                break;

            case STATS_OP_MIN:
                if (value < it->second._aggr)
                    it->second._aggr = value;
                break;

            case STATS_OP_MAX:
                if (value > it->second._aggr)
                    it->second._aggr = value;
                break;

            case STATS_OP_STD:
                it->second._aggr += value;
                it->second._sumq += value * value;
                break;
        }
    }

}

void PerfdataAggregator::output(Query *q)
{
    char format[64];
    string perf_data;
    for (_aggr_t::const_iterator it = _aggr.begin();
            it != _aggr.end();
            ++it)
    {
        double value;
        switch (_operation) {
            case STATS_OP_SUM:
            case STATS_OP_MIN:
            case STATS_OP_MAX:
            case STATS_OP_SUMINV:
                value = it->second._aggr;
                break;

            case STATS_OP_AVG:
            case STATS_OP_AVGINV:
                if (it->second._count == 0)
                    value = 0.00;
                else
                    value = it->second._aggr / it->second._count;
                break;

            case STATS_OP_STD:
                if (it->second._count <= 1)
                    value = 0.0;
                else
                    value = sqrt((it->second._sumq - (it->second._aggr * it->second._aggr) / it->second._count)/(it->second._count - 1));
                break;
        }
        snprintf(format, sizeof(format), "%s=%.8f", it->first.c_str(), value);
        if (it != _aggr.begin())
            perf_data += " ";
        perf_data += format;

    }
    q->outputString(perf_data.c_str());
}
