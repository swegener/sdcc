/*
   pr68390.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

/* { dg-do run }  */
/* { dg-options "-O2" } */

double direct(int x, ...)
{
  return x*x;
}

double broken(double (*indirect)(int x, ...), int v)
{
  return indirect(v);
}

void
testTortureExecute (void)
{
  double d1;
  int i = 2;
  d1 = broken (direct, i);
  ASSERT (d1 == i*i);
  return;
}

