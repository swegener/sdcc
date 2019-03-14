/*
 * Simulator of microcontrollers (pdk.cc)
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

#include <ctype.h>
#include <stdarg.h> /* for va_list */
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include "i_string.h"

// prj
#include "globals.h"
#include "pobjcl.h"

// sim
#include "simcl.h"

// local
#include "glob.h"
#include "pdkcl.h"
#include "portcl.h"
#include "regspdk.h"

/*******************************************************************/

/*
 * Base type of PDK controllers
 */

cl_pdk::cl_pdk(struct cpu_entry *IType, class cl_sim *asim) : cl_uc(asim) {
  type = IType;
}

int cl_pdk::init(void) {
  cl_uc::init(); /* Memories now exist */

  xtal = 8000000;
  sp_max = 0x00;

  // rom = address_space(MEM_ROM_ID);
  // ram = mem(MEM_XRAM);
  // ram = rom;

  // zero out ram(this is assumed in regression tests)
  // for (int i = 0x0; i < 0x8000; i++) {
  //   ram->set((t_addr)i, 0);
  // }

  return (0);
}

void cl_pdk::reset(void) {
  cl_uc::reset();

  PC = 0x0000;
  regs.a = 0;

  regs.flag = 0x00;
  regs.sp = 0x00;
  regs.clkmd = 0xF6;
  regs.inten = 0x00;
  regs.intrq = 0x00;
  regs.t16m = 0x00;
  regs.eoscr = 0x00;
  regs.integs = 0x00;
  regs.padier = 0xF9;
  regs.pbdier = 0xFF;
  regs.pa = 0x00;
  regs.pac = 0x00;
  regs.paph = 0x00;
  regs.pb = 0x00;
  regs.pbc = 0x00;
  regs.pbph = 0x00;
  regs.misc = 0x00;
  regs.tm2c = 0x00;
  regs.tm2ct = 0x00;
  regs.tm2s = 0x00;
  regs.tm2s = 0x00;
  regs.tm2b = 0x00;
  regs.tm3c = 0x00;
  regs.tm3ct = 0x00;
  regs.tm3s = 0x00;
  regs.tm3b = 0x00;
  regs.gpcc = 0x00;
  regs.gpcs = 0x00;
  regs.pwmg0c = 0x00;
  regs.pwmg0s = 0x00;
  regs.pwmg0cubh = 0x00;
  regs.pwmg0cubl = 0x00;
  regs.pwmg0dth = 0x00;
  regs.pwmg0dtl = 0x00;
  regs.pwmg1c = 0x00;
  regs.pwmg1s = 0x00;
  regs.pwmg1cubh = 0x00;
  regs.pwmg1cubl = 0x00;
  regs.pwmg1dth = 0x00;
  regs.pwmg1dtl = 0x00;
  regs.pwmg2c = 0x00;
  regs.pwmg2s = 0x00;
  regs.pwmg2cubh = 0x00;
  regs.pwmg2cubl = 0x00;
  regs.pwmg2dth = 0x00;
  regs.pwmg2dtl = 0x00;
}

char *cl_pdk::id_string(void) { return ((char *)"pdk14"); }

/*
 * Making elements of the controller
 */
/*
t_addr
cl_pdk::get_mem_size(enum mem_class type)
{
  switch(type)
    {
    case MEM_ROM: return(0x10000);
    case MEM_XRAM: return(0x10000);
    default: return(0);
    }
 return(cl_uc::get_mem_size(type));
}
*/

void cl_pdk::mk_hw_elements(void) {
  // TODO: Add hardware stuff here.
  cl_uc::mk_hw_elements();
}

class cl_memory_chip *c;

