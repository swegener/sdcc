/*
   921013-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

void f(int *d, float *x, float *y,int n)
{
  while(n--){*d++=*x++==*y++;}
}

void
testTortureExecute (void)
{
#ifndef __SDCC_ds390
  int r[4];
  float a[]={5,1,3,5};
  float b[]={2,4,3,0};
  int i;
  f(r,a,b,4);
  for(i=0;i<4;i++)
    if((a[i]==b[i])!=r[i])
      ASSERT(0);
  return;
#endif
}

