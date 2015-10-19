/*
   bug-2089.c 
 */

#include <testfwk.h>
#include <string.h>

struct t0_t {
  char *p2;
  char v2[5];
};

struct t1_t {
  char *p0;
  char v0[5];
  char *p1;
  char v1[5];
  struct t0_t t;
} ww = {"11", "22", "33", "44", {"55", "66"}};

const char p0[] = {'1', '1', 0, 0, 0};
const char v0[] = {'2', '2', 0, 0, 0};
const char p1[] = {'3', '3', 0, 0, 0};
const char v1[] = {'4', '4', 0, 0, 0};
const char p2[] = {'5', '5', 0, 0, 0};
const char v2[] = {'6', '6', 0, 0, 0};

void testBug (void)
{
  ASSERT (strcmp (ww.p0, p0) == 0);
  ASSERT (strcmp (ww.p1, p1) == 0);
  ASSERT (strcmp (ww.t.p2, p2) == 0);

  ASSERT (memcmp (ww.v0, v0, sizeof (v0)) == 0);
  ASSERT (memcmp (ww.v1, v1, sizeof (v1)) == 0);
  ASSERT (memcmp (ww.t.v2, v2, sizeof (v2)) == 0);
}
