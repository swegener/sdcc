/*
 * Simulator of microcontrollers (i8041cl.h)
 *
 * Copyright (C) 2022 Drotos Daniel
 * 
 * To contact author send email to dr.dkdb@gmail.com
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


#ifndef I8041CL_HEADER
#define I8041CL_HEADER

#include "i8048cl.h"


enum i8041cpu_confs
  {
    i8041cpu_in		= i8020cpu_nuof+0,
    i8041cpu_ctrl	= i8020cpu_nuof+1,
    i8041cpu_out	= i8020cpu_nuof+2,
    i8041cpu_status	= i8020cpu_nuof+3,
    i8041cpu_obfclear	= i8020cpu_nuof+4,
    i8041cpu_enflags	= i8020cpu_nuof+5,
    i8041cpu_nuof	= i8020cpu_nuof+6
  };

enum i8041_status {
  // Output Buffer Full: OUTDBBA->1, external read->0
  stat_obf	= 1,
  // Input Buffer Full: External write->1, INADBB->0
  stat_ibf	= 2,
  // General flag: copy of PSW.F0 ??
  stat_f0	= 4,
  // A0 address input: set by external write (data or ctrl)
  // or CLR F1, CPL F1
  // uc.flagF1, uc.cflagF1
  stat_f1	= 8
};


class cl_i8041: public cl_i8048
{
 public:
  cl_i8041(class cl_sim *asim);
  cl_i8041(class cl_sim *asim,
	   unsigned int rom_siz,
	   unsigned int ram_siz);
  virtual int init(void);
  virtual class cl_memory_operator *make_flagop(void);
  virtual void make_irq_sources(void);
  virtual void make_cpu_hw(void);
  virtual void decode_regs(void);
  
  // UPI41 sepcific instructions to implement
  //02 OUT DBB,A
  virtual int OUTDBBA(MP);
  //22 IN A,DBB
  virtual int INADBB(MP);
  //90 MOV STS,A
  virtual int MOVSTSA(MP);
  //f5 EN FLAGS
  virtual int ENFLAGS(MP);
  //e5 EN DMA
  virtual int ENDMA(MP) { return resGO; }
  //86 'a8' JOBF
  virtual int JOBF(MP) { return jif(cpu->cfg_get(i8041cpu_status) & stat_obf); }
  //d6 'a8' JNIBF
  virtual int JNIBF(MP) { return jif(!(cpu->cfg_get(i8041cpu_status) & stat_ibf)); }
};


class cl_i8041_cpu: public cl_i8020_cpu
{
public:
  cl_i8041_cpu(class cl_uc *auc);
  virtual int init(void);
  virtual void reset(void);
  virtual unsigned int cfg_size(void) { return i8041cpu_nuof; }
  virtual const char *cfg_help(t_addr addr);
  virtual t_mem conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val);
  virtual void set_flags(void);
  virtual void print_info(class cl_console_base *con);
};


#endif

/* End of i8048.src/i8041cl.h */
