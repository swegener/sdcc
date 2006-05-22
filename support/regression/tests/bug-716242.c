/* bug-716242.c

   syntax tests about function pointers at compile time
 */
#include <testfwk.h>

void *p;
int ret;

int mul2 (int i)
{
  return 2 * i;
}

void g (int (*h) (int))
{
  ret = h (2);
}

void f1()
{
#if defined(SDCC_ds390)
  p = (void code *) mul2;
#else
  p = (void *) mul2;
#endif
  g ((int (*) (int)) p);
}

/****************************/

#ifndef SDCC_pic16
// Ifdefed out for PIC16 port since it generates the error:
// Duplicate label or redefinin g symbol that cannot be redefined. (_g)
// Submitted to the bug tracker:
// [ 1443651 ] pic 16: redefining symbol that cannot be redefined. (_g)

void g (int (*h) (int));
#endif

void f2()
{
  int (*fp) (int) = p;

  g (fp);
}

/****************************/

#ifndef SDCC_pic16
// Ifdefed out for PIC16 port since it generates the error:
// Duplicate label or redefinin g symbol that cannot be redefined. (_g)
// Submitted to the bug tracker:
// [ 1443651 ] pic 16: redefining symbol that cannot be redefined. (_g)

void g (int (*h) (int));
#endif

void f3()
{
  int (*fp) (int) = (int (*) (int)) p;

  g (fp);
}

/****************************/

void f4()
{
  ((void (code *) (void)) p) ();
}

/****************************/

void f5()
{
  int (*fp) (int) = mul2;

  fp(1);
}

/****************************/

void f6()
{
  ((void (code *) (void)) 0) ();
}

/****************************/

static void
testFuncPtr(void)
{
  f1();
  ASSERT(ret == 4);
}
