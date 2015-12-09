Feature: Queries work as expected
	
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

	Scenario: Livestatus responds to queries on servicesbyhostgroup table

		Given I submit the following livestatus query
			|GET servicesbyhostgroup|
			|Columns: description|
			|Columns: hostgroup_name|
		Then I should see the following livestatus response
			|service1;hg1|
			|service2;hg2|
			|service3;hg2|

	Scenario: Livestatus lists services in services table
		Given I submit the following livestatus query
			|GET services|
			|Columns: description|
		Then I should see the following livestatus response
			|service1|
			|service2|
			|service3|

	Scenario: Livestatus lists hosts in hosts table
		Given I submit the following livestatus query
			|GET hosts|
			|Columns: host_name|
		Then I should see the following livestatus response
			|host1|
			|host2|
			|host3|
