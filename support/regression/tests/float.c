/* Simple floating pt tests.
 */
#include <testfwk.h>

void
testFloatAdd(void)
{
  volatile float result, left, right;

  left = 4;
  right = 5;
  ASSERT(left+right == 9);

  left = 7;
  right = -3;
  ASSERT(left+right == 4);

  left = -1234;
  right = 409;
  ASSERT(left+right == (-1234+409));

  left = -34567;
  right = -123456;
  ASSERT(left+right == (-34567-123456));
}
