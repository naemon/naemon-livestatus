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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>
#include <pthread.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>

#include "nagios.h"
#include "livestatus.h"
#include "store.h"
#include "logger.h"
#include "config.h"
#include "global_counters.h"
#include "strutil.h"
#include "auth.h"
#include "data_encoding.h"
#include "waittriggers.h"

#ifndef AF_LOCAL
#define   AF_LOCAL AF_UNIX
#endif
#ifndef PF_LOCAL
#define   PF_LOCAL PF_UNIX
#endif

// usually - but not always - in sys/un.h
#ifndef SUN_LEN
# define SUN_LEN(ptr) ((size_t) (((struct sockaddr_un *) 0)->sun_path) + strlen ((ptr)->sun_path))
#endif

NEB_API_VERSION(CURRENT_NEB_API_VERSION)
extern unsigned long event_broker_options;
extern char *log_file;

int g_idle_timeout_msec = 300 * 1000; /* maximum idle time for connection in keep alive state */
int g_query_timeout_msec = 10 * 1000;      /* maximum time for reading a query */

size_t g_thread_stack_size = 65536; /* stack size of threads */
pthread_t **g_client_threads;
size_t g_num_client_threads;

#define false 0
#define true 1

void *g_nagios_handle;
int g_unix_socket = -1;
char g_socket_path[4096];
char g_pnp_path[4096];
char g_logfile_path[4096];
int g_debug_level = 0;
int g_should_terminate = false;
unsigned long g_max_cached_messages = 500000;
unsigned long g_max_lines_per_logfile = 1000000; // do never read more than that number of lines from a logfile
unsigned long g_max_response_size = 100 * 1024 * 1024; // limit answer to 10 MB
char g_hidden_custom_var_prefix[256];
int g_service_authorization = AUTH_LOOSE;
int g_group_authorization = AUTH_STRICT;
int g_data_encoding = ENCODING_UTF8;

void *client_thread(void *data __attribute__ ((__unused__)));

static void reap_client_threads()
{
    size_t i = 0;
    while (i < g_num_client_threads) {
        if (0 != pthread_tryjoin_np(*(g_client_threads[i]), NULL)) {
            ++i;
            continue;
        }

        free(g_client_threads[i]);
        /* fill the gap with last element (the order of the threads doesn't
         * matter) */
        g_client_threads[i] = g_client_threads[--g_num_client_threads];

        if ((g_client_threads = realloc(g_client_threads, g_num_client_threads * sizeof(pthread_t))) == NULL) {
            if (g_num_client_threads > 0) {
                logger(LG_ERR, "Failed to shrink client thread array (Number of threads: %d)", g_num_client_threads);
            }
            else if (g_debug_level >= 2) {
                logger(LG_DEBUG, "All client threads reaped.");
            }
        }
    }
}

static int accept_connection(int sd, int events, void *discard)
{
    int cc = accept(sd, NULL, NULL);
    int ret = 0;
    pthread_t *thr;
    pthread_attr_t attr;
    do_statistics();
    pthread_attr_init(&attr);
    size_t defsize;

    reap_client_threads(); /* try to join any finished client threads */

    if (g_debug_level >= 2 && 0 == pthread_attr_getstacksize(&attr, &defsize))
      logger(LG_INFO, "Default stack size is %lu", defsize);
    if (0 != pthread_attr_setstacksize(&attr, g_thread_stack_size))
      logger(LG_INFO, "Error: Cannot set thread stack size to %lu", g_thread_stack_size);
    else {
      if (g_debug_level >= 2)
        logger(LG_INFO, "Setting thread stack size to %lu", g_thread_stack_size);
    }

    int *arg = malloc(sizeof(int));
    *arg = cc;

    thr = malloc(sizeof(pthread_t));
    if (thr == NULL) {
        /* Just log, and do nothing else, since the pthread_create will abort
         * anyways */
        logger(LG_ERR, "Failed to allocate memory for client thread ID: %s",
                strerror(errno));
    }

    if ((ret = pthread_create(thr, &attr, client_thread, arg)) != 0) {
        logger(LG_ERR, "Failed to create client thread: %s", strerror(ret));
        free(arg);
        close(cc);
    }
    pthread_attr_destroy(&attr);

    if ((g_client_threads = realloc(g_client_threads, (g_num_client_threads + 1) * sizeof(pthread_t))) == NULL) {
        logger(LG_ERR, "Failed to allocate space for client thread: %s (Number of threads: %d)", strerror(errno), g_num_client_threads + 1);
        close(cc);
        logger(LG_ERR, "Joining with client thread right away to avoid losing it.");
        if (0 != pthread_join(*thr, NULL)) {
            logger(LG_CRIT, "Failed to join with client thread: %s", strerror(errno));
        }
        free(thr);
    }
    else {
        ++g_num_client_threads;
        g_client_threads[g_num_client_threads-1] = thr;
    }

    g_counters[COUNTER_CONNECTIONS]++;
    return 0;
}

