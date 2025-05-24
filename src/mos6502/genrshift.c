/*-------------------------------------------------------------------------
  genrshift.c - source file for right shift code generation for the MOS6502

  Copyright (C) 1998, Sandeep Dutta . sandeep.dutta@usa.net
  Copyright (C) 1999, Jean-Louis VERN.jlvern@writeme.com
  Bug Fixes - Wojciech Stryjewski  wstryj1@tiger.lsu.edu (1999 v2.1.9a)
  Hacked for the HC08:
  Copyright (C) 2003, Erik Petrich
  Hacked for the MOS6502:
  Copyright (C) 2020, Steven Hugg  hugg@fasterlight.com
  Copyright (C) 2021-2025, Gabriele Gorla

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 2, or (at your option) any
  later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
  -------------------------------------------------------------------------*/

#include "m6502.h"
#include "ralloc.h"
#include "gen.h"
#include "dbuf_string.h"

/**************************************************************************
 * AccSRsh - signed right shift accumulator by known count
 *************************************************************************/
static void AccSRsh (int shCount)
{
  int i;

  shCount &= 0x0007;            // shCount : 0..7

  if (shCount == 7)
    {
      emit6502op ("rol", "a");
      loadRegFromConst(m6502_reg_a, 0);
      emit6502op ("adc", "#0xff");
      emit6502op ("eor", "#0xff");
    }
  else if (shCount == 6)
    {
      symbol *tlbl = safeNewiTempLabel (NULL);
      emit6502op ("ora", "#0x3f");
      emitSetCarry(1);
      emit6502op ("bmi", "%05d$", safeLabelNum (tlbl));
      emit6502op ("and", "#0xc0");
      emitSetCarry(0);
      safeEmitLabel(tlbl);
      emit6502op ("rol", "a");
      emit6502op ("rol", "a");
      emit6502op ("rol", "a");
    }
  else
    {
      // TODO: optimize?
      for (i = 0; i < shCount; i++)
        {
          emitCmp (m6502_reg_a, 0x80);
          emit6502op ("ror", "a");
        }
    }
}

/**************************************************************************
 * AccRsh - right shift accumulator by known count
 *************************************************************************/
void AccRsh (int shCount, bool sign)
{
  int i;

  if (sign) {
    AccSRsh (shCount);
    return;
  }

  shCount &= 0x0007;            // shCount : 0..7

  /* For shift counts of 6 and 7, the unrolled loop is never optimal.      */
  if (shCount==7)
    {
      emit6502op ("rol", "a");
      loadRegFromConst(m6502_reg_a, 0);
      emit6502op ("rol", "a");
      /* total: 6 cycles, 4 bytes */
    }
  else if(shCount==6)
    {
      emit6502op ("rol", "a");
      emit6502op ("rol", "a");
      emit6502op ("rol", "a");
      emit6502op ("and", "#0x03");
      /* total: 8 cycles, 5 bytes */
    }
  else
    {
      /* lsra is 2 cycles and 1 byte, so an unrolled loop is the      */
      /* the fastest and shortest (shCount<6).            */
      for (i = 0; i < shCount; i++)
        emit6502op ("lsr", "a");
    }
}

/**************************************************************************
 * XAccSRsh - signed right shift register pair XA by known count
 *************************************************************************/
static void XAccSRsh (int shCount)
{
  symbol *tlbl;
  int i;

  shCount &= 0x000f;            // shCount : 0..7

  /* if we can beat 2n cycles or bytes for some special case, do it here */
  switch (shCount) { // TODO
  case 15:
  case 14:
  case 13:
  case 12:
  case 11:
  case 10:
  case 9:
    transferRegReg (m6502_reg_x, m6502_reg_a, false);
    loadRegFromConst (m6502_reg_x, 0);
    AccSRsh (shCount - 8);
    tlbl = safeNewiTempLabel (NULL);
    emit6502op ("bpl", "%05d$", safeLabelNum (tlbl));
    loadRegFromConst (m6502_reg_x, 0xff);
    safeEmitLabel(tlbl);
    break;
  case 8:
    transferRegReg (m6502_reg_x, m6502_reg_a, false);
    loadRegFromConst (m6502_reg_x, 0);
    emit6502op("cmp","#0x00");
    tlbl = safeNewiTempLabel (NULL);
    emit6502op ("bpl", "%05d$", safeLabelNum (tlbl));
    loadRegFromConst (m6502_reg_x, 0xff);
    safeEmitLabel(tlbl);
    break;

  default:
    // TODO: handle when X is litconst
    storeRegTempAlways(m6502_reg_x, true);
    for (i = 0; i < shCount; i++)
      {
	// TODO: see if theree is a better way
	emit6502op ("cpx", "#0x80");
	emit6502op ("ror", TEMPFMT, getLastTempOfs() );
	dirtyRegTemp(getLastTempOfs() );
	rmwWithReg ("ror", m6502_reg_a);
      }
    loadRegTemp(m6502_reg_x);
  }
}

