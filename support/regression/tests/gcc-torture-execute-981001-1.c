/*
   981001-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#define NG   0x100L

#if !defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) // Lack of memory
unsigned long flg = 0;

long sub (int n) __reentrant
{
  int a, b ;

  __prints(" FAIL: n:");
  __printd(n);
  __prints("\n");
  
  if (n >= 2)
    {
      if (n % 2 == 0)
	{
	  __prints(" FAIL: even\n");
	  a = sub (n / 2);
	  
	  return (a + 2 * sub (n / 2 - 1)) * a;
	}
      else
	{
	  int tmp;
	  //  __prints(" FAIL: odd\n");
	  tmp=n/2+1;
	  //	  __prints(" FAIL: tmp=");
	  //	  __printd(tmp);
	  //	  __prints("\n");
	  //a = sub (n / 2 + 1);
	  a = sub (tmp);
	  b = sub (n / 2);
	  
	  return a * a + b * b;
	}
    }
  else 
    return (long) n;
}
#endif

void
testTortureExecute (void)
{
#if !defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) // Lack of memory
  long t;
  t=sub(30);

  __prints(" FAIL: t:");
  __printd((int)t);
  __prints("\n");
  
    if (sub (30) != 832040L)
      flg |= NG;

  if (flg)
    ASSERT (0);
  
  return;
#endif
}

