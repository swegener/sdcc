#include <ds80c390.h>
#include <serial390.h>

/* This routine is intended to setup the DS80C390 in contiguous addressing
   mode, using a 10-bit stack (mapped to 0x400000).
   It assumes that _sdcc_gsinit_startup is called from a boot loader
   in 22-bit contiguous addressing mode */

/* Uncomment this if you are using the tini loader 000515. Make sure 
   XSEG starts at 0x100080 or it will overwrite your IVT. This won't harm 
   if you are using the 990824 loader */
#define TINI_LOADER_0515

unsigned char _sdcc_external_startup(void)
{
  IE=0; // disable ALL interrupts

  _asm
    ; save the 24-bit return address
    pop ar2; msb
    pop ar1
    pop ar0; lsb

    ; use A19..16 and !CE3..0, no CAN
    mov _TA,#0xaa; timed access
    mov _TA,#0x55
    mov _P4CNT,#0x3f

    ; use !PCE3..0, serial 1 at P5.2/3
    mov _TA,#0xaa; timed access
    mov _TA,#0x55
    mov _P5CNT,#0x27

    ; disable watchdog
    mov _EWT,#0x00

    ; watchdog set to 9.1 seconds
    ; timers at 1/4 xtal
    ; no strech-cycles for movx
    mov _CKCON,#0xf9;

    ; use internal 4k RAM as data(stack) memory at 0x400000 and
    ; move CANx memory access to 0x401000 and upwards
    ; use !CE* for program and/or data memory access
    mov _TA,#0xaa; timed access
    mov _TA,#0x55
    mov _MCON,#0xaf;

    mov _PMR,#0x82; two clocks per cycle
    mov _PMR,#0x92; enable multiplier
_Startup390WaitForClock:
    mov a,_EXIF
    jnb acc.3,_Startup390WaitForClock; wait for multiplier to be ready
    mov _PMR,#0x12; one clock per cycle, xtal*2

    mov _TA,#0xaa; timed access
    mov _TA,#0x55
    mov _ACON,#0x06; 24-bit addresses, 10-bit stack at 0x400000

    mov _ESP,#0x00; reinitialize the stack
    mov _SP,#0x00

    ; restore the 24-bit return address
    push ar0; lsb
    push ar1
    push ar2; msb
  _endasm;

#ifdef TINI_LOADER_0515
  // Copy the Interrupt Vector Table (128 bytes) from 0x10000 to 0x100000
  // Make sure that XSEG starts at 0x100080 in this case!
  _asm
  push dpx
  push dph
  push dpl
  push dps
  push b
  push acc
  mov dps,#0x00 ; make sure no autoincrement in progress
  mov dptr,#0x10000 ; from
  inc dps ; switch to alternate dptr
  mov dptr,#0x100000 ; to
  mov b,#0x80 ; count

_Startup390CopyIVT:
  inc dps
  movx a,@dptr
  inc dptr
  inc dps
  movx @dptr,a
  inc dptr
  djnz b,_Startup390CopyIVT

  pop acc
  pop b
  pop dps
  pop dpl
  pop dph
  pop dpx
  _endasm;
#endif

  // global interrupt enable, all masks cleared
  // let the Gods be with us :)
  IE = 0x80; 

  // now that the stack is re-initialized, we can safely call
  Serial390Init();

  // signal _sdcc_gsinit_startup to initialize data (call _sdcc_init_data)
  return 0; 
}

