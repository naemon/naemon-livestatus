# Naemon-Livestatus

This is a fork of Mattias Kettner's mk_livestatus. It has been ported to
Naemon and contains some extra features such as sorting and page offsets.

To find out more about livestatus, see:
 [www.naemon.io](http://www.naemon.io/documentation/usersguide/livestatus.html)

## License

Naemon Livestatus is published under the GPL-2.0 license, see [LICENSE](https://github.com/naemon/naemon-livestatus/blob/master/LICENSE)

## Differences between mk_livestatus and naemons fork of livestatus

Three additions to Livestatus Query Language were added:

### Sorting

    Sort: <column name> <asc/desc>

Sorts the result set by the specified column in the given direction. Multiple
Sort lines can be added. First sort line takes precedance.

Example:

    GET hosts
    Sort: last_hard_state_change desc

### Offset

    Offset: <number of lines>

Lines to skip from the beginning of the result set. Useful for pagination in
combination with Limit header.

Example:

    GET services
    Sort: host_name asc
    Sort: description asc
    Limit: 100
    Offset: 300

### Wrapped JSON

    OutputFormat: wrapped_json

An extension to the json output format.
The result set is packed in a json object, with a couple of possible fields:

- columns: an array of column names. (optional)
- data: an array of arrays, describing the result set, in the same syntax common
  json output, without embedded column names.
- total_count: The number of lines in the resultsed, except the limitation of
  Limit and Offset headers.

Example:

    GET hosts
    OutputFormat: wrapped_json
    Columns: name
    Limit: 1

will result in something like this:

```json
    {
      "data":[
        ["localhost"]
      ],
      "total_count":22
    }
```

## About Naemon

To find out more about Naemon, see [www.naemon.io](https://www.naemon.io)

## Installation

To install, run:

    autoreconf -s
    automake --add-missing
    ./configure CPPFLAGS=-I$(path to naemon include files, usually /usr/local/naemon/include)
    make
    sudo make install
