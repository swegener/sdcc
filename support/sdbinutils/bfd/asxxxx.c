/* BFD back-end for asxxxx .rel objects.
   Copyright 2011
   Borut Razem (Free Software Foundation, Inc.)
   Written by Borut Razem <borut.razem@gmail.com>.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */


/* SUBSECTION
        The object module contains the following designators:  

                [XDQ][HL][234]
                        X       Hexidecimal radix
                        D       Decimal radix
                        Q       Octal radix
        
                        H       Most significant byte first
                        L       Least significant byte first
        
                        2       16-Bit Addressing
                        3       24-Bit Addressing
                        4       32-Bit Addressing
        
                H       Header 
                M       Module
                G       Merge Mode
                B       Bank
                A       Area
                S       Symbol
                T       Object code
                R       Relocation information
                P       Paging information

        3.5.1  Object Module Format
                [XDQ][HL][234]

        3.5.2  Header Line 
                H aa areas gg global symbols 

        3.5.3  Module Line 
                M name 

        3.5.4  Merge Mode Line 
                G nn ii 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 

        3.5.5  Bank Line 
                B name base nn size nn map nn flags nn fsfx string 

        3.5.6  Area Line 
                A label size ss flags ff 

        3.5.7  Symbol Line 
                S name Defnnnn 
                        or 
                S name Refnnnn 

        3.5.8  T Line 
                T xx xx nn nn nn nn nn ...  

        3.5.9  R Line 
                R 0 0 nn nn n1 n2 xx xx ...  

        3.5.10  P Line 
                P 0 0 nn nn n1 n2 xx xx 
 */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "libiberty.h"
#include "safe-ctype.h"


/* Macros for converting between hex and binary.  */

static const char digs[] = "0123456789ABCDEF";

#define NIBBLE(x)    hex_value(x)
#define HEX(buffer) ((NIBBLE ((buffer)[0])<<4) + NIBBLE ((buffer)[1]))
#define TOHEX(d, x, ch) \
        d[1] = digs[(x) & 0xf]; \
        d[0] = digs[((x)>>4)&0xf]; \
        ch += ((x) & 0xff);
#define ISHEX(x)    hex_p(x)

/* When writing an asxxxx .rel file, the asxxxx .rel records can not be output as
   they are seen.  This structure is used to hold them in memory.  */

struct asxxxx_data_list_struct
{
  struct asxxxx_data_list_struct *next;
  bfd_byte *data;
  bfd_vma where;
  bfd_size_type size;
};

typedef struct asxxxx_data_list_struct asxxxx_data_list_type;

/* When scanning the asxxxx .rel file, a linked list of asxxxx_symbol
   structures is built to represent the symbol table (if there is
   one).  */

struct asxxxx_symbol
{
  struct asxxxx_symbol *next;
  const char *name;
  bfd_vma val;
};

/* The asxxxx .rel tdata information.  */

typedef struct asxxxx_data_struct
  {
    asxxxx_data_list_type *head;
    asxxxx_data_list_type *tail;
    unsigned int type;
    struct asxxxx_symbol *symbols;
    struct asxxxx_symbol *symtail;
    asymbol *csymbols;
  }
tdata_type;

/* Initialize by filling in the hex conversion array.  */

static void
asxxxx_init (void)
{
  static bfd_boolean inited = FALSE;

  if (! inited)
    {
      inited = TRUE;
      hex_init ();
    }
}

/* Set up the asxxxx .rel tdata information.  */

static bfd_boolean
asxxxx_mkobject (bfd *abfd)
{
  tdata_type *tdata;

  asxxxx_init ();

  tdata = (tdata_type *) bfd_alloc (abfd, sizeof (tdata_type));
  if (tdata == NULL)
    return FALSE;

  abfd->tdata.asxxxx_data = tdata;
  tdata->type = 1;
  tdata->head = NULL;
  tdata->tail = NULL;
  tdata->symbols = NULL;
  tdata->symtail = NULL;
  tdata->csymbols = NULL;

  return TRUE;
}

