/*
 * Simulator of microcontrollers (p2223.cc)
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

#include "glob.h"
#include "pmon.h"

#include "p2223cl.h"


t_mem
cl_f_write::write(t_mem val)
{
  /*
  if (val&0x80) uc->dbg_reg= true;
  else if (val&0x40) uc->dbg_reg= false;
  */
  return val&0x3f;
}


CLP2::cl_p2223(class cl_sim *asim):
  cl_p1516(asim)
{
}

int
CLP2::init(void)
{
  cl_p1516::init();

  //dbg_reg= false;
  class cl_memory_chip *chip= rom_chip;
  t_addr a;
  t_mem v;
  int i;
  i= 0;
  a= pmon[i++];
  v= pmon[i++];
  while ((a!=0xffffffff) || (v!=0xffffffff))
    {
      chip->d(a, v);
      a= pmon[i++];
      v= pmon[i++];	
    }

  //class cl_f_write *fw= new cl_f_write(&cF, this);
  //fw->init();
  //cF.append_operator(fw);
  
  return 0;
}
  
const char *
CLP2::id_string(void)
{
  return "p2223";
}


struct dis_entry *
CLP2::dis_tbl(void)
{
  return(disass_p2223);
}

char *
CLP2::disassc(t_addr addr, chars *comment)
{
  chars work= chars(), temp= chars(), fmt;
  const char *b;
  t_mem code, data= 0;
  int i;
  bool first= true;
  t_addr a;
    
  code= rom->get(addr);
  
  i= 0;
  while ((code & dis_tbl()[i].mask) != dis_tbl()[i].code &&
	 dis_tbl()[i].mnemonic)
    i++;
  if (dis_tbl()[i].mnemonic == NULL)
    {
      return strdup("-- UNKNOWN/INVALID");
    }
  b= dis_tbl()[i].mnemonic;

  work= "";
  
  data= (code&0xf0000000)>>28;
  if (dis_tbl()[i].branch == 'M')
    work.append("   ");
  else
    {
      switch (data)
	{
	case   0: work.append("   "); break;
	case   1: work.append("EQ "); break;
	case   2: work.append("NE "); break;
	case   3: work.append("CS "); break;
	case   4: work.append("CC "); break;
	case   5: work.append("MI "); break;
	case   6: work.append("PL "); break;
	case   7: work.append("VS "); break;
	case   8: work.append("VC "); break;
	case 0x9: work.append("HI "); break;
	case 0xA: work.append("LS "); break;
	case 0xB: work.append("GE "); break;
	case 0xC: work.append("LT "); break;
	case 0xD: work.append("GT "); break;
	case 0xE: work.append("LE "); break;
	case 0xF: work.append("   "); break;
	}
    }

  first= true;
  for (i=0; b[i]; i++)
    {
      if ((b[i] == ' ') && first)
	{
	  first= false;
	  while (work.len() < 8) work.append(' ');
	}
      if (b[i] == '\'')
	{
	  fmt= "";
	  i++;
	  while (b[i] && (b[i]!='\''))
	    fmt.append(b[i++]);
	  if (!b[i]) i--;
	  if (fmt.empty())
	    work.append("'");
	  if (strcmp(fmt.c_str(), "*Ra") == 0)
	    {
	      data= (code & 0x000f0000)>>16;
	      work.appendf("*r%d", data);
	    }
	  if (strcmp(fmt.c_str(), "ar") == 0)
	    {
	      // CALL abs
	      u32_t ua;
	      ua= (code & 0x00ffffff);
	      a= ua;
	      work.appendf("0x%x", a);
	      addr_name(a, rom, &work);
	    }
	  if (strcmp(fmt.c_str(), "s20") == 0)
	    {
	      // CALL Rd,s20 ; indexed
	      i32_t ia= (code & 0x000fffff);
	      if (ia & 0x00080000) ia|= 0xfff00000;
	      data= (code & 0x00f00000)>>20;
	      work.appendf("%c0x%x", (ia<0)?'-':'+', (ia<0)?-ia:ia);
	      a= R[data]+ia;
	      if (comment)
		comment->format("; 0x%x%c0x%x=0x%x", R[data],
				(ia<0)?'-':'+',
				(ia<0)?-ia:ia, a);
	    }
	  if (strcmp(fmt.c_str(), "j") == 0)
	    {
	      // Macro jump
	      u32_t u16= code&0x0000ffff;
	      work.appendf("0x%x", u16);
	      addr_name(u16, rom, &work);
	    }
	  if (strcmp(fmt.c_str(), "jp") == 0)
	    {
	      // Macro jump: mov r15,rb
	      data= (code & 0x00000f00)>>8;
	      u32_t u32= R[data];
	      work.appendf("r%d", data);
	      if (comment)
		comment->appendf("; 0x%x", u32);
	      addr_name(u32, rom, &work);
	    }
	  if (strcmp(fmt.c_str(), "s16") == 0)
	    {
	      i32_t s16= code&0x0000ffff;
	      if (code & 0x00008000)
		s16|= 0xffff0000;
	      work.appendf("%c0x%x", (s16<0)?'-':'+',
			   (s16<0)?-s16:s16);
	    }
	  if (strcmp(fmt.c_str(), "u16") == 0)
	    {
	      u32_t u16= code&0x0000ffff;
	      work.appendf("0x%x", u16);
	    }
	  if (strcmp(fmt.c_str(), "and16") == 0)
	    {
	      i32_t s16= (code&0x0000ffff)|0xffff0000;
	      work.appendf("0x%08x", s16);
	    }
	  if (strcmp(fmt.c_str(), "s15") == 0)
	    {
	      i32_t s15= code&0x00007fff;
	      if (code & 0x00004000)
		s15|= 0xffff8000;
	      work.appendf("%c0x%x", (s15<0)?'-':'+',
			   (s15<0)?-s15:s15);
	    }
	  if (strcmp(fmt.c_str(), "*ra") == 0)
	    {
	      int ra= (code & 0x000f0000)>>16;
	      work.appendf("*r%d", ra);
	      if (comment)
		{
		  comment->appendf("; ");
		  bool u= F&U, p= F&P;
		  char c= u?'+':'-';
		  if (p)
		    comment->appendf("%cr%d", c, ra);
		  else
		    comment->appendf("r%d%c", ra, c);
		}
	    }
	  continue;
	}
      if (b[i] == '%')
	{
	  b++;
	  switch (b[i])
	    {
	    case 'd': // Rd
	      data= (code & 0x00f00000)>>20;
	      work.appendf("r%d", data);
	      break;
	    case 'a': // Ra
	      data= (code & 0x000f0000)>>16;
	      work.appendf("r%d", data);
	      break;
	    case 'R': // Ra in LD, ST
	      data= (code & 0x000f0000)>>16;
	      work.appendf("r%d", data);
	      if (comment)
		{
		  chars n= "";
		  addr_name(R[data], rom, &n);
		  comment->format("; [%08x%s]= %08x",
				  R[data],
				  n.c_str(),
				  rom->get(R[data]));
		}
	      break;
	    case 'b': // Rb
	      data= (code & 0x00000f00)>>8;
	      work.appendf("r%d", data);
	      break;
	    case '0': // LDL0
	      data= (code & 0x0000ffff);
	      work.appendf("0x0000%04x", data);
	      addr_name(data, rom, &work);
	      break;
	    case 'O': // LDL0 -> jump
	      data= (code & 0x0000ffff);
	      work.appendf("0x%04x", data);
	      addr_name(data, rom, &work);
	      break;
	    case 'l': // LDL
	      data= (code & 0x0000ffff);
	      work.appendf("0x....%04x", data);
	      break;
	    case 'h': // LDH
	      data= (code & 0x0000ffff);
	      work.appendf("0x%04x....", data);
	      break;
	    default:
	      temp= "?";
	      break;
	    }
	  if (comment && temp.nempty())
	    comment->append(temp);
	}
      else
	work+= b[i];
    }

  return strdup(work.c_str());
}

