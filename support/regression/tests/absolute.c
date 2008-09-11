/** Absolute addressing tests.

    mem: code, xdata
*/
#include <testfwk.h>

#ifdef SDCC_pic16
# define ADDRESS(x) (0x02 ## x)
#else
# define ADDRESS(x) (0xCA ## x)
#endif

typedef struct
{
        int a,b;
} TestStruct;

{mem} volatile at(ADDRESS(BC)) TestStruct TestVar = {0,0};
{mem} at(ADDRESS(B7)) char u;
{mem} at(ADDRESS(B7)) char x = 'x';
{mem} at(ADDRESS(B9)) char y = 'y';
{mem} at(ADDRESS(B0)) int  k = 0x1234;

char z = 'z';

void
testAbsolute(void)
{
#if defined(SDCC_mcs51) || defined(SDCC_ds390) || defined(SDCC_hc08)
  static {mem} at(ADDRESS(B6)) char s = 's';
  char {mem} *pC = (char {mem} *)(ADDRESS(B0));
  int  {mem} *pI = (int  {mem} *)(ADDRESS(B0));

  ASSERT(u == 'x');
  ASSERT(pC[7] == 'x');
  ASSERT(pC[9] == 'y');
  ASSERT(pC[6] == 's');
  ASSERT(pI[0] == 0x1234);
#endif
}

#if defined(SDCC_mcs51) || defined(SDCC_ds390)
volatile data at(0x20) unsigned char Byte0 = 0x00;
volatile data at(0x22) unsigned char Byte1 = 0x00;
volatile bit Bit0, Bit1, Bit2, Bit3, Bit4, Bit5, Bit6, Bit7, Bit8;
#endif

void
testAbsBdata(void)
{
#if defined(SDCC_mcs51) || defined(SDCC_ds390)
  Bit0 = 1;
  ASSERT(Byte0 == 0x00);
  Byte0 = 0xFF;
  Bit0 = 0;
  ASSERT(Byte0 == 0xFF);
#endif
}
