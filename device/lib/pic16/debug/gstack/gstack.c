/*-------------------------------------------------------------------------

   gstack.c :- debug stack tracing support function

   Written for pic16 port by Vangelis Rokas, 2004 (vrokas@otenet.gr)

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/
/*
** $Id$
*/

extern WREG;
extern FSR1L;
extern FSR1H;
extern FSR0L;
extern FSR0H;
extern STATUS;
extern POSTINC0;
extern POSTDEC1;
extern PREINC1;
extern TOSL;
extern TOSH;
extern TOSU;
extern PCL;
extern PCLATH;
extern PCLATU;
extern stack;
extern stack_end;

#pragma udata access _wreg_store _status_store _fsr0_store
#pragma udata access _gstack_begin _gstack_end _init_ok

static char _init_ok=0;
static char _wreg_store;
static char _status_store;
static unsigned int _fsr0_store;
static unsigned int _gstack_begin;
static unsigned int _gstack_end;

char _gstack_fail_str[]="Stack overflow\n";
char _gstack_succ_str[]="Stack ok\n";


static
void _gstack_overflow_default(void) _naked
{
  _asm
    lfsr	0, __gstack_fail_str
;    incf	_FSR0L, f

@0:
    movf	_POSTINC0, w
    movff	_WREG, 0xf7f
    bnz		@0
    
;    sleep
@00:
    goto	@00
    
  _endasm ;
}

void (* _gstack_overflow)(void)=_gstack_overflow_default;

    
void _gstack_init(void) _naked
{
  _asm
    
    movlw	LOW(_stack)
    movwf	__gstack_begin
    
    movlw	HIGH(_stack)
    movwf	__gstack_begin+1

    movlw	LOW(_stack_end)
    movwf	__gstack_end
    
    movlw	HIGH(_stack_end)
    movwf	__gstack_end+1

    ; load default handler
;    movlw	LOW(__gstack_overflow_default)
;    movwf	__gstack_overflow
    
;    movlw	HIGH(__gstack_overflow_default)
;    movwf	__gstack_overflow+1
    
;    movlw	UPPER(__gstack_overflow_default)
;    movwf	__gstack_overflow+2
    

    movlw	1
    movwf	__init_ok
    
    return;    
  _endasm ;
}


void _gstack_test(void) _naked
{
  _asm
    movff	_WREG, __wreg_store
    movff	_STATUS, __status_store

    ; if first time, initialize boundary variables
    movf	__init_ok, w
    bnz		@1
    call	__gstack_init
    
@1:
    movf	__gstack_begin, w
    cpfslt	_FSR1L
    bra		@2
    bra		@3

@2:
    movf	__gstack_begin+1, w
    cpfslt	_FSR1H
    bra		@4
    bra		@3

@4:
    movf	__gstack_end, w
    cpfsgt	_FSR1L
    bra		@5
    bra		@3

@5:
    movf	__gstack_end+1, w
    cpfsgt	_FSR1H
    bra		@6

    ; fail

@3:

    push
    movlw	LOW(ret_lab)
    movwf	_TOSL

    movlw	HIGH(ret_lab)
    movwf	_TOSH

    movlw	UPPER(ret_lab)
    movwf	_TOSU

    movff	__gstack_overflow+2, _PCLATU
    movff	__gstack_overflow+1, _PCLATH
    movf	__gstack_overflow, w
    
    ; execute 
    movwf	_PCL
    
ret_lab:
    bra		@10

    ; success
@6:
    movff	_FSR0L, __fsr0_store
    movff	_FSR0H, __fsr0_store+1
    lfsr	0, __gstack_succ_str

    ; print corresponding string
@8:
    movf	_POSTINC0, w
    movff	_WREG, 0xf7f
    bnz		@8

@9:
    movff	__fsr0_store+1, _FSR0H
    movff	__fsr0_store, _FSR0L

@10:
    movff	__status_store, _STATUS
    movff	__wreg_store, _WREG
    
    return
    
    _endasm ;
}
