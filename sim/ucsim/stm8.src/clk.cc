/*
 * Simulator of microcontrollers (stm8.src/clk.cc)
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

#include "utils.h"

#include "stm8cl.h"

#include "clkcl.h"


// FIXME: there needs to be some way to set the value of a cell ignoring
// the CELL_READ_ONLY flag.
static void
write_ro(class cl_memory_cell *cell, t_mem val)
{
  cell->set_flag(CELL_READ_ONLY, false);
  cell->write(val);
  cell->set_flag(CELL_READ_ONLY, true);
}


cl_clk::cl_clk(class cl_uc *auc):
       cl_hw(auc, HW_CLOCK, 0, "clk")
{
  f_LSI = 0;
  f_LSE = 0;

  CLK_SOURCE_HSI = 0;
  CLK_SOURCE_LSI = 0;
  CLK_SOURCE_HSE = 0;
  CLK_SOURCE_LSE = 0;

  base= 0x50C0;

  cmsr= NULL;
  swr= NULL;
  swcr= NULL;
  ckdivr= NULL;
  pckenr1= NULL;
  pckenr2= NULL;
  pckenr3= NULL;

  cpudiv = false;
}

int
cl_clk::init(void)
{
  cl_hw::init();

  uc->set_xtal(f_OSC = f_HSI);

  make_partner(HW_TIMER, 1);
  make_partner(HW_TIMER, 2);
  make_partner(HW_TIMER, 3);
  make_partner(HW_TIMER, 4);
  make_partner(HW_TIMER, 5);
  make_partner(HW_TIMER, 6);

  make_partner(HW_UART, 1);
  make_partner(HW_UART, 2);
  make_partner(HW_UART, 3);
  make_partner(HW_UART, 4);
  
  return 0;
}

int
cl_clk::clock_per_cycle(void)
{
  return 1;
}

int
cl_clk::xtal_per_clock(t_mem ckdivr_val)
{
  return (1 << (ckdivr_val & 0x03));
}

void
cl_clk::set_osc(t_mem osc)
{
  if (osc == CLK_SOURCE_HSI)
    {
      if (ickr)
        ickr->write((ickr->get() | CLK_ICKR_HSIRDY));
      write_ro(cmsr, CLK_SOURCE_HSI);
      f_OSC = f_HSI;
    }
  else if (osc == CLK_SOURCE_HSE)
    {
      if (eckr)
        eckr->write((eckr->get() | CLK_ECKR_HSERDY));
      write_ro(cmsr, CLK_SOURCE_HSE);
      f_OSC= uc->xtal_option->get_value(0.0);
    }
  else if (osc == CLK_SOURCE_LSI)
    {
      // FIXME: if LSI_EN option bit is set after SWEN is set and SWBSY is clear
      // should it trigger a clock change?
      class cl_stm8 *stm8 = static_cast<cl_stm8 *>(uc);
      if ((stm8->option_chip->get(0x05) & 0x08) && (~stm8->option_chip->get(0x06) & 0x08))
        {
          if (ickr)
            ickr->write((ickr->get() | CLK_ICKR_LSIRDY));
          write_ro(cmsr, CLK_SOURCE_LSI);
          f_OSC = f_LSI;
        }
    }
  else if (osc == CLK_SOURCE_LSE)
    {
      if (eckr)
        eckr->write((eckr->get() | CLK_ECKR_LSERDY));
      write_ro(cmsr, CLK_SOURCE_LSE);
      f_OSC = f_LSE;
    }

  uc->set_xtal((ckdivr ? f_OSC / xtal_per_clock(ckdivr->get()) : f_OSC));
}

void
cl_clk::write(class cl_memory_cell *cell, t_mem *val)
{
  if (ckdivr && cell == ckdivr)
    {
      // FIXME: this changes xtal before we tick. But what length should
      // the current tick be? Do we need to fudge the tickers here?
      uc->set_xtal(f_OSC / xtal_per_clock(*val));
    }
  else if (swr && cell == swr)
    {
      swcr->write(swcr->get() | CLK_SWCR_SWBSY);

      // If the new clock was not already active we should now wait for the oscillator
      // to stabilize (<1μs for HSI) then clear SWBSY (and do the change if SWEN is set)
      // however it is rare that oscillators are stopped and restarted (and not possible
      // for HSI according to the erratas) so we assume it is ready.
      // N.B. Manually clearing SWBSY cancels a pending oscillator change so this should
      // ONLY happen if SWBSY is still set. The check is redundant here but would be
      // required if we add a stabilization delay.
      t_mem v = swcr->get();
      if (v & CLK_SWCR_SWBSY)
        {
          if (v & CLK_SWCR_SWEN)
            set_osc(*val);
          swcr->write(v & ~CLK_SWCR_SWBSY);
        }
    }
  else if (swcr && cell == swcr)
    {
      // Setting SWEN when SWBSY is clear triggers a clock change
      if ((swcr->get() & (CLK_SWCR_SWEN|CLK_SWCR_SWBSY)) == 0x00 && (*val & (CLK_SWCR_SWEN|CLK_SWCR_SWBSY)) == CLK_SWCR_SWEN)
        set_osc(swr->get());
    }
  else if ((cell == pckenr1) ||
      (cell == pckenr2) ||
      (cell == pckenr3))
    {
      cl_clk_event e;
      hw_event ev;

      e.set(HW_TIMER, 1);
      ev= tim(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);

      e.id= 2;
      ev= tim(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);

      e.id= 3;
      ev= tim(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);

      e.id= 4;
      ev= tim(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);

      e.id= 5;
      ev= tim(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);

      e.id= 6;
      ev= tim(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);

      e.set(HW_UART, 1);
      ev= usart(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);

      e.id= 2;
      ev= usart(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);

      e.id= 3;
      ev= usart(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);

      e.id= 4;
      ev= usart(e.id, val)?EV_CLK_ON:EV_CLK_OFF;
      inform_partners(ev, &e);
    }
}

void
cl_clk::print_info(class cl_console_base *con)
{
  con->dd_printf("%s[%d] at 0x%06x\n", id_string, id, base);

  if (cmsr)
    {
      t_mem curr = cmsr->get();

      if (curr == CLK_SOURCE_HSI)
          con->dd_printf("  HSI");
      else if (curr == CLK_SOURCE_HSE)
          con->dd_printf("  HSE");
      else if (curr == CLK_SOURCE_LSI)
          con->dd_printf("  LSI");
      else if (curr == CLK_SOURCE_LSE)
          con->dd_printf("  LSE");

      t_mem want = swr->get();
      if (want != curr)
        {
          if (want == CLK_SOURCE_HSI)
            con->dd_printf(" (pending change to HSI)");
	  else if (want == CLK_SOURCE_HSE)
            con->dd_printf(" (pending change to HSE)");
	  else if (want == CLK_SOURCE_LSI)
            con->dd_printf(" (pending change to LSI)");
	  else if (want == CLK_SOURCE_LSE)
            con->dd_printf(" (pending change to LSE)");
        }

      con->dd_printf("\n");
    }

  con->dd_printf("  f_OSC:    %.0f Hz\n", f_OSC);

  if (ckdivr)
    {
      con->dd_printf("  HSIDIV:   %d\n", xtal_per_clock(ckdivr->get()));
      con->dd_printf("  f_MASTER: %.0f Hz\n", uc->get_xtal());
      if (cpudiv)
        con->dd_printf("  CPUDIV:   %d\n", clock_per_cycle());
      con->dd_printf("  f_CPU:    %.0f Hz\n", uc->get_xtal() / clock_per_cycle());
    }

  if (pckenr1)
    con->dd_printf("  PCKENR1:  0x%02x\n", pckenr1->get());
  if (pckenr2)
    con->dd_printf("  PCKENR2:  0x%02x\n", pckenr2->get());
  if (pckenr3)
    con->dd_printf("  PCKENR3:  0x%02x\n", pckenr3->get());
}


/* SAF */

