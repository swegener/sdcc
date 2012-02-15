/*
   pr37924.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

/* PR c/37924 */


signed char a;
unsigned char b;

#if !(defined (__GNUC__) && defined (__GNUC_MINOR__) && (__GNUC__ < 5 && __GNUC_MINOR < 4))
int
test1 (void)
{
  int c = -1;
  return ((unsigned int) (a ^ c)) >> 9;
}

int
test2 (void)
{
  int c = -1;
  return ((unsigned int) (b ^ c)) >> 9;
}
#endif

void
testTortureExecute (void)
{
#if !(defined (__GNUC__) && defined (__GNUC_MINOR__) && (__GNUC__ < 5 && __GNUC_MINOR < 4))
  a = 0;
  if (test1 () != (-1U >> 9))
    ASSERT (0);
  a = 0x40;
  if (test1 () != (-1U >> 9))
    ASSERT (0);
  a = 0x80;
  if (test1 () != (a < 0) ? 0 : (-1U >> 9))
    ASSERT (0);
  a = 0xff;
  if (test1 () != (a < 0) ? 0 : (-1U >> 9))
    ASSERT (0);
  b = 0;
  if (test2 () != (-1U >> 9))
    ASSERT (0);
  b = 0x40;
  if (test2 () != (-1U >> 9))
    ASSERT (0);
  b = 0x80;
  if (test2 () != (-1U >> 9))
    ASSERT (0);
  b = 0xff;
  if (test2 () != (-1U >> 9))
    ASSERT (0);
  return;
#endif
}

