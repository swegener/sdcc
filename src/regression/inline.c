#define __16F873
#include "p16f873.h"
unsigned char success=0;
unsigned char failures=0;
unsigned char dummy=0;

unsigned char test_tris=0;

#define PORTA_ADR  5
#define PORTB_ADR  6
#define TRISA_ADR  0x85
#define TRISB_ADR  0x86

data at TRISA_ADR unsigned char  TRISA;

void done(void)
{

  dummy++;

}

void
delay_1ms(void)
{
  unsigned char cnt1m = 2;
  unsigned char cnt500u = 249;

  do {
    do {
      _asm
        nop
        nop
      _endasm;
    } while (--cnt500u > 0);
  } while (--cnt1m > 0);
}


void main(void)
{
  dummy = 0;

  TRISA = 0x0f;

  _asm
    BSF   STATUS,RP0
    MOVF  TRISA,W
    BCF   STATUS,RP0
    MOVWF _test_tris
  _endasm;

  if(test_tris != 0x0f)
    failures++;

  success = failures;
  done();
}
