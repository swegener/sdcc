/** Bitfield tests.

*/
#include <testfwk.h>

struct {
  char c0_3 : 3;
  char c3_5 : 5;
} c_bf;

struct {
  int i0_7 : 7;
  int i7_9 : 9;
} i_bf;

struct {
  long l0_7 : 7;
  long l7_10 : 10;
  long l17_15 : 15;
} l_bf;


struct {
  unsigned int b0 : 1;
  unsigned int b1 : 1;
  unsigned int b2 : 1;
  unsigned int b3 : 1;
  unsigned int b4 : 1;
  unsigned int b5 : 1;
  unsigned int b6 : 1;
  unsigned int b7 : 1;
  unsigned int b8 : 1;
  unsigned int b9 : 1;
} sb_bf;

struct {
  unsigned int b0 : 1;
  unsigned int b2 : 1;
} size1a_bf;

struct {
  unsigned int b0 : 1;
  unsigned int b1 : 1;
  unsigned int    : 0;
} size1b_bf;

struct {
  unsigned int b0 : 1;
  unsigned int b1 : 1;
  unsigned int b2 : 6;
} size1c_bf;

struct {
  unsigned int b0 : 1;
  unsigned int    : 0;
  unsigned int b1 : 1;
} size2a_bf;

struct {
  unsigned int b0 : 1;
  unsigned int b1 : 1;
  unsigned int b2 : 1;
  unsigned int b3 : 1;
  unsigned int b4 : 1;
  unsigned int b5 : 1;
  unsigned int b6 : 1;
  unsigned int b7 : 1;
  unsigned int b8 : 1;
  unsigned int b9 : 1;
} size2b_bf;

struct {
  unsigned int b0 : 4;
  unsigned int b1 : 5;
} size2c_bf;

struct {
  unsigned int b0 : 12;
  unsigned int b1 : 3;
} size2d_bf;

struct {
  unsigned int b0 : 3;
  unsigned int b1 : 12;
} size3a_bf;


void
testBitfieldSizeof(void)
{
  /* Although bitfields are extremely implementation dependant, these
     assertions should hold for all implementations with storage units
     of 8 bits or larger (nearly universal).
  */
  ASSERT( sizeof(size1a_bf) >= 1);
  ASSERT( sizeof(size1b_bf) >= 1);
  ASSERT( sizeof(size1c_bf) >= 1);
  ASSERT( sizeof(size2a_bf) >= 2);
  ASSERT( sizeof(size2b_bf) >= 2);
  ASSERT( sizeof(size2c_bf) >= 2);
  ASSERT( sizeof(size2d_bf) >= 2);
  ASSERT( sizeof(size3a_bf) >= 2);
  ASSERT( sizeof(size1a_bf) == sizeof(size1b_bf));
  ASSERT( sizeof(size1a_bf) < sizeof(size2a_bf));

  /* Some SDCC specific assertions. SDCC uses 8 bit storage units.
     Bitfields that are less than 8 bits, but would (due to earlier
     bitfield declarations) span a storage unit boundary are
     realigned to the next storage unit boundary. Bitfields of
     8 or greater bits are always aligned to start on a storage
     unit boundary.
  */
#ifdef SDCC
  ASSERT( sizeof(size1a_bf) == 1);
  ASSERT( sizeof(size1b_bf) == 1);
  ASSERT( sizeof(size1c_bf) == 1);
  ASSERT( sizeof(size2a_bf) == 2);
  ASSERT( sizeof(size2b_bf) == 2);
  ASSERT( sizeof(size2c_bf) == 2);
  ASSERT( sizeof(size2d_bf) == 2);
  ASSERT( sizeof(size3a_bf) == 3);
#endif
}


