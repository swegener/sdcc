/** Function pointer tests.

    type: char, int, long
 */
#include <testfwk.h>

/* Must use a typedef as there is no way of adding the code modifier
   on the z80.
*/
typedef void (*NOARGFUNPTR)(void);
typedef void (*ONEARGFUNPTR)({type}) REENTRANT;
typedef long int (*FOURARGFUNPTR)(char, char, long int, long int) REENTRANT;

int count;
FOURARGFUNPTR fafp;

void
incCount(void)
{
  count++;
}

void
incBy({type} a) REENTRANT
{
  count += a;
}

long int f6(char a, char b, long int c, long int d) REENTRANT
{
  switch (a)
    {
    case 0: return a;
    case 1: return b;
    case 2: return c;
    case 3: return d;
    }
  return 0;
}


void
callViaPtr(NOARGFUNPTR fptr)
{
  (*fptr)();
}

void
callViaPtr2(ONEARGFUNPTR fptr, {type} arg)
{
  (*fptr)(arg);
}

void
callViaPtr3(void (*fptr)(void))
{
  (*fptr)();
}

void
callViaPtrAnsi(NOARGFUNPTR fptr)
{
  fptr();
}

void
callViaPtr2Ansi(ONEARGFUNPTR fptr, {type} arg)
{
  fptr(arg);
}

void
callViaPtr3Ansi(void (*fptr)(void))
{
  fptr();
}



void
testFunPtr(void)
{
  fafp = f6;

  ASSERT(count == 0);
  callViaPtr(incCount);
  ASSERT(count == 1);
  callViaPtr2(incBy, 7);
  ASSERT(count == 8);

  ASSERT((*fafp)(0, 0x55, 0x12345678, 0x9abcdef0) == 0);
  ASSERT((*fafp)(1, 0x55, 0x12345678, 0x9abcdef0) == 0x55);
  ASSERT((*fafp)(2, 0x55, 0x12345678, 0x9abcdef0) == 0x12345678);
  ASSERT((*fafp)(3, 0x55, 0x12345678, 0x9abcdef0) == 0x9abcdef0);
}

void
testFunPtrAnsi(void)
{
  fafp = f6;

  count = 0;
  callViaPtrAnsi(incCount);
  ASSERT(count == 1);
  callViaPtr2Ansi(incBy, 7);
  ASSERT(count == 8);

  ASSERT(fafp(0, 0x55, 0x12345678, 0x9abcdef0) == 0);
  ASSERT(fafp(1, 0x55, 0x12345678, 0x9abcdef0) == 0x55);
  ASSERT(fafp(2, 0x55, 0x12345678, 0x9abcdef0) == 0x12345678);
  ASSERT(fafp(3, 0x55, 0x12345678, 0x9abcdef0) == 0x9abcdef0);
}

