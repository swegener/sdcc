/*-------------------------------------------------------------------------
  stackops.c - source file for stack operations for the MOS6502

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
 * pushReg - Push register reg onto the stack. If freereg is true, reg is
 *           marked free and available for reuse.
 *************************************************************************/
void
pushReg (reg_info * reg, bool freereg)
{
  int regidx = reg->rIdx;

  emitComment (REGOPS, "  pushReg(%s) %s %s", reg->name, reg->isFree?"free":"", reg->isDead?"dead":"");

  switch (regidx)
    {
    case A_IDX:
      emit6502op ("pha", "");
      updateCFA ();
      break;
    case X_IDX:
      if (IS_MOS65C02)
        {
          emit6502op ("phx", "");
        }
      else
        {
          bool needloada = storeRegTempIfUsed (m6502_reg_a);
          transferRegReg (m6502_reg_x, m6502_reg_a, false);
          pushReg (m6502_reg_a, true);
          loadOrFreeRegTemp (m6502_reg_a, needloada);
        }
      updateCFA ();
      break;
    case Y_IDX:
      if (IS_MOS65C02)
        {
          emit6502op ("phy", "");
        }
      else
        {
          bool needloada = storeRegTempIfUsed (m6502_reg_a);
          transferRegReg (m6502_reg_y, m6502_reg_a, true);
          pushReg (m6502_reg_a, true);
          loadOrFreeRegTemp (m6502_reg_a, needloada);
        }
      updateCFA ();
      break;
      // little-endian order
    case XA_IDX:
      if(m6502_reg_y->isFree && !IS_MOS65C02)
        {
          transferRegReg (m6502_reg_a, m6502_reg_y, freereg);
          transferRegReg (m6502_reg_x, m6502_reg_a, freereg);
          pushReg(m6502_reg_a, freereg);
          transferRegReg (m6502_reg_y, m6502_reg_a, true);
          pushReg(m6502_reg_a, freereg);
        }
      else
        {
	  pushReg(m6502_reg_x, freereg);
	  pushReg(m6502_reg_a, freereg);
        }
      break;
    case YX_IDX:
      pushReg(m6502_reg_y, freereg);
      pushReg(m6502_reg_x, freereg);
      break;
    default:
      emitcode("ERROR", "    %s: bad reg idx: 0x%02x", __func__, regidx);
      break;
    }
  if (freereg)
    m6502_freeReg (reg);
}

/**************************************************************************
 * pullReg - Pull register reg off the stack.
 *************************************************************************/
void
pullReg (reg_info * reg)
{
  int regidx = reg->rIdx;

  emitComment (REGOPS, __func__ );

  switch (regidx) {
  case A_IDX:
    emit6502op ("pla", "");
    updateCFA ();
    break;
  case X_IDX:
    if (IS_MOS65C02)
      {
        emit6502op ("plx", "");
      }
    else
      {
        // FIXME: saving A makes regression fail
        //      bool needloada = storeRegTempIfUsed (m6502_reg_a);
        pullReg (m6502_reg_a);
        transferRegReg (m6502_reg_a, m6502_reg_x, true);
        //      loadOrFreeRegTemp (m6502_reg_a, needloada);
      }
    updateCFA ();
    break;
  case Y_IDX:
    if (IS_MOS65C02)
      {
        emit6502op ("ply", "");
      }
    else
      {
        // FIXME: saving A makes regression fail
        //      bool needloada = storeRegTempIfUsed (m6502_reg_a);
        pullReg (m6502_reg_a);
        transferRegReg (m6502_reg_a, m6502_reg_y, true);
        //      loadOrFreeRegTemp (m6502_reg_a, needloada);
      }
    updateCFA ();
    break;
    // little-endian order
  case XA_IDX:
    pullReg(m6502_reg_a);
    pullReg(m6502_reg_x);
    break;
  case YX_IDX:
    pullReg(m6502_reg_x);
    pullReg(m6502_reg_y);
    break;
  default:
    emitcode("ERROR", "    %s: bad reg idx: 0x%02x", __func__, regidx);
    break;
  }
  m6502_useReg (reg);
  m6502_dirtyReg (reg);
}

