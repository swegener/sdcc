/* diagnostic subroutines for the SDCC
   Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
   2009, 2010 Free Software Foundation, Inc.
   Copyright (C) 2010 Borut Razem
   Contributed by Gabriel Dos Reis <gdr@codesourcery.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "internal.h"

#ifndef _
# define _(msgid) (msgid)
#endif

/* from libcpp/line-map.c */

/* Print the file names and line numbers of the #include commands
   which led to the map MAP, if any, to stderr.  Nothing is output if
   the most recently listed stack is the same as the current one.  */

void
linemap_print_containing_files (struct line_maps *set,
                                const struct line_map *map)
{
  if (MAIN_FILE_P (map) || set->last_listed == map->included_from)
    return;

  set->last_listed = map->included_from;
  map = INCLUDED_FROM (set, map);

  fprintf (stderr,  _("In file included from %s:%u"),
           map->to_file, LAST_SOURCE_LINE (map));

  while (! MAIN_FILE_P (map))
    {
      map = INCLUDED_FROM (set, map);
      /* Translators note: this message is used in conjunction
         with "In file included from %s:%ld" and some other
         tricks.  We want something like this:

         | In file included from sys/select.h:123,
         |                  from sys/types.h:234,
         |                  from userfile.c:31:
         | bits/select.h:45: <error message here>

         with all the "from"s lined up.
         The trailing comma is at the beginning of this message,
         and the trailing colon is not translated.  */
      fprintf (stderr, _(",\n                 from %s:%u"),
               map->to_file, LAST_SOURCE_LINE (map));
    }

  fputs (":\n", stderr);
}

/* from libcpp/errors.c */

/* Print the logical file location (LINE, COL) in preparation for a
   diagnostic.  Outputs the #include chain if it has changed.  A line
   of zero suppresses the include stack, and outputs the program name
   instead.  */
static void
print_location (cpp_reader *pfile, source_location line, unsigned int col)
{
  if (line == 0)
    fprintf (stderr, "%s: ", progname);
  else
    {
      const struct line_map *map;
      unsigned int lin;

      map = linemap_lookup (pfile->line_table, line);
      linemap_print_containing_files (pfile->line_table, map);

      lin = SOURCE_LINE (map, line);
      if (col == 0)
	{
	  col = SOURCE_COLUMN (map, line);
	  if (col == 0)
	    col = 1;
	}

      if (lin == 0)
	fprintf (stderr, "%s:", map->to_file);
      else if (CPP_OPTION (pfile, show_column) == 0)
	fprintf (stderr, "%s:%u:", map->to_file, lin);
      else
	fprintf (stderr, "%s:%u:%u:", map->to_file, lin, col);

      fputc (' ', stderr);
    }
}

/* from diagnostic.def */

/* DK_UNSPECIFIED must be first so it has a value of zero.  We never
   assign this kind to an actual diagnostic, we only use this in
   variables that can hold a kind, to mean they have yet to have a
   kind specified.  I.e. they're uninitialized.  Within the diagnostic
   machinery, this kind also means "don't change the existing kind",
   meaning "no change is specified".  */
#define DK_UNSPECIFIED ""

/* If a diagnostic is set to DK_IGNORED, it won't get reported at all.
   This is used by the diagnostic machinery when it wants to disable a
   diagnostic without disabling the option which causes it.  */
#define DK_IGNORED ""

/* The remainder are real diagnostic types.  */
#define DK_FATAL "fatal error: "
#define DK_ICE "internal compiler error: "
#define DK_ERROR "error: "
#define DK_SORRY "sorry, unimplemented: "
#define DK_WARNING "warning: "
#define DK_ANACHRONISM "anachronism: "
#define DK_NOTE "note: "
#define DK_DEBUG "debug: "
/* These two would be re-classified as DK_WARNING or DK_ERROR, so the
prefix does not matter.  */
#define DK_PEDWARN "pedwarn: "
#define DK_PERMERROR "permerror: "

/* from c-common.c */

/* Callback from cpp_error for PFILE to print diagnostics from the
   preprocessor.  The diagnostic is of type LEVEL, at location
   LOCATION unless this is after lexing and the compiler's location
   should be used instead, with column number possibly overridden by
   COLUMN_OVERRIDE if not zero; MSG is the translated message and AP
   the arguments.  Returns true if a diagnostic was emitted, false
   otherwise.  */

bool
c_cpp_error (cpp_reader *pfile ATTRIBUTE_UNUSED, int level,
	     location_t location, unsigned int column_override,
	     const char *msg, va_list *ap)
{
  const char *dlevel;
  int save_warn_system_headers = warn_system_headers;

  switch (level)
    {
    case CPP_DL_WARNING_SYSHDR:
      if (flag_no_output)
	return false;
      warn_system_headers = 1;
      /* Fall through.  */
    case CPP_DL_WARNING:
      if (flag_no_output)
	return false;
      dlevel = DK_WARNING;
      break;
    case CPP_DL_PEDWARN:
      if (flag_no_output && !flag_pedantic_errors)
	return false;
      dlevel = DK_PEDWARN;
      break;
    case CPP_DL_ERROR:
      dlevel = DK_ERROR;
      break;
    case CPP_DL_ICE:
      dlevel = DK_ICE;
      break;
    case CPP_DL_NOTE:
      dlevel = DK_NOTE;
      break;
    case CPP_DL_FATAL:
      dlevel = DK_FATAL;
      break;
    default:
      gcc_unreachable ();
    }

  print_location (pfile, location, column_override);
  fputs (_(dlevel), stderr);
  vfprintf (stderr, _(msg), *ap);
  putc ('\n', stderr);

  if (level == CPP_DL_WARNING_SYSHDR)
    warn_system_headers = save_warn_system_headers;

  return true;
}

