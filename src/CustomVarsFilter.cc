// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "CustomVarsFilter.h"
#include "opids.h"
#include "logger.h"
#include <ctype.h>
#include "OutputBuffer.h"
#include <strings.h>
#include <string.h>

	CustomVarsFilter::CustomVarsFilter(CustomVarsColumn *column, int opid, char *value)
    : _column(column)
    , _opid(abs(opid))
    , _negate(opid < 0)
    , _ref_text(value)
#ifdef HAVE_ICU
    , _regex_matcher(0)
#else
    , _regex(0)
#endif
{
    // Prepare part in case of DICT filter
    if (_column->type() == COLTYPE_DICT) {
        /* Filter for custom_variables:
Filter: custom_variables = PATH /hirni.mk

The variable name is part of the value and separated
with spaces
         */
        const char *cstr = _ref_text.c_str();
        const char *search_space = cstr;
#ifdef HAVE_ICU
        UErrorCode status = U_ZERO_ERROR;
#endif
        while (*search_space && !isspace(*search_space))
            search_space ++;
        _ref_varname = std::string(cstr, search_space - cstr);
        while (*search_space && isspace(*search_space))
            search_space ++;
        _ref_string = search_space;

        // Prepare regular expression
        if (_opid == OP_REGEX || _opid == OP_REGEX_ICASE) {
            if (strchr(search_space, '{') || strchr(search_space, '}')) {
                setError(RESPONSE_CODE_INVALID_HEADER, "disallowed regular expression '%s': must not contain { or }", value);
            }
            else {
#ifdef HAVE_ICU
                icu::UnicodeString s = icu::UnicodeString::fromUTF8(search_space);
                _regex_matcher = new icu::RegexMatcher(s, (_opid == OP_REGEX_ICASE ? UREGEX_CASE_INSENSITIVE : 0), status);
                if (U_FAILURE(status))
                {
                    setError(RESPONSE_CODE_INVALID_HEADER, "invalid regular expression '%s'", value);
                    delete _regex_matcher;
                    _regex_matcher = 0;
                }
#else
                _regex = new regex_t();
                if (0 != regcomp(_regex, search_space, REG_EXTENDED | REG_NOSUB | (_opid == OP_REGEX_ICASE ? REG_ICASE : 0)))
                {
                    setError(RESPONSE_CODE_INVALID_HEADER, "invalid regular expression '%s'", value);
                    delete _regex;
                    _regex = 0;
                }
#endif
            }
        }
    }
}

CustomVarsFilter::~CustomVarsFilter()
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

bool CustomVarsFilter::accepts(void *data)
{
    if (_column->type() == COLTYPE_DICT)
    {
        const char *act_string = _column->getVariable(data, _ref_varname.c_str());
        if (!act_string)
            act_string = "";

        bool pass = true;
        switch (_opid) {
            case OP_EQUAL:
                pass = _ref_string == act_string; break;
            case OP_EQUAL_ICASE:
                pass = !strcasecmp(_ref_string.c_str(), act_string); break;
            case OP_REGEX:
            case OP_REGEX_ICASE:
#ifdef HAVE_ICU
                if ( _regex_matcher != 0) {
                    icu::UnicodeString s = icu::UnicodeString::fromUTF8(act_string);
                    _regex_matcher->reset(s);
                    _regex_matcher->reset(act_string);
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
                logger(LG_INFO, "Sorry. Operator %d for strings not implemented.", _opid);
                break;
        }
        return pass != _negate;

    }
    else
    {
        bool is_member = _column->contains(data, _ref_text.c_str());
        switch (_opid) {
            case OP_LESS:
                return (!is_member) == (!_negate);
            default:
                logger(LG_INFO, "Sorry, Operator %s for custom variable lists not implemented.", op_names_plus_8[_opid]);
                return true;
        }
    }
}
