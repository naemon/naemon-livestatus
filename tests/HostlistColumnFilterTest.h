#include <cppunit/extensions/HelperMacros.h>

class HostlistColumnFilterTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( HostlistColumnFilterTest );
	CPPUNIT_TEST( testHostlistEmpty );
	CPPUNIT_TEST( testHostlistNotEmpty );
	CPPUNIT_TEST_SUITE_END();

	public:
	void setUp();
	void tearDown();
	void testHostlistEmpty();
	void testHostlistNotEmpty();
};
