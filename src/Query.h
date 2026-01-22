// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef Query_h
#define Query_h

#include "config.h"
#include "nagios.h"

#include <stdio.h>
#include <string>
#include <map>
using namespace std;

#include "AndingFilter.h"
#include "global_counters.h"
#include "RowSortedSet.h"

class Table;
class Column;
class OutputBuffer;
class InputBuffer;
class StatsColumn;
class Aggregator;

#define OUTPUT_FORMAT_CSV          0
#define OUTPUT_FORMAT_JSON         1
#define OUTPUT_FORMAT_PYTHON       2
#define OUTPUT_FORMAT_WRAPPED_JSON 3

#define RESPONSE_HEADER_OFF     0
#define RESPONSE_HEADER_FIXED16 1
#define RESPONSE_HEADER_HTTP    2 // not yet implemented

#define ANDOR_OR     0
#define ANDOR_AND    1
#define ANDOR_NEGATE 2


class Query
{
    OutputBuffer *_output;
    Table        *_table;
    AndingFilter  _filter;
    contact      *_auth_user;
    AndingFilter  _wait_condition;
    unsigned      _wait_timeout;
    unsigned      _wait_trigger;
    void         *_wait_object;
    string        _field_separator;
    string        _dataset_separator;
    string        _list_separator;
    string        _host_service_separator;
    bool          _show_column_headers;
    bool          _need_ds_separator;
    int           _output_format;
    int           _limit;
    int           _time_limit;
    time_t        _time_limit_timeout;
    int           _offset;
    unsigned      _current_line;
    int           _timezone_offset;

    // normal queries
    typedef vector<Column *> _columns_t;
    _columns_t _columns;
    _columns_t _dummy_columns; // dynamically allocated. Must delete them.

    bool          _do_sorting;
    RowSortedSet  _sorter;

    // stats queries
    typedef vector<StatsColumn *> _stats_columns_t;
    _stats_columns_t _stats_columns; // must also delete
    Aggregator **_stats_aggregators;

    typedef vector<string> _stats_group_spec_t;
    typedef map<_stats_group_spec_t, Aggregator **> _stats_groups_t;
    _stats_groups_t _stats_groups;

public:
    void *table_tmp_storage; // Used for tables to store per query temporary data

    Query(InputBuffer *, OutputBuffer *out, Table *);
    ~Query();
    bool processDataset(void *);
    bool timelimitReached();
    void start();
    void finish();
    void setDefaultColumns(const char *);
    void addColumn(Column *column);
    void setShowColumnHeaders(bool x) { _show_column_headers = x; }
    void setError(int error_code, const char * msg);
    bool hasNoColumns();
    contact *authUser() { return _auth_user; }
    void outputDatasetBegin();
    void outputDatasetEnd();
    void outputFieldSeparator();
    void outputInteger(int32_t);
    void outputInteger64(int64_t);
    void outputTime(time_t);
    void outputTimeVal(timeval);
    void outputUnsignedLong(unsigned long);
    void outputCounter(counter_t);
    void outputDouble(double);
    void outputNull();
    void outputAsciiEscape(char value);
    void outputUnicodeEscape(unsigned value);
    void outputString(const char *);
    void outputBlob(const char *buffer, int size);
    void outputHostService(const char *, const char *);
    void outputBeginList();
    void outputListSeparator();
    void outputEndList();
    void outputBeginSublist();
    void outputSublistSeparator();
    void outputEndSublist();
    void outputBeginDict();
    void outputDictSeparator();
    void outputDictValueSeparator();
    void outputEndDict();
    void *findIndexFilter(const char *columnname);
    void *findTimerangeFilter(const char *columnname, time_t *, time_t *);
    void findTimeLimits(const char *columnname, time_t *lower, time_t *upper);
    void optimizeBitmask(const char *columnname, uint32_t *bitmask);
    int timezoneOffset() { return _timezone_offset; }
    AndingFilter *filter() { return &_filter; }

private:
    bool doStats();
    void doWait();
    Aggregator **getStatsGroup(_stats_group_spec_t &groupspec, bool *is_new);
    void computeStatsGroupSpec(_stats_group_spec_t &groupspec, void *data);
    Filter *createFilter(Column *column, int operator_id, char *value);
    void parseFilterLine(char *line, bool filter /* and not cond */);
    void parseSortLine(char *line);
    void parseStatsLine(char *line);
    void parseStatsGroupLine(char *line);
    void parseAndOrLine(char *line, int andor, bool filter /* and not cond */);
    void parseNegateLine(char *line, bool filter /* and not cond */);
    void parseStatsAndOrLine(char *line, int andor);
    void parseStatsNegateLine(char *line);
    void parseColumnsLine(char *line);
    void parseColumnHeadersLine(char *line);
    void parseLimitLine(char *line);
    void parseTimelimitLine(char *line);
    void parseOffsetLine(char *line);
    void parseSeparatorsLine(char *line);
    void parseOutputFormatLine(char *line);
    void parseKeepAliveLine(char *line);
    void parseResponseHeaderLine(char *line);
    void parseAuthUserHeader(char *line);
    void parseWaitTimeoutLine(char *line);
    void parseWaitTriggerLine(char *line);
    void parseWaitObjectLine(char *line);
    void parseLocaltimeLine(char *line);
    int lookupOperator(const char *opname);
    Column *createDummyColumn(const char *name);
    void printRow(void *data);
};


#endif // Query_h
