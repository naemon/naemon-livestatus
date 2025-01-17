// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#ifndef store_h
#define store_h

#include "nagios.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void store_init();
    void store_deinit();
    void store_register_comment(nebstruct_comment_data *);
    void store_register_downtime(nebstruct_downtime_data *);
    int  store_answer_request(void *input_buffer, void *output_buffer, int fd);
    void *create_outputbuffer(int *termination_flag);
    void flush_output_buffer(void *ob, int fd);
    void delete_outputbuffer(void *);
    void *create_inputbuffer(int *termination_flag);
    void set_inputbuffer_fd(void *, int fd);
    void delete_inputbuffer(void *);
    void queue_add_connection(int cc);
    int  queue_pop_connection();
    void queue_wakeup_all();
    void update_timeperiods_cache(time_t);

#ifdef __cplusplus
}
#endif

#endif /* store_h */
