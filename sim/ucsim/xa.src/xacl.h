/*
 * Simulator of microcontrollers (xacl.h)
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
 *
 * Written by Karl Bongers karl@turbobit.com
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

#ifndef XACL_HEADER
#define XACL_HEADER

#include "uccl.h"

#include "regsxa.h"

/*
 * Base type of XA microcontrollers
 */

class cl_xa: public cl_uc
{
public:
  cl_mem *ram;
  cl_mem *rom;
  struct t_regs regs;

  // for now make it as simple as possible
  TYPE_UBYTE mem_direct[1024*2];
#ifndef WORDS_BIGENDIAN
  TYPE_UWORD *wmem_direct;  /* word pointer at mem_direct */
#endif

public:
  cl_xa(class cl_sim *asim);
  virtual int init(void);
  virtual char *id_string(void);

  virtual t_addr get_mem_size(enum mem_class type);
  virtual void mk_hw_elements(void);

  virtual struct dis_entry *dis_tbl(void);

  virtual int inst_length(t_addr addr);
  virtual int inst_branch(t_addr addr);
  virtual int longest_inst(void);

  virtual int get_disasm_info(t_addr addr,
                       int *ret_len,
                       int *ret_branch,
                       int *immed_offset,
                       int *parms,
                       int *mnemonic);

  virtual char *disass(t_addr addr, char *sep);
  virtual void print_regs(class cl_console *con);

  virtual int exec_inst(void);
  virtual int get_reg(int word_flag, unsigned int index);

#include "instcl.h"
};


#endif

/* End of xa.src/xacl.h */
