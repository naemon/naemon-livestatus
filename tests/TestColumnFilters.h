#include <cppunit/extensions/HelperMacros.h>

class StringColumnFilterTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( StringColumnFilterTest );
	CPPUNIT_TEST( testEqualOp );
	CPPUNIT_TEST( testRegexOp );
	CPPUNIT_TEST( testRegexNonAscii );
	CPPUNIT_TEST( testEqualIcaseOp );
	CPPUNIT_TEST( testRegexIcaseOp );
	CPPUNIT_TEST( testRegexIcaseNonAscii );
	CPPUNIT_TEST( testGreaterOp );
	CPPUNIT_TEST( testLessOp );
	CPPUNIT_TEST_SUITE_END();

	public:
	void setUp();
	void tearDown();
	void testEqualOp();
	void testRegexOp();
	void testRegexNonAscii();
	void testEqualIcaseOp();
	void testRegexIcaseOp();
	void testRegexIcaseNonAscii();
	void testGreaterOp();
	void testLessOp();
};
