/* lksdcclib.c */

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

#include <stdlib.h>
#include <string.h>

#include "getline.h"
#include "aslink.h"
#include "lklibr.h"
#include "lkrel.h"

#define EQ(A,B) !strcmp((A),(B))
#define MAXLINE 254             /*when using getline */


static int
is_sdcclib (FILE * libfp)
{
#define SDCCLIB_MAGIC "<SDCCLIB>"
#define SDCCLIB_MAGIC_LEN (sizeof ("<SDCCLIB>") - 1)

  char buf[SDCCLIB_MAGIC_LEN];

  if (fread (buf, 1, sizeof (buf), libfp) == sizeof (buf) && memcmp (buf, SDCCLIB_MAGIC, SDCCLIB_MAGIC_LEN) == 0)
    {
      switch (getc (libfp))
        {
        case '\r':
          if (getc (libfp) == '\n')
            return 1;

        case '\n':
          return 1;
        }
    }
  rewind (libfp);
  return 0;
}

/* Load a .rel file embedded in a sdcclib file */
static int
LoadRel (char *libfname, FILE * libfp, char *ModName)
{
  char str[NINPUT];
  int state = 0;

  while (getline (str, sizeof (str), libfp) != NULL)
    {
      switch (state)
        {
        case 0:
          if (EQ (str, "<FILE>"))
            {
              if (NULL != getline (str, sizeof (str), libfp) && EQ (str, ModName))
                state = 1;
              else
                return 0;
            }
          else
            return 0;
          break;

        case 1:
          return EQ (str, "<REL>") ? load_rel (libfp, -1) : 0;
        }
    }

  return 0;
}

#ifdef INDEXLIB
static pmlibraryfile
buildlibraryindex_sdcclib (struct lbname *lbnh, FILE * libfp, pmlibraryfile This, int type)
{
  char ModName[NCPS] = "";
  char FLine[MAXLINE];
  char buff[PATH_MAX];
  int state = 0;
  long IndexOffset = 0, FileOffset;
  pmlibrarysymbol ThisSym = NULL;

  while (getline (FLine, sizeof (FLine), libfp))
    {
      switch (state)
        {
        case 0:
          if (EQ (FLine, "<INDEX>"))
            {
              /*The next line has the size of the index */
              getline (FLine, sizeof (FLine), libfp);
              IndexOffset = atol (FLine);
              state = 1;
            }
          break;

        case 1:
          if (EQ (FLine, "<MODULE>"))
            {
              /* The next line has the name of the module and the offset
                 of the corresponding embedded file in the library */
              getline (FLine, sizeof (FLine), libfp);
              sscanf (FLine, "%s %ld", ModName, &FileOffset);
              state = 2;

              /* Create a new libraryfile object for this module */
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
              This->offset = FileOffset + IndexOffset;
              This->libspc = lbnh->libspc;
              This->relfil = strdup (ModName);
              sprintf (buff, "%s%s%c%s", lbnh->path, ModName, FSEPX, LKOBJEXT);
              This->filename = strdup (buff);
              This->type = type;

              This->symbols = ThisSym = NULL;   /* Start a new linked list of symbols */
            }
          else if (EQ (FLine, "</INDEX>"))
            {
              return This;      /* Finish, get out of here */
            }
          break;

        case 2:
          if (EQ (FLine, "</MODULE>"))
            {
              This->loaded = 0;
              /* Create the index for the next module */
              state = 1;
            }
          else
            {
              /* Add the symbols */
              if (ThisSym == NULL)      /* First symbol of the current module */
                {
                  ThisSym = This->symbols = (pmlibrarysymbol) new (sizeof (mlibrarysymbol));
                }
              else
                {
                  ThisSym->next = (pmlibrarysymbol) new (sizeof (mlibrarysymbol));
                  ThisSym = ThisSym->next;
                }
              ThisSym->next = NULL;
              ThisSym->name = strdup (FLine);
            }
          break;

        default:
          return This;          /* State machine should never reach this point, but just in case... */
          break;
        }
    }

  return This;                  /* State machine should never reach this point, but just in case... */
}

#else

/* Load an .adb file embedded in a sdcclib file. If there is
something between <ADB> and </ADB> returns 1, otherwise returns 0.
This way the aomf51 will not have useless empty modules. */

static int
LoadAdb (FILE * libfp)
{
  char str[MAXLINE];
  int state = 0;
  int ret = 0;

  while (getline (str, sizeof (str), libfp) != NULL)
    {
      switch (state)
        {
        case 0:
          if (EQ (str, "<ADB>"))
            state = 1;
          break;

        case 1:
          if (EQ (str, "</ADB>"))
            return ret;
          fprintf (dfp, "%s\n", str);
          ret = 1;
          break;
        }
    }
  return ret;
}

/* Check for a symbol in a SDCC library. If found, add the embedded .rel and
   .adb files from the library.  The library must be created with the SDCC
   librarian 'sdcclib' since the linking process depends on the correct file offsets
   embedded in the library file. */

