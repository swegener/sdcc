/*
   bug1546986.c
*/

#include <testfwk.h>
#include <stdbool.h>

#ifdef __bool_true_false_are_defined

static unsigned char pdata tst1 = 0x01;
static unsigned char pdata tst2 = 0x00;

static bool test;

#endif //__bool_true_false_are_defined

void
testBug(void)
{
#ifdef __bool_true_false_are_defined
  test = (tst1 | tst2);
  ASSERT( test );
  test = (tst2 | tst1);
  ASSERT( test );
#endif //__bool_true_false_are_defined
}
