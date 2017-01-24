/*
 * Simulator of microcontrollers (uc51.cc)
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
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

#include "ddconfig.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
//#if FD_HEADER_OK
//# include HEADER_FD
//#endif
#include "i_string.h"

// prj
#include "utils.h"
#include "globals.h"

// sim
#include "optioncl.h"

//cmd.src
#include "cmd_uccl.h"

// local
#include "uc51cl.h"
#include "glob.h"
#include "regs51.h"
#include "timer0cl.h"
#include "timer1cl.h"
#include "serialcl.h"
#include "portcl.h"
#include "interruptcl.h"
#include "types51.h"


/*
 * Options of uc51
 */

cl_irq_stop_option::cl_irq_stop_option(class cl_51core *the_uc51):
  cl_optref(the_uc51)
{
  uc51= the_uc51;
}

int
cl_irq_stop_option::init(void)
{
  cl_optref::init();
  create(uc51, bool_opt, "irq_stop", "Stop when IRQ accepted");
  return(0);
}

void
cl_irq_stop_option::option_changed(void)
{
  if (!uc51)
    return;
  bool b;
  option->get_value(&b);
  uc51->stop_at_it= b;
}


/*
 * Making a new micro-controller and reset it
 */

cl_51core::cl_51core(struct cpu_entry *Itype, class cl_sim *asim):
  cl_uc(asim)
{
  type= Itype;

  irq_stop_option= new cl_irq_stop_option(this);
  stop_at_it= false;
}


/*
 * Initializing. Virtual calls go here
 * This method must be called first after object creation.
 */

int
cl_51core::init(void)
{
  irq_stop_option->init();
  dptr= 0;
  cl_uc::init();
  decode_dptr();
  set_name("mcs51_controller");
  reset();

  make_vars();

  return(0);
}

static char id_string_51[100];

char *
cl_51core::id_string(void)
{
  int i;

  for (i= 0;
       (cpus_51[i].type_str != NULL) &&
	 (cpus_51[i].type != type->type);
       i++) ;
  sprintf(id_string_51, "%s %s",
	  cpus_51[i].type_str?cpus_51[i].type_str:"51",
	  (type->subtype & CPU_HMOS)?"HMOS":"CMOS");
  return(id_string_51);
}

void
cl_51core::make_cpu_hw(void)
{
  cpu= new cl_uc51_cpu(this);
  cpu->init();
}

void
cl_51core::mk_hw_elements(void)
{
  cl_uc::mk_hw_elements();

  class cl_hw *h;

  acc= sfr->get_cell(ACC);
  psw= sfr->get_cell(PSW);

  add_hw(h= new cl_timer0(this, 0, "timer0"));
  h->init();
  add_hw(h= new cl_timer1(this, 1, "timer1"));
  h->init();
  add_hw(h= new cl_serial(this));
  h->init();

  class cl_port_ui *d;
  add_hw(d= new cl_port_ui(this, 0, "dport"));
  d->init();

  class cl_port *p0, *p1, *p2, *p3;
  add_hw(p0= new cl_port(this, 0));
  p0->init();
  add_hw(p1= new cl_port(this, 1));
  p1->init();
  add_hw(p2= new cl_port(this, 2));
  p2->init();
  add_hw(p3= new cl_port(this, 3));
  p3->init();

  class cl_port_data pd;
  pd.init();
  pd.cell_dir= NULL;

  pd.set_name("P0");
  pd.cell_p  = p0->cell_p;
  pd.cell_in = p0->cell_in;
  pd.keyset  = chars(keysets[0]);
  pd.basx    = 1;
  pd.basy    = 4;
  d->add_port(&pd, 0);
  
  pd.set_name("P1");
  pd.cell_p  = p1->cell_p;
  pd.cell_in = p1->cell_in;
  pd.keyset  = chars(keysets[1]);
  pd.basx    = 20;
  pd.basy    = 4;
  d->add_port(&pd, 1);
  
  pd.set_name("P2");
  pd.cell_p  = p2->cell_p;
  pd.cell_in = p2->cell_in;
  pd.keyset  = chars(keysets[2]);
  pd.basx    = 40;
  pd.basy    = 4;
  d->add_port(&pd, 2);
  
  pd.set_name("P3");
  pd.cell_p  = p3->cell_p;
  pd.cell_in = p3->cell_in;
  pd.keyset  = chars(keysets[3]);
  pd.basx    = 60;
  pd.basy    = 4;
  d->add_port(&pd, 3);
  
  add_hw(interrupt= new cl_interrupt(this));
  interrupt->init();
}

void
cl_51core::build_cmdset(class cl_cmdset *cmdset)
{
  class cl_cmd *cmd;
  //class cl_super_cmd *super_cmd;
  //class cl_cmdset *cset;

  cl_uc::build_cmdset(cmdset);

  cmdset->add(cmd= new cl_di_cmd("di", true,
"di [start [stop]]  Dump Internal RAM",
"long help of di"));
  cmd->init();

  cmdset->add(cmd= new cl_dx_cmd("dx", true,
"dx [start [stop]]  Dump External RAM",
"long help of dx"));
  cmd->init();

  cmdset->add(cmd= new cl_ds_cmd("ds", true,
"ds [start [stop]]  Dump SFR",
"long help of ds"));
  cmd->init();
}


void
cl_51core::make_memories(void)
{
  make_address_spaces();
  make_chips();
  
  acc= sfr->get_cell(ACC);
  psw= sfr->get_cell(PSW);

  decode_regs();
  decode_rom();
  decode_iram();
  decode_sfr();
  decode_xram();
  decode_bits();
}