/**************************************************************************
 * pullNull - Discard n bytes off the top of the stack
 *************************************************************************/
void
pullNull (int n)
{
  emitComment (REGOPS, __func__ );
  if(n < 0) emitcode("ERROR", "pullNull called with negative parameter");

  adjustStack (n);
}

/**************************************************************************
 * pushRegIfUsed - Push register reg if marked in use. Returns true if the
 *                 push was performed, false otherwise.
 *************************************************************************/
bool
pushRegIfUsed (reg_info *reg)
{
  if (!reg->isFree)
    {
      pushReg (reg, true);
      return true;
    }
  else
    return false;
}

/**************************************************************************
 * pushRegIfSurv - Push register reg if marked surviving. Returns true if
 *                 the push was performed, false otherwise.
 *************************************************************************/
bool
pushRegIfSurv (reg_info *reg)
{
  if (!reg->isDead)
    {
      pushReg (reg, true);
      return true;
    } else
    return false;
}

/**************************************************************************
 * pullOrFreeReg - If needpull is true, register reg is pulled from the
 *                 stack. Otherwise register reg is marked as free.
 *************************************************************************/
void
pullOrFreeReg (reg_info * reg, bool needpull)
{
  if (needpull)
    pullReg (reg);
  else
    m6502_freeReg (reg);
}

/**************************************************************************
 * adjustStack - Adjust the stack pointer by n bytes.
 *************************************************************************/
// TODO: optimize for 65C02
void
adjustStack (int n)
{
  bool restore_a = false;
  bool restore_x = false;
  char *inst=NULL;

  int sa_cycle = (m6502_reg_a->isFree)?0:6;
  int sx_cycle = (m6502_reg_x->isFree)?0:6;
  int abs_n = (n>0)?n:-n;

  emitComment (REGOPS, __func__ );
  emitComment (REGOPS, "  %s reg:  %s", __func__, regInfoStr());

  // unrolled PHA      1b, 3c x n
  // unrolled PLA      1b, 4c x n + 4b, 6c if A is used
  // unrolled INX/DEX  1b, 2c x n + 4b, 4c + 4b, 6c if X is used
  // ADC               5b, 8c + 4b, 4c + 4b, 6c if A is used + 4b, 6c if X is used
  
  // TODO: implement optimize for space
  int stack = (n>0) ? 4*abs_n + sa_cycle : 3*abs_n ; // PLA : PHA
  int incdec = 2*abs_n + 4 + sx_cycle ; // INC : DEC
  int adc = 12 + sa_cycle + sx_cycle;

  if(m6502_reg_x->aop==&tsxaop)
    {
      adc-=2;
      incdec-=2;
    }

  emitComment (ALWAYS /*REGOPS|VVDBG*/, "  %s : cycles stk:%d  incdec:%d  adc:%d", __func__,
               stack, incdec, adc);

  if(stack<=incdec && stack<=adc)
    {
     inst = (n>0) ? "pla" : "pha";
     if(n>0)
       restore_a=storeRegTempIfUsed (m6502_reg_a);

      while((abs_n--)>0)
        emit6502op(inst, "");

      loadOrFreeRegTemp(m6502_reg_a, restore_a);
    }
  else
    {
      restore_x = storeRegTempIfUsed(m6502_reg_x);

      if(incdec<=adc)
        {
          inst = (n>0) ? "inx" : "dex";
          emitTSX();
          while((abs_n--)>0)
            emit6502op(inst, "");
        }
      else
        {
          restore_a = storeRegTempIfUsed(m6502_reg_a);
          emitTSX();
          transferRegReg (m6502_reg_x, m6502_reg_a, true);
          emitSetCarry(0);
          emit6502op ("adc", IMMDFMT, (unsigned int)(n & 0xff));
          transferRegReg (m6502_reg_a, m6502_reg_x, true);
        }
      _S.stackPushes -= n;
      emit6502op ("txs", "");
      loadOrFreeRegTemp(m6502_reg_a, restore_a);
      loadOrFreeRegTemp(m6502_reg_x, restore_x);
    }
  updateCFA ();
}
