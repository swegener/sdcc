/* m08ext.c */

/*
 * (C) Copyright 1993-2002
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */

#include <stdio.h>
#include <setjmp.h>
#include "asm.h"
#include "m6808.h"

char	*cpu	= "Motorola 68HC08";
int	hilo	= 1;
char	*dsft	= "asm";
