
#include <pic18fregs.h>
#include <adc.h>


char adc_busy(void) __naked
{
#if 0
  return (ADCON0bits.GO);
#else
  __asm
    movlw       0x00
#if defined(__SDCC_ADC_STYLE2455)
    btfsc       _ADCON0bits, 0
#else /* all other devices */
    btfsc       _ADCON0bits, 2
#endif
    addlw       0x01
    return
  __endasm;
#endif
}

