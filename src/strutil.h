// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef strutil_h
#define strutil_h

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif
    char *rstrip(char *);
    char *lstrip(char *);
    int ends_with(const char *a, const char *b);
#ifdef __cplusplus
    char *next_token(char **c, char delim = ',');
    char *save_next_token(char **c, char delim = ',');
#else
    char *next_token(char **c, char delim);
    char *save_next_token(char **c, char delim);
#endif
    char *next_field(char **line);
    char *unescape_newlines(char *);
#ifdef __cplusplus
}
#endif

#endif // strutil_h
