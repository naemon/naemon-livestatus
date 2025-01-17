// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "TableDownComm.h"
#include "TableHosts.h"
#include "TableServices.h"
#include "DowntimeOrComment.h"
#include "logger.h"
#include "Query.h"
#include "OffsetStringColumn.h"
#include "OffsetIntColumn.h"
#include "OffsetTimeColumn.h"
#include "auth.h"
#include "tables.h"
#include <pthread.h>
#include <string.h>

extern int g_eventloopstarted;

TableDownComm::TableDownComm(bool is_downtime)
{
    int err;
    char errmsg[256] = "unknown error";

    pthread_mutexattr_t attr;
    err = pthread_mutexattr_init(&attr);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error creating mutex attr: %s (%d)", errmsg, err);
   }

    err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error setting mutex type: %s (%d)", errmsg, err);
    }

    err = pthread_mutex_init(&_entries_mutex, &attr);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error creating mutex: %s (%d)", errmsg, err);
    }

    pthread_mutexattr_destroy(&attr);

    if (is_downtime)
        _name = "downtimes";
    else
        _name = "comments";

    DowntimeOrComment *ref = 0;
    addColumn(new OffsetStringColumn("author",
                is_downtime ? "The contact that scheduled the downtime" : "The contact that entered the comment",
                (char *)&(ref->_author_name) - (char *)ref));
    addColumn(new OffsetStringColumn("comment",
                "A comment text", (char *)&(ref->_comment) - (char *)ref));
    addColumn(new OffsetIntColumn("id",
                is_downtime ? "The id of the downtime" : "The id of the comment", (char *)&(ref->_id) - (char *)ref));
    addColumn(new OffsetTimeColumn("entry_time",
                "The time the entry was made as UNIX timestamp", (char *)&(ref->_entry_time) - (char *)ref));
    addColumn(new OffsetIntColumn("type",
                is_downtime ?  "The type of the downtime: 0 if it is active, 1 if it is pending" :
                "The type of the comment: 1 is host, 2 is service", (char *)&(ref->_type) - (char *)ref));
    addColumn(new OffsetIntColumn("is_service",
                "0, if this entry is for a host, 1 if it is for a service", (char *)&(ref->_is_service) - (char *)ref));

    if (is_downtime)
    {
        Downtime *ref = 0;
        addColumn(new OffsetTimeColumn("start_time",
                    "The start time of the downtime as UNIX timestamp", (char *)&(ref->_start_time) - (char *)ref));
        addColumn(new OffsetTimeColumn("end_time",
                    "The end time of the downtime as UNIX timestamp", (char *)&(ref->_end_time) - (char *)ref));
        addColumn(new OffsetIntColumn("fixed",
                    "A 1 if the downtime is fixed, a 0 if it is flexible", (char *)&(ref->_fixed) - (char *)ref));
        addColumn(new OffsetIntColumn("duration",
                    "The duration of the downtime in seconds", (char *)&(ref->_duration) - (char *)ref));
        addColumn(new OffsetIntColumn("triggered_by",
                    "The id of the downtime this downtime was triggered by or 0 if it was not triggered by another downtime",
                    (char *)&(ref->_triggered_by) - (char *)ref));
    }
    else {
        Comment *ref = 0;
        addColumn(new OffsetIntColumn("persistent",
                    "Whether this comment is persistent (0/1)", (char *)&(ref->_persistent) - (char *)ref));
        addColumn(new OffsetIntColumn("source",
                    "The source of the comment (0 is internal and 1 is external)", (char *)&(ref->_persistent) - (char *)ref));
        addColumn(new OffsetIntColumn("entry_type",
                    "The type of the comment: 1 is user, 2 is downtime, 3 is flap and 4 is acknowledgement", (char *)&(ref->_entry_type) - (char *)ref));
        addColumn(new OffsetIntColumn("expires",
                    "Whether this comment expires", (char *)&(ref->_expires) - (char *)ref));
        addColumn(new OffsetTimeColumn("expire_time",
                    "The time of expiry of this comment as a UNIX timestamp", (char *)&(ref->_expire_time) - (char *)ref));
    }

    g_table_hosts->addColumns(this, "host_",    (char *)&(ref->_host)    - (char *)ref);
    g_table_services->addColumns(this, "service_", (char *)&(ref->_service) - (char *)ref, false /* no hosts table */);
}

