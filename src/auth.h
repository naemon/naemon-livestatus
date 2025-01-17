// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef auth_h
#define auth_h

#include "nagios.h"

#define AUTH_LOOSE  0
#define AUTH_STRICT 1

// Dummy pointer for unknown user (not no user)
#define UNKNOWN_AUTH_USER ((contact *)0xdeadbeaf)


extern char g_hidden_custom_var_prefix[256];
extern int g_service_authorization;
extern int g_group_authorization;

#ifdef __cplusplus
extern "C" {
#endif
  int is_authorized_for(contact *ctc, host *host, service *svc);
#ifdef __cplusplus
}
#endif

#endif // auth_h