void *client_thread(void *data)
{
    void *input_buffer = create_inputbuffer(&g_should_terminate);
    void *output_buffer = create_outputbuffer();

    int cc = *((int *)data);
    free(data);
    if (cc >= 0) {
        if (g_debug_level >= 2)
            logger(LG_INFO, "Accepted client connection on fd %d", cc);
        set_inputbuffer_fd(input_buffer, cc);
        int keepalive = 1;
        unsigned requestnr = 1;
        while (keepalive && !g_should_terminate) {
            if (g_debug_level >= 2 && requestnr > 1)
                logger(LG_INFO, "Handling request %d on same connection", requestnr);
            keepalive = store_answer_request(input_buffer, output_buffer);
            flush_output_buffer(output_buffer, cc, &g_should_terminate);
            g_counters[COUNTER_REQUESTS]++;
            requestnr ++;
        }
        close(cc);
    }
    delete_outputbuffer(output_buffer);
    delete_inputbuffer(input_buffer);
    return NULL;
}

int open_unix_socket()
{
    struct stat st;
    if (0 == stat(g_socket_path, &st)) {
        if (0 == unlink(g_socket_path)) {
            logger(LG_DEBUG , "Removed old left over socket file %s", g_socket_path);
        }
        else {
            logger(LG_ALERT, "Cannot remove in the way file %s: %s",
                    g_socket_path, strerror(errno));
            return false;
        }
    }

    g_unix_socket = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (g_unix_socket < 0)
    {
        logger(LG_CRIT , "Unable to create UNIX socket: %s", strerror(errno));
        return false;
    }

    // Imortant: close on exec -> check plugins must not inherit it!
    if (0 < fcntl(g_unix_socket, F_SETFD, FD_CLOEXEC))
        logger(LG_INFO, "Cannot set FD_CLOEXEC on socket: %s", strerror(errno));

    // Bind it to its address. This creates the file with the name g_socket_path
    struct sockaddr_un sockaddr;
    sockaddr.sun_family = AF_LOCAL;
    strncpy(sockaddr.sun_path, g_socket_path, sizeof(sockaddr.sun_path));
    if (bind(g_unix_socket, (struct sockaddr *) &sockaddr, SUN_LEN(&sockaddr)) < 0)
    {
        logger(LG_ERR , "Unable to bind adress %s to UNIX socket: %s",
                g_socket_path, strerror(errno));
        close(g_unix_socket);
        return false;
    }

    // Make writable group members
    if (0 != chmod(g_socket_path, 0660)) {
        logger(LG_ERR , "Cannot chown unix socket at %s to 0660: %s", g_socket_path, strerror(errno));
        close(g_unix_socket);
        return false;
    }

    if (0 != listen(g_unix_socket, 3 /* backlog */)) {
        logger(LG_ERR , "Cannot listen to unix socket at %s: %s", g_socket_path, strerror(errno));
        close(g_unix_socket);
        return false;
    }

    if (g_debug_level > 0)
        logger(LG_INFO, "Opened UNIX socket %s\n", g_socket_path);
    return true;

}

