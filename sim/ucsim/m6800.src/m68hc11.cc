/*
 * Simulator of microcontrollers (m68hc11.cc)
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

#include <stdlib.h>
#include <ctype.h>

#include "globals.h"
#include "utils.h"

#include "dregcl.h"
#include "glob.h"
#include "glob11.h"
#include "g11p0.h"
#include "wraps.h"

#include "m68hc11cl.h"


instruction_wrapper_fn itab18[256];

int8_t p0ticks11[256]= {
  /*      0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  */
  /* 0 */ 0, 2,40,40, 3, 3, 2, 2, 3, 3, 2, 2, 2, 2, 2, 2,
  /* 1 */ 2, 2, 6, 6, 6, 6, 2, 2, 0, 2, 0, 2, 7, 7, 7, 7,
  /* 2 */ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  /* 3 */ 3, 3, 4, 4, 3, 3, 3, 3, 5, 5, 3,12, 4, 3,14,14,
  /* 4 */ 2, 0, 0, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 2,
  /* 5 */ 2, 0, 0, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 2,
  /* 6 */ 6, 0, 0, 6, 6, 0, 6, 6, 6, 6, 6, 0, 6, 6, 3, 6,
  /* 7 */ 6, 0, 0, 6, 6, 0, 6, 6, 6, 6, 6, 0, 6, 6, 3, 6,
  /* 8 */ 2, 2, 2, 4, 2, 2, 2, 0, 2, 2, 2, 2, 4, 6, 3, 3,
  /* 9 */ 3, 3, 3, 5, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 4, 4,
  /* a */ 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 6, 6, 5, 5,
  /* b */ 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 6, 6, 5, 5,
  /* c */ 2, 2, 2, 4, 2, 2, 2, 0, 2, 2, 2, 2, 3, 0, 3, 2,
  /* d */ 3, 3, 3, 5, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4,
  /* e */ 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5,
  /* f */ 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5
};


int
cl_m68hcbase::init(void)
{
  cl_m6800::init();
  
#define RCV(R) reg_cell_var(&c ## R , &r ## R , "" #R "" , "CPU register " #R "")
  RCV(IY);
  RCV(D);
#undef RCV

  fill_def_18_wrappers(itab18);
  
  return 0;
}

void
cl_m68hcbase::print_regs(class cl_console_base *con)
{
  con->dd_color("answer");
  con->dd_printf("A= $%02x %3d %+4d %c  ", rA, rA, (i8_t)rA, isprint(rA)?rA:'.');
  con->dd_printf("B= $%02x %3d %+4d %c  ", rB, rB, (i8_t)rB, isprint(rB)?rB:'.');
  con->dd_printf("D= $%04x %5d %+5d ", rD, rD, (i16_t)rD);
  con->dd_printf("\n");
  con->dd_printf("CC= "); con->print_bin(rF, 8); con->dd_printf("\n");
  con->dd_printf("      HINZVC\n");

  con->dd_printf("IX= ");
  rom->dump(0, IX, IX+7, 8, con);
  con->dd_color("answer");
  
  con->dd_printf("IY= ");
  rom->dump(0, IY, IY+7, 8, con);
  con->dd_color("answer");
  
  con->dd_printf("SP= ");
  rom->dump(0, SP, SP+7, 8, con);
  con->dd_color("answer");
  
  print_disass(PC, con);
}


cl_m68hc11::cl_m68hc11(class cl_sim *asim):
  cl_m68hcbase(asim)
{
}


int
cl_m68hc11::init(void)
{
  cl_m68hcbase::init();
  
  xtal= 8000000;
  
  return 0;
}


const char *
cl_m68hc11::id_string(void)
{
  return "M68HC11";
}

void
cl_m68hc11::reset(void)
{
  cl_m68hcbase::reset();
}

int8_t *
CL11::tick_tab(t_mem code)
{
  if (code == 0x18)
    ;
  else if (code == 0x1a)
    ;
  else if (code == 0xcd)
    ;
  return p0ticks11;
}


/*
struct dis_entry *
cl_m68hc11::dis_tbl(void)
{
  return disass_m68hc11;
}
*/

