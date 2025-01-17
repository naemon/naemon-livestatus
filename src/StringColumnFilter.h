// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef StringColumnFilter_h
#define StringColumnFilter_h

#include "config.h"

#include <sys/types.h>
#ifdef HAVE_ICU
#include <unicode/regex.h>
#else
#include <regex.h>
#endif
#include <string>

using namespace std;

#include "Filter.h"
class StringColumn;

class StringColumnFilter : public Filter
{
    StringColumn *_column;
    string _ref_string;
    int _opid;
    bool _negate;
#ifdef HAVE_ICU
    icu::RegexMatcher *_regex_matcher;
#else
    regex_t *_regex;
#endif

public:
    StringColumnFilter(StringColumn *_column, int opid, char *value);
    ~StringColumnFilter();
    bool accepts(void *data);
    void *indexFilter(const char *column);
};


#endif // StringColumnFilter_h
