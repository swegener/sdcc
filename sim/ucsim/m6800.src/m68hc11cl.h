/*
 * Simulator of microcontrollers (m68hc11cl.h)
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

#ifndef M68HC11CL_HEADER
#define M68HC11CL_HEADER

#include "uccl.h"
#include "memcl.h"

#include "m6800cl.h"
#include "d11p0.h"


#define rY   (IY)
#define rIY  (IY)

#define cY   (cIY)


extern instruction_wrapper_fn itab18[256];
extern int8_t p0ticks11[256];

class cl_m68hcbase: public cl_m6800
{
public:
  u16_t IY;
  class cl_cell16 cIY, cD;
public:
  cl_m68hcbase(class cl_sim *asim): cl_m6800(asim) {}
  virtual int init(void);
public:
  virtual int clock_per_cycle(void) { return 1; }
  virtual void print_regs(class cl_console_base *con);
public:
#include "hc18.h"
};  


/*
 * Base of M68HC11 processor
 */
#define CL11 cl_m68hc11

class cl_m68hc11: public cl_m68hcbase
{
public:
  cl_m68hc11(class cl_sim *asim);
  virtual int init(void);
  virtual const char *id_string(void);
  virtual void reset(void);

  virtual int8_t *tick_tab(t_mem code);
  virtual struct dis_entry *get_dis_entry(t_addr addr);
  virtual char *disassc(t_addr addr, chars *comment=NULL);

  // ALU
  virtual int sub16(class cl_cell16 &dest, u16_t op, bool c);

  // hc11 specific insts on Page0 
  virtual int TEST(t_mem code);
  virtual int IDIV(t_mem code);
  virtual int FDIV(t_mem code);
  virtual int LSRD(t_mem code);
  virtual int ASLD(t_mem code);
  virtual int BRN(t_mem code);
  virtual int PULX(t_mem code);
  virtual int ABX(t_mem code);
  virtual int PSHX(t_mem code);
  virtual int MUL(t_mem code);
  virtual int SUBDi(t_mem code) { return sub16(cD, i16(), false); }
};


#endif

/* End of m6800.src/m68hc11cl.h */
