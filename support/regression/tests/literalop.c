/* Test operandOperation() in SDCCicode.c

    type: char, short, LONG
 */
#include <testfwk.h>

/* 64 bit hosts */
#if defined(__alpha) || defined(__amd64)
#  define LONG int
#else
#  define LONG long
#endif

typedef   signed {type} stype;
typedef unsigned {type} utype;

#define _{type}

#if defined(PORT_HOST) || defined(SDCC_z80) || defined(SDCC_gbz80)
#  define idata
#  define code
#endif

volatile char is8 = 8;

signed char  sc;
signed short ss;
signed LONG  sl;
unsigned char  uc;
unsigned short us;
unsigned LONG  ul;
volatile signed char  vsc;
volatile signed short vss;
volatile signed LONG  vsl;
volatile unsigned char  vuc;
volatile unsigned short vus;
volatile unsigned LONG  vul;
stype s;
volatile stype vs;
utype u;
volatile utype vu;

unsigned LONG t1, t2;

void
testOpOp(void)
{
  /* mul ast */
  ASSERT((stype) -3 * (stype) -1 == (stype)  3);
  ASSERT((stype) -3 * (stype)  1 == (stype) -3);
  ASSERT((stype)  3 * (stype) -1 == (stype) -3);

  ASSERT((stype)  1 * (utype) 0xfffffff7 == (utype) 0xfffffff7);

  ASSERT((unsigned char ) 0xfffffff8 * (unsigned char ) 0xfffffff7 == 0xef48);
  ASSERT((unsigned short) 0xfffffff8 * (unsigned short) 0xfffffff7 == (sizeof(int) == 2 ? 0x0048 : 0xffef0048));
  ASSERT((unsigned LONG ) 0xfffffff8 * (unsigned LONG ) 0xfffffff7 == 0x0048);

  ASSERT((stype         ) 0xfffffff8 * (stype         ) 0xfffffff7 == 72);

  ASSERT((signed char ) -1 * (unsigned char ) 0xfffffff7 == (sizeof(int) == 2 ? 0xff09 : 0xffffff09));
  ASSERT((signed short) -1 * (unsigned short) 0xfffffff7 == (sizeof(int) == 2 ?     9u : 0xffff0009));
  ASSERT((signed LONG ) -1 * (unsigned LONG ) 0xfffffff7 == 9u);

  ASSERT((signed char ) -2 * (unsigned char ) 0x8004 == (sizeof(int) == 2 ? 0xfff8 : 0xfffffff8));
  ASSERT((signed short) -2 * (unsigned short) 0x8004 == (sizeof(int) == 2 ? 0xfff8 : 0xfffefff8));
  ASSERT((signed LONG ) -2 * (unsigned LONG ) 0x8004 == 0xfffefff8);

  ASSERT(-1 * 0xfff7 == (sizeof(int) == 2 ? 9 : 0xffff0009)); // 0xfff7 is stored in 'unsigned int'
  // but:
  ASSERT(-1 * 65527  == -65527); // 65527 (== 0xfff7) is stored in 'signed LONG'
  ASSERT(-1 * 33000  == -33000);

  ASSERT(1 *  10000  * is8 == (sizeof(int) == 2 ? 14464  :  80000)); /* int	 */
  ASSERT(1 *  10000l * is8 == 80000);				     /* LONG     */
  ASSERT(1 *  40000u * is8 == (sizeof(int) == 2 ? 57856u : 320000)); /* unsigned */
  ASSERT(1 *  40000  * is8 == 320000);				     /* LONG	 */
  ASSERT(1 * 0x4000  * is8 == (sizeof(int) == 2 ? 0 : 0x20000));     /* unsigned */

  ASSERT(-1 * 1  < 0);
  ASSERT(-1 * 1u > 0);


  /* mul icode */
  s = -3;
  ASSERT(s * (stype) -1 == (stype)  3);
  ASSERT(s * (stype)  1 == (stype) -3);
  s =  3;
  ASSERT(s * (stype) -1 == (stype) -3);

  s = 1;
  ASSERT(s * (utype) 0xfffffff7 == (utype) 0xfffffff7);
  uc = (unsigned char ) 0xfffffff8;
  ASSERT(uc * (unsigned char ) 0xfffffff7 == 0xef48);
  us = (unsigned short) 0xfffffff8;
  ASSERT(us * (unsigned short) 0xfffffff7 == (sizeof(int) == 2 ? 0x0048 : 0xffef0048));
  ul = (unsigned LONG ) 0xfffffff8;
  ASSERT(ul * (unsigned LONG ) 0xfffffff7 == 0x0048);
  ul = (unsigned LONG ) 0xfffffff8;

  ASSERT((stype         ) 0xfffffff8 * (stype         ) 0xfffffff7 == 72);

  ASSERT((signed char ) -1 * (unsigned char ) 0xfffffff7 == (sizeof(int) == 2 ? 0xff09 : 0xffffff09));
  ASSERT((signed short) -1 * (unsigned short) 0xfffffff7 == (sizeof(int) == 2 ?     9u : 0xffff0009));
  ASSERT((signed LONG ) -1 * (unsigned LONG ) 0xfffffff7 == 9u);

  ASSERT((signed char ) -2 * (unsigned char ) 0x8004 == (sizeof(int) == 2 ? 0xfff8 : 0xfffffff8));
  ASSERT((signed short) -2 * (unsigned short) 0x8004 == (sizeof(int) == 2 ? 0xfff8 : 0xfffefff8));
  ASSERT((signed LONG ) -2 * (unsigned LONG ) 0x8004 == 0xfffefff8);



  ASSERT((stype) -12 / (stype) -3 == (stype)  4);
  ASSERT((stype) -12 / (stype)  3 == (stype) -4);
  ASSERT((stype)  12 / (stype) -3 == (stype) -4);

//  ASSERT((stype) -12 / (utype) -3 == (stype)  4);
//  ASSERT((utype) -12 / (stype) -3 == (stype)  4);
//  ASSERT((utype) -12 / (utype) -3 == (stype)  4);


  ASSERT(12u / 3 * 10000 == 40000);

  ASSERT(-1 / 1 < 0);



  ASSERT((stype) -14 % (stype) -3 == (stype) -2);
  ASSERT((stype) -14 % (stype)  3 == (stype) -2);
  ASSERT((stype)  14 % (stype) -3 == (stype)  2);

  ASSERT(-3 % 2 < 0);
}
