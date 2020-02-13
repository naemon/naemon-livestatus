Feature: Statehist queries work as expected

	Background: Set up naemon configuration
		Given I have naemon hostgroup objects
			| hostgroup_name |
			| hg1			 |
			| hg2		     |
		And I have naemon host objects
			| use			| host_name	| contacts			| hostgroups	|
			| default-host	| host1		| default-contact	| hg1		|
			| default-host	| host2		| default-contact	| hg2		|
			| default-host	| host3		| default-contact	| hg2		|
		And I have naemon service objects
			| use			 | service_description | host_name 	|
			| default-service| service1			   | host1 		|
			| default-service| service2			   | host2 		|
			| default-service| service3			   | host3 		|
		And I start naemon

	Scenario: Livestatus responds to queries on statehist table
		Given I submit the following livestatus query
			| GET statehist 							|
			| Columns: host_name service_description 	|
			| Filter: time >= 1500000000 				|
			| Stats: sum duration_ok 					|
			| Stats: sum duration_warning 				|
			| Stats: sum duration_critical 				|
			| Sort: host_name asc 						|
		Then I should see the following livestatus response, using regular expression
			"""
			host1;;\d+;0;0
			host1;service1;\d+;0;0
			host2;;\d+;0;0
			host2;service2;\d+;0;0
			host3;;\d+;0;0
			host3;service3;\d+;0;0
			"""
