// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <stdlib.h>
#include "HostServiceState.h"

HostServiceState::HostServiceState()
    : _is_host(false)
    , _time(0)
    , _lineno(0)
    , _from(0)
    , _until(0)
    , _duration(0)
    , _duration_part(0)
    , _duration_state_UNMONITORED(0)
    , _duration_part_UNMONITORED(0)
    , _duration_state_OK(0)
    , _duration_part_OK(0)
    , _duration_state_WARNING(0)
    , _duration_part_WARNING(0)
    , _duration_state_CRITICAL(0)
    , _duration_part_CRITICAL(0)
    , _duration_state_UNKNOWN(0)
    , _duration_part_UNKNOWN(0)
    , _host_down(0)
    , _state(0)
    , _in_notification_period(0)
    , _in_service_period(0)
    , _in_downtime(0)
    , _in_host_downtime(0)
    , _is_flapping(0)
    , _may_no_longer_exist(false)
    , _has_vanished(false)
    , _last_known_time(0)
    , _host(NULL)
    , _service(NULL)
    , _log_output(0)
    , _notification_period(0)
    , _service_period(0) {}

HostServiceState::~HostServiceState()
{
    if (_log_output != 0)
        free(_log_output);
}