/**************************************************************************
 * XAccRsh - right shift register pair XA by known count
 *************************************************************************/
void XAccRsh (int shCount, bool sign)
{
  int i;

  shCount &= 0x000f;            // shCount : 0..15

  if (sign)
    {
      XAccSRsh (shCount);
      return;
    }

  if(shCount>=8)
    {
      transferRegReg(m6502_reg_x, m6502_reg_a, true);
      AccRsh (shCount - 8, false);
      loadRegFromConst (m6502_reg_x, 0);
    }
  else if(shCount==7)
    {
      storeRegTempAlways(m6502_reg_x, true);
      rmwWithReg ("rol", m6502_reg_a);
      emit6502op ("rol", TEMPFMT, getLastTempOfs() );
      dirtyRegTemp(getLastTempOfs() );
      loadRegFromConst (m6502_reg_a, 0);
      rmwWithReg ("rol", m6502_reg_a);
      transferRegReg(m6502_reg_a, m6502_reg_x, true);
      loadRegTemp(m6502_reg_a);
    }
  else if(shCount!=0)
    {
      /* lsr/rora is only 2 cycles and bytes, so an unrolled loop is often  */
      /* the fastest and shortest.                                           */
      storeRegTempAlways(m6502_reg_x, true);
      for (i = 0; i < shCount; i++)
        {
          emit6502op ("lsr", TEMPFMT, getLastTempOfs() );
          rmwWithReg ("ror", m6502_reg_a);
          dirtyRegTemp(getLastTempOfs() );
        }
      loadRegTemp(m6502_reg_x);
    }
}

/**************************************************************************
 * genrsh8 - right shift a one byte quantity by known count
 *************************************************************************/
static void
genrsh8 (operand * result, operand * left, int shCount, int sign)
{
  bool needpulla = false;

  emitComment (TRACEGEN, "  %s - shift=%d", __func__, shCount);
  if (shCount==0)
    return;

  if (!IS_AOP_A(AOP(result)) && sameRegs (AOP (left), AOP (result)) 
      && shCount<3 && aopCanShift(AOP(left)) && !sign)
    {
      while (shCount--)
        rmwWithAop ("lsr", AOP (result), 0);
    }
  else
    {
      if(!IS_AOP_A(AOP(result)))
	needpulla = pushRegIfSurv (m6502_reg_a);
      loadRegFromAop (m6502_reg_a, AOP (left), 0);
      AccRsh (shCount, sign);
      storeRegToFullAop (m6502_reg_a, AOP (result), sign);
      pullOrFreeReg (m6502_reg_a, needpulla);
    }
}

/**************************************************************************
 * genrsh16 - right shift two bytes by known amount != 0
 *************************************************************************/
static void
genrsh16 (operand * result, operand * left, int shCount, int sign)
{
  bool needpulla, needpullx;
  emitComment (TRACEGEN, "  %s - shift=%d", __func__, shCount);

  if (shCount >= 8)
    {
      if (shCount != 8 || sign)
	{
	  needpulla = pushRegIfSurv (m6502_reg_a);
	  loadRegFromAop (m6502_reg_a, AOP (left), 1);
	  AccRsh (shCount - 8, sign);
	  storeRegToFullAop (m6502_reg_a, AOP (result), sign);
	  pullOrFreeReg (m6502_reg_a, needpulla);
	}
      else
	{
	  transferAopAop (AOP (left), 1, AOP (result), 0);
	  storeConstToAop (0, AOP (result), 1);
	}
    }
  else
    {
      /*  1 <= shCount <= 7 */
      needpulla = storeRegTempIfSurv (m6502_reg_a);
      needpullx = storeRegTempIfSurv (m6502_reg_x);
      loadRegFromAop (m6502_reg_xa, AOP (left), 0);
      XAccRsh (shCount, sign);
      storeRegToAop (m6502_reg_xa, AOP (result), 0);
      loadOrFreeRegTemp (m6502_reg_x, needpullx);
      loadOrFreeRegTemp (m6502_reg_a, needpulla);
    }
}