/* Read a byte from an asxxxx .rel file.  Set *ERRORPTR if an error
   occurred.  Return EOF on error or end of file.  */

static int
asxxxx_get_byte (bfd *abfd, bfd_boolean *errorptr)
{
  bfd_byte c;

  if (bfd_bread (&c, (bfd_size_type) 1, abfd) != 1)
    {
      if (bfd_get_error () != bfd_error_file_truncated)
        *errorptr = TRUE;
      return EOF;
    }

  return (int) (c & 0xff);
}

/* Report a problem in an asxxxx .rel file.  FIXME: This probably should
   not call fprintf, but we really do need some mechanism for printing
   error messages.  */

static void
asxxxx_bad_byte (bfd *abfd,
               unsigned int lineno,
               int c,
               bfd_boolean error)
{
  if (c == EOF)
    {
      if (! error)
        bfd_set_error (bfd_error_file_truncated);
    }
  else
    {
      char buf[10];

      if (! ISPRINT (c))
        sprintf (buf, "\\%03o", (unsigned int) c);
      else
        {
          buf[0] = c;
          buf[1] = '\0';
        }
      (*_bfd_error_handler)
        (_("%B:%d: Unexpected character `%s' in asxxxx .rel file\n"),
         abfd, lineno, buf);
      bfd_set_error (bfd_error_bad_value);
    }
}

/* Add a new symbol found in an asxxxx .rel file.  */

static bfd_boolean
asxxxx_new_symbol (bfd *abfd, const char *name, bfd_vma val)
{
  struct asxxxx_symbol *n;

  n = (struct asxxxx_symbol *) bfd_alloc (abfd, sizeof (* n));
  if (n == NULL)
    return FALSE;

  n->name = name;
  n->val = val;

  if (abfd->tdata.asxxxx_data->symbols == NULL)
    abfd->tdata.asxxxx_data->symbols = n;
  else
    abfd->tdata.asxxxx_data->symtail->next = n;
  abfd->tdata.asxxxx_data->symtail = n;
  n->next = NULL;

  ++abfd->symcount;

  return TRUE;
}

/* Read the asxxxx .rel file and turn it into sections.  We create a new
   section for each contiguous set of bytes.  */

