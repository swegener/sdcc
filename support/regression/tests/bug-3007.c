/* bug 3007
  type: char, int, long, long long
 */
#include <testfwk.h>

{type} a = 0;

void inc(void)
{
  a++;
}

void testBug(void)
{
  {type} i;
  for (i = 0; i < ({type})300; i++)
    inc();
  ASSERT (a == ({type})300);
}

