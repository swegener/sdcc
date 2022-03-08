/*
 * Simulator of microcontrollers (stm8.src/port.cc)
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

#include "stm8cl.h"
#include "itsrccl.h"
#include "portcl.h"


struct state {
  t_mem pins, odr, idr, ddr, cr1, cr2;
  t_mem exti_conf1, exti_conf2;
  t_mem exti_cr1, exti_cr2, exti_cr3, exti_cr4;
  t_mem exti_sr1, exti_sr2;
};


cl_port::cl_port(class cl_uc *auc, int iportnr/*, int aid*/, const char *aname):
	cl_hw(auc, HW_PORT, /*aid*/0, aname)
{
  base = 0x5000 + iportnr * 5;
  portnr = iportnr;
}

int
cl_port::init(void)
{
  cl_hw::init();

  class cl_stm8 *stm8 = static_cast<class cl_stm8 *>(uc);

  chars pn= chars(get_name());

  cell_pins = register_cell(stm8->ports, portnr, pn+"_pins", "Outside value of port pins");
  // Compat alias
  uc->vars->add(pn+"_pin", stm8->ports, portnr, 7, 0, "Outside value of port pins");

  pn.uppercase();

  cell_odr = register_cell(uc->rom, base + 0, pn+"_ODR", "Output Data Register");
  cell_idr = register_cell(uc->rom, base + 1, pn+"_IDR", "Input Data Register");
  cell_ddr = register_cell(uc->rom, base + 2, pn+"_DDR", "Data Direction Register");
  cell_cr1 = register_cell(uc->rom, base + 3, pn+"_CR1", "Control Register 1");
  cell_cr2 = register_cell(uc->rom, base + 4, pn+"_CR2", "Control Register 2");

  class cl_itc *itc = stm8->itc;

  // We need to hook EXTI_CR* because changing interrupt sensitivity
  // may lead to an immediate interrupt.
  register_cell(itc->exti_cr1);
  register_cell(itc->exti_cr2);
  register_cell(itc->exti_cr3);
  register_cell(itc->exti_cr4);

  // We need to hook EXTI_CR* because changing them may change what
  // interrupts are presented.
  // N.B. We assume the presentation of edge-triggered interrupts
  // is fixed at the time the event is handled. Documentation is
  // not clear on this point but it seems reasonable?
  register_cell(itc->exti_conf1);
  register_cell(itc->exti_conf2);

  // We need to hook EXTI_SR* because a level-triggered interrupt
  // may come straight back when cleared.
  register_cell(itc->exti_sr1);
  register_cell(itc->exti_sr2);

  return 0;
}

void
cl_port::reset(void)
{
  cell_cr2->write(0);
  if (uc->type->type == CPU_STM8S)
    cell_cr1->write(base == 0x500f ? 0x02: 0); // PD_CR1 reset value is 0x02 (RM0016: 11.9.5)
  else
    cell_cr1->write(base == 0x5000 ? 0x01: 0); // PA_CR1 reset value is 0x01 (RM0013: 10.9.4 and RM0031: 10.9.4)
  cell_ddr->write(0);
  cell_odr->write(0);
  cell_idr->write(0);
}

bool
cl_port::high_bits_are_port_interrupt(const struct state *s)
{
  if (uc->type->type != CPU_STM8S)
    {
      switch (portnr)
        {
          case 0: // Port A
            return false;
          case 1: // Port B
            return (s->exti_conf1 & (1U << 1));
          case 2: // Port C
            return false;
          case 3: // Port D
            return (s->exti_conf1 & (1U << 3));
          case 4: // Port E
            return (s->exti_conf1 & (1U << 5));
          case 5: // Port F
            return (s->exti_conf2 & (1U << 0));
          case 6: // Port G
            return (s->exti_conf2 & (1U << 2));
          case 7: // Port H
            return (s->exti_conf2 & (1U << 4));
          case 8: // Port I
            return false;
        }
    }

  return true;
}

bool
cl_port::low_bits_are_port_interrupt(const struct state *s)
{
  //class cl_stm8 *stm8 = static_cast<class cl_stm8 *>(uc);

  if (uc->type->type != CPU_STM8S)
    {
      switch (portnr)
        {
          case 0: // Port A
            return false;
          case 1: // Port B
            return (s->exti_conf1 & (1U << 0));
          case 2: // Port C
            return false;
          case 3: // Port D
            return (s->exti_conf1 & (1U << 2));
          case 4: // Port E
            return (s->exti_conf1 & (1U << 4));
          case 5: // Port F
            return (s->exti_conf1 & (1U << 6));
          case 6: // Port G
            return (s->exti_conf2 & (1U << 1));
          case 7: // Port H
            return (s->exti_conf2 & (1U << 3));
          case 8: // Port I
            return false;
        }
    }

  return true;
}

