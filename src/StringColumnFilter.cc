// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "StringColumnFilter.h"
#include "StringColumn.h"
#include "opids.h"
#include "logger.h"
#include "OutputBuffer.h"
    StringColumnFilter::StringColumnFilter(StringColumn *column, int opid, char *value)
    : _column(column)
    , _ref_string(value)
    , _opid(abs(opid))
    , _negate(opid < 0)
#ifdef HAVE_ICU
    , _regex_matcher(0)
#else
    , _regex(0)
#endif
{
#ifdef HAVE_ICU
    UErrorCode status = U_ZERO_ERROR;
#endif
    if (_opid == OP_REGEX || _opid == OP_REGEX_ICASE) {
        if (strchr(value, '{') || strchr(value, '}')) {
            setError(RESPONSE_CODE_INVALID_HEADER, "disallowed regular expression '%s': must not contain { or }", value);
        }
        else {
#ifdef HAVE_ICU
            icu::UnicodeString s = icu::UnicodeString::fromUTF8(value);
            _regex_matcher = new icu::RegexMatcher(s, (_opid == OP_REGEX_ICASE ? UREGEX_CASE_INSENSITIVE : 0), status);
            if (U_FAILURE(status)) {
                setError(RESPONSE_CODE_INVALID_HEADER, "invalid regular expression '%s'", value);
                delete _regex_matcher;
                _regex_matcher = 0;
            }
#else
            _regex = new regex_t();
            if (0 != regcomp(_regex, value, REG_EXTENDED | REG_NOSUB | (_opid == OP_REGEX_ICASE ? REG_ICASE : 0)))
            {
                setError(RESPONSE_CODE_INVALID_HEADER, "invalid regular expression '%s'", value);
                delete _regex;
                _regex = 0;
            }
#endif
        }
    }
}

StringColumnFilter::~StringColumnFilter()
{
#ifdef HAVE_ICU
    if (_regex_matcher) {
        delete _regex_matcher;
    }
#else
    if (_regex) {
        regfree(_regex);
        delete _regex;
    }
#endif
}


bool StringColumnFilter::accepts(void *data)
{
    bool pass = true;
    const char *act_string = _column->getValue(data);
    switch (_opid) {
        case OP_EQUAL:
            pass = _ref_string == act_string; break;
        case OP_EQUAL_ICASE:
            pass = !strcasecmp(_ref_string.c_str(), act_string); break;
        case OP_REGEX:
        case OP_REGEX_ICASE:
#ifdef HAVE_ICU
            if ( _regex_matcher != 0 ) {
                icu::UnicodeString s = icu::UnicodeString::fromUTF8(act_string);
                _regex_matcher->reset(s);
                pass = _regex_matcher->find();
            }
            else {
                pass = false;
            }
#else
            pass = _regex != 0 && 0 == regexec(_regex, act_string, 0, 0, 0);
#endif
            break;
        case OP_GREATER:
            pass = 0 > strcmp(_ref_string.c_str(), act_string); break;
        case OP_LESS:
            pass = 0 < strcmp(_ref_string.c_str(), act_string); break;
        default:
            // this should never be reached, all operators are handled
            logger(LG_INFO, "Sorry. Operator %s for strings not implemented.", op_names_plus_8[_opid]);
            break;
    }
    return pass != _negate;
}

void *StringColumnFilter::indexFilter(const char *column)
{
    if (_opid == OP_EQUAL && !strcmp(column, _column->name()))
        return (void *)_ref_string.c_str();
    else
        return 0;
}
