// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef CustomVarsFilter_h
#define CustomVarsFilter_h

#include "config.h"

#include "CustomVarsColumn.h"
#include "Filter.h"
#ifdef HAVE_ICU
#include <unicode/regex.h>
#else
#include <regex.h>
#endif

class CustomVarsFilter : public Filter
{
    CustomVarsColumn *_column;
    int _opid;
    bool _negate;
    string _ref_text;
#ifdef HAVE_ICU
    icu::RegexMatcher * _regex_matcher;
#else
    regex_t *_regex;
#endif
    // needed in case of COLTYPE_DICT
    string _ref_string;
    string _ref_varname;

public:
    CustomVarsFilter(CustomVarsColumn *column, int opid, char *value);
    ~CustomVarsFilter();
    bool accepts(void *data);
};

#endif // CustomVarsFilter_h
