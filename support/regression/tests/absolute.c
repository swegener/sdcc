/** Absolute addressing tests.

    mem: code
*/
#include <testfwk.h>

{mem} at(0xCAB7) char u;
{mem} at(0xCAB7) char x = 'x';
{mem} at(0xCAB9) char y = 'y';
{mem} at(0xCAB0) int  k = 0x1234;

char z = 'z';

void
testAbsolute(void)
{
#if defined(SDCC_mcs51) || defined(SDCC_ds390) || defined(SDCC_hc08)
  char {mem} *pC = (char {mem} *)(0xCAB0);
  int  {mem} *pI = (char {mem} *)(0xCAB0);

  ASSERT(u == 'x');
  ASSERT(pC[7] == 'x');
  ASSERT(pC[9] == 'y');
  ASSERT(pI[0] == 0x1234);
#endif
}