cl_clk_saf::cl_clk_saf(class cl_uc *auc):
  cl_clk(auc)
{
  f_LSI = 128000;

  CLK_SOURCE_HSI = 0xe1;
  CLK_SOURCE_LSI = 0xd2;
  CLK_SOURCE_HSE = 0xb4;

  cpudiv = true;
}

int
cl_clk_saf::init(void)
{
  static const struct var_def vars[] = {
    { "ICKR",     var_offset(0), "Internal clock register" },
      { "REGAH",  var_bit(5), "Regulator power off in Active-halt mode" },
      { "LSIRDY", var_bit(4), "Low speed internal oscillator ready" },
      { "LSIEN",  var_bit(3), "Low speed internal RC oscillator enable" },
      { "FHWU",   var_bit(2), "Fast wakeup from Halt/Active-halt modes" },
      { "HSIRDY", var_bit(1), "High speed internal oscillator ready" },
      { "HSIEN",  var_bit(0), "High speed internal RC oscillator enable" },

    { "ECKR",        var_offset(1), "External clock register" },
      { "HSERDY", var_bit(1), "High speed external crystal oscillator ready" },
      { "HSEEN",  var_bit(0), "High speed external crystal oscillator enable" },

    { "CMSR", var_offset(3), "Clock master status register" },
      { "CKM", var_bitset(7,  0), "Clock master status bits" },

    { "SWR", var_offset(4), "Clock master switch register" },
      { "SWI", var_bitset(7,  0), "Clock master selection bits" },

    { "SWCR", var_offset(5), "Clock switch control register" },
      { "SWIF",  var_bit(3), "Clock switch interrupt flag" },
      { "SWIEN", var_bit(2), "Clock switch interrupt enable" },
      { "SWEN",  var_bit(1), "Switch start/stop" },
      { "SWBSY", var_bit(0), "Switch busy" },

    { "CKDIVR", var_offset(6), "Clock divider register" },
      { "HSIDIV", var_bitset(4,  3), "High speed internal clock prescalar" },
      { "CPUDIV", var_bitset(2,  0), "CPU clock prescalar" },

    { "PCKENR1", var_offset(7), "Peripheral clock gating register 1" },
      { "TIM1",     var_bit(7), "Peripheral clock enable TIM1" },
      { "TIM3",     var_bit(6), "Peripheral clock enable TIM3" },
      { "TIM25",    var_bit(5), "Peripheral clock enable TIM2/TIM5" },
      { "TIM46",    var_bit(4), "Peripheral clock enable TIM4/TIM6" },
      { "UART1234", var_bit(3), "Peripheral clock enable UART1/2/3/4" },
      { "UART1234", var_bit(2), "Peripheral clock enable UART1/2/3/4" },
      { "SPI",      var_bit(1), "Peripheral clock enable SPI" },
      { "I2C",      var_bit(0), "Peripheral clock enable I2C" },

    { "CSSR", var_offset(8), "Clock security system register" },
      { "CSSD",   var_bit(3), "Clock security system detection" },
      { "CSSDIE", var_bit(2), "Clock security system detection interrupt enable" },
      { "AUX",    var_bit(1), "Auxiliary oscillator connected to master clock" },
      { "CSSEN",  var_bit(0), "Clock security system enable" },

    { "CCOR", var_offset(9), "Configurable clock output register" },
      { "CCOBSY", var_bit(6), "Configurable clock output busy" },
      { "CCORDY", var_bit(5), "Configurable clock output ready" },
      { "CCOSEL", var_bit(4), "Configurable clock output selection" },
      { "CCOEN",  var_bit(0), "Configurable clock output enable" },

    { "PCKENR2", var_offset(10), "Peripheral clock gating register 2" },
      { "CAN", var_bit(7), "Peripheral clock enable CAN" },
      { "ADC", var_bit(3), "Peripheral clock enable ADC" },
      { "AWU", var_bit(2), "Peripheral clock enable AWU" },

    { "HSITRIMR", var_offset(12), "HSI clock calibration trimming register" },
      { "HSITRIM", var_bitset(3,  0), "HSI trimming value" },

    { "SWIMCCR", var_offset(13), "SWIM clock control register" },
      { "SWIMCLK", var_bit(0), "SWIM clock divider" },
  };

  cl_clk::init();

  uc->vars->add(chars(get_name()).uppercase() + "_", uc->rom, base, vars, sizeof(vars)/sizeof(vars[0]));

  ickr = register_cell(uc->rom, base+0);
  eckr = register_cell(uc->rom, base+1);
  cmsr = register_cell(uc->rom, base+3);
  cmsr->set_flag(CELL_READ_ONLY, true);
  swr = register_cell(uc->rom, base+4);
  swcr = register_cell(uc->rom, base+5);
  ckdivr = register_cell(uc->rom, base+6);
  pckenr1 = register_cell(uc->rom, base+7);
  // cssr = register_cell(uc->rom, base+8);
  // ccor = register_cell(uc->rom, base+9);
  pckenr2 = register_cell(uc->rom, base+10);
  // hsitrimr = register_cell(uc->rom, base+12);
  // swimccr = register_cell(uc->rom, base+13);

  return 0;
}

