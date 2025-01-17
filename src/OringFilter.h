// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OringFilter_h
#define OringFilter_h

#include "config.h"

#include "AndingFilter.h"

class OringFilter : public AndingFilter
{
public:
    bool accepts(void *data);
    bool optimizeBitmask(const char *columnname, uint32_t *mask);
};


#endif // OringFilter_h
