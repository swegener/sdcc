/*
 * crt0iz.c - SDCC pic16 port runtime start code with
 *            initialisation and RAM memory zero
 *
 * Converted for SDCC and pic16 port
 * by Vangelis Rokas (vrokas@otenet.gr)
 *
 * based on Microchip MPLAB-C18 startup files
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *  
 * In other words, you are welcome to use, share and improve this program.
 * You are forbidden to forbid anyone else to use, share and improve
 * what you give them.   Help stamp out software-hoarding!  
 *
 * $Id$
 */

extern stack;
extern stack_end;

extern TBLPTRU;
extern TBLPTRH;
extern TBLPTRL;
extern FSR0L;
extern FSR0H;
extern TABLAT;
extern POSTINC0;
extern POSTDEC0;

#if 1
/* global variable for forcing gplink to add _cinit section */
char __uflags = 0;
#endif

/* external reference to the user's main routine */
extern void main (void);

/* prototype for the startup function */
void _entry (void) _naked interrupt 0;
void _startup (void) _naked;

/* prototype for the initialized data setup */
void _do_cinit (void) _naked;


/*
 * entry function, placed at interrupt vector 0 (RESET)
 */

void _entry (void) _naked interrupt 0
{
  _asm goto __startup _endasm;
}


void _startup (void) _naked
{
  _asm
    // Initialize the stack pointer
    lfsr 1, _stack_end
    lfsr 2, _stack_end
    clrf _TBLPTRU, 0	// 1st silicon doesn't do this on POR
    
    // initialize the flash memory access configuration. this is harmless
    // for non-flash devices, so we do it on all parts.
    bsf 0xa6, 7, 0
    bcf 0xa6, 6, 0
  _endasm ;
    
  /* cleanup the RAM */
  _asm
    /* load FSR0 with top of RAM memory */
        ; movlw 0xff
        ; movwf _FSR0L, 0
    setf _FSR0L
    movlw 0x0e
    movwf _FSR0H, 0
		
    /* place a 1 at address 0x00, as a marker 
     * we haven't reached yet to it */
        ; movlw 1
        ; movwf 0x00, 0
    setf 0x00
		
    /* load WREG with zero */
    movlw 0x00
		
clear_loop:
    movwf _POSTDEC0
    movf 0x00, w
    bnz clear_loop
  _endasm ;

  _do_cinit();

  /* Call the user's main routine */
  main();

loop:
  /* return from main will lock up */
  goto loop;
}


/* the cinit table will be filled by the linker */
extern code struct
{
  unsigned short num_init;
  struct _init_entry {
    unsigned long from;
    unsigned long to;
    unsigned long size;
  } entries[];
} cinit;


#define TBLRDPOSTINC	tblrd*+

#define prom		0x00		/* 0x00 0x01 0x02*/
#define curr_byte	0x03		/* 0x03 0x04 */
#define curr_entry	0x05		/* 0x05 0x06 */
#define data_ptr	0x07		/* 0x07 0x08 0x09 */

/*
 * static short long _do_cinit_prom;
 * static unsigned short _do_cinit_curr_byte;
 * static unsigned short _do_cinit_curr_entry;
 * static short long _do_cinit_data_ptr;
 */

/* the variable initialisation routine */
void _do_cinit (void) _naked
{
  /*
   * access registers 0x00 - 0x09 are not saved in this function
   */

  _asm
      ; TBLPTR = &cinit
    movlw low(_cinit)
    movwf _TBLPTRL
    movlw high(_cinit)
    movwf _TBLPTRH
    movlw upper(_cinit)
    movwf _TBLPTRU
	
                  ; curr_entry = cinit.num_init
                  ; movlb data_ptr
    TBLRDPOSTINC
    movf _TABLAT, w
    movwf curr_entry

    TBLRDPOSTINC
    movf _TABLAT, w
    movwf curr_entry+1

                  ; while (curr_entry) {
test:
    bnz done1
    tstfsz curr_entry, 1
    bra cont1

done1:
    goto done

cont1:

    ; Count down so we only have to look up the data in _cinit once. 

    ; At this point we know that TBLPTR points to the top of the current 
    ; entry in _cinit, so we can just start reading the from, to, and 
    ; size values. 

    ; read the source address low 
    TBLRDPOSTINC
    movf _TABLAT, w
    movwf prom
		
    ; source address high 
    TBLRDPOSTINC
    movf _TABLAT, w
    movwf prom + 1

    ; source address upper 
    TBLRDPOSTINC
    movf _TABLAT, w
    movwf prom + 2

    ; skip a byte since it is stored as a 32bit int 
    TBLRDPOSTINC

    ; read the destination address directly into FSR0 
    ; destination address low 
    TBLRDPOSTINC
    movf _TABLAT, w
    movwf _FSR0L

    ; destination address high 
    TBLRDPOSTINC
    movf _TABLAT, w
    movwf _FSR0H

    ; skip two bytes since it is stored as a 32bit int 
    TBLRDPOSTINC
    TBLRDPOSTINC

    ; read the destination address directly into FSR0 
    TBLRDPOSTINC
    movf _TABLAT, w
    movwf curr_byte
    
    TBLRDPOSTINC
    movf _TABLAT, w
    movwf curr_byte+1
    

    ; skip two bytes since it is stored as a 32bit int 
    TBLRDPOSTINC
    TBLRDPOSTINC

    ;  prom = data_ptr->from; 
    ;  FSR0 = data_ptr->to; 
    ;  curr_byte = (unsigned short) data_ptr->size; 

    ; the table pointer now points to the next entry. Save it 
    ; off since we will be using the table pointer to do the copying 
    ; for the entry 
  
    ; data_ptr = TBLPTR 

    movff _TBLPTRL, data_ptr
    movff _TBLPTRH, data_ptr + 1
    movff _TBLPTRU, data_ptr + 2
      
    ; now assign the source address to the table pointer 
    ; TBLPTR = prom 
    
    movff prom, _TBLPTRL
    movff prom + 1, _TBLPTRH
    movff prom + 2, _TBLPTRU

    ; do the copy loop 

    ; determine if we have any more bytes to copy 
                  ; movlb curr_byte 
    movf curr_byte, w

copy_loop:
    bnz copy_one_byte 		; copy_one_byte 
    movf curr_byte + 1, w
    bz done_copying

copy_one_byte:
    TBLRDPOSTINC
    movf _TABLAT, w
    movwf _POSTINC0

    ; decrement byte counter 
    decf curr_byte, f
    bnc copy_loop 		; copy_loop 
    decf curr_byte + 1, f

    bra copy_loop
done_copying:

  
    ; restore the table pointer for the next entry 
    ; TBLPTR = data_ptr 
    movff data_ptr, _TBLPTRL
    movff data_ptr + 1, _TBLPTRH
    movff data_ptr + 2, _TBLPTRU


    dcfsnz curr_entry, f
    decf curr_entry + 1, f

    ; next entry...
    ; _do_cinit_curr_entry--; 

    goto test;

    ; emit done label 
done:
    return
  _endasm;
}

