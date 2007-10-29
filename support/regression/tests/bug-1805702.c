/* bug-1805702.c
 */

/* disabled */
#if 0
#include <testfwk.h>

int foo;
extern int foo;

void
test(void)
{
  foo = 10;

  ASSERT(foo == 10);
}
#endif
