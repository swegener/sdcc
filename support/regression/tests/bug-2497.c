/*
   bug-2497.c
 */

#include <testfwk.h>

#ifndef __SDCC_pdk14
typedef int func_t (int a);
typedef int (*func_p_t)(int a);

int fa (int a)
{
  return a + 1;
}

int fb (int b)
{
  return b + 10;
}

func_t *pa[2] = {fa, fb};
func_p_t pb[2] = {fb, fa};
#endif

void testBug (void)
{
#ifndef __SDCC_pdk14
  ASSERT (pa[0](5) == 6);
  ASSERT (pa[1](5) == 15);
  ASSERT (pb[0](15) == 25);
  ASSERT (pb[1](15) == 16);
#endif
}
