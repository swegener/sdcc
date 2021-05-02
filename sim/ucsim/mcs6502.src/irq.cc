/*
 * Simulator of microcontrollers (irq.cc)
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

#include "globals.h"

#include "irqcl.h"


/* NMI source */
/*
class cl_m6xxx_src *
cl_nmi::get_parent(void)
{
  class cl_mcs6502 *muc= (class cl_mcs6502 *)(application->get_uc());
  switch (pass_to)
    {
    case irq_nmi:
      return muc->src_nmi;
      break;
    case irq_irq:
      return muc->src_irq;
      break;
    default:
      return NULL;
    }
  return NULL;
}
*/

/* IRQ source */

bool
cl_irq::enabled(void)
{
  if (!ie_cell)
    return false;
  t_mem e= ie_cell->get();
  e&= ie_mask;
  return e == 0;
}


/* IRQ handling peripheral */

cl_irq_hw::cl_irq_hw(class cl_uc *auc):
  cl_hw(auc, HW_INTERRUPT, 0, "irq")
{
  muc= (class cl_mcs6502 *)auc;
}

int
cl_irq_hw::init()
{
  class cl_var *v;

  cl_hw::init();
  uc->vars->add(v= new cl_var("NMI", cfg, m65_nmi, "NMI request/clear"));
  v->init();
  uc->vars->add(v= new cl_var("IRQ", cfg, m65_irq, "IRQ request/clear"));
  v->init();

  return 0;
}

/* End of mcs6502.src/irq.cc */
