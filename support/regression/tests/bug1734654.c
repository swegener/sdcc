/* bug1734654.c
 */
#include <testfwk.h>
#include <stdbool.h>

volatile bool b;
volatile unsigned char c = 1;

static void foo (void)
{
  b = (c<2);
}

void
testMyFunc(void)
{
  foo ();
  ASSERT (b);
}