/**************************************************************************
 * shiftRLongInPlace - shift left one long in place
 *
 * @param result pointer to the dst aop
 * @param shift  number of shifts (must be >=0 and <8)
 * @param ofs    LSB to begin the shift (must be >=0 and <4)
 * @param msb_in_a MSB is already in A
 *************************************************************************/
static void
shiftRLongInPlace (operand * result, int shift, int ofs, int sign, bool msb_in_a)
{
  int i;

  if(shift==0)
    return;

#if 0
  if(!msb_in_a && shift==1 && !sign /*|| (shift==2 && AOP_TYPE(result)==AOP_DIR) */ )
    {
      while(shift--)
	{
	  rmwWithAop ("lsr", AOP(result), 3-ofs);
          for(i=2-ofs; i>=0; i--)
	    rmwWithAop ("ror", AOP(result), i);
	}
      else
	{

	  if(!msb_in_a)
	    loadRegFromAop (m6502_reg_a, AOP (result), 0);
	  while(shift)
	    {
	      if(ofs<3)
		rmwWithAop ("lsr", AOP(result), ofs);
	      if(ofs<2)
		rmwWithAop ("ror", AOP(result), ofs+1);
	      if(ofs<1)
		rmwWithAop ("ror", AOP(result), ofs+2);

	      if(ofs==3)
		rmwWithReg ("lsr", m6502_reg_a);
	      else
		rmwWithReg ("ror", m6502_reg_a);
	      --shift;
	    }
	  if(!msb_in_a)
	    storeRegToAop (m6502_reg_a, AOP (result), 0);
	}
    }
#else
  if(!msb_in_a)
    loadRegFromAop (m6502_reg_a, AOP (result), 3-ofs);

  while(shift)
    {
      if(sign)
	{
	  emitCmp(m6502_reg_a, 0x80);
	  rmwWithReg ("ror", m6502_reg_a);
	}
      else
	rmwWithReg ("lsr", m6502_reg_a);
      for(i=2-ofs;i>=0;i--)
	rmwWithAop ("ror", AOP(result), i);

      --shift;
    }
  if(!msb_in_a)
    storeRegToAop (m6502_reg_a, AOP (result), 3-ofs);
#endif
}

/**************************************************************************
 * shiftRLong1 - shift right one long from left to result
 *
 * @param left  pointer to the src aop
 * @param result  pointer to the dst aop
 * @param shift  number of shifts (must be >=16 and <24)
 *************************************************************************/
static void
shiftRLong1 (operand * left, operand * result, int shift, int sign)
{
  bool needpulla = false;

  wassertl(shift>=24, "shiftRLong1 - shift<24");
  wassertl(shift<32, "shiftRLong1 - shift>=32");

  needpulla = pushRegIfUsed (m6502_reg_a);

  if(shift==24)
    {
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      //	  storeRegToAop (m6502_reg_a, AOP (result), 0);
      shift=0;
    }
  else if (shift>29)
    {
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      emit6502op ("asl", "a");

      if(shift!=31)
	storeRegTemp(m6502_reg_a, true);
      if(sign)
	{
	  //	  signExtendA();
	  //  emit6502op ("asl", "a");
	  loadRegFromConst (m6502_reg_a, 0);
	  emit6502op ("adc", "#0xff");
	  emit6502op ("eor", "#0xff");
	  storeRegToAop (m6502_reg_a, AOP (result), 0);
	  storeRegToAop (m6502_reg_a, AOP (result), 1);
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	  storeRegToAop (m6502_reg_a, AOP (result), 3);
	}
      else
	{
	  //	  rmwWithReg ("asl", m6502_reg_a);
	  loadRegFromConst(m6502_reg_a, 0);
	  storeRegToAop (m6502_reg_a, AOP (result), 3);
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	  storeRegToAop (m6502_reg_a, AOP (result), 1);
	  rmwWithReg ("rol", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 0);
	}

      if(shift!=31)
        {
	  while(shift!=31)
	    {
	      emitRegTempOp( "rol", getLastTempOfs() );
	      rmwWithAop ("rol", AOP (result), 0);
              shift++;
	    }
	  loadRegTemp(NULL);
        }

      goto release;
    }
  else
    {
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      if(sign)
	{
	  emitCmp(m6502_reg_a, 0x80);     
	  rmwWithReg ("ror", m6502_reg_a);
        }
      else
	rmwWithReg ("lsr", m6502_reg_a);

      shift-=25;
    }

  shiftRLongInPlace (result, shift, 3, sign, true);
  storeRegToAop (m6502_reg_a, AOP (result), 0);

  if(sign)
    {
      signExtendA();
      storeRegToAop (m6502_reg_a, AOP (result), 1);
      storeRegToAop (m6502_reg_a, AOP (result), 2);
      storeRegToAop (m6502_reg_a, AOP (result), 3);
    }
  else
    {
      storeConstToAop (0, AOP (result), 1);
      storeConstToAop (0, AOP (result), 2);
      storeConstToAop (0, AOP (result), 3);
    }

 release:
  pullOrFreeReg (m6502_reg_a, needpulla);
}

