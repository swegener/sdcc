/*-------------------------------------------------------------------------
  genminus.c - source file for subtraction code generation for the MOS6502

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
 * genMinusDec :- does subtraction with decrement if possible
 *************************************************************************/
static bool
genMinusDec (iCode * ic)
{
  operand *right  = IC_RIGHT (ic);
  operand *left   = IC_LEFT (ic);
  operand *result = IC_RESULT (ic);

  int icount;
  unsigned int size = AOP_SIZE (result);
  symbol *tlbl = NULL;

  /* will try to generate an increment */
  /* if the right side is not a literal
     we cannot */
  if (AOP_TYPE (right) != AOP_LIT)
    return false;

  icount = (unsigned int) ulFromVal (AOP (right)->aopu.aop_lit);

  emitComment (TRACEGEN, "  %s - size=%d  icount=%d", __func__, size, icount);

  if (icount>255 && ((icount&0xff)!=0) )
    return false;

  if (icount>255)
    {
#if 0
      int bcount = icount>>8;
      if (!IS_AOP_XA (AOP (result)) || bcount>4 )
        return false;

      while (bcount--)
        emit6502op ("dex", "");
#endif
      return false;
    }

  if(IS_AOP_XA (AOP (result)) && icount >=0 )
  {
    loadRegFromAop (m6502_reg_xa, AOP (left), 0);
    if (icount)
      {
        tlbl = safeNewiTempLabel (NULL);
        emitSetCarry (1);
        accopWithAop ("sbc", AOP (right), 0);
        emitBranch ("bcs", tlbl);
        emit6502op ("dex", "");
        safeEmitLabel (tlbl);
        m6502_dirtyReg(m6502_reg_x);
      }
    return true;
    }

  if (!sameRegs (AOP (left), AOP (result)))
    {
      if (icount==1 && size==1 )
        {
          reg_info *src_reg = (AOP_TYPE(left)==AOP_REG)? AOP(left)->aopu.aop_reg[0] :NULL;
          reg_info *dst_reg = (AOP_TYPE(result)==AOP_REG)? AOP(result)->aopu.aop_reg[0] :NULL;

	  if(src_reg)
	    {
	      if(dst_reg && dst_reg!=m6502_reg_a)
		{
		  transferRegReg (src_reg, dst_reg, src_reg->isDead);
		  rmwWithReg ("dec", dst_reg);
		  return true;  
		}
	      if(src_reg->isDead /* && src_reg!=m6502_reg_a */ )
		{
		  rmwWithReg ("dec", src_reg);
	          storeRegToAop (src_reg, AOP (result), 0);
		  return true;  
		}
	    }
	}
      return false;
    }

  // sameRegs

  // TODO: can inc blah,x
  if (!aopCanIncDec (AOP (result)))
    return false;

  emitComment (TRACEGEN|VVDBG, "    %s", __func__);

  if (size==1 && AOP(result)->type==AOP_REG)
  {
    // if it's in a 8-bit register try to do small adjust
    if(smallAdjustReg(AOP(result)->aopu.aop_reg[0], -icount))
      return true;
  }

  if (icount < 0)
    return false;

  if(icount==1 && size==2 && aopCanIncDec(AOP(result)) )
    {
      reg_info *reg = getFreeByteReg();
      if(reg)
        {
          tlbl = safeNewiTempLabel (NULL);
          loadRegFromAop (reg, AOP (left), 0);
          emitBranch ("bne", tlbl);
          rmwWithAop ("dec", AOP (result), 1);
          safeEmitLabel (tlbl);
          rmwWithAop ("dec", AOP (result), 0);
          return true;
        }
    }

  if (size != 1|| icount>1)
    return false;

  rmwWithAop ("dec", AOP (result), 0);

      return true;
  }

/**************************************************************************
 * genMinus - generates code for subtraction
 *************************************************************************/
