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

void
testTortureExecute (void)
{
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
#if !defined(__SDCC_stm8) // Bug (assertion fails in code generation)
#if !defined(__SDCC_z80) && !defined(__SDCC_z180) && !defined(__SDCC_sm83) && !defined(__SDCC_ez80_z80) && !defined(__SDCC_r2k) && !defined(__SDCC_r2ka) && !defined(__SDCC_r3ka) && !defined(__SDCC_tlcs90) && !defined(__SDCC_z80n)// Bug (test fails)
#if !defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) // TODO: Enable when pdk supports struct argument of odd size!
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) // Bug (fails test)
  f (3, x[0], x[1], x[2], (long) 123);
#endif
#endif
#endif
#endif
  return;
}
