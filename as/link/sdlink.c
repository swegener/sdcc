/* sdlink.c

   Copyright (C) 2009 Borut Razem

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

#ifndef _WIN32
#include <libgen.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sdlink.h"

#define NELEM(x)  (sizeof (x) / sizeof (x)[0])


static int sdlink = -1;
static enum sdlink_target_e target = TARGET_IS_UNKNOWN;


static char
*program_name (char *path)
{
#ifdef _WIN32
  static char fname[_MAX_FNAME];

  _splitpath (path, NULL, NULL, fname, NULL);
  return fname;
#else
  return basename (path);
#endif
}


static void
check_init(void)
{
  if (sdlink == -1) {
    fprintf(stderr, "sdlink_init not called!\n");
    exit (1);
  }
}


void
sdlink_init (char *path)
{
  struct tgt_s {
    char *str;
    enum sdlink_target_e target;
  } tgt[] = {
    { "gb", TARGET_IS_GB, },
    { "z80", TARGET_IS_Z80, },
    { "8051", TARGET_IS_8051, },
    { "6808", TARGET_IS_6808, },
  };
  int i;

  char *progname = program_name (path);
  if ((sdlink = (strncmp(progname, "sdlink", 6) == 0)) != 0)
    {
      /* exception: sdlink is 8051 linker */
      if (progname[6] == '\0')
	target = TARGET_IS_8051;
      else
        {
          for (i = 0; i < NELEM (tgt); ++i)
            {
              if (strstr(progname, tgt[i].str))
                {
                  target = tgt[i].target;
                  break;
		}
            }
        }
    }
}


int
is_sdlink(void)
{
  check_init();
  return sdlink;
}


enum sdlink_target_e
get_sdlink_target(void)
{
  check_init();
  return sdlink;
}


int
is_sdlink_target_z80_like(void)
{
  check_init();
  return target == TARGET_IS_Z80 || target == TARGET_IS_GB;
}


int
is_sdlink_target_8051_like(void)
{
  check_init();
  return target == TARGET_IS_8051;
}
