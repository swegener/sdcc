/** memory function test
*/
#include <testfwk.h>

#include <string.h>

unsigned char destination[4];
const unsigned char source[4] = {0, 1, 2, 3};

void testmemory(void)
{
  /* Test memset() */
  destination[3] = 23;
  memset(destination, 42, 3);
  ASSERT(destination[0] == 42);
  ASSERT(destination[2] == 42);
  ASSERT(destination[3] == 23);

  /* Test memcpy() */
  memcpy(destination + 1, source + 1, 2);
  ASSERT(destination[0] == 42);
  ASSERT(destination[2] == source[2]);
  ASSERT(destination[3] == 23);

  /* Test memmove() */
  memcpy(destination, source, 4);
  memmove(destination, destination + 1, 3);
  ASSERT(destination[0] == source[1]);
  ASSERT(destination[2] == source[3]);
  ASSERT(destination[3] == source[3]);
  memcpy(destination, source, 4);
  memmove(destination + 1, destination, 3);
  ASSERT(destination[0] == source[0]);
  ASSERT(destination[1] == source[0]);
  ASSERT(destination[3] == source[2]);

  /* Test memchr() */
  /* memchr() is not yet supported by sdcc.
  ASSERT(NULL == memchr(destination, 5, 4));
  ASSERT(destination == memchr(destination, 0, 4));
  ASSERT(destination + 3 == memchr(destination, 3, 4));*/
}

