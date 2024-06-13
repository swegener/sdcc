/** tests for strnlen
*/

#include <testfwk.h>

#include <string.h>

const char* hello1 = "hello1";
const char hello2[] = "hello2";
char hello3[] = "hello3";

void
testStr(void)
{
  const char hello4[] = "hello4";
  const char hello5[7];

  memcpy (hello5, hello4, 7);

  ASSERT(strnlen("hello0", 23) == strlen("hello0"));
  ASSERT(strnlen(hello1, 23) == strlen(hello1));
  ASSERT(strnlen(hello2, 23) == strlen(hello2));
  ASSERT(strnlen(hello3, 23) == strlen(hello3));
  ASSERT(strnlen(hello4, 23) == strlen(hello4));
  ASSERT(strnlen(hello5, 23) == strlen(hello5));

  ASSERT(strnlen("hello0", 3) == 3);
  ASSERT(strnlen(hello1, 3) == 3);
  ASSERT(strnlen(hello2, 3) == 3);
  ASSERT(strnlen(hello3, 3) == 3);
  ASSERT(strnlen(hello4, 3) == 3);
  ASSERT(strnlen(hello5, 3) == 3);

  ASSERT(strnlen("hello0", 6) == 6);
  ASSERT(strnlen(hello1, 6) == 6);
  ASSERT(strnlen(hello2, 6) == 6);
  ASSERT(strnlen(hello3, 6) == 6);
  ASSERT(strnlen(hello4, 6) == 6);
  ASSERT(strnlen(hello5, 6) == 6);

#if !(defined (__SDCC_pdk15) && defined(__SDCC_STACK_AUTO)) // Lack of code memory
  ASSERT(strnlen("hello0", 7) == 6);
  ASSERT(strnlen(hello1, 7) == 6);
  ASSERT(strnlen(hello2, 7) == 6);
  ASSERT(strnlen(hello3, 7) == 6);
  ASSERT(strnlen(hello4, 7) == 6);
  ASSERT(strnlen(hello5, 7) == 6);
#endif
}

