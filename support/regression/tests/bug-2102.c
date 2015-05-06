/** bug-2102.c
*/
#include <testfwk.h>
#include <stdlib.h>

#pragma disable_warning 219

struct tst {
  char a;
  char b;
  char c;
  short p[0];
};

void
testBug(void)
{
  struct tst w[2] = {{0x55, 0x33, 0x11}, {0x11, 0x22, 0x33}}; // will rise warning 219
  struct tst *p = w; // won't rise warning 219
  ASSERT (w[1].a == 0x11);
  ASSERT (w[1].b == 0x22);
  ASSERT (w[1].c == 0x33);
  p->p[0] = 0x4444;
  ASSERT (w[1].a == 0x44);
  ASSERT (w[1].b == 0x44);
  ASSERT (w[1].c == 0x33);
}
