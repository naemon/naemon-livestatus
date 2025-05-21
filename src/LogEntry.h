// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef LogEntry_h
#define LogEntry_h

#include <stdint.h>

#define LOGCLASS_INFO              0 // all messages not in any other class
#define LOGCLASS_ALERT             1 // alerts: the change service/host state
#define LOGCLASS_PROGRAM           2 // important programm events (restart, ...)
#define LOGCLASS_NOTIFICATION      3 // host/service notifications
#define LOGCLASS_PASSIVECHECK      4 // passive checks
#define LOGCLASS_COMMAND           5 // external commands
#define LOGCLASS_STATE             6 // initial or current states
#define LOGCLASS_TEXT              7 // specific text passages. e.g "logging initial states"
#define LOGCLASS_INVALID          -1 // never stored
#define LOGCLASS_ALL          0xffff

#include "nagios.h"

enum LogEntryType {
    NONE                      = 0,
    ALERT_HOST                = 1,
    ALERT_SERVICE             = 2,
    DOWNTIME_ALERT_HOST       = 3,
    DOWNTIME_ALERT_SERVICE    = 4,
    STATE_HOST                = 5,
    STATE_HOST_INITIAL        = 6,
    STATE_SERVICE             = 7,
    STATE_SERVICE_INITIAL     = 8,
    FLAPPING_HOST             = 9,
    FLAPPING_SERVICE          = 10,
    TIMEPERIOD_TRANSITION     = 11,
    CORE_STARTING             = 12,
    CORE_STOPPING             = 13,
    LOG_VERSION               = 14,
    LOG_INITIAL_STATES        = 15,
    ACKNOWLEDGE_ALERT_HOST    = 16,
    ACKNOWLEDGE_ALERT_SERVICE = 17,
};

struct LogEntry
{
    uint64_t     _lineno;      // line number in file
    time_t       _time;
    unsigned     _logclass;
    LogEntryType _type;
    char        *_complete;  // copy of complete unsplit message
    char        *_options;   // points into _complete after ':'
    char        *_msg;       // split up with binary zeroes
    unsigned     _msglen;    // size of _msg
    char        *_text;      // points into msg
    char        *_host_name; // points into msg or is 0
    char        *_svc_desc;  // points into msg or is 0
    char        *_command_name;
    char        *_contact_name;
    int         _state;
    char        *_state_type;
    int         _attempt;
    char        *_check_output;
    char        *_comment;

    host        *_host;
    service     *_service;
    contact     *_contact;
    command     *_command;

    LogEntry(uint64_t lineno, char *line);
    ~LogEntry();
    uint64_t updateReferences();
    static int serviceStateToInt(char *s);
    static int hostStateToInt(char *s);

private:
    bool handleStatusEntry();
    bool handleStatusEntryBetter();
    bool handleNotificationEntry();
    bool handlePassiveCheckEntry();
    bool handleNotes();
    bool handleExternalCommandEntry();
    bool handleProgrammEntry();
    bool handleLogversionEntry();
    bool handleInfoEntry();
    bool handleTextEntry();
};

#endif // LogEntry_h
