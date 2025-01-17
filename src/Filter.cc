// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "Filter.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


void Filter::setError(unsigned code, const char *format, ...)
{
    char buffer[8192];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer) - 1, format, ap);
    va_end(ap);
    _error_message = buffer;
    _error_code = code;
}
