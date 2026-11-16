#include <cppunit/extensions/HelperMacros.h>

class IntFormatTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( IntFormatTest );
	CPPUNIT_TEST( testIntegersMatchSnprintf );
	CPPUNIT_TEST( testUnsignedMatchSnprintf );
	CPPUNIT_TEST_SUITE_END();

	public:
	void setUp();
	void tearDown();
	void testIntegersMatchSnprintf();
	void testUnsignedMatchSnprintf();
};
