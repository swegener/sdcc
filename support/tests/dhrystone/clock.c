#include <ds80c390.h>
#include <stdio.h>

#include "clock.h"

volatile unsigned long milliSeconds=0;

#define RELOAD_VALUE 18432000/2/CLOCKS_PER_SEC 

void Timer2Handler (void) interrupt 5 using 1 {
  TF2=0; // reset overflow flag
  milliSeconds++;
  // that's all for now :)
}

// we can't just use milliSeconds
unsigned long clock(void) {
  unsigned long ms;
  ET2=0;
  ms=milliSeconds;
  ET2=1;
  return ms;
}

void startTimer (void) 
{
  // initialise timer
  ET2=0; // disable timer interrupts initially
  T2CON=0; // stop, timer mode, autoreload
  T2MOD&=0xf4;
  
  TL2=RTL2=(-RELOAD_VALUE)&0xff;
  TH2=RTH2=(-RELOAD_VALUE)>>8;
  TR2=1; // run

  ET2=1; // enable timer interrupts
}