/*
void
CLP2::analyze_start(void)
{
}
*/

void
CLP2::analyze(t_addr addr)
{
  struct dis_entry *de;
  int i;
  
  while (!inst_at(addr))
    {
      t_mem code= rom->read(addr);
      i= 0;
      while ((code & dis_tbl()[i].mask) != dis_tbl()[i].code &&
	     dis_tbl()[i].mnemonic)
	i++;
      de= &dis_tbl()[i];
      if (de->mnemonic == NULL)
	return;
      set_inst_at(addr);
      if (de->branch!=' ')
	switch (de->branch)
	  {
	  case 'x': case '_': // non-followable
	    return;
	  case 'M': // LDL0 r15,#imm
	    {
	      t_addr target= rom->read(addr) & 0xffff;
	      analyze_jump(addr, target, de->branch);
	      break;
	    }
	  }
      addr= rom->validate_address(addr+1);
    }
}


void
CLP2::print_regs(class cl_console_base *con)
{
  int i;
  con->dd_color("answer");
  con->dd_printf("  F= 0x%02x  ", F);
  con->dd_printf("U=%c ", (F&U)?'1':'0');
  con->dd_printf("P=%c ", (F&P)?'1':'0');
  con->dd_printf("O=%c ", (F&O)?'1':'0');
  con->dd_printf("C=%c ", (F&C)?'1':'0');
  con->dd_printf("Z=%c ", (F&Z)?'1':'0');
  con->dd_printf("S=%c ", (F&S)?'1':'0');
  con->dd_printf("\n");
  for (i= 0; i<16; i++)
    {
      if (i<10) con->dd_printf(" ");
      con->dd_printf("R%d= 0x%08x ", i, R[i]);
      if (i<10) con->dd_printf(" ");
      con->dd_printf("[R%d]= 0x%08x", i, rom->get(R[i]));
      if (i%2)
	con->dd_printf("\n");
      else
	con->dd_printf(" ");
    }
  print_disass(PC, con);
}

