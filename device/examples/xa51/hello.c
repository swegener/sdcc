#include <xa.h>

bit b1, b2=1;
data d1, d2=2;
xdata x1, x2=3;

#define	BAUD_RATE 9600
#define	OSC 20000000L	/* Xtal frequency */

#define	DIVIDER	(OSC/(64L*BAUD_RATE))

void external_startup(void) {
  _asm
    mov.b	_WDCON,#0	;shut down the watchdog
    mov.b	_WFEED1,#0a5h
    mov.b	_WFEED2,#05ah
;   mov.b	_BCR,#1	;BCR: 8 data, 16 address
;   mov.b	_SCR,#1	;SCR: page zero mode
  _endasm;

  // init serial io
  TL1 = RTL1 = -DIVIDER;
  TH1 = RTH1 = -DIVIDER >> 8;
  TR1 = 1;		/* enable timer 1 */
  
  S0CON = 0x52;		/* mode 1, receiver enable */
  IPA4 |= 0x6;		/* maximum priority */
  ERI0=1;		/* enable receiver interupts */
  TI0==1;               /* transmitter empty */
  RI0=0;                /* receiver empty */
  
  //PSWH &= 0xf0;	/* start interupt system */
}

void putchar(char c) {
  while(!TI0) 
    ;
  S0BUF = c;
  TI0 = 0;
}


char getchar(void) {
  char	c;
  
  while (!RI0) 
    ;
  c=S0BUF;
  RI0=0;
  return c;
}

void puts(char *s) {
  while (*s) {
    putchar (*s++);
  }
}

int kbhit(void) {
  return RI0;
}

void main(void) {
  puts ("Hello world.\n\r");
}
