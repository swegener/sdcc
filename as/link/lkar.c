/* lkar.c - ar library format handling

   Copyright (C) 1989-1995 Alan R. Baldwin
   721 Berkeley St., Kent, Ohio 44240
   Copyright (C) 2008-2009 Borut Razem, borut dot razem at siol dot net

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

/*
 * With contributions for the
 * object libraries from
 * Ken Hornstein
 * kenh@cmf.nrl.navy.mil
 *
 */

/*
 * Extensions: P. Felber
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "aslink.h"
#include "lklibr.h"
#include "lkrel.h"
#include "lkar.h"


#ifndef max
# define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
# define min(a,b) ((a) < (b) ? (a) : (b))
#endif


static int
is_ar (FILE * libfp)
{
  char buf[SARMAG];
  int ret;

  if (!(ret = fread (buf, 1, sizeof (buf), libfp) == sizeof (buf) && memcmp (buf, ARMAG, SARMAG) == 0))
    rewind (libfp);

  return ret;
}

static struct ar_hdr *
ar_get_header (struct ar_hdr *hdr, FILE * libfp)
{
  char header[ARHDR_LEN];
  char buf[AR_DATE_LEN + 1];

  if (fread (header, 1, sizeof (header), libfp) != sizeof (header)
      || memcmp (header + AR_FMAG_OFFSET, ARFMAG, AR_FMAG_LEN) != 0)
    {
      /* not an ar archive */
      return NULL;
    }

  memcpy (hdr->ar_name, &header[AR_NAME_OFFSET], AR_NAME_LEN);
  hdr->ar_name[AR_NAME_LEN] = '\0';

  memcpy (buf, &header[AR_DATE_OFFSET], AR_DATE_LEN);
  buf[AR_DATE_LEN] = '\0';
  hdr->ar_date = strtol (buf, NULL, 0);

  memcpy (buf, &header[AR_UID_OFFSET], AR_GID_LEN);
  buf[AR_GID_LEN] = '\0';
  hdr->ar_uid = (uid_t) strtol (buf, NULL, 0);

  memcpy (buf, &header[AR_GID_OFFSET], AR_DATE_LEN);
  buf[AR_DATE_LEN] = '\0';
  hdr->ar_gid = (gid_t) strtol (buf, NULL, 0);

  memcpy (buf, &header[AR_MODE_OFFSET], AR_MODE_LEN);
  buf[AR_MODE_LEN] = '\0';
  hdr->ar_mode = (mode_t) strtoul (buf, NULL, 0);

  memcpy (buf, &header[AR_SIZE_OFFSET], AR_SIZE_LEN);
  buf[AR_SIZE_LEN] = '\0';
  hdr->ar_size = strtol (buf, NULL, 0);

  return hdr;
}

static char *sym_tab;
static int sym_tab_size;

static void
loadfile_ar (struct lbfile *lbfh)
{
  FILE *fp;

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
      struct ar_hdr hdr;

      fseek (fp, lbfh->offset, SEEK_SET);
      if (ar_get_header (&hdr, fp))
        {
          D ("Loading module %s from file %s.\n", hdr.ar_name, lbfh->libspc);
          load_rel (fp, hdr.ar_size);
          fclose (fp);
        }
      else
        {
          fprintf (stderr, "?ASlink-Error-Bad offset in library file %s(%s)\n", lbfh->libspc, lbfh->relfil);
          fclose (fp);
          lkexit (1);
        }
    }
  else
    {
      fprintf (stderr, "?ASlink-Error-Opening library '%s'\n", lbfh->libspc);
      lkexit (1);
    }
}

