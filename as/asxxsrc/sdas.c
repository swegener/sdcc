/* sdas.c

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

#include "sdas.h"

#define NELEM(x)  (sizeof (x) / sizeof (x)[0])


static int sdas = -1;
static enum sdas_target_e target = TARGET_IS_UNKNOWN;


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
  if (sdas == -1) {
    fprintf(stderr, "sdas_init not called!\n");
    exit (1);
  }
}


void
sdas_init (char *path)
{
  struct tgt_s {
    char *str;
    enum sdas_target_e target;
  } tgt[] = {
    { "gbz80", TARGET_IS_GBZ80, },  /* must be before z80! */
    { "z80",   TARGET_IS_Z80, },
    { "mcs51", TARGET_IS_MCS51, },
    { "hc08",  TARGET_IS_HC08, },
  };
  int i;

  char *progname = program_name (path);
  sdas = (strncmp(progname, "sdas", 4) == 0);

  for (i = 0; i < NELEM (tgt); ++i)
    {
      if (strstr(progname, tgt[i].str))
        {
          target = tgt[i].target;
          break;
        }
    }
}


int
is_sdas(void)
{
  check_init();
  /* TODO: should be uncommented when adcc asxxxx renamed to sdas
  return sdas; */
  return 1;
}


enum sdas_target_e
get_sdas_target(void)
{
  check_init();
  return sdas;
}


int
is_sdas_target_z80_like(void)
{
  check_init();
  return target == TARGET_IS_Z80 || target == TARGET_IS_GBZ80;
}
