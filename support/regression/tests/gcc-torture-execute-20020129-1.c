/*
   20020129-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

/* This testcase failed at -O2 on IA-64, because scheduling did not take
   into account conditional execution when using cselib for alias
   analysis.  */

struct D { int d1; struct D *d2; };
struct C { struct D c1; long c2, c3, c4, c5, c6; };
struct A { struct A *a1; struct C *a2; };
struct B { struct C b1; struct A *b2; };

#ifndef __SDCC_pdk14 // Lack of memory
void
foo (struct B *x, struct B *y)
{
  if (x->b2 == 0)
    {
      struct A *a;

      x->b2 = a = y->b2;
      y->b2 = 0;
      for (; a; a = a->a1)
	a->a2 = &x->b1;
    }

  if (y->b2 != 0)
    ASSERT (0);

  if (x->b1.c3 == -1)
    {
      x->b1.c3 = y->b1.c3;
      x->b1.c4 = y->b1.c4;
      y->b1.c3 = -1;
      y->b1.c4 = 0;
    }

  if (y->b1.c3 != -1)
    ASSERT (0);
}

struct B x, y;
#endif

void
testTortureExecute (void)
{
#ifndef __SDCC_pdk14 // Lack of memory
  y.b1.c1.d1 = 6;
  y.b1.c3 = 145;
  y.b1.c4 = 2448;
  x.b1.c3 = -1;
  foo (&x, &y);
#endif
}