static bfd_boolean
asxxxx_scan (bfd *abfd, unsigned int *p_lineno)
{
  int c;
  bfd_boolean error = FALSE;
  char *symbuf = NULL;

  bfd_set_error (bfd_error_file_truncated);

  while ((c = asxxxx_get_byte (abfd, &error)) != EOF)
    {
      switch (c)
        {
        default:
          /* unknown line */
          asxxxx_bad_byte (abfd, *p_lineno, c, error);
          goto error_return;
          break;

        case '\n':
          ++p_lineno;
          break;

        case '\r':
          break;

        case 'H':
        case 'M':
        case 'G':
        case 'B':
        case 'A':
        case 'T':
        case 'R':
        case 'P':
        case 'O':
          /* check if the next character is space */
          if ((c = asxxxx_get_byte (abfd, &error)) != ' ')
            {
              asxxxx_bad_byte (abfd, *p_lineno, c, error);
              goto error_return;
            }

          /* eat the rest of line */
          while ((c = asxxxx_get_byte (abfd, &error)) != EOF
                 && (c != '\n' && c != '\r'))
            ;
          if (c == '\n')
            ++*p_lineno;
          else if (c != '\r')
            {
              asxxxx_bad_byte (abfd, *p_lineno, c, error);
              goto error_return;
            }
          break;

        case 'S':
          /* S __ret3 Def0001 */
          {
            bfd_size_type alc;
            char *p, *symname;
            bfd_vma symval;
            bfd_boolean is_def;

            /* Starting a symbol definition.  */
            while ((c = asxxxx_get_byte (abfd, &error))  == ' ' || c == '\t')
              ;

            if (c == EOF)
              {
                asxxxx_bad_byte (abfd, *p_lineno, c, error);
                goto error_return;
              }

            alc = 10;
            symbuf = (char *) bfd_malloc (alc + 1);
            if (symbuf == NULL)
              goto error_return;

            p = symbuf;

            *p++ = c;
            while (! ISSPACE (c = asxxxx_get_byte (abfd, &error)))
              {
                if ((bfd_size_type) (p - symbuf) >= alc)
                  {
                    char *n;

                    alc *= 2;
                    n = (char *) bfd_realloc (symbuf, alc + 1);
                    if (n == NULL)
                      goto error_return;
                    p = n + (p - symbuf);
                    symbuf = n;
                  }

                *p++ = c;
              }

            if (c == EOF)
              {
                asxxxx_bad_byte (abfd, *p_lineno, c, error);
                goto error_return;
              }

            *p++ = '\0';

            while ((c = asxxxx_get_byte (abfd, &error)) == ' ' || c == '\t')
              ;

            if (c != 'D' && c != 'R')
              {
                asxxxx_bad_byte (abfd, *p_lineno, c, error);
                goto error_return;
              }

            is_def = (c == 'D');

            if ((c = asxxxx_get_byte (abfd, &error)) != 'e' ||
                (c = asxxxx_get_byte (abfd, &error)) != 'f')
              {
                asxxxx_bad_byte (abfd, *p_lineno, c, error);
                goto error_return;
              }

            symval = 0;
            while (ISHEX (c = asxxxx_get_byte (abfd, &error)))
              {
                symval <<= 4;
                symval += NIBBLE (c);
                c = asxxxx_get_byte (abfd, &error);
              }

            if (c == '\n')
              ++*p_lineno;
            else if (c != '\r')
              {
                asxxxx_bad_byte (abfd, *p_lineno, c, error);
                goto error_return;
              }

            if (is_def && strncmp (symbuf, ".__.ABS.", 8))
              {
                symname = (char *) bfd_alloc (abfd, (bfd_size_type) (p - symbuf));
                if (symname == NULL)
                  goto error_return;
                strcpy (symname, symbuf);
                free (symbuf);
                symbuf = NULL;

                if (! asxxxx_new_symbol (abfd, symname, symval))
                  goto error_return;
              }
          }
          break;
        }
    }

  if (error)
    goto error_return;
  return TRUE;

error_return:
  if (symbuf != NULL)
    free (symbuf);
  return FALSE;
}

/* Check whether an existing file is an asxxxx .rel file.  */

static bfd_boolean
asxxxx_is_rel (bfd *abfd, unsigned int *p_lineno)
{
  int c;
  bfd_boolean error = FALSE;

  /* [XDQ][HL][234] */
  switch (c = asxxxx_get_byte (abfd, &error))
    {
    default:
      /* unknown line */
      asxxxx_bad_byte (abfd, *p_lineno, c, error);
      return FALSE;
      break;

    case ';':
      if ((c = asxxxx_get_byte (abfd, &error)) != '!' ||
          (c = asxxxx_get_byte (abfd, &error)) != 'F' ||
          (c = asxxxx_get_byte (abfd, &error)) != 'I' ||
          (c = asxxxx_get_byte (abfd, &error)) != 'L' ||
          (c = asxxxx_get_byte (abfd, &error)) != 'E' ||
          (c = asxxxx_get_byte (abfd, &error)) != ' ')
        {
          asxxxx_bad_byte (abfd, *p_lineno, c, error);
          return FALSE;
        }
      /* eat the rest of line */
      while ((c = asxxxx_get_byte (abfd, &error)) != '\n' && c != '\r')
        ;
      if (c == '\n')
        ++*p_lineno;
      else if (c != '\r')
        {
          asxxxx_bad_byte (abfd, *p_lineno, c, error);
          return FALSE;
        }
      break;

    case 'X':
    case 'D':
    case 'Q':
      switch (c = asxxxx_get_byte (abfd, &error))
        {
        default:
          asxxxx_bad_byte (abfd, *p_lineno, c, error);
          return FALSE;
          break;

        case 'H':
        case 'L':
          switch (c = asxxxx_get_byte (abfd, &error))
            {
            default:
              asxxxx_bad_byte (abfd, *p_lineno, c, error);
              return FALSE;
              break;

            case '\n':
              ++*p_lineno;
              break;

            case '2':
            case '3':
            case '4':
              if ((c = asxxxx_get_byte (abfd, &error)) == '\n')
                ++*p_lineno;
              else if (c != '\r')
                {
                  asxxxx_bad_byte (abfd, *p_lineno, c, error);
                  return FALSE;
                }
              break;

            case '\r':
              break;
           }
          break;
        }
    }

  return error ? FALSE : TRUE;
}

