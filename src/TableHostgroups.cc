// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <glib.h>
#include "nagios.h"
#include "TableHostgroups.h"
#include "Query.h"
#include "OffsetStringColumn.h"
#include "OffsetIntColumn.h"
#include "HostlistColumn.h"
#include "HostlistStateColumn.h"
#include "auth.h"
#include "tables.h"
#include "TableHosts.h"

struct ctc_with_result {
	contact *ctc;
	gboolean result;
};

TableHostgroups::TableHostgroups()
{
    addColumns(this, "", -1);
}

void TableHostgroups::addColumns(Table *table, string prefix, int indirect_offset)
{
    hostgroup hgr;
    char *ref = (char *)&hgr;
    table->addColumn(new OffsetIntColumn(prefix + "id",
                "Hostgroup id", (char *)(&hgr.id) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "name",
                "Name of the hostgroup",       (char *)(&hgr.group_name) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "alias",
                "An alias of the hostgroup",      (char *)(&hgr.alias) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "notes",
                "Optional notes to the hostgroup",      (char *)(&hgr.notes) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "notes_url",
                "An optional URL with further information about the hostgroup",  (char *)(&hgr.notes_url) - ref, indirect_offset));
    table->addColumn(new OffsetStringColumn(prefix + "action_url",
                "An optional URL to custom actions or information about the hostgroup", (char *)(&hgr.action_url) - ref, indirect_offset));
    table->addColumn(new HostlistColumn(    prefix + "members",
                "A list of all host names that are members of the hostgroup",    (char *)(&hgr.members) - ref, indirect_offset, false));
    table->addColumn(new HostlistColumn(    prefix + "members_with_state",
                "A list of all host names that are members of the hostgroup together with state and has_been_checked",
                (char *)(&hgr.members) - ref, indirect_offset, true));

    table->addColumn(new HostlistStateColumn(prefix + "worst_host_state",
                "The worst state of all of the groups' hosts (UP <= UNREACHABLE <= DOWN)",     HLSC_WORST_HST_STATE, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_hosts",
                "The total number of hosts in the group",            HLSC_NUM_HST, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_hosts_pending",
                "The number of hosts in the group that are pending",    HLSC_NUM_HST_PENDING, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_hosts_up",
                "The number of hosts in the group that are up",         HLSC_NUM_HST_UP, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_hosts_down",
                "The number of hosts in the group that are down",       HLSC_NUM_HST_DOWN, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_hosts_unreach",
                "The number of hosts in the group that are unreachable",    HLSC_NUM_HST_UNREACH, (char *)(&hgr.members) - ref, indirect_offset));

    table->addColumn(new HostlistStateColumn(prefix + "num_services",
                "The total number of services of hosts in this group",         HLSC_NUM_SVC, (char *)(&hgr.members) - ref, indirect_offset));

    // soft states
    table->addColumn(new HostlistStateColumn(prefix + "worst_service_state",
                "The worst state of all services that belong to a host of this group (OK <= WARN <= UNKNOWN <= CRIT)",  HLSC_WORST_SVC_STATE, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_services_pending",
                "The total number of services with the state Pending of hosts in this group", HLSC_NUM_SVC_PENDING, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_services_ok",
                "The total number of services with the state OK of hosts in this group",      HLSC_NUM_SVC_OK, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_services_warn",
                "The total number of services with the state WARN of hosts in this group",    HLSC_NUM_SVC_WARN, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_services_crit",
                "The total number of services with the state CRIT of hosts in this group",    HLSC_NUM_SVC_CRIT, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_services_unknown",
                "The total number of services with the state UNKNOWN of hosts in this group", HLSC_NUM_SVC_UNKNOWN, (char *)(&hgr.members) - ref, indirect_offset));

    // hard state
    table->addColumn(new HostlistStateColumn(prefix + "worst_service_hard_state",
                "The worst state of all services that belong to a host of this group (OK <= WARN <= UNKNOWN <= CRIT)",  HLSC_WORST_SVC_HARD_STATE, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_services_hard_ok",
                "The total number of services with the state OK of hosts in this group",      HLSC_NUM_SVC_HARD_OK, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_services_hard_warn",
                "The total number of services with the state WARN of hosts in this group",    HLSC_NUM_SVC_HARD_WARN, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_services_hard_crit",
                "The total number of services with the state CRIT of hosts in this group",    HLSC_NUM_SVC_HARD_CRIT, (char *)(&hgr.members) - ref, indirect_offset));
    table->addColumn(new HostlistStateColumn(prefix + "num_services_hard_unknown",
                "The total number of services with the state UNKNOWN of hosts in this group", HLSC_NUM_SVC_HARD_UNKNOWN, (char *)(&hgr.members) - ref, indirect_offset));

    table->clearNatSort();
    table->addNatSort( prefix + "name" );
}

void TableHostgroups::answerQuery(Query *query)
{
    hostgroup *hg = hostgroup_list;
    while (hg) {
        if (!query->processDataset(hg))
            break;
        hg = hg->next;
    }
}


void *TableHostgroups::findObject(char *objectspec)
{
    return find_hostgroup(objectspec);
}

static gboolean is_authorized_for(gpointer _name, gpointer _hst, gpointer user_data)
{
    struct ctc_with_result *ctcr = (struct ctc_with_result *)user_data;
    host *hst = (host *)_hst;
    bool is = g_table_hosts->isAuthorized(ctcr->ctc, hst);
    if (is && g_group_authorization == AUTH_LOOSE) {
        ctcr->result = TRUE;
        return TRUE;
    }
    else if (!is && g_group_authorization == AUTH_STRICT) {
        ctcr->result = TRUE;
        return TRUE;
    }
    ctcr->result = FALSE;
    return FALSE;
}

bool TableHostgroups::isAuthorized(contact *ctc, void *data)
{
    struct ctc_with_result ctcr;
    if (ctc == UNKNOWN_AUTH_USER)
        return false;

    hostgroup *hg = (hostgroup *)data;
    GTree *mem = hg->members;
    if (!g_tree_nnodes(mem))
        return false;

    ctcr.ctc = ctc;
    ctcr.result = FALSE;
    g_tree_foreach(mem, is_authorized_for, &ctcr);
    if(ctcr.result)
        return g_group_authorization == AUTH_LOOSE;

    if(g_group_authorization == AUTH_LOOSE)
        return false;
    return true;
}