void
cl_51core::make_address_spaces(void)
{
  rom= new cl_address_space("rom", 0, 0x10000, 8);
  rom->init();
  address_spaces->add(rom);
  
  iram= new cl_address_space("iram", 0, 0x80, 8);
  iram->init();
  address_spaces->add(iram);

  sfr= new cl_address_space("sfr", 0x80, 0x80, 8);
  sfr->init();
  address_spaces->add(sfr);

  xram= new cl_address_space("xram", 0, 0x10000, 8);
  xram->init();
  address_spaces->add(xram);

  regs= new cl_address_space("regs", 0, 8, 8);
  regs->init();
  address_spaces->add(regs);

  bits= new cl_address_space("bits", 0, 0x100, 1);
  bits->init();
  address_spaces->add(bits);

  dptr= new cl_address_space("dptr", 0, 2, 8);
  dptr->init();
  address_spaces->add(dptr);
}

void
cl_51core::make_chips(void)
{
  rom_chip= new cl_memory_chip("rom_chip", 0x10000, 8, 0/*, 0xff*/);
  rom_chip->init();
  memchips->add(rom_chip);
  
  iram_chip= new cl_memory_chip("iram_chip", 0x100, 8);
  iram_chip->init();
  memchips->add(iram_chip);

  xram_chip= new cl_memory_chip("xram_chip", 0x10000, 8);
  xram_chip->init();
  memchips->add(xram_chip);

  sfr_chip= new cl_memory_chip("sfr_chip", 0x80, 8);
  sfr_chip->init();
  memchips->add(sfr_chip);
}

void
cl_51core::decode_rom(void)
{
  class cl_address_decoder *ad;
  ad= new cl_address_decoder(rom, rom_chip, 0, 0xffff, 0);
  ad->init();
  rom->decoders->add(ad);
  ad->activate(0);
}

void
cl_51core::decode_regs(void)
{
  int i;
  cl_banker *b= new cl_banker(sfr, 0xd0, 0x18,
			      regs, 0, 7);
  b->init();
  regs->decoders->add(b);
  b->add_bank(0, memory("iram_chip"), 0);
  b->add_bank(1, memory("iram_chip"), 8);
  b->add_bank(2, memory("iram_chip"), 16);
  b->add_bank(3, memory("iram_chip"), 24);
  psw->write(0);
  for (i= 0; i < 8; i++)
    R[i]= regs->get_cell(i);
}

void
cl_51core::decode_bits(void)
{
  class cl_address_decoder *ad;
  
  ad= new cl_bander(bits, 0, 127,
		    iram_chip, 32,
		    8, 1);
  ad->init();
  bits->decoders->add(ad);
  ad->activate(0);

  ad= new cl_bander(bits, 128, 255,
		    sfr_chip, 0,
		    8, 8);
  ad->init();
  bits->decoders->add(ad);
  ad->activate(0);
}

void
cl_51core::decode_iram(void)
{
  class cl_address_decoder *ad;
  
  ad= new cl_address_decoder(iram, iram_chip, 0, 0x7f, 0);
  ad->init();
  iram->decoders->add(ad);
  ad->activate(0);
}

void
cl_51core::decode_sfr(void)
{
  class cl_address_decoder *ad;
  
  ad= new cl_address_decoder(sfr, sfr_chip, 0x80, 0xff, 0);
  ad->init();
  sfr->decoders->add(ad);
  ad->activate(0);
}

void
cl_51core::decode_xram(void)
{
  class cl_address_decoder *ad;
  
  ad= new cl_address_decoder(xram, xram_chip, 0, 0xffff, 0);
  ad->init();
  xram->decoders->add(ad);
  ad->activate(0);
}

void
cl_51core::decode_dptr(void)
{
  class cl_address_decoder *ad;
  t_mem adps= 0, mdps, dpl1, dph1, mdpc, adpc;
  class cl_banker *banker;

  dptr->undecode_area(NULL, 0, 1, NULL);
  
  if (cpu)
    {
      adps= cpu->cfg_get(uc51cpu_aof_mdps);
      mdps= cpu->cfg_get(uc51cpu_mask_mdps);
      dpl1= cpu->cfg_get(uc51cpu_aof_mdps1l);
      dph1= cpu->cfg_get(uc51cpu_aof_mdps1h);

      adpc= cpu->cfg_get(uc51cpu_aof_mdpc);
      mdpc= cpu->cfg_get(uc51cpu_mask_mdpc);
      
      if ((adps > 0x7f) &&
	  (dpl1 > 0x7f) &&
	  (dph1 > 0x7f))
	{
	  // multi DPTR sfr style
	  //printf("MDPS %x %x %x %x\n", adps, mdps, dpl1, dph1);
	  banker= new cl_banker(sfr, adps, mdps,
				dptr, 0, 0);
	  banker->init();
	  dptr->decoders->add(banker);
	  banker->add_bank(0, memory("sfr_chip"), DPL-0x80);
	  banker->add_bank(1, memory("sfr_chip"), dpl1-0x80);
	  banker->activate(0);

	  banker= new cl_banker(sfr, adps, mdps,
				dptr, 1, 1);
	  banker->init();
	  dptr->decoders->add(banker);
	  banker->add_bank(0, memory("sfr_chip"), DPH-0x80);
	  banker->add_bank(1, memory("sfr_chip"), dph1-0x80);
	  banker->activate(0);

	  sfr->write(adps, sfr->get(adps));
	}
      else if (adpc > 0x7f)
	{
	  // multi DPTR chip style
	  adps=0x80;
	  class cl_memory_chip *dptr_chip= (cl_memory_chip*)memory("dptr_chip");
	  if (dptr_chip == 0)
	    {
	      dptr_chip= new cl_memory_chip("dptr_chip", 3*8, 8);
	      dptr_chip->init();
	      memchips->add(dptr_chip);
	    }
	  if (dptr_chip &&
	      (mdpc != 0))
	    {
	      int a, m= mdpc;
	      //printf("MDPC %x %x\n", adpc, mdpc);
	      while ((m&1) == 0)
		m>>= 1;
	      
	      banker= new cl_banker(sfr, adpc, mdpc,
			    dptr, 0, 1);
	      banker->init();
	      dptr->decoders->add(banker);
	      for (a= 0; a <= m; a++)
		banker->add_bank(a, dptr_chip, a*2);
	      banker->activate(0);

	      banker= new cl_banker(sfr, adpc, mdpc,
			    sfr, DPL, DPH);
	      banker->init();
	      sfr->decoders->add(banker);
	      for (a= 0; a <= m; a++)
		banker->add_bank(a, dptr_chip, a*2);
	      banker->activate(0);
	      
	      sfr->write(adpc, sfr->get(adpc));
	    }
	}
      else
	adps= 0;
    }
  if (adps == 0)
    {
      //printf("DPTR\n");
      ad= new cl_address_decoder(dptr, sfr_chip, 0, 1, DPL-0x80);
      ad->init();
      dptr->decoders->add(ad);
      ad->activate(0);
    }
  
  cl_var *v;
  vars->add(v= new cl_var(chars("dpl"), dptr, 0));
  v->init();
  vars->add(v= new cl_var(chars("DPL"), dptr, 0));
  v->init();
  vars->add(v= new cl_var(chars("dph"), dptr, 1));
  v->init();
  vars->add(v= new cl_var(chars("DPH"), dptr, 1));
  v->init();
}

