/*
   bug-2776.c. Originally a segfault on trying to pass an array element as variable argument struct parameter.
   Later a iCode generation issue for pdk that triggered an assertion in codegen (when trying
   to pass an array element of size 1 as a variable argument struct parameter).
 */

#include <testfwk.h>

#pragma disable_warning 283
#pragma disable_warning 278

#include <stdarg.h>

#if !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) // Todo: enable when struct parameters are supported!
struct tiny
{
  char c;
};

void f (int n, ...);

m ()
{
  struct tiny x[3];
  x[0].c = 10;
  x[1].c = 11;
  x[2].c = 12;

  f (3, x[0], x[1], x[2], (long) 123);
}

void f (int n, ...)
{
	ASSERT (n == 3);
	struct tiny x;
	
	va_list args;
	va_start(args, n);
	x = va_arg (args, struct tiny);
	ASSERT (x.c == 10);
	x = va_arg (args, struct tiny);
	ASSERT (x.c == 11);
	x = va_arg (args, struct tiny);
	ASSERT (x.c == 12);

	ASSERT (va_arg (args, long) == 123);

	va_end (args);
}
#endif

void
testBug (void)
{
#if !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) // Todo: enable when struct parameters are supported!
  m ();
#endif
}



