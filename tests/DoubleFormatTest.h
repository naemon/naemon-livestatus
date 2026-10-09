#include <cppunit/extensions/HelperMacros.h>

class DoubleFormatTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DoubleFormatTest );
	CPPUNIT_TEST( testDoublesMatchSnprintf );
	CPPUNIT_TEST( testWholeNumbersKeepTheirSign );
	CPPUNIT_TEST_SUITE_END();

	public:
	void setUp();
	void tearDown();
	void testDoublesMatchSnprintf();
	void testWholeNumbersKeepTheirSign();
};
