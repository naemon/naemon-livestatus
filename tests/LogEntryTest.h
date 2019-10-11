#include <cppunit/extensions/HelperMacros.h>

class LogEntryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LogEntryTest );
	CPPUNIT_TEST( testParserHostState );
	CPPUNIT_TEST( testParserIncompleteLine );
	CPPUNIT_TEST_SUITE_END();

	public:
	void setUp();
	void tearDown();
	void testParserHostState();
	void testParserIncompleteLine();
};