//#include "p16c84.h"

unsigned char success=0;
unsigned char failures=0;
unsigned char dummy=0;

bit bit0 = 0;
unsigned int aint0 = 0;
unsigned int aint1 = 0;
unsigned char achar0 = 0;
unsigned char achar1 = 0;

unsigned char call3(void);

void done()
{

  dummy++;

}

void for1(void)
{
  unsigned char i=0;

  for(i=0; i<10; i++)
    achar0++;

  if(achar0 != 10)
    failures++;

}

void for2(void)
{
  unsigned char i=0;

  for(i=0; i<10; i++)
    achar0++;

  if(i < 10)
    failures++;

}

void main(void)
{
  for1();
  for2();

  success = failures;
  done();
}
