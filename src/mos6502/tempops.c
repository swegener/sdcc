/*-------------------------------------------------------------------------
  tempops.c - source file for regtemp operations for the MOS6502

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
 * Store register onto the REGTEMP stack. If freereg is true,
 * reg is marked free and available for reuse. If force is true,
 * no literal optimizations are performed.
 *
 * @param reg pointer for the register to save
 * @param freereg free the register if true
 *************************************************************************/
static void
storeRegTempi(reg_info * reg, bool freereg, bool force)
{
  emitComment (REGOPS, "  storeRegTemp(%s) %s", reg ? reg->name : "-", freereg ? "free" : "");

  int regidx = reg->rIdx;
  char storeOp[4] = "st?";

  switch (regidx)
    {
    case A_IDX:
    case X_IDX:
    case Y_IDX:
      storeOp[2]=reg->name[0];
      _S.tempAttr[_S.tempOfs].isLiteral=reg->isLitConst;
      _S.tempAttr[_S.tempOfs].literalValue=reg->litConst;
      _S.tempAttr[_S.tempOfs].aop=reg->aop;
      _S.tempAttr[_S.tempOfs].aopofs=reg->aopofs;
      if(reg->isLitConst && !force)
        {
        emitComment(REGOPS|VVDBG, "  %s: virtual store literal 0x%02x",__func__,
                      reg->litConst);
        }
      else if(reg->aop && !force && (reg->aop->type==AOP_DIR || reg->aop->type==AOP_EXT) )
        {
          emitComment(REGOPS|VVDBG, "  %s: virtual store %s+%d",__func__,
                      reg->aop->aopu.aop_dir, reg->aopofs);
        }
      else 
        {    
        emit6502op (storeOp, TEMPFMT, _S.tempOfs);
        }
      _S.tempOfs++;
      break;
    case XA_IDX:
      storeRegTempi (m6502_reg_a, freereg, force);
      storeRegTempi (m6502_reg_x, freereg, force);
      break;
    case YX_IDX:
      storeRegTempi (m6502_reg_x, freereg, force);
      storeRegTempi (m6502_reg_y, freereg, force);
      break;
    default:
      emitcode("ERROR", "%s : bad reg %02x (%s)", __func__, regidx, reg->name);
      break;
    }
  
  if (freereg)
    m6502_freeReg (reg);

  if(_S.tempOfs > NUM_TEMP_REGS)
    emitcode("ERROR", "storeRegTemp(): overflow");
}


void
storeRegTemp (reg_info * reg, bool freereg)
{
  storeRegTempi (reg, freereg, false);
}

void
storeRegTempAlways (reg_info * reg, bool freereg)
{
  storeRegTempi (reg, freereg, true);
}


/**************************************************************************
 * Store register onto the REGTEMP stack if register is alive
 *
 * @param reg pointer for the register to save
 * @return true if the register was saved
 *************************************************************************/
bool
storeRegTempIfSurv (reg_info *reg)
{
  if (!reg->isDead)
    {
      storeRegTemp (reg, true);
      return true;
    }
  return false;
}

/**************************************************************************
 * Store register onto the REGTEMP stack if register is in use
 *
 * @param reg pointer for the register to save
 * @return true if the register was saved
 *************************************************************************/
bool
storeRegTempIfUsed (reg_info *reg)
{
  if (!reg->isFree)
    {
      storeRegTemp (reg, true);
      return true;
    }
  return false;
}

/**************************************************************************
 * Load register from the REGTEMP stack at an arbitrary offset
 *
 * @param reg pointer for the register to save
 *************************************************************************/
