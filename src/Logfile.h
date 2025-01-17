// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef Logfile_h
#define Logfile_h

#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <map>

using namespace std;

#define MAX_LOGLINE 65536

class LogEntry;
class Query;
class LogCache;

typedef map<uint64_t, LogEntry *> logfile_entries_t; // key is time_t . lineno

class Logfile
{
private:
    char      *_path;
    time_t     _since;         // time of first entry
    time_t     _end;           // time of last entry
    bool       _watch;         // true only for current logfile
    ino_t      _inode;         // needed to detect switching
    fpos_t     _read_pos;      // read until this position
    uint32_t   _lineno;        // read until this line

    logfile_entries_t  _entries;
    char       _linebuffer[MAX_LOGLINE];


public:
    Logfile(const char *path, bool watch);
    ~Logfile();

    char *path() { return _path; }
    char *readIntoBuffer(int *size);
    void load(LogCache *LogCache, time_t since, time_t until, unsigned logclasses);
    void flush();
    time_t since() { return _since; }
    time_t end();
    bool watch() { return _watch; }
    unsigned classesRead() { return _logclasses_read; }
    long numEntries() { return _entries.size(); }
    logfile_entries_t* getEntriesFromQuery(Query *query, LogCache *lc, time_t since, time_t until, unsigned);
    bool answerQuery(Query *query, LogCache *lc, time_t since, time_t until, unsigned);
    bool answerQueryReverse(Query *query, LogCache *lc, time_t since, time_t until, unsigned);

    long freeMessages(unsigned logclasses);

    unsigned   _logclasses_read; // only these types have been read


private:
    void loadRange(FILE *file, unsigned missing_types, LogCache *,
                   time_t since, time_t until, unsigned logclasses);
    bool processLogLine(uint32_t, unsigned);
    uint64_t makeKey(time_t, unsigned);
};


#endif // Logfile_h
