TESTS = TestColumnFilters
check_PROGRAMS = $(TESTS)

TEST_COLUMN_FILTERS_DEPS = StringColumnFilter.o CustomVarsFilter.o Column.o Filter.o
TestColumnFilters_SOURCES = $(top_builddir)/src/StringColumnFilter.h $(top_builddir)/src/CustomVarsFilter.h stubs.c TestColumnFilters.h TestColumnFilters.cc
TestColumnFilters_CXXFLAGS = $(CPPUNIT_FLAGS) -I$(top_builddir)
TestColumnFilters_LDFLAGS = $(CPPUNIT_LIBS) -ldl -no-install
TestColumnFilters_LDADD = $(top_builddir)/src/livestatus.la