bool
cl_port::port_used_for_interrupt(const struct state *s)
{
  switch (portnr)
    {
      case 0: // Port A
        return (uc->type->type == CPU_STM8S);
      case 1: // Port B
        return (uc->type->type == CPU_STM8S) || !(s->exti_conf2 & (1U << 5));
      case 2: // Port C
        return (uc->type->type == CPU_STM8S);
      case 3: // Port D
        return (uc->type->type == CPU_STM8S) || !(s->exti_conf2 & (1U << 6));
      case 4: // Port E
        return (uc->type->type == CPU_STM8S) || !(s->exti_conf1 & (1U << 7));
      case 5: // Port F
        return (uc->type->type != CPU_STM8S) && (s->exti_conf1 & (1U << 7));
      case 6: // Port G
        return (s->exti_conf2 & (1U << 5));
      case 7: // Port H
        return (s->exti_conf2 & (1U << 6));
      case 8: // Port I
        break;
    }
  return false;
}

int
cl_port::port_sensitivity(const struct state *s)
{
  if (uc->type->type == CPU_STM8S)
    {
      switch (portnr)
        {
          case 0: // Port A
            return ((s->exti_cr1 >> 0) & 3);
          case 1: // Port B
            return ((s->exti_cr1 >> 2) & 3);
          case 2: // Port C
            return ((s->exti_cr1 >> 4) & 3);
          case 3: // Port D
            return ((s->exti_cr1 >> 6) & 3);
          case 4: // Port E
            return ((s->exti_cr2 >> 0) & 3);
        }
    }
  else
    {
      switch (portnr)
        {
          case 0: // Port A
            break;
          case 1: // Port B
            return ((s->exti_cr3 >> 0) & 3);
          case 2: // Port C
            break;
          case 3: // Port D
            return ((s->exti_cr3 >> 2) & 3);
          case 4: // Port E
            return ((s->exti_cr3 >> 4) & 3);
          case 5: // Port F
            return ((s->exti_cr3 >> 6) & 3);
          case 6: // Port G
            return ((s->exti_cr4 >> 0) & 3);
          case 7: // Port H
            return ((s->exti_cr4 >> 2) & 3);
          case 8: // Port I
            break;
        }
    }

  return 0;
}

void
cl_port::port_interrupt(struct state *s)
{
  if (uc->type->type == CPU_STM8S)
      s->exti_sr1 |= (1U << portnr);
  else if (portnr == 1)
      s->exti_sr2 |= (1U << 0);
  else if (portnr >= 3 && portnr <= 5)
      s->exti_sr2 |= (1U << (portnr - 2));
}

void
cl_port::port_check_interrupt(struct state *s, t_mem bitmask, int bithigh, int bitlow)
{
  t_mem input_and_enabled = (~s->ddr) & s->cr2;

  switch (port_sensitivity(s))
    {
      case 0: // Falling edge and low level
        for (int i = bitlow; i <= bithigh; i++)
          {
            if ((input_and_enabled & (1U << i)) && !(s->idr & (1U << i)))
              {
                port_interrupt(s);
                break;
              }
          }
        break;

      case 1: // Rising edge only
        for (int i = bitlow; i <= bithigh; i++)
          {
            if ((input_and_enabled & (1U << i)) && !(last_idr & (1U << i)) && (s->idr & (1U << i)))
              {
                port_interrupt(s);
                break;
              }
          }
        break;

      case 2: // Falling edge only
        for (int i = bitlow; i <= bithigh; i++)
          {
            if ((input_and_enabled & (1U << i)) && (last_idr & (1U << i)) && !(s->idr & (1U << i)))
              {
                port_interrupt(s);
                break;
              }
          }
        break;

      case 3: // Rising and falling edge
        for (int i = bitlow; i <= bithigh; i++)
          {
            if ((input_and_enabled & (1U << i)) && (last_idr & (1U << i)) != (s->idr & (1U << i)))
              {
                port_interrupt(s);
                break;
              }
          }
        break;
    }
}

void
cl_port::pin_check_interrupt(struct state *s, t_mem exti_cr, int bithigh, int bitlow)
{
  t_mem input_and_enabled = (~s->ddr) & s->cr2;

  for (int i = bitlow; i <= bithigh; i++)
    {
      t_mem bit = 1U << i;

      if ((input_and_enabled & bit))
        {
          switch (exti_cr >> ((i - bitlow) * 2) & 3)
            {
              case 0: // Falling edge and low level
                if (!(s->idr & bit))
                  s->exti_sr1 |= bit;
                break;

              case 1: // Rising edge only
                if (!(last_idr & bit) && (s->idr & bit))
                  s->exti_sr1 |= bit;
                break;

              case 2: // Falling edge only
                if ((last_idr & bit) && !(s->idr & bit))
                  s->exti_sr1 |= bit;
                break;

              case 3: // Rising and falling edge
                if ((last_idr & bit) != (s->idr & bit))
                  s->exti_sr1 |= bit;
                break;
            }
        }
    }
}

