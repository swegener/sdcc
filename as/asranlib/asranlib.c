/* asranlib.c - ranlib for asxxxx arvhives
   version 1.0.0, April 27th, 2008

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "dbuf_string.h"
#include "lkar.h"

#ifdef _WIN32
#include <io.h>
#define mktemp  _mktemp
#endif

#define NELEM(x)  (sizeof (x) / sizeof (*x))


int
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

  if (fread (header, 1, sizeof (header), libfp) != sizeof (header) ||
      memcmp (header + AR_FMAG_OFFSET, ARFMAG, AR_FMAG_LEN) != 0)
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


struct symbol_s
  {
    const char *name;
    off_t offset;
    struct symbol_s *next;
  };

struct symbol_s *symlist, *lastsym;
unsigned int offset, first_member_offset;

int
add_symbol (const char *sym, void *param)
{
  struct symbol_s *s;

  if ((s = (struct symbol_s *) malloc (sizeof (struct symbol_s))) == NULL)
    return 0;

  s->name = strdup (sym);
  s->offset = offset - first_member_offset;
  s->next = NULL;

  if (NULL == symlist)
    {
      lastsym = symlist = s;
    }
  else
    {
      lastsym->next = s;
      lastsym = s;
    }

  return 0;
}


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

int
enum_symbols (FILE * fp, long size, int (*func) (const char *sym, void *param), void *param)
{
  long end;
  struct dbuf_s buf;
  struct dbuf_s symname;

  assert (func != NULL);

  dbuf_init (&buf, 512);
  dbuf_init (&symname, 32);

  end = (size >= 0) ? ftell (fp) + size : -1;

  /*
   * Read in the object file.  Look for lines that
   * begin with "S" and end with "D".  These are
   * symbol table definitions.  If we find one, see
   * if it is our symbol.  Make sure we only read in
   * our object file and don't go into the next one.
   */

  while (end < 0 || ftell (fp) < end)
    {
      const char *p;

      dbuf_set_length (&buf, 0);
      if (dbuf_getline (&buf, fp) == 0)
        break;

      p = dbuf_c_str (&buf);

      if ('T' == p[0])
        break;

      /*
       * Skip everything that's not a symbol record.
       */
      if ('S' == p[0] && ' ' == p[1])
        {
          dbuf_set_length (&symname, 0);

          for (p += 2; *p && ' ' != *p; ++p)
            dbuf_append_char (&symname, *p);

          /* If it's an actual symbol, record it */
          if (' ' == p[0] && 'D' == p[1])
            if (func != NULL)
              if ((*func) (dbuf_c_str (&symname), NULL))
                return 1;
        }
    }

  dbuf_destroy (&buf);
  dbuf_destroy (&symname);

  return 0;
}

int
get_symbols (FILE * fp, const char *archive)
{
  struct ar_hdr hdr;

  if (!is_ar (fp) || !ar_get_header (&hdr, fp))
    {
      fprintf (stderr, "asranlib: %s: File format not recognized\n", archive);
      exit (1);
    }

  if (AR_IS_SYMBOL_TABLE (hdr))
    {
      /* skip the symbol table */
      fseek (fp, hdr.ar_size + (hdr.ar_size & 1), SEEK_CUR);
    }

  first_member_offset = ftell (fp) - ARHDR_LEN;

  /* walk trough all archive members */
  do
    {
      if (is_rel (fp))
        {
          long mdule_offset = ftell (fp);

          offset = mdule_offset - ARHDR_LEN;

          enum_symbols (fp, hdr.ar_size, add_symbol, NULL);

          fseek (fp, mdule_offset + hdr.ar_size, SEEK_SET);

          if (hdr.ar_size & 1)
            {
              int c = getc (fp);
              assert (c == EOF || c == '\n');
            }
        }
      else
        {
          /* skip if the member is not a .REL format */
          fseek (fp, hdr.ar_size + (hdr.ar_size & 1), SEEK_CUR);
        }
    }
  while (ar_get_header (&hdr, fp));

  return 1;
}


