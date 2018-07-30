/*
wchar_t-1.c from the execute part of the gcc torture tests.
*/

#include <testfwk.h>

#include <wchar.h>

/* { dg-options "-finput-charset=utf-8" } */
#if 0 // SDCC bug #2791 in handling wide string literals in initialization
wchar_t x[] = L"Ä";
wchar_t y = L'Ä';
#endif

void
testTortureExecute (void)
{
#if 0
  if (sizeof (x) / sizeof (wchar_t) != 2)
    ASSERT (0);
  if (x[0] != L'Ä' || x[1] != L'\0')
    ASSERT (0);
  if (y != L'Ä')
    ASSERT (0);
  return;
#endif
}
