// Copyright (c) 2026 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef NumberFormat_h
#define NumberFormat_h

#include <cstdint>
#include <stdint.h>

// snprintf() spends time parsing its format and variadic arugments, which can be avoided
// snprintf() does not cache its (input + format) -> (output) . It is however a pure function, and can be cached

// Writes the decimal digits of value with the last digit at end[-1]
// Returns a pointer to the first digit.
// It writes the characters backwards starting from the end pointer, requires an adequately sized buffer and passing the *end correctly.
static inline char *formatUnsignedBackward(char *end, uint64_t value)
{
    do {
        *--end = (char)('0' + (value % 10));
        value /= 10;
    } while (value);
    return end;
}

#endif