void
cl_port::write(class cl_memory_cell *cell, t_mem *val)
{
  // IDR is not writeable except by us
  if (cell == cell_idr)
    {
      *val = cell_idr->get();
      return;
    }

  class cl_stm8 *stm8 = static_cast<class cl_stm8 *>(uc);
  class cl_itc *itc = stm8->itc;

  struct state s;
  s.pins = (cell == cell_pins ? *val : cell_pins->get()),
  s.odr = (cell == cell_odr ? *val : cell_odr->get()),
  s.idr = (cell == cell_idr ? *val : cell_idr->get()),
  s.ddr = (cell == cell_ddr ? *val : cell_ddr->get()),
  s.cr1 = (cell == cell_cr1 ? *val : cell_cr1->get()),
  s.cr2 = (cell == cell_cr2 ? *val : cell_cr2->get()),

  s.exti_conf1 = (cell == itc->exti_conf1 ? *val : itc->exti_conf1->get()),
  s.exti_conf2 = (cell == itc->exti_conf2 ? *val : itc->exti_conf2->get()),

  s.exti_cr1 = (cell == itc->exti_cr1 ? *val : itc->exti_cr1->get()),
  s.exti_cr2 = (cell == itc->exti_cr2 ? *val : itc->exti_cr2->get()),
  s.exti_cr3 = (cell == itc->exti_cr3 ? *val : itc->exti_cr3->get()),
  s.exti_cr4 = (cell == itc->exti_cr4 ? *val : itc->exti_cr4->get()),

  s.exti_sr1 = (cell == itc->exti_sr1 ? *val : itc->exti_sr1->get()),
  s.exti_sr2 = (cell == itc->exti_sr2 ? *val : itc->exti_sr2->get()),

  // Output bits are mirrored in the input so IDR and pins are the same
  // except that pins are writeable externally and that's where we get
  // the inputs from.
  s.pins = s.idr = (s.pins & (~s.ddr)) | (s.odr & s.ddr);

  if (cell == cell_pins)
    {
      *val = s.pins;
      cell_idr->write(s.pins);
    }
  else if (cell == cell_odr || cell == cell_ddr)
    {
      cell_pins->write(s.pins);
      cell_idr->write(s.pins);
    }

  t_mem orig_sr1 = s.exti_sr1;
  t_mem orig_sr2 = s.exti_sr2;

  if (high_bits_are_port_interrupt(&s))
    {
      if (port_used_for_interrupt(&s))
        port_check_interrupt(&s, 0xf0, 7, 4);
    }
  else
    pin_check_interrupt(&s, s.exti_cr2, 7, 4);

  if (low_bits_are_port_interrupt(&s))
    {
      if (port_used_for_interrupt(&s))
        port_check_interrupt(&s, 0x0f, 3, 0);
    }
  else
    pin_check_interrupt(&s, s.exti_cr1, 3, 0);

  last_idr = s.idr;

  if (s.exti_sr1 != orig_sr1)
    {
      if (cell == itc->exti_sr1)
        *val = s.exti_sr1;
      else
        itc->exti_sr1->write(s.exti_sr1);
    }

  if (s.exti_sr2 != orig_sr2)
    {
      if (cell == itc->exti_sr2)
        *val = s.exti_sr2;
      else
        itc->exti_sr2->write(s.exti_sr2);
    }
}

void
cl_port::print_info(class cl_console_base *con)
{
  int m;
  t_mem o= cell_odr->get(),
    i= cell_idr->get(),
    d= cell_ddr->get();
  con->dd_printf("%s at 0x%04x\n", get_name(), base);
  con->dd_printf("dir: 0x%02x ", d);
  for (m= 0x80; m; m>>= 1)
    con->dd_printf("%c", (d & m)?'O':'I');
  con->dd_printf("\n");
  con->dd_printf("out: 0x%02x ", o);
  for (m= 0x80; m; m>>= 1)
    {
      if (d & m)
	con->dd_printf("%c", (o & m)?'1':'0');
      else
	con->dd_printf("-");
    }
  con->dd_printf("\n");
  con->dd_printf("in : 0x%02x ", i);
  for (m= 0x80; m; m>>= 1)
    {
      //if (!(d & m))
	con->dd_printf("%c", (i & m)?'1':'0');
	//else
	//con->dd_printf("-");
    }
  con->dd_printf("\n");
  i= cell_cr1->get();
  con->dd_printf("cr1: 0x%02x ", i);
  for (m= 0x80; m; m>>= 1)
    con->dd_printf("%c", (i & m)?'1':'0');
  con->dd_printf("\n");
  i= cell_cr2->get();
  con->dd_printf("cr2: 0x%02x ", i);
  for (m= 0x80; m; m>>= 1)
    con->dd_printf("%c", (i & m)?'1':'0');
  con->dd_printf("\n");
  //print_cfg_info(con);
}


/* End of stm8.src/port.cc */
