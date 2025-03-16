/*-------------------------------------------------------------------------
  m6502.h - header for SDCC mos6502 port

  Copyright (C) 2022-2025, Gabriele Gorla

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
  Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
-------------------------------------------------------------------------*/

typedef enum
  {
    SUB_MOS6502,
    SUB_MOS65C02
  }
MOS6502_SUB_PORT;

typedef struct
  {
    MOS6502_SUB_PORT sub;
  }
MOS6502_OPTS;

extern MOS6502_OPTS mos6502_opts;

#define IS_MOS6502 (mos6502_opts.sub == SUB_MOS6502)
#define IS_MOS65C02 (mos6502_opts.sub == SUB_MOS65C02)

