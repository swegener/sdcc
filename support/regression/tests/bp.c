/* Base pointer tests, specifically for the z80.
 */
#include <testfwk.h>
#include <string.h>
#include <stdio.h>

int
verifyBlock(char *p, char val, int len)
{
  while (len--) {
    if (*p++ != val) {
      return 0;
    }
  }
  return 1;
}

char
spoil(char a)
{
  return a;
}

void
testBP(void)
{
  char above[400];
  char f;
  char below[200];

  memset(above, 17, sizeof(above));
  memset(below, 74, sizeof(below));

  ASSERT(verifyBlock(above, 17, sizeof(above)));
  ASSERT(verifyBlock(below, 74, sizeof(below)));

  f = spoil(-5);
  spoil(f);

  ASSERT(verifyBlock(above, 17, sizeof(above)));
  ASSERT(verifyBlock(below, 74, sizeof(below)));
}

