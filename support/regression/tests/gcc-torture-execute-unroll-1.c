/*
   unroll-1.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#ifndef __SDCC_pdk14 // Bug #2874
static
inline int
f (int x)
{
  return (x + 1);
}
#endif

void
testTortureExecute (void)
{
#ifndef __SDCC_pdk14 // Bug #2874
  int a = 0 ;
 
  while ( (f(f(f(f(f(f(f(f(f(f(1))))))))))) + a < 12 )
    {
      a++;
      return;
    }
  if (a != 1)
    ASSERT (0);
#endif
}