int
cl_clk_saf::clock_per_cycle(void)
{
  return (1 << (ckdivr->get() & 0x07));
}

int
cl_clk_saf::xtal_per_clock(t_mem ckdivr_val)
{
  if (cmsr->get() == CLK_SOURCE_HSI)
    return (1 << ((ckdivr_val >> 3) & 0x03));
  else
    return 1;
}

void
cl_clk_saf::reset(void)
{
  ickr->write(0x01);
  eckr->write(0x00);
  ckdivr->write(0x18);
  swr->write(CLK_SOURCE_HSI);
  swcr->write(0x00);
  pckenr1->write(0xff);
  uc->rom->write(base+8, 0x00); //cssr->write(0x00);
  uc->rom->write(base+9, 0x00); //ccor->write(0x00);
  pckenr2->write(0xff);
  uc->rom->write(base+12, 0x00); //hsitrimr->write(0x00);
  uc->rom->write(base+13, 0x00); //swimccr->write(0x00);

  set_osc(CLK_SOURCE_HSI);
}

void
cl_clk_saf::write(class cl_memory_cell *cell, t_mem *val)
{
  if (cell == eckr)
    {
      t_mem preserve = 0xfc;

      // RM0016: 9.9.2: HSERDY: This bit is set and cleared by hardware.
      preserve |= CLK_ECKR_HSERDY;

      // RM0016 9.9.2: It [HSEEN] cannot be cleared when HSE is selected as clock
      // master (CLK_CMSR register) or as the active CCO source.
      // FIXME: CCO is not yet implemented.
      if (cmsr->get() == CLK_SOURCE_HSE)
        preserve |= CLK_ECKR_HSEEN;

      *val = (*val & (~preserve)) | (eckr->get() & preserve);
    }
  else if (cell == ickr)
    {
      t_mem preserve = 0;

      // RM0016: 9.9.1: HSIRDY: This bit is set and cleared by hardware.
      // RM0016: 9.9.1: LSIRDY: This bit is set and cleared by hardware.
      preserve = CLK_ICKR_HSIRDY | CLK_ICKR_LSIRDY;

      // RM0016 9.9.1: It [LSIEN] cannot be cleared when LSI is selected as clock
      // master (CLK_CMSR register), as the active CCO source or as a clock source
      // for the AWU peripheral or independent Watchdog.
      // FIXME: CCO, AWU and IWDG are not yet implemented.
      if (cmsr->get() == CLK_SOURCE_LSI)
        preserve |= CLK_ICKR_LSIEN;

      // RM0016 9.9.1: It [HSIEN] cannot be cleared when HSI is selected as clock
      // master (CLK_CMSR register), as the active CCO source or if the safe oscillator
      // (AUX) is enabled.
      // FIXME: CCO and AUX are not yet implemented.
      if (cmsr->get() == CLK_SOURCE_HSI)
        preserve |= CLK_ICKR_HSIEN;

      *val = (*val & (~preserve)) | (ickr->get() & preserve);
    }
  else if (cell == swr)
    {
      if (*val == CLK_SOURCE_LSI)
        {
          // RM0016 9.9.1: It [LSIEN] is set by hardware whenever the LSI oscillator is
          // required, for example: [...] When switching to LSI clock (see CLK_SWR register).
          ickr->write(ickr->get() | CLK_ICKR_LSIEN);
        }
      if (*val == CLK_SOURCE_HSI)
        {
          // RM0016 9.9.1: It [HSIEN] is set by hardware whenever the HSI oscillator is
          // required, for example: [...] When switching to HSI clock (see CLK_SWR register).
          ickr->write(ickr->get() | CLK_ICKR_HSIEN);
        }
    }

  cl_clk::write(cell, val);
}

