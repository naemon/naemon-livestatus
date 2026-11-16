#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "IntFormatTest.h"
#include "../src/IntFormat.h"

CPPUNIT_TEST_SUITE_REGISTRATION( IntFormatTest );

// The point of these formatters is that they are *byte exact* replacements for the
// snprintf() calls they replaced, so every test here compares against snprintf()
// itself rather than against a hand written expectation.

void
IntFormatTest::setUp()
{
}

void
IntFormatTest::tearDown()
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
IntFormatTest::testIntegersMatchSnprintf()
{
	static const int64_t values[] = {
		0, 1, -1, 9, 10, -9, -10, 42, 99, 100, -99, -100, 127, 128, 255, 256,
		32767, 32768, 65535, 65536, 2147483647LL, -2147483648LL, 4294967295LL,
		4294967296LL, 999999999999LL, -999999999999LL,
		9223372036854775807LL, -9223372036854775807LL, INT64_MIN
	};
	char buf[24], want[64];

	for (unsigned i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
		char *end = buf + sizeof(buf);
		char *p = formatIntegerBackward(end, values[i]);
		snprintf(want, sizeof(want), "%lld", (long long)values[i]);
		CPPUNIT_ASSERT_EQUAL(std::string(want), std::string(p, (size_t)(end - p)));
	}

	for (int i = 0; i < 200000; i++) {
		int64_t v = (int64_t)rng();
		char *end = buf + sizeof(buf);
		char *p = formatIntegerBackward(end, v);
		snprintf(want, sizeof(want), "%lld", (long long)v);
		CPPUNIT_ASSERT_EQUAL(std::string(want), std::string(p, (size_t)(end - p)));
	}
}

void
IntFormatTest::testUnsignedMatchSnprintf()
{
	static const uint64_t values[] = { 0, 1, 9, 10, 255, 65535, 4294967295ULL,
	                                   9223372036854775807ULL, UINT64_MAX };
	char buf[24], want[64];

	for (unsigned i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
		char *end = buf + sizeof(buf);
		char *p = formatUnsignedBackward(end, values[i]);
		snprintf(want, sizeof(want), "%llu", (unsigned long long)values[i]);
		CPPUNIT_ASSERT_EQUAL(std::string(want), std::string(p, (size_t)(end - p)));
	}

	for (int i = 0; i < 200000; i++) {
		uint64_t v = rng();
		char *end = buf + sizeof(buf);
		char *p = formatUnsignedBackward(end, v);
		snprintf(want, sizeof(want), "%llu", (unsigned long long)v);
		CPPUNIT_ASSERT_EQUAL(std::string(want), std::string(p, (size_t)(end - p)));
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