void
cl_51core::make_vars(void)
{
  cl_var *v;

  vars->add(v= new cl_var(cchars("R0"), regs, 0));
  v->init();
  vars->add(v= new cl_var(cchars("R1"), regs, 1));
  v->init();
  vars->add(v= new cl_var(cchars("R2"), regs, 2));
  v->init();
  vars->add(v= new cl_var(cchars("R3"), regs, 3));
  v->init();
  vars->add(v= new cl_var(cchars("R4"), regs, 4));
  v->init();
  vars->add(v= new cl_var(cchars("R5"), regs, 5));
  v->init();
  vars->add(v= new cl_var(cchars("R6"), regs, 6));
  v->init();
  vars->add(v= new cl_var(cchars("R7"), regs, 7));
  v->init();
}

/*
 * Destroying the micro-controller object
 */

cl_51core::~cl_51core(void)
{
  delete irq_stop_option;
}


/*
 * Disassembling an instruction
 */

struct dis_entry *
cl_51core::dis_tbl(void)
{
  return(disass_51);
}

struct name_entry *
cl_51core::sfr_tbl(void)
{
  return(sfr_tab51);
}

struct name_entry *
cl_51core::bit_tbl(void)
{
  return(bit_tab51);
}

char *
cl_51core::disass(t_addr addr, const char *sep)
{
  char work[256], temp[20], c[2];
  char *buf, *p, *t, *s;
  const char *b;
  t_mem code= rom->get(addr);

  p= work;
  b= dis_tbl()[code].mnemonic;
  while (*b)
    {
      if (*b == '%')
	{
	  b++;
	  switch (*(b++))
	    {
	    case 'A': // absolute address
	      sprintf(temp, "%04x",
		      /*t_addr*/int((addr&0xf800)|
				    (((code>>5)&0x07)*256 +
				     rom->get(addr+1))));
	      break;
	    case 'l': // long address
	      sprintf(temp, "%04x",
		      /*t_addr*/int(rom->get(addr+1)*256 +
				    rom->get(addr+2)));
	      break;
	    case 'a': // addr8 (direct address) at 2nd byte
 	      if (!get_name(rom->get(addr+1), sfr_tbl(), temp))
		sprintf(temp, "%02x", (int)rom->get(addr+1));
	      break;
	    case '8': // addr8 (direct address) at 3rd byte
 	      if (!get_name(rom->get(addr+2), sfr_tbl(), temp))
		sprintf(temp, "%02x", (int)rom->get(addr+2));
	      break;
	    case 'b': // bitaddr at 2nd byte
	      {
		t_addr ba= rom->get(addr+1);
		if (get_name(ba, bit_tbl(), temp))
		  break;
		if (get_name((ba<128)?((ba/8)+32):(ba&0xf8), sfr_tbl(), temp))
		  {
		    strcat(temp, ".");
		    sprintf(c, "%1d", (int)(ba & 0x07));
		    strcat(temp, c);
		    break;
		  }
		sprintf(temp, "%02lx.%d", (long int)((ba<128)?((ba/8)+32):(ba&0xf8)),
			(int)(ba & 0x07));
		break;
	      }
	    case 'r': // rel8 address at 2nd byte
	      sprintf(temp, "%04x",
		      /*t_addr*/int(addr+2+(signed char)(rom->get(addr+1))));
	      break;
	    case 'R': // rel8 address at 3rd byte
	      sprintf(temp, "%04x",
		      /*t_addr*/int(addr+3+(signed char)(rom->get(addr+2))));
	      break;
	    case 'd': // data8 at 2nd byte
	      sprintf(temp, "%02x", (int)rom->get(addr+1));
	      break;
	    case 'D': // data8 at 3rd byte
	      sprintf(temp, "%02x", (int)rom->get(addr+2));
	      break;
	    case '6': // data16 at 2nd(H)-3rd(L) byte
	      sprintf(temp, "%04x",
		      /*t_addr*/int(rom->get(addr+1)*256 +
				    rom->get(addr+2)));
	      break;
	    default:
	      strcpy(temp, "?");
	      break;
	    }
	  t= temp;
	  while (*t)
	    *(p++)= *(t++);
	}
      else
	*(p++)= *(b++);
    }
  *p= '\0';

  p= strchr(work, ' ');
  if (!p)
    {
      buf= strdup(work);
      return(buf);
    }
  if (sep == NULL)
    buf= (char *)malloc(6+strlen(p)+1);
  else
    buf= (char *)malloc((p-work)+strlen(sep)+strlen(p)+1);
  for (p= work, s= buf; *p != ' '; p++, s++)
    *s= *p;
  p++;
  *s= '\0';
  if (sep == NULL)
    {
      while (strlen(buf) < 6)
	strcat(buf, " ");
    }
  else
    strcat(buf, sep);
  strcat(buf, p);
  return(buf);
}


