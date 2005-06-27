/*
   sfr16.c
*/

#include <testfwk.h>

#if defined (SDCC_mcs51)
sfr at 0x8A TL0; //timer 0 low byte
sfr at 0x8C TH0; //timer 0 high byte

sfr16 at 0x8C8A TMR0; //timer 0
#endif

void
test_sfr(void)
{
#if defined (SDCC_mcs51)
  TMR0 = 0x00FE;
  ASSERT (TH0 == 0x00);
  ASSERT (TL0 == 0xFE);

  TMR0++;
  ASSERT (TH0 == 0x00);
  ASSERT (TL0 == 0xFF);

  TMR0++;
  ASSERT (TH0 == 0x01);
  ASSERT (TL0 == 0x00);

  TL0++;
  ASSERT (TMR0 == 0x0101);
#endif
}
