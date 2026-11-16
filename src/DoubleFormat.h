// Copyright (c) 2026 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef DoubleFormat_h
#define DoubleFormat_h

#include <cstdint>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Whole numbers in Floating Point Columns ------------------------------------
//
// Naemon data has floating point fields for objects, but those mostly take whole numbers, e.g : check_interval
// Whole numbers can be formatted in a shorter way
// The text output keeps a decimal point, and adds only one digit after the decimal point, e.g "60.0"
// Both Thruk and LMD work with this, and CSV exports are uneffected - they parse it as a floating point number still.


// Every integer with |v| <= 2^53 is exactly representable as a double, so its digits match with strtod().
// Everything else (fractions, NaN, Inf, larger magnitudes) falls through to the "%.10e" path.
#define DOUBLE_WHOLE_MAX 9007199254740992.0 // 2^53

// Writes the decimal digits of value with the last digit at end[-1], returns a pointer to the first digit.
static inline char *formatUnsignedBackward(char *end, uint64_t value)
{
    do {
        *--end = (char)('0' + (value % 10));
        value /= 10;
    } while (value);
    return end;
}

// Same as above but with a leading '-' for negative values.
static inline char *formatIntegerBackward(char *end, int64_t value)
{
    if (value < 0) {
        // INT64_MIN cannot be negated: add 1 before negating, add it back in unsigned arithmetic
        char *p = formatUnsignedBackward(end, (uint64_t)-(value + 1) + 1);
        *--p = '-';
        return p;
    }
    return formatUnsignedBackward(end, (uint64_t)value);
}

// Formats value as "<integer>.0" when it is a whole number in the exact range
// (e.g. "60.0"), returns false otherwise.
// Never writes more than 24 bytes.
static inline bool formatWholeNumber(char *buf, double value, int *len)
{
    // out of range, NaN or Inf
    if (!(value >= -DOUBLE_WHOLE_MAX && value <= DOUBLE_WHOLE_MAX)) {
        return false;
    }

    // no UB: |value| <= 2^53
    int64_t as_int = (int64_t)value;
    if ((double)as_int != value) {
        // has a fractional part
        return false;
    }

    char tmp[24];
    char *const end = tmp + sizeof(tmp);
    char *first;
    if (as_int == 0 && signbit(value)) {
        // -0.0 must keep its sign, "-0.0" round-trips to -0.0
        tmp[sizeof(tmp) - 2] = '-';
        tmp[sizeof(tmp) - 1] = '0';
        first = tmp + sizeof(tmp) - 2;
    } else {
        first = formatIntegerBackward(end, as_int);
    }
    // 16 digits plus sign plus the ".0" suffix fit in 24 bytes
    size_t n = (size_t)(end - first);
    memcpy(buf, first, n);
    buf[n++] = '.';
    buf[n++] = '0';
    *len = (int)n;
    return true;
}

// Formats a double for output: whole numbers through formatWholeNumber(), everything else with the
// exact "%.10e" format.  buf must hold at least 24 bytes, returns the number of bytes written.
static inline int formatDouble(char *buf, size_t size, double value)
{
    int len;
    if (size >= 24 && formatWholeNumber(buf, value, &len)) {
        return len;
    }
    int n = snprintf(buf, size, "%.10e", value);
    return n > 0 ? n : 0;
}

#endif
