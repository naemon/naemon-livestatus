#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <vector>
#include <naemon/naemon.h>
#include "HostlistColumnFilterTest.h"
#include "../src/HostlistColumn.h"
#include "../src/HostlistColumnFilter.h"
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

CPPUNIT_TEST_SUITE_REGISTRATION( HostlistColumnFilterTest );
void
HostlistColumnFilterTest::setUp()
{
}

void
HostlistColumnFilterTest::tearDown()
{
}

void
HostlistColumnFilterTest::testHostlistEmpty()
{
    char *refstring = strdup("");
    rbtree *tree = rbtree_create(compare_host);
	HostlistColumn * column = new HostlistColumn("hsts", "this is a hostlist", 0, -1, false);
	Filter * filter = column->createFilter(OP_EQUAL, refstring);

    /* It's hard to mock up a host... but what we want is for the test to
     * accept a non-empty pointer as empty list (after conversion from linked
     * list to rbtree)
     */
	CPPUNIT_ASSERT(filter->accepts(&tree));

    /* Adding a single host won't read the host content through compare. It's when comparing two */
    rbtree_insert(tree, (void*)1);
	CPPUNIT_ASSERT(!filter->accepts(&tree));

    rbtree_destroy(tree, NULL);
	delete filter;
	delete column;
	free(refstring);
}

void
HostlistColumnFilterTest::testHostlistNotEmpty()
{
    char *refstring = strdup("");
    rbtree *tree = rbtree_create(compare_host);
	HostlistColumn * column = new HostlistColumn("hsts", "this is a hostlist", 0, -1, false);
	Filter * filter = column->createFilter(-OP_EQUAL, refstring);

    /* It's hard to mock up a host... but what we want is for the test to
     * accept a non-empty pointer as empty list (after conversion from linked
     * list to rbtree)
     */
	CPPUNIT_ASSERT(!filter->accepts(&tree));

    /* Adding a single host won't read the host content through compare. It's when comparing two */
    rbtree_insert(tree, (void*)1);
	CPPUNIT_ASSERT(filter->accepts(&tree));

    rbtree_destroy(tree, NULL);
	delete filter;
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
