// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef TableDownComm_h
#define TableDownComm_h

#include "config.h"

#include <map>
#include <pthread.h>
#include "Table.h"
#include "nagios.h"

class DowntimeOrComment;
class TableHosts;
class TableContacts;
class TableServices;

using namespace std;

class TableDownComm : public Table
{
    const char *_name;
    typedef map<unsigned long, DowntimeOrComment *> _entries_t;
    _entries_t _entries;
    pthread_mutex_t _entries_mutex;

public:
    TableDownComm(bool is_downtime);
    const char *name() { return _name; }
    ~TableDownComm();
    DowntimeOrComment *findEntry(unsigned long id);
    void addDowntime(nebstruct_downtime_data *);
    void addComment(nebstruct_comment_data *);
    void add(DowntimeOrComment *data);
    void remove(unsigned id);
    void answerQuery(Query *);
    bool isAuthorized(contact *ctc, void *data);
    _entries_t::iterator entriesIteratorBegin() { return _entries.begin(); }
    _entries_t::iterator entriesIteratorEnd() { return _entries.end(); }
    void lock();
    void unlock();
};


#endif // TableDownComm_h
