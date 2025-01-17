// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef HostServiceState_h
#define HostServiceState_h

#include <time.h>
#include <string.h>
#include <nagios.h>
#include <vector>
using namespace std;

struct HostServiceState;
typedef vector<HostServiceState*> HostServices;

typedef void* HostServiceKey;

struct HostServiceState {
    bool    _is_host;
    time_t  _time;
    int     _lineno;
    time_t  _from;
    time_t  _until;

    time_t  _duration;
    double  _duration_part;

    time_t  _duration_state_UNMONITORED;
    double  _duration_part_UNMONITORED;
    time_t  _duration_state_OK;
    double  _duration_part_OK;
    time_t  _duration_state_WARNING;
    double  _duration_part_WARNING;
    time_t  _duration_state_CRITICAL;
    double  _duration_part_CRITICAL;
    time_t  _duration_state_UNKNOWN;
    double  _duration_part_UNKNOWN;

    // State information
    int     _host_down;      // used if service
    int     _state;             // -1/0/1/2/3
    int     _in_notification_period;
    int     _in_service_period;
    int     _in_downtime;
    int     _in_host_downtime;
    int     _is_flapping;

    // Service information
    HostServices _services;

    // Absent state handling
    bool    _may_no_longer_exist;
    bool    _has_vanished;
    time_t  _last_known_time;

    const char  *_debug_info;

    // Pointer to dynamically allocated strings (strdup) that live here.
    // These pointers are 0, if there is no output (e.g. downtime)
    char        *_log_output;
    char        *_notification_period;  // may be "": -> no period known, we assume "always"
    char        *_service_period;  // may be "": -> no period known, we assume "always"
    host        *_host;
    service     *_service;
    const char  *_host_name;            // Fallback if host no longer exists
    const char  *_service_description;  // Fallback if service no longer exists

    HostServiceState();
    ~HostServiceState();
};

#endif // HostServiceState_h
