/* bug-983491.c
 */

#include <testfwk.h>

code struct {
  char* b;
} c[2] = {
  {"abc"},
  {"abc"}
};

testMergeStr(void)
{
  ASSERT(c[0].b == c[1].b);
}
