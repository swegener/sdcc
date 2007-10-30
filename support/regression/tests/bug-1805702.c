/* bug-1805702.c
 */

#include <testfwk.h>

/* disabled */
#if 0
int foo;
extern int foo;

void
test(void)
{
  foo = 10;

  ASSERT(foo == 10);
}
#endif
