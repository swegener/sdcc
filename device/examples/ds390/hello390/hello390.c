/* This example compiles right out of the box for TINI after you did a 
   make install of sdcc (latest revision :).
   
   Just do a "make" which compiles and compresses it a little.
   Than use JavaKit to load hello390.hex (it goes into bank 1, see Makefile)
   Type 'E' in JavaKit and enjoy :) */

/* if you are using the (default) interrupt scheme for serial i/o,
   you need to include this in the main program, so the vector will
   be initialized */
#include <serial390.h>

#include <stdio.h>

void main (void) {
  printf ("\n\rHello from 390.\n\rSee you, bye...");
}
