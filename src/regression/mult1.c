#define __16F873
#include "p16f873.h"
unsigned char success=0;
unsigned char failures=0;
unsigned char dummy=0;


signed char c1,c2,c3;
unsigned char uc1,uc2,uc3;

unsigned int ui1,ui2,ui3;
signed int i1,i2;

void done()
{

  dummy++;

}

void m1(void)
{

  c1 = c1*5;       // char = char * lit

  c2 = c1 *c3;     // char = char * char

  uc1= uc1*5;      // uchar = uchar * lit
  uc2=uc1*uc3;     // uchar = uchar * uchar

  if(c2 != 25)
    failures++;
}

void m2(unsigned char uc)
{

  uc2 = uc1 * uc;

  if(uc2 != 0x20)
    failures++;
}

void main(void)
{
  dummy = 0;

  c1 = 1;
  c3 = 5;

  //m1();

  uc1 = 0x10;
  m2(2);

  ui1 = uc1*uc2;   // uint = uchar * uchar

  i1 = c1*c2;      // int = char * char

  ui3 = ui1*ui2;   // uint = uint * unit


  success = failures;
  done();
}
