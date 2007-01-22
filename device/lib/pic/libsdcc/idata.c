/*
 * idata.c - startup code evaluating gputils' cinit structure
 *
 * This code fragment copies initialized data from ROM to their
 * assigned RAM locations. The requierd cinit structure is created
 * by gputils' linker and comprises initial values of all linked in
 * modules.
 *
 * (c) 2007 by Raphael Neider <rneider @ web.de>
 * 
 * This file is part of SDCC's pic14 library and distributed under
 * the terms of the GPLv2 with linking exception; see COPYING in some
 * parent directory for details.
 */

/*
 * We call the user's main() after initialization is done.
 */
extern void main(void);

/*
 * Force generation of _cinit symbol.
 */
static char force_cinit = 0;

/*
 * This struct describes one initialized variable.
 */
typedef struct {
    unsigned src;   // source address of data in CODE space
    unsigned dst;   // destination address of values in DATA space
    unsigned size;  // number of bytes to copy from `src' to `dst'
} cinit_t;

/*
 * This structure provides the number and position of the above
 * structs. to initialize all variables in the .hex file.
 */
extern __code struct {
    unsigned	records;    // number of entries in this file
    cinit_t	entry[];    // intialization descriptor
} cinit;

/*
 * Iterate over all records and copy values from ROM to RAM.
 */
void
_sdcc_gsinit_startup(void)
{
    unsigned num, size;
    __code cinit_t *cptr;
    __code char *src;
    __data char *dst;
    
    num = cinit.records;
    cptr = &cinit.entry[0];
    
    // iterate over all cinit entries
    while (num--) {
	size = cptr->size;
	src = (__code char *) cptr->src;
	dst = (__data char *) cptr->dst;
	
	// copy data byte-wise from ROM to RAM
	while (size--) {
	    *dst = *src;
	    src++;
	    dst++;
	} // while
	
	// XXX: might need to clear the watchdog timer here...
	cptr++;
    } // while

    // call main after initialization
    __asm
	GOTO _main
    __endasm;
}

