
#include <pic18fregs.h>

#include <adc.h>


char adc_busy(void) _naked
{
#if 1
  return (ADCON0bits.GO);
#else
  _asm
    movlw       0x00
    btfsc       _ADCON0bits, 2
    addlw       0x01
    return
  _endasm;
#endif
}
