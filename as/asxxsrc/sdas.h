/* sdas.h

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

#ifndef __SDAS_H
#define __SDAS_H

enum sdas_target_e {
  TARGET_IS_UNKNOWN,
  TARGET_IS_GBZ80,
  TARGET_IS_Z80,
  TARGET_IS_MCS51,
  TARGET_IS_HC08,
};

void sdas_init (char *path);
int is_sdas(void);
enum sdas_target_e get_sdas_target(void);
int is_sdas_target_z80_like(void);

#endif  /* __SDCA_H */