bool
CLP2::cond(t_mem code)
{
  t_mem cond= (code & 0xf0000000) >> 28;
  u8_t n= (F&N)?1:0, v= (F&V)?1:0;
  switch (cond)
    {
      //case 0x0: return true;		// uncond
    case 0x1: return F&Z;		// EQ
    case 0x2: return !(F&Z);		// NE
    case 0x3: return F&C;		// CS HS
    case 0x4: return !(F&C);		// CC LO
    case 0x5: return F&S;		// MI
    case 0x6: return !(F&S);		// PL
    case 0x7: return F&O;		// VS
    case 0x8: return !(F&O);		// VC
    case 0x9: return (F&C) && !(F&Z);	// HI
    case 0xa: return !(F&C) || (F&Z);	// LS
    case 0xb: return !(n^v);		// GE
    case 0xc: return n^v;		// LT
    case 0xd: return !(F&Z) && !(n^v);	// GT
    case 0xe: return (F&Z) || (n^v);	// LE
      //case 0xf: return true;		// always
    }
  return true;
}

int
CLP2::inst_alu_1op(t_mem code)
{
  u8_t  d= (code & 0x00f00000) >> 20;
  u8_t op= (code & 0x000f0000) >> 16;
  u8_t c1, c2;
  switch (op)
    {
    case 0x0: // ZEXB
      RC[d]->W(R[d] & 0x000000ff);
      break;
    case 0x1: // ZEXW
      RC[d]->W(R[d] & 0x0000ffff);
      break;
    case 0x2: // SEXB
      R[d]= R[d] & 0x000000ff;
      if (R[d] & 0x00000080)
	R[d]|= 0xffffff00;
      RC[d]->W(R[d]);
      break;
    case 0x3: // SEXW
      R[d]= R[d] & 0x0000ffff;
      if (R[d] & 0x00008000)
	R[d]|= 0xffff0000;
      RC[d]->W(R[d]);
      break;
    case 0x4: // NOT
      RC[d]->W(~R[d]);
      setZSw(R[d]);
      break;
    case 0x5: // NEG
      {
	/*i32_t i32= R[d];
	i32= -i32;
	RC[d]->W(i32);
	setZSw(R[d]);*/
	RC[d]->W(inst_ad(0, ~R[d], 1));
      }
      break;
    case 0x6: // ROR
      c1= (F&C)?1:0;
      c2= R[d] & 1;
      R[d]= R[d] >> 1;
      if (c1)
	R[d]|= 0x80000000;
      RC[d]->W(R[d]);
      SET_C(c2);
      setZSw(R[d]);
      break;
    case 0x7: // ROL
      c1= (F&C)?1:0;
      c2= (R[d] & 0x80000000)?1:0;
      R[d]= (R[d]<<1) + c1;
      SET_C(c2);
      setZSw(R[d]);
      break;
    case 0x8: // SHL
      SET_C(R[d] & 0x80000000);
      RC[d]->W(R[d] << 1);
      setZSw(R[d]);
      break;
    case 0x9: // SHR
      SET_C(R[d] & 1);
      RC[d]->W(R[d] >> 1);
      setZSw(R[d]);
      break;
    case 0xa: //SHA      
      SET_C(R[d] & 1);
      RC[d]->W(((i32_t)(R[d])) >> 1);
      setZSw(R[d]);
      break;
    case 0xb: // SZ
      setZSw(R[d]);
      break;
    case 0xc: // SEC
      cF.W(F|C);
      break;
    case 0xd: // CLC
      cF.W(F&~C);
      break;
    case 0xe: // GETF
      RC[d]->W(F);
      break;
    case 0xf: // SETF
      cF.W(R[d] & 0xff/*0x3f*/);
      break;
    }
  return resGO;
}

