
#include <pic18fregs.h>

#include <i2c.h>

void i2c_open(unsigned char mode, unsigned char slew, unsigned char addr_brd)
{
  SSPSTAT &= 0x3f;
  SSPCON1 = 0;
  SSPCON2 = 0;
  SSPCON1 |= mode;
  SSPSTAT |= slew;
  
  
#if defined(pic18f2455) || defined (pic18f2550) \
    || defined(pic18f4455) || defined (pic18f4550)

  TRISBbits.TRISB1 = 1;
  TRISBbits.TRISB0 = 1;

#else	/* all other devices */

  TRISCbits.TRISC3 = 1;
  TRISCbits.TRISC2 = 1;

#endif

  SSPADD = addr_brd;

  SSPCON1 |= 0x20;
}

