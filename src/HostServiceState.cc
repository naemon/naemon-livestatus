// +------------------------------------------------------------------+
// |             ____ _               _        __  __ _  __           |
// |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
// |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
// |           | |___| | | |  __/ (__|   <    | |  | | . \            |
// |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
// |                                                                  |
// | Copyright Mathias Kettner 2014             mk@mathias-kettner.de |
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