void close_unix_socket()
{
    unlink(g_socket_path);
    iobroker_close(nagios_iobs, g_unix_socket);
    g_unix_socket = -1;
}

int broker_host(int event_type __attribute__ ((__unused__)), void *data __attribute__ ((__unused__)))
{
    g_counters[COUNTER_NEB_CALLBACKS]++;
    return 0;
}


int broker_check(int event_type, void *data)
{
    int result = NEB_OK;
    if (event_type == NEBCALLBACK_SERVICE_CHECK_DATA) {
        nebstruct_service_check_data *c = (nebstruct_service_check_data *)data;
        if (c->type == NEBTYPE_SERVICECHECK_PROCESSED) {
            g_counters[COUNTER_SERVICE_CHECKS]++;
        }
    }
    else if (event_type == NEBCALLBACK_HOST_CHECK_DATA) {
        nebstruct_host_check_data *c = (nebstruct_host_check_data *)data;
        if (c->type == NEBTYPE_HOSTCHECK_PROCESSED) {
            g_counters[COUNTER_HOST_CHECKS]++;
        }
    }
    pthread_cond_broadcast(&g_wait_cond[WT_ALL]);
    pthread_cond_broadcast(&g_wait_cond[WT_CHECK]);
    return result;
}


int broker_comment(int event_type __attribute__ ((__unused__)), void *data)
{
    nebstruct_comment_data *co = (nebstruct_comment_data *)data;
    store_register_comment(co);
    g_counters[COUNTER_NEB_CALLBACKS]++;
    pthread_cond_broadcast(&g_wait_cond[WT_ALL]);
    pthread_cond_broadcast(&g_wait_cond[WT_COMMENT]);
    return 0;
}

int broker_downtime(int event_type __attribute__ ((__unused__)), void *data)
{
    nebstruct_downtime_data *dt = (nebstruct_downtime_data *)data;
    store_register_downtime(dt);
    g_counters[COUNTER_NEB_CALLBACKS]++;
    pthread_cond_broadcast(&g_wait_cond[WT_ALL]);
    pthread_cond_broadcast(&g_wait_cond[WT_DOWNTIME]);
    return 0;
}

int broker_log(int event_type __attribute__ ((__unused__)), void *data __attribute__ ((__unused__)))
{
    g_counters[COUNTER_NEB_CALLBACKS]++;
    g_counters[COUNTER_LOG_MESSAGES]++;
    pthread_cond_broadcast(&g_wait_cond[WT_ALL]);
    pthread_cond_broadcast(&g_wait_cond[WT_LOG]);
    return 0;
}


int broker_command(int event_type __attribute__ ((__unused__)), void *data)
{
    nebstruct_external_command_data *sc = (nebstruct_external_command_data *)data;
    if (sc->type == NEBTYPE_EXTERNALCOMMAND_START)
        g_counters[COUNTER_COMMANDS]++;
    g_counters[COUNTER_NEB_CALLBACKS]++;
    pthread_cond_broadcast(&g_wait_cond[WT_ALL]);
    pthread_cond_broadcast(&g_wait_cond[WT_COMMAND]);
    return 0;
}

int broker_state(int event_type __attribute__ ((__unused__)), void *data __attribute__ ((__unused__)))
{
    g_counters[COUNTER_NEB_CALLBACKS]++;
    pthread_cond_broadcast(&g_wait_cond[WT_ALL]);
    pthread_cond_broadcast(&g_wait_cond[WT_STATE]);
    return 0;
}

