/*
 * Simulator of microcontrollers (globals.cc)
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
 * 
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 */

/* This file is part of microcontroller simulator: ucsim.

UCSIM is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

UCSIM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UCSIM; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA. */
/*@1@*/

#include "ddconfig.h"

#include <stdio.h>

#include "stypes.h"


// No other global variable should exists!
class cl_sim *simulator;


/* Delimiters to split up command line */
char delimiters[]= " ,;\t\r\n";

struct id_element mem_ids[]= {
  { MEM_ROM , "ROM " },
  { MEM_XRAM, "XRAM" },
  { MEM_IRAM, "IRAM" },
  { MEM_SFR , "SFR " },
  { 0, 0 }
};

struct id_element cpu_states[]= {
  { stGO,	"OK" },
  { stIDLE,	"Idle" },
  { stPD,	"PowerDown" },
  { 0, 0 }
};


/* End of globals.cc */