TableDownComm::~TableDownComm()
{
    char errmsg[256] = "unknown error";
    for (_entries_t::iterator it = _entries.begin();
            it != _entries.end();
            ++it)
    {
        delete it->second;
    }

    int err;
    err = pthread_mutex_destroy(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error destroying mutex: %s (%d)", errmsg, err);
    }
}

void TableDownComm::addComment(nebstruct_comment_data *data) {
    unsigned long id = data->comment_id;
    if (data->type == NEBTYPE_COMMENT_ADD || data->type == NEBTYPE_COMMENT_LOAD) {
        add(new Comment(data));
    }
    else if (data->type == NEBTYPE_COMMENT_DELETE) {
        remove(id);
    }
}


void TableDownComm::addDowntime(nebstruct_downtime_data *data)
{
    unsigned long id = data->downtime_id;
    if (data->type == NEBTYPE_DOWNTIME_ADD || data->type == NEBTYPE_DOWNTIME_LOAD) {
        add(new Downtime(data));
    }
    else if (data->type == NEBTYPE_DOWNTIME_DELETE) {
        remove(id);
    }
}


void TableDownComm::add(DowntimeOrComment *data)
{
    _entries_t::iterator it;

    int err;
    char errmsg[256] = "unknown error";
    err = pthread_mutex_lock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error locking mutex: %s (%d)", errmsg, err);
    }

    it = _entries.find(data->_id);
    // might be update -> delete previous data set
    if (it != _entries.end()) {
        delete it->second;
        _entries.erase(it);
    }
    _entries.insert(make_pair(data->_id, data));

    err = pthread_mutex_unlock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error unlocking mutex: %s (%d)", errmsg, err);
    }
}

void TableDownComm::remove(unsigned id)
{
    _entries_t::iterator it;

    int err;
    char errmsg[256] = "unknown error";
    err = pthread_mutex_lock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error locking mutex: %s (%d)", errmsg, err);
    }

    it = _entries.find(id);
    if (it == _entries.end()) {
        if(g_eventloopstarted)
            logger(LG_INFO, "Cannot delete non-existing downtime/comment %u", id);
    }
    else {
        delete it->second;
        _entries.erase(it);
    }

    err = pthread_mutex_unlock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error unlocking mutex: %s (%d)", errmsg, err);
    }
}

void TableDownComm::answerQuery(Query *query)
{
    int err;
    char errmsg[256] = "unknown error";
    err = pthread_mutex_lock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error locking mutex: %s (%d)", errmsg, err);
    }

    for (_entries_t::const_iterator it = _entries.begin();
            it != _entries.end();
            ++it)
    {
        if (!query->processDataset(it->second))
            break;
    }

    err = pthread_mutex_unlock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error unlocking mutex: %s (%d)", errmsg, err);
    }
}

bool TableDownComm::isAuthorized(contact *ctc, void *data)
{
    DowntimeOrComment *dtc = (DowntimeOrComment *)data;
    return is_authorized_for(ctc, dtc->_host, dtc->_service);
}

DowntimeOrComment *TableDownComm::findEntry(unsigned long id)
{
    /* This method is only used from the DownCommColumn, and therefore shouldn't
     * be possible to have any deadlocks or recursive mutexes-problems with.
     */
    DowntimeOrComment *res = NULL;

    int err;
    char errmsg[256] = "unknown error";
    err = pthread_mutex_lock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error locking mutex: %s (%d)", errmsg, err);
    }

    _entries_t::iterator it = _entries.find(id);
    if (it != _entries.end()) {
        res = it->second;
    }

    err = pthread_mutex_unlock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error unlocking mutex: %s (%d)", errmsg, err);
    }

    return res;
}

void TableDownComm::lock()
{
    int err;
    char errmsg[256] = "unknown error";
    err = pthread_mutex_lock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error locking mutex: %s (%d)", errmsg, err);
    }
}

void TableDownComm::unlock()
{
    int err;
    char errmsg[256] = "unknown error";
    err = pthread_mutex_unlock(&_entries_mutex);
    if(err) {
        strerror_r(err, errmsg, 256);
        logger(LG_INFO, "Error unlocking mutex: %s (%d)", errmsg, err);
    }
}