void
do_ranlib (const char *archive)
{
  FILE *infp;

  if (NULL == (infp = fopen (archive, "rb")))
    {
      fprintf (stderr, "asranlib: %s: ", archive);
      perror (NULL);
      exit (1);
    }

  if (get_symbols (infp, archive))
    {
      FILE *outfp = NULL;
      struct symbol_s *symp;
      char buf[4];
      int str_length = 0;
      int pad = 0;
      int nsym;
      int symtab_size;
      char tmpfile[] = "arXXXXXX";

      if (NULL == mktemp (tmpfile) || NULL == (outfp = fopen (tmpfile, "wb")))
        {
          fclose (infp);
          fprintf (stderr, "asranlib: %s: ", tmpfile);
          perror (NULL);
          exit (1);
        }

      /* calculate the size of symbol table */
      for (nsym = 0, symp = symlist; symp; ++nsym, symp = symp->next)
        {
          str_length += strlen (symp->name) + 1;
        }

      symtab_size = 4 + 4 * nsym + str_length;

      fprintf (outfp, ARMAG AR_SYMBOL_TABLE_NAME "%-12d%-6d%-6d%-8d%-10d" ARFMAG, (int) time (NULL), 0, 0, 0, symtab_size);

      if (symtab_size & 1)
        {
          pad = 1;
          ++symtab_size;
        }
      else
        pad = 0;

      symtab_size += SARMAG + ARHDR_LEN;

      sputl (nsym, buf);
      fwrite (buf, 1, sizeof (buf), outfp);

      for (symp = symlist; symp; symp = symp->next)
        {
          sputl (symp->offset + symtab_size, buf);
          fwrite (buf, 1, sizeof (buf), outfp);
        }


      for (symp = symlist; symp; symp = symp->next)
        {
          fputs (symp->name, outfp);
          putc ('\0', outfp);
        }

      if (pad)
        putc ('\n', outfp);

      fseek (infp, first_member_offset, SEEK_SET);

      while (EOF != (pad = getc (infp)))
        putc (pad, outfp);

      fclose (outfp);
      fclose (infp);

      if (0 != remove (archive))
        {
          fprintf (stderr, "asranlib: can't remove %s to %s: ", tmpfile, archive);
          perror (NULL);
        }
      else if (0 != rename (tmpfile, archive))
        {
          fprintf (stderr, "asranlib: can't rename %s to %s: ", tmpfile, archive);
          perror (NULL);
        }
    }
  else
    fclose (infp);
}


void
print_version (void)
{
  printf ("SDCC asxxxx ranlib 1.0.0 $Revision$\n");
  exit (0);
}


void
usage (void)
{
  printf ("Usage: asranlib [options] archive\n"
    " Generate an index to speed access to archives\n"
    " The options are:\n"
    "  -h --help                    Print this help message\n"
    "  -V --version                 Print version information\n"
    "asranlib: supported targets: asxxxx\n");

  exit (1);
}


struct opt_s
  {
    const char *opt;
    void (*optfnc) (void);
  }
opts[] =
  {
    { "-v", &print_version, },
    { "-V", &print_version, },
    { "--version", &print_version, },
    { "-h", &usage, },
    { "--help", &usage, },
  };


void
process_options (int argc, char *argv[])
{
  char **argp;
  int noopts = 0;
  int narch = 0;
  for (argp = argv + 1; *argp; ++argp)
    {
      if (!noopts && (*argp)[0] == '-')
        {
          int i;

          if ((*argp)[1] == '-' && (*argp)[2] == '\0')
            {
              noopts = 1;
              continue;
            }

          for (i = 0; i < NELEM (opts); ++i)
            {
              if (0 == strcmp (*argp, opts[i].opt))
                {
                  if (NULL != opts[i].optfnc)
                    {
                      (*opts[i].optfnc) ();
                      continue;
                    }
                }
            }
        }

      do_ranlib (*argp);
      ++narch;
    }

  if (!narch)
    usage ();
}


int
main (int argc, char *argv[])
{
  process_options (argc, argv);

  return 0;
}
