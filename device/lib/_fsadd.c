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
    unsigned long l;
  };

/* add two floats */
float __fsadd (float a1, float a2)
{
  volatile long mant1, mant2;
  volatile union float_long fl1, fl2;
  volatile int exp1, exp2;
  volatile unsigned long sign = 0;

  fl1.f = a1;
  fl2.f = a2;

  /* check for zero args */
  if (!fl1.l)
    return (fl2.f);
  if (!fl2.l)
    return (fl1.f);

  exp1 = EXP (fl1.l);
  exp2 = EXP (fl2.l);

  if (exp1 > exp2 + 25)
    return (fl1.f);
  if (exp2 > exp1 + 25)
    return (fl2.f);

  mant1 = MANT (fl1.l);
  mant2 = MANT (fl2.l);

  if (SIGN (fl1.l))
    mant1 = -mant1;
  if (SIGN (fl2.l))
    mant2 = -mant2;

  if (exp1 > exp2)
    {
      mant2 >>= exp1 - exp2;
    }
  else
    {
      mant1 >>= exp2 - exp1;
      exp1 = exp2;
    }
  mant1 += mant2;

  if (mant1 < 0)
    {
      mant1 = -mant1;
      sign = SIGNBIT;
    }
  else if (!mant1)
    return (0);

  /* normalize */
  /* jwk: TODO: changing the next two whiles in nested ifs 
     seriously breaks it. Why?????????????????? */ 
  while (mant1<HIDDEN) {
    mant1 <<= 1;
    exp1--;
  }

  while (mant1 & 0xff000000) {
    // round off
    if (mant1&1)
      mant1 += 2;
    mant1 >>= 1 ;
    exp1++;
  }

  /* turn off hidden bit */
  mant1 &= ~HIDDEN;

  /* pack up and go home */
  fl1.l = PACK (sign, (unsigned long) exp1, mant1);

  return (fl1.f);
}