void
cl_51core::print_regs(class cl_console_base *con)
{
  t_addr start;
  t_mem data;
  t_mem dp;
  
  // show regs
  start= psw->get() & 0x18;
  iram->dump(start, start+7, 8, con->get_fout());
  // show indirectly addressed IRAM and some basic regs
  data= iram->get(iram->get(start));
  con->dd_printf("@R0 %02x %c", data, isprint(data) ? data : '.');

  con->dd_printf("  ACC= 0x%02x %3d %c  B= 0x%02x\n", sfr->get(ACC), sfr->get(ACC),
              isprint(sfr->get(ACC))?(sfr->get(ACC)):'.', sfr->get(B));
  data= iram->get(iram->get(start+1));
  con->dd_printf("@R1 %02x %c", data, isprint(data) ? data : '.');
  data= psw->get();
  con->dd_printf("  PSW= 0x%02x CY=%c AC=%c OV=%c P=%c\n", data,
              (data&bmCY)?'1':'0', (data&bmAC)?'1':'0',
              (data&bmOV)?'1':'0', (data&bmP)?'1':'0');
  /* show stack pointer */
  start = sfr->get (SP);
  con->dd_printf ("SP ", start);
  iram->dump (start, start - 7, 8, con->get_fout());
  // show DPTR(s)
  if (dptr)
    {
      int act;
      int mask;
	      int i;
      if (cpu &&
	  (cpu->cfg_get(uc51cpu_aof_mdpc) > 0x7f))
	{
	  // multi DPTR chip style
	  act= sfr->get(cpu->cfg_get(uc51cpu_aof_mdpc));
	  mask= cpu->cfg_get(uc51cpu_mask_mdpc);
	  while ((mask&1) == 0)
	    {
	      act>>= 1;
	      mask>>= 1;
	    }
	  act&= mask;
	  class cl_memory *dptr_chip= memory("dptr_chip");
	  if (dptr_chip)
	    {
	      for (i= 0; i <= mask; i++)
		{
		  int a= i*dptr->get_size();
		  dp= 0;
		  int di;
		  for (di= dptr->get_size()-1; di >= 0; di--)
		    dp= (dp<<8) + dptr_chip->get(a+di);
		  con->dd_printf(" %cDPTR%d= ", (i==act)?'*':' ', i);
		  con->dd_printf(xram->addr_format, dp);
		  data= xram->read(dp);
		  con->dd_printf(" @DPTR%d= ", i);
		  con->dd_printf("0x%02x %3d %c\n", data, data,
				 isprint(data)?data:'.');
		}
	    }
	}
      else if (cpu &&
	  (cpu->cfg_get(uc51cpu_aof_mdps) > 0x7f))
	{
	  // multi DPTR sfr style
	  act= sfr->get(cpu->cfg_get(uc51cpu_aof_mdps));
	  mask= cpu->cfg_get(uc51cpu_mask_mdps);
	  while ((mask&1) == 0)
	    {
	      act>>= 1;
	      mask>>= 1;
	    }
	  act&= mask;
	  i= 0;
	  dp= sfr_chip->get(DPL-0x80) +
	    sfr_chip->get(DPH-0x80) * 256;
	  con->dd_printf(" %cDPTR%d= ", (i==act)?'*':' ', i);
	  con->dd_printf(xram->addr_format, dp);
	  data= xram->read(dp);
	  con->dd_printf(" @DPTR%d= ", i);
	  con->dd_printf("0x%02x %3d %c\n", data, data,
			 isprint(data)?data:'.');
	  i= 1;
	  dp= sfr_chip->get(cpu->cfg_get(uc51cpu_aof_mdps1l) - 0x80) +
	    sfr_chip->get(cpu->cfg_get(uc51cpu_aof_mdps1h) - 0x80) * 256;
	  con->dd_printf(" %cDPTR%d= ", (i==act)?'*':' ', i);
	  con->dd_printf(xram->addr_format, dp);
	  data= xram->read(dp);
	  con->dd_printf(" @DPTR%d= ", i);
	  con->dd_printf("0x%02x %3d %c\n", data, data,
			 isprint(data)?data:'.');
	}
      else
	{
	  // non-multi DPTR
	  t_mem dp= dptr->get(0) +
	    dptr->get(1) * 256 +
	    dptr->get(2) * 256*256 +
	    dptr->get(3) * 256*256*256;
	  data= xram->get(dp);
	  con->dd_printf("   DPTR= ");
	  con->dd_printf(xram->addr_format, dp);
	  con->dd_printf(" @DPTR= 0x%02x %3d %c\n",
			 data, data, isprint(data)?data:'.');
	}
    }
  else
    {
      // no dptr address space, read SFR directly
      data= xram->get(sfr->read(DPH)*256+sfr->read(DPL));
      con->dd_printf("   DPTR= 0x%02x%02x @DPTR= 0x%02x %3d %c\n",
		     sfr->get(DPH),
		     sfr->get(DPL),
		     data, data, isprint(data)?data:'.');
    }
  
  print_disass(PC, con);
}


/*
 * Converting bit address into real memory
 */

class cl_address_space *
cl_51core::bit2mem(t_addr bitaddr, t_addr *memaddr, t_mem *bitmask)
{
  class cl_address_space *m;
  t_addr ma;

  bitaddr&= 0xff;
  if (bitaddr < 128)
    {
      m= iram;
      ma= bitaddr/8 + 0x20;
    }
  else
    {
      m= sfr;
      ma= bitaddr & 0xf8;
    }
  if (memaddr)
    *memaddr= ma;
  if (bitmask)
    *bitmask= 1 << (bitaddr & 0x7);
  return(m);
}

