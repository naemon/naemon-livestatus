// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef Filter_h
#define Filter_h

#include "config.h"

#include <vector>
#include <string>
#include <stdint.h>

using namespace std;

#define HOSTSERVICE_SEPARATOR '|'

class Query;
class Column;

class Filter
{
    string _error_message; // Error in constructor
    unsigned _error_code;
    Column *_column;

protected:
    Query *_query; // needed by TimeOffsetFilter (currently)
    void setError(unsigned code, const char *format, ...);

public:
    Filter() : _query(0), _column(0) {}
    virtual ~Filter() {}
    string errorMessage() { return _error_message; }
    unsigned errorCode() { return _error_code; }
    bool hasError() { return _error_message != ""; }
    void setQuery(Query *q) { _query = q; }
    void setColumn(Column *c) { _column = c; }
    Column *column() { return _column; }
    virtual bool accepts(void *data) = 0;
    virtual void *indexFilter(const char *columnname __attribute__ ((__unused__))) { return 0; }
    virtual void findTimeLimits(const char *columnname __attribute__ ((__unused__)), time_t *lower __attribute__ ((__unused__)), time_t *upper __attribute__ ((__unused__))) {}
    virtual bool optimizeBitmask(const char *columnname __attribute__ ((__unused__)), uint32_t *mask __attribute__ ((__unused__))) { return false; }
};

#endif // Filter_h
