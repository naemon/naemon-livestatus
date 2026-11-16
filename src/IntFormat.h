// Copyright (c) 2026 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef IntFormat_h
#define IntFormat_h

#include "NumberFormat.h"

// Writes the decimal digits of value, and '-' only for negative values, with the last digit at end[-1]
// Returns a pointer to the first digit, or pointer to '-' char if the value is negative
// It writes the characters backwards starting from the end pointer, requires an adequately sized buffer and passing *end correctly
static inline char *formatIntegerBackward(char *end, int64_t value)
{
    if (value < 0) {
        // INT64_MIN cannot be negated, because its magnitude (absolute value) does not fit in an int64_t;
        // bit trick: add 1 before negating, then and add 1 back in unsigned arithmetic by casting to uint64_t
        // this produces the right magnitude (absolute value) for every input, positive or not.
        // then you can use formatUnsignedBackward
        char *p = formatUnsignedBackward(end, (uint64_t)-(value + 1) + 1);
        *--p = '-';
        return p;
    }

    return formatUnsignedBackward(end, (uint64_t)value);
}

#endif