t_addr
cl_51core::bit_address(class cl_memory *mem,
		       t_addr mem_address, int bit_number)
{
  if (bit_number < 0 ||
      bit_number > 7 ||
      mem_address < 0)
    return(-1);
  class cl_memory *sfrchip= memory("sfr_chip");
  if (mem == sfrchip)
    {
      mem= sfr;
      mem_address+= sfr->start_address;
    }
  if (mem == sfr)
    {
      if (mem_address < 128 ||
	  mem_address % 8 != 0 ||
	  mem_address > 255)
	return(-1);
      return(128 + (mem_address-128) + bit_number);
    }
  if (mem == iram)
    {
      if (mem_address < 0x20 ||
	  mem_address >= 0x20+32)
	return(-1);
      return((mem_address-0x20)*8 + bit_number);
    }
  return(-1);
}


/*
 * Resetting the micro-controller
 */

void
cl_51core::reset(void)
{
  cl_uc::reset();

  clear_sfr();

  result= resGO;

  //was_reti= false;
}


/*
 * Setting up SFR area to reset value
 */

void
cl_51core::clear_sfr(void)
{
  int i;

  for (i= 0x80; i <= 0xff; i++)
    sfr->set(i, 0);
  sfr->/*set*/write(P0, 0xff);
  sfr->/*set*/write(P1, 0xff);
  sfr->/*set*/write(P2, 0xff);
  sfr->/*set*/write(P3, 0xff);
  prev_p1= /*port_pins[1] &*/ sfr->/*get*/read(P1);
  prev_p3= /*port_pins[3] &*/ sfr->/*get*/read(P3);
  sfr->write(ACC, 0);
  sfr->write(B, 0);
  sfr->write(PSW, 0);
  sfr->write(SP, 7);
  sfr->write(DPL, 0);
  sfr->write(DPH, 0);
  sfr->write(IP, 0);
  sfr->write(IE, 0);
  sfr->write(TMOD, 0);
  sfr->write(TCON, 0);
  sfr->write(TH0, 0);
  sfr->write(TL0, 0);
  sfr->write(TH1, 0);
  sfr->write(TL1, 0);
  sfr->write(SCON, 0);
  sfr->write(PCON, 0);

  sfr->set_nuof_writes(0);
  sfr->set_nuof_reads(0);
}


/*
 * Analyzing code and settig up instruction map
 */

void
cl_51core::analyze(t_addr addr)
{
  uint code;
  struct dis_entry *tabl;

  code= rom->get(addr);
  tabl= &(dis_tbl()[code]);
  while (!inst_at(addr) &&
	 code != 0xa5 /* break point */)
    {
      set_inst_at(addr);
      switch (tabl->branch)
	{
	case 'a': // acall
	  analyze((addr & 0xf800)|
		  ((rom->get(addr+1)&0x07)*256+
		   rom->get(addr+2)));
	  analyze(addr+tabl->length);
	  break;
	case 'A': // ajmp
	  addr= (addr & 0xf800)|
	    ((rom->get(addr+1) & 0x07)*256 + rom->get(addr+2));
	  break;
	case 'l': // lcall
	  analyze(rom->get(addr+1)*256 + rom->get(addr+2));
	  analyze(addr+tabl->length);
	  break;
	case 'L': // ljmp
	  addr= rom->get(addr+1)*256 + rom->get(addr+2);
	  break;
	case 'r': // reljmp (2nd byte)
	  analyze(rom->validate_address(addr+(signed char)(rom->get(addr+1))));
	  analyze(addr+tabl->length);
	  break;
	case 'R': // reljmp (3rd byte)
	  analyze(rom->validate_address(addr+(signed char)(rom->get(addr+2))));
	  analyze(addr+tabl->length);
	  break;
	case 's': // sjmp
	  {
	    signed char target;
	    target= rom->get(addr+1);
	    addr+= 2;
	    addr= rom->validate_address(addr+target);
	    break;
	  }
	case '_':
	  return;
	default:
	  addr= rom->validate_address(addr+tabl->length);
	  break;
	}
      code= rom->get(addr);
      tabl= &(dis_tbl()[code]);
    }
}


/*
 * Inform hardware elements that `cycles' machine cycles have elapsed
 */

/*int
cl_51core::tick_hw(int cycles)
{
  cl_uc::tick_hw(cycles);
  //do_hardware(cycles);
  return(0);
}*/

/*int
cl_51core::tick(int cycles)
{
  cl_uc::tick(cycles);
  //do_hardware(cycles);
  return(0);
}*/


/*
 * Correcting direct address
 *
 * This function returns address of addressed element which can be an IRAM
 * or an SFR.
 */

class cl_memory_cell *
cl_51core::get_direct(t_mem addr)
{
  if (addr < sfr->start_address)
    return(iram->get_cell(addr));
  else
    return(sfr->get_cell(addr));
}


/*
 * Fetching one instruction and executing it
 */