bool
cl_clk_saf::tim(int id, t_mem *val)
{
  switch (id)
    {
    case 1:
      return pckenr1 && (*val & 0x80);
    case 2: case 5:
      return pckenr1 && (*val & 0x20);
    case 3:
      return pckenr1 && (*val & 0x40);
    case 4: case 6:
      return pckenr1 && (*val & 0x10);
    }
  return false;
}

bool
cl_clk_saf::usart(int id, t_mem *val)
{
  cl_stm8 *u= (cl_stm8 *)uc;
  if (id == 1)
    switch (u->type->subtype)
      {
      case DEV_STM8S003: case DEV_STM8S103: case DEV_STM8S903:
	return pckenr1 && (*val & 0x08);
      case DEV_STM8S007: case DEV_STM8S207: case DEV_STM8S208:
      case DEV_STM8AF52:
	return pckenr1 && (*val & 0x04);
      }
  else if (id == 2)
    switch (u->type->subtype)
      {
      case DEV_STM8S005: case DEV_STM8S105: case DEV_STM8AF62_46:
	return pckenr1 && (*val & 0x08);
      }
  else if (id == 3)
    switch (u->type->subtype)
      {
      case DEV_STM8S007: case DEV_STM8S207: case DEV_STM8S208:
      case DEV_STM8AF52:
	return pckenr1 && (*val & 0x08);
      }
  else if (id == 4)
    switch (u->type->subtype)
      {
      case DEV_STM8AF62_12:
	return pckenr1 && (*val & 0x08);
      }
  return false;
}

