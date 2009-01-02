/* lkrel.h */

/*
 * (C) Copyright 1989-1995
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 *
 * With contributions for the
 * object libraries from
 * Ken Hornstein
 * kenh@cmf.nrl.navy.mil
 *
 */

#ifndef __LKREL_H
#define __LKREL_H

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

  int is_rel (FILE * libfp);
  int load_rel (FILE * libfp, long size);
  int enum_symbols (FILE * fp, long size, int (*func) (const char *symvoid, void *param), void *param);


#ifdef __cplusplus
}
#endif

#endif				/* __LKREL_H */
