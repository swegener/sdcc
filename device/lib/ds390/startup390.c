#include <ds80c390.h>
#include <serial390.h>

/* This routine is intended to setup the DS80C390 in contiguous addressing
   mode, using a 10-bit stack (mapped to 0x400000).
   It can be called from _sdcc_gs_init_startup in ANY mode.
   Since it reinitializes the stack, it does a "ljmp" back to
   __sdcc_init_data instead of returning. */


unsigned char _sdcc_external_startup(void)
{

  // Let us assume we got here because the reset vector pointed to
  // _sdcc_gsinit_startup. So, we are in 16-bit mode, 8051 stack.
  // (not tested yet)
  // 
  // But it could as well have been a bootloader calling _sdcc_gsinit_startup
  // at 0x10000 in 22-bit Contiguous Addressing Mode, 8051 or whatever stack.
  // Hey :) that is a TINI!
  // (tested "TINI loader 08-24-99 09:34" and "TINI loader 05-15-00 17:45")

  // disable ALL interrupts
  IE=0;

  // use A19..16 and !CE3..0, no CAN
  TA = 0xaa; // timed access
  TA = 0x55;
  P4CNT = 0x3f;
  
  // use !PCE3..0, serial 1 at P5.2/3
  TA = 0xaa; // timed access
  TA = 0x55;
  P5CNT = 0x27;
  
  // disable watchdog
  EWT=0;

  // watchdog set to 9.1 seconds
  // timers at 1/4 xtal
  // no stretch-cycles for movx
  CKCON = 0xf9;
  
  // use internal 4k RAM as stack memory at 0x400000 and
  // move CANx Memory access to 0x401000 and upwards
  TA = 0xaa; // timed access
  TA = 0x55;
  //MCON = 0xef; // program and/or data memory access
  MCON = 0xaf; // data memory access only

  PMR = 0x82; // two clocks per cycle
  PMR = 0x92; // enable multiplier
  while (!(EXIF&0x08))
    // wait for multiplier to be ready
    ;
  PMR = 0x12; // one clock per cycle, xtal*2 

  // Never mind port 2. If external hardware sets !MUX, it should take care
  // of demultiplexing port 0 using !ALE as well, but we do not care...

  // switch to: 
  //   22-bit Contiguous Addressing Mode
  //   10-bit Stack Mode
  TA = 0xaa; // timed access
  TA = 0x55;
  ACON = 0x06;

  // Set the stack to 0, although it is now mapped to 0x400000
  // So be aware when accessing the stack !
  ESP=SP=0; // note: we can not return from here anymore :)

  // global interrupt enable, all masks cleared
  // let the Gods be with you :)
  IE = 0x80; 

  // now that the stack is initialized, we can safely call
  Serial390Init();

  // I HATE this, but:
  // We can do this safely because either AP is zero-ed when called from
  // the reset vector, or should have been set by the bootloader to the bank
  // in which we were called. We did not change it, so it will work as 
  // long as we stay within the same (64k) bank.

_asm
  ljmp __sdcc_init_data
_endasm;

  // the compiler _SHOULD_ warn us if we did not do a:
// return 0;
}

