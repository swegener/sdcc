
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
    || defined(pic18f4455) || defined (pic18f4550) \
    || defined(pic18f66j60) || defined(pic18f66j65) || defined(pic18f67j60) \
    || defined(pic18f86j60) || defined(pic18f86j65) || defined(pic18f87j60) \
    || defined(pic18f96j60) || defined(pic18f96j65) || defined(pic18f97j60)

  TRISBbits.TRISB1 = 1;
  TRISBbits.TRISB0 = 1;

#elif  defined(pic18f24j50) || defined(pic18f25j50) || defined(pic18f26j50) \
    || defined(pic18f44j50) || defined(pic18f45j50) || defined(pic18f46j50)

  TRISBbits.TRISB4 = 1;
  TRISBbits.TRISB5 = 1;

#else	/* all other devices */

  TRISCbits.TRISC3 = 1;
  TRISCbits.TRISC2 = 1;

#endif

  SSPADD = addr_brd;

  SSPCON1 |= 0x20;
}

