/*
   bug-1953.c
 */

#include <testfwk.h>
#include <string.h>

char pat[8] = {'S', 'D', 'C', 'C', 0, 0, 0, 0};

char str_p0[8] = "SDCC";
char str_p1[8] = {'S', 'D', 'C', 'C'};

void testBug(void)
{
  char str_l0[8] = "SDCC";
  char str_l1[8] = {'S', 'D', 'C', 'C'};
  static char str_s0[8] = "SDCC";
  static char str_s1[8] = {'S', 'D', 'C', 'C'};

#if 0
  ASSERT (memcmp (str_p0, pat, sizeof (pat)) == 0);
#endif

  ASSERT (memcmp (str_p1, pat, sizeof (pat)) == 0);
  ASSERT (memcmp (str_l0, pat, sizeof (pat)) == 0);
  ASSERT (memcmp (str_l1, pat, sizeof (pat)) == 0);
  ASSERT (memcmp (str_s0, pat, sizeof (pat)) == 0);
  ASSERT (memcmp (str_s1, pat, sizeof (pat)) == 0);
}
