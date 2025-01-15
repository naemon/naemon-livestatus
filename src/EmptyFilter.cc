#include <stdlib.h>
#include "EmptyFilter.h"
#include "EmptyColumn.h"
#include "opids.h"
#include "logger.h"
#include "OutputBuffer.h"
    EmptyFilter::EmptyFilter(Column *column, int opid, char *value)
    : _column(column)
    , _opid(abs(opid))
{
}

EmptyFilter::~EmptyFilter()
{
}


bool EmptyFilter::accepts(void *data)
{
    bool pass = false;

    return pass != _negate;
}

void *EmptyFilter::indexFilter(const char *column)
{
    return 0;
}
