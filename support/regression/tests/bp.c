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

#ifndef SDCC_mcs51

#define ABOVE_MEM_SIZE      400
#define ABOVE_MEM_TEST_SIZE  17
#define BELOW_MEM_SIZE      200
#define BELOW_MEM_TEST_SIZE  74

#else

// test mcs51 with much less memory
#define ABOVE_MEM_SIZE       35
#define ABOVE_MEM_TEST_SIZE  17
#define BELOW_MEM_SIZE       20
#define BELOW_MEM_TEST_SIZE   7

#endif

void
testBP(void)
{
  char above[ABOVE_MEM_SIZE];
  char f;
  char below[BELOW_MEM_SIZE];

  memset(above, ABOVE_MEM_TEST_SIZE, sizeof(above));
  memset(below, BELOW_MEM_TEST_SIZE, sizeof(below));

  ASSERT(verifyBlock(above, ABOVE_MEM_TEST_SIZE, sizeof(above)));
  ASSERT(verifyBlock(below, BELOW_MEM_TEST_SIZE, sizeof(below)));

  f = spoil(-5);
  spoil(f);

  ASSERT(verifyBlock(above, ABOVE_MEM_TEST_SIZE, sizeof(above)));
  ASSERT(verifyBlock(below, BELOW_MEM_TEST_SIZE, sizeof(below)));
}
