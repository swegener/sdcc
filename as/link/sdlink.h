/* sdlink.h

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

#ifndef __SDLINK_H
#define __SDLINK_H

enum sdlink_target_e {
  TARGET_IS_UNKNOWN,
  TARGET_IS_GB,
  TARGET_IS_Z80,
  TARGET_IS_8051,
  TARGET_IS_6808,
};

void sdlink_init (char *path);
int is_sdlink(void);
enum sdlink_target_e get_sdlink_target(void);
int is_sdlink_target_z80_like(void);
int is_sdlink_target_8051_like(void);

#endif  /* __SDLINK_H */
