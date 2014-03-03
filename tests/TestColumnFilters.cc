#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <string.h>
#include <locale.h>
#include "TestColumnFilters.h"
#include "../src/OffsetStringColumn.h"
#include "../src/StringColumnFilter.h"
#include "../src/opids.h"

/**
 * Supported operators:
 * OP_INVALID       0
 * OP_EQUAL         1 // =
 * OP_REGEX         2 // ~
 * OP_EQUAL_ICASE   3 // =~
 * OP_REGEX_ICASE   4 // ~~
 * OP_GREATER       5 // >
 * OP_LESS          6 // <
 */

CPPUNIT_TEST_SUITE_REGISTRATION( StringColumnFilterTest );
#define DUMMY_COLUMN new OffsetStringColumn("some-column", "a description for the column", 0)
void
StringColumnFilterTest::setUp()
{
}

void
StringColumnFilterTest::tearDown()
{
}

void
StringColumnFilterTest::testEqualOp()
{
	char * refstring = strdup("Tinky-Winky");
	string * value_eq = new string("Tinky-Winky");
	string * value_neq = new string("Po");
	StringColumnFilter *scf = new StringColumnFilter(DUMMY_COLUMN, OP_EQUAL, refstring);
	CPPUNIT_ASSERT(scf->accepts(value_eq));
	CPPUNIT_ASSERT(!scf->accepts(value_neq));
}

void
StringColumnFilterTest::testRegexOp()
{
	char * refstring = strdup("^Laa.*$");
	string * value_match = new string("Laa-Laa");
	string * value_nomatch = new string("Dipsy");
	StringColumnFilter *scf = new StringColumnFilter(DUMMY_COLUMN, OP_REGEX, refstring);
	CPPUNIT_ASSERT(scf->accepts(value_match));
	CPPUNIT_ASSERT(!scf->accepts(value_nomatch));
}

void
StringColumnFilterTest::testRegexNonAscii()
{
	char * refstring = strdup("^.*Յ$");
	string * value_match = new string("Capital yi: Յ");
	string * value_nomatch = new string("Small yi: յ");
	StringColumnFilter *scf = new StringColumnFilter(DUMMY_COLUMN, OP_REGEX, refstring);
	CPPUNIT_ASSERT(scf->accepts(value_match));
	CPPUNIT_ASSERT(!scf->accepts(value_nomatch));
}

void
StringColumnFilterTest::testEqualIcaseOp()
{
	char * refstring = strdup("Tinky-Winky");
	string * value_match = new string("tInkY-WinKy");
	string * value_nomatch = new string("DIPsY");
	StringColumnFilter *scf = new StringColumnFilter(DUMMY_COLUMN, OP_EQUAL_ICASE, refstring);
	CPPUNIT_ASSERT(scf->accepts(value_match));
	CPPUNIT_ASSERT(!scf->accepts(value_nomatch));
}

void
StringColumnFilterTest::testRegexIcaseOp()
{
	char * refstring = strdup("^Ti[a-z]ky-Winky$");
	string * value_match = new string("tiskY-wiNkY");
	string * value_nomatch = new string("JUGGERNAUT");
	StringColumnFilter *scf = new StringColumnFilter(DUMMY_COLUMN, OP_REGEX_ICASE, refstring);
	CPPUNIT_ASSERT(scf->accepts(value_match));
	CPPUNIT_ASSERT(!scf->accepts(value_nomatch));
}

/**
 * Ensure that regex case insensitivity matching works for characters outside of ASCII
 **/
void
StringColumnFilterTest::testRegexIcaseNonAscii()
{
	char * refstring = strdup("te tse incoming: Ҵ");
	string * value_match = new string("Capital te tse incoming: Ҵ");
	string * value_match2 = new string("Small te tse incoming: ҵ");
	StringColumnFilter *scf = new StringColumnFilter(DUMMY_COLUMN, OP_REGEX_ICASE, refstring);
	CPPUNIT_ASSERT(scf->accepts(value_match));
	CPPUNIT_ASSERT(scf->accepts(value_match2));
}

void
StringColumnFilterTest::testGreaterOp()
{
	char * refstring = strdup("a relatively low value string");
	string * value_less = new string("littlestringer");
	string * value_greater = new string("AN EXTREEEEEMELY VALUABLE STRING WITH LOTS OF SHOUTING");
	StringColumnFilter *scf = new StringColumnFilter(DUMMY_COLUMN, OP_GREATER, refstring);
	CPPUNIT_ASSERT(scf->accepts(value_less));
	CPPUNIT_ASSERT(!scf->accepts(value_greater));
}

void
StringColumnFilterTest::testLessOp()
{
	char * refstring = strdup("a relatively low value string");
	string * value_less = new string("littlestringer");
	string * value_greater = new string("AN EXTREEEEEMELY VALUABLE STRING WITH LOTS OF SHOUTING");
	StringColumnFilter *scf = new StringColumnFilter(DUMMY_COLUMN, OP_LESS, refstring);
	CPPUNIT_ASSERT(!scf->accepts(value_less));
	CPPUNIT_ASSERT(scf->accepts(value_greater));
}

int main(int argc, char * argv[])
{
	CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
	CppUnit::TextTestRunner runner;
	setlocale(LC_ALL, "");
	runner.addTest(suite);

	runner.setOutputter(new CppUnit::CompilerOutputter( &runner.result(), std::cerr));

	bool wasSuccessful = runner.run();

	return wasSuccessful ? 0 : 1;

}
