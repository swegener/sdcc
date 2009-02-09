/* asranlib.c - ranlib for asxxxx arvhives
   version 1.0.0, April 27th, 2008

   Copyright (C) 2008-2009 Borut Razem, borut dot razem at siol dot net

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "dbuf_string.h"
#include "lkar.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#define NELEM(x)  (sizeof (x) / sizeof (*x))


int verbose = 0;
int list = 0;
int print_index = 0;

int
is_ar (FILE * libfp)
{
  char buf[SARMAG];
  int ret;

  if (!(ret = fread (buf, 1, sizeof (buf), libfp) == sizeof (buf) && memcmp (buf, ARMAG, SARMAG) == 0))
    rewind (libfp);

  return ret;
}

static char *sym_tab;
static int sym_tab_size;

char *
get_member_name (char *name, size_t *p_size, int allocate, FILE * libfp)
{
  *p_size = 0;

  if (0 == memcmp (name, "#1/", 3))
    {
      char *p;
      size_t len = strtoul (&name [3], &p, 10);
      if (p > &name [3])
        {
          /* BSD appends real file name to the file header */
          if (p_size != NULL)
            *p_size = len;

          if (allocate)
            {
              char *n = (char *) malloc (len);
              if (fread (n, 1, len, libfp) != len)
                {
                  /* not an ar archive or broken ar archive */
                  return NULL;
                }
              else
                return n;
            }
          else
            {
              /* just advance the file pointer */
              fseek (libfp, len, SEEK_CUR);
              return NULL;
            }
        }
      else
        {
          /* not an ar archive or broken ar archive */
          return NULL;
        }
    }
  else if (allocate)
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
          else
            {
              /* BSD formed member name:
                 trim trailing spaces */
              char *n;

              p = name + AR_NAME_LEN;
              while (*--p == ' ' && p >= name)
                ;
              ++p;
              n = (char *) malloc (p - name + 1);
              memcpy (n, name, p - name);
              n[p - name] = '\0';
              return n;
            }
        }

      /* bad formed member name:
       just return it */

      return strdup (name);
    }
  else
    return NULL;
}

