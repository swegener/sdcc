/** Simple set of tests for floating pt.
 */
#include <testfwk.h>

void
testCmp(void)
{
  volatile float left, right;

  left = 5;
  right = 13;
  ASSERT(left + right == 18);
  ASSERT(left + right <= 18);
  ASSERT(left + right >= 18);
  ASSERT(left + right > 17.9);
  ASSERT(left + right < 18.1);
}

void
testDiv(void)
{
#if !PORT_HOST
  volatile float left, right;

  left = 17;
  right = 343;

  ASSERT(left/right == (17.0/343.0));
  ASSERT(right/left == (343.0/17.0));

  right = 17;
  ASSERT(left/right == 1.0);
#endif
}

void
testDivNearOne(void)
{
  volatile float left, right, result;

  left = 12392.4;
  right = 12392.4;
  result = left / right;

  if (result > 0.999999)
    {
      /* Fine */
    }
  else
    {
      FAIL();
    }
  if (result < 1.00001)
    {
      /* Fine */
    }
  else
    {
      FAIL();
    }
  if (result > 0.999999 && result < 1.00001)
    {
      /* Fine */
    }
  else
    {
      FAIL();
    }
}
