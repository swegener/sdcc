/*
** libgcc support for software floating point.
** Copyright (C) 1991 by Pipeline Associates, Inc.  All rights reserved.
** Permission is granted to do *anything* you want with this file,
** commercial or otherwise, provided this message remains intact.  So there!
** I would appreciate receiving any updates/patches/changes that anyone
** makes, and am willing to be the repository for said changes (am I
** making a big mistake?).
**
** Pat Wood
** Pipeline Associates, Inc.
** pipeline!phw@motown.com or
** sun!pipeline!phw or
** uunet!motown!pipeline!phw
*/

/* (c)2000/2001: hacked a little by johan.knol@iduna.nl for sdcc */

#include <float.h>

union float_long
  {
    float f;
    long l;
  };

float __ulong2fs (unsigned long a )
{
  int exp = 24 + EXCESS;
  volatile union float_long fl;

  if (!a)
    {
      return 0.0;
    }

  while (a & NORM) 
    {
      // we lose accuracy here
      a >>= 1;
      exp++;
    }
  
  while (a < HIDDEN)
    {
      a <<= 1;
      exp--;
    }

#if 1
  if ((a&0x7fffff)==0x7fffff) {
    a=0;
    exp++;
  }
#endif

  a &= ~HIDDEN ;
  /* pack up and go home */
  fl.l = PACK(0,(unsigned long)exp, a);

  return (fl.f);
}
