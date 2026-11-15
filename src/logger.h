// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef logger_h
#define logger_h

#include "config.h"
#include "nagios.h"

/* internal severity tags, written as [I/W/D/E] prefix to the livestatus logfile;
 * the main-thread path is always logged to naemon as NSLOG_INFO_MESSAGE */
#define LG_INFO  0
#define LG_WARN  1
#define LG_ERR   2
#define LG_CRIT  3
#define LG_DEBUG 4
#define LG_ALERT 5

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
