/* Bad addition for adding a length and char[]
 */
#include <testfwk.h>

typedef struct _Foo
{
  char sz[100];
} Foo;

typedef struct _Bar
{
  unsigned int uLen;
} Bar;

char *getOffset(Foo *pFoo, Bar *pBar)
{
  return pFoo->sz + pBar->uLen;
}

void
testOffset(void)
{
  Foo foo = {
    "Foo"
  };
  Bar bar = {
    3
  };

  ASSERT(getOffset(&foo, &bar) 
	 == (((char *)&foo) + 3));
}