void
testBitfieldsSingleBitLiteral(void)
{
  size2b_bf.b0 = 0; 
  size2b_bf.b1 = 0; 
  size2b_bf.b2 = 0; 
  size2b_bf.b3 = 0; 
  size2b_bf.b4 = 0; 
  size2b_bf.b5 = 0; 
  size2b_bf.b6 = 0; 
  size2b_bf.b7 = 0; 
  size2b_bf.b8 = 0; 
  size2b_bf.b9 = 0; 

  /* make sure modulo 2 truncation works */
  size2b_bf.b0 = 0x3fe;
  ASSERT(size2b_bf.b0==0);
  ASSERT(size2b_bf.b1==0);
  ASSERT(size2b_bf.b2==0);
  ASSERT(size2b_bf.b3==0);
  ASSERT(size2b_bf.b4==0);
  ASSERT(size2b_bf.b5==0);
  ASSERT(size2b_bf.b6==0);
  ASSERT(size2b_bf.b7==0);
  ASSERT(size2b_bf.b8==0);
  ASSERT(size2b_bf.b9==0);
  size2b_bf.b0 = 0x3ff;
  ASSERT(size2b_bf.b0==1);
  ASSERT(size2b_bf.b1==0);
  ASSERT(size2b_bf.b2==0);
  ASSERT(size2b_bf.b3==0);
  ASSERT(size2b_bf.b4==0);
  ASSERT(size2b_bf.b5==0);
  ASSERT(size2b_bf.b6==0);
  ASSERT(size2b_bf.b7==0);
  ASSERT(size2b_bf.b8==0);
  ASSERT(size2b_bf.b9==0);

  /* make sure both bytes work */
  size2b_bf.b9 = 0x3ff;
  ASSERT(size2b_bf.b0==1);
  ASSERT(size2b_bf.b1==0);
  ASSERT(size2b_bf.b2==0);
  ASSERT(size2b_bf.b3==0);
  ASSERT(size2b_bf.b4==0);
  ASSERT(size2b_bf.b5==0);
  ASSERT(size2b_bf.b6==0);
  ASSERT(size2b_bf.b7==0);
  ASSERT(size2b_bf.b8==0);
  ASSERT(size2b_bf.b9==1);
}

void
testBitfieldsSingleBit(void)
{
  volatile unsigned char c;

  c = 0;
  size2b_bf.b0 = c; 
  size2b_bf.b1 = c; 
  size2b_bf.b2 = c; 
  size2b_bf.b3 = c; 
  size2b_bf.b4 = c; 
  size2b_bf.b5 = c; 
  size2b_bf.b6 = c; 
  size2b_bf.b7 = c; 
  size2b_bf.b8 = c; 
  size2b_bf.b9 = c; 

  /* make sure modulo 2 truncation works */
  c = 0xfe;
  size2b_bf.b0 = c;
  ASSERT(size2b_bf.b0==0);
  ASSERT(size2b_bf.b1==0);
  ASSERT(size2b_bf.b2==0);
  ASSERT(size2b_bf.b3==0);
  ASSERT(size2b_bf.b4==0);
  ASSERT(size2b_bf.b5==0);
  ASSERT(size2b_bf.b6==0);
  ASSERT(size2b_bf.b7==0);
  ASSERT(size2b_bf.b8==0);
  ASSERT(size2b_bf.b9==0);
  c++;
  size2b_bf.b0 = c;
  ASSERT(size2b_bf.b0==1);
  ASSERT(size2b_bf.b1==0);
  ASSERT(size2b_bf.b2==0);
  ASSERT(size2b_bf.b3==0);
  ASSERT(size2b_bf.b4==0);
  ASSERT(size2b_bf.b5==0);
  ASSERT(size2b_bf.b6==0);
  ASSERT(size2b_bf.b7==0);
  ASSERT(size2b_bf.b8==0);
  ASSERT(size2b_bf.b9==0);

  /* make sure both bytes work */
  size2b_bf.b9 = c;
  ASSERT(size2b_bf.b0==1);
  ASSERT(size2b_bf.b1==0);
  ASSERT(size2b_bf.b2==0);
  ASSERT(size2b_bf.b3==0);
  ASSERT(size2b_bf.b4==0);
  ASSERT(size2b_bf.b5==0);
  ASSERT(size2b_bf.b6==0);
  ASSERT(size2b_bf.b7==0);
  ASSERT(size2b_bf.b8==0);
  ASSERT(size2b_bf.b9==1);
}

