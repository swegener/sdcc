/*
   bug-2271.c
*/

#include <testfwk.h>

extern int foo0 (int *[]);
extern int foo1 (int (*[])(int));

int a = 56;
int b = 33;
int *p[2] = {&a, &b};

int foo0 (int *p[])
{
  return *p[0] + *p[1];
}

int foo1 (int (*p[])(int))
{
  return p ? 1 : 0;
}

void testBug (void)
{
  ASSERT (foo0 (p) == 89);
}