size_t
ar_get_header (struct ar_hdr *hdr, FILE * libfp, char **p_obj_name)
{
  char header[ARHDR_LEN];
  char buf[AR_DATE_LEN + 1];
  char *obj_name;
  size_t size;

  if (fread (header, 1, sizeof (header), libfp) != sizeof (header)
      || memcmp (header + AR_FMAG_OFFSET, ARFMAG, AR_FMAG_LEN) != 0)
    {
      /* not an ar archive */
      return 0;
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

  obj_name = get_member_name (hdr->ar_name, &size, p_obj_name != NULL, libfp);

  if (p_obj_name != NULL)
    *p_obj_name = obj_name;

  /* treat BSD appended real file name as a part of the header */
  hdr->ar_size -= size;

  return size + ARHDR_LEN;
}

static char *
get_member_name_by_offset (FILE * fp, long offset)
{
  struct ar_hdr hdr;
  char *name;

  fseek (fp, offset, SEEK_SET);
  return (ar_get_header (&hdr, fp, &name) != 0) ? name : NULL;
}

struct symbol_s
  {
    const char *name;
    size_t offset;
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

  if (verbose)
    printf ("%s\n", sym);

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


static int
process_symbol_table (struct ar_hdr *hdr, FILE *fp)
{
  long pos = ftell (fp);

  if (print_index)
    {
      char *buf, *po, *ps;
      int i;
      long nsym;

      printf ("Archive index:\n");

      buf = (char *) malloc (hdr->ar_size);

      if (fread (buf, 1, hdr->ar_size, fp) != hdr->ar_size)
        {
          free (buf);
          return 0;
        }

      nsym = sgetl (buf);

      po = buf + 4;
      ps = po + nsym * 4;

      for (i = 0; i < nsym; ++i)
        {
          char *obj;

          offset = sgetl (po);
          po += 4;

          obj = get_member_name_by_offset (fp, offset);  /* member name */
          printf ("%s in %s", ps, obj);
          if (verbose)
            printf (" at 0x%04x\n", offset);
          else
            putchar ('\n');
          free (obj);

          ps += strlen(ps) + 1;
        
        }
      free (buf);

      fseek (fp, pos, SEEK_SET);

      putchar ('\n');
    }

  /* skip the symbol table */
  fseek (fp, pos + hdr->ar_size + (hdr->ar_size & 1), SEEK_SET);

  return 1;
}

static int
process_bsd_symbol_table (struct ar_hdr *hdr, FILE *fp)
{
  long pos = ftell (fp);

  if (print_index)
    {
      char *buf, *po, *ps;
      int i;
      long tablesize;
      long nsym;

      printf ("Archive index:\n");

      buf = (char *) malloc (hdr->ar_size);

      if (fread (buf, 1, hdr->ar_size, fp) != hdr->ar_size)
        {
          free (buf);
          return 0;
        }

      tablesize = sgetl (buf);
      nsym = tablesize / 8;

      po = buf + 4;

      ps = po + tablesize + 4;

      for (i = 0; i < nsym; ++i)
        {
          char *obj;
          long sym;

          sym = sgetl (po);
          po += 4;
          offset = sgetl (po);
          po += 4;

          printf ("%s in ", ps + sym);

          obj = get_member_name_by_offset (fp, offset);  /* member name */
          printf ("%s\n", obj);
          free (obj);
        }
      free (buf);

      fseek (fp, pos, SEEK_SET);

      putchar ('\n');
    }

  /* skip the symbol table */
  fseek (fp, pos + hdr->ar_size + (hdr->ar_size & 1), SEEK_SET);

  return 1;
}

int
get_symbols (FILE * fp, const char *archive)
{
  struct ar_hdr hdr;
  size_t hdr_len;
  char *name;

  if (!is_ar (fp) || !(hdr_len = ar_get_header (&hdr, fp, &name)))
    {
      fprintf (stderr, "asranlib: %s: File format not recognized\n", archive);
      exit (1);
    }

  if (AR_IS_SYMBOL_TABLE (name))
    {
      free (name);

      process_symbol_table (&hdr, fp);
      if (!(hdr_len = ar_get_header (&hdr, fp, (verbose || list) ? &name : NULL)))
        return 1;
    }
  else if (AR_IS_BSD_SYMBOL_TABLE (name))
    {
      free (name);

      process_bsd_symbol_table (&hdr, fp);
      if (!(hdr_len = ar_get_header (&hdr, fp, (verbose || list) ? &name : NULL)))
        return 1;
    }
  else if (!verbose && !list)
    free (name);

  first_member_offset = ftell (fp) - hdr_len;

  /* walk trough all archive members */
  do
    {
      if (is_rel (fp))
        {
          if (verbose || list)
            {
              printf ("%s%s\n", name, verbose ? ":" : "");
              free (name);
            }

          if (!list)
            {
              long mdule_offset = ftell (fp);

              offset = mdule_offset - hdr_len;

              enum_symbols (fp, hdr.ar_size, add_symbol, NULL);

              fseek (fp, mdule_offset + hdr.ar_size + (hdr.ar_size & 1), SEEK_SET);
            }

          if (verbose)
            putchar ('\n');
        }
      else
        {
          if (verbose || list)
            {
              fprintf (stderr, "asranlib: %s: File format not recognized\n", name);
              free (name);
            }

          /* skip if the member is not a .REL format */
          fseek (fp, hdr.ar_size + (hdr.ar_size & 1), SEEK_CUR);
        }
    }
  while ((hdr_len = ar_get_header (&hdr, fp, (verbose || list) ? &name : NULL)));

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
      FILE *outfp;
      struct symbol_s *symp;
      char buf[4];
      int str_length;
      int pad;
      int nsym;
      int symtab_size;
      char tmpfile[] = "arXXXXXX";

#ifdef _WIN32
      if (NULL == _mktemp (tmpfile) || NULL == (outfp = fopen (tmpfile, "wb")))
        {
          fclose (infp);
          fprintf (stderr, "asranlib: %s: ", tmpfile);
          perror (NULL);
          exit (1);
        }
#else
      if ((pad = mkstemp (tmpfile)) < 0)
        {
          fclose (infp);
          fprintf (stderr, "asranlib: %s: ", tmpfile);
          perror (NULL);
          exit (1);
        }

      if (NULL == (outfp = fdopen (pad, "wb")))
        {
          close (pad);
          fclose (infp);
          perror ("asranlib");
          exit (1);
        }
#endif

      /* calculate the size of symbol table */
      for (str_length = 0, nsym = 0, symp = symlist; symp; ++nsym, symp = symp->next)
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
do_verbose (void)
{
  verbose = 1;
}

void
print_version (void)
{
  printf ("SDCC asxxxx ranlib 1.0.0 $Revision$\n");
  exit (0);
}

void
do_list (void)
{
  list = 1;
}

void
print_armap (void)
{
  print_index = 1;
}

void usage (void);

struct opt_s
  {
    const char *short_opt;
    const char *long_opt;
    void (*optfnc) (void);
    const char *comment;
  }
opts[] =
  {
    { "-v", "--verbose", &do_verbose, "Be more verbose about the operation" },
    { "-V", "--version", &print_version, "Print this help message" },
    { "-h", "--help", &usage, "Print version information" },
    { "-t", "--list", &do_list, "List the contents of an archive" },
    { "-s", "--print-armap", &print_armap, "Print the archive index" },
  };

void
usage (void)
{
  int i;

  printf ("Usage: asranlib [options] archive\n"
    " Generate an index to speed access to archives\n"
    " The options are:\n");

  for (i = 0; i < NELEM (opts); ++i)
    {
      int len = 3;
      printf ("  %s ", (NULL != opts[i].short_opt) ? opts[i].short_opt : "  ");
      if (NULL != opts[i].long_opt)
        {
          printf ("%s ", opts[i].long_opt);
          len += strlen (opts[i].long_opt);
        }

      while (len++ < 32)
        putchar (' ');
      printf ("%s\n", opts[i].comment);
    }

  printf ("asranlib: supported targets: asxxxx\n");

  exit (1);
}

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
              if (0 == strcmp (*argp, opts[i].short_opt) || 0 == strcmp (*argp, opts[i].long_opt))
                {
                  if (NULL != opts[i].optfnc)
                    {
                      (*opts[i].optfnc) ();
                      break;
                    }
                }
            }
        }

      else
        {
          do_ranlib (*argp);
          ++narch;
        }
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