int
cl_51core::exec_inst(void)
{
  t_mem code;
  int res= resGO;

  //pr_inst();
  instPC= PC;
  if (fetch(&code))
    return(resBREAKPOINT);
  //tick_hw(1);
  tick(1);
  switch (code)
    {
    case 0x30: res= inst_jnb_bit_addr(code); break;
    case 0xf5: res= inst_mov_addr_a(code); break;
    case 0xe5: res= inst_mov_a_addr(code); break;
    case 0x05: res= inst_inc_addr(code); break;
    case 0x04: res= inst_inc_a(code); break;
    case 0x60: res= inst_jz_addr(code); break;
    case 0xe4: res= inst_clr_a(code); break;
    case 0x80: res= inst_sjmp(code); break;
    case 0x74: res= inst_mov_a_Sdata(code); break;
    case 0x45: res= inst_orl_a_addr(code); break;
    case 0x35: res= inst_addc_a_addr(code); break;
    case 0x25: res= inst_add_a_addr(code); break;
    case 0x85: res= inst_mov_addr_addr(code); break;
    case 0xf8: case 0xf9: case 0xfa: case 0xfb:
    case 0xfc: case 0xfd: case 0xfe: case 0xff: res= inst_mov_rn_a(code);break;
    case 0x95: res= inst_subb_a_addr(code); break;
    case 0x93: res= inst_movc_a_Sa_dptr(code); break;
    case 0x88: case 0x89: case 0x8a: case 0x8b:
    case 0x8c: case 0x8d: case 0x8e: case 0x8f:res=inst_mov_addr_rn(code);break;
    case 0xd0: res= inst_pop(code); break;
    case 0xc0: res= inst_push(code); break;
    case 0x02: res= inst_ljmp(code); break;

    case 0x00: res= inst_nop(code); break;
    case 0x01: case 0x21: case 0x41: case 0x61:
    case 0x81: case 0xa1: case 0xc1: case 0xe1:res=inst_ajmp_addr(code);break;
    case 0x03: res= inst_rr(code); break;
    case 0x06: case 0x07: res= inst_inc_Sri(code); break;
    case 0x08: case 0x09: case 0x0a: case 0x0b:
    case 0x0c: case 0x0d: case 0x0e: case 0x0f: res= inst_inc_rn(code); break;
    case 0x10: res= inst_jbc_bit_addr(code); break;
    case 0x11: case 0x31: case 0x51: case 0x71:
    case 0x91: case 0xb1: case 0xd1: case 0xf1:res=inst_acall_addr(code);break;
    case 0x12: res= inst_lcall(code, 0, false); break;
    case 0x13: res= inst_rrc(code); break;
    case 0x14: res= inst_dec_a(code); break;
    case 0x15: res= inst_dec_addr(code); break;
    case 0x16: case 0x17: res= inst_dec_Sri(code); break;
    case 0x18: case 0x19: case 0x1a: case 0x1b:
    case 0x1c: case 0x1d: case 0x1e: case 0x1f: res= inst_dec_rn(code); break;
    case 0x20: res= inst_jb_bit_addr(code); break;
    case 0x22: res= inst_ret(code); break;
    case 0x23: res= inst_rl(code); break;
    case 0x24: res= inst_add_a_Sdata(code); break;
    case 0x26: case 0x27: res= inst_add_a_Sri(code); break;
    case 0x28: case 0x29: case 0x2a: case 0x2b:
    case 0x2c: case 0x2d: case 0x2e: case 0x2f:res= inst_add_a_rn(code);break;
    case 0x32: res= inst_reti(code); break;
    case 0x33: res= inst_rlc(code); break;
    case 0x34: res= inst_addc_a_Sdata(code); break;
    case 0x36: case 0x37: res= inst_addc_a_Sri(code); break;
    case 0x38: case 0x39: case 0x3a: case 0x3b:
    case 0x3c: case 0x3d: case 0x3e: case 0x3f:res= inst_addc_a_rn(code);break;
    case 0x40: res= inst_jc_addr(code); break;
    case 0x42: res= inst_orl_addr_a(code); break;
    case 0x43: res= inst_orl_addr_Sdata(code); break;
    case 0x44: res= inst_orl_a_Sdata(code); break;
    case 0x46: case 0x47: res= inst_orl_a_Sri(code); break;
    case 0x48: case 0x49: case 0x4a: case 0x4b:
    case 0x4c: case 0x4d: case 0x4e: case 0x4f: res= inst_orl_a_rn(code);break;
    case 0x50: res= inst_jnc_addr(code); break;
    case 0x52: res= inst_anl_addr_a(code); break;
    case 0x53: res= inst_anl_addr_Sdata(code); break;
    case 0x54: res= inst_anl_a_Sdata(code); break;
    case 0x55: res= inst_anl_a_addr(code); break;
    case 0x56: case 0x57: res= inst_anl_a_Sri(code); break;
    case 0x58: case 0x59: case 0x5a: case 0x5b:
    case 0x5c: case 0x5d: case 0x5e: case 0x5f: res= inst_anl_a_rn(code);break;
    case 0x62: res= inst_xrl_addr_a(code); break;
    case 0x63: res= inst_xrl_addr_Sdata(code); break;
    case 0x64: res= inst_xrl_a_Sdata(code); break;
    case 0x65: res= inst_xrl_a_addr(code); break;
    case 0x66: case 0x67: res= inst_xrl_a_Sri(code); break;
    case 0x68: case 0x69: case 0x6a: case 0x6b:
    case 0x6c: case 0x6d: case 0x6e: case 0x6f: res= inst_xrl_a_rn(code);break;
    case 0x70: res= inst_jnz_addr(code); break;
    case 0x72: res= inst_orl_c_bit(code); break;
    case 0x73: res= inst_jmp_Sa_dptr(code); break;
    case 0x75: res= inst_mov_addr_Sdata(code); break;
    case 0x76: case 0x77: res= inst_mov_Sri_Sdata(code); break;
    case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c:
    case 0x7d: case 0x7e: case 0x7f: res=inst_mov_rn_Sdata(code); break;
    case 0x82: res= inst_anl_c_bit(code); break;
    case 0x83: res= inst_movc_a_Sa_pc(code); break;
    case 0x84: res= inst_div_ab(code); break;
    case 0x86: case 0x87: res= inst_mov_addr_Sri(code); break;
    case 0x90: res= inst_mov_dptr_Sdata(code); break;
    case 0x92: res= inst_mov_bit_c(code); break;
    case 0x94: res= inst_subb_a_Sdata(code); break;
    case 0x96: case 0x97: res= inst_subb_a_Sri(code); break;
    case 0x98: case 0x99: case 0x9a: case 0x9b:
    case 0x9c: case 0x9d: case 0x9e: case 0x9f:res= inst_subb_a_rn(code);break;
    case 0xa0: res= inst_orl_c_Sbit(code); break;
    case 0xa2: res= inst_mov_c_bit(code); break;
    case 0xa3: res= inst_inc_dptr(code); break;
    case 0xa4: res= inst_mul_ab(code); break;
    case 0xa5: res= inst_unknown(); break;
    case 0xa6: case 0xa7: res= inst_mov_Sri_addr(code); break;
    case 0xa8: case 0xa9: case 0xaa: case 0xab:
    case 0xac: case 0xad: case 0xae: case 0xaf:res=inst_mov_rn_addr(code);break;
    case 0xb0: res= inst_anl_c_Sbit(code); break;
    case 0xb2: res= inst_cpl_bit(code); break;
    case 0xb3: res= inst_cpl_c(code); break;
    case 0xb4: res= inst_cjne_a_Sdata_addr(code); break;
    case 0xb5: res= inst_cjne_a_addr_addr(code); break;
    case 0xb6: case 0xb7: res= inst_cjne_Sri_Sdata_addr(code); break;
    case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc:
    case 0xbd: case 0xbe: case 0xbf: res=inst_cjne_rn_Sdata_addr(code); break;
    case 0xc2: res= inst_clr_bit(code); break;
    case 0xc3: res= inst_clr_c(code); break;
    case 0xc4: res= inst_swap(code); break;
    case 0xc5: res= inst_xch_a_addr(code); break;
    case 0xc6: case 0xc7: res= inst_xch_a_Sri(code); break;
    case 0xc8: case 0xc9: case 0xca: case 0xcb:
    case 0xcc: case 0xcd: case 0xce: case 0xcf: res= inst_xch_a_rn(code);break;
    case 0xd2: res= inst_setb_bit(code); break;
    case 0xd3: res= inst_setb_c(code); break;
    case 0xd4: res= inst_da_a(code); break;
    case 0xd5: res= inst_djnz_addr_addr(code); break;
    case 0xd6: case 0xd7: res= inst_xchd_a_Sri(code); break;
    case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc:
    case 0xdd: case 0xde: case 0xdf: res=inst_djnz_rn_addr(code); break;
    case 0xe0: res= inst_movx_a_Sdptr(code); break;
    case 0xe2: case 0xe3: res= inst_movx_a_Sri(code); break;
    case 0xe6: case 0xe7: res= inst_mov_a_Sri(code); break;
    case 0xe8: case 0xe9: case 0xea: case 0xeb:
    case 0xec: case 0xed: case 0xee: case 0xef: res= inst_mov_a_rn(code);break;
    case 0xf0: res= inst_movx_Sdptr_a(code); break;
    case 0xf2: case 0xf3: res= inst_movx_Sri_a(code); break;
    case 0xf4: res= inst_cpl_a(code); break;
    case 0xf6: case 0xf7: res= inst_mov_Sri_a(code); break;
    default:
      res= inst_unknown();
      break;
    }
  //post_inst();
  return(res);
}


