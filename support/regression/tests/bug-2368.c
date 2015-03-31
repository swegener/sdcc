/* bug-2368.c
   Wrong byte order in optimization of pointer assignment for stm8
 */
#include <testfwk.h>
#include <stdint.h>

#define TEST_VAL 0x1234
#define loc ((volatile uint16_t *)0x7fd)

volatile uint16_t *p = loc;

void testfunc1(void)
{
	*p = TEST_VAL;
}

void testfunc2(void)
{
	*loc = TEST_VAL;
}


void testBug(void)
{
#if defined(__SDCC_stm8) || defined(__SDCC_z80)
	testfunc1();
	ASSERT (*loc == TEST_VAL);
	testfunc2();
	ASSERT (*loc == TEST_VAL);
#endif
}

