/* This example compiles right out of the box for TINI after you did a 
   make install of sdcc (latest revision :).

   It prints the value of the DS1621 temperature sensor (at address 2) 
   and the 4 values of the PCF8591 ad-convertor (at address 0).
   It (ab)uses the CAN CTX and CTR pins of the DS80C390.
   Make sure you have pull-up resistors connected or it WON'T work.

   Just do a "make" which compiles and compresses it a little.
   Than use JavaKit to load i2c390.hex (it goes into bank 1, see Makefile)
   Type 'E' in JavaKit and enjoy :) */

/* if you are using the (default) interrupt scheme for serial i/o,
   you need to include this in the main program, so the vector will
   be initialized */
#include <serial390.h>

#include <stdio.h>

#include "ds1621.h"
#include "pcf8591.h"


void main (void) {

  while(1) {
    printf ("% 5.2f %03bu %03bu %03bu %03bu\n\r", 
	    ReadDS1621(2),
	    ReadPCF8591(0,0), 
	    ReadPCF8591(0,1), 
	    ReadPCF8591(0,2), 
	    ReadPCF8591(0,3));
  }
}
