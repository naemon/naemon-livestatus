Given /^I submit the following livestatus query$/ do |table|
  @naemon.brokers[:livestatus].query(table.raw.join('\n'))
end

Given(/^I submit the following livestatus external command "(.*?)"$/) do |cmd|
  @naemon.brokers[:livestatus].query("COMMAND [#{Time.now.to_i}] #{cmd}")
end

Then /^I should see the following raw livestatus response$/ do |table|
  response = @naemon.brokers[:livestatus].last_response()
  table.raw.each_with_index do |line, idx|
    expected = "\"#{line[0]}\"".undump
    raise "row #{idx}: expected #{expected.inspect}, got #{response[idx].inspect}" unless response[idx] == expected
  end
end

Then /^I should see the following livestatus response$/ do |table|
  response = @naemon.brokers[:livestatus].last_response()
  table.raw.each_with_index do |line, idx|
    raise "row #{idx}: expected #{line[0].inspect}, got #{response[idx].inspect}" unless response[idx] == line[0]
  end
end

Then /^I should see the following livestatus response, ignoring whitespace$/ do |output|
  response = @naemon.brokers[:livestatus].last_response()
  processed_response = response.join("").gsub(/[ \t\n\r]/,"")
  processed_output = output.gsub(/[ \t\n\r]/,"")
  raise "expected #{processed_output.inspect}, got #{processed_response.inspect}" unless processed_response == processed_output
end

Then /^I should see the following livestatus response, using regular expression$/ do |output|
  response = @naemon.brokers[:livestatus].last_response()
  processed_response = response.join("\n")
  raise "expected response to match /#{output}/, got #{processed_response.inspect}" unless processed_response.match?(Regexp.new(output))
end

Given(/^I wait for (\d+) seconds$/) do |secs|
  sleep(secs.to_i)
end

Then /^I should see at least (\d+) distinct rows in the livestatus response$/ do |min_rows|
  response = @naemon.brokers[:livestatus].last_response()
  distinct = response.uniq
  raise "expected at least #{min_rows} distinct rows, got #{distinct.length}" unless distinct.length >= min_rows.to_i
end

Given /^I clobber livestatus with (\d+) queries with (\d+) seconds idle time$/ do |nqueries, idle_time|
  @naemon.brokers[:livestatus].clobber(nqueries, idle_time)
end

Given /^I clobber livestatus with (\d+) queries$/ do |nqueries|
  step "I clobber livestatus with #{nqueries} queries with 0 seconds idle time"
end

Then /^the slowest query should be no more than (\d+) times slower than the fastest query$/ do |factor|
  clobber_data = @naemon.brokers[:livestatus].clobber_data()
  fastest = clobber_data['min_time']
  slowest = clobber_data['max_time']
  raise "slowest #{slowest} exceeds #{factor}x fastest #{fastest}" unless slowest < (fastest * factor.to_i)
end

Then /^the average query response time should be no more than (\d+.\d+) seconds$/ do |threshold|
  avg = @naemon.brokers[:livestatus].clobber_data()['avg_time']
  raise "average #{avg} exceeds #{threshold} seconds" unless avg < threshold.to_f
end
