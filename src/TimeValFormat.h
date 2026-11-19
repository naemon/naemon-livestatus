// Copyright (c) 2026 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TimeValFormat_h
#define TimeValFormat_h

#include <string.h>
#include <time.h>
#include "NumberFormat.h"

// snprintf("%lu.%06lu") produces such a format for a normal timeval:
// "<seconds>.<microseconds zero padded to six digits>"
// This function formats it the same way, but faster without using snprintf()
// Writes at most 20 + 1 + 6 = 27 bytes at buf and returns the length.
// Only callers that agree on seconds >= 0 and 0 <= usec < 1000000 may use this
static inline int formatTimeVal(char *buf, time_t seconds, long usec)
{
    // time_t is platform dependent, but maximum of 64 bits -> max 20 chars in decimal form. use 8 * 3 = 24 for alignment
    char tmp_seconds[24];
    char *end = tmp_seconds + sizeof(tmp_seconds);
    char *p = formatUnsignedBackward(end, (uint64_t)seconds);
    size_t n = (size_t)(end - p);

    memcpy(buf, p, n);
    buf[n++] = '.';

    // Usec , microseconds takes a value between (0,1000000] . Has at most 6 digits
    // Uses for loop without breaks, therefore writes '0' as padding unconditionally.
    for (int i = 5; i >= 0; i--) {
        buf[n + i] = (char)('0' + (usec % 10));
        usec /= 10;
    }
    return (int)(n + 6);
}

#endif
