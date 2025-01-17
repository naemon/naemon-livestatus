// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef opids_h
#define opids_h

#include "config.h"

#define OP_INVALID       0
#define OP_EQUAL         1 // =
#define OP_REGEX         2 // ~
#define OP_EQUAL_ICASE   3 // =~
#define OP_REGEX_ICASE   4 // ~~
#define OP_GREATER       5 // >
#define OP_LESS          6 // <

extern const char *op_names_plus_8[];

// Note: The operators !=, <= and >= are parsed into ! =, ! > and ! <.
// The negation is represented by negating the value of the operator.
// Example >= is represented as -6 (- OP_LESS)

#endif // opids_h
