// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <string.h>

#include "ServiceSpecialDoubleColumn.h"
#include "nagios.h"
#include "logger.h"
#include "time.h"

extern int      interval_length;

double ServiceSpecialDoubleColumn::getValue(void *data)
{
    data = shiftPointer(data);
    if (!data) return 0;

    service *svc = (service *)data;
    switch (_type) {
        case SSDC_STALENESS:
        {
            bool is_cmk_passive = !strncmp(svc->check_command_ptr->name, "check_mk-", 9);

            time_t check_result_age = time(0) - svc->last_check;
            service *tmp_svc;

            // check_mk PASSIVE CHECK: Find check-mk service and get its check interval
            if (is_cmk_passive) {
                host *host = svc->host_ptr;
                servicesmember *svc_member = host->services;
                double check_interval = 1;
                while (svc_member != 0) {
                    tmp_svc = svc_member->service_ptr;
                    if (!strncmp(tmp_svc->check_command_ptr->name, "check-mk", 9)) {
                        return check_result_age / ((tmp_svc->check_interval == 0 ? 1 : tmp_svc->check_interval) * interval_length);
                    }
                    svc_member = svc_member->next;
                }
                return 1; // Shouldnt happen! We always except check-mk service
            }
            else // Other non-cmk passive and active checks
            {
                return check_result_age / ((svc->check_interval == 0 ? 1 : svc->check_interval) * interval_length);
            }
        }
    }
    return -1; // Never reached
}
