Feature: Queries have a consistent time-to-response

    Background: Set up naemon configuration
        Given I have naemon hostgroup objects
            | hostgroup_name |
            | hg1            |
            | hg2            |
        And I have naemon host objects
            |use         |host_name|contacts       |hostgroups     |
            |default-host|host1    |default-contact|hg1            |
            |default-host|host2    |default-contact|hg2            |
            |default-host|host3    |default-contact|hg2            |
        And I have naemon service objects
            | use            | service_description         | host_name |
            | default-service| service1                    | host1     |
            | default-service| service2                    | host2     |
            | default-service| service3                    | host3     |
        And I start naemon

    Scenario Outline: Livestatus responds within a consistent timeframe to concurrent queries

        Given I clobber livestatus with <n_queries> queries with <idle_time> seconds idle time
        Then the slowest query should be no more than 5 times slower than the fastest query

        Examples:
            |n_queries|idle_time|
            |10       |1        |
            |100      |1        |
            |500      |1        |

    Scenario Outline: Livestatus average response time is decent

        Given I clobber livestatus with <n_queries> queries
        Then the average query response time should be no more than 0.01 seconds

        Examples:
            |n_queries|
            |10       |
            |100      |
            |500      |