/* ALL */

cl_clk_all::cl_clk_all(class cl_uc *auc):
  cl_clk(auc)
{
  f_LSI = 38000;
  f_LSE = 32768;

  CLK_SOURCE_HSI = 0x01;
  CLK_SOURCE_LSI = 0x02;
  CLK_SOURCE_HSE = 0x04;
  CLK_SOURCE_LSE = 0x08;
}

int
cl_clk_all::init(void)
{
  static const struct var_def vars[] = {
    { "CKDIVR",     var_offset(0), "Clock divider register" },
      { "CKM", var_bitset(2,  0), "System clock prescalar" },

    { "CRTCR",          var_offset(1), "Clock RTC register" },
      { "RTCDIV",   var_bitset(7,  5), "Clock RTC prescalar" },
      { "RTCSEL",   var_bitset(4,  1), "Configurable RTC clock source selection" },
      { "RTCSWBSY", var_bit(0),         "RTC clock busy" },

    { "ICKCR",          var_offset(2), "Internal clock register" },
      { "BEEPHALT", var_bit(6), "BEEP clock Halt/Active-halt mode" },
      { "FHWU",     var_bit(5), "Fast wakeup from Halt/Active-halt modes" },
      { "SAHALT",   var_bit(4), "Slow Halt/Active-halt mode" },
      { "LSIRDY",   var_bit(3), "Low speed internal oscillator ready" },
      { "LSION",    var_bit(2), "Low speed internal oscillator enable" },
      { "HSIRDY",   var_bit(1), "High speed internal oscillator ready" },
      { "HSION",    var_bit(0), "High speed internal oscillator enable" },

    { "PCKENR1", var_offset(3), "Peripheral clock gating register 1" },
      { "DAC",    var_bit(7), "Peripheral clock enable DAC" },
      { "BEEP",   var_bit(6), "Peripheral clock enable BEEP" },
      { "USART1", var_bit(5), "Peripheral clock enable USART1" },
      { "SPI",    var_bit(4), "Peripheral clock enable SPI1" },
      { "I2C1",   var_bit(3), "Peripheral clock enable I2C1" },
      { "TIM4",   var_bit(2), "Peripheral clock enable TIM4" },
      { "TIM3",   var_bit(1), "Peripheral clock enable TIM3" },
      { "TIM2",   var_bit(0), "Peripheral clock enable TIM2" },

    { "PCKENR2", var_offset(4), "Peripheral clock gating register 2" },
      { "BOOTROM", var_bit(7), "Peripheral clock enable boot ROM" },
      { "COMP12",  var_bit(5), "Peripheral clock enable COMP1 and COMP2" },
      { "DMA1",    var_bit(4), "Peripheral clock enable DMA1" },
      { "LCD",     var_bit(3), "Peripheral clock enable LCD" },
      { "RTC",     var_bit(2), "Peripheral clock enable RTC" },
      { "TIM1",    var_bit(1), "Peripheral clock enable TIM1" },
      { "ADC1",    var_bit(0), "Peripheral clock enable ADC1" },

    { "CCOR", var_offset(5), "Configurable clock output register" },
      { "CCODIV",   var_bitset(7, 5), "Configurable clock output prescalar" },
      { "CCOSEL",   var_bitset(4, 1), "Configurable clock output selection" },
      { "CCOSWBSY", var_bit(0), "Configurable clock output switch busy" },

    { "ECKCR", var_offset(6), "External clock register" },
      { "LSEBYP", var_bit(5), "Low speed external clock bypass" },
      { "HSEBYP", var_bit(4), "High speed external clock bypass" },
      { "LSERDY", var_bit(3), "Low speed external crystal oscillator ready" },
      { "LSEON",  var_bit(2), "Low speed external crystal oscillator enable" },
      { "HSERDY", var_bit(1), "High speed external crystal oscillator ready" },
      { "HSEON",  var_bit(0), "High speed external crystal oscillator enable" },

    { "SCSR", var_offset(7), "System clock status register" },
      { "CKM", var_bitset(7, 0), "System clock status bits" },

    { "SWR", var_offset(8), "System clock switch register" },
      { "SWI", var_bitset(7, 0), "System clock selection bits" },

    { "SWCR", var_offset(9), "Switch control register" },
      { "SWIF",  var_bit(3), "Clock switch interrupt flag" },
      { "SWIEN", var_bit(2), "Clock switch interrupt enable" },
      { "SWEN",  var_bit(1), "Switch start/stop" },
      { "SWBSY", var_bit(0), "Switch busy" },

    { "CSSR", var_offset(10), "Clock security system register" },
      { "CSSDGON", var_bit(4), "Clock security system deglitcher system" },
      { "CSSD",    var_bit(3), "Clock security system detection" },
      { "CSSDIE",  var_bit(2), "Clock security system detection interrupt enable" },
      { "AUX",     var_bit(1), "Auxiliary oscillator connected to master clock" },
      { "CSSEN",   var_bit(0), "Clock security system enable" },

    { "CBEEPR", var_offset(11), "Clock BEEP register" },
      { "CLKBEEPSEL", var_bitset(2, 1), "Configurable BEEP clock source selection" },
      { "BEEPSWBSY",  var_bit(0), "System busy during BEEP clock change" },

    { "HSICALR", var_offset(12), "HSI calibration register" },
      { "HSICAL", var_bitset(7, 0), "HSI calibration" },

    { "HSITRIMR", var_offset(13), "HSI clock calibration trimming register" },
      { "HSITRIM", var_bitset(7, 0), "HSI trimming value" },

    { "HSIUNLCKR", var_offset(14), "HSI unlock register" },
      { "HSIUNLCK", var_bitset(7, 0), "HSI unlock mechanism" },

    { "REGCSR", var_offset(15), "Main regulator control status register" },
      { "EEREADY",  var_bit(7), "Flash program memory and data EEPROM ready" },
      { "EEBUSY",   var_bit(6), "Flash program memory and data EEPROM busy" },
      { "LSEPD",    var_bit(5), "LSE power-down" },
      { "HSEPD",    var_bit(4), "HSE power-down" },
      { "LSIPD",    var_bit(3), "LSI power-down" },
      { "HSIPD",    var_bit(2), "HSI power-down" },
      { "REGOFF",   var_bit(1), "Main regulator OFF" },
      { "REGREADY", var_bit(0), "Main regulator ready" },

    { "PCKENR3", var_offset(16), "Peripheral clock gating register 3" },
      { "CSS_LSE", var_bit(5), "Peripheral clock enable CSS_LSE" },
      { "USART3",  var_bit(4), "Peripheral clock enable USART3" },
      { "USART2",  var_bit(3), "Peripheral clock enable USART2" },
      { "SPI2",    var_bit(2), "Peripheral clock enable SPI2" },
      { "TIM5",    var_bit(1), "Peripheral clock enable TIM5" },
      { "AES",     var_bit(0), "Peripheral clock enable AES" },
  };

  cl_clk::init();

  uc->vars->add(chars(get_name()).uppercase() + "_", uc->rom, base, vars, sizeof(vars)/sizeof(vars[0]));

  ckdivr = register_cell(uc->rom, base+0);
  //crtcr = register_cell(uc->rom, base+1);
  ickr = register_cell(uc->rom, base+2);
  pckenr1 = register_cell(uc->rom, base+3);
  pckenr2 = register_cell(uc->rom, base+4);
  //ccor = register_cell(uc->rom, base+5);
  eckr = register_cell(uc->rom, base+6);
  cmsr = register_cell(uc->rom, base+7);
  uc->rom->set_cell_flag(base+7, true, CELL_READ_ONLY);
  swr = register_cell(uc->rom, base+8);
  swcr = register_cell(uc->rom, base+9);
  //cssr = register_cell(uc->rom, base+10);
  //cbeepr = register_cell(uc->rom, base+11);
  //hsicalr = register_cell(uc->rom, base+12);
  //hsitrimr = register_cell(uc->rom, base+13);
  //hsiunlckr = register_cell(uc->rom, base+14);
  //regcsr = register_cell(uc->rom, base+15);
  pckenr3 = register_cell(uc->rom, base+16);

  return 0;
}

