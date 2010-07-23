/*
   bug3027957.c
 */

#include <testfwk.h>

void foo(void)
{
  ((unsigned char __xdata *)0xF000)[100] = 0x12;
}

void testBug(void)
{
#ifdef SDCC
  foo();
  ASSERT (*(unsigned char __xdata *)(0xF064) == 0x12);
#endif
}