struct dis_entry *
cl_m68hc11::get_dis_entry(t_addr addr)
{
  struct dis_entry *dt= dis_tbl();//, *dis_e;
  int i= 0;
  t_mem code= rom->get(addr);

  if (dt == NULL)
    return NULL;
  while (((code & dt[i].mask) != dt[i].code) &&
	 dt[i].mnemonic)
    i++;
  if (dt[i].mnemonic)
    return &dt[i];

  i= 0;
  dt= disass11p0;
  while (((code & dt[i].mask) != dt[i].code) &&
	 dt[i].mnemonic)
    i++;
  if (dt[i].mnemonic)
    return &dt[i];

  return &dt[i];
}
/*
char *
cl_m68hc11::disassc(t_addr addr, chars *comment)
{
  chars work= chars(), temp= chars();
  const char *b;
  //t_mem code= rom->get(addr);
  struct dis_entry *dis_e;
  int i;
  bool first;
  
  if ((dis_e= get_dis_entry(addr)) == NULL)
    return NULL;
  if (dis_e->mnemonic == NULL)
    return strdup("-- UNKNOWN/INVALID");
  b= dis_e->mnemonic;

  first= true;
  work= "";
  for (i=0; b[i]; i++)
    {
      if ((b[i] == ' ') && first)
	{
	  first= false;
	  while (work.len() < 6) work.append(' ');
	}
      if (b[i] == '%')
	{
	  t_addr a;
	  u8_t h, l;
	  i++;
	  temp= "";
	  switch (b[i])
	    {
	    case 'x': case 'X': // indexed
	      h= rom->read(addr+1);
	      a= rX+h;
	      work.appendf("$%02x,X", h);
	      //add_spaces(&work, 20);
	      if (b[i]=='x')
		temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      else
		temp.appendf("; [$%04x]=$%04x", a, read_addr(rom, a));
	      break;
	    case 'e': case 'E': // extended
	      h= rom->read(addr+1);
	      l= rom->read(addr+2);
	      a= h*256 + l;
	      work.appendf("$%04x", a);
	      //add_spaces(&work, 20);
	      if (b[i]=='e')
		temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      else
		temp.appendf("; [$%04x]=$%04x", a,
			     read_addr(rom, a));
	      break;
	    case 'd': case 'D': // direct
	      h= a= rom->read(addr+1);
	      work.appendf("$00%02x", h);
	      //add_spaces(&work, 20);
	      if (b[i]=='d')
		temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      else
	        temp.appendf("; [$%04x]=$%04x", a,
			     read_addr(rom, a));
	      break;
	    case 'b': // immediate 8 bit
	      work.appendf("#$%02x",
			   rom->read(addr+1));
	      break;
	    case 'B': // immediate 16 bit
	      work.appendf("#$%04x",
			   read_addr(rom, addr+1));
	      break;
	    case 'r': // relative
	      work.appendf("$%04x",
			   (addr+2+(i8_t)(rom->read(addr+1))) & 0xffff );
	      break;
	    }
	  //work+= temp;
	  if (comment && temp.nempty())
	    comment->append(temp);
	}
      else
	work+= b[i];
    }

  return(strdup(work.c_str()));
}
*/

/* 
 * OTHER instructions
 */

int
CL11::TEST(t_mem code)
{
  return resGO;
}


int
CL11::STOP(t_mem code)
{
  if (!(rF & flagS))
    state= stIDLE;
  return resGO;
}


/*
 * MOVE instructions
 */

int
CL11::ldd16(u16_t op)
{
  u8_t f= rF & ~(flagN|flagZ);
  cD.W(op);
  if (!op) f|= flagZ;
  if (op & 0x8000) f|= flagN;
  cF.W(f);
  return resGO;
}


int
CL11::std16(t_addr addr)
{
  u8_t f= rF & ~(flagN|flagZ);
  rom->write(addr, rA);
  rom->write(u16_t(addr+1), rB);
  vc.wr+= 2;
  if (!rD) f|= flagZ;
  if (rD & 0x8000) f|= flagN;
  cF.W(f);
  return resGO;
}


int
CL11::PULX(t_mem code)
{
  u8_t h, l;
  h= rom->read(++rSP);
  l= rom->read(++rSP);
  cSP.W(rSP);
  cX.W(h*256+l);
  vc.rd+= 2;
  return resGO;
}


int
CL11::PSHX(t_mem code)
{
  rom->write(rSP--, rX);
  rom->write(rSP--, rX>>8);
  cSP.W(rSP);
  vc.wr+= 2;
  return resGO;
}


int
CL11::XGDX(t_mem code)
{
  u16_t t= rD;
  cD.W(rX);
  cX.W(t);
  return resGO;
}


/*
 * ALU instructions
 */

