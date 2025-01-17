// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "auth.h"

int is_authorized_for(contact *ctc, host *hst, service *svc) {
    if (ctc == UNKNOWN_AUTH_USER)
        return false;

    if (svc) {
        if (g_service_authorization == AUTH_STRICT) {
            return is_contact_for_service(svc, ctc)
                || is_escalated_contact_for_service(svc, ctc);
        }
        else { // AUTH_LOOSE
            return  is_contact_for_host(hst, ctc)
                || is_escalated_contact_for_host(hst, ctc)
                || is_contact_for_service(svc, ctc)
                || is_escalated_contact_for_service(svc, ctc);
        }
    }
    // Entries for hosts
    else {
        return is_contact_for_host(hst, ctc)
            || is_escalated_contact_for_host(hst, ctc);
    }
}
