/* bug-1805702.c
 */

#include <testfwk.h>

/* disabled */
#if 0
int foo;
extern int foo;
#endif

void
test(void)
{
/* disabled */
#if 0
  foo = 10;

  ASSERT(foo == 10);
#endif
}