static int
findsym_sdcclib (const char *name, struct lbname *lbnh, FILE * libfp, int type)
{
  struct lbfile *lbfh, *lbf;
  char ModName[NCPS] = "";
  char FLine[MAXLINE];
  int state = 0;
  long IndexOffset = 0, FileOffset;

  while (getline (FLine, sizeof (FLine), libfp))
    {
      char str[PATH_MAX];

      if (lbnh->path != NULL)
        {
          strcpy (str, lbnh->path);
#ifdef  OTHERSYSTEM
          if (*str != '\0' && (str[strlen (str) - 1] != '/') && (str[strlen (str) - 1] != LKDIRSEP))
            {
              strcat (str, LKDIRSEPSTR);
            }
#endif
        }

      switch (state)
        {
        case 0:
          if (EQ (FLine, "<INDEX>"))
            {
              /* The next line has the size of the index */
              getline (FLine, sizeof (FLine), libfp);
              IndexOffset = atol (FLine);
              state = 1;
            }
          break;

        case 1:
          if (EQ (FLine, "<MODULE>"))
            {
              /* The next line has the name of the module and the offset
                 of the corresponding embedded file in the library */
              getline (FLine, sizeof (FLine), libfp);
              sscanf (FLine, "%s %ld", ModName, &FileOffset);
              state = 2;
            }
          else if (EQ (FLine, "</INDEX>"))
            {
              /* Reached the end of the index.  The symbol is not in this library. */
              return 0;
            }
          break;

        case 2:
          if (EQ (FLine, "</MODULE>"))
            {
              /* The symbol is not in this module, try the next one */
              state = 1;
            }
          else
            {
              /* Check if this is the symbol we are looking for. */
              if (strncmp (name, FLine, NCPS) == 0)
                {
                  /* The symbol is in this module. */

                  /* As in the original library format, it is assumed that the .rel
                     files reside in the same directory as the lib files. */
                  sprintf (&str[strlen (str)], "%s%c%s", ModName, FSEPX, LKOBJEXT);

                  /* If this module has been loaded already don't load it again. */
                  lbf = lbfhead;
                  while (lbf)
                    {
                      if (EQ (str, lbf->filspc))
                        return 1;       /* Already loaded */
                      lbf = lbf->next;
                    }

                  /* Add the embedded file to the list of files to be loaded in
                     the second pass.  That is performed latter by the function
                     library() below. */
                  lbfh = (struct lbfile *) new (sizeof (struct lbfile));
                  if (lbfhead == NULL)
                    {
                      lbfhead = lbfh;
                    }
                  else
                    {
                      lbf = lbfhead;
                      while (lbf->next)
                        {
                          lbf = lbf->next;
                        }
                      lbf->next = lbfh;
                    }

                  lbfh->libspc = lbnh->libspc;
                  lbfh->filspc = strdup (str);
                  lbfh->relfil = strdup (ModName);
                  /* Library embedded file, so lbfh->offset must be >=0 */
                  lbfh->offset = IndexOffset + FileOffset;

                  /* Jump to where the .rel begins and load it. */
                  fseek (libfp, lbfh->offset, SEEK_SET);
                  if (!LoadRel (lbnh->libspc, libfp, ModName))
                    {
                      fclose (libfp);
                      fprintf (stderr, "?ASlink-Error-Bad offset in library file %s(%s)\n", lbfh->libspc, ModName);
                      lkexit (1);
                    }
                  /* if cdb information required & .adb file present */
                  if (dflag && dfp)
                    {
                      if (LoadAdb (libfp))
                        SaveLinkedFilePath (str);
                    }
                  return 1;     /* Found the symbol, so success! */
                }
            }
          break;

        default:
          return 0;             /* It should never reach this point, but just in case... */
          break;
        }
    }

  return 0;                     /* The symbol is not in this library */
}

#endif

static void
loadfile_sdcclib (struct lbfile *lbfh)
{
  FILE *fp;
  int res;

#ifdef __CYGWIN__
  char posix_path[PATH_MAX];
  void cygwin_conv_to_full_posix_path (char *win_path, char *posix_path);
  cygwin_conv_to_full_posix_path (lbfh->libspc, posix_path);
  fp = fopen (posix_path, "rb");
#else
  fp = fopen (lbfh->libspc, "rb");
#endif

  if (fp != NULL)
    {
      fseek (fp, lbfh->offset, SEEK_SET);
      res = LoadRel (lbfh->libspc, fp, lbfh->relfil);
      fclose (fp);

      if (!res)
        {
          fprintf (stderr, "?ASlink-Error-Bad offset in library file %s(%s)\n", lbfh->libspc, lbfh->relfil);
          lkexit (1);
        }
    }
  else
    {
      fprintf (stderr, "?ASlink-Error-Opening library '%s'\n", lbfh->libspc);
      lkexit (1);
    }
}

struct aslib_target aslib_target_sdcclib = {
  &is_sdcclib,
#ifdef INDEXLIB
  &buildlibraryindex_sdcclib,
#else
  &findsym_sdcclib,
#endif
  &loadfile_sdcclib,
};
