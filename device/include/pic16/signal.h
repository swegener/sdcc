
/*
 * Signal handler header
 *
 * written by Vangelis Rokas, 2005 <vrokas AT otenet.gr>
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
 *
 *
 * $Id$
 */

#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define RBIF    0x0
#define INT0IF  0x1
#define TMR0IF  0x2

#define INT1IF  0x0
#define INT2IF  0x1

#define TMR1IF  0x0
#define TMR2IF  0x1
#define CCP1IF  0x2
#define SSPIF   0x3
#define TXIF    0x4
#define RCIF    0x5
#define ADIF    0x6
#define PSPIF   0x7

#define CCP2IF  0x0
#define TMR3IF  0x1
#define LVDIF   0x2
#define BCLIF   0x3
#define EEIF    0x4
#define USBIF	0x5

/* interrupt testing arguments */
#define SIG_RB          _INTCON, RBIF
#define SIG_INT0        _INTCON, INT0IF
#define SIG_INT1        _INTCON3, INT1IF
#define SIG_INT2        _INTCON3, INT2IF
#define SIG_CCP1        _PIR1, CCP1IF
#define SIG_CCP2        _PIR2, CCP2IF
#define SIG_TMR0        _INTCON, TMR0IF
#define SIG_TMR1        _PIR1, TMR1IF
#define SIG_TMR2        _PIR1, TMR2IF
#define SIG_TMR3        _PIR2, TMR3IF
#define SIG_EE          _PIR2, EEIF
#define SIG_BCOL        _PIR2, BCLIF
#define SIG_LVD         _PIR2, LVDIF
#define SIG_PSP         _PIR1, PSPIF
#define SIG_AD          _PIR1, ADIF
#define SIG_RC          _PIR1, RCIF
#define SIG_TX          _PIR1, TXIF
#define SIG_MSSP        _PIR1, SSPIF
#define SIG_USB		_PIR2, USBIF


#define DEF_ABSVECTOR(vecno, name)      \
void __ivt_ ## name(void) __interrupt vecno __naked \
{\
  __asm                          \n\
    goto        _ ## name           \n\
  __endasm;                      \
}

#define DEF_INTHIGH(name)       \
DEF_ABSVECTOR(1, name)                   \
void name(void) __naked __interrupt        \
{\
  __asm                                  \n
  

#define DEF_INTLOW(name)        \
DEF_ABSVECTOR(2, name)                   \
void name(void) __naked __interrupt        \
{\
  __asm                          \n


#define END_DEF                 \
    retfie                        \n\
  __endasm;\
}


#define DEF_HANDLER(sig, handler)       \
    btfsc       sig                 \n\
    goto        _ ## handler


#define SIGHANDLER(handler)             void handler (void) __interrupt
#define SIGHANDLERNAKED(handler)        void handler (void) __naked __interrupt

#endif  /* __SIGNAL_H__ */
