/*
strct-stdarg-1.c from the execute part of the gcc torture tests.
*/

#include <testfwk.h>

#include <stdarg.h>

struct tiny
{
  char c;
  char d;
  char e;
  char f;
  char g;
};

#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && \
  (!defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) || defined(__SDCC_STACK_AUTO)) // Todo: enable when struct parmeters are supported!
f (int n, ...)
{
  struct tiny x;
  int i;

  va_list ap;
  va_start (ap,n);
  for (i = 0; i < n; i++)
    {
      x = va_arg (ap,struct tiny);
      if (x.c != i + 10)
	ASSERT(0);
      if (x.d != i + 20)
	ASSERT(0);
      if (x.e != i + 30)
	ASSERT(0);
      if (x.f != i + 40)
	ASSERT(0);
      if (x.g != i + 50)
	ASSERT(0);
    }
  {
    long x = va_arg (ap, long);
    if (x != 123)
      ASSERT(0);
  }
  va_end (ap);
}
#endif

void
testTortureExecute (void)
{
#if 0 // Todo: enable when struct parmeters are supported!
  struct tiny x[3];
  x[0].c = 10;
  x[1].c = 11;
  x[2].c = 12;
  x[0].d = 20;
  x[1].d = 21;
  x[2].d = 22;
  x[0].e = 30;
  x[1].e = 31;
  x[2].e = 32;
  x[0].f = 40;
  x[1].f = 41;
  x[2].f = 42;
  x[0].g = 50;
  x[1].g = 51;
  x[2].g = 52;
  f (3, x[0], x[1], x[2], (long) 123);
  return;
#endif
}
