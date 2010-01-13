// #define MICROCONTROLLER_8051
#include <mcs51reg.h>

unsigned char
_sdcc_external_startup (void)
{
  /* serial port mode 0 is default */
  /* enable transmission of first byte */
  TI = 1;
  return 0;
}

void
_putchar (char c)
{
  while (!TI)
    ;
  TI = 0;
  SBUF = c;
}

void
_initEmu (void)
{
}

void
_exitEmu (void)
{
  while (!TI) /* wait for the last character to be transmitted */
    ;         /* before hitting the breakpoint */
  * (char __idata *) 0 = * (char __xdata *) 0x7654;
}
