#ifndef TINIBIOS_H

#define TINIBIOS_H

#include <ds80c390.h>

#define Serial0GetChar getchar
#define Serial0PutChar putchar

void Serial0Init (unsigned long baud, unsigned char buffered);
char Serial0GetChar(void);
void Serial0PutChar(char);
char Serial0CharArrived(void);
void Serial0Baud(unsigned long baud);
void Serial0SendBreak(void);
void Serial0Flush(void);

void Serial1Init (unsigned long baud, unsigned char buffered);
char Serial1GetChar(void);
void Serial1PutChar(char);
char Serial1CharArrived(void);
void Serial1Baud(unsigned long baud);
void Serial1SendBreak(void);
void Serial1Flush(void);

unsigned long ClockTicks();
void ClockMilliSecondsDelay(unsigned long ms);
void ClockMicroSecondsDelay(unsigned int us);

#define SERIAL_0_BAUD 115200L
#define SERIAL_1_BAUD 9600L

// these need to be binary numbers
#define SERIAL_0_RECEIVE_BUFFER_SIZE 1024
#define SERIAL_1_RECEIVE_BUFFER_SIZE 64

// I know someone is fooling with the crystals
#define OSCILLATOR 18432000L

/* Set the cpu speed in clocks per machine cycle, valid values are:
   1024: Divide-by-1024 (power management) mode (screws ALL timers and serial)
      4: Standard 8051 divide-by-4 mode
      2: Use 2x xtal multiplier 
      1: Use 4x xtal multiplier (Don't do this with a TINI at 18.432MHz)
*/
#define CPU_SPEED 2
void CpuSpeed(unsigned int speed);

// The MOVX stretch cycles, see datasheet
#define CPU_MOVX_STRETCH 0x01

// internal functions used by tinibios.c
unsigned char _sdcc_external_startup(void);
void Serial0IrqHandler (void) interrupt 4;
void Serial1IrqHandler (void) interrupt 7;
void ClockInit();
void ClockIrqHandler (void) interrupt 1;

#endif TINIBIOS_H
