/** tests for strspn
*/
#include <testfwk.h>
#include <string.h>

static void 
teststrspn(void)
{
  ASSERT( strspn("aabbcd", "ab") == 4);
  ASSERT( strspn("abbacd", "") == 0);
  ASSERT( strspn("abbacd", "ac") == 1);
  ASSERT( strspn("abbacd", "x") == 0);
  ASSERT( strspn("abbacd", "c") == 0);
  ASSERT( strspn("abbacd", "cba") == 5);
  ASSERT( strspn("abbacd", "cdba") == 6);
}
