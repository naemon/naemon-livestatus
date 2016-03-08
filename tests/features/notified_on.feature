Feature: Hosts and services have the "notified_on" column

	Background: Set up naemon configuration 

		Given I have naemon hostgroup objects
			| hostgroup_name |
			| hg1			 |
			| hg2		     |
		And I have naemon host objects
			|use|host_name|contacts|			hostgroups| 
		    |default-host|host1|default-contact|hg1            | 
		    |default-host|host2|default-contact|hg2            | 
		    |default-host|host3|default-contact|hg2            | 
		And I have naemon service objects
			| use			 | service_description | host_name |
			| default-service| service1			   | host1 |
			| default-service| service2			   | host2 |
			| default-service| service3			   | host3 |
		And I start naemon

	Scenario: Livestatus lists notified_on for services
		Given I submit the following livestatus query
			|GET services|
			|Columns: host_name description notified_on|
		Then I should see the following livestatus response
			|host1;service1;0|
			|host2;service2;0|
			|host3;service3;0|

	Scenario: Livestatus lists hosts in hosts table
		Given I submit the following livestatus query
			|GET hosts|
			|Columns: host_name notified_on|
		Then I should see the following livestatus response
			|host1;0|
			|host2;0|
			|host3;0|
