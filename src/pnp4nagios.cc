// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <string.h>
#include <unistd.h>

#include "pnp4nagios.h"

extern char g_pnp_path[];

void cleanup_pnpname(char *p)
{
    while (*p) {
        if (*p == ' ' || *p == '/' || *p == '\\' || *p == ':')
            *p = '_';
        p++;
    }
}

int pnpgraph_present(char *host, char *service)
{
    if (!g_pnp_path[0])
        return -1;

    char path[4096];
    size_t needed_size = strlen(g_pnp_path) + strlen(host) + 16;
    if (service)
        needed_size += strlen(service);
    if (needed_size > sizeof(path))
        return -1;

    strcpy(path, g_pnp_path);
    char *end = path + strlen(path);
    strcpy(end, host);
    cleanup_pnpname(end);
    strcat(end, "/");
    end = end + strlen(end);
    if (service) {
        strcat(end, service);
        cleanup_pnpname(end);
        strcat(end, ".xml");
    }
    else
        strcat(end, "_HOST_.xml");

    if (0 == access(path, R_OK))
        return 1;
    else
        return 0;
}
