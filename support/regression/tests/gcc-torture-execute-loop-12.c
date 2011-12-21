/*
   loop-12.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

#if !defined(SDCC_ds390) && !defined(SDCC_hc08)

/* Checks that pure functions are not treated as const.  */

const char *p;

static int
is_end_of_statement (void)
{
  return *p == '\n' || *p == ';' || *p == '!';
}

void foo (void)
{
  /* The is_end_of_statement call was moved out of the loop at one stage,
     resulting in an endless loop.  */
  while (!is_end_of_statement ())
    p++;
}
#endif

void
testTortureExecute (void)
{
#if !defined(SDCC_ds390) && !defined(SDCC_hc08)
  p = "abc\n";
  foo ();
  return;
#endif
}
