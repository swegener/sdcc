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

/*
** $Id$
*/

/* (c)2000/2001: hacked a little by johan.knol@iduna.nl for sdcc */

#include <float.h>

union float_long
  {
    float f;
    long l;
  };

/* divide two floats */
float __fsdiv (float a1, float a2) _FS_REENTRANT
{
  volatile union float_long fl1, fl2;
  volatile long result;
  volatile unsigned long mask;
  volatile long mant1, mant2;
  volatile int exp ;
  char sign;

  fl1.f = a1;
  fl2.f = a2;

  /* subtract exponents */
  exp = EXP (fl1.l) ;
  exp -= EXP (fl2.l);
  exp += EXCESS;

  /* compute sign */
  sign = SIGN (fl1.l) ^ SIGN (fl2.l);

  /* divide by zero??? */
  if (!fl2.l)
    /* return NaN or -NaN */
    return (-1.0);

  /* numerator zero??? */
  if (!fl1.l)
    return (0);

  /* now get mantissas */
  mant1 = MANT (fl1.l);
  mant2 = MANT (fl2.l);

  /* this assures we have 25 bits of precision in the end */
  if (mant1 < mant2)
    {
      mant1 <<= 1;
      exp--;
    }

  /* now we perform repeated subtraction of fl2.l from fl1.l */
  mask = 0x1000000;
  result = 0;
  while (mask)
    {
      if (mant1 >= mant2)
	{
	  result |= mask;
	  mant1 -= mant2;
	}
      mant1 <<= 1;
      mask >>= 1;
    }

  /* round */
  result += 1;

  /* normalize down */
  exp++;
  result >>= 1;

  result &= ~HIDDEN;

  /* pack up and go home */
  fl1.l = PACK (sign ? 1ul<<31 : 0, (unsigned long) exp, result);
  return (fl1.f);
}

