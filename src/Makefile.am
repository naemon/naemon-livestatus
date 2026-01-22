bin_PROGRAMS = unixcat
unixcat_SOURCES = unixcat.c
unixcat_LDADD = -lpthread

EXTRA_DIST = *.h

pkglib_LTLIBRARIES = livestatus.la
livestatus_la_LDFLAGS = -module -avoid-version -shared $(ICU_LIBS) $(NAEMON_LIBS)
livestatus_la_CXXFLAGS = $(CXXFLAGS) -lpthread -lstdc++ $(NAEMON_CFLAGS)
livestatus_la_CFLAGS = $(CFLAGS) $(ICU_CFLAGS) $(NAEMON_CFLAGS)
livestatus_la_SOURCES = \
	AndingFilter.cc Column.cc ColumnsColumn.cc CustomVarsExplicitColumn.cc \
	ContactsColumn.cc CustomVarsColumn.cc CustomVarsFilter.cc DoubleColumn.cc \
	DoubleColumnFilter.cc DowntimeOrComment.cc DownCommColumn.cc EmptyColumn.cc \
	Filter.cc GlobalCountersColumn.cc HostContactsColumn.cc HostgroupsColumn.cc \
	HostlistColumn.cc HostlistColumnFilter.cc HostlistStateColumn.cc \
	HostSpecialIntColumn.cc ServiceSpecialIntColumn.cc InputBuffer.cc IntColumn.cc IntColumnFilter.cc \
	ListColumn.cc ListColumnFilter.cc OffsetDoubleColumn.cc OffsetIntColumn.cc \
	OffsetStringColumn.cc OffsetTimeperiodColumn.cc OringFilter.cc OutputBuffer.cc \
	OffsetTimeColumn.cc TimeColumn.cc TimeColumnFilter.cc PerfdataAggregator.cc \
	OffsetTimeValColumn.cc TimeValColumn.cc TimeValColumnFilter.cc \
	Query.cc ServiceContactsColumn.cc ServicegroupsColumn.cc ServicelistColumn.cc \
	ServicelistColumnFilter.cc ServicelistStateColumn.cc store.cc Store.cc \
	StringColumn.cc StringColumnFilter.cc strutil.cc Table.cc TableColumns.cc \
	HostSpecialDoubleColumn.cc TableCommands.cc TableContacts.cc TableDownComm.cc TableHostgroups.cc \
	ServiceSpecialDoubleColumn.cc TableHosts.cc TableServicegroups.cc TableServices.cc TableStatus.cc  \
	LogEntry.cc LogCache.cc Logfile.cc TableStateHistory.cc TableLog.cc TableTimeperiods.cc TableContactgroups.cc \
	ContactgroupsMemberColumn.cc OffsetStringMacroColumn.cc OffsetStringServiceMacroColumn.cc \
	OffsetStringHostMacroColumn.cc StatsColumn.cc IntAggregator.cc CountAggregator.cc \
	DoubleAggregator.cc AttributelistColumn.cc AttributelistFilter.cc \
	global_counters.c  module.c  logger.c waittriggers.c TimeperiodsCache.cc \
	TimeperiodExclusionColumn.cc TimeperiodDaysColumn.cc TimeperiodExceptionsColumn.cc pnp4nagios.cc \
	HostlistDependencyColumn.cc HostlistDependencyColumnFilter.cc \
	ServicelistDependencyColumn.cc ServicelistDependencyColumnFilter.cc \
	ContactgroupsColumn.cc RowSortedSet.cc HostServiceState.cc opids.cc auth.cc \
	ContactgroupsObjectlistColumn.cc CommandsMemberColumn.cc EmptyFilter.cc

if HAVE_DIET
	diet -v $(CC) -x c -Wno-deprecated-declarations $(CFLAGS) $(LDFLAGS) -I.. -s -o $@ $^
else
	$(CC) -x c -Wno-deprecated-declarations $(CFLAGS) $(LDFLAGS) -I.. -s -o $@ $^
endif
