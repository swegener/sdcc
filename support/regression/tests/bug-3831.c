/** bug-3831.c: a code generation bug for mos6502 target.
  The code generated for the function outer_func causes the pointer address
  passed to inner_func to contain the high byte in both the upper and lower byte.

  Correct behaviour (SDCC 4.4.0) : Byte at 0x0100 is overwritten with 0x42
  Current behaviour (SDCC 4.5.0) : Byte at 0x0101 is overwritten with 0x42
 */

#include <testfwk.h>
#include <stdint.h>

typedef struct foo_t
{   
    uint8_t b0;
    uint8_t b1;
} foo_t;

/* Place at known address 0x0100 to easier see how bug changes write location from 0x0100 to 0x0101 */
/* On 6502, 0x0100-0x0101 is virtually guaranteed to be RAM due to being used for the stack */
#ifdef __SDCC_mos6502
foo_t __at(0x0100) foo_0100 = { 0x00, 0x00 };
#else
foo_t foo_0100 = { 0x00, 0x00 };
#endif

void inner_func(foo_t* foo) {
    foo->b0 = 0x42;
}

void outer_func(foo_t* foo) __reentrant {
    if (foo == 0x1234) {
        foo->b0 |= 4;
    } else {
        foo->b0 |= 8;
        inner_func(foo);
    }
}

void testBug(void)
{
    outer_func(&foo_0100);
    ASSERT(foo_0100.b0 == 0x42);
    ASSERT(foo_0100.b1 == 0x00);
}

