Feature: JSON output works as intended
	
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

	Scenario: Regular query with headers, unsorted
		Given I submit the following livestatus query
			| GET hosts          |
			| Limit: 2           |
			| Columns: name      |
			| ColumnHeaders: on  |
			| OutputFormat: json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			[
				["name"],
				["host1"],
				["host2"]
			]
			"""

	Scenario: Regular query with headers, sorted
		Given I submit the following livestatus query
			| GET hosts          |
			| Limit: 2           |
			| Sort: name desc    |
			| Columns: name      |
			| ColumnHeaders: on  |
			| OutputFormat: json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			[
				["name"],
				["host3"],
				["host2"]
			]
			"""

	Scenario: Stats only query with headers
		Given I submit the following livestatus query
			| GET hosts          |
			| StatsAnd: 0        |
			| ColumnHeaders: on  |
			| OutputFormat: json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			[
				["stats_1"],
				[3]
			]
			"""

	Scenario: aggregated stats with headers
		Given I submit the following livestatus query
			| GET hosts          |
			| StatsAnd: 0        |
			| Columns: name      |
			| ColumnHeaders: on  |
			| OutputFormat: json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			[
				["name", "stats_1"],
				["host1", 1],
				["host2", 1],
				["host3", 1]
			]
			"""

	Scenario: Regular query without headers, unsorted
		Given I submit the following livestatus query
			| GET hosts          |
			| Limit: 2           |
			| Columns: name      |
			| ColumnHeaders: off |
			| OutputFormat: json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			[
				["host1"],
				["host2"]
			]
			"""

	Scenario: Regular query without headers, sorted
		Given I submit the following livestatus query
			| GET hosts          |
			| Limit: 2           |
			| Sort: name desc    |
			| Columns: name      |
			| ColumnHeaders: off |
			| OutputFormat: json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			[
				["host3"],
				["host2"]
			]
			"""

	Scenario: Stats only query without headers
		Given I submit the following livestatus query
			| GET hosts          |
			| StatsAnd: 0        |
			| ColumnHeaders: off |
			| OutputFormat: json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			[
				[3]
			]
			"""

	Scenario: aggregated stats without headers
		Given I submit the following livestatus query
			| GET hosts          |
			| StatsAnd: 0        |
			| Columns: name      |
			| ColumnHeaders: off |
			| OutputFormat: json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			[
				["host1", 1],
				["host2", 1],
				["host3", 1]
			]
			"""