/* lkrel.c */

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

#include <string.h>
#include <assert.h>

#include "getline.h"
#include "aslink.h"
#include "lkrel.h"

int
is_rel (FILE * libfp)
{
  int c;
  long pos = ftell (libfp);
  int ret = 0;

  /* [XDQ][HL] */
  if (((c = getc (libfp)) == 'X' || c == 'D' || c == 'Q') && ((c = getc (libfp)) == 'H' || c == 'L'))
    {
      switch (getc (libfp))
	{
	case '\r':
	  if (getc (libfp) == '\n')
	    ret = 1;
	  break;

	case '\n':
	  ret = 1;
	}
    }
  else if (c == ';')
    {
      char buf[6];

      if (fread (buf, 1, sizeof (buf), libfp) == sizeof (buf) && memcmp (buf, "!FILE ", 6) == 0)
	ret = 1;
    }
  fseek (libfp, pos, SEEK_SET);
  return ret;
}

/* Load a standalone or embedded .rel */
int
load_rel (FILE * libfp, long size)
{
  if (is_rel (libfp))
    {
      char str[NINPUT];
      long end;

      end = (size >= 0) ? ftell (libfp) + size : -1;

      while ((end < 0 || ftell (libfp) < end) && getline (str, sizeof (str), libfp) != NULL)
	{
	  if (0 == strcmp (str, "</REL>"))
	    return 1;

	  ip = str;
	  link_main ();
	}

      return 1;
    }
  else
    return 0;
}

int
enum_symbols (FILE * fp, long size, int (*func) (const char *symvoid, void *param), void *param)
{
  char buf[NINPUT];
  long end = (size >= 0) ? ftell (fp) + size : -1;

  assert (func != NULL);

  /*
   * Read in the object file.  Look for lines that
   * begin with "S" and end with "D".  These are
   * symbol table definitions.  If we find one, see
   * if it is our symbol.  Make sure we only read in
   * our object file and don't go into the next one.
   */

  while ((end <= 0 || ftell (fp) < end) && getline (buf, sizeof (buf), fp) != NULL)
    {
      char symname[NINPUT];
      char c;

      /*
       * When a 'T line' is found terminate file scan.
       * All 'S line's preceed 'T line's in .REL files.
       */
      if (buf[0] == 'T')
	break;

      /*
       * Skip everything that's not a symbol record.
       */
      if (buf[0] != 'S')
	continue;

      sscanf (buf, "S %s %c", symname, &c);

      /* If it's an actual symbol, record it */
      if (c == 'D')
	{
	  if ((*func) (symname, param))
	    return 1;
	}
    }

  return 0;
}