int
CL11::sub16(class cl_cell16 &dest, u16_t op, bool c)
{
  u8_t orgc= rF&flagC;
  u8_t f= rF & ~(flagN|flagZ|flagV|flagC);
  u16_t a= dest.R(), b= op, r;
  r= a-b;
  if (c && orgc) r--;
  if (r&0x8000) f|= flagN;
  if (!r) f|= flagZ;
  if (( (~a&b)|(b&r)|(r&~a) ) & 0x8000) f|= flagC;
  if (( (a&~b&~r)|(~a&b&r) ) & 0x8000) f|= flagV;
  dest.W(r);
  cF.W(f);
  return resGO;
}


int
CL11::add16(class cl_cell16 &dest, u16_t op, bool c)
{
  u8_t orgc= rF&flagC;
  u8_t f= rF & ~(flagN|flagZ|flagV|flagC);
  u16_t a= dest.R(), b= op, r;
  r= a+b;
  if (c && orgc) r++;
  if (r&0x8000) f|= flagN;
  if (!r) f|= flagZ;
  if (( (a&b)|(b&~r)|(~r&a) ) & 0x8000) f|= flagC;
  if (( (a&b&~r)|(~a&~b&r) ) & 0x8000) f|= flagV;
  dest.W(r);
  cF.W(f);
  return resGO;
}


int
CL11::bset(class cl_cell8 &dest)
{
  u8_t m= fetch();
  u8_t r= dest.R();
  u8_t f= rF & ~(flagN|flagZ);
  r|= m;
  dest.W(r);
  if (!r) f|= flagZ;
  if (r & 0x80) f|= flagN;
  cF.W(f);
  return resGO;
}


int
CL11::bclr(class cl_cell8 &dest)
{
  u8_t m= fetch();
  u8_t r= dest.R();
  u8_t f= rF & ~(flagN|flagZ|flagV);
  r&= ~m;
  dest.W(r);
  if (!r) f|= flagZ;
  if (r & 0x80) f|= flagN;
  cF.W(f);
  return resGO;
}


int
CL11::IDIV(t_mem code)
{
  u16_t q, r;
  u8_t f= rF & ~(flagZ|flagV|flagC);
  if (rX == 0)
    {
      q= 0xffff;
      r= rX;
      f|= flagC;
    }
  else
    {
      cX.W(q= rD/rX);
      cD.W(r= rD%rX);
    }
  if (!q) f|= flagZ;
  cF.W(f);
  return resGO;
}


int
CL11::FDIV(t_mem code)
{
  u32_t n= rD << 16;
  u16_t q, r;
  u8_t f= rF & ~(flagZ|flagV|flagC);
  if( rX <= rD) f|= flagV;
  if (rX == 0)
    {
      q= 0xffff;
      r= rX;
      f|= flagC;
    }
  else
    {
      cX.W(q= n/rX);
      cD.W(r= n%rX);
    }
  if (!q) f|= flagZ;
  cF.W(f);
  return resGO;
}


int
CL11::LSRD(t_mem code)
{
  u8_t newc= rB&1;
  u8_t f= rF & ~(flagN|flagZ|flagV|flagC);
  cD.W(rD>>1);
  if (!rD) f|= flagZ;
  if (newc) f|= (flagC|flagV);
  cF.W(f);
  return resGO;
}


int
CL11::ASLD(t_mem code)
{
  u8_t newc= rA&0x80;
  u8_t f= rF & ~(flagN|flagZ|flagV|flagC);
  cD.W(rD<<1);
  if (!rD) f|= flagZ;
  if (newc) f|= flagC;
  if (rA&0x80) f|= flagN;
  if ((f ^ (f<<3)) & flagN) f|= flagV;
  cF.W(f);
  return resGO;
}


int
CL11::ABX(t_mem code)
{
  cX.W(rX+rB);
  return resGO;
}


int
CL11::MUL(t_mem code)
{
  u8_t f= rF & ~flagC;
  u16_t r= rA * rB;
  cD.W(r);
  if (rB & 0x80) f|= flagC;
  cF.W(f);
  return resGO;
}


/*
 * BRANCH instructions
 */

int
CL11::brset(u8_t op)
{
  u8_t m= fetch();
  i8_t r= fetch();
  if ((~op & m) == 0)
    branch(PC+r, true);
  return resGO;
}


int
CL11::brclr(u8_t op)
{
  u8_t m= fetch();
  i8_t r= fetch();
  if ((op & m) == 0)
    branch(PC+r, true);
  return resGO;
}


int
CL11::BRN(t_mem code)
{
  fetch();
  return resGO;
}
  

/* End of m6800.src/m68hc11.cc */
