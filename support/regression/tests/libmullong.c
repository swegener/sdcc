/* Test _mullong.c from library

    type: asm, c
 */
#include <testfwk.h>

#define type_{type}

#if defined(PORT_HOST)
#  if defined(type_c)
#    define _SDCC_NO_ASM_LIB_FUNCS 1
#    define near
#    define long int
#    include "../../../../../device/lib/_mullong.c"
#  else
#    define _mullong(a,b) (a*b)
#  endif
#else
#  if defined(type_c)
#    define _SDCC_NO_ASM_LIB_FUNCS 1
#  endif
#  include "../../../../../device/lib/_mullong.c"
#endif

void
testlibmullong(void)
{
  ASSERT(_mullong (         0,          0) ==          0);
  ASSERT(_mullong (     0x100,      0x100) ==    0x10000);
  ASSERT(_mullong (0x01020304,          3) == 0x0306090c);
  ASSERT(_mullong (         3, 0x01020304) == 0x0306090c);
  ASSERT(_mullong (0x000000ff,          2) == 0x000001fe);
  ASSERT(_mullong (         2, 0x000000ff) == 0x000001fe);
  ASSERT(_mullong (0x00007fff,          4) == 0x0001fffc);
  ASSERT(_mullong (         4, 0x00007fff) == 0x0001fffc);
  ASSERT(_mullong (0x003fffff,          8) == 0x01fffff8);
  ASSERT(_mullong (         8, 0x003fffff) == 0x01fffff8);

  ASSERT(_mullong (      0x33,       0x34) == 0x00000a5c);
  ASSERT(_mullong (      0x34,       0x33) == 0x00000a5c);
  ASSERT(_mullong (    0x3334,     0x3536) == 0x0aa490f8);
  ASSERT(_mullong (    0x3536,     0x3334) == 0x0aa490f8);
  ASSERT(_mullong (  0x333435,   0x363738) == 0x0e98ce98);
  ASSERT(_mullong (  0x363738,   0x333435) == 0x0e98ce98);
  ASSERT(_mullong (0x33343536, 0x3738393a) == 0x777d143c);
  ASSERT(_mullong (0x3738393a, 0x33343536) == 0x777d143c);

  ASSERT(_mullong (      0xff,       0xfe) == 0x0000fd02);
  ASSERT(_mullong (      0xfe,       0xff) == 0x0000fd02);
  ASSERT(_mullong (    0xfffe,     0xfdfc) == 0xfdfa0408);
  ASSERT(_mullong (    0xfdfc,     0xfffe) == 0xfdfa0408);
  ASSERT(_mullong (  0xfffefd,   0xfcfbfa) == 0xfa0d1212);
  ASSERT(_mullong (  0xfcfbfa,   0xfffefd) == 0xfa0d1212);
  ASSERT(_mullong (0xfffefdfc, 0xfbfaf9f8) == 0x20282820);
  ASSERT(_mullong (0xfbfaf9f8, 0xfffefdfc) == 0x20282820);

  ASSERT(_mullong (0xff000000, 0xff000000) ==          0);
  ASSERT(_mullong (0xffff0000, 0xffff0000) ==          0);
  ASSERT(_mullong (0xfffffe00, 0xfffffd00) == 0x00060000);
  ASSERT(_mullong (0xfffffd00, 0xfffffe00) == 0x00060000);
  ASSERT(_mullong (0xfffffefd, 0xfffffcfb) == 0x00030e0f);
  ASSERT(_mullong (0xfffffcfb, 0xfffffefd) == 0x00030e0f);

  ASSERT(_mullong (0xffffffff, 0xffffffff) ==          1);
}
