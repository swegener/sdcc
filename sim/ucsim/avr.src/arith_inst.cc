/*
 * Simulator of microcontrollers (arith_inst.cc)
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

#include "avrcl.h"
#include "regsavr.h"


int
cl_avr::cpi_Rd_K(t_mem code)
{
  return(resGO);
}


int
cl_avr::sbci_Rd_K(t_mem code)
{
  return(resGO);
}


int
cl_avr::subi_Rd_K(t_mem code)
{
  return(resGO);
}


int
cl_avr::muls_Rd_Rr(t_mem code)
{
  return(resGO);
}


int
cl_avr::mulsu_Rd_Rr(t_mem code)
{
  return(resGO);
}


int
cl_avr::fmul_Rd_Rr(t_mem code)
{
  return(resGO);
}


int
cl_avr::fmuls_Rd_Rr(t_mem code)
{
  return(resGO);
}


int
cl_avr::fmulsu_Rd_Rr(t_mem code)
{
  return(resGO);
}


int
cl_avr::cpc_Rd_Rr(t_mem code)
{
  return(resGO);
}


int
cl_avr::sbc_Rd_Rr(t_mem code)
{
  return(resGO);
}


/*
 * Add without Carry
 * ADD Rd,Rr 0<=d<=31, 0<=r<=31
 * 0000 11rd dddd rrrr
 *____________________________________________________________________________
 */

int
cl_avr::add_Rd_Rr(t_mem code)
{
  t_addr r, d;
  t_mem R, D, result, res;

  d= (code&0x1f0)>>4;
  r= ((code&0x200)>>5)|(code&0xf);
  R= ram->read(r);
  D= ram->read(d);
  result= D+R;
  res= result & 0xff;
  ram->write(d, &res);
  
  t_mem sreg= ram->get(SREG);
  if (!res)
    sreg|= BIT_Z;
  else
    sreg&= ~BIT_Z;
  if (((D&R&~res)&0x80) ||
      ((~D&~R&res)&0x80))
    sreg|= (BIT_V|BIT_S);
  else
    sreg&= ~(BIT_V|BIT_S);
  if (res & 0x80)
    {
      sreg|= BIT_N;
      sreg^= BIT_S;
    }
  else
    sreg&= ~BIT_N;
  if (result & ~0xff)
    sreg|= BIT_C;
  else
    sreg&= ~BIT_C;
  if ((R&0xf) + (D&0xf) > 15)
    sreg|= BIT_H;
  else
    sreg&= ~BIT_H;
  ram->set(SREG, sreg);

  return(resGO);
}


int
cl_avr::cp_Rd_Rr(t_mem code)
{
  return(resGO);
}


int
cl_avr::sub_Rd_Rr(t_mem code)
{
  return(resGO);
}


/*
 * Add with Carry
 * ADC Rd,Rr 0<=d<=31, 0<=r<=31
 * 0001 11rd dddd rrrr
 *____________________________________________________________________________
 */

int
cl_avr::adc_Rd_Rr(t_mem code)
{
  t_addr r, d;
  t_mem R, D, result, res;

  d= (code&0x1f0)>>4;
  r= ((code&0x200)>>5)|(code&0xf);
  R= ram->read(r);
  D= ram->read(d);
  t_mem sreg= ram->get(SREG);
  result= D+R+((sreg&BIT_C)?1:0);
  res= result & 0xff;
  ram->write(d, &res);
  
  if (!res)
    sreg|= BIT_Z;
  else
    sreg&= ~BIT_Z;
  if (((D&R&~res)&0x80) ||
      ((~D&~R&res)&0x80))
    sreg|= (BIT_V|BIT_S);
  else
    sreg&= ~(BIT_V|BIT_S);
  if (res & 0x80)
    {
      sreg|= BIT_N;
      sreg^= BIT_S;
    }
  else
    sreg&= ~BIT_N;
  if (result & ~0xff)
    sreg|= BIT_C;
  else
    sreg&= ~BIT_C;
  if ((R&0xf) + (D&0xf) > 15)
    sreg|= BIT_H;
  else
    sreg&= ~BIT_H;
  ram->set(SREG, sreg);

  return(resGO);
}


int
cl_avr::com_Rd(t_mem code)
{
  return(resGO);
}


int
cl_avr::neg_Rd(t_mem code)
{
  return(resGO);
}


/*
 * Increment
 * INC Rd 0<=d<=31
 * 1001 010d dddd 0011
 *____________________________________________________________________________
 */

int
cl_avr::inc_Rd(t_mem code)
{
  t_addr d;

  d= (code&0x1f0)>>4;
  t_mem data= ram->read(d)+1;
  ram->write(d, &data);

  t_mem sreg= ram->get(SREG);
  data= data&0xff;
  if (data & 0x80)
    {
      sreg|= (BIT_N);
      if (data == 0x80)
	{
	  sreg|= BIT_V;
	  sreg&= ~BIT_S;
	}
      else
	{
	  sreg&= ~BIT_V;
	  sreg|= BIT_S;
	}
      sreg&= ~BIT_Z;
    }
  else
    {
      sreg&= ~(BIT_N|BIT_V|BIT_S);
      if (!data)
	sreg|= BIT_Z;
      else
	sreg&= ~BIT_Z;
    }
  ram->set(SREG, sreg);
  return(resGO);
}


int
cl_avr::asr_Rd(t_mem code)
{
  return(resGO);
}


int
cl_avr::lsr_Rd(t_mem code)
{
  return(resGO);
}


int
cl_avr::ror_Rd(t_mem code)
{
  return(resGO);
}


int
cl_avr::dec_Rd(t_mem code)
{
  return(resGO);
}


int
cl_avr::mul_Rd_Rr(t_mem code)
{
  return(resGO);
}


/*
 * Add Immediate to Word
 * ADIW Rdl,K dl={24,26,28,30}, 0<=K<=63
 * 1001 0110 KK dd KKKK
 *____________________________________________________________________________
 */

int
cl_avr::adiw_Rdl_K(t_mem code)
{
  t_addr dl;
  t_mem D, K, result, res;

  dl= 24+(2*((code&0x30)>>4));
  K= ((code&0xc0)>>2)|(code&0xf);
  D= ram->read(dl+1)*256 + ram->read(dl);
  result= D+K;
  res= result & 0xffff;
  t_mem resl= result&0xff, resh= (result>>8)&0xff;
  ram->write(dl+1, &resh);
  ram->write(dl, &resl);
  
  t_mem sreg= ram->get(SREG);
  if (!res)
    sreg|= BIT_Z;
  else
    sreg&= ~BIT_Z;
  if (D&res&0x8000)
    sreg|= (BIT_V|BIT_S);
  else
    sreg&= ~(BIT_V|BIT_S);
  if (res & 0x8000)
    {
      sreg|= BIT_N;
      sreg^= BIT_S;
    }
  else
    sreg&= ~BIT_N;
  if ((~res)&D&0x8000)
    sreg|= BIT_C;
  else
    sreg&= ~BIT_C;
  ram->set(SREG, sreg);
  tick(1);

  return(resGO);
}


/* End of avr.src/arith_inst.cc */
