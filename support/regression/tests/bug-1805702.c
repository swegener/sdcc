/* bug-1805702.c
 */

#include <testfwk.h>

int foo;
extern int foo;

void
test(void)
{
  foo = 10;

  ASSERT(foo == 10);
}

/* compile time check for compiler defined functions (cdef) */

float __fsmul (float, float);

/* multiply two floats */
float __fsmul (float a1, float a2) {
  return (a1 + a2);
}

extern unsigned char _divuchar (unsigned char a, unsigned char b);

signed char
_divschar (signed char a, signed char b)
{
  return _divuchar ((unsigned int)a, (unsigned int)b);
}