/*
 * Simulating execution of next instruction
 *
 * This is an endless loop if requested number of steps is negative.
 * In this case execution is stopped if an instruction results other
 * status than GO. Execution can be stopped if `cmd_in' is not NULL
 * and there is input available on that file. It is usefull if the
 * command console is on a terminal. If input is available then a
 * complete line is read and dropped out because input is buffered
 * (inp_avail will be TRUE if ENTER is pressed) and it can confuse
 * command interepter.
 */
//static class cl_console *c= NULL;
int
cl_51core::do_inst(int step)
{
  result= resGO;
  while ((result == resGO) &&
	 (state != stPD) &&
	 (step != 0))
    {
      if (step > 0)
	step--;
      if (state == stGO)
	{
	  interrupt->was_reti= false;
	  pre_inst();
	  result= exec_inst();
	  post_inst();
	}
      else
	{
	  // tick hw in idle state
	  inst_ticks= 1;
	  post_inst();
	  tick(1);
	}
      if (result == resGO)
	{
	  int res;
	  if ((res= do_interrupt()) != resGO)
	    result= res;
	  else
	    result= idle_pd();
	}
      if (((result == resINTERRUPT) &&
	   stop_at_it) ||
	  result >= resSTOP)
	{
	  sim->stop(result);
	  break;
	}
    }
  if (state == stPD)
    {
      //FIXME: tick outsiders eg. watchdog
    }
  return(result);
}

/*
 * Abstract method to handle WDT
 */

/*int
cl_51core::do_wdt(int cycles)
{
  return(resGO);
}*/


/*
 * Checking for interrupt requests and accept one if needed
 */

int
cl_51core::do_interrupt(void)
{
  int i, ie= 0;

  if (interrupt->was_reti)
    {
      interrupt->was_reti= false;
      return(resGO);
    }
  if (!((ie= sfr->get(IE)) & bmEA))
    return(resGO);
  class it_level *il= (class it_level *)(it_levels->top()), *IL= 0;
  for (i= 0; i < it_sources->count; i++)
    {
      class cl_it_src *is= (class cl_it_src *)(it_sources->at(i));
      if (is->is_active() &&
	  is->enabled() &&
	  is->pending())
	{
	  int pr= priority_of(is->ie_mask);
	  if (il->level >= 0 &&
	      pr <= il->level)
	    continue;
	  if (state == stIDLE)
	    {
	      state= stGO;
	      sfr->set_bit0(PCON, bmIDL);
	      interrupt->was_reti= true;
	      return(resGO);
	    }
	  is->clear();
	  sim->app->get_commander()->
	    debug("%g sec (%d clks): Accepting interrupt `%s' PC= 0x%06x\n",
			  get_rtime(), ticks->ticks, object_name(is), PC);
	  IL= new it_level(pr, is->addr, PC, is);
	  return(accept_it(IL));
	}
    }
  return(resGO);
}

int
cl_51core::priority_of(uchar nuof_it)
{
  if (sfr->get(IP) & /*ie_mask*/nuof_it)
    return(1);
  return(0);
}


/*
 * Accept an interrupt
 */

