/*
   bug1546986.c
*/

#include <testfwk.h>
#include <stdbool.h>

static unsigned char pdata tst1 = 0x01;
static unsigned char pdata tst2 = 0x00;

static bool test;

void
testBug(void)
{
  test = (tst1 | tst2);
  ASSERT( test );
  test = (tst2 | tst1);
  ASSERT( test );
}