int
cl_clk_all::xtal_per_clock(t_mem ckdivr_val)
{
  return (1 << (ckdivr_val & 0x07));
}

void
cl_clk_all::reset(void)
{
  ckdivr->write(0x03);
  uc->rom->write(base+1, 0x00); //crtcr->write(0x00);
  ickr->write(0x11);
  uc->rom->write(base+5, 0x00); //ccor->write(0x00);
  eckr->write(0x00);
  swr->write(CLK_SOURCE_HSI);
  swcr->write(0x00);
  uc->rom->write(base+10, 0x00); //cssr->write(0x00);
  uc->rom->write(base+11, 0x00); //cbeepr->write(0x00);
  uc->rom->write(base+12, 0x00); //hsicalr->write(0x00); // Actually a factory calibration value...
  uc->rom->write(base+13, 0x00); //hsitrimr->write(0x00);
  uc->rom->write(base+14, 0x00); //hsiunlckr->write(0x00);
  // FIXME: CLKREGCSR should reset to 0xb9 but that means LSE, HSE and LSI are powered down.
  // Since we don't emulate the auto-power up behaviour and reflect that in CLKREGCSR
  // we had best pretend they are always powered up.
  uc->rom->write(base+15, 0x81); //clkregcsr->write(0x81);
  pckenr1->write(0x00);
  pckenr2->write(0x80);
  pckenr3->write(0x00);

  set_osc(CLK_SOURCE_HSI);
}

