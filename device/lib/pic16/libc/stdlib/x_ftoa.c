
/*
 * wrapper function to use _convert_float
 *
 * written by Vangelis Rokas, 2004, <vrokas AT otenet.gr>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/*
 * $Id$
 */

#include <float.h>

extern convert_frac;
extern convert_int;

/* char x_ftoa(float, data char *, unsigned char, unsigned char); */


extern POSTDEC1;
extern PLUSW2;
extern FSR0L;
extern FSR0H;
extern PREINC1;
extern PREINC2;
extern FSR2L;
extern FSR2H;

#define _vv0x00	0x00
#define _vv0x01	0x01
#define _vv0x02	0x02
#define _vv0x03	0x03
#define _vv0x04	0x04

char x_cnvint_wrap(unsigned long num, data char *buffer)
{
  num;
  buffer;
  
  _asm
    movff	_vv0x00, _POSTDEC1
    movff	_vv0x01, _POSTDEC1
    movff	_vv0x02, _POSTDEC1
    movff	_vv0x03, _POSTDEC1

    movlw	2
    movff	_PLUSW2, _vv0x00
    movlw	3
    movff	_PLUSW2, _vv0x01
    movlw	4
    movff	_PLUSW2, _vv0x02
    movlw	5
    movff	_PLUSW2, _vv0x03

    movlw	6
    movff	_PLUSW2, _FSR0L
    movlw	7
    movff	_PLUSW2, _FSR0H

    call	_convert_int

    /* return value is already in WREG */

    movff	_PREINC1, _vv0x03
    movff	_PREINC1, _vv0x02
    movff	_PREINC1, _vv0x01
    movff	_PREINC1, _vv0x00
  _endasm ;
}

char x_cnvfrac_wrap(unsigned long num, data char *buffer, unsigned char prec)
{
  num;
  buffer;
  prec;
  
  _asm
    movff	_vv0x00, _POSTDEC1
    movff	_vv0x01, _POSTDEC1
    movff	_vv0x02, _POSTDEC1
    movff	_vv0x03, _POSTDEC1
    movff	_vv0x04, _POSTDEC1

    movlw	2
    movff	_PLUSW2, _vv0x00
    movlw	3
    movff	_PLUSW2, _vv0x01
    movlw	4
    movff	_PLUSW2, _vv0x02
    movlw	5
    movff	_PLUSW2, _vv0x03
    
    movlw	6
    movff	_PLUSW2, _FSR0L
    movlw	7
    movff	_PLUSW2, _FSR0H

    movlw	8
    movff	_PLUSW2, _vv0x04
    
    call	_convert_frac

    /* return value is already in WREG */
    
    movff	_PREINC1, _vv0x04
    movff	_PREINC1, _vv0x03
    movff	_PREINC1, _vv0x02
    movff	_PREINC1, _vv0x01
    movff	_PREINC1, _vv0x00
  _endasm ;
}



union float_long {
  unsigned long l;
  float f;
};

char x_ftoa(float num, data char *buffer, unsigned char buflen, unsigned char prec)
{
  char len;
  char expn;
  unsigned long ll;
  unsigned long li;
//  volatile
  union float_long f_l;

    len = buflen;
    while(len--)buffer[len] = 0;

    f_l.f = num;

    if((f_l.l & SIGNBIT) == SIGNBIT) {
      f_l.l &= ~SIGNBIT;
      *buffer = '-';
      buffer++;
    }

    expn = EXCESS - EXP(f_l.l);	// - 24;
      
    ll = MANT(f_l.l);
    li = 0;
    
    while( expn ) {
      if(expn < 0) {
        li <<= 1;
        if(ll & 0x00800000UL)li |= 1;
        ll <<= 1;
        expn++;
      } else {
        ll >>= 1;
        expn--;
      }
    }

    if(li)
      len = x_cnvint_wrap(li, buffer);
    else {
      *buffer = '0'; len = 1;
    }
    
    buffer += len;

    if(prec) {
      *buffer = '.'; len++;
      buffer++;
    
      len += x_cnvfrac_wrap(ll, buffer, 24-prec);
      buffer[ prec ] = '\0';
    }
    
  return (len);
}
