/*
 * crt0i.c - SDCC pic16 port runtime start code with
 *           initialisation
 *
 * Converted for SDCC and pic16 port
 * by Vangelis Rokas (vrokas@otenet.gr)
 *
 * based on Microchip MPLAB-C18 startup files
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

/* global variable for forcing gplink to add _cinit section */
char __uflags = 0;

/* external reference to the user's main routine */
extern void main (void);

/* prototype for the startup function */
void _entry (void) _naked interrupt 0;
void _startup (void) _naked;

/* prototype for the initialized data setup */
void _do_cinit (void);


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
    
	_do_cinit();

	/* Call the user's main routine */
	main ();

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


#define tblrdpostinc	tblrd*+

/* the variable initialisation routine */
void _do_cinit (void)
{
  /*
   * we'll make the assumption in the following code that these statics
   * will be allocated into the same bank.
   */
  static short long prom;
  static unsigned short curr_byte;
  static unsigned short curr_entry;
  static short long data_ptr;


	/* TBLPTR = &cinit */
	_asm
		movlw low(_cinit)
		movwf _TBLPTRL, 0
		movlw high(_cinit)
		movwf _TBLPTRH, 0
		movlw upper(_cinit)
		movwf _TBLPTRU, 0
	_endasm;

	
	/* curr_entry = cinit.num_init */
	_asm
		movlb __do_cinit_data_ptr_1_1
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf __do_cinit_curr_entry_1_1, 1
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf __do_cinit_curr_entry_1_1+1, 1
	_endasm;


	//while (curr_entry) {
	_asm
test:
	bnz done1
	tstfsz __do_cinit_curr_entry_1_1, 1
	bra cont1

done1:
	goto done

cont1:
	_endasm;


      /* Count down so we only have to look up the data in _cinit
       * once.
       *
       * At this point we know that TBLPTR points to the top of the current
       * entry in _cinit, so we can just start reading the from, to, and
       * size values.
       */
	_asm
		
		/* read the source address low */
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf __do_cinit_prom_1_1, 1
		
		/* source address high */
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf __do_cinit_prom_1_1 + 1, 1

		/* source address upper */
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf __do_cinit_prom_1_1 + 2, 1

		/* skip a byte since it's stored as a 32bit int */
		tblrdpostinc

		/* read the destination address directly into FSR0 */
		/* destination address low */
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf _FSR0L, 0

		/* destination address high */
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf _FSR0H, 0

		/* skip two bytes since it's stored as a 32bit int */
		tblrdpostinc
		tblrdpostinc

		/* read the destination address directly into FSR0 */
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf __do_cinit_curr_byte_1_1, 1
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf __do_cinit_curr_byte_1_1+1, 1

		/* skip two bytes since it's stored as a 32bit int */
		tblrdpostinc
		tblrdpostinc
	_endasm;

	//prom = data_ptr->from;
	//FSR0 = data_ptr->to;
	//curr_byte = (unsigned short) data_ptr->size;
	/* the table pointer now points to the next entry. Save it
	 * off since we'll be using the table pointer to do the copying
	 * for the entry */
  
	/*  data_ptr = TBLPTR */
	_asm
		movff _TBLPTRL, __do_cinit_data_ptr_1_1
		movff _TBLPTRH, __do_cinit_data_ptr_1_1 + 1
		movff _TBLPTRU, __do_cinit_data_ptr_1_1 + 2
	_endasm;
  
      
	/* now assign the source address to the table pointer */
	/*  TBLPTR = prom */
	_asm
		movff __do_cinit_prom_1_1, _TBLPTRL
		movff __do_cinit_prom_1_1 + 1, _TBLPTRH
		movff __do_cinit_prom_1_1 + 2, _TBLPTRU
	_endasm;

	/* do the copy loop */
	_asm

		/* determine if we have any more bytes to copy */
		movlb __do_cinit_curr_byte_1_1
		movf __do_cinit_curr_byte_1_1, 1, 1
copy_loop:
		bnz copy_one_byte // copy_one_byte
		movf __do_cinit_curr_byte_1_1 + 1, 1, 1
		bz done_copying

copy_one_byte:
		tblrdpostinc
		movf _TABLAT, 0, 0
		movwf _POSTINC0, 0

		/* decrement byte counter */
		decf __do_cinit_curr_byte_1_1, 1, 1
		bnc copy_loop // copy_loop
		decf __do_cinit_curr_byte_1_1 + 1, 1, 1

		bra copy_loop
done_copying:
	_endasm;
  
	/* restore the table pointer for the next entry */
	/*  TBLPTR = data_ptr */
	_asm
		movff __do_cinit_data_ptr_1_1, _TBLPTRL
		movff __do_cinit_data_ptr_1_1 + 1, _TBLPTRH
		movff __do_cinit_data_ptr_1_1 + 2, _TBLPTRU
	_endasm;

  
	/* next entry... */
	curr_entry--;

	_asm
		goto test;

	/* emit done label */
done:
	_endasm;
}