void
cl_clk_all::write(class cl_memory_cell *cell, t_mem *val)
{
  if (cell == eckr)
    {
      t_mem preserve = 0xc0;

      // RM0031: 9.14.8: HSERDY: This bit is set and cleared by hardware.
      preserve |= CLK_ECKR_HSERDY;

      t_mem clk = cmsr->get();
      if (clk == CLK_SOURCE_HSE)
        {
          // RM0031 9.14.8: It [HSEON] cannot be cleared when HSE is selected as system
          // clock (CLK_SCSR register) or as the active CCO source or as active RTC
          // clock source.
          // FIXME: CCO and RTC are not yet implemented.
          preserve |= CLK_ECKR_HSEEN;
        }
      else if (clk == CLK_SOURCE_LSE)
        {
          // RM0031 9.14.8: It [LSEON] cannot be cleared when LSE is selected as system
          // clock (CLK_SCSR register), as active CCO source, as clock source for the
          // BEEP peripheral and BEEPAHALT is set or as active RTC.
          // FIXME: CCO, BEEP and RTC are not yet implemented.
          preserve |= CLK_ECKR_LSEEN;
        }

      *val = (*val & (~preserve)) | (eckr->get() & preserve);
    }
  else if (cell == ickr)
    {
      t_mem preserve = 0;

      // RM0031: 9.14.3: HSIRDY: This bit is set and cleared by hardware.
      // RM0031: 9.14.3: LSIRDY: This bit is set and cleared by hardware.
      preserve = CLK_ICKR_HSIRDY | CLK_ICKR_LSIRDY;

      t_mem clk = cmsr->get();
      if (clk == CLK_SOURCE_LSI)
        {
          // RM0031 9.14.3: It [LSION] cannot be cleared when LSI is selected as clock
          // master (CLK_SCSR register), as active CCO source, as clock source for the
          // BEEP peripheral while BEEPAHALT is set or as active clock source for RTC.
          // FIXME: CCO, BEEP and RTC are not yet implemented.
          preserve |= CLK_ICKR_LSIEN;
        }
      else if (clk == CLK_SOURCE_HSI)
        {
          // RM0031 9.14.3: It [HSIEN] cannot be cleared when HSI is selected as clock
          // master (CLK_CMSR register), as the active CCO source or if the safe oscillator
          // (AUX) is enabled.
          // FIXME: CCO and AUX are not yet implemented.
          preserve |= CLK_ICKR_HSIEN;
        }

      *val = (*val & (~preserve)) | (ickr->get() & preserve);
    }
  else if (cell == swr)
    {
      if (*val == CLK_SOURCE_LSE)
        {
          // RM0031 9.14.8: It [LSEON] is set by hardware whenever the LSE oscillator is
          // required, for example: [...] When switching to LSI clock (see CLK_SWR register).
          eckr->write(eckr->get() | CLK_ECKR_LSEEN);
        }
      else if (*val == CLK_SOURCE_HSE)
        {
          // RM0031 9.14.8: It [HSEON] is set by hardware whenever the HSE oscillator is
          // required, for example: [...] When switching to HSI clock (see CLK_SWR register).
          eckr->write(eckr->get() | CLK_ECKR_HSEEN);
        }
      else if (*val == CLK_SOURCE_LSI)
        {
          // RM0031 9.14.3: It [LSION] is set by hardware whenever the LSI oscillator is
          // required, for example: [...] When switching to LSI clock (see CLK_SWR register).
          ickr->write(ickr->get() | CLK_ICKR_LSIEN);
        }
      else if (*val == CLK_SOURCE_HSI)
        {
          // RM0031 9.14.3: It [HSION] is set by hardware whenever the HSI oscillator is
          // required, for example: [...] When switching to HSI clock (see CLK_SWR register).
          ickr->write(ickr->get() | CLK_ICKR_HSIEN);
        }
    }

  cl_clk::write(cell, val);
}

