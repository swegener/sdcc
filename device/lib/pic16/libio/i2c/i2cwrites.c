
#include <pic18fregs.h>

#include <i2c.h>

char i2c_writestr(unsigned char *ptr)
{
  while( *ptr ) {
    if( SSPCON1bits.SSPM3 ) {
      if(i2c_writechar( *ptr )) {
        return (-3);
      }
      I2C_IDLE();
      if( SSPCON2bits.ACKSTAT ) {
        return (-2);
      }
    } else {
      PIR1bits.SSPIF = 0;
      SSPBUF = *ptr;
      SSPCON1bits.CKP = 1;
      while( !PIR1bits.SSPIF );
      
      if((!SSPSTATbits.R_W) && ( !SSPSTATbits.BF )) {
        return (-2);
      }
    }
    
    ptr++;
  }

  return 0;
}
