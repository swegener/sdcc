/** tests for strncmp
*/
#include <testfwk.h>
#include <string.h>

static void 
teststrncmp(void)
{
  ASSERT( strncmp("", "", 0) == 0);
  ASSERT( strncmp("ab", "ab", 0) == 0);
  ASSERT( strncmp("a", "a", 2) == 0);
  ASSERT( strncmp("aa", "ab", 1) == 0);
  ASSERT( strncmp("aa", "ab", 2) < 0);
  ASSERT( strncmp("abc", "abd", 2) == 0);
  ASSERT( strncmp("abc", "abc", 3) == 0);
}
