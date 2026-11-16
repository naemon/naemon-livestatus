#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include "DoubleFormatTest.h"
#include "../src/DoubleFormat.h"

CPPUNIT_TEST_SUITE_REGISTRATION( DoubleFormatTest );

// Whole numbers are printed short with a decimal point ("60.0"), everything else keeps the exact
// "%.10e" format.  The oracle below expresses that contract with snprintf() only, so these tests do
// not reimplement the optimisation they check.
static bool isWholeNumber(double value)
{
	return fabs(value) <= 9007199254740992.0 && (double)(int64_t)value == value;
}

static std::string expectedText(double value)
{
	char want[64];
	if (isWholeNumber(value)) {
		snprintf(want, sizeof(want), "%.1f", value);
	} else {
		snprintf(want, sizeof(want), "%.10e", value);
	}
	return want;
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
DoubleFormatTest::setUp()
{
}

void
DoubleFormatTest::tearDown()
{
}

void
DoubleFormatTest::testDoublesMatchSnprintf()
{
	static const double values[] = {
		0.0, -0.0, 1.0, -1.0, 0.1, 0.5, 30.0, 60.0, 5.0e-3, 1e308, -1e308, 1e-308,
		4.9406564584124654e-324, 2.2250738585072014e-308, 1.7976931348623157e308,
		3.14159265358979, -0.0000000001, 1e15, 1e16, 12345.6789, 180.47248292,
		994637.34889
	};
	char got[64];

	for (unsigned i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
		int len = formatDouble(got, sizeof(got), values[i]);
		CPPUNIT_ASSERT_EQUAL(expectedText(values[i]), std::string(got, (size_t)len));
		// the short whole-number form is exact and has to round-trip; "%.10e" only
		// carries 11 significant digits, so those are compared by text only
		if (isWholeNumber(values[i])) {
			CPPUNIT_ASSERT(strtod(got, NULL) == values[i]);
		}
	}

	/* random bit patterns: NaN payloads, infinities and denormals included */
	for (int i = 0; i < 200000; i++) {
		uint64_t bits = rng();
		double d;
		memcpy(&d, &bits, sizeof(d));
		int len = formatDouble(got, sizeof(got), d);
		CPPUNIT_ASSERT_EQUAL(expectedText(d), std::string(got, (size_t)len));
	}
}

void
DoubleFormatTest::testWholeNumbersKeepTheirSign()
{
	char got[64];

	int len = formatDouble(got, sizeof(got), -0.0);
	CPPUNIT_ASSERT_EQUAL(std::string("-0.0"), std::string(got, (size_t)len));
	CPPUNIT_ASSERT(signbit(strtod(got, NULL)));

	len = formatDouble(got, sizeof(got), 9007199254740992.0);
	CPPUNIT_ASSERT_EQUAL(std::string("9007199254740992.0"), std::string(got, (size_t)len));
	CPPUNIT_ASSERT(strtod(got, NULL) == 9007199254740992.0);
}

int main(int argc, char * argv[])
{
	CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
	CppUnit::TextTestRunner runner;
	setlocale(LC_ALL, "");
	runner.addTest(suite);
	runner.setOutputter(new CppUnit::CompilerOutputter( &runner.result(), std::cerr));
	return runner.run() ? 0 : 1;
}
