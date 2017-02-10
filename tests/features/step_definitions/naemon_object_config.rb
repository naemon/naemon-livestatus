require 'tmpdir'
Given(/^I have naemon (.+) objects$/) do |type, table|
  table.hashes.each do |obj|
    @naemonconfig.add_obj(type, obj)
  end
end

And /^I start naemon$/ do
  @naemon = Naemon.new
  @naemon.set_object_config @naemonconfig
  livestatus = Livestatus.new Dir.pwd+"/live"
  @naemon.add_broker livestatus
  @naemon.start()
end

After do
  @naemon.stop
end
