// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <string.h>
#include <sys/time.h>

#include "Store.h"
#include "Query.h"
#include "logger.h"
#include "strutil.h"
#include "OutputBuffer.h"

#ifdef EXTERN
#undef EXTERN
#endif
#define EXTERN
#include "tables.h"
#undef EXTERN

extern int g_debug_level;
extern unsigned long g_max_cached_messages;
extern char *qh_socket_path;
extern Store *g_store;

Store::Store()
  : _log_cache(g_max_cached_messages)
  , _table_hosts(false)
  , _table_hostsbygroup(true)
  , _table_services(false, false)
  , _table_servicesbygroup(true, false)
  , _table_servicesbyhostgroup(false, true)
  , _table_downtimes(true)
  , _table_comments(false)
{
    _tables.insert(make_pair("hosts", &_table_hosts));
    _tables.insert(make_pair("hostsbygroup", &_table_hostsbygroup));
    _tables.insert(make_pair("services", &_table_services));
    _tables.insert(make_pair("servicesbygroup", &_table_servicesbygroup));
    _tables.insert(make_pair("servicesbyhostgroup", &_table_servicesbyhostgroup));
    _tables.insert(make_pair("hostgroups", &_table_hostgroups));
    _tables.insert(make_pair("servicegroups", &_table_servicegroups));
    _tables.insert(make_pair("contacts", &_table_contacts));
    _tables.insert(make_pair("commands", &_table_commands));
    _tables.insert(make_pair("downtimes", &_table_downtimes));
    _tables.insert(make_pair("comments", &_table_comments));
    _tables.insert(make_pair("status", &_table_status));
    _tables.insert(make_pair("log", &_table_log));
    _tables.insert(make_pair("statehist", &_table_statehistory));
    _tables.insert(make_pair("timeperiods", &_table_timeperiods));
    _tables.insert(make_pair("contactgroups", &_table_contactgroups));
    _tables.insert(make_pair("columns", &_table_columns));

    g_table_hosts = &_table_hosts;
    g_table_services = &_table_services;
    g_table_servicesbygroup = &_table_servicesbygroup;
    g_table_servicesbyhostgroup = &_table_servicesbyhostgroup;
    g_table_hostgroups = &_table_hostgroups;
    g_table_servicegroups = &_table_servicegroups;
    g_table_contacts = &_table_contacts;
    g_table_commands = &_table_commands;
    g_table_downtimes = &_table_downtimes;
    g_table_comments = &_table_comments;
    g_table_status = &_table_status;
    g_table_timeperiods = &_table_timeperiods;
    g_table_contactgroups = &_table_contactgroups;
    g_table_log = &_table_log;
    g_table_statehistory = &_table_statehistory;
    g_table_columns = &_table_columns;

    for (_tables_t::iterator it = _tables.begin();
            it != _tables.end();
            ++it)
    {
        _table_columns.addTable(it->second);
    }
}

Table *Store::findTable(string name)
{
    _tables_t::iterator it = _tables.find(name);
    if (it == _tables.end())
        return 0;
    else
        return it->second;
}


void Store::registerComment(nebstruct_comment_data *d)
{
    _table_comments.addComment(d);
}

void Store::registerDowntime(nebstruct_downtime_data *d)
{
    _table_downtimes.addDowntime(d);
}

bool Store::answerRequest(InputBuffer *input, OutputBuffer *output, int fd)
{
    output->reset();
    int r = input->readRequest();
    if (r != IB_REQUEST_READ) {
        if (r != IB_END_OF_FILE && r != IB_EMPTY_REQUEST)
            output->setError(RESPONSE_CODE_INCOMPLETE_REQUEST,
                "Client connection terminated while request still incomplete");
        return false;
    }
    string l = input->nextLine();
    const char *line = l.c_str();
    if (g_debug_level > 0)
        logger(LG_INFO, "Query: %s", line);
    if (!strncmp(line, "GET ", 4))
        answerGetRequest(input, output, lstrip((char *)line + 4), fd);
    else if (!strcmp(line, "GET"))
        answerGetRequest(input, output, "", fd); // only to get error message
    else if (!strncmp(line, "COMMAND ", 8)) {
        answerCommandRequest(unescape_newlines(lstrip((char *)line + 8)), output);
        output->setDoKeepalive(true);
    }
    else if (!strncmp(line, "LOGROTATE", 9)) {
        logger(LG_INFO, "Forcing logfile rotation");
        rotate_log_file(time(0));
    }
    else {
        logger(LG_INFO, "Invalid request '%s'", line);
        output->setError(RESPONSE_CODE_INVALID_REQUEST, "Invalid request method");
    }
    return output->doKeepalive();
}

void Store::answerCommandRequest(const char *command, OutputBuffer *output)
{
    int ret, sd;
    char buf[4096];
    sd = nsock_unix(qh_socket_path, NSOCK_TCP | NSOCK_CONNECT);
    if (sd < 0) {
        logger(LG_INFO, "Failed to connect to query socket '%s': %s: %s", qh_socket_path, nsock_strerror(sd), strerror(errno));
        return;
    }
    ret = nsock_printf_nul(sd, "#command run %s", command);
    if (ret < 0) {
        logger(LG_INFO, "failed to submit command by query handler");
    }
    output->reset();
    while(read(sd, buf, 4095) > 0) {
        int code = atoi(buf);
        if(code != 200) {
            logger(LG_INFO, "Unsuccessful command: '%s'", command);
            output->setError(RESPONSE_CODE_INVALID_REQUEST, "%s", rstrip(buf));
        }
    }
    close(sd);
    return;
}


void Store::answerGetRequest(InputBuffer *input, OutputBuffer *output, const char *tablename, int fd)
{
    bool logcacheLocked = false;
    output->reset();

    if (!tablename[0]) {
        output->setError(RESPONSE_CODE_INVALID_REQUEST, "Invalid GET request, missing tablename");
    }
    Table *table = findTable(tablename);
    if (!table) {
        output->setError(RESPONSE_CODE_NOT_FOUND, "Invalid GET request, no such table '%s'", tablename);
    }
    Query query(input, output, table);

    if(table && table->hasLogcache()) {
        g_store->logCache()->lockLogCache();
        // check if client is still connected, we might have waited too long for the lock
        if(!output->isAlive(fd)) {
            output->setError(RESPONSE_CODE_INCOMPLETE_REQUEST, "Client already disconnected");
            g_store->logCache()->unlockLogCache();
            return;
        }

        g_store->logCache()->logCachePreChecks();
        logcacheLocked = true;
    }

    if (table && !output->hasError()) {
        if (query.hasNoColumns()) {
            table->addAllColumnsToQuery(&query);
            query.setShowColumnHeaders(true);
        }
        struct timeval before, after;
        gettimeofday(&before, 0);
        query.start();
        table->answerQuery(&query);
        if (!output->hasError()) // crashes on stats queries which result in errors before
            query.finish();
        table->cleanupQuery(&query);
        gettimeofday(&after, 0);
        unsigned long ustime = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
        if (g_debug_level > 0)
            logger(LG_INFO, "Time to process request: %lu us. Size of answer: %d bytes", ustime, output->size());
    }

    if(logcacheLocked)
        g_store->logCache()->unlockLogCache();
}
