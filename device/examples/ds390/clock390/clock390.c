/* This example compiles right out of the box for TINI after you did a 
   make install of sdcc (latest revision :).
   
   Just do a "make" which compiles and compresses it a little.
   Than use JavaKit to load clock390.hex (it goes into bank 1, see Makefile)
   Type 'E' in JavaKit and enjoy :) */

/* if you are using the (default) interrupt scheme for serial i/o,
   you need to include this in the main program, so the vector will
   be initialized */
#include <serial390.h>

#include <ds80c390.h>
#include <stdio.h>
#include "lcd.h"

volatile unsigned long milliSeconds=0;

#define RELOAD_VALUE 18432000/2/1000 // appr. 1ms

void Timer2Handler (void) interrupt 5 using 1 {
  TF2=0; // reset overflow flag
  milliSeconds++;
  // that's all for now :)
}

// we can't just use milliSeconds
unsigned long GetMilliSeconds(void) {
  unsigned long ms;
  ET2=0;
  ms=milliSeconds;
  ET2=1;
  return ms;
}

void main (void) {
  unsigned long ms, seconds, oldSeconds=-1;
  
  printf ("\n\rStarting timer 2 test.\n\r");

  // initialise timer
  ET2=0; // disable timer interrupts initially
  T2CON=0; // stop, timer mode, autoreload
  T2MOD&=0xf4;
  
  TL2=RTL2=(-RELOAD_VALUE)&0xff;
  TH2=RTH2=(-RELOAD_VALUE)>>8;
  TR2=1; // run

  ET2=1; // enable timer interrupts

  // now do something
  LcdInit();
  LcdLPutString(0, "Testing timer2");
  LcdLPutString(2, "ms: ");
  while (1) {
    ms=GetMilliSeconds();
    seconds=ms/1000;
    LcdLPrintf (2 + (4<<8), "%ld", ms);
    if (seconds!=oldSeconds) {
      printf ("%02d:%02d.%02d\n\r", (int)seconds/3600, 
	      (int)(seconds/60)%60, 
	      (int)seconds%60);
      oldSeconds=seconds;
      _asm
	cpl P3.5 ; toggle led
      _endasm;
    }
  }
}


