/** Compare with 2^n greater then 255
    test for patch #2702889 - Summary of all uncommitted changes I applied on "my" SDCC
 
    int_right: 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0x00ff, 0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff
*/

#include <testfwk.h>

int
lwr (unsigned left)
{
  if (left < {int_right})
    return 1;
  else
    return 0;
}

int
gtr (unsigned left)
{
  return (left > {int_right}) ? 1 : 0;
}

#define LONG_RIGHT_LWR 0x80000000
int
long_lwr (unsigned long left)
{
  return (left < LONG_RIGHT_LWR) ? 1 : 0;
}

#define LONG_RIGHT_GTR 0x7fffffff
int
long long_gtr (unsigned long left)
{
  if (left > LONG_RIGHT_GTR)
    return 1;
  else
    return 0;
}

void
test_lwr_gtr (void)
{
  ASSERT (lwr ({int_right} - 1));
  ASSERT (!lwr ({int_right}));
  ASSERT (!lwr ({int_right} + 1));
  ASSERT (gtr ({int_right} + 1));
  ASSERT (!gtr ({int_right}));
  ASSERT (!gtr ({int_right} - 1));

  ASSERT (long_lwr (LONG_RIGHT_LWR - 1));
  ASSERT (!long_lwr (LONG_RIGHT_LWR));
  ASSERT (!long_lwr (LONG_RIGHT_LWR + 1));

  ASSERT (long_gtr (LONG_RIGHT_GTR + 1));
  ASSERT (!long_gtr (LONG_RIGHT_GTR));
  ASSERT (!long_gtr (LONG_RIGHT_GTR - 1));
}
