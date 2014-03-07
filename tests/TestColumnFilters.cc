#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <vector>
#include "TestColumnFilters.h"
#include "../src/OffsetStringServiceMacroColumn.h"
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
	string value_eq("Tinky-Winky");
	string value_neq("Po");
	OffsetStringColumn * column = new OffsetStringColumn("some-column", "a description for the column", 0);
	StringColumnFilter * scf = new StringColumnFilter(column, OP_EQUAL, refstring);
	CPPUNIT_ASSERT(scf->accepts(&value_eq));
	CPPUNIT_ASSERT(!scf->accepts(&value_neq));
	delete scf;
	delete column;
	free(refstring);
}

void
StringColumnFilterTest::testRegexOp()
{
	char * refstring = strdup("Dell open");
	const char *values[] = {"my Dell open manage service", "i Dell open manage", "Dell open manage global", "Dell open manage storage"};
	vector<string> value_match_v (values, values+(sizeof(values)/sizeof(values[0])));
	string value_nomatch("Dipsy");
	OffsetStringColumn * column = new OffsetStringColumn("some-column", "a description for the column", 0);
	StringColumnFilter *scf = new StringColumnFilter(column, OP_REGEX, refstring);
	for (vector<string>::iterator it = value_match_v.begin(); it != value_match_v.end(); ++it) {
		CPPUNIT_ASSERT(scf->accepts(&*it));
	}
	CPPUNIT_ASSERT(!scf->accepts(&value_nomatch));
	delete scf;
	delete column;
	free(refstring);
}

void
StringColumnFilterTest::testRegexNonAscii()
{
	char * refstring = strdup("^.*Յ$");
	string value_match("Capital yi: Յ");
	string value_nomatch("Small yi: յ");
	OffsetStringColumn * column = new OffsetStringColumn("some-column", "a description for the column", 0);
	StringColumnFilter *scf = new StringColumnFilter(column, OP_REGEX, refstring);
	CPPUNIT_ASSERT(scf->accepts(&value_match));
	CPPUNIT_ASSERT(!scf->accepts(&value_nomatch));
	delete scf;
	delete column;
	free(refstring);
}

void
StringColumnFilterTest::testEqualIcaseOp()
{
	char * refstring = strdup("Tinky-Winky");
	string value_match("tInkY-WinKy");
	string value_nomatch("DIPsY");
	OffsetStringColumn * column = new OffsetStringColumn("some-column", "a description for the column", 0);
	StringColumnFilter *scf = new StringColumnFilter(column, OP_EQUAL_ICASE, refstring);
	CPPUNIT_ASSERT(scf->accepts(&value_match));
	CPPUNIT_ASSERT(!scf->accepts(&value_nomatch));
	delete scf;
	delete column;
	free(refstring);
}

void
StringColumnFilterTest::testRegexIcaseOp()
{
	char * refstring = strdup("^Ti[a-z]ky-Winky$");
	string value_match("tiskY-wiNkY");
	string value_nomatch("JUGGERNAUT");
	OffsetStringColumn * column = new OffsetStringColumn("some-column", "a description for the column", 0);
	StringColumnFilter *scf = new StringColumnFilter(column, OP_REGEX_ICASE, refstring);
	CPPUNIT_ASSERT(scf->accepts(&value_match));
	CPPUNIT_ASSERT(!scf->accepts(&value_nomatch));
	delete scf;
	delete column;
	free(refstring);
}

/**
 * Ensure that regex case insensitivity matching works for characters outside of ASCII
 **/
void
StringColumnFilterTest::testRegexIcaseNonAscii()
{
	char * refstring = strdup("te tse incoming: Ҵ");
	string value_match("Capital te tse incoming: Ҵ");
	string value_match2("Small te tse incoming: ҵ");
	OffsetStringColumn * column = new OffsetStringColumn("some-column", "a description for the column", 0);
	StringColumnFilter *scf = new StringColumnFilter(column, OP_REGEX_ICASE, refstring);
	CPPUNIT_ASSERT(scf->accepts(&value_match));
	CPPUNIT_ASSERT(scf->accepts(&value_match2));
	delete scf;
	delete column;
	free(refstring);
}

void
StringColumnFilterTest::testGreaterOp()
{
	char * refstring = strdup("a relatively low value string");
	string value_less("littlestringer");
	string value_greater("AN EXTREEEEEMELY VALUABLE STRING WITH LOTS OF SHOUTING");
	OffsetStringColumn * column = new OffsetStringColumn("some-column", "a description for the column", 0);
	StringColumnFilter *scf = new StringColumnFilter(column, OP_GREATER, refstring);
	CPPUNIT_ASSERT(scf->accepts(&value_less));
	CPPUNIT_ASSERT(!scf->accepts(&value_greater));
	delete scf;
	delete column;
	free(refstring);
}

void
StringColumnFilterTest::testLessOp()
{
	char * refstring = strdup("a relatively low value string");
	string value_less("littlestringer");
	string value_greater("AN EXTREEEEEMELY VALUABLE STRING WITH LOTS OF SHOUTING");
	OffsetStringColumn * column = new OffsetStringColumn("some-column", "a description for the column", 0);
	StringColumnFilter *scf = new StringColumnFilter(column, OP_LESS, refstring);
	CPPUNIT_ASSERT(!scf->accepts(&value_less));
	CPPUNIT_ASSERT(scf->accepts(&value_greater));
	delete scf;
	delete column;
	free(refstring);
}

void
StringColumnFilterTest::testOffsetStringMacroColumnFiltering()
{
	char * refstring = strdup("A search String");
	const char *values[] = {"a search string", "some tEXT containing a search strING and also some other stuff", "a seaRCH STring"};
	vector<string> value_match_v (values, values+(sizeof(values)/sizeof(values[0])));
	string value_nomatch("Dipsy");
	OffsetStringServiceMacroColumn * column = new OffsetStringServiceMacroColumn( "a string macro column", "and a description for it", 0);
	StringColumnFilter *scf = new StringColumnFilter(column, OP_REGEX_ICASE, refstring);
	for (vector<string>::iterator it = value_match_v.begin(); it != value_match_v.end(); ++it) {
		CPPUNIT_ASSERT(scf->accepts(&*it));
	}
	CPPUNIT_ASSERT(!scf->accepts(&value_nomatch));
	delete scf;
	delete column;
	free(refstring);

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