int
CLP2::inst_alu(t_mem code)
{
  if ((code & 0x0e000000) == 0x02000000)
    return inst_alu_1op(code);
  u32_t uop, op2;
  i32_t iop;
  u8_t  d= (code & 0x00f00000) >> 20;
  u8_t op= (code & 0x000f0000) >> 16;
  if (code & 0x01000000)
    {
      // #u16
      op2= uop= iop= code & 0x0000ffff;
      if (iop & 0x00008000)
	iop|= 0xffff0000;
      switch (op)
	{
	case 0: // MVL
	  R[d]&= 0xffff0000;
	  RC[d]->W(R[d] | uop);
	  return resGO;
	case 1: // MVH
	  R[d]&= 0x0000ffff;
	  op2<<= 16;
	  RC[d]->W(R[d] | op2);
	  return resGO;
	case 2: // MVL0
	  RC[d]->W(uop);
	  return resGO;
	case 3: // MVS
	  RC[d]->W(iop);
	  return resGO;
	case 0xf: // AND
	  op2|= 0xffff0000;
	  RC[d]->W(R[d] & op2);
	  setZSw(R[d]);
	  return resGO;
	}
    }
  else
    {
      // Rb
      u8_t b= (code & 0x00000f00) >> 8;
      op2= uop= iop= R[b];
      switch (op)
	{
	case 0: // MOV
	  RC[d]->W(R[b]);
	  return resGO;
	case 1: // 
	  return resINV;
	case 2: // 
	  return resINV;
	case 3: // SEXD
	  op2= 0;
	  if (R[b] & 0x80000000)
	    op2= 0xffffffff;
	  RC[d]->W(op2);
	  return resGO;
	case 0xf: // AND
	  RC[d]->W(R[d] & op2);
	  setZSw(R[d]);
	  return resGO;
	case 0xb:
	  return resINV;
	}
    }
  switch (op)
    {
    case 0x4: // ADD
      RC[d]->W(inst_ad(R[d], iop, 0));
      break;
    case 0x5: // ADC
      RC[d]->W(inst_ad(R[d], iop, (F&C)?1:0));
      break;
    case 0x6: // SUB
      RC[d]->W(inst_ad(R[d], ~iop, 1));
      break;
    case 0x7: // SBB
      RC[d]->W(inst_ad(R[d], ~iop, (F&C)?1:0));
      break;
    case 0x8: // CMP
      inst_ad(R[d], ~iop, 1);
      break;
    case 0x9: // MUL
      RC[d]->W(R[d] * iop);
      setZSw(R[d]);
      break;
    case 0xa: // PLUS
      RC[d]->W(R[d]+iop);
      break;
    case 0xb: // BTST
      {
	u32_t r= R[d] & uop;
	setZSw(r);
      }
    case 0xc: // TEST
      setZSw(R[d] & uop);
      break;
    case 0xd: // OR
      RC[d]->W(R[d] | uop);
      setZSw(R[d]);
      break;
    case 0xe: // XOR
      RC[d]->W(R[d] ^ uop);
      setZSw(R[d]);
      break;
    }
  return resGO;
}

