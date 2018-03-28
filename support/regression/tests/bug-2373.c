/** Bug 2373
 */
#include <testfwk.h>

char func(char a)
{
  return a-1;
}

static char a;

static struct
{
  char c;
} s;

static char (* __xdata func_ptr) (char);

void
testFptr(void)
{
  char b = 10;
  a      = 10;
  s.c    = 10;
#if !((defined __SDCC_stm8) && defined (__SDCC_MODEL_LARGE)) // Assembler not suitable for 24-bit function pointers
  func_ptr=func;
  
  // works as expected
  ASSERT(func_ptr(a) == 9);
  ASSERT(func_ptr(b) == 9);
  
  // error passing s.c to func
  ASSERT(func_ptr(s.c) == 9);
#endif
}
