/*
  dbuf_string.c - Append formatted string to the dynamic buffer
  version 1.0.0, January 6th, 2007

  Copyright (c) 2002-2007 Borut Razem

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street - Fifth Floor,
  Boston, MA 02110-1301, USA.
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "dbuf_string.h"


/*
 * Append string to the end of the buffer.
 */

int
dbuf_append_str(struct dbuf_s *dbuf, const char *str)
{
  assert(str != NULL);

  return dbuf_append(dbuf, str, strlen(str));
}


/*
 * Append single character to the end of the buffer.
 */

int
dbuf_append_char(struct dbuf_s *dbuf, char chr)
{
  return dbuf_append(dbuf, &chr, 1);
}

/*
 * Calculate length of the resulting formatted string.
 *
 * Borrowed from vasprintf.c
 */

static int
calc_result_length (const char *format, va_list args)
{
  const char *p = format;
  /* Add one to make sure that it is never zero, which might cause malloc
     to return NULL.  */
  int total_width = strlen (format) + 1;
  va_list ap;

  memcpy (&ap, &args, sizeof (va_list));

  while (*p != '\0')
    {
      if (*p++ == '%')
	{
	  while (strchr ("-+ #0", *p))
	    ++p;
	  if (*p == '*')
	    {
	      ++p;
	      total_width += abs (va_arg (ap, int));
	    }
	  else
	    total_width += strtoul (p, (char **) &p, 10);
	  if (*p == '.')
	    {
	      ++p;
	      if (*p == '*')
		{
		  ++p;
		  total_width += abs (va_arg (ap, int));
		}
	      else
	      total_width += strtoul (p, (char **) &p, 10);
	    }
	  while (strchr ("hlL", *p))
	    ++p;
	  /* Should be big enough for any format specifier except %s and floats.  */
	  total_width += 30;
	  switch (*p)
	    {
	    case 'd':
	    case 'i':
	    case 'o':
	    case 'u':
	    case 'x':
	    case 'X':
	    case 'c':
	      (void) va_arg (ap, int);
	      break;
	    case 'f':
	    case 'e':
	    case 'E':
	    case 'g':
	    case 'G':
	      (void) va_arg (ap, double);
	      /* Since an ieee double can have an exponent of 307, we'll
		 make the buffer wide enough to cover the gross case. */
	      total_width += 307;
	      break;
	    case 's':
	      total_width += strlen (va_arg (ap, char *));
	      break;
	    case 'p':
	    case 'n':
	      (void) va_arg (ap, char *);
	      break;
	    }
	  p++;
	}
    }

  return total_width;
}


/*
 * Append the formatted string to the end of the buffer.
 */

int
dbuf_vprintf (struct dbuf_s *dbuf, const char *format, va_list args)
{
  int size = calc_result_length (format, args);

  assert(dbuf != NULL);
  assert(dbuf->alloc != 0);
  assert(dbuf->buf != NULL);

  if (_dbuf_expand(dbuf, size) != 0) {
    int len = vsprintf(&(((char *)dbuf->buf)[dbuf->len]), format, args);

    if (len >= 0) {
      /* if written length is greater then the calculated one,
        we have a buffer overrun! */
      assert(len <= size);
      dbuf->len += len;
    }
    return len;
  }

  return 0;
}


/*
 * Append the formatted string to the end of the buffer.
 */

int
dbuf_printf (struct dbuf_s *dbuf, const char *format, ...)
{
  va_list arg;
  int len;

  va_start (arg, format);
  len = dbuf_vprintf(dbuf, format, arg);
  va_end (arg);

  return len;
}


/*
 * Write dynamic buffer to the file.
 */

void
dbuf_write (struct dbuf_s *dbuf, FILE *dest)
{
  fwrite(dbuf_get_buf(dbuf), 1, dbuf_get_length(dbuf), dest);
}


/*
 * Write dynamic buffer to the file and destroy it.
 */

void
dbuf_write_and_destroy (struct dbuf_s *dbuf, FILE *dest)
{
  dbuf_write (dbuf, dest);

  dbuf_destroy(dbuf);
}
