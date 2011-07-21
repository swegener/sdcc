/** Compare with 2^n greater then 255
    test for patch #2702889 - Summary of all uncommitted changes I applied on "my" SDCC
 
    right: 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000
*/

#include <testfwk.h>

int
lwr (unsigned left)
{
  if (left < {right})
    return 1;
  else
    return 0;
}

int
gtr (unsigned left)
{
  if (left > {right})
    return 1;
  else
    return 0;
}

void
test_lwr_gtr (void)
{
  ASSERT (lwr ({right} - 1));
  ASSERT (!lwr ({right}));
  ASSERT (!lwr ({right} + 1));
  ASSERT (gtr ({right} + 1));
  ASSERT (!gtr ({right}));
  ASSERT (!gtr ({right} - 1));
}
