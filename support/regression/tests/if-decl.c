/** Test if-declarations (C2y) */

#include <testfwk.h>

#ifdef __SDCC
_Pragma("std_c2y")

bool foo(int i)
{
  if (int j = i + 1; j != 0)
    return j;
  else
    return false;
}

bool bar(int i)
{
  switch (int j = i + 1)
  {
    case 0:
      return j;
    default:
      return true;
  }
}
#endif

void testIfDecl(void)
{
#ifdef __SDCC
  ASSERT(foo(0) == true);
  ASSERT(foo(-1) == false);
  ASSERT(bar(0) == true);
  ASSERT(bar(-1) == false);
#endif
}