int
cl_51core::accept_it(class it_level *il)
{
  state= stGO;
  sfr->set_bit0(PCON, bmIDL);
  it_levels->push(il);
  tick(1);
  int res= inst_lcall(0, il->addr, true);
  if (res != resGO)
    return(res);
  else
    return(resINTERRUPT);
}


/* check if interrupts are enabled (globally)
 */

bool
cl_51core::it_enabled(void)
{
  return sfr->get(IE) & bmEA;
}


/*
 * Checking if Idle or PowerDown mode should be activated
 */

int
cl_51core::idle_pd(void)
{
  uint pcon= sfr->get(PCON);

  if (!(type->subtype & CPU_CMOS))
    return(resGO);
  if (pcon & bmIDL)
    {
      if (state != stIDLE)
	sim->app->get_commander()->
	  debug("%g sec (%d clks): CPU in Idle mode (PC=0x%x, PCON=0x%x)\n",
		get_rtime(), ticks->ticks, PC, pcon);
      state= stIDLE;
      //was_reti= 1;
    }
  if (pcon & bmPD)
    {
      if (state != stPD)
	sim->app->get_commander()->
	  debug("%g sec (%d clks): CPU in PowerDown mode\n",
			get_rtime(), ticks->ticks);
      state= stPD;
    }
  return(resGO);
}


/*
 * Simulating an unknown instruction
 *
 * Normally this function is called for unimplemented instructions, because
 * every instruction must be known!
 */

int
cl_51core::inst_unknown(void)
{
  //PC--;
  class cl_error_unknown_code *e= new cl_error_unknown_code(this);
  error(e);
  return(resGO);
}


/*
 * 0x00 1 12 NOP
 */

int
cl_51core::inst_nop(uchar code)
{
  return(resGO);
}


/*
 * 0xe4 1 12 CLR A
 */

int
cl_51core::inst_clr_a(uchar code)
{
  acc->write(0);
  return(resGO);
}


/*
 * 0xc4 1 1 SWAP A
 */

int
cl_51core::inst_swap(uchar code)
{
  uchar temp;

  temp= (acc->read() >> 4) & 0x0f;
  sfr->write(ACC, (acc->get() << 4) | temp);
  return(resGO);
}


/*
 */

cl_uc51_cpu::cl_uc51_cpu(class cl_uc *auc):
  cl_hw(auc, HW_DUMMY, 0, "cpu")
{
}

int
cl_uc51_cpu::init(void)
{
  class cl_address_space *sfr= uc->address_space(MEM_SFR_ID),
    *bas= uc->address_space("bits");
  int i;
  cl_hw::init();
  if (!sfr)
    {
      fprintf(stderr, "No SFR to register %s[%d] into\n", id_string, id);
    }
  cell_psw= sfr->get_cell(PSW);//use_cell(sfr, PSW);
  cell_acc= register_cell(sfr, ACC);
  cell_sp= register_cell(sfr, SP);
  for (i= 0; i < 8; i++)
    acc_bits[i]= register_cell(bas, ACC+i);

  cl_var *v;
  uc->vars->add(v= new cl_var(cchars("cpu_aof_mdps"), cfg, uc51cpu_aof_mdps));
  v->init();
  uc->vars->add(v= new cl_var(cchars("cpu_mask_mdps"), cfg, uc51cpu_mask_mdps));
  v->init();
  uc->vars->add(v= new cl_var(cchars("cpu_aof_mdps1l"), cfg, uc51cpu_aof_mdps1l));
  v->init();
  uc->vars->add(v= new cl_var(cchars("cpu_aof_mdps1h"), cfg, uc51cpu_aof_mdps1h));
  v->init();
  uc->vars->add(v= new cl_var(cchars("cpu_aof_mdpc"), cfg, uc51cpu_aof_mdpc));
  v->init();
  uc->vars->add(v= new cl_var(cchars("cpu_mask_mdpc"), cfg, uc51cpu_mask_mdpc));
  v->init();
  
  return(0);
}

void
cl_uc51_cpu::write(class cl_memory_cell *cell, t_mem *val)
{
  if (conf(cell, val))
    return;
  if (cell == cell_sp)
    {
      if (*val > uc->sp_max)
	uc->sp_max= *val;
      uc->sp_avg= (uc->sp_avg+(*val))/2;
    }
  else 
    {
      bool p;
      int i;
      uchar uc, n= *val;

      if (cell != cell_acc)
	{
	  cell->set(*val);
	  n= cell_acc->get();
	}
      p = false;
      uc= n;
      for (i= 0; i < 8; i++)
	{
	  if (uc & 1)
	    p= !p;
	  uc>>= 1;
	}
      if (p)
	cell_psw->set_bit1(bmP);
      else
	cell_psw->set_bit0(bmP);
    }
  /*else if (cell == cell_pcon)
    {
      printf("PCON write 0x%x (PC=0x%x)\n", *val, uc->PC);
      uc->sim->stop(0);
      }*/
}

t_mem
cl_uc51_cpu::conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val)
{
  if (val)
    cell->set(*val);
  switch ((enum uc51cpu_cfg)addr)
    {
    case uc51cpu_aof_mdps: // addr of multi_DPTR_sfr selector
      if (val)
	((cl_51core *)uc)->decode_dptr();
      break;
    case uc51cpu_mask_mdps: // mask in mutli_DPTR_sfr selector
      break;
    case uc51cpu_aof_mdps1l: // addr of multi_DPTR_sfr DPL1
      break;
    case uc51cpu_aof_mdps1h: // addr of multi_DPTR_sfr DPH1
      break;

    case uc51cpu_aof_mdpc: // addr of multi_DPTR_chip selector
      if (val)
	((cl_51core *)uc)->decode_dptr();
      break;
    case uc51cpu_mask_mdpc: // mask in multi_DPTR_chip selector
      break;
  
    case uc51cpu_nuof:
      break;
    }
  return cell->get();
}

/* End of s51.src/uc51.cc */