/**************************************************************************
 * shiftRLong2 - shift right one long from left to result
 *
 * @param left  pointer to the src aop
 * @param result  pointer to the dst aop
 * @param shift  number of shifts (must be >=16 and <24)
 *************************************************************************/
static void
shiftRLong2 (operand * left, operand * result, int shift, int sign)
{
  bool needpulla = false;

  wassertl(shift>=16, "shiftRLong3 - shift<16");
  wassertl(shift<24, "shiftRLong3 - shift>23");

  needpulla = pushRegIfUsed (m6502_reg_a);

  if(shift==16)
    {
      loadRegFromAop (m6502_reg_a, AOP (left), 2);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      storeRegToAop (m6502_reg_a, AOP (result), 1);
      shift=0;
    }
  else if(shift>20)
    {
      loadRegFromAop (m6502_reg_a, AOP (left), 2);
      rmwWithReg ("rol", m6502_reg_a);
      if(shift!=23)
	storeRegTemp(m6502_reg_a, true);
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      rmwWithReg ("rol", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      loadRegFromConst (m6502_reg_a, 0);
      if(sign)
	{
	  emit6502op ("adc", "#0xff");
	  emit6502op ("eor", "#0xff");
	  storeRegToAop (m6502_reg_a, AOP (result), 1);
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	  storeRegToAop (m6502_reg_a, AOP (result), 3);
	}
      else
	{
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	  storeRegToAop (m6502_reg_a, AOP (result), 3);
          rmwWithReg ("rol", m6502_reg_a);
          storeRegToAop (m6502_reg_a, AOP (result), 1);
	}

      if(shift!=23)
        {
	  while(shift!=23)
	    {
	      emitRegTempOp( "rol", getLastTempOfs() );
	      rmwWithAop ("rol", AOP (result), 0);
	      rmwWithAop ("rol", AOP (result), 1);
              shift++;
	    }
	  loadRegTemp(NULL);
        }

      goto release;
    }
  else
    {
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      if(sign)
	{
	  emitCmp(m6502_reg_a, 0x80);
	  rmwWithReg ("ror", m6502_reg_a);
	}
      else
	rmwWithReg ("lsr", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 1);
      loadRegFromAop (m6502_reg_a, AOP (left), 2);
      rmwWithReg ("ror", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 0);


      shift-=17;
      loadRegFromAop (m6502_reg_a, AOP (result), 1);
 
    }

  shiftRLongInPlace (result, shift, 2, sign, false);

  if(sign)
    {
      signExtendA();
      storeRegToAop (m6502_reg_a, AOP (result), 2);
      storeRegToAop (m6502_reg_a, AOP (result), 3);

    }
  else
    {
      storeConstToAop (0, AOP (result), 2);
      storeConstToAop (0, AOP (result), 3);
    }

 release:
  pullOrFreeReg (m6502_reg_a, needpulla);
}

/**************************************************************************
 * shiftRLong3 - shift right one long from left to result
 *
 * @param left  pointer to the src aop
 * @param result  pointer to the dst aop
 * @param shift  number of shifts (must be >0 and <8)
 *************************************************************************/
static void
shiftRLong3 (operand * left, operand * result, int shift, int sign)
{
  bool needpulla = false;
  bool needloadx = false;

  wassertl(shift>=8, "shiftRLong3 - shift<8");
  wassertl(shift<16, "shiftRLong3 - shift>15");

  needpulla = pushRegIfUsed (m6502_reg_a);

  if(shift==8)
    {
      loadRegFromAop (m6502_reg_a, AOP (left), 1);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      loadRegFromAop (m6502_reg_a, AOP (left), 2);
      storeRegToAop (m6502_reg_a, AOP (result), 1);
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      storeRegToAop (m6502_reg_a, AOP (result), 2);
      shift=0;
    }
  else if(shift>12)
    {
      loadRegFromAop (m6502_reg_a, AOP (left), 1);
      rmwWithReg ("rol", m6502_reg_a);
      if(shift!=15)
	storeRegTemp(m6502_reg_a, true);
      loadRegFromAop (m6502_reg_a, AOP (left), 2);
      rmwWithReg ("rol", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      rmwWithReg ("rol", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 1);
      if(sign)
	{
	  loadRegFromConst(m6502_reg_a, 0);
	  emit6502op("adc", "#0xff");
	  emit6502op("eor", "#0xff");
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	  storeRegToAop (m6502_reg_a, AOP (result), 3);
	}
      else
	{
          loadRegFromConst(m6502_reg_a, 0);
	  storeRegToAop (m6502_reg_a, AOP (result), 3); // out of order store
	  rmwWithReg ("rol", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	}

      if(shift!=15)
        {
	  while(shift!=15)
	    {
	      emitRegTempOp( "rol", getLastTempOfs() );
	      rmwWithAop ("rol", AOP (result), 0);
	      rmwWithAop ("rol", AOP (result), 1);
	      rmwWithAop ("rol", AOP (result), 2);
              shift++;
	    }
	  loadRegTemp(NULL);
        }

      goto release;
    }
  else if (shift==9 && sameRegs (AOP (left), AOP (result)) && sign)
    {
      needloadx = storeRegTempIfUsed (m6502_reg_x);
      loadRegFromConst(m6502_reg_x, 0);
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      //            if(sign)
      //              {
      symbol *tlbl = safeNewiTempLabel (NULL);

      emitCmp(m6502_reg_a, 0x80);
      emitBranch ("bcc", tlbl);
      rmwWithReg ("dec", m6502_reg_x);
      safeEmitLabel(tlbl);
      rmwWithReg ("ror", m6502_reg_a);
      storeRegToAop (m6502_reg_x, AOP (result), 3);
      //              }
      //            else
      //	      rmwWithReg ("lsr", m6502_reg_a);

      loadRegFromAop (m6502_reg_x, AOP (left), 2);
      storeRegToAop (m6502_reg_a, AOP (result), 2);
      transferRegReg(m6502_reg_x, m6502_reg_a, true);
      rmwWithReg ("ror", m6502_reg_a);
      loadRegFromAop (m6502_reg_x, AOP (left), 1);
      storeRegToAop (m6502_reg_a, AOP (result), 1);
      transferRegReg(m6502_reg_x, m6502_reg_a, true);
      rmwWithReg ("ror", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 0);

      goto release;
    }
  else
    {
      if(!sameRegs (AOP (left), AOP (result)) && shift==12)
	{
	  loadRegFromAop (m6502_reg_a, AOP (left), 3);
	  rmwWithReg ("lsr", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	  loadRegFromAop (m6502_reg_a, AOP (left), 2);
          rmwWithReg ("ror", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 1);
	  loadRegFromAop (m6502_reg_a, AOP (left), 1);
          rmwWithReg ("ror", m6502_reg_a);
	  //	  storeRegToAop (m6502_reg_a, AOP (result), 0);
          shift-=9;
	  for(int i=0;i<shift;i++)
	    {
	      rmwWithAop ("lsr", AOP(result), 2);
	      rmwWithAop ("ror", AOP(result), 1);
	      rmwWithReg ("ror", m6502_reg_a);
	    }
	  storeRegToAop (m6502_reg_a, AOP (result), 0);
          if(sign)
	    {
	      loadRegFromAop (m6502_reg_a, AOP (left), 3);
	      signExtendA();
	      storeRegToAop (m6502_reg_a, AOP (result), 3);
	      emit6502op("and", "#0xf0");
	      rmwWithAop ("ora", AOP(result), 2);
	      storeRegToAop (m6502_reg_a, AOP (result), 2);          
	    }
          else
	    {
	      storeConstToAop (0, AOP (result), 3);
	    }
	  goto release;
	}
      else if(!sameRegs (AOP (left), AOP (result)))
	{
	  loadRegFromAop (m6502_reg_a, AOP (left), 3);
	  if(sign)
	    {
	      emitCmp(m6502_reg_a, 0x80);
	      rmwWithReg ("ror", m6502_reg_a);
	    }
	  else
	    rmwWithReg ("lsr", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	  loadRegFromAop (m6502_reg_a, AOP (left), 2);
	  rmwWithReg ("ror", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 1);
	  loadRegFromAop (m6502_reg_a, AOP (left), 1);
	  rmwWithReg ("ror", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 0);
	}
      else
        {
          needloadx = storeRegTempIfUsed (m6502_reg_x);
	  loadRegFromAop (m6502_reg_a, AOP (left), 3);
	  if(sign)
	    {
	      emitCmp(m6502_reg_a, 0x80);
	      rmwWithReg ("ror", m6502_reg_a);
	    }
	  else
	    rmwWithReg ("lsr", m6502_reg_a);

	  loadRegFromAop (m6502_reg_x, AOP (left), 2);
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	  transferRegReg(m6502_reg_x, m6502_reg_a, true);
	  rmwWithReg ("ror", m6502_reg_a);
	  loadRegFromAop (m6502_reg_x, AOP (left), 1);
	  storeRegToAop (m6502_reg_a, AOP (result), 1);
	  transferRegReg(m6502_reg_x, m6502_reg_a, true);
	  rmwWithReg ("ror", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 0);
          m6502_dirtyReg(m6502_reg_x);
        }
      shift-=9;
    }
  shiftRLongInPlace (result, shift, 1, sign, false);

  if(sign)
    {
      loadRegFromAop (m6502_reg_a, AOP (result), 2);
      signExtendA();
      storeRegToAop (m6502_reg_a, AOP (result), 3);
    }
  else
    {
      storeConstToAop (0, AOP (result), 3);
    }

 release:
  loadOrFreeRegTemp(m6502_reg_x, needloadx);
  pullOrFreeReg (m6502_reg_a, needpulla);
}

/**************************************************************************
 * shiftRLong4 - shift right one long from left to result
 *
 * @param left  pointer to the src aop
 * @param result  pointer to the dst aop
 * @param shift  number of shifts (must be >0 and <8)
 *************************************************************************/
static void
shiftRLong4 (operand * left, operand * result, int shift, int sign)
{
  bool needpulla = false;

  wassertl(shift>0, "shiftRLong4 - shift==0");
  wassertl(shift<8, "shiftRLong4 - shift>=8");

  needpulla = pushRegIfUsed (m6502_reg_a);

  if(shift>4)
    {
      loadRegFromAop (m6502_reg_a, AOP (left), 0);
      rmwWithReg ("rol", m6502_reg_a);
      if (shift!=7)
	storeRegTemp(m6502_reg_a, true);
      loadRegFromAop (m6502_reg_a, AOP (left), 1);
      rmwWithReg ("rol", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      loadRegFromAop (m6502_reg_a, AOP (left), 2);
      rmwWithReg ("rol", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 1);
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      rmwWithReg ("rol", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 2);

      if (sign)
	{
	  loadRegFromConst(m6502_reg_a, 0);
	  emit6502op("adc", "#0xff");
	  emit6502op("eor", "#0xff");
	  storeRegToAop (m6502_reg_a, AOP (result), 3);
	}
      else
	{
	  loadRegFromConst(m6502_reg_a, 0);
	  rmwWithReg ("rol", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 3);
	}

      if(shift!=7)
	{
          while(shift!=7)
	    {
	      emitRegTempOp( "rol", getLastTempOfs() );
	      rmwWithAop ("rol", AOP (result), 0);
	      rmwWithAop ("rol", AOP (result), 1);
	      rmwWithAop ("rol", AOP (result), 2);
	      rmwWithAop ("rol", AOP (result), 3);
	      shift++;
	    }
          loadRegTemp(NULL);
        }
    }
  else
    {
      if(!sameRegs (AOP (left), AOP (result)))
	{ 
	  loadRegFromAop (m6502_reg_a, AOP (left), 3);
          if(sign)
            {
              emitCmp(m6502_reg_a, 0x80);
              rmwWithReg ("ror", m6502_reg_a);
	    }
          else
	    rmwWithReg ("lsr", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 3);
	  loadRegFromAop (m6502_reg_a, AOP (left), 2);
	  rmwWithReg ("ror", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 2);
	  loadRegFromAop (m6502_reg_a, AOP (left), 1);
	  rmwWithReg ("ror", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 1);
	  loadRegFromAop (m6502_reg_a, AOP (left), 0);
	  rmwWithReg ("ror", m6502_reg_a);
	  storeRegToAop (m6502_reg_a, AOP (result), 0);
          shiftRLongInPlace (result, shift-1, 0, sign, false);
	  //          storeRegToAop (m6502_reg_a, AOP (result), 0);
	}
      else
	{
	  //  loadRegFromAop (m6502_reg_a, AOP (left), 0);
          shiftRLongInPlace (result, shift, 0, sign, false);
	  //  storeRegToAop (m6502_reg_a, AOP (result), 0);
        }
    }
  pullOrFreeReg (m6502_reg_a, needpulla);
}

/**************************************************************************
 * genrsh32 - shift four byte by a known amount != 0
 *************************************************************************/
static void
genrsh32 (operand * result, operand * left, int shCount, int sign)
{
  emitComment (TRACEGEN, "  %s - shift=%d", __func__, shCount);

  if (shCount >= 24)
    {
      shiftRLong1(left, result, shCount, sign);
    }
  else if (shCount >= 16)
    {
      shiftRLong2(left, result, shCount, sign);
    }
  else if (shCount >= 8)
    {
      shiftRLong3(left, result, shCount, sign);
    }
  else
    {
      shiftRLong4(left, result, shCount, sign);
    }
}

/**************************************************************************
 * genRightShiftLiteral - right shifting by known count
 *************************************************************************/
static void
genRightShiftLiteral (operand * left, operand * result, int shCount, int sign)
{
  bool restore_x = false;
  int size;

  emitComment (TRACEGEN, __func__);


  size = AOP_SIZE (left);
  /* test the LEFT size !!! */
  size = AOP_SIZE (result);
  emitComment (TRACEGEN|VVDBG, "  %s - result size=%d, left size=%d",
               __func__, size, AOP_SIZE (left));

  /* I suppose that the left size >= result size */
  if (shCount == 0)
    {
      genCopy (result, left);
    }
  else if (shCount >= (size * 8))
    {
      bool needpulla = pushRegIfSurv (m6502_reg_a);
      if (sign)
	{
	  /* get sign in acc.7 */
	  loadRegFromAop (m6502_reg_a, AOP (left), size - 1);
	}
      addSign (result, LSB, sign);
      pullOrFreeReg (m6502_reg_a, needpulla);
    }
  else
    {
      if(AOP_TYPE(left)==AOP_SOF || AOP_TYPE(result)==AOP_SOF)
	restore_x=storeRegTempIfUsed(m6502_reg_x);

      switch (size)
	{
	case 1:
	  genrsh8 (result, left, shCount, sign);
	  break;
	case 2:
	  genrsh16 (result, left, shCount, sign);
	  break;
	case 4:
	  genrsh32 (result, left, shCount, sign);
	  break;
	default:
	  wassertl (0, "Invalid operand size in right shift.");
	  break;
	}
      loadOrFreeRegTemp(m6502_reg_x, restore_x);
    }
}

// TODO: can refactor lots of left shift with right shift

/**************************************************************************
 * genRightShift - generate code for right shifting
 *************************************************************************/
void
genRightShift (iCode * ic)
{
  operand *right  = IC_RIGHT (ic);
  operand *left   = IC_LEFT (ic);
  operand *result = IC_RESULT (ic);

  int size, offset;
  symbol *tlbl, *tlbl1;
  bool sign;
  reg_info *countreg = NULL;
  int count_offset=0;
  bool restore_y = false;
  bool x_in_regtemp = false;

  emitComment (TRACEGEN, __func__);

  /* if signed then we do it the hard way preserve the
     sign bit moving it inwards */
  sign = !SPEC_USIGN (getSpec (operandType (left)));

  /* signed & unsigned types are treated the same : i.e. the
     signed is NOT propagated inwards : quoting from the
     ANSI - standard : "for E1 >> E2, is equivalent to division
     by 2**E2 if unsigned or if it has a non-negative value,
     otherwise the result is implementation defined ", MY definition
     is that the sign does not get propagated */

  aopOp (right, ic);
  aopOp (left, ic);
  aopOp (result, ic);

  printIC(ic);

  /* if the shift count is known then do it
     as efficiently as possible */
  if (AOP_TYPE (right) == AOP_LIT &&
      (getSize (operandType (result)) == 1 || getSize (operandType (result)) == 2 || getSize (operandType (result)) == 4))
    {
      int shCount = (int) ulFromVal (AOP (right)->aopu.aop_lit);
      genRightShiftLiteral (left, result, shCount, sign);
      goto release;
    }

  /* shift count is unknown then we have to form
     a loop get the loop count in X : Note: we take
     only the lower order byte since shifting
     more that 32 bits make no sense anyway, ( the
     largest size of an object can be only 32 bits ) */

  // TODO
#if 0
  if (sameRegs (AOP (right), AOP (result)) || regsInCommon (right, result) || IS_AOP_XA (AOP (result)) || isOperandVolatile (result, false))
    aopResult = forceZeropageAop (AOP (result), sameRegs (AOP (left), AOP (result)));
#endif

  /* load the count register */
  if (m6502_reg_y->isDead && !IS_AOP_WITH_Y (AOP (result)) && !IS_AOP_WITH_Y (AOP (left)))
    countreg = m6502_reg_y;
  else if (m6502_reg_x->isDead && !IS_AOP_WITH_X (AOP (result)) && !IS_AOP_WITH_X (AOP (left))
           && AOP_TYPE(left)!=AOP_SOF && AOP_TYPE(result)!=AOP_SOF )
    countreg = m6502_reg_x;
  else if (m6502_reg_a->isDead && !IS_AOP_WITH_A (AOP (result)) && !IS_AOP_WITH_A (AOP (left)))
    countreg = m6502_reg_a;
  else if (!IS_AOP_WITH_Y (AOP (result)) && !IS_AOP_WITH_Y (AOP (left)))
    {
      // Y is live
      storeRegTemp(m6502_reg_y, true);
      restore_y=true;
      countreg = m6502_reg_y;
    }

  if(countreg && IS_AOP_WITH_A(AOP (right)))
    {
      m6502_useReg(countreg);
      loadRegFromAop (countreg, AOP (right), 0);
    }

  /* now move the left to the result if they are not the
     same */
  // TODO: can we keep it in A?
  if (IS_AOP_YX (AOP (result)))
    {
      loadRegFromAop (m6502_reg_yx, AOP (left), 0);
    }
  else if (!sameRegs (AOP (left), AOP(result)))
    {
      size = AOP_SIZE (result);
      offset = 0;
      while (size--)
	{
	  transferAopAop (AOP (left), offset, AOP(result), offset);
	  offset++;
	}
    }

  tlbl = safeNewiTempLabel (NULL);
  size = AOP_SIZE (result);
  offset = 0;
  tlbl1 = safeNewiTempLabel (NULL);

  if (countreg)
    {
      if(!IS_AOP_WITH_A(AOP (right)))
        {
          m6502_useReg(countreg);
          loadRegFromAop (countreg, AOP (right), 0);
        }
      emitCmp(countreg, 0);
      emitBranch ("beq", tlbl1);
    }
  else
    {
      emitComment (TRACEGEN|VVDBG, "  count is not a register");
      bool needpulla = pushRegIfUsed (m6502_reg_a);
      loadRegFromAop (m6502_reg_a, AOP (right), 0);
      storeRegTemp (m6502_reg_a, true);
      count_offset=getLastTempOfs();
      pullOrFreeReg(m6502_reg_a, needpulla);
      emit6502op ("dec", TEMPFMT, count_offset);
      // could keep it literal
      dirtyRegTemp(getLastTempOfs() );
      emitBranch ("bmi", tlbl1);
    }

  if(IS_AOP_XA (AOP (result)))
    {  
      storeRegTempAlways(m6502_reg_x, true);
      dirtyRegTemp (getLastTempOfs());
      x_in_regtemp = true;
    }
  else
    loadRegFromAop (m6502_reg_a, AOP (result), size-1);

  safeEmitLabel (tlbl);

  if(IS_AOP_XA (AOP (result)))
    {
      if(sign)
	{
	  emit6502op("cpx", "#0x80");
	  emitRegTempOp("ror", getLastTempOfs() );
	}
      else
	emitRegTempOp( "lsr", getLastTempOfs() );

      rmwWithReg ("ror", m6502_reg_a);
    }
  else
    {
      if(sign)
        {
          emitCmp(m6502_reg_a, 0x80);
          rmwWithReg ("ror", m6502_reg_a);
        }
      else
        rmwWithReg ("lsr", m6502_reg_a);

      for (offset = size - 2; offset >= 0; offset--)
        {
	  rmwWithAop ("ror", AOP (result), offset);
        }
    }

  if (countreg)
    {
      rmwWithReg("dec", countreg);
      emit6502op("bne", "%05d$", safeLabelNum (tlbl));
    }
  else
    {
      emit6502op("dec", TEMPFMT, count_offset );
      dirtyRegTemp(getLastTempOfs() );
      emit6502op("bpl", "%05d$", safeLabelNum (tlbl));
    }

  if(!IS_AOP_XA (AOP (result)))
    storeRegToAop (m6502_reg_a, AOP (result), size-1);

  if (!countreg)
    loadRegTemp(NULL);

  if (x_in_regtemp)
    loadRegTemp(m6502_reg_x);

  safeEmitLabel (tlbl1);

  // After loop, countreg is always 0
  if (countreg)
    {
      m6502_dirtyReg(countreg);
      countreg->isLitConst = 1;
      countreg->litConst = 0;
    }

  if(restore_y)
    loadRegTemp(m6502_reg_y);

 release:
  freeAsmop (right, NULL);
  freeAsmop (left, NULL);
  freeAsmop (result, NULL);
}

