/*
  bug-2239.c
*/

#include <testfwk.h>

static unsigned short buf[] = {0xaa55, 0xa5a5, 0xc33c, 0xf00f};

void testBug(void)
{
#ifndef __SDCC_stm8
  unsigned char *p = ((unsigned char *) buf) + 4;
  ASSERT (*((unsigned char *) (p + 2 - 4)) == 0xa5);
  ASSERT (*((unsigned char *) (p - 2)) == 0xa5);
  ASSERT (*((unsigned short *) (p + 3 - 7)) == 0xaa55);
  ASSERT (*((unsigned short *) (p - 4)) == 0xaa55);
#endif
}
