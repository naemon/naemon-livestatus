#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <vector>
#include "LogEntryTest.h"
#include "../src/LogEntry.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEntryTest );
void
LogEntryTest::setUp()
{
}

void
LogEntryTest::tearDown()
{
}

void
LogEntryTest::testParserHostState()
{
	char * line = strdup("[1570658402] CURRENT HOST STATE: testhost;UP;HARD;1;PING OK - Packet loss = 0%, RTA = 38.65 ms");
	LogEntry * entry = new LogEntry(1, line);
	CPPUNIT_ASSERT_EQUAL((unsigned int) 1, entry->_lineno);
	CPPUNIT_ASSERT_EQUAL((unsigned int) LOGCLASS_STATE, entry->_logclass);
	CPPUNIT_ASSERT_EQUAL(0, entry->_state);
	free(line);
}

void
LogEntryTest::testParserIncompleteLine()
{
	char * line = strdup("[1570658402] CURRENT HOST STATE:");
	LogEntry * entry = new LogEntry(1, line);
	CPPUNIT_ASSERT_EQUAL((unsigned int) 1, entry->_lineno);
	free(line);
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
