/*
 * Simulator of microcontrollers (stm8.src/clkcl.h)
 *
 * Copyright (C) 2017,17 Drotos Daniel, Talker Bt.
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

#ifndef CLKCL_HEADER
#define CLKCL_HEADER

#include "hwcl.h"

class cl_clk_event: public cl_base
{
 public:
  enum hw_cath cath;
  int id;
 cl_clk_event(void): cl_base() {}
 cl_clk_event(enum hw_cath acath, int aid): cl_base()
    { cath= acath; id= aid; }
  virtual void set(enum hw_cath acath, int aid)
    { cath= acath; id= aid; }
};

class cl_clk: public cl_hw
{
 protected:
  static const unsigned long f_HSI = 16000000;

  static const t_mem CLK_ECKR_LSERDY = (1 << 3);
  static const t_mem CLK_ECKR_LSEEN = (1 << 2);
  static const t_mem CLK_ECKR_HSERDY = (1 << 1);
  static const t_mem CLK_ECKR_HSEEN = (1 << 0);

  static const t_mem CLK_ICKR_LSIRDY = (1 << 4);
  static const t_mem CLK_ICKR_LSIEN = (1 << 3);
  static const t_mem CLK_ICKR_HSIRDY = (1 << 1);
  static const t_mem CLK_ICKR_HSIEN = (1 << 0);

  static const t_mem CLK_SWCR_SWBSY = 0x01;
  static const t_mem CLK_SWCR_SWEN = 0x02;

  unsigned long f_LSI;
  unsigned long f_LSE;

  t_mem CLK_SOURCE_HSI;
  t_mem CLK_SOURCE_LSI;
  t_mem CLK_SOURCE_HSE;
  t_mem CLK_SOURCE_LSE;

  double f_OSC;
  t_addr base;

  class cl_memory_cell
    *cmsr,
    *eckr,
    *ickr,
    *swr,
    *swcr,
    *ckdivr,
    *pckenr1,
    *pckenr2,
    *pckenr3;

  bool cpudiv;

  void set_osc(t_mem osc);

 public:
  cl_clk(class cl_uc *auc);

  virtual int init(void);
  virtual void reset(void) {}

  virtual int clock_per_cycle(void);
  virtual int xtal_per_clock(t_mem ckdivr_val);

  virtual void write(class cl_memory_cell *cell, t_mem *val);
  virtual bool tim(int id, t_mem *val) { return false; }
  virtual bool usart(int id, t_mem *val) { return false; }

  virtual void print_info(class cl_console_base *con);
};

class cl_clk_saf: public cl_clk
{
private:
  static const t_mem CLK_ICKR_FHWU = (1 << 2);
  static const t_mem CLK_ICKR_REGAH = (1 << 5);

 public:
  cl_clk_saf(class cl_uc *auc);

  virtual int init(void);
  virtual void reset(void);

  virtual int clock_per_cycle(void);
  virtual int xtal_per_clock(t_mem ckdivr_val);

  virtual void write(class cl_memory_cell *cell, t_mem *val);
  virtual bool tim(int id, t_mem *val);
  virtual bool usart(int id, t_mem *val);
};

class cl_clk_all: public cl_clk
{
private:
  static const t_mem CLK_ICKR_FHWU = (1 << 5);
  static const t_mem CLK_ICKR_SAHALT = (1 << 4);

 public:
  cl_clk_all(class cl_uc *auc);
  virtual int init(void);

  virtual void reset(void);

  virtual int xtal_per_clock(t_mem ckdivr_val);

  virtual void write(class cl_memory_cell *cell, t_mem *val);
  virtual bool tim(int id, t_mem *val);
  virtual bool usart(int id, t_mem *val);
};

class cl_clk_l101: public cl_clk
{
 public:
  cl_clk_l101(class cl_uc *auc);

  virtual int init(void);
  virtual void reset(void);

  virtual int xtal_per_clock(t_mem ckdivr_val);

  virtual bool tim(int id, t_mem *val);
  virtual bool usart(int id, t_mem *val);
};


#endif

/* End of stm8.src/clkcl.h */