int broker_program(int event_type __attribute__ ((__unused__)), void *data __attribute__ ((__unused__)))
{
    g_counters[COUNTER_NEB_CALLBACKS]++;
    pthread_cond_broadcast(&g_wait_cond[WT_ALL]);
    pthread_cond_broadcast(&g_wait_cond[WT_PROGRAM]);
    return 0;
}

static void broker_event(struct nm_event_execution_properties *evprop)
{
    g_counters[COUNTER_NEB_CALLBACKS]++;
    update_timeperiods_cache(time(0));
    if(!sigshutdown && !sigrestart) {
        schedule_event(1, broker_event, NULL);
    }
    return;
}

int broker_process(int event_type __attribute__ ((__unused__)), void *data)
{
    int ret;
    struct nebstruct_process_struct *ps = (struct nebstruct_process_struct *)data;
    if (ps->type == NEBTYPE_PROCESS_EVENTLOOPSTART) {
        update_timeperiods_cache(time(0));
        do_statistics();
        if (0 != (ret = iobroker_register(nagios_iobs, g_unix_socket, NULL, accept_connection))) {
            logger(LG_ERR, "Cannot register unix socket with Naemon listener: %s", iobroker_strerror(ret));
            close(g_unix_socket);
            g_unix_socket = -1;
            return ERROR;
        }
    }
    return OK;
}




int verify_event_broker_options()
{
    int errors = 0;
    if (!(event_broker_options & BROKER_PROGRAM_STATE)) {
        logger( LG_CRIT, "need BROKER_PROGRAM_STATE (%i) event_broker_option enabled to work.", BROKER_PROGRAM_STATE );
        errors++;
    }
    if (!(event_broker_options & BROKER_TIMED_EVENTS)) {
        logger( LG_CRIT, "need BROKER_TIMED_EVENTS (%i) event_broker_option enabled to work.", BROKER_TIMED_EVENTS );
        errors++;
    }
    if (!(event_broker_options & BROKER_SERVICE_CHECKS)) {
        logger( LG_CRIT, "need BROKER_SERVICE_CHECKS (%i) event_broker_option enabled to work.", BROKER_SERVICE_CHECKS );
        errors++;
    }
    if (!(event_broker_options & BROKER_HOST_CHECKS)) {
        logger( LG_CRIT, "need BROKER_HOST_CHECKS (%i) event_broker_option enabled to work.", BROKER_HOST_CHECKS );
        errors++;
    }
    if (!(event_broker_options & BROKER_LOGGED_DATA)) {
        logger( LG_CRIT, "need BROKER_LOGGED_DATA (%i) event_broker_option enabled to work.", BROKER_LOGGED_DATA );
        errors++;
    }
    if (!(event_broker_options & BROKER_COMMENT_DATA)) {
        logger( LG_CRIT, "need BROKER_COMMENT_DATA (%i) event_broker_option enabled to work.", BROKER_COMMENT_DATA );
        errors++;
    }
    if (!(event_broker_options & BROKER_DOWNTIME_DATA)) {
        logger( LG_CRIT, "need BROKER_DOWNTIME_DATA (%i) event_broker_option enabled to work.", BROKER_DOWNTIME_DATA );
        errors++;
    }
    if (!(event_broker_options & BROKER_STATUS_DATA)) {
        logger( LG_CRIT, "need BROKER_STATUS_DATA (%i) event_broker_option enabled to work.", BROKER_STATUS_DATA );
        errors++;
    }
    if (!(event_broker_options & BROKER_ADAPTIVE_DATA)) {
        logger( LG_CRIT, "need BROKER_ADAPTIVE_DATA (%i) event_broker_option enabled to work.", BROKER_ADAPTIVE_DATA );
        errors++;
    }
    if (!(event_broker_options & BROKER_EXTERNALCOMMAND_DATA)) {
        logger( LG_CRIT, "need BROKER_EXTERNALCOMMAND_DATA (%i) event_broker_option enabled to work.", BROKER_EXTERNALCOMMAND_DATA );
        errors++;
    }
    if (!(event_broker_options & BROKER_STATECHANGE_DATA)) {
        logger( LG_CRIT, "need BROKER_STATECHANGE_DATA (%i) event_broker_option enabled to work.", BROKER_STATECHANGE_DATA );
        errors++;
    }

    return errors == 0;
}


