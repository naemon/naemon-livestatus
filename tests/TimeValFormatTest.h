#include <cppunit/extensions/HelperMacros.h>

class TimeValFormatTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TimeValFormatTest );
	CPPUNIT_TEST( testTimeValMatchesSnprintf );
	CPPUNIT_TEST_SUITE_END();

	public:
	void setUp();
	void tearDown();
	void testTimeValMatchesSnprintf();
};
