// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef logger_h
#define logger_h

#include "config.h"
#include "nagios.h"

#define LG_INFO  NSLOG_INFO_MESSAGE
#define LG_WARN  LG_INFO
#define LG_ERR   LG_INFO
#define LG_CRIT  LG_INFO
#define LG_DEBUG LG_INFO
#define LG_ALERT LG_INFO

#ifdef __cplusplus
extern "C" {
#endif

void logger(int priority, const char *loginfo, ...);
void initialize_logger();
void open_logfile();
void close_logfile();
void reopen_logfile();

#ifdef __cplusplus
}
#endif

#endif // logger_h
