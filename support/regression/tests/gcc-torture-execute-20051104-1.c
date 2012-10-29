/*
   20051104-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#pragma disable_warning 196
#endif

/* PR rtl-optimization/23567 */

struct
{
  int len;
  char *name;
} s;

void
testTortureExecute (void)
{
  s.len = 0;
  s.name = "";
  if (s.name [s.len] != 0)
    s.name [s.len] = 0;
  return;
}
