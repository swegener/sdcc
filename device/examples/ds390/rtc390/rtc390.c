/*-------------------------------------------------------------------------
  rtc390.c - rtc demo for the DS1315 (tested on TINI)
  
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

#include <stdio.h>
#include <ctype.h>

//#define USE_LCD

#ifdef USE_LCD
#include "lcd.h"
#endif

int ScanInt(int current) {
  char reply[32], *r;

  gets(reply);
  if (isdigit(*(r=reply))) {
    current=0;
    do {
      current*=10;
      current+=(*r++)-'0';
    } while (isdigit(*r));
  }
  return current;
}

char GetTime(struct RTCDate *rtcDate) {
  printf ("Enter year [%d]: ", rtcDate->year);
  rtcDate->year=ScanInt(rtcDate->year);
  printf ("Enter month [%d]: ", rtcDate->month);
  rtcDate->month=ScanInt(rtcDate->month);
  printf ("Enter day [%d]: ", rtcDate->day);
  rtcDate->day=ScanInt(rtcDate->day);
  printf ("Enter hour [%d]: ", rtcDate->hour);
  rtcDate->hour=ScanInt(rtcDate->hour);
  printf ("Enter minute [%d]: ", rtcDate->minute);
  rtcDate->minute=ScanInt(rtcDate->minute);
  printf ("Enter second [%d]: ", rtcDate->second);
  rtcDate->second=ScanInt(rtcDate->second);
  rtcDate->hundredth=0;
}

void PrintTime(struct RTCDate *rtcDate) {
  printf ("%04d-%02bd-%02bd %02bd:%02bd:%02bd.%02bd\n", 
	  rtcDate->year, rtcDate->month, rtcDate->day,
	  rtcDate->hour, rtcDate->minute, rtcDate->second,
	  rtcDate->hundredth);
}

void main (void) {
  struct RTCDate rtcDate;
  unsigned char seconds=0xff;

  printf ("\nStarting RTC demo.\n");

#ifdef USE_LCD
  LcdInit();
  LcdLPrintf (0, "Starting RTC demo.");
#endif
  
  while(1) {
    RtcRead(&rtcDate);

#ifdef USE_LCD
    // if lcd is enabled it only syncs the second time
    RtcRead(&rtcDate);
#endif    

#ifdef USE_LCD
    LcdLPrintf (2,"%04d-%02bd-%02bd",
		rtcDate.year, rtcDate.month, rtcDate.day);
    LcdLPrintf (3, "%02bd:%02bd:%02bd.%02bd", 
		rtcDate.hour, rtcDate.minute, rtcDate.second,
		rtcDate.hundredth);
#endif
    if (rtcDate.second!=seconds) {
      PrintTime(&rtcDate);
      seconds=rtcDate.second;
    }
    
    if (Serial0CharArrived()) {
      switch (getchar()) 
	{
	case 0:
	  break;
	case 'q': 
	  printf ("Ok.\n");
	  return;
	default:
	  if (GetTime(&rtcDate)) {
#ifndef USE_LCD
	    PrintTime(&rtcDate);
	    RtcWrite(&rtcDate);
	    printf ("Time written.\n");
#endif
	  }
	}
    }
  }

}
