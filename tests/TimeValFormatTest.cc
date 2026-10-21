#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "TimeValFormatTest.h"
#include "../src/TimeValFormat.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TimeValFormatTest );

// The point of this formatter is that it is a *byte exact* replacement for the
// snprintf() call it replaced, so the test compares against snprintf() itself
// rather than against a hand written expectation.

void
TimeValFormatTest::setUp()
{
}

void
TimeValFormatTest::tearDown()
{
}

static uint64_t rng_state = 88172645463325252ULL;
static uint64_t rng()
{
	rng_state ^= rng_state << 13;
	rng_state ^= rng_state >> 7;
	rng_state ^= rng_state << 17;
	return rng_state;
}

void
TimeValFormatTest::testTimeValMatchesSnprintf()
{
	char buf[40], want[64];
	time_t secs = 0;
	long usec = 0;

	for (int i = 0; i < 100000; i++) {
		secs = (time_t)(rng() >> 24);
		usec = (long)(rng() % 1000000);
		if (i == 0) { secs = 0; usec = 0; }
		if (i == 1) { secs = 1790275176; usec = 665651; }
		if (i == 2) { secs = 0; usec = 999999; }
		if (i == 3) { secs = 100; usec = 999990; }
		if (i == 4) { secs = 1; usec = 1; }
		int len = formatTimeVal(buf, secs, usec);
		snprintf(want, sizeof(want), "%lu.%06lu", (unsigned long)secs, (unsigned long)usec);
		CPPUNIT_ASSERT_EQUAL(std::string(want), std::string(buf, (size_t)len));
	}
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
