
// Addition tests - mostly int's

/* bit types are not ANSI - so provide a way of disabling bit types
 * if this file is used to test other compilers besides SDCC */
#define SUPPORT_BIT_TYPES 1


unsigned char success=0;
unsigned char failures=0;
unsigned char dummy=0;


char char0 = 0;
char char1 = 0;
char char2 = 0;

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


void done()
{

  dummy++;

}

void add_char2char(void)
{
  if(char0 != 4)
    failures++;
  if(char1 != 5)
    failures++;

  char0 = char0 + char1;

  if(char0 != 9)
    failures++;

  char0 += 127;
  if(char0 > 0)
    failures++;

  if(char0 != -0x78)
    failures++;


}


void main(void)
{
  char0=4;
  char1 = char0 + 1;
  add_char2char();

  success = failures;
  done();
}
