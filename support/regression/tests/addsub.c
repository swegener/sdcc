/** Add, sub tests.

    type: signed char, int, long
    storage: static, 
    attr: volatile
*/
#include <testfwk.h>

void 
testAdd(void)
{
  {storage} {attr} {type} left, right, result;

  left = 5;
  right = 26;

  result = left+right;
  ASSERT(result == 31);
  
  left = 39;
  right = -120;
  
  result = left+right;
  ASSERT(result == (39-120));
}

void 
testSub(void)
{
  {storage} {attr} {type} left, right, result;

  left = 5;
  right = 26;

  result = left-right;
  ASSERT(result == (5-26));
  
  left = 39;
  right = -76;
  
  result = left-right;
  ASSERT(result == (39+76));
}