bool
cl_clk_all::tim(int id, t_mem *val)
{
  switch (id)
    {
    case 1:
      return pckenr2 && (*val & 0x02);
    case 2:
      return pckenr1 && (*val & 0x01);
    case 3:
      return pckenr1 && (*val & 0x02);
    case 4:
      return pckenr1 && (*val & 0x04);
    case 5:
      return pckenr3 && (*val & 0x02);
    }
  return false;
}

bool
cl_clk_all::usart(int id, t_mem *val)
{
  switch (id)
    {
    case 1:
      return pckenr1 && (*val & 0x20);
    case 2:
      return pckenr3 && (*val & 0x08);
    case 3:
      return pckenr3 && (*val & 0x10);
    }
  return false;
}


/* L101 */

cl_clk_l101::cl_clk_l101(class cl_uc *auc):
  cl_clk(auc)
{
}

int
cl_clk_l101::init(void)
{
  static const struct var_def vars[] = {
    { "CKDIVR", var_offset(0), "Clock divider register" },
      { "HSIDIV", var_bitset(1, 0), "High speed internal clock prescalar" },

    { "PCKENR", var_offset(3), "Peripheral clock gating register" },
      { "AWU_BEEP", var_bit(6), "Peripheral clock enable AWU+BEEP" },
      { "USART",    var_bit(5), "Peripheral clock enable USART" },
      { "SPI",      var_bit(4), "Peripheral clock enable SPI" },
      { "I2C",      var_bit(3), "Peripheral clock enable I2C" },
      { "TIM4",     var_bit(2), "Peripheral clock enable TIM4" },
      { "TIM3",     var_bit(1), "Peripheral clock enable TIM3" },
      { "TIM2",     var_bit(0), "Peripheral clock enable TIM2" },

    { "CCOR", var_offset(5), "Configurable clock output register" },
      { "CCOSEL", var_bitset(2, 1), "Configurable clock output selection" },
      { "CCOEN",  var_bit(0), "Configurable clock output enable" },
  };

  cl_clk::init();

  uc->vars->add(chars(get_name()).uppercase() + "_", uc->rom, base, vars, sizeof(vars)/sizeof(vars[0]));

  ckdivr = register_cell(uc->rom, base+0);
  pckenr1 = register_cell(uc->rom, base+3);
  //ccor = register_cell(uc->rom, base+5);

  return 0;
}

int
cl_clk_l101::xtal_per_clock(t_mem ckdivr_val)
{
  return (1 << (ckdivr_val & 0x03));
}

void
cl_clk_l101::reset(void)
{
  ckdivr->write(0x03);
  pckenr1->write(0x00);
  uc->rom->write(base+5, 0x00); //ccor->write(0x00);
}

bool
cl_clk_l101::tim(int id, t_mem *val)
{
  switch (id)
    {
    case 2:
      return pckenr1 && (*val & 0x01);
    case 3:
      return pckenr1 && (*val & 0x02);
    case 4:
      return pckenr1 && (*val & 0x04);
    }
  return false;
}

bool
cl_clk_l101::usart(int id, t_mem *val)
{
  switch (id)
    {
    case 1:
      return pckenr1 && (*val & 0x20);
    }
  return false;
}


/* End of stm8.src/clk.cc */