void register_callbacks()
{
    neb_register_callback(NEBCALLBACK_HOST_STATUS_DATA,      g_nagios_handle, 0, broker_host); // Needed to start threads
    neb_register_callback(NEBCALLBACK_COMMENT_DATA,          g_nagios_handle, 0, broker_comment); // dynamic data
    neb_register_callback(NEBCALLBACK_DOWNTIME_DATA,         g_nagios_handle, 0, broker_downtime); // dynamic data
    neb_register_callback(NEBCALLBACK_SERVICE_CHECK_DATA,    g_nagios_handle, 0, broker_check); // only for statistics
    neb_register_callback(NEBCALLBACK_HOST_CHECK_DATA,       g_nagios_handle, 0, broker_check); // only for statistics
    neb_register_callback(NEBCALLBACK_LOG_DATA,              g_nagios_handle, 0, broker_log); // only for trigger 'log'
    neb_register_callback(NEBCALLBACK_EXTERNAL_COMMAND_DATA, g_nagios_handle, 0, broker_command); // only for trigger 'command'
    neb_register_callback(NEBCALLBACK_STATE_CHANGE_DATA,     g_nagios_handle, 0, broker_state); // only for trigger 'state'
    neb_register_callback(NEBCALLBACK_ADAPTIVE_PROGRAM_DATA, g_nagios_handle, 0, broker_program); // only for trigger 'program'
    neb_register_callback(NEBCALLBACK_PROCESS_DATA,          g_nagios_handle, 0, broker_process); // used for starting threads
    schedule_event(1, broker_event, NULL);  // used for timeperiods cache
}

void deregister_callbacks()
{
    neb_deregister_callback(NEBCALLBACK_HOST_STATUS_DATA,      broker_host);
    neb_deregister_callback(NEBCALLBACK_COMMENT_DATA,          broker_comment);
    neb_deregister_callback(NEBCALLBACK_DOWNTIME_DATA,         broker_downtime);
    neb_deregister_callback(NEBCALLBACK_SERVICE_CHECK_DATA,    broker_check);
    neb_deregister_callback(NEBCALLBACK_HOST_CHECK_DATA,       broker_check);
    neb_deregister_callback(NEBCALLBACK_LOG_DATA,              broker_log);
    neb_deregister_callback(NEBCALLBACK_EXTERNAL_COMMAND_DATA, broker_command);
    neb_deregister_callback(NEBCALLBACK_STATE_CHANGE_DATA,     broker_state);
    neb_deregister_callback(NEBCALLBACK_ADAPTIVE_PROGRAM_DATA, broker_program);
    neb_deregister_callback(NEBCALLBACK_PROCESS_DATA,          broker_program);
}


void check_pnp_path()
{
    struct stat st;
    if (0 == stat(g_pnp_path, &st)) {
        if (0 != access(g_pnp_path, R_OK)) {
            logger(LG_ERR, "PNP perfdata directory '%s' not readable. Please fix permissions.", g_pnp_path);
            g_pnp_path[0] = 0; // disable
        }
    }
    else {
        logger(LG_ERR, "PNP perfdata directory '%s' not existing. Please check pnp_path option!", g_pnp_path);
        g_pnp_path[0] = 0; // disable
    }
}


