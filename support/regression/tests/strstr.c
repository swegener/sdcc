/** tests for strstr
*/
#include <testfwk.h>
#include <string.h>

static void 
teststrstr(void)
{
  char *a = "aabbcd";
  ASSERT( strstr(a, "\0\1") == a);
  ASSERT( strstr(a, "") == a);
  ASSERT( strstr(a, "ab") == &a[1]);
  ASSERT( strstr(a, "abc") == NULL);
  ASSERT( strstr(a, "abbc") == &a[1]);
  ASSERT( strstr("", "abbc") == NULL);
/* ASSERT( strstr("", "") == a); should work, but it doesn't */
  ASSERT( strstr(a, "cd") == &a[4]);
}
