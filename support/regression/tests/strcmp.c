/** tests for strcmp
*/
#include <testfwk.h>
#include <string.h>

static void 
teststrcmp(void)
{
  int result = strcmp("", "");
  ASSERT( result == 0);
  
  result = strcmp("", "a");
  ASSERT( result < 0);

  result = strcmp("a", "");
  ASSERT( result > 0);

  result = strcmp("ab", "ab");
  ASSERT( result == 0);

  result = strcmp("aa", "ab");
  ASSERT( result < 0);
}
