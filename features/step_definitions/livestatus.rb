Given /^I submit the following livestatus query$/ do |table|
  @naemon.brokers[:livestatus].query(table.raw.join('\n'))
end

Then /^I should see the following livestatus response$/ do |table|
  response = @naemon.brokers[:livestatus].last_response()
  table.raw.each_with_index do |line, idx|
    response[idx].should == line[0]
  end
end
