/*
   bug3037889.c
 */

#include <testfwk.h>

#ifdef SDCC
#pragma disable_warning 88 //no warning about casting LITERAL value to 'generic' pointer
#endif

/* causes error 9: FATAL Compiler Internal Error in file 'SDCCicode.c' line number '2939' : code generator internal error
   Contact Author with source code
   Internal error: validateLink failed in DCL_TYPE(ptr) @ SDCCicode.c:2887: expected DECLARATOR, got SPECIFIER
   And on ds390 this further on caused also:
   Internal error: validateOpType failed in OP_SYMBOL(result) @ gen.c:11914: expected symbol, got value */
void testBug3037889(void)
{
    *((char *) 0x42e0 - 1) = 1;     /* compiler crashes */
    *((char *) 0x42e0 + 1) = 1;     /* works fine */
    *((char *) (0x42e0 - 1)) = 1;   /* works fine */
}

/* Infinite loop should not be executed, caused:
   Internal error: validateOpType failed in OP_SYMBOL(IC_RESULT (ic)) @ SDCCloop.c:1024: expected symbol, got value */
void Bug3034976(void)
{
	while(1)
	{
		*(unsigned char __data *)0 = 0;
	}
}
