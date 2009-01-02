/* lklibr.h */

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

/*
 * Extensions: P. Felber
 */

#ifndef __LKLIBR_H
#define __LKLIBR_H

#include <stdio.h>

typedef struct slibrarysymbol mlibrarysymbol;
typedef struct slibrarysymbol *pmlibrarysymbol;

struct slibrarysymbol
{
  char *name;                   /*Warning: allocate memory before using */
  pmlibrarysymbol next;
};

typedef struct slibraryfile mlibraryfile;
typedef struct slibraryfile *pmlibraryfile;

struct slibraryfile
{
  int loaded;
  char *libspc;
  char *relfil;                 /* Warning: allocate memory before using */
  char *filename;               /* Warning: allocate memory before using */
  long offset;                  /* The embedded file offset in the library file libspc */
  unsigned int type;
  pmlibrarysymbol symbols;
  pmlibraryfile next;
};

extern pmlibraryfile libr;

#ifdef INDEXLIB
pmlibrarysymbol add_rel_index (FILE * fp, long size, pmlibraryfile This);
#else
int add_rel_file (const char *name, struct lbname *lbnh, const char *relfil,
                  const char *filspc, int offset, FILE * fp, long size, int type);
#endif

struct aslib_target
{
  int (*is_lib) (FILE * libfp);
#ifdef INDEXLIB
    pmlibraryfile (*buildlibraryindex) (struct lbname * lbnh, FILE * libfp, pmlibraryfile This, int type);
#else
  int (*fndsym) (const char *name, struct lbname * lbnh, FILE * libfp, int type);
#endif
  void (*loadfile) (struct lbfile * lbfh);
};

extern struct aslib_target aslib_target_sdcclib;
extern struct aslib_target aslib_target_lib;

#endif /* __LKLIBR_H */