#if 0
{
  if (bfd_seek (abfd, (file_ptr) 0, SEEK_SET) == 0
      && bfd_bread (buf, (bfd_size_type) 4, abfd) == 4)
    {
      if ((buf[0] == 'X' || buf[0] == 'D' || buf[0] == 'Q') && (buf[1] == 'H' || buf[1] == 'L'))
        {
         bfd_boolean error = FALSE;

         switch (buf[2])
            {
            case '2':
            case '3':
            case '4':
              switch (buf[3])
                {
                case '\r':
                  if (asxxxx_get_byte(abfd, &error) == '\n')
                    ret = TRUE;
                  break;

                case '\n':
                  ret = TRUE;
                }
              break;

            case '\r':
              if (asxxxx_get_byte(abfd, &error) == '\n')
                ret = TRUE;
              break;

            case '\n':
              ret = TRUE;
            }
        }
      else if (buf[0] == ';' && buf[1] == '!' && buf[2] == 'F' && buf[3] == 'I')
        {
          if (bfd_bread (buf, (bfd_size_type) 3, abfd) == 3 && buf[0] == 'L' && buf[1] == 'E' && buf[2] == ' ')
            ret = TRUE;
        }
    }
  return ret;
}
#endif

static const bfd_target *
asxxxx_object_p (bfd *abfd)
{
  void * tdata_save;
  unsigned int lineno = 1;

  asxxxx_init ();

  if (! asxxxx_is_rel (abfd, &lineno))
    {
      bfd_set_error (bfd_error_wrong_format);
      return NULL;
    }

  tdata_save = abfd->tdata.any;
  if (! asxxxx_mkobject (abfd) || ! asxxxx_scan (abfd, &lineno))
    {
      if (abfd->tdata.any != tdata_save && abfd->tdata.any != NULL)
        bfd_release (abfd, abfd->tdata.any);
      abfd->tdata.any = tdata_save;
      return NULL;
    }

  if (abfd->symcount > 0)
    abfd->flags |= HAS_SYMS;

  return abfd->xvec;
}

/* Get the contents of a section.  */

static bfd_boolean
asxxxx_get_section_contents (bfd *abfd ATTRIBUTE_UNUSED,
                           asection *section ATTRIBUTE_UNUSED,
                           void * location ATTRIBUTE_UNUSED,
                           file_ptr offset ATTRIBUTE_UNUSED,
                           bfd_size_type count ATTRIBUTE_UNUSED)
{
  return FALSE;
}

/* Set the architecture.  We accept an unknown architecture here.  */

static bfd_boolean
asxxxx_set_arch_mach (bfd *abfd, enum bfd_architecture arch, unsigned long mach)
{
  if (arch != bfd_arch_unknown)
    return bfd_default_set_arch_mach (abfd, arch, mach);

  abfd->arch_info = & bfd_default_arch_struct;
  return TRUE;
}

/* Set the contents of a section.  */

static bfd_boolean
asxxxx_set_section_contents (bfd *abfd ATTRIBUTE_UNUSED,
                           sec_ptr section ATTRIBUTE_UNUSED,
                           const void * location ATTRIBUTE_UNUSED,
                           file_ptr offset ATTRIBUTE_UNUSED,
                           bfd_size_type bytes_to_do ATTRIBUTE_UNUSED)
{
  return FALSE;
}