int
CLP2::inst_mem(t_mem code)
{
  u8_t d, a, b;
  i32_t offset;
  bool w= (code&0x01000000), u, p;
  d= (code & 0x00f00000) >> 20;
  a= (code & 0x000f0000) >> 16;
  b= (code & 0x00000f00) >> 8;
  if (code & 0x04000000)
    {
      offset= code & 0x0000ffff;
      if (offset & 0x00008000)
	offset|= 0xffff0000;
      u= F&U;
      p= F&P;
    }
  else
    {
      offset= R[b];
      u= code & 0x00008000;
      p= code & 0x00004000;
    }
  if ((code & 0x02000000) &&
      (code & 0x04000000))
    {
      // LD #im offset
      u= !u;
      p= !p;
    }
  t_addr addr, opa= R[a];
  t_addr opa_chg, base;
  opa_chg= opa+(u?+1:-1);
  base= p?opa_chg:opa;
  t_addr opa_offset= opa+offset;
  t_addr base_offset= base+offset;
  if (w)
    addr= base_offset;
  else
    addr= opa_offset;
  
  if (code & 0x02000000)
    // LD
    RC[d]->W(rom->read(addr));
  else
    // ST
    rom->write(addr, R[d]);
  
  if (w)
    RC[a]->W(opa_chg);
  
  return resGO;
}

int
CLP2::inst_ext(t_mem code)
{
  t_mem cod= (code & 0x000f000)>>16;
  int d;
  t_addr addr;
  switch (cod)
    {
    case 0:
      d= (code & 0x00f00000) >> 20;
      addr= code&0xffff;
      if (code & 0x01000000)
	{
	  // LD direct
	  RC[d]->W(rom->read(addr));
	}
      else
	{
	  // ST direct
	  rom->write(addr, R[d]);
	}
      return resGO;
    }
  return resINV;
}

int
CLP2::exec_inst(void)
{
  t_mem code;
  u8_t inst;
  bool fe;
  
  PC= R[15];
  instPC= PC;
  fe= fetch(&code);
  //if (dbg_reg) fprintf(stderr, "F: %08x %08x %08x\n", instPC, code, F);
  tick(4);
  R[15]= PC;
  if (fe)
    return(resBREAKPOINT);

  if (!cond(code))
    return resGO;
  
  u8_t d;
  d= (code & 0x00f00000) >> 20;
  inst= (code & 0x0e000000) >> 25;
  int ret= resGO;
  switch (inst)
    {
    case 0: case 1:
      ret= inst_alu(code);
      break;
    case 2:
      {
	t_addr call_a;
	i32_t i32;
	// CALL
	if (code & 0x01000000)
	  {
	    // CALL Rd,s20
	    i32= (code & 0x000fffff);
	    if (i32 & 0x00080000)
	      i32|= 0xfff00000;
	    call_a= R[d]+i32;
	  }
	else
	  {
	    // CALL abs
	    call_a= code & 0x00ffffff;
	  }
	RC[14]->W(R[15]);
	RC[15]->W(PC= call_a);
	return resGO;
      }
    case 3:
      ret= inst_ext(code);
      break;
    case 4: case 5: case 6: case 7:
      ret= inst_mem(code);
      break;
    }
  PC= R[15];
  
  return ret;
}

/* End of p1516.src/p2223.cc */
