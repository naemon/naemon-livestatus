// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef tables_h
#define tables_h

#ifndef EXTERN
#define EXTERN extern
#endif

class TableContacts;
EXTERN TableContacts      *g_table_contacts;
class TableCommands;
EXTERN TableCommands      *g_table_commands;
class TableHosts;
EXTERN TableHosts         *g_table_hosts;
EXTERN TableHosts         *g_table_hostsbygroup;
class TableServices;
EXTERN TableServices      *g_table_services;
EXTERN TableServices      *g_table_servicesbygroup;
EXTERN TableServices      *g_table_servicesbyhostgroup;
class TableHostgroups;
EXTERN TableHostgroups    *g_table_hostgroups;
class TableServicegroups;
EXTERN TableServicegroups *g_table_servicegroups;
class TableDownComm;
EXTERN TableDownComm      *g_table_downtimes;
EXTERN TableDownComm      *g_table_comments;
class TableTimeperiods;
EXTERN TableTimeperiods   *g_table_timeperiods;
class TableContactgroups;
EXTERN TableContactgroups *g_table_contactgroups;
class TableStatus;
EXTERN TableStatus        *g_table_status;
class TableLog;
EXTERN TableLog           *g_table_log;
class TableStateHistory;
EXTERN TableStateHistory  *g_table_statehistory;
class TableColumns;
EXTERN TableColumns       *g_table_columns;

#endif // tables_h
