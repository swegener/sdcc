/*
  dbuf_string.h - Append formatted string to the dynamic buffer
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


#ifndef __DBUF_STRING_H
#define __DBUF_STRING_H

#include <stdarg.h>
#include "dbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

int dbuf_append_str(struct dbuf_s *dbuf, const char *str);
int dbuf_append_char(struct dbuf_s *dbuf, char chr);
int dbuf_vprintf(struct dbuf_s *dbuf, const char *format, va_list args);
int dbuf_printf (struct dbuf_s *dbuf, const char *format, ...);
void dbuf_write (struct dbuf_s *dbuf, FILE *dest);
void dbuf_write_and_destroy (struct dbuf_s *dbuf, FILE *dest);

#ifdef __cplusplus
}
#endif

#endif  /* __DBUF_STRING_H */
