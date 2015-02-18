if HAVE_CPPUNIT
TESTS = TestColumnFilters

check_PROGRAMS = $(TESTS)

TEST_COLUMN_FILTERS_DEPS = StringColumnFilter.o CustomVarsFilter.o Column.o Filter.o
TestColumnFilters_SOURCES = $(top_builddir)/src/StringColumnFilter.h $(top_builddir)/src/CustomVarsFilter.h stubs.c TestColumnFilters.h TestColumnFilters.cc
TestColumnFilters_CXXFLAGS = $(CPPUNIT_FLAGS) -I$(top_builddir) $(NAEMON_CFLAGS)
TestColumnFilters_LDFLAGS = $(CPPUNIT_LIBS) -ldl -no-install $(NAEMON_LIBS)
TestColumnFilters_LDADD = $(top_builddir)/src/livestatus.la
endif
