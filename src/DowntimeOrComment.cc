// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "DowntimeOrComment.h"
#include "logger.h"
#include <string.h>

DowntimeOrComment::DowntimeOrComment(nebstruct_downtime_struct *dt,
        unsigned long id)
    : _type(dt->downtime_type)
    , _entry_time(dt->entry_time)
    , _author_name(strdup(dt->author_name))
    , _comment(strdup(dt->comment_data))
      , _id(id)
{
    _host = find_host(dt->host_name);
    if (dt->service_description) {
        _service = find_service(dt->host_name, dt->service_description);
        _is_service = 1;
    }
    else {
        _service = 0;
        _is_service = 0;
    }
}


DowntimeOrComment::~DowntimeOrComment()
{
    free(_author_name);
    free(_comment);
}


    Downtime::Downtime(nebstruct_downtime_struct *dt)
    : DowntimeOrComment(dt, dt->downtime_id)
    , _start_time(dt->start_time)
    , _end_time(dt->end_time)
    , _fixed(dt->fixed)
    , _duration(dt->duration)
      , _triggered_by(dt->triggered_by)
{
}

    Comment::Comment(nebstruct_comment_struct *co)
    : DowntimeOrComment((nebstruct_downtime_struct *)co, co->comment_id)
    , _expire_time(co->expire_time)
    , _persistent(co->persistent)
    , _source(co->source)
    , _entry_type(co->entry_type)
      , _expires(co->expires)
{
}