void
loadRegTempAt (reg_info * reg, int offset)
{
  int regidx = reg->rIdx;
  char loadOp[4] = "ld?";
  
  if (offset<0)
    emitcode("ERROR", " %s - reg temp stack underflow", __func__);

  if(_S.tempAttr[offset].isLiteral)
    {
      loadRegFromConst(reg, _S.tempAttr[offset].literalValue);
      return;
    }

  switch (regidx)
    {
    case A_IDX:
    case X_IDX:
    case Y_IDX:
      loadOp[2]=reg->name[0];
      if(_S.tempAttr[offset].aop && (_S.tempAttr[offset].aop->type==AOP_DIR || _S.tempAttr[offset].aop->type==AOP_EXT))
        {
          emitComment(REGOPS|VVDBG, "  %s: should load from %s+%d", __func__,
                      _S.tempAttr[offset].aop->aopu.aop_dir, _S.tempAttr[offset].aopofs);
          emit6502op (loadOp, "%s(%s+%d)", (_S.tempAttr[offset].aop->type==AOP_DIR)?"*":"",
           _S.tempAttr[offset].aop->aopu.aop_dir, _S.tempAttr[offset].aopofs );
        }
      else
      emit6502op (loadOp, TEMPFMT, offset);
      break;
    default:
      emitcode("ERROR","loadRegTempAt called with illegal regidx %d", regidx);
    }
  
  m6502_useReg (reg);
  m6502_dirtyReg (reg);
}

/**************************************************************************
 * Load register from the REGTEMP stack.
 *
 * @param reg pointer for the register to save
 *************************************************************************/
void
loadRegTemp (reg_info * reg)
{
  // pop off stack, unused
  if (reg == NULL)
    {
      _S.tempOfs--;
      return;
    }

  switch (reg->rIdx)
    {
    case A_IDX:
    case X_IDX:
    case Y_IDX:
      loadRegTempAt(reg, --_S.tempOfs);
      return;
    case XA_IDX:
      loadRegTemp(m6502_reg_x);
      loadRegTemp(m6502_reg_a);
      break;
    case YX_IDX:
      loadRegTemp(m6502_reg_y);
      loadRegTemp(m6502_reg_x);
      break;
    default:
      emitcode("ERROR", "bad reg in loadRegTemp()");
      break;
    }

  // FIXME: figure out if register pairs are literals
  m6502_useReg (reg);
  m6502_dirtyReg (reg);
}

/**************************************************************************
 * Conditionally load a register from the REGTEMP stack.
 *
 * @param reg pointer for the register to load
 * @param load register if true otherwise free the register
 *************************************************************************/
void
loadOrFreeRegTemp (reg_info * reg, bool needpull)
{
  if (needpull)
    loadRegTemp (reg);
  else
    m6502_freeReg (reg);
}

/**************************************************************************
 * Conditionally load a register from the REGTEMP stack without
 * affecting condition flags
 *
 * @param reg pointer for the register to load
 * @param load register if true otherwise free the register
 *************************************************************************/
void
loadRegTempNoFlags (reg_info * reg, bool needpull)
{
  if (needpull)
    {
      int tempflag=_S.lastflag;
      emit6502op("php", "");
      loadRegTemp (reg);
      emit6502op("plp", "");
      _S.lastflag=tempflag;
    }
  else
    {
      m6502_freeReg (reg);
    }
}

void
emitRegTempOp( char *op, int offset)
{
  if(_S.tempAttr[offset].isLiteral)
    {
      emit6502op(op, IMMDFMT, _S.tempAttr[offset].literalValue );
    }
  else if(_S.tempAttr[offset].aop && (_S.tempAttr[offset].aop->type==AOP_DIR || _S.tempAttr[offset].aop->type==AOP_EXT))
    {
      emitComment(REGOPS|VVDBG, "  %s: %s with %s+%d", __func__,
                     op, _S.tempAttr[offset].aop->aopu.aop_dir, _S.tempAttr[offset].aopofs);
      emit6502op (op, "%s(%s+%d)", (_S.tempAttr[offset].aop->type==AOP_DIR)?"*":"",
           _S.tempAttr[offset].aop->aopu.aop_dir, _S.tempAttr[offset].aopofs );
    }
  else
    {
      emit6502op(op, TEMPFMT, offset);
    }
}



void
dirtyRegTemp (int temp_reg_idx)
{
  _S.tempAttr[temp_reg_idx].isLiteral=false;
  _S.tempAttr[temp_reg_idx].aop=NULL;
}

