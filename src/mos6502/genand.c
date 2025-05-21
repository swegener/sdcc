/*-------------------------------------------------------------------------
  genand.c - source file for AND code generation for the MOS6502

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
 * genAnd  - code for and
 *************************************************************************/
void
genAnd (iCode * ic, iCode * ifx)
{
  operand *right  = IC_RIGHT (ic);
  operand *left   = IC_LEFT (ic);
  operand *result = IC_RESULT (ic);

  int size, offset = 0;
  bool needpulla = false;
  bool isLit = false;
  unsigned long long lit = 0ull;
  unsigned int bytemask;
  int bitpos = -1;

  emitComment (TRACEGEN, __func__);

  aopOp (left, ic);
  aopOp (right, ic);
  aopOp (result, ic);
  printIC(ic);

  /* force literal on the right and reg on the left */
  if (AOP_TYPE (left) == AOP_LIT || AOP_TYPE (right) == AOP_REG)
    {
      if(!IS_AOP_WITH_A (AOP (left)))
	{
	  operand *tmp = right;
	  right = left;
	  left = tmp;
	}
    }

  size = (AOP_SIZE (left) >= AOP_SIZE (right)) ? AOP_SIZE (left) : AOP_SIZE (right);

  isLit = (AOP_TYPE (right) == AOP_LIT);

  if (isLit)
    {
      lit = ullFromVal (AOP (right)->aopu.aop_lit);
      lit &= litmask(size);
      bitpos = isLiteralBit (lit) - 1;
      emitComment (TRACEGEN|VVDBG, "  %s: lit=%04x bitpos=%d", __func__, lit, bitpos);
    }

#if 0
  // Rockwell and WDC only - unimplemented
  if (IS_MOS65C02 && ifx 
      && AOP_TYPE (result) == AOP_CRY 
      && AOP_TYPE (right) == AOP_LIT 
      && AOP_TYPE (left) == AOP_DIR && bitpos >= 0)
    {
      symbol *tlbl = safeNewiTempLabel (NULL);
      if (IC_TRUE (ifx))
	{
	  // FIXME: unimplemented
	  emit6502op ("brclr", "#%d,%s,%05d$", bitpos & 7, aopAdrStr (AOP (left), bitpos >> 3, false), safeLabelKey2num ((tlbl->key)));
	  emitBranch ("jmp", IC_TRUE (ifx));
	  safeEmitLabel (tlbl);
	  if (IC_FALSE (ifx))
	    emitBranch ("jmp", IC_FALSE (ifx));
	}
      else
	{
	  // FIXME: unimplemented
	  if (!regalloc_dry_run)
	    emit6502op ("brset", "#%d,%s,%05d$", bitpos & 7, aopAdrStr (AOP (left), bitpos >> 3, false), safeLabelKey2num ((tlbl->key)));
	  emitBranch ("jmp", IC_FALSE (ifx));
	  safeEmitLabel (tlbl);
	}
      ifx->generated = true;
      goto release;
    }
#endif

  // special case for bit 7 and 6
  if (AOP_TYPE (result) == AOP_CRY && isLit && canBitOp(left))
    {
      emitComment (TRACEGEN|VVDBG, "  %s: special case bit %d", __func__, bitpos);

      if (bitpos >= 0 && (bitpos & 7) == 7)
	{
	  rmwWithAop ("bit", AOP (left), bitpos >> 3);
	  genIfxJump (ifx, "n");
	  goto release;
	}
      if (bitpos >= 0 && (bitpos & 7) == 6)
	{
	  rmwWithAop ("bit", AOP (left), bitpos >> 3);
	  genIfxJump (ifx, "v");
	  goto release;
	}
    }

  // test A for flags only
  if (AOP_TYPE (result) == AOP_CRY && IS_AOP_A (AOP (left)))
    {
      emitComment (TRACEGEN|VVDBG, "  %s: test A for flags", __func__);

      if (m6502_reg_a->isDead)
        accopWithAop ("and", AOP (right), 0);
      else if (canBitOp(right))
        accopWithAop ("bit", AOP (right), 0);
      else
        {
          if(isLit && bitpos==7)
            {
	      emitCmp(m6502_reg_a, 0);
              genIfxJump (ifx, "n");
              goto release;
            }
            
          if(isLit && bitpos==6)
            {
              // FIXME: this can be improved for 65C02
              storeRegTempAlways(m6502_reg_a, false);
              emit6502op ("bit", TEMPFMT, getLastTempOfs() );
              loadRegTemp(NULL);
              genIfxJump(ifx, "v");
              goto release; 
            }

          reg_info* freereg = getDeadByteReg();
          if (freereg)
            {
              // FIXME: this can be improved for 65C02
              loadRegFromAop (freereg, AOP(right), 0);
              storeRegTempAlways(freereg, true);
              emit6502op ("bit", TEMPFMT, getLastTempOfs() );
              loadRegTemp(NULL);
            }
	  else
	    {
	      storeRegTemp(m6502_reg_a, true);
	      accopWithAop ("and", AOP(right), 0);
	      loadRegTempNoFlags(m6502_reg_a, true); // preserve flags
	    }
        }
      genIfxJump (ifx, "z");
      goto release;
    }

  // test for flags only (general case)
  if (AOP_TYPE (result) == AOP_CRY)
    {
      symbol *tlbl = safeNewiTempLabel (NULL);
      needpulla = storeRegTempIfSurv (m6502_reg_a);
      offset = 0;

      if ( isLit && lit == 0)
	{
	  loadRegFromConst (m6502_reg_a, 0x00);
	}
      else
	while (size--)
	  {
	    bytemask = (isLit) ? (lit >> (offset * 8)) & 0xff : 0x100;

	    if (bytemask != 0x00)
	      {
		loadRegFromAop (m6502_reg_a, AOP(left), offset);
		if (bytemask != 0xff)
		  accopWithAop ("and", AOP(right), offset);
		if (size)
		  emitBranch ("bne", tlbl);
	      }
	    offset++;
	  }
      m6502_freeReg (m6502_reg_a);
      safeEmitLabel (tlbl);

      // TODO: better way to preserve flags?
      if (ifx)
	{
	  loadRegTempNoFlags (m6502_reg_a, needpulla);
	  genIfxJump (ifx, "z");
	}
      else
	{
	  if (needpulla) loadRegTemp (NULL);
	}
      goto release;
    }

  size = AOP_SIZE (result);

#if 0
  // Rockwell and WDC only - works but limited usefulness
  if (IS_MOS65C02 && AOP_TYPE (right) == AOP_LIT)
    {
      unsigned long long litinv = (~lit) & litmask(size);
      if (sameRegs (AOP (left), AOP (result)) && (AOP_TYPE (left) == AOP_DIR) 
	  && isLiteralBit (litinv))
	{
	  char inst[5] = "rmbx";
	  inst[3] = '0' + (bitpos & 7);
	  emit6502op (inst, "%s", aopAdrStr (AOP (left), bitpos >> 3, false));
	  goto release;
	}
    }
#endif

  bool savea = false;
  unsigned int bmask0 = (isLit) ? ((lit >> (0 * 8)) & 0xff) : 0x100;
  unsigned int bmask1 = (isLit) ? ((lit >> (1 * 8)) & 0xff) : 0x100;

  if (IS_AOP_XA(AOP(left)))
    {
      if(m6502_reg_a->isLitConst) bmask0 &= m6502_reg_a->litConst;
      if(m6502_reg_x->isLitConst) bmask1 &= m6502_reg_x->litConst;
    }

  if(IS_AOP_XA(AOP(result)))
    {
      emitComment (TRACEGEN|VVDBG, "  %s: XA", __func__);

      //    if(/*AOP_TYPE(right)==AOP_LIT &&*/ AOP_TYPE(left)!=AOP_SOF)
      {
        if (IS_AOP_A(AOP(left)))
          storeConstToAop(0x00, AOP(result), 1);
        else if(bmask1==0x00)
          storeConstToAop(0x00, AOP(result), 1);
        else if(IS_AOP_XA(AOP(left)) && m6502_reg_x->isLitConst && m6502_reg_x->litConst==0xff)
          transferAopAop(AOP(right), 1, AOP(result), 1);
        else if (bmask1==0xff)
          transferAopAop(AOP(left), 1, AOP(result), 1);
        else
          {
            if(IS_AOP_XA(AOP(left)))
	      {
		if(bmask0!=0x00)
		  {
		    if (bmask0!=0xff)
		      accopWithAop ("and", AOP (right), 0);
		    storeRegTemp(m6502_reg_a, true);
		    needpulla=true;
		  }
	      }
            loadRegFromAop (m6502_reg_a, AOP (left), 1);
            accopWithAop ("and", AOP (right), 1);
            storeRegToAop (m6502_reg_a, AOP (result), 1);          
          }
        if(bmask0==0x00)
          storeConstToAop(0x00, AOP(result), 0);
        else
	  {
	    if(needpulla) loadRegTemp(m6502_reg_a);
	    else
	      {
		loadRegFromAop (m6502_reg_a, AOP (left), 0);
		if (bmask0!=0xff)
		  accopWithAop ("and", AOP (right), 0);
	      }
	  }
        goto release;
      }
    }

  {
    int i;
    needpulla = !m6502_reg_a->isDead;
    if(needpulla) storeRegTemp(m6502_reg_a, true);

    emitComment (TRACEGEN|VVDBG, "  %s: general path", __func__);

    for(i=0; i<size; i++)
      {
	bytemask = (isLit) ? ((lit >> (i * 8)) & 0xff) : 0x100;
	if ( bytemask==0x00 )
	  {
	    storeConstToAop(0x00, AOP(result), i);
	  }
	else if ( bytemask==0xff )
	  {
	    transferAopAop(AOP(left), i, AOP(result), i);
	  }
	else
	  {
	    loadRegFromAop (m6502_reg_a, AOP (left), i);
	    accopWithAop ("and", AOP (right), i);
	    storeRegToAop (m6502_reg_a, AOP (result), i);
	  }
      }
  }

  if(needpulla)
    {
      loadRegTemp(m6502_reg_a);
    }
  else
    {
      if(savea) loadRegTemp(NULL);
      m6502_freeReg(m6502_reg_a);
    }

 release:
  freeAsmop (left, NULL);
  freeAsmop (right, NULL);
  freeAsmop (result, NULL);
}

