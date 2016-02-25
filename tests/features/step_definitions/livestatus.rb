Given /^I submit the following livestatus query$/ do |table|
  @naemon.brokers[:livestatus].query(table.raw.join('\n'))
end

Then /^I should see the following livestatus response$/ do |table|
  response = @naemon.brokers[:livestatus].last_response()
  table.raw.each_with_index do |line, idx|
    response[idx].should == line[0]
  end
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
  slowest.should < (fastest * factor.to_i)
end

Then /^the average query response time should be no more than (\d+.\d+) seconds$/ do |threshold|
  @naemon.brokers[:livestatus].clobber_data()['avg_time'].should < threshold.to_f 
end