void livestatus_parse_arguments(const char *args_orig)
{
    /* set default socket path */
    strncpy(g_socket_path, DEFAULT_SOCKET_PATH, sizeof(g_socket_path));

    /* set default path to our logfile to be in the same path as nagios.log */
    strncpy(g_logfile_path, log_file, sizeof(g_logfile_path) - 16 /* len of "livestatus.log" */);
    char *slash = strrchr(g_logfile_path, '/');
    if (!slash)
        strcpy(g_logfile_path, "/tmp/livestatus.log");
    else
        strcpy(slash + 1, "livestatus.log");

    /* there is no default PNP path */
    g_pnp_path[0] = 0;

    /* also no custom variables is hidden by default */
    g_hidden_custom_var_prefix[0] = 0;

    if (!args_orig)
        return; // no arguments, use default options

    char *args = strdup(args_orig), *tmp;
    /*save pointer so that we can free it later*/
    tmp = args;
    char *token;
    while (0 != (token = next_field(&args)))
    {
        /* find = */
        char *part = token;
        char *left = next_token(&part, '=');
        char *right = next_token(&part, 0);
        if (!right) {
            char *sock_path;
            sock_path = nspath_absolute(left, config_file_dir);
            strncpy(g_socket_path, sock_path, sizeof(g_socket_path));
            free(sock_path);
        }
        else {
            if (!strcmp(left, "debug")) {
                g_debug_level = atoi(right);
                logger(LG_INFO, "Setting debug level to %d", g_debug_level);
            }
            else if (!strcmp(left, "log_file")) {
                strncpy(g_logfile_path, right, sizeof(g_logfile_path));
            }
            else if (!strcmp(left, "max_cached_messages")) {
                g_max_cached_messages = strtoul(right, 0, 10);
                logger(LG_INFO, "Setting max number of cached log messages to %lu", g_max_cached_messages);
            }
            else if (!strcmp(left, "max_lines_per_logfile")) {
                g_max_lines_per_logfile = strtoul(right, 0, 10);
                logger(LG_INFO, "Setting max number lines per logfile to %lu", g_max_lines_per_logfile);
            }
            else if (!strcmp(left, "thread_stack_size")) {
                g_thread_stack_size = strtoul(right, 0, 10);
                logger(LG_INFO, "Setting size of thread stacks to %lu", g_thread_stack_size);
            }
            else if (!strcmp(left, "max_response_size")) {
                g_max_response_size = strtoul(right, 0, 10);
                logger(LG_INFO, "Setting maximum response size to %lu bytes (%.1f MB)",
                        g_max_response_size, g_max_response_size / (1024.0*1024.0));
            }
            else if (!strcmp(left, "num_client_threads")) {
                logger(LG_INFO, "Ignoring deprecated option %s, there is no longer a limit to the number of concurrent threads", left);
            }
            else if (!strcmp(left, "query_timeout")) {
                int c = atoi(right);
                if (c < 0)
                    logger(LG_INFO, "Error: query_timeout must be >= 0");
                else {
                    g_query_timeout_msec = c;
                    if (c == 0)
                        logger(LG_INFO, "Disabled query timeout!");
                    else
                        logger(LG_INFO, "Setting timeout for reading a query to %d ms", c);
                }
            }
            else if (!strcmp(left, "idle_timeout")) {
                int c = atoi(right);
                if (c < 0)
                    logger(LG_INFO, "Error: idle_timeout must be >= 0");
                else {
                    g_idle_timeout_msec = c;
                    if (c == 0)
                        logger(LG_INFO, "Disabled idle timeout!");
                    else
                        logger(LG_INFO, "Setting idle timeout to %d ms", c);
                }
            }
            else if (!strcmp(left, "service_authorization")) {
                if (!strcmp(right, "strict"))
                    g_service_authorization = AUTH_STRICT;
                else if (!strcmp(right, "loose"))
                    g_service_authorization = AUTH_LOOSE;
                else {
                    logger(LG_INFO, "Invalid service authorization mode. Allowed are strict and loose.");
                }
            }
            else if (!strcmp(left, "group_authorization")) {
                if (!strcmp(right, "strict"))
                    g_group_authorization = AUTH_STRICT;
                else if (!strcmp(right, "loose"))
                    g_group_authorization = AUTH_LOOSE;
                else {
                    logger(LG_INFO, "Invalid group authorization mode. Allowed are strict and loose.");
                }
            }
            else if (!strcmp(left, "pnp_path")) {
                strncpy(g_pnp_path, right, sizeof(g_pnp_path) - 1);
                if (right[strlen(right) - 1] != '/')
                    strncat(g_pnp_path, "/",  sizeof(g_pnp_path) - strlen(g_pnp_path) - 1 ); // make sure, that trailing slash is always there
                check_pnp_path();
            }
            else if (!strcmp(left, "data_encoding")) {
                if (!strcmp(right, "utf8"))
                    g_data_encoding = ENCODING_UTF8;
                else if (!strcmp(right, "latin1"))
                    g_data_encoding = ENCODING_LATIN1;
                else if (!strcmp(right, "mixed"))
                    g_data_encoding = ENCODING_MIXED;
                else {
                    logger(LG_INFO, "Invalid data_encoding %s. Allowed are utf8, latin1 and mixed.", right);
                }
            }
            else if (!strcmp(left, "livecheck")) {
                logger(LG_INFO, "Livecheck has been removed from Livestatus. Sorry.");
            }
            else if (!strcmp(left, "hidden_custom_var_prefix")) {
                strncpy(g_hidden_custom_var_prefix, right, sizeof(g_hidden_custom_var_prefix));
            }
            else {
                logger(LG_INFO, "Ignoring invalid option %s=%s", left, right);
            }
        }
    }
    free(tmp);
}

