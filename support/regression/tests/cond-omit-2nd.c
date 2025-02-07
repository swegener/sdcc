/** Test conditional operator with omitted second operator (GNU extension)
 */

#include <testfwk.h>

#if defined(__SDCC) || defined(__GNUC__)
int func1(int a, int b)
{
  return a ?: b;
}

int func2(int a, int b)
{
  return ++a ?: b;
}
#endif

void testCondOpOmitted2nd(void)
{
#if defined(__SDCC) || defined(__GNUC__)
  ASSERT((0 ?: 2) == 2);
  ASSERT((1 ?: 2) == 1);
  ASSERT(func1(0, 2) == 2);
  ASSERT(func1(1, 2) == 1);
  ASSERT(func2(-1, 5) == 5);
  ASSERT(func2(1, 5) == 2);
#endif
}
