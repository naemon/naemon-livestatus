Feature: JSON output works as intended
	By some reason the array of columns has been packed within another array.
	Since it has been so for a while, it should be kept like that, not to break
	compatibilit, even though it might not be the best syntax.
	
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
			| GET hosts                  |
			| Limit: 2                   |
			| Columns: name              |
			| ColumnHeaders: on          |
			| OutputFormat: wrapped_json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			{
				"columns":[
					["name"]
				],
				"data":[
					["host1"],
					["host2"]
				],
				"total_count":3
			}
			"""

	Scenario: Regular query with headers, sorted
		Given I submit the following livestatus query
			| GET hosts                  |
			| Limit: 2                   |
			| Columns: name              |
			| Sort: name desc            |
			| ColumnHeaders: on          |
			| OutputFormat: wrapped_json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			{
				"columns":[
					["name"]
				],
				"data":[
					["host3"],
					["host2"]
				],
				"total_count":3
			}
			"""

	Scenario: Stats only query with headers
		Given I submit the following livestatus query
			| GET hosts                  |
			| StatsAnd: 0                |
			| ColumnHeaders: on          |
			| OutputFormat: wrapped_json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			{
				"columns":[
					["stats_1"]
				],
				"data":[
					[3]
				],
				"total_count":1
			}
			"""

	Scenario: aggregated stats with headers
		Given I submit the following livestatus query
			| GET hosts                  |
			| StatsAnd: 0                |
			| Columns: name              |
			| ColumnHeaders: on          |
			| OutputFormat: wrapped_json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			{
				"columns":[
					["name", "stats_1"]
				],
				"data":[
					["host1", 1],
					["host2", 1],
					["host3", 1]
				],
				"total_count":3
			}
			"""

	Scenario: Regular query without headers, unsorted
		Given I submit the following livestatus query
			| GET hosts                  |
			| Columns: name              |
			| Limit: 2                   |
			| ColumnHeaders: off         |
			| OutputFormat: wrapped_json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			{
				"data":[
					["host1"],
					["host2"]
				],
				"total_count":3
			}
			"""

	Scenario: Regular query without headers, sorted
		Given I submit the following livestatus query
			| GET hosts                  |
			| Columns: name              |
			| Limit: 2                   |
			| Sort: name desc            |
			| ColumnHeaders: off         |
			| OutputFormat: wrapped_json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			{
				"data":[
					["host3"],
					["host2"]
				],
				"total_count":3
			}
			"""

	Scenario: Stats only query without headers
		Given I submit the following livestatus query
			| GET hosts                  |
			| StatsAnd: 0                |
			| ColumnHeaders: off         |
			| OutputFormat: wrapped_json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			{
				"data":[
					[3]
				],
				"total_count":1
			}
			"""

	Scenario: aggregated stats without headers
		Given I submit the following livestatus query
			| GET hosts                  |
			| StatsAnd: 0                |
			| Columns: name              |
			| ColumnHeaders: off         |
			| OutputFormat: wrapped_json |
		Then I should see the following livestatus response, ignoring whitespace
			"""
			{
				"data":[
					["host1", 1],
					["host2", 1],
					["host3", 1]
				],
				"total_count":3
			}
			"""