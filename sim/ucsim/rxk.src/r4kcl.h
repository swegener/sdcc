/*
 * Simulator of microcontrollers (r4kcl.h)
 *
 * Copyright (C) 2020,2021 Drotos Daniel, Talker Bt.
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

#ifndef R4KCL_HEADER
#define R4KCL_HEADER

#include "r3kacl.h"
#include "dp0m4.h"
#include "dpddm4.h"


extern inline u32_t px8(u32_t px, u8_t offset);
extern inline u32_t px8se(u32_t px, u8_t offset);
extern inline u32_t px16(u32_t px, u16_t offset);
extern inline u32_t px16se(u32_t px, u16_t offset);

#define rJ  (JK.r.J)
#define rK  (JK.r.K)
#define rJK (JK.JK)

#define raJ  (aJK.r.J)
#define raK  (aJK.r.K)
#define raJK (aJK.JK)

class cl_r4k: public cl_r3ka
{
public:
  RP(JK,JK,J,K);
  RP(aJK,JK,J,K);
  u32_t rPW, rPX, rPY, rPZ;
  u32_t raPW, raPX, raPY, raPZ;
  class cl_cell8 cJ, caJ, cK, caK;
  class cl_cell16 cJK, caJK;
  class cl_cell32 cPW, cPX, cPY, cPZ;
  class cl_cell32 caPW, caPX, caPY, caPZ;
 public:
  cl_r4k(class cl_sim *asim);
  virtual int init();
  virtual const char *id_string(void);
  virtual void reset(void);
  
  virtual void make_cpu_hw(void);

  virtual struct dis_entry *dis_entry(t_addr addr);
  virtual struct dis_entry *dis_6d_entry(t_addr addr);
  
  virtual void print_regs(class cl_console_base *con);

  virtual void mode3k(void);
  virtual void mode4k(void);

  virtual int EXX(t_mem code);
  
  // Page DD/FD
  virtual int LD_A_iIRA(t_mem code);

  // Starter of page 6D
  virtual int PAGE_4K6D(t_mem code);
};

class cl_r4k_cpu: public cl_rxk_cpu
{
protected:
  class cl_r4k *r4uc;
  class cl_cell8 *edmr;
public:
  cl_r4k_cpu(class cl_uc *auc);
  virtual int init(void);
  virtual t_mem read(class cl_memory_cell *cell);
  virtual void write(class cl_memory_cell *cell, t_mem *val);
  virtual const char *cfg_help(t_addr addr);
  
  //virtual t_mem conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val);

  virtual void print_info(class cl_console_base *con);
};


#endif

/* End of rxk.src/r4kcl.h */
