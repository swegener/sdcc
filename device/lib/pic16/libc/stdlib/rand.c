/*-
 * rand.c - random number generation routines
 *
 * this source was part of the avr-lib library
 * modified for SDCC/pic16 by Vangelis Rokas, 2005 <vrokas AT users.sourceforge.net>
 *
 *
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Posix rand_r function added May 1999 by Wes Peters <wes@softweyr.com>.
 */

#include <stdlib.h>

static long do_rand(unsigned long *ctx)
{
  return ((*ctx = *ctx * 1103515245UL + 12345UL) % ((unsigned long)RAND_MAX + 1));
}

long rand_r(unsigned long *ctx)
{
  unsigned long val = (unsigned long) *ctx;
  
    *ctx = do_rand(&val);
    return (long) *ctx;
}

static unsigned long next = 1;

long rand(void)
{
  return do_rand(&next);
}

void srand(unsigned long seed)
{
  next = seed;
}

#ifdef TEST

main()
{
    int i;
    unsigned long myseed;

    stdout = STREAM_GPSIM;

    printf("seeding rand with 0x19610910: \n");
    srand(0x19610910);

    printf("generating three pseudo-random numbers:\n");
    for (i = 0; i < 10; i++)
    {
	printf("next random number = %ld\n", rand());
    }

    printf("generating the same sequence with rand_r:\n");
    myseed = 0x19610910;
    for (i = 0; i < 10; i++)
    {
	printf("next random number = %ld\n", rand_r(&myseed));
    }

    return 0;
}

#endif /* TEST */