void
testBitfieldsMultibitLiteral(void)
{
  size2c_bf.b0 = 0xff;	/* should truncate to 0x0f */
  size2c_bf.b1 = 0;
  ASSERT(size2c_bf.b0==0x0f);
  ASSERT(size2c_bf.b1==0);

  size2c_bf.b1 = 0xff;	/* should truncate to 0x1f */
  size2c_bf.b0 = 0;
  ASSERT(size2c_bf.b0==0);
  ASSERT(size2c_bf.b1==0x1f);

  size2d_bf.b0 = 0xffff; /* should truncate to 0x0fff */
  size2d_bf.b1 = 0;
  ASSERT(size2d_bf.b0==0x0fff);
  ASSERT(size2d_bf.b1==0);

  size2d_bf.b1 = 0xffff; /* should truncate to 0x07 */
  size2d_bf.b0 = 0;
  ASSERT(size2d_bf.b0==0);
  ASSERT(size2d_bf.b1==0x07);

  size2d_bf.b0 = 0x0321;
  size2d_bf.b1 = 1;
  ASSERT(size2d_bf.b0==0x0321);
  ASSERT(size2d_bf.b1==1);

  size2d_bf.b0 = 0x0a46;
  size2d_bf.b1 = 5;
  ASSERT(size2d_bf.b0==0x0a46);
  ASSERT(size2d_bf.b1==5);
}

void
testBitfieldsMultibit(void)
{
  volatile int allones = 0xffff;
  volatile int zero = 0;
  volatile int x;
  
  size2c_bf.b0 = allones;	/* should truncate to 0x0f */
  size2c_bf.b1 = zero;
  ASSERT(size2c_bf.b0==0x0f);
  ASSERT(size2c_bf.b1==0);

  size2c_bf.b1 = allones;	/* should truncate to 0x1f */
  size2c_bf.b0 = zero;
  ASSERT(size2c_bf.b0==0);
  ASSERT(size2c_bf.b1==0x1f);

  size2d_bf.b0 = allones; /* should truncate to 0x0fff */
  size2d_bf.b1 = zero;
  ASSERT(size2d_bf.b0==0x0fff);
  ASSERT(size2d_bf.b1==0);

  size2d_bf.b1 = allones; /* should truncate to 0x07 */
  size2d_bf.b0 = zero;
  ASSERT(size2d_bf.b0==0);
  ASSERT(size2d_bf.b1==0x07);

  x = 0x0321;
  size2d_bf.b0 = x;
  x = 1;
  size2d_bf.b1 = x;
  ASSERT(size2d_bf.b0==0x0321);
  ASSERT(size2d_bf.b1==1);

  x = 0x0a46;
  size2d_bf.b0 = x;
  x = 5;
  size2d_bf.b1 = x;
  ASSERT(size2d_bf.b0==0x0a46);
  ASSERT(size2d_bf.b1==5);
}

void
testBitfields(void)
{
#if 0 // not yet
  c_bitfield.c0_3 = 2;
  c_bitfield.c3_5 = 3;
  ASSERT(*(char *)(&c_bitfield) == (2 + (3<<3)) );

  i_bitfield.i0_7 = 23;
  i_bitfield.i7_9 = 234;
  ASSERT(*(int *)(&i_bitfield) == (23 + (234<<7)) );

  l_bitfield.l0_7 = 23;
  l_bitfield.l7_10 = 234;
  l_bitfield.l17_15 = 2345;
  ASSERT(*(long *)(&l_bitfield) == (23 + (234<<7) + (2345<<17)) );
#endif
}
