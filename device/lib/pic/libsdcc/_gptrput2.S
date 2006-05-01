; -------------------------------------------------------------------------
;  _gptrput2.S : write two bytes pointed to by a generic pointer
;
; 	Written By	Raphael Neider <rneider AT web.de> (2005)
;
;  This library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU Library General Public
;  License as published by the Free Software Foundation; either
;  version 2 of the License, or (at your option) any later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;  Library General Public License for more details.
;
;  You should have received a copy of the GNU Library General Public
;  License along with this library; if not, write to the 
;  Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
;  Boston, MA  02111-1307  USA.
;
;  In other words, you are welcome to use, share and improve this program.
;  You are forbidden to forbid anyone else to use, share and improve
;  what you give them.   Help stamp out software-hoarding!
;
;  $Id$
;  ------------------------------------------------------------------------

; calling conventions:
;   3 byte generic pointer is passed in via (WREG STK00 STK01).
;   The result is returned in (WREG (STK00 (STK01 (STK02)))).
;
;   unsigned char _gptrget  (void *gptr);
;   unsigned char _gptrget1 (void *gptr);
;   unsigned int  _gptrget2 (void *gptr);
;   void *        _gptrget3 (void *gptr);
;   unsigned long _gptrget4 (void *gptr);
;
;   void _gptrput  (void *ptr, unsigned char val);
;   void _gptrput1 (void *ptr, unsigned char val);
;   void _gptrput2 (void *ptr, unsigned int  val);
;   void _gptrput3 (void *ptr, unsigned int  val);
;   void _gptrput4 (void *ptr, unsigned long val);

include macros.inc
include p16f_common.inc

	global	__gptrput2
	
	CODE

__gptrput2:
	check_data	__dataptrput2
	; cannot write to __code space
	return

__dataptrput2:
	setup_fsr
	movf	STK03, W	; get LSB(val)
	movwf	_INDF
	inc_fsr
	movf	STK02, W	; get MSB(val)
	movwf	_INDF
	return

	END
