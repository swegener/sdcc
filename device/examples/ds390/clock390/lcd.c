#define LCD_ROWS 4
#define LCD_COLLUMNS 20

xdata at 0x380002 static unsigned char lcdIwr;
xdata at 0x38000a static unsigned char lcdDwr;

#ifdef LCD_RW

xdata at 0x380003 static unsigned char lcdIrd;
xdata at 0x38000b static unsigned char lcdDrd;

#define LcdWait { while (lcdIrd&0x80) ; }

#else //#ifdef LCD_RW

// wait for 100us*time
// check this, _PLEASE_

static void Wait100u(long time) {
  register long timeout=time*50;
  while (timeout--)
    ;
}

#define LcdWait { Wait100u(1);}

#endif //#ifdef LCD_RW

// set the dd ram addresses for the rows
// this one is for a 20x4 LCD
xdata static unsigned char lcdLinesStart[LCD_ROWS]={0, 0x40, 0x14, 0x54};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "lcd.h"

void LcdInit() {
  
  Wait100u(150); // >15 ms
  
  lcdIwr=0x38 ;
  Wait100u(50); // >4.1 ms
  
  lcdIwr=0x38;
  Wait100u(1); // >100 us
  
  lcdIwr=0x38;
  Wait100u(1); // >100 u
  
  lcdIwr=0x38; // interface 8 bit
  Wait100u(1); // >40u
  
  lcdIwr=0x0c; // display on
  Wait100u(1); // >40u
  LcdClear();
  Wait100u(1); // >40u
}

void LcdOn() {
  lcdIwr=0x0c; // display on
  LcdWait;
}

void LcdOff() {
  lcdIwr=0x08; // display off
  LcdWait;
}

void LcdCursorOn() {
  // TODO
}

void LcdCursorOff() {
  // TODO
}

void LcdScrollOn() {
  // TODO
}

void LcdScrollOff() {
  // TODO
}

void LcdCharDefine() {
  // TODO
}

void LcdClear() {
  lcdIwr=0x01; // display clear
  Wait100u(50);
}

void LcdHome() {
  lcdIwr=0x80; // set dd ram address 0
  LcdWait;
}

void LcdGoto(unsigned int collumnRow) { // msb=collumn, lsb=row
  lcdIwr=0x80 + \
    lcdLinesStart[collumnRow&0xff] + \
    (collumnRow>>8);
  LcdWait;
}

void LcdPutChar(char c) {
  lcdDwr=c;
  LcdWait;
}

void LcdPutString (char *string) {
  char c;
  while (c=*string++) {
    LcdPutChar (c);
  }
}

void LcdLPutString (unsigned int collumnRow, char *string) {
  LcdGoto(collumnRow);
  LcdPutString(string);
}

// let's hope that no one ever printf's more than the display width,
// however they will :), so to be sure
xdata static char lcdPrintfBuffer[LCD_COLLUMNS*4];

void LcdPrintf (xdata const char *format, ...) reentrant {
  va_list arg;

  va_start (arg, format);
  vsprintf (lcdPrintfBuffer, format, arg);

  LcdPutString(lcdPrintfBuffer);

  va_end (arg);
}

void LcdLPrintf (unsigned int collumnRow, xdata const char *format, ...) reentrant {
  va_list arg;

  LcdGoto(collumnRow);

  // we can not just call LcdPrintf since we have no idea what is on the stack,
  // so we have to do it all over again
  va_start (arg, format);
  vsprintf (lcdPrintfBuffer, format, arg);

  LcdPutString(lcdPrintfBuffer);

  va_end (arg);
}

#if 0
// These don't belong here
void LedOn (void) {
  _asm
    setb P3.5
  _endasm;
}

void LedOff (void) {
  _asm
    clr P3.5
  _endasm;
}

void LedToggle (void) {
  _asm
    cpl P3.5
  _endasm;
}
#endif
