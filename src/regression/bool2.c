#define __16F873
#include "p16f873.h"
//#include "p16c84.h"

unsigned char success=0;
unsigned char failures=0;
unsigned char dummy=0;

bit bit0 = 0;
bit bit1 = 0;
unsigned int aint0 = 0;
unsigned int aint1 = 0;
unsigned char achar0 = 0;
unsigned char achar1 = 0;

void done()
{

  dummy++;

}



void bit_invert(void)
{

  bit0 = !bit0;
  bit1 = !bit1;

  if((bit0 != bit1) || (bit0 == 0))
    failures++;
    
}

void bit_copy(void)
{

  bit0 = !bit0;
  bit1 = bit0;
}

void main(void)
{

  bit_invert();
  bit_copy();

  success = failures;
  done();
}
