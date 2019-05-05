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