static int
asxxxx_sizeof_headers (bfd *abfd ATTRIBUTE_UNUSED,
                     struct bfd_link_info *info ATTRIBUTE_UNUSED)
{
  return 0;
}

/* Return the amount of memory needed to read the symbol table.  */

static long
asxxxx_get_symtab_upper_bound (bfd *abfd)
{
  return (bfd_get_symcount (abfd) + 1) * sizeof (asymbol *);
}

/* Return the symbol table.  */

static long
asxxxx_canonicalize_symtab (bfd *abfd, asymbol **alocation)
{
  bfd_size_type symcount = bfd_get_symcount (abfd);
  asymbol *csymbols;
  unsigned int i;

  csymbols = abfd->tdata.asxxxx_data->csymbols;
  if (csymbols == NULL && symcount != 0)
    {
      asymbol *c;
      struct asxxxx_symbol *s;

      csymbols = (asymbol *) bfd_alloc (abfd, symcount * sizeof (asymbol));
      if (csymbols == NULL)
        return -1;
      abfd->tdata.asxxxx_data->csymbols = csymbols;

      for (s = abfd->tdata.asxxxx_data->symbols, c = csymbols;
           s != NULL;
           s = s->next, ++c)
        {
          c->the_bfd = abfd;
          c->name = s->name;
          c->value = s->val;
          c->flags = BSF_GLOBAL;
          c->section = bfd_abs_section_ptr;
          c->udata.p = NULL;
        }
    }

  for (i = 0; i < symcount; i++)
    *alocation++ = csymbols++;
  *alocation = NULL;

  return symcount;
}

static void
asxxxx_get_symbol_info (bfd *ignore_abfd ATTRIBUTE_UNUSED,
                      asymbol *symbol,
                      symbol_info *ret)
{
  bfd_symbol_info (symbol, ret);
}

static void
asxxxx_print_symbol (bfd *abfd,
                   void * afile,
                   asymbol *symbol,
                   bfd_print_symbol_type how)
{
  FILE *file = (FILE *) afile;

  switch (how)
    {
    case bfd_print_symbol_name:
      fprintf (file, "%s", symbol->name);
      break;
    default:
      bfd_print_symbol_vandf (abfd, (void *) file, symbol);
      fprintf (file, " %-5s %s",
               symbol->section->name,
               symbol->name);
    }
}

#define asxxxx_close_and_cleanup                    _bfd_generic_close_and_cleanup
#define asxxxx_bfd_free_cached_info                 _bfd_generic_bfd_free_cached_info
#define asxxxx_new_section_hook                     _bfd_generic_new_section_hook
#define asxxxx_bfd_is_target_special_symbol         ((bfd_boolean (*) (bfd *, asymbol *)) bfd_false)
#define asxxxx_bfd_is_local_label_name              bfd_generic_is_local_label_name
#define asxxxx_get_lineno                           _bfd_nosymbols_get_lineno
#define asxxxx_find_nearest_line                    _bfd_nosymbols_find_nearest_line
#define asxxxx_find_inliner_info                    _bfd_nosymbols_find_inliner_info
#define asxxxx_make_empty_symbol                    _bfd_generic_make_empty_symbol
#define asxxxx_bfd_make_debug_symbol                _bfd_nosymbols_bfd_make_debug_symbol
#define asxxxx_read_minisymbols                     _bfd_generic_read_minisymbols
#define asxxxx_minisymbol_to_symbol                 _bfd_generic_minisymbol_to_symbol
#define asxxxx_get_section_contents_in_window       _bfd_generic_get_section_contents_in_window
#define asxxxx_bfd_get_relocated_section_contents   bfd_generic_get_relocated_section_contents
#define asxxxx_bfd_relax_section                    bfd_generic_relax_section
#define asxxxx_bfd_gc_sections                      bfd_generic_gc_sections
#define asxxxx_bfd_lookup_section_flags             bfd_generic_lookup_section_flags
#define asxxxx_bfd_merge_sections                   bfd_generic_merge_sections
#define asxxxx_bfd_is_group_section                 bfd_generic_is_group_section
#define asxxxx_bfd_discard_group                    bfd_generic_discard_group
#define asxxxx_section_already_linked               _bfd_generic_section_already_linked
#define asxxxx_bfd_define_common_symbol             bfd_generic_define_common_symbol
#define asxxxx_bfd_link_hash_table_create           _bfd_generic_link_hash_table_create
#define asxxxx_bfd_link_hash_table_free             _bfd_generic_link_hash_table_free
#define asxxxx_bfd_link_add_symbols                 _bfd_generic_link_add_symbols
#define asxxxx_bfd_link_just_syms                   _bfd_generic_link_just_syms
#define asxxxx_bfd_copy_link_hash_symbol_type \
  _bfd_generic_copy_link_hash_symbol_type
