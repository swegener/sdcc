/*KA******************************************************************
* PROJECT: PLOne/8052
**********************************************************************
* FILE: ser.h
**********************************************************************
* CHANGES:
* date      author            description
* --------------------------------------------------------------------
* 04/26/99  we                update
* 04/27/99  we                add comments/header
**********************************************************************
* DESCRIPTION:
* This file is the header to be included by modules which use the
* ser.c module.
**********************************************************************
* FUNCTIONS DECLARED:
* see ser.c
**********************************************************************
* COMPILE TIME OPTIONS: -
* DEBUG OPTIONS: -
******************************************************************KE*/
/*      $Id$    */

#ifndef _SER_H_
#define _SER_H_

void ser_init(void);
void ser_interrupt_handler(void) __interrupt 4 __using 1;
void ser_putc(unsigned char);
unsigned char ser_getc(void);
void ser_printString(char *String);
char ser_charAvail(void);

/*********************End of File************************************/
#endif
