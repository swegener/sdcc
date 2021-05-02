/*
 * Simulator of microcontrollers (irqcl.h)
 *
 * Copyright (C) @@S@@,@@Y@@ Drotos Daniel, Talker Bt.
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

#include "hwcl.h"

#include "mcs6502cl.h"

enum irq_cfg
  {
    m65_nmi_en	= 0,
    m65_nmi	= 1,
    m65_irq_en	= 2,
    m65_irq	= 3,
    m65_nr	= 4
  };

// NMI source
class cl_nmi: public cl_m6xxx_src
{
 public:
  cl_nmi(cl_uc  *Iuc,
	 int    Inuof,
	 class  cl_memory_cell *Iie_cell,
	 t_mem  Iie_mask,
	 class  cl_memory_cell *Isrc_cell,
	 t_mem  Isrc_mask,
	 t_addr Iaddr,
	 const  char *Iname,
	 int    apoll_priority):
  cl_m6xxx_src(Iuc, Inuof, Iie_cell, Iie_mask, Isrc_cell, Isrc_mask, Iaddr, Iname, apoll_priority, irq_none) {}
  virtual bool is_nmi(void) { return true; }
};

// IRQ source
class cl_irq: public cl_nmi
{
 public:
  cl_irq(cl_uc  *Iuc,
	 int    Inuof,
	 class  cl_memory_cell *Iie_cell,
	 t_mem  Iie_mask,
	 class  cl_memory_cell *Isrc_cell,
	 t_mem  Isrc_mask,
	 t_addr Iaddr,
	 const  char *Iname,
	 int    apoll_priority):
  cl_nmi(Iuc, Inuof, Iie_cell, Iie_mask, Isrc_cell, Isrc_mask, Iaddr, Iname, apoll_priority)
  {}
  virtual bool is_nmi(void) { return false; }
  virtual bool enabled(void);
};


// IRQ peripheral
class cl_irq_hw: public cl_hw
{
 public:
  class cl_mcs6502 *muc;
 public:
  cl_irq_hw(class cl_uc *auc);
  virtual int init(void);
  virtual int cfg_size(void) { return m65_nr; }
};


/* End of mcs6502.src/irqcl.h */
