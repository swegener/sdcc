#include "izt.h"

void iemit(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    vprintf(format, ap);
    printf("\n");

    va_end(ap);
}
