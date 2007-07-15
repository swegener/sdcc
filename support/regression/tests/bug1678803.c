/*
    bug 1678803
    This should not generate error 12 "called object is not a function".
*/

#include <testfwk.h>

typedef void (*func)(void);

void foo(void)
{
}

#ifdef SDCC_mcs51
func GetFunc(void) __naked
{
  __asm

    ; some assembler code
    mov dptr,#_foo
    ret

  __endasm;
}
#endif

void testCaller(void)
{
#ifdef SDCC_mcs51
  GetFunc()();
#endif

  ASSERT (1);
}
