/*
   bug2805333.c
*/

#include <testfwk.h>
#include <stdbool.h>

#ifndef BOOL
#define BOOL bool
#endif

void g(char c)
{
  c;
}

BOOL b;

void test_bug(void)
{
  g(!b); /* Causes SDCC 2.9.0 to segfault.. */
}
