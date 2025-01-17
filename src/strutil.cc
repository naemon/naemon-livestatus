// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <string.h>
#include <ctype.h>

#include "strutil.h"

char *rstrip(char *c)
{
    char *w = c + strlen(c) - 1;
    while (w >= c && isspace(*w))
        *w-- = '\0';
    return c;
}

char *lstrip(char *c)
{
    while (isspace(*c)) c++;
    return c;
}

/* *c points to a string containing
   white space separated columns. This method returns
   a pointer to the zero-terminated next field. That
   might be identical with *c itself. The pointer c
   is then moved to the possible beginning of the
   next field. */
char *next_field(char **c)
{
    /* *c points to first character of field */
    char *begin = lstrip(*c); // skip leading spaces
    if (!*begin) {
        *c = begin;
        return 0; // found end of string -> no more field
    }

    char *end = begin; // copy pointer, search end of field
    while (*end && !isspace(*end)) end++;  // search for \0 or white space
    if (*end) { // string continues -> terminate field with '\0'
        *end = '\0';
        *c = end + 1; // skip to character right *after* '\0'
    }
    else
        *c = end; // no more field, point to '\0'
    return begin;
}

/* similar to next_field() but takes one character as delimiter */
char *next_token(char **c, char delim)
{
    char *begin = *c;
    if (!*begin) {
        *c = begin;
        return 0;
    }

    char *end = begin;
    while (*end && *end != delim) end++;
    if (*end) {
        *end = 0;
        *c = end + 1;
    }
    else
        *c = end;
    return begin;
}

/* same as next_token() but returns "" instead of 0 if
   no tokens has been found */
char *save_next_token(char **c, char delim)
{
    char *result = next_token(c, delim);
    if (result == 0)
        return (char *)"";
    else
        return result;
}


int ends_with(const char *a, const char *b)
{
    return !strcmp(a + strlen(a) - strlen(b), b);
}


/* unescapes newlines in a string */
char *unescape_newlines(char *rawbuf) {
  int x, y;

  for(x = 0, y = 0; rawbuf[x] != (char)'\x0'; x++) {

    if(rawbuf[x] == '\\') {

      /* unescape newlines */
      if(rawbuf[x + 1] == 'n') {
        rawbuf[y++] = '\n';
        x++;
        }

      /* unescape backslashes and other stuff */
      else if(rawbuf[x + 1] != '\x0') {
        rawbuf[y++] = rawbuf[x + 1];
        x++;
        }

      }
    else
      rawbuf[y++] = rawbuf[x];
    }
  rawbuf[y] = '\x0';

  return rawbuf;
  }
