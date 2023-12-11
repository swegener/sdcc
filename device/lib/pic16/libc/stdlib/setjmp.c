/*-------------------------------------------------------------------------
   setjmp.c - source file for ANSI routines setjmp & longjmp

   Copyright (C) 2023, Jonathon Hall <dabigjhall@gmail.com>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#include <sdcc-lib.h>
#include <pic18fregs.h>
#define __SDCC_HIDE_LONGJMP
#include <setjmp.h>

extern int _gptrput1;
extern int _gptrget1;

// We have to stash the stack pointer outside of the stack while manipulating
// the stack in setjmp and longjmp.  longjump must also stash its return value
// parameter.
#if defined(__SDCC_MODEL_LARGE)
#define STACK_PTR_SIZE 2
#else
#define STACK_PTR_SIZE 1
#endif
static unsigned char longjmp_stack[STACK_PTR_SIZE];
static int longjmp_rv;

// Jump buffer bytes:
// 0 - TOSU
// 1 - TOSH
// 2 - TOSL
// 3 - STKPTR
// 4 (sm) / 4-5 (lg) - FSR2 (H, L for large)
// 5 (sm) / 6-7 (lg) - FSR1 (H, L for large)

int __setjmp (jmp_buf buf) __naked
{
   (void)buf;
   __asm
      // Grab FSR1 into the scratch area before touching the stack.
      MOVFF _FSR1L, _longjmp_stack
#if defined(__SDCC_MODEL_LARGE)
      MOVFF _FSR1H, (_longjmp_stack+1) // Also get FSR1H
#endif

      // Load the buf pointer into WREG:PRODL:FSR0L for _gptrput1().
      // Since we are not creating a stack frame, buf is at FSR1+{1-3}
      // Load buf into W:PRODL:FSR0L
      MOVFF _PREINC1, _FSR0L
      MOVFF _PREINC1, _PRODL
      MOVF _PREINC1, W
      // Rewind stack to restore buf on the stack (caller cleans stack)
      MOVF _POSTDEC1, F
      MOVF _POSTDEC1, F
      MOVF _POSTDEC1, F

      // Push everything needed for jmp_buf onto the stack
#if defined(__SDCC_MODEL_LARGE)
      MOVFF (_longjmp_stack+1), _POSTDEC1 // FSR1H
#endif
      MOVFF _longjmp_stack, _POSTDEC1  // FSR1L
      MOVFF _FSR2L, _POSTDEC1
#if defined(__SDCC_MODEL_LARGE)
      MOVFF _FSR2H, _POSTDEC1
#endif
      MOVFF _STKPTR, _POSTDEC1
      MOVFF _TOSL, _POSTDEC1
      MOVFF _TOSH, _POSTDEC1
      MOVFF _TOSU, _POSTDEC1

      // Write out 6 (sm) or 8 (lg) bytes to buf from the stack

      // buf[0] = TOSU
      CALL __gptrput1
      // __gptrput1 increments FSR0, but it expects the high byte in PRODL,
      // refresh PRODL in case FSR0L carried
      MOVFF _FSR0H, _PRODL
      // buf[1] = TOSH
      CALL __gptrput1
      MOVFF _FSR0H, _PRODL
      // buf[2] = TOSL
      CALL __gptrput1
      MOVFF _FSR0H, _PRODL
      // buf[3] = STKPTR
      CALL __gptrput1
      MOVFF _FSR0H, _PRODL
#if defined(__SDCC_MODEL_LARGE)
      // buf[4 lg] = FSR2H
      CALL __gptrput1
      MOVFF _FSR0H, _PRODL
#endif
      // buf[4 sm / 5 lg] = FSR2L
      CALL __gptrput1
      MOVFF _FSR0H, _PRODL
#if defined(__SDCC_MODEL_LARGE)
      // buf[6 lg] = FSR1H
      CALL __gptrput1
      MOVFF _FSR0H, _PRODL
#endif
      // bur[5 sm / 7 lg] = FSR1L
      CALL __gptrput1

      // Return 0
      CLRF _PRODL
      RETLW 0
   __endasm;
}

int longjmp (jmp_buf buf, int rv) __naked
{
   (void)buf;
   (void)rv;
   __asm
      // Load buf into WREG:PRODL:FSR0L for _gptrget1()
      MOVFF _PREINC1, _FSR0L
      MOVFF _PREINC1, _PRODL
      // Use PRODH to hold high byte instead of WREG since each call to
      // _gptrget overwrites WREG.
      MOVFF _PREINC1, _PRODH

      // Stash return value
      MOVFF _PREINC1, _longjmp_rv   // LSB
      MOVFF _PREINC1, (_longjmp_rv+1)  // MSB

      // Read 6 (small) / 8 (large) bytes from buf onto the stack
      // buf[0] = TOSU
      MOVF _PRODH, W // Load WREG, high byte of address
      CALL __gptrget1
      // Like _gptrput1(), FSR0 is incremented but we must refresh PRODL in case
      // it carries into the high byte.
      MOVFF _FSR0H, _PRODL
      // Push the result onto the stack
      MOVWF _POSTDEC1

      // buf[1] = TOSH
      MOVF _PRODH, W
      CALL __gptrget1
      MOVFF _FSR0H, _PRODL
      MOVWF _POSTDEC1

      // buf[2] = TOSL
      MOVF _PRODH, W
      CALL __gptrget1
      MOVFF _FSR0H, _PRODL
      MOVWF _POSTDEC1

      // buf[3] = STKPTR
      MOVF _PRODH, W
      CALL __gptrget1
      MOVFF _FSR0H, _PRODL
      MOVWF _POSTDEC1

#if defined(__SDCC_MODEL_LARGE)
      // buf[4 lg] = FSR2H
      MOVF _PRODH, W
      CALL __gptrget1
      MOVFF _FSR0H, _PRODL
      MOVWF _POSTDEC1
#endif

      // buf[4 sm / 5 lg] = FSR2L
      MOVF _PRODH, W
      CALL __gptrget1
      MOVFF _FSR0H, _PRODL
      MOVWF _POSTDEC1

#if defined(__SDCC_MODEL_LARGE)
      // buf[6 lg] = FSR1H
      MOVF _PRODH, W
      CALL __gptrget1
      MOVFF _FSR0H, _PRODL
      MOVWF _POSTDEC1
#endif

      // bur[5 sm / 7 lg] = FSR1L
      MOVF _PRODH, W
      CALL __gptrget1
      MOVWF _POSTDEC1

      // Pop FSR1 into the scratch space
      MOVFF _PREINC1, _longjmp_stack   // FSR1L
#if defined(__SDCC_MODEL_LARGE)
      MOVFF _PREINC1, (_longjmp_stack+1)  // FSR1H
#endif

      // Pop everything else into the actual registers
#if defined(__SDCC_MODEL_LARGE)
      // FSR2H must be written first, load FSR2L into WREG for a moment
      MOVF _PREINC1, W
      MOVFF _PREINC1, _FSR2H
      MOVWF _FSR2L
#else
      MOVFF _PREINC1, _FSR2L
#endif
      MOVFF _PREINC1, _STKPTR
      MOVFF _PREINC1, _TOSL
      MOVFF _PREINC1, _TOSH
      MOVFF _PREINC1, _TOSU

      // Restore the stack
#if defined(__SDCC_MODEL_LARGE)
      MOVFF (_longjmp_stack+1), _FSR1H
#endif
      MOVFF _longjmp_stack, _FSR1L

      // Put the return value in PRODL:WREG
      MOVF _longjmp_rv, W
      MOVFF (_longjmp_rv+1), _PRODL

      // If either PRODL or WREG is nonzero, return as-is, otherwise return 1
      TSTFSZ _PRODL
      RETURN
      TSTFSZ _WREG
      RETURN
      RETLW 1  // PRODL:WREG are zero, return with WREG=1
   __endasm;
}
