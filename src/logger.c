// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "logger.h"
#include "nagios.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <syslog.h>

extern char g_logfile_path[];
extern int g_debug_level;
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
    pthread_t tid = pthread_self();

    /* Only the main process may use the Nagios log methods */
    if (g_mainthread_id == tid) {
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
            struct timeval tv;
            gettimeofday(&tv, NULL);
            struct tm now; localtime_r(&tv.tv_sec, &now);
            strftime(timestring, 64, "[%F %T", &now);
            fputs(timestring, g_logfile);
            if (g_debug_level > 0)
                snprintf(timestring, 64, ".%03ld][thr-%ld] ", tv.tv_usec/1000, tid);
            else
                snprintf(timestring, 64, ".%03ld] ", tv.tv_usec/1000);
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
