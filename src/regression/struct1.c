//#include "p16c84.h"
// Addition tests

/* bit types are not ANSI - so provide a way of disabling bit types
 * if this file is used to test other compilers besides SDCC */
#define SUPPORT_BIT_TYPES 1

/* Some compilers that support bit types do not support bit arithmetic 
 * (like bitx = bity + bitz;) */
#define SUPPORT_BIT_ARITHMETIC 1

unsigned char success=0;
unsigned char failures=0;
unsigned char dummy=0;

#if SUPPORT_BIT_TYPES

bit bit0 = 0;
bit bit1 = 0;
bit bit2 = 0;
bit bit3 = 0;
bit bit4 = 0;
bit bit5 = 0;
bit bit6 = 0;
bit bit7 = 0;
bit bit8 = 0;
bit bit9 = 0;
bit bit10 = 0;
bit bit11 = 0;

#endif

unsigned int aint0 = 0;
unsigned int aint1 = 0;
unsigned char achar0 = 0;
unsigned char achar1 = 0;
unsigned char *acharP = 0;

struct chars {
  unsigned char c0,c1;
};


struct chars char_struct;

void done()
{

  dummy++;

}

void struct_test(void)
{

  if(char_struct.c0 || char_struct.c1)
    failures++;

  char_struct.c0++;

  if(char_struct.c0 != 1)
    failures++;
}

void ptr_to_struct(struct chars *p)
{

  if(p->c1)
    failures++;


  p->c1++;

  if(p->c1 != 1)
    failures++;
}


void main(void)
{


  char_struct.c0 = 0;
  char_struct.c1 = 0;
  struct_test();
  ptr_to_struct(&char_struct);

  success = failures;
  done();
}
