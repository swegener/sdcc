/*
 * Simulator of microcontrollers (irqcl.h)
 *
 * Copyright (C) 2020,20 Drotos Daniel, Talker Bt.
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

#ifndef IRQ_HEADER
#define IRQ_HEADER

#include "hwcl.h"
#include "itsrccl.h"


enum cpu_cfg
  {
   cpu_nmi_en	= 0,
   cpu_nmi	= 1,
   cpu_irq_en	= 2,
   cpu_irq	= 3,
   cpu_firq_en	= 4,
   cpu_firq	= 5,
   cpu_nr	= 6
  };

// This is used as NMI source
class cl_m6809_src_base: public cl_m6xxx_src
{
public:
  cl_m6809_src_base(cl_uc  *Iuc,
		    int    Inuof,
		    class  cl_memory_cell *Iie_cell,
		    t_mem  Iie_mask,
		    class  cl_memory_cell *Isrc_cell,
		    t_mem  Isrc_mask,
		    t_addr Iaddr,
		    const  char *Iname,
		    int    apoll_priority,
		    u8_t   aEvalue,
		    u8_t   aIFvalue,
		    enum irq_nr Ipass_to):
    cl_m6xxx_src(Iuc, Inuof, Iie_cell, Iie_mask, Isrc_cell, Isrc_mask, Iaddr, Iname, apoll_priority, aEvalue, aIFvalue, Ipass_to) {}
  virtual bool is_nmi(void) { return true; }
  virtual class cl_m6xxx_src *get_parent(void);
};

// Source of IRQ and FIRQ
class cl_m6809_irq_src: public cl_m6809_src_base
{
public:
  cl_m6809_irq_src(cl_uc  *Iuc,
		   int    Inuof,
		   class  cl_memory_cell *Iie_cell,
		   t_mem  Iie_mask,
		   class  cl_memory_cell *Isrc_cell,
		   t_mem  Isrc_mask,
		   t_addr Iaddr,
		   const  char *Iname,
		   int    apoll_priority,
		   u8_t   aEvalue,
		   u8_t   aIFvalue,
		   enum irq_nr Ipass_to):
    cl_m6809_src_base(Iuc, Inuof, Iie_cell, Iie_mask, Isrc_cell, Isrc_mask, Iaddr, Iname, apoll_priority, aEvalue, aIFvalue, Ipass_to)
  {}
  virtual bool is_nmi(void) { return false; }
  virtual bool enabled(void);
};

// This irq will be passed to a parent (one of IRQ, FIRQ, NMI)
class cl_m6809_slave_src: public cl_m6809_irq_src
{
protected:
  t_mem ie_value;
public:
  cl_m6809_slave_src(cl_uc *Iuc,
		     class  cl_memory_cell *Iie_cell,
		     t_mem  Iie_mask,
		     t_mem  Iie_value,
		     class  cl_memory_cell *Isrc_cell,
		     t_mem  Isrc_mask,
		     const  char *Iname):
    cl_m6809_irq_src(Iuc, 0,
		     Iie_cell, Iie_mask, Isrc_cell, Isrc_mask,
		     0,
		     Iname,
		     0, 0, 0,
		     irq_irq)
  {
    ie_value= Iie_value;
  }
  virtual bool enabled(void);
  virtual void clear(void) {}
};


// Interrupt peripheral
class cl_m6809_irq: public cl_hw
{
public:
  class cl_m6809 *muc;
public:
  cl_m6809_irq(class cl_uc *auc);
  virtual int init(void);
  virtual int cfg_size(void) { return cpu_nr; }
  virtual const char *cfg_help(t_addr addr);
  virtual void reset(void);
  virtual t_mem conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val);
  virtual void print_info(class cl_console_base *con);  
};


#endif

/* End of m6809.src/irqcl.h */
