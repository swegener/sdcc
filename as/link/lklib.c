/* lklib.c */

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

#include "getline.h"
#include "aslink.h"
#include "lklibr.h"
#include "lkrel.h"

static int
is_lib (FILE * libfp)
{
  return 1;
}

#ifdef INDEXLIB
/* buildlibraryindex - build an in-memory cache of the symbols contained in
 *                     the libraries
 */
static pmlibraryfile
buildlibraryindex_lib (struct lbname *lbnh, FILE * libfp, pmlibraryfile This, int type)
{
  char relfil[NINPUT];

  while (getline (relfil, sizeof (relfil), libfp) != NULL)
    {
      FILE *fp;
      char str[PATH_MAX];

      if (lbnh->path != NULL)
        {
          strcpy (str, lbnh->path);
#ifdef  OTHERSYSTEM
          if ((*str != '\0') && (str[strlen (str) - 1] != '/') && (str[strlen (str) - 1] != LKDIRSEP))
            {
              strcat (str, LKDIRSEPSTR);
            }
#endif
        }
      else
        str[0] = '\0';

      if ((relfil[0] == '/') || (relfil[0] == LKDIRSEP))
        {
          strcat (str, relfil + 1);
        }
      else
        {
          strcat (str, relfil);
        }

      if (strchr (relfil, FSEPX) == NULL)
        {
          sprintf (&str[strlen (str)], "%c%s", FSEPX, LKOBJEXT);
        }

      if ((fp = fopen (str, "rb")) != NULL)
        {
          /* Opened OK - create a new libraryfile object for it */
          if (libr == NULL)
            {
              libr = This = (pmlibraryfile) new (sizeof (mlibraryfile));
            }
          else
            {
              This->next = (pmlibraryfile) new (sizeof (mlibraryfile));
              This = This->next;
            }
          This->next = NULL;
          This->loaded = -1;
          This->libspc = lbnh->libspc;
          This->relfil = strdup (relfil);
          This->filename = strdup (str);
          This->type = type;

          /* Start a new linked list of symbols for this module: */
          This->symbols = NULL;

          add_rel_index (fp, -1, This);
          fclose (fp);
        }                       /* Closes if object file opened OK */
      else
        {
          fprintf (stderr, "?ASlink-Warning-Cannot open library module %s\n", str);
        }
    }                           /* Ends while - processing all in libr */

  return This;
}

#else

static int
fndsym_lib (const char *name, struct lbname *lbnh, FILE * libfp, int type)
{
  char relfil[NINPUT];

  while (getline (relfil, sizeof (relfil), libfp) != NULL)
    {
      char str[PATH_MAX];
      FILE *fp;

      if (lbnh->path != NULL)
        {
          strcpy (str, lbnh->path);
#ifdef  OTHERSYSTEM
          if ((*str != '\0') && (str[strlen (str) - 1] != '/') && (str[strlen (str) - 1] != LKDIRSEP))
            {
              strcat (str, LKDIRSEPSTR);
            }
#endif
        }
      else
        str[0] = '\0';

      if ((relfil[0] == '/') || (relfil[0] == LKDIRSEP))
        {
          strcat (str, relfil + 1);
        }
      else
        {
          strcat (str, relfil);
        }

      if (strchr (relfil, FSEPX) == NULL)
        {
          sprintf (&str[strlen (str)], "%c%s", FSEPX, LKOBJEXT);
        }

      if ((fp = fopen (str, "rb")) != NULL)
        {
          /* Opened OK - create a new libraryfile object for it */
          int ret = add_rel_file (name, lbnh, relfil, str, -1, fp, -1, type);
          fclose (fp);
          if (ret)
            {
              /* if cdb information required & adb file present */
              if (dflag && dfp)
                {
                  FILE *xfp = afile (str, "adb", 0);    //JCF: Nov 30, 2002
                  if (xfp)
                    {
                      SaveLinkedFilePath (str);
                      copyfile (dfp, xfp);
                      fclose (xfp);
                    }
                }
              return 1;         /* Found the symbol, so success! */
            }
        }                       /* Closes if object file opened OK */
      else
        {
          fprintf (stderr, "?ASlink-Warning-Cannot open library module %s\n", str);
        }
    }                           /* Ends while - processing all in libr */

  return 0;                     /* The symbol is not in this library */
}
#endif

/*)Function VOID    loadfile_lib(filspc)
 *
 *      char    *filspc     library object file specification
 *
 *  The function loadfile() links the library object module.
 *
 *  local variables:
 *      FILE    *fp         file handle
 *      int     i           input line length
 *      char    str[]       file input line
 *
 *  global variables:
 *      char    *ip         pointer to linker input string
 *
 *   functions called:
 *      int     fclose()    c_library
 *      char    *getline()  getline.c
 *      FILE *  fopen()     c_library
 *      VOID    link_main() lkmain.c
 *      int     strlen()    c_library
 *
 *  side effects:
 *      If file exists it is linked.
 */

static VOID
loadfile_lib (struct lbfile *lbfh)
{
  FILE *fp;
#ifdef __CYGWIN__
  char posix_path[PATH_MAX];
  void cygwin_conv_to_full_posix_path (char *win_path, char *posix_path);
  cygwin_conv_to_full_posix_path (lbfh->filspc, posix_path);
  fp = fopen (posix_path, "rb");
#else
  fp = fopen (lbfh->filspc, "rb");
#endif

  if (fp != NULL)
    {
      load_rel (fp, -1);
      fclose (fp);
    }
  else
    {
      fprintf (stderr, "?ASlink-Error-Opening library '%s'\n", lbfh->filspc);
      fclose (fp);
      lkexit (1);
    }
}

struct aslib_target aslib_target_lib = {
  &is_lib,
#ifdef INDEXLIB
  &buildlibraryindex_lib,
#else
  &fndsym_lib,
#endif
  &loadfile_lib,
};
