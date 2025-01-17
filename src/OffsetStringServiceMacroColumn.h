// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef OffsetStringServiceMacroColumn_h
#define OffsetStringServiceMacroColumn_h

#include "nagios.h"
#include "OffsetStringMacroColumn.h"

class OffsetStringServiceMacroColumn : public OffsetStringMacroColumn
{
public:
    OffsetStringServiceMacroColumn(string name, string description, int offset, int indirect_offset = -1) :
        OffsetStringMacroColumn(name, description, offset, indirect_offset) {}
    host *getHost(void *data);
    service *getService(void *data);
};

#endif // OffsetStringServiceMacroColumn_h
