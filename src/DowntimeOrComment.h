// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef DowntimeOrComment_h
#define DowntimeOrComment_h

#include "config.h"

#include "nagios.h"
#include <string>
using namespace std;

/* The structs for downtime and comment are so similar, that
   we handle them with the same logic */




/*
   typedef struct nebstruct_downtime_struct{
   int             type;
   int             flags;
   int             attr;
   struct timeval  timestamp;

   int             downtime_type;
   char            *host_name;
   char            *service_description;
   time_t          entry_time;
   char            *author_name;
   char            *comment_data;
   time_t          start_time;
   time_t          end_time;
   int             fixed;
   unsigned long   duration;
   unsigned long   triggered_by;
   unsigned long   downtime_id;

   void            *object_ptr; // not implemented yet
   }nebstruct_downtime_data;

   typedef struct nebstruct_comment_struct{
   int             type;
   int             flags;
   int             attr;
   struct timeval  timestamp;

   int             comment_type;
   char            *host_name;
   char            *service_description;
   time_t          entry_time;
   char            *author_name;
   char            *comment_data;
   int             persistent;
   int             source;
   int             entry_type;
   int             expires;
   time_t          expire_time;
   unsigned long   comment_id;

   void            *object_ptr; // not implemented yet
   }nebstruct_comment_data;
 */

struct DowntimeOrComment
{
    int           _type;
    host         *_host;
    service      *_service;
    time_t        _entry_time;
    char *        _author_name;
    char*         _comment;
    unsigned long _id;
    int           _is_service;

    DowntimeOrComment(nebstruct_downtime_struct *data, unsigned long id);
    virtual ~DowntimeOrComment();
};

struct Downtime : public DowntimeOrComment
{
    time_t        _start_time;
    time_t        _end_time;
    int           _fixed;
    int           _duration;
    int           _triggered_by;
    Downtime(nebstruct_downtime_struct *data);
};

struct Comment : public DowntimeOrComment
{
    time_t        _expire_time;
    int           _persistent;
    int           _source;
    int           _entry_type;
    int           _expires;
    Comment(nebstruct_comment_struct *data);
};


#endif // Downtime_h
