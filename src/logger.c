// +------------------------------------------------------------------+
// |             ____ _               _        __  __ _  __           |
// |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
// |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
// |           | |___| | | |  __/ (__|   <    | |  | | . \            |
// |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
// |                                                                  |
// | Copyright Mathias Kettner 2012             mk@mathias-kettner.de |
// +------------------------------------------------------------------+
//
// This file is part of Check_MK.
// The official homepage is at http://mathias-kettner.de/check_mk.
//
// check_mk is free software;  you can redistribute it and/or modify it
// under the  terms of the  GNU General Public License  as published by
// the Free Software Foundation in version 2.  check_mk is  distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY;  with-
// out even the implied warranty of  MERCHANTABILITY  or  FITNESS FOR A
// PARTICULAR PURPOSE. See the  GNU General Public License for more de-
// ails.  You should have  received  a copy of the  GNU  General Public
// License along with GNU Make; see the file  COPYING.  If  not,  write
// to the Free Software Foundation, Inc., 51 Franklin St,  Fifth Floor,
// Boston, MA 02110-1301 USA.

#include "logger.h"
#include "nagios.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <syslog.h>

extern char g_logfile_path[];
pthread_t g_mainthread_id;

/* This protects the log file variable g_logfile to avoid concurrent writes as
 * well as writes-after-close */
static pthread_mutex_t g_log_file_mutex;

FILE *g_logfile = 0;

static void lock_mutex_or_die(pthread_mutex_t *mutex) {
    int status;

    if ((status = pthread_mutex_lock(mutex)) != 0) {
        fprintf(stderr, "Failed to lock mutex (%s). Aborting.", strerror(status));
        abort();
    }
}

static void unlock_mutex_or_die(pthread_mutex_t *mutex) {
    int status;

    if ((status = pthread_mutex_unlock(mutex)) != 0) {
        fprintf(stderr, "Failed to unlock mutex (%s). Aborting.", strerror(status));
        abort();
    }
}

void initialize_logger()
{
    /* Needed to determine main thread in logger() calls. */
    g_mainthread_id = pthread_self();
}

void open_logfile()
{
    int status;
    if (g_logfile)
        return;

    if ((status = pthread_mutex_init(&g_log_file_mutex, NULL)) != 0){
        /* Logging here is okay, under the assumption that we never try to open_logfile() from anywhere but the
         * main thread - meaning that the log message will go to the naemon log */
        logger(LG_ERR, "Failed to initialise log file mutex (%s). Aborting.", strerror(status));
        abort();
    }

    g_logfile = fopen(g_logfile_path, "a");
    if (!g_logfile)
        logger(LG_WARN, "Cannot open logfile %s: %s", g_logfile_path, strerror(errno));
}

void reopen_logfile()
{
    lock_mutex_or_die(&g_log_file_mutex);
    if(g_logfile)
        fclose(g_logfile);
    g_logfile = fopen(g_logfile_path, "a");
    if (!g_logfile)
        logger(LG_WARN, "Cannot open logfile %s: %s", g_logfile_path, strerror(errno));
    unlock_mutex_or_die(&g_log_file_mutex);
}

void close_logfile()
{
    lock_mutex_or_die(&g_log_file_mutex);
    if (g_logfile)
        fclose(g_logfile);

    g_logfile = 0;
    unlock_mutex_or_die(&g_log_file_mutex);
}

void logger(int priority, const char *loginfo, ...)
{
    va_list ap;
    va_start(ap, loginfo);

    /* Only the main process may use the Nagios log methods */
    if (g_mainthread_id == pthread_self()) {
        char buffer[8192];
        snprintf(buffer, 20, "livestatus: ");
        vsnprintf(buffer + strlen(buffer),
        sizeof(buffer) - strlen(buffer), loginfo, ap);
        va_end(ap);
        nm_log(priority, "%s", buffer);
    } else {
        lock_mutex_or_die(&g_log_file_mutex);
        if (g_logfile) {
            /* write date/time */
            char timestring[64];
            time_t now_t = time(0);
            struct tm now; localtime_r(&now_t, &now);
            strftime(timestring, 64, "%F %T ", &now);
            fputs(timestring, g_logfile);

            /* write log message */
            vfprintf(g_logfile, loginfo, ap);
            fputc('\n', g_logfile);
            fflush(g_logfile);
            va_end(ap);
        }
        unlock_mutex_or_die(&g_log_file_mutex);
    }
}