void
genMinus (iCode * ic)
{
  operand *right  = IC_RIGHT (ic);
  operand *left   = IC_LEFT (ic);
  operand *result = IC_RESULT (ic);

  bool init_carry = true;
  int size, offset = 0;
  bool needpulla = false;
  bool earlystore = false;
  bool delayedstore = false;

  sym_link *resulttype = operandType (IC_RESULT (ic));
  unsigned topbytemask = (IS_BITINT (resulttype) && SPEC_USIGN (resulttype) && (SPEC_BITINTWIDTH (resulttype) % 8)) ?
    (0xff >> (8 - SPEC_BITINTWIDTH (resulttype) % 8)) : 0xff;
  bool maskedtopbyte = (topbytemask != 0xff);

  emitComment (TRACEGEN, __func__);

  aopOp (left, ic);
  aopOp (right, ic);
  aopOp (result, ic);

  printIC(ic);

  if (!maskedtopbyte && genMinusDec (ic))
    goto release;

  emitComment (TRACEGEN|VVDBG, "    %s - Can't Dec", __func__);

  size = AOP_SIZE (result);

  offset = 0;

  if ( size==2 && AOP_TYPE (right) == AOP_LIT && !maskedtopbyte
       && operandLitValue (right) >= 0
       && operandLitValue (right) <= 255
       && AOP_TYPE(result) != AOP_SOF
       && sameRegs(AOP(result),AOP(left)) )
    {
      symbol *skiplabel = safeNewiTempLabel (NULL);

      emitComment (TRACEGEN|VVDBG, "    %s: size==2 && AOP_LIT", __func__);
      needpulla = pushRegIfSurv (m6502_reg_a);
      emitSetCarry(1);
      loadRegFromAop (m6502_reg_a, AOP(left), 0);
      accopWithAop ("sbc", AOP(right), 0);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      emitBranch ("bcs", skiplabel);
      rmwWithAop ("dec", AOP(result), 1);
      if(IS_AOP_WITH_X(AOP(result)))
	m6502_dirtyReg(m6502_reg_x);
      if(IS_AOP_WITH_Y(AOP(result)))
	m6502_dirtyReg(m6502_reg_y);
      safeEmitLabel (skiplabel);
      pullOrFreeReg (m6502_reg_a, needpulla);
      goto release;
    }

  //  if ( IS_AOP_XA(AOP(left)) && AOP_TYPE(result) == AOP_SOF)
  if ( IS_AOP_XA (AOP(left)) && !IS_AOP_XA(AOP(result)) &&
       (AOP_TYPE(result) == AOP_SOF || AOP_TYPE(right) == AOP_SOF) )
    {
      bool restore_a = pushRegIfSurv(m6502_reg_a);
      bool restore_x = !m6502_reg_x->isDead;
      storeRegTemp(m6502_reg_x, true);
      emitSetCarry(1);
      accopWithAop ("sbc", AOP (right), 0);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      loadRegTempAt(m6502_reg_a, getLastTempOfs() );
      accopWithAop ("sbc", AOP (right), 1);
      storeRegToAop (m6502_reg_a, AOP (result), 1);

      if(restore_x)
        loadRegTemp(m6502_reg_x);
      else
        loadRegTemp(NULL);

      pullOrFreeReg (m6502_reg_a, restore_a);
      goto release;
    }

  if (IS_AOP_A (AOP(right)))
  {
    // op - a = neg(a - op) = not(a - op) + 1 = not(a - op - 1)
    needpulla = pushRegIfSurv (m6502_reg_a);
    emitSetCarry(0);
    accopWithAop ("sbc", AOP(left) , offset);
    emit6502op("eor", "#0xff");
    if (maskedtopbyte)
      emit6502op ("and", IMMDFMT, topbytemask);
    storeRegToAop (m6502_reg_a, AOP (result), offset);
    pullOrFreeReg (m6502_reg_a, needpulla);
    goto release;
  }

  needpulla = storeRegTempIfSurv (m6502_reg_a);

  while (size--)
    {
      if (earlystore &&
	  (AOP_TYPE (left) == AOP_REG && AOP (left)->aopu.aop_reg[offset]->rIdx == A_IDX ||
	   AOP_TYPE (right) == AOP_REG && AOP (right)->aopu.aop_reg[offset]->rIdx == A_IDX))
	pullReg (m6502_reg_a);
      if (AOP_TYPE (right) == AOP_REG && AOP (right)->aopu.aop_reg[offset]->rIdx == A_IDX)
	{
	  storeRegTemp (m6502_reg_a, true);
	  loadRegFromAop (m6502_reg_a, AOP(left), offset);
	  if (init_carry)
	    emitSetCarry(1);

	  emitRegTempOp("sbc", getLastTempOfs() );
	  loadRegTemp (NULL);
	}
      else
	{
	  emitComment (TRACEGEN|VVDBG, "    - default path");

	  loadRegFromAop (m6502_reg_a, AOP(left), offset);
	  if (init_carry)
	    emitSetCarry(1);

	  accopWithAop ("sbc", AOP(right), offset);
	}
      if (!size && maskedtopbyte)
	emit6502op ("and", IMMDFMT, topbytemask);
      if (size && AOP_TYPE (result) == AOP_REG && AOP (result)->aopu.aop_reg[offset]->rIdx == A_IDX)
	{
	  emitComment (TRACEGEN|VVDBG, "    - push");
	  pushReg (m6502_reg_a, true);
	  delayedstore = true;
	}
      else
	{
	  emitComment (TRACEGEN|VVDBG, "    - store");
	  storeRegToAop (m6502_reg_a, AOP (result), offset);
	}
      offset++;
      init_carry = false;
    }
  if(delayedstore)
    pullReg (m6502_reg_a);

  loadOrFreeRegTemp (m6502_reg_a, needpulla);

  wassert (!earlystore || !delayedstore);

 release:
  freeAsmop (left, NULL);
  freeAsmop (right, NULL);
  freeAsmop (result, NULL);
}