#if INDEXLIB
char *
get_member_name (char *name)
{
  if (name[0] == '/')
    {
      if (NULL != sym_tab)
        {
          char *p;

          int name_offset = strtol (++name, &p, 0);
          if (p != name && name_offset < sym_tab_size)
            {
              int len = p - name + 1;
              while (len < AR_NAME_LEN && name[len++] == ' ')
                ;
              if (len == AR_NAME_LEN)
                {
                  char *n;

                  /* long name: get it from the symbol table */
                  name = &sym_tab[name_offset];
                  for (p = name; *p != '/' && *p != '\n'; ++p)
                    assert (p < &sym_tab[sym_tab_size]);

                  if (p[0] != '/' || p[1] != '\n')
                    while (*++p != '\n')
                      assert (p < &sym_tab[sym_tab_size]);

                  n = (char *) malloc (p - name + 1);
                  memcpy (n, name, p - name);
                  n[p - name] = '\0';
                  return n;
                }
            }
        }
    }
  else
    {
      char *p = strrchr (name, '/');

      if (NULL != p)
        {
          int len = p - name;
          while (name[++len] == ' ')
            ;
          if (len == AR_NAME_LEN)
            {
              char *n = (char *) malloc (p - name + 1);
              memcpy (n, name, p - name);
              n[p - name] = '\0';
              return n;
            }
        }
    }

  /* bad formed member name:
     just return it */

  return strdup (name);
}

static char *
get_member_name_by_offset (FILE * fp, long offset)
{
  struct ar_hdr hdr;

  fseek (fp, offset, SEEK_SET);

  /* walk trough all archive members */
  return (NULL != ar_get_header (&hdr, fp)) ? get_member_name (hdr.ar_name) : NULL;
}

static pmlibraryfile
find_member_by_offset (const char *libspc, long offset)
{
  pmlibraryfile p;

  for (p = libr; p; p = p->next)
    {
      if (0 == strcmp (libspc, p->libspc) && p->offset == offset)
        return p;
    }

  return NULL;
}

