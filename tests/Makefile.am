if HAVE_CPPUNIT
check_PROGRAMS = \
	StringColumnFilterTest \
	HostlistColumnFilterTest

TESTS = $(check_PROGRAMS)

StringColumnFilterTest_SOURCES = $(top_builddir)/src/StringColumnFilter.h $(top_builddir)/src/CustomVarsFilter.h stubs.c StringColumnFilterTest.h StringColumnFilterTest.cc
StringColumnFilterTest_CXXFLAGS = $(CPPUNIT_FLAGS) -I$(top_builddir) $(NAEMON_CFLAGS)
StringColumnFilterTest_LDFLAGS = $(CPPUNIT_LIBS) -ldl -no-install $(NAEMON_LIBS)
StringColumnFilterTest_LDADD = $(top_builddir)/src/livestatus.la

HostlistColumnFilterTest_SOURCES = $(top_builddir)/src/StringColumnFilter.h $(top_builddir)/src/CustomVarsFilter.h stubs.c HostlistColumnFilterTest.h HostlistColumnFilterTest.cc
HostlistColumnFilterTest_CXXFLAGS = $(CPPUNIT_FLAGS) -I$(top_builddir) $(NAEMON_CFLAGS)
HostlistColumnFilterTest_LDFLAGS = $(CPPUNIT_LIBS) -ldl -no-install $(NAEMON_LIBS)
HostlistColumnFilterTest_LDADD = $(top_builddir)/src/livestatus.la

endif
