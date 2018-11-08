/* bug-2833.c
   A problem resulting in compile-time error on some casts from bool to float.
 */

#include <testfwk.h>

#if 0
float cast1 (float a)
{
  return !(int)a;
}

float cast2 (_Bool b)
{
  return b;
}
#endif

const float b0 = 0.0f;
const float b1 = 1.0f;

void testBug(void)
{
#if 0
  ASSERT (cast1 (1.0f) == b0);
  ASSERT (cast1 (0.0f) == b1);

  ASSERT (cast2 (0.0f) == b0);
  ASSERT (cast2 (1.0f) == b1);
#endif
}