void cl_pdk::make_memories(void) {
  class cl_address_space *as;

  rom = as = new cl_address_space("rom", 0, 0x7D0, 16);
  as->init();
  address_spaces->add(as);
  ram = as = new cl_address_space("ram", 0, 0x80, 8);
  as->init();
  address_spaces->add(as);

  class cl_address_decoder *ad;
  class cl_memory_chip *chip;

  chip = new cl_memory_chip("rom_chip", 0x7Dd0, 16);
  chip->init();
  memchips->add(chip);

  ad = new cl_address_decoder(as = address_space("rom"), chip, 0, 0x7CF, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  regs8 = new cl_address_space("regs8", 0, 45, 8);
  regs8->init();
  int i = 0;
  regs8->get_cell(i++)->decode((t_mem *)&regs.a);
  regs8->get_cell(i++)->decode((t_mem *)&regs.flag);
  regs8->get_cell(i++)->decode((t_mem *)&regs.sp);
  regs8->get_cell(i++)->decode((t_mem *)&regs.clkmd);
  regs8->get_cell(i++)->decode((t_mem *)&regs.inten);
  regs8->get_cell(i++)->decode((t_mem *)&regs.intrq);
  regs8->get_cell(i++)->decode((t_mem *)&regs.t16m);
  regs8->get_cell(i++)->decode((t_mem *)&regs.eoscr);
  regs8->get_cell(i++)->decode((t_mem *)&regs.integs);
  regs8->get_cell(i++)->decode((t_mem *)&regs.padier);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pbdier);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pa);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pac);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pb);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pbc);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pbph);
  regs8->get_cell(i++)->decode((t_mem *)&regs.misc);
  regs8->get_cell(i++)->decode((t_mem *)&regs.tm2c);
  regs8->get_cell(i++)->decode((t_mem *)&regs.tm2ct);
  regs8->get_cell(i++)->decode((t_mem *)&regs.tm2s);
  regs8->get_cell(i++)->decode((t_mem *)&regs.tm2b);
  regs8->get_cell(i++)->decode((t_mem *)&regs.tm3c);
  regs8->get_cell(i++)->decode((t_mem *)&regs.tm3ct);
  regs8->get_cell(i++)->decode((t_mem *)&regs.tm3s);
  regs8->get_cell(i++)->decode((t_mem *)&regs.tm3b);
  regs8->get_cell(i++)->decode((t_mem *)&regs.gpcc);
  regs8->get_cell(i++)->decode((t_mem *)&regs.gpcs);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg0c);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg0s);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg0cubh);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg0cubl);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg0dth);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg0dtl);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg1c);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg1s);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg1cubh);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg1cubl);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg1dth);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg1dtl);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg2c);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg2s);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg2cubh);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg2cubl);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg2dth);
  regs8->get_cell(i++)->decode((t_mem *)&regs.pwmg2dtl);
  /* TODO: Add the other registers. */

  address_spaces->add(regs8);

  class cl_var *v;
  i = 0;
  vars->add(v = new cl_var(cchars("a"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("flag"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("sp"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("clkmd"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("inten"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("intrq"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("t16m"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("eoscr"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("integs"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("padier"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pbdier"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pa"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pac"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("paph"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pb"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pbc"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pbph"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("misc"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("tm2c"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("tm2ct"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("tm2s"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("tm2b"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("tm3c"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("tm3ct"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("tm3s"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("tm3b"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("gpcc"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("gpcs"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg0c"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg0s"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg0cubh"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg0cubl"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg0dth"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg0dtl"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg1c"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg1s"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg1cubh"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg1cubl"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg1dth"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg1dtl"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg2c"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg2s"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg2cubh"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg2cubl"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg2dth"), regs8, i++, ""));
  v->init();
  vars->add(v = new cl_var(cchars("pwmg2dtl"), regs8, i++, ""));
  v->init();
}

/*
 * Help command interpreter
 */

struct dis_entry *cl_pdk::dis_tbl(void) {
  return (disass_pdk_14);
}

/*struct name_entry *
cl_pdk::sfr_tbl(void)
{
  return(0);
}*/

/*struct name_entry *
cl_pdk::bit_tbl(void)
{
  //FIXME
  return(0);
}*/

int cl_pdk::inst_length(t_addr /*addr*/) {
  return 1;
}
int cl_pdk::inst_branch(t_addr addr) {
  int b;

  get_disasm_info(addr, NULL, &b, NULL, NULL);

  return b;
}

bool cl_pdk::is_call(t_addr addr) {
  struct dis_entry *e;

  get_disasm_info(addr, NULL, NULL, NULL, &e);

  return e ? (e->is_call) : false;
}

int cl_pdk::longest_inst(void) { return 1; }

const char *cl_pdk::get_disasm_info(t_addr addr, int *ret_len, int *ret_branch,
                                    int *immed_offset,
                                    struct dis_entry **dentry) {
  const char *b = NULL;
  uint code;
  int len = 0;
  int immed_n = 0;
  int i;
  int start_addr = addr;
  struct dis_entry *dis_e;

  //code = rom->get(addr++);
  dis_e = NULL;

  switch (code = 0x14) {
      /* Dispatch to appropriate pdk port. */
      /*case 0x13 :
        code= rom->get(addr++);
    i= 0;
    while ((code & disass_pdk_13[i].mask) != disass_pdk13[i].code &&
      disass_pdk_13[i].mnemonic)
      i++;
    dis_e = &disass_pdk_13[i];
    b= disass_pdk_13[i].mnemonic;
    if (b != NULL)
      len += (disass_pdk_13[i].length + 1);
  break;*/

    case 0x14:
      code = rom->get(addr++);
      i = 0;
      while ((code & disass_pdk_14[i].mask) != disass_pdk_14[i].code &&
             disass_pdk_14[i].mnemonic)
        i++;
      dis_e = &disass_pdk_14[i];
      b = disass_pdk_14[i].mnemonic;
      if (b != NULL) len += 1; //(disass_pdk_14[i].length + 1);
      break;

      /*case 0x15 :
        code= rom->get(addr++);
    i= 0;
    while ((code & disass_pdk_15[i].mask) != disass_pdk_15[i].code &&
      disass_pdk_15[i].mnemonic)
      i++;
    dis_e = &disass_pdk_15[i];
    b= disass_pdk_15[i].mnemonic;
    if (b != NULL)
      len += (disass_pdk_15[i].length + 1);
  break;*/

      /*case 0x16 :
        code= rom->get(addr++);
    i= 0;
    while ((code & disass_pdk_16[i].mask) != disass_pdk_16[i].code &&
      disass_pdk_16[i].mnemonic)
      i++;
    dis_e = &disass_pdk_16[i];
    b= disass_pdk_16[i].mnemonic;
    if (b != NULL)
      len += (disass_pdk_16[i].length + 1);
  break;*/

      /*default:
        i= 0;
        while ((code & disass_pdk[i].mask) != disass_pdk[i].code &&
               disass_pdk[i].mnemonic)
          i++;
        dis_e = &disass_pdk[i];
        b= disass_pdk[i].mnemonic;
        if (b != NULL)
          len += (disass_pdk[i].length);
      break;*/
  }

  if (ret_branch) {
    *ret_branch = dis_e->branch;
  }

  if (immed_offset) {
    if (immed_n > 0)
      *immed_offset = immed_n;
    else
      *immed_offset = (addr - start_addr);
  }

  if (len == 0) len = 1;

  if (ret_len) *ret_len = len;

  if (dentry) *dentry = dis_e;

  return b;
}

char *cl_pdk::disass(t_addr addr, const char *sep) {
  char work[256], temp[20];
  const char *b;
  char *buf, *p, *t;
  int len = 0;
  int immed_offset = 0;
  struct dis_entry *dis_e;

  p = work;

  b = get_disasm_info(addr, &len, NULL, &immed_offset, &dis_e);

  if (b == NULL) {
    buf = (char *)malloc(30);
    strcpy(buf, "UNKNOWN/INVALID");
    return (buf);
  }

  while (*b) {
    if (*b == '%') {
      b++;
      uint code = rom->get(addr) & ~(uint)dis_e->mask;
      switch (*(b++)) {
        case 'k':  // k    immediate addressing
          sprintf(temp, "#%u", code);
          break;
        case 'm':  // m    memory addressing
          if (*b == 'n') {
            code &= 0x3F;
            ++b;
          }
          sprintf(temp, "%u", code);
          break;
        case 'i':  // i    IO addressing
          // TODO: Maybe add pretty printing.
          if (*b == 'n') {
            code &= 0x3F;
            ++b;
          }
          sprintf(temp, "[%u]", code);
          break;
        case 'n':  // n    N-bit addressing
          sprintf(temp, "#%u", (code & 0x1C0) >> 6);
          break;
        default:
          strcpy(temp, "%?");
          break;
      }
      t = temp;
      while (*t) *(p++) = *(t++);
    } else
      *(p++) = *(b++);
  }
  *p = '\0';

  p = strchr(work, ' ');
  if (!p) {
    buf = strdup(work);
    return (buf);
  }
  if (sep == NULL)
    buf = (char *)malloc(6 + strlen(p) + 1);
  else
    buf = (char *)malloc((p - work) + strlen(sep) + strlen(p) + 1);
  for (p = work, t = buf; *p != ' '; p++, t++) *t = *p;
  p++;
  *t = '\0';
  if (sep == NULL) {
    while (strlen(buf) < 6) strcat(buf, " ");
  } else
    strcat(buf, sep);
  strcat(buf, p);
  return (buf);
}

void cl_pdk::print_regs(class cl_console_base *con) {
  con->dd_printf("A= 0x%02x(%3d)\n", regs.a, regs.a);
  con->dd_printf("Flag= 0x%02x(%3d)  \n", regs.flag, regs.flag);
  con->dd_printf("SP= 0x%02x(%3d)\n", regs.sp, regs.sp);

  print_disass(PC, con);
}

/*
 * Execution
 */

int cl_pdk::exec_inst(void) {
  t_mem code;

  if (fetch(&code)) {
    return (resBREAKPOINT);
  }
  tick(1);

  int status = execute(code);
  if (status == resINV_INST) {
    PC = rom->inc_address(PC, -1);

    sim->stop(resINV_INST);
    return (resINV_INST);
  }
  return (status);
}

/* End of pdk.src/pdk.cc */
