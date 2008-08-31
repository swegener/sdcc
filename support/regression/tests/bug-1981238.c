/*
    bug1981238.c
*/
#include <testfwk.h>

code struct {
  char x:1;
  char  :0;
  char d:2;
  char b:6;
} pad = {1, 2, 1};

code struct {
  int p:1;
  int q:7;
  char :0;
  int s:1;
  int t:7;
} noPad = {1, 120, 1, 1, 127};

code struct {
  char  :0;
  char b;
} initialNoPad[] = {{1, 2}, {3,4}};

void testBitfield (void)
{
  ASSERT (sizeof (pad) == 2);
  ASSERT (sizeof (noPad) == 2);
  ASSERT (sizeof (initialNoPad[0]) == 1);
}
