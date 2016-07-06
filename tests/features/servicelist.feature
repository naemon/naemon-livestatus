Feature: Service list columns should be correctly filtered

	Service list columns can exist in both hosts and servicegroups. On hosts
	filtering should be done without need for specifying a host name, but in
	servicegroups, it's not obvious which host it is. For that, servicelist
	columns contains host|service_description, which is what to search for.
	
	Background: Set up environment
		And I have naemon host objects
			| use          | host_name | contacts        |
			| default-host | host1     | default-contact |
			| default-host | host2     | default-contact |
		Given I have naemon servicegroup objects
			| servicegroup_name |
			| sg1               |
		And I have naemon service objects
			| use			  | service_description | host_name | servicegroups |
			| default-service | PING                | host1     | sg1           |
			| default-service | PING                | host2     | sg1           |
			| default-service | web                 | host2     | sg1           |
		And I start naemon

	Scenario: Searching on host only filters on services
		Given I submit the following livestatus query
			| GET hosts                |
			| Columns: name services   |
			| Filter: services >= PING |
		Then I should see the following livestatus response
			| host1;PING     |
			| host2;web,PING |
		Given I submit the following livestatus query
			| GET hosts                |
			| Columns: name services   |
			| Filter: services >= web |
		Then I should see the following livestatus response
			| host2;web,PING |

	Scenario: Searching on servicegroup filters on services filters with hostname
		Given I submit the following livestatus query
			| GET servicegroups              |
			| Columns: name members          |
			| Filter: members >= host2\|PING |
		Then I should see the following livestatus response
			| sg1;host2\|web,host2\|PING,host1\|PING |