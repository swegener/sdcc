/** atomic.c
*/
#include <testfwk.h>

#if defined(PORT_HOST) || defined(__SDCC_z80) || defined(__SDCC_gbz80) || defined(__SDCC_r2k) || defined(__SDCC_stm8)

#include <stdatomic.h>
#include <stdbool.h>

atomic_flag f1 = ATOMIC_FLAG_INIT;
atomic_flag f2;

#endif

void testAtomic(void)
{
#if defined(PORT_HOST) || defined(__SDCC_z80) || defined(__SDCC_gbz80) || defined(__SDCC_r2k) || defined(__SDCC_stm8)
	ASSERT(atomic_flag_test_and_set(&f1) == false);
	ASSERT(atomic_flag_test_and_set(&f1) == true);
	atomic_flag_clear(&f1);
	atomic_flag_clear(&f2);
	ASSERT(atomic_flag_test_and_set(&f1) == false);
	ASSERT(atomic_flag_test_and_set(&f2) == false);
#endif
}