static pmlibraryfile
buildlibraryindex_ar (struct lbname *lbnh, FILE * libfp, pmlibraryfile This, int type)
{
  struct ar_hdr hdr;

  /* walk trough all archive members */
  while (ar_get_header (&hdr, libfp))
    {
      if (AR_IS_SYMBOL_TABLE (hdr))
        {
          char *buf, *po, *ps;
          int i;
          long nsym;

          buf = (char *) new (hdr.ar_size);

          if ((off_t) fread (buf, 1, hdr.ar_size, libfp) != hdr.ar_size)
            {
              free (buf);
              return This;
            }

          nsym = sgetl (buf);

          po = buf + 4;
          ps = po + nsym * 4;

          for (i = 0; i < nsym; ++i)
            {
              pmlibrarysymbol ThisSym;
              char *sym;
              long offset;
              pmlibraryfile entry;

              offset = sgetl (po);
              po += 4;

              sym = strdup (ps);
              while (*ps++ != '\0')
                ;

              if ((entry = find_member_by_offset (lbnh->libspc, offset)) != NULL)
                {
                  for (ThisSym = entry->symbols; ThisSym->next != NULL; ThisSym = ThisSym->next)
                    ;
                }
              else
                {
                  /* Opened OK - create a new libraryfile object for it */
                  if (This == NULL)
                    {
                      assert (libr == NULL);
                      libr = This = (pmlibraryfile) new (sizeof (mlibraryfile));
                    }
                  else
                    {
                      This->next = (pmlibraryfile) new (sizeof (mlibraryfile));
                      This = This->next;
                    }
                  This->next = NULL;
                  This->loaded = 0;
                  This->libspc = lbnh->libspc;
                  This->offset = offset;
                  This->relfil = get_member_name_by_offset (libfp, offset);     /* member name */
                  This->filspc = strdup (This->relfil); /* member file name */
                  This->type = type;

                  /* start a new linked list of symbols for this module. */
                  This->symbols = ThisSym = NULL;
                }

              if (ThisSym == NULL)
                ThisSym = This->symbols = (pmlibrarysymbol) new (sizeof (mlibrarysymbol));
              else
                {
                  ThisSym->next = (pmlibrarysymbol) new (sizeof (mlibrarysymbol));
                  ThisSym = ThisSym->next;
                }
              ThisSym->next = NULL;
              ThisSym->name = sym;
            }
          free (buf);

          break;
        }
      else if (AR_IS_STRING_TABLE (hdr))
        {
          if (sym_tab)
            free (sym_tab);

          sym_tab = (char *) new (hdr.ar_size);

          if ((off_t) fread (sym_tab, 1, hdr.ar_size, libfp) != hdr.ar_size)
            {
              free (sym_tab);
              sym_tab_size = 0;
              return This;
            }
          sym_tab_size = hdr.ar_size;
        }
      else
        {
          long moduleOffset = ftell (libfp);

          /* Opened OK - create a new libraryfile object for it */
          if (This == NULL)
            {
              assert (libr == NULL);
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
          This->offset = moduleOffset - ARHDR_LEN;

          This->relfil = get_member_name (hdr.ar_name); /* member name */
          This->filspc = strdup (This->relfil); /* member file name */

          D ("  Indexing module: %s\n", This->relfil);

          This->type = type;

          /* start a new linked list of symbols for this module. */
          This->symbols = NULL;

          add_rel_index (libfp, hdr.ar_size, This);

          fseek (libfp, moduleOffset + hdr.ar_size, SEEK_SET);
        }

      if (hdr.ar_size & 1)
        {
          int c = getc (libfp);
          assert (c == EOF || c == '\n');
        }
    }

  if (NULL != sym_tab)
    {
      free (sym_tab);
      sym_tab = NULL;
      sym_tab_size = 0;
    }

  return This;
}

#else

#if 0
static int
load_adb (FILE * libfp, struct lbfile *lbfh)
{
  struct ar_hdr hdr;
  char *adb_name;

  /* check if it is a .rel file */
  if (0 != stricmp (&lbfh->relfil[strlen (lbfh->relfil) - 4], ".rel"))
    return 0;


  adb_name = (char *) new (strlen (lbfh->relfil) + 1);
  memcpy (adb_name, lbfh->relfil, strlen (lbfh->relfil) - 4);
  memcpy (&adb_name[strlen (lbfh->relfil) - 4], ".adb", 5);

  if (!is_ar (libfp))
    {
      fprintf (stderr, "?ASlink-Error-%s is not an archive\n", lbfh->libspc);
      fclose (libfp);
      lkexit (1);
    }


  /* walk trough all archive members */
  while (ar_get_header (&hdr, libfp))
    {
      if (AR_IS_STRING_TABLE (hdr))
        {
          if (sym_tab)
            free (sym_tab);

          sym_tab = (char *) new (hdr.ar_size);

          if ((off_t) fread (sym_tab, 1, hdr.ar_size, libfp) != hdr.ar_size)
            {
              free (sym_tab);
              sym_tab_size = 0;
              return 0;
            }
          sym_tab_size = hdr.ar_size;
        }
      if (AR_IS_SYMBOL_TABLE (hdr) || 0 != stricmp (get_member_name (hdr.ar_name), adb_name))
        {
          /* skip the mamber */
          fseek (libfp, hdr.ar_size + (hdr.ar_size & 1), SEEK_CUR);
        }
      else
        {
          long left = hdr.ar_size;
          char buf[4096];

          while (left)
            {
              size_t n = min (left, sizeof buf);

              if (fread (buf, 1, n, libfp) != n)
                {
                  assert (0);
                }

              fwrite (buf, 1, n, dfp);

              left -= n;
            }

          if (hdr.ar_size & 1)
            {
              int c = getc (libfp);
              assert (c == EOF || c == '\n');
            }

          free (adb_name);
          return 1;
        }
    }

  free (adb_name);
  return 0;
}
#endif

static int
fndsym_ar (const char *name, struct lbname *lbnh, FILE * libfp, int type)
{
  struct ar_hdr hdr;
  int ret = 0;

  /* walk trough all archive members */
  while (ar_get_header (&hdr, libfp))
    {
      char filspc[PATH_MAX];

      if (lbnh->path != NULL)
        {
          strcpy (filspc, lbnh->path);
#ifdef  OTHERSYSTEM
          if (*filspc != '\0' && (filspc[strlen (filspc) - 1] != '/') && (filspc[strlen (filspc) - 1] != LKDIRSEP))
            {
              strcat (filspc, LKDIRSEPSTR);
            }
#endif
        }

      if (AR_IS_SYMBOL_TABLE (hdr))
        {
          char *buf, *po, *ps;
          int i;
          long nsym;

          buf = (char *) new (hdr.ar_size);

          if ((off_t) fread (buf, 1, hdr.ar_size, libfp) != hdr.ar_size)
            {
              free (buf);
              return 0;
            }

          nsym = sgetl (buf);

          po = buf + 4;
          ps = po + nsym * 4;

          for (i = 0; i < nsym; ++i)
            {
              char *sym;
              long offset;

              offset = sgetl (po);
              po += 4;

              sym = ps;
              while (*ps++ != '\0')
                ;

              if (0 == strcmp (name, sym))
                {
                  fseek (libfp, offset, SEEK_SET);
                  if (ar_get_header (&hdr, libfp))
                    {
                      sprintf (&filspc[strlen (filspc)], "%s", hdr.ar_name);

                      /* If this module has been loaded already don't load it again. */
                      if (!is_module_loaded (filspc))
                        {
                          struct lbfile *lbfh, *lbf;

                          lbfh = (struct lbfile *) new (sizeof (struct lbfile));
                          lbfh->libspc = strdup (lbnh->libspc);
                          lbfh->relfil = strdup (hdr.ar_name);
                          lbfh->filspc = strdup (filspc);
                          lbfh->offset = offset;
                          lbfh->type = type;

                          if (lbfhead == NULL)
                            {
                              lbfhead = lbfh;
                            }
                          else
                            {
                              for (lbf = lbfhead; lbf->next != NULL; lbf = lbf->next)
                                ;

                              lbf->next = lbfh;
                            }

                          D ("Loading module %s from file %s.\n", hdr.ar_name, lbfh->libspc);
                          load_rel (libfp, hdr.ar_size);
                          ///* if cdb information required & .adb file present */
                          //if (dflag && dfp)
                          //  {
                          //    if (load_adb(FILE *libfp, struct lbfile *lbfh))
                          //      SaveLinkedFilePath (filspc);
                          //  }
                          ret = 1;
                          break;
                        }
                    }
                  else
                    {
                      fprintf (stderr, "?ASlink-Error-Bad offset in library file %s(%s)\n", lbnh->libspc, name);
                      fclose (libfp);
                      lkexit (1);
                    }
                }
            }
          free (buf);

          break;
        }
      else if (AR_IS_STRING_TABLE (hdr))
        {
          if (sym_tab)
            free (sym_tab);

          sym_tab = (char *) new (hdr.ar_size);

          if ((off_t) fread (sym_tab, 1, hdr.ar_size, libfp) != hdr.ar_size)
            {
              free (sym_tab);
              sym_tab = NULL;
              sym_tab_size = 0;
              return 0;
            }
          sym_tab_size = hdr.ar_size;
        }
      else
        {
          long moduleOffset = ftell (libfp);

          D ("  Module: %s\n", hdr.ar_name);

          sprintf (&filspc[strlen (filspc)], "%s", hdr.ar_name);

          /* Opened OK - create a new libraryfile object for it */
          ret = add_rel_file (name, lbnh, hdr.ar_name, filspc, moduleOffset - ARHDR_LEN, libfp, hdr.ar_size, type);
          ///* if cdb information required & .adb file present */
          //if (dflag && dfp)
          //  {
          //    if (load_adb(FILE *libfp, struct lbfile *lbfh))
          //      SaveLinkedFilePath (filspc);
          //  }
          if (ret)
            break;

          fseek (libfp, moduleOffset + hdr.ar_size, SEEK_SET);
        }

      if (hdr.ar_size & 1)
        {
          int c = getc (libfp);
          assert (c == EOF || c == '\n');
        }
    }

  if (NULL != sym_tab)
    {
      free (sym_tab);
      sym_tab = NULL;
      sym_tab_size = 0;
    }

  return ret;
}
#endif

struct aslib_target aslib_target_ar = {
  &is_ar,
#ifdef INDEXLIB
  &buildlibraryindex_ar,
#else
  &fndsym_ar,
#endif
  &loadfile_ar,
};
