/*  math.h: Floating point math function declarations

    Copyright (C) 2001  Jesus Calvino-Fraga, jesusc@ieee.org 

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA */

/* Version 1.0 - Initial release */

#ifndef _INC_MATH
#define _INC_MATH

#define PI          3.1415926536
#define TWO_PI      6.2831853071
#define HALF_PI     1.5707963268
#define QUART_PI    0.7853981634
#define iPI         0.3183098862
#define iTWO_PI     0.1591549431
#define TWO_O_PI    0.6366197724

// EPS=B**(-t/2), where B is the radix of the floating-point representation
// and there are t base-B digits in the significand.  Therefore, for floats
// EPS=2**(-12).  Also define EPS2=EPS*EPS.
#define EPS 244.14062E-6
#define EPS2 59.6046E-9
#define XMAX 3.402823466E+38

union float_long
{
    float f;
    long l;
};

/* Functions on the z80 & gbz80 are always reentrant and so the "reentrant" */
/* keyword is not defined. */
#if defined(SDCC_z80) || defined(SDCC_gbz80)
#define _FLOAT_FUNC_REENTRANT
#else
#define _FLOAT_FUNC_REENTRANT reentrant
#endif

/**********************************************
 * Prototypes for float ANSI C math functions *
 **********************************************/

/* Trigonometric functions */
float sinf(const float x) _FLOAT_FUNC_REENTRANT;
float cosf(const float x) _FLOAT_FUNC_REENTRANT;
float tanf(const float x) _FLOAT_FUNC_REENTRANT;
float cotf(const float x) _FLOAT_FUNC_REENTRANT;
float asinf(const float x) _FLOAT_FUNC_REENTRANT;
float acosf(const float x) _FLOAT_FUNC_REENTRANT;
float atanf(const float x) _FLOAT_FUNC_REENTRANT;
float atan2f(const float x, const float y);

/* Hyperbolic functions */
float sinhf(const float x) _FLOAT_FUNC_REENTRANT;
float coshf(const float x) _FLOAT_FUNC_REENTRANT;
float tanhf(const float x) _FLOAT_FUNC_REENTRANT;

/* Exponential, logarithmic and power functions */
float expf(const float x);
float logf(const float x) _FLOAT_FUNC_REENTRANT;
float log10f(const float x) _FLOAT_FUNC_REENTRANT;
float powf(const float x, const float y);
float sqrtf(const float a) _FLOAT_FUNC_REENTRANT;

/* Nearest integer, absolute value, and remainder functions */
float fabsf(const float x) _FLOAT_FUNC_REENTRANT;
float frexpf(const float x, int *pw2);
float ldexpf(const float x, const int pw2);
float ceilf(float x) _FLOAT_FUNC_REENTRANT;
float floorf(float x) _FLOAT_FUNC_REENTRANT;
float modff(float x, float * y);

#endif  /* _INC_MATH */