#define asxxxx_bfd_final_link                       _bfd_generic_final_link
#define asxxxx_bfd_link_split_section               _bfd_generic_link_split_section

const bfd_target asxxxx_vec =
{
  "asxxxx",                     /* Name.  */
  bfd_target_asxxxx_flavour,
  BFD_ENDIAN_UNKNOWN,           /* Target byte order.  */
  BFD_ENDIAN_UNKNOWN,           /* Target headers byte order.  */
  (HAS_RELOC | EXEC_P |         /* Object flags.  */
   HAS_LINENO | HAS_DEBUG |
   HAS_SYMS | HAS_LOCALS | WP_TEXT | D_PAGED),
  (SEC_CODE | SEC_DATA | SEC_ROM | SEC_HAS_CONTENTS
   | SEC_ALLOC | SEC_LOAD | SEC_RELOC), /* Section flags.  */
  0,                            /* Leading underscore.  */
  '/',                          /* AR_pad_char.  */
  15,                           /* AR_max_namelen.  */
  1,                            /* match priority.  */
  bfd_getb64, bfd_getb_signed_64, bfd_putb64,
  bfd_getb32, bfd_getb_signed_32, bfd_putb32,
  bfd_getb16, bfd_getb_signed_16, bfd_putb16,   /* Data.  */
  bfd_getb64, bfd_getb_signed_64, bfd_putb64,
  bfd_getb32, bfd_getb_signed_32, bfd_putb32,
  bfd_getb16, bfd_getb_signed_16, bfd_putb16,   /* Hdrs.  */

  {     /* Check the format of a file being read.  Return a <<bfd_target *>> or zero.  */

    _bfd_dummy_target,
    asxxxx_object_p,            /* object */
    bfd_generic_archive_p,      /* archive */
    _bfd_dummy_target,          /* core */
  },
  {     /* Set the format of a file being written.  */
    bfd_false,
    asxxxx_mkobject,            /* object */
    _bfd_generic_mkarchive,     /* archive */
    bfd_false,                  /* core */
  },
  {     /* Write cached information into a file being written, at <<bfd_close>>.  */
    bfd_false,
    bfd_false, //    asxxxx_write_object_contents, /* object */
    _bfd_write_archive_contents,  /* archive */
    bfd_false,                    /* core */
  },

  BFD_JUMP_TABLE_GENERIC (asxxxx),
  BFD_JUMP_TABLE_COPY (_bfd_generic),
  BFD_JUMP_TABLE_CORE (_bfd_nocore),
  BFD_JUMP_TABLE_ARCHIVE (_bfd_archive_coff),
  BFD_JUMP_TABLE_SYMBOLS (asxxxx),
  BFD_JUMP_TABLE_RELOCS (_bfd_norelocs),
  BFD_JUMP_TABLE_WRITE (asxxxx),
  BFD_JUMP_TABLE_LINK (asxxxx),
  BFD_JUMP_TABLE_DYNAMIC (_bfd_nodynamic),

  NULL,

  NULL
};
