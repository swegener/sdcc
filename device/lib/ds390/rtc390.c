/*-------------------------------------------------------------------------
  rtc390.c - rtc routines for the DS1315 (tested on TINI)
  
   Written By - Johan Knol, johan.knol@iduna.nl
    
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
   
   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/

#include <tinibios.h>

#include <stdio.h>
#include <ctype.h>

/* this is the address of the ds1315 phantom time chip, although
   it doesn't really matter as long as it's in the 300000-3ffff
   range since the chip only uses CE3*
*/

xdata at 0x310000 static volatile unsigned char rtc;

// this is the 64bit pattern that has to be recognized by the ds1315
code unsigned char rtcMagic[8]={0xc5,0x3a,0xa3,0x5c,0xc5,0x3a,0xa3,0x5c};

static struct RTCDate{
  int year;
  unsigned char month, day, weekDay, hour, minute, second, hundredth;
};

#define BCDtoINT(x) (((x)&0x0f)+((x)>>4)*10)
#define INTtoBCD(x) (((x)%10)+(((x)/10)<<4))

static void RtcSync(void) {
  unsigned char dummy, byte,bitMask;

  // reset rtc chip
  dummy=rtc;

  // say the magic word
  for (byte=0; byte<8; byte++) {
    for (bitMask=0x01; bitMask; bitMask<<=1) {
      rtc = (rtcMagic[byte]&bitMask) ? 0xff : 0x00;
    }
  }
}

unsigned char RtcRead(struct RTCDate *rtcDate) {
  unsigned char rtcBytes[8];
  unsigned char byte,bitMask;

  RtcSync();

  for (byte=0; byte<8; byte++) {
    rtcBytes[byte]=0;
    for (bitMask=0x01; bitMask; bitMask<<=1) {
      if (rtc&1) {
	rtcBytes[byte]|=bitMask;
      }
    }
  }
  rtcDate->year=2000 + BCDtoINT(rtcBytes[7]);
  rtcDate->month=BCDtoINT(rtcBytes[6]);
  rtcDate->day=BCDtoINT(rtcBytes[5]);
  rtcDate->weekDay=rtcBytes[4]&0x07;
  rtcDate->hour=BCDtoINT(rtcBytes[3]);
  rtcDate->minute=BCDtoINT(rtcBytes[2]);
  rtcDate->second=BCDtoINT(rtcBytes[1]);
  rtcDate->hundredth=BCDtoINT(rtcBytes[0]);
  if ((rtcBytes[4]&0x30) || (rtcBytes[3]&0x80)) {
    //oscillator not running, reset not active or in 12h mode
    return 0;
  }
  return 1;
}

void RtcWrite(struct RTCDate *rtcDate) {
  unsigned char rtcBytes[8];
  unsigned char byte,bitMask;

  rtcBytes[7]=INTtoBCD(rtcDate->year-2000);
  rtcBytes[6]=INTtoBCD(rtcDate->month);
  rtcBytes[5]=INTtoBCD(rtcDate->day);
  rtcBytes[4]=INTtoBCD(rtcDate->weekDay)&0x07; //set 24h  mode
  rtcBytes[3]=INTtoBCD(rtcDate->hour)&0x3f; // oscilator on, reset on
  rtcBytes[2]=INTtoBCD(rtcDate->minute);
  rtcBytes[1]=INTtoBCD(rtcDate->second);
  rtcBytes[0]=INTtoBCD(rtcDate->hundredth);

  RtcSync();

  for (byte=0; byte<8; byte++) {
    for (bitMask=0x01; bitMask; bitMask<<=1) {
      rtc = (rtcBytes[byte]&bitMask) ? 0xff : 0x00;
    }
  }
}