void omd_advertize()
{
    char *omd_site = getenv("OMD_SITE");
    if (omd_site) {
        if (g_debug_level > 0)
            logger(LG_INFO, "Running on OMD site %s. Cool.", omd_site);
    }
    else {
        logger(LG_INFO, "Hint: please try out OMD - the Open Monitoring Distribution");
        logger(LG_INFO, "Please visit OMD at http://omdistro.org");
    }
}


/* this function gets called when the module is loaded by the event broker */
int nebmodule_init(int flags __attribute__ ((__unused__)), char *args, void *handle)
{
    g_nagios_handle = handle;
    g_num_client_threads = 0;
    g_should_terminate = false;
    g_client_threads = NULL;
    g_num_client_threads = 0;
    livestatus_parse_arguments(args);
    open_logfile();

    logger(LG_INFO, "Naemon Livestatus %s Socket: '%s'", VERSION, g_socket_path);

    if (!open_unix_socket())
        return 1;

	event_broker_options = ~0; /* force settings to "everything" */
    if (!verify_event_broker_options()) {
        logger(LG_CRIT, "Fatal: bailing out. Please fix event_broker_options.");
        logger(LG_CRIT, "Hint: your event_broker_options are set to %d. Try setting it to -1.", event_broker_options);
        return 1;
    }
    else if (g_debug_level > 0)
        logger(LG_INFO, "Your event_broker_options are sufficient for livestatus.");

    store_init();
    register_callbacks();

    logger(LG_INFO, "Finished initialization. Further log messages go to %s", g_logfile_path);
    return OK;
}

int nebmodule_deinit(int flags __attribute__ ((__unused__)), int reason __attribute__ ((__unused__)))
{
    size_t i;
    logger(LG_INFO, "deinitializing");
    g_should_terminate = true;
    close_unix_socket();

    /*
     * Make sure all client connections are terminated before
     * returning control. Since we touch global naemon state everywhere,
     * this is strictly mandatory.
     */
    for (i = 0; i < g_num_client_threads; ++i) {
        if (pthread_join(*(g_client_threads[i]), NULL) != 0) {
            logger(LG_ERR, "Failed to join with client thread");
        }
        free(g_client_threads[i]);
    }
    free(g_client_threads);

    store_deinit();
    deregister_callbacks();
    close_logfile();
    return 0;
}
