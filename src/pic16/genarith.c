/*-------------------------------------------------------------------------

 genarith.c - source file for code generation - arithmetic 
  
  Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)
         and -  Jean-Louis VERN.jlvern@writeme.com (1999)
  Bug Fixes  -  Wojciech Stryjewski  wstryj1@tiger.lsu.edu (1999 v2.1.9a)
  PIC port   -  Scott Dattalo scott@dattalo.com (2000)
  PIC16 port   -  Martin Dubuc m.dubuc@rogers.com (2002)
  
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
  
  In other words, you are welcome to use, share and improve this program.
  You are forbidden to forbid anyone else to use, share and improve
  what you give them.   Help stamp out software-hoarding!
  
  Notes:
  000123 mlh	Moved aopLiteral to SDCCglue.c to help the split
  		Made everything static
-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SDCCglobl.h"
#include "newalloc.h"

#if defined(_MSC_VER) && (_MSC_VER < 1300)
#define __FUNCTION__		__FILE__
#endif

#include "common.h"
#include "SDCCpeeph.h"
#include "ralloc.h"
#include "pcode.h"
#include "gen.h"


#define BYTEofLONG(l,b) ( (l>> (b<<3)) & 0xff)
void DEBUGpic16_pic16_AopType(int line_no, operand *left, operand *right, operand *result);

const char *pic16_AopType(short type)
{
  switch(type) {
  case AOP_LIT:
    return "AOP_LIT";
    break;
  case AOP_REG:
    return "AOP_REG";
    break;
  case AOP_DIR:
    return "AOP_DIR";
    break;
  case AOP_DPTR:
    return "AOP_DPTR";
    break;
  case AOP_DPTR2:
    return "AOP_DPTR2";
    break;
  case AOP_R0:
    return "AOP_R0";
    break;
  case AOP_R1:
    return "AOP_R1";
    break;
  case AOP_STK:
    return "AOP_STK";
    break;
  case AOP_IMMD:
    return "AOP_IMMD";
    break;
  case AOP_STR:
    return "AOP_STR";
    break;
  case AOP_CRY:
    return "AOP_CRY";
    break;
  case AOP_ACC:
    return "AOP_ACC";
    break;
  case AOP_PCODE:
    return "AOP_PCODE";
    break;
  }

  return "BAD TYPE";
}

const char *pic16_pCodeOpType(  pCodeOp *pcop)
{

  if(pcop) {

    switch(pcop->type) {

    case  PO_NONE:
      return "PO_NONE";
    case  PO_W:
      return  "PO_W";
    case  PO_WREG:
      return  "PO_WREG";
    case  PO_STATUS:
      return  "PO_STATUS";
    case  PO_BSR:
      return  "PO_BSR";
    case  PO_FSR0:
      return  "PO_FSR0";
    case  PO_INDF0:
      return  "PO_INDF0";
    case  PO_INTCON:
      return  "PO_INTCON";
    case  PO_GPR_REGISTER:
      return  "PO_GPR_REGISTER";
    case  PO_GPR_BIT:
      return  "PO_GPR_BIT";
    case  PO_GPR_TEMP:
      return  "PO_GPR_TEMP";
    case  PO_SFR_REGISTER:
      return  "PO_SFR_REGISTER";
    case  PO_PCL:
      return  "PO_PCL";
    case  PO_PCLATH:
      return  "PO_PCLATH";
    case PO_LITERAL:
      return  "PO_LITERAL";
    case PO_REL_ADDR:
      return "PO_REL_ADDR";
    case  PO_IMMEDIATE:
      return  "PO_IMMEDIATE";
    case  PO_DIR:
      return  "PO_DIR";
    case  PO_CRY:
      return  "PO_CRY";
    case  PO_BIT:
      return  "PO_BIT";
    case  PO_STR:
      return  "PO_STR";
    case  PO_LABEL:
      return  "PO_LABEL";
    case  PO_WILD:
      return  "PO_WILD";
    }
  }

  return "BAD PO_TYPE";
}

/*-----------------------------------------------------------------*/
/* pic16_genPlusIncr :- does addition with increment if possible         */
/*-----------------------------------------------------------------*/
bool pic16_genPlusIncr (iCode *ic)
{
    unsigned int icount ;
    unsigned int size = pic16_getDataSize(IC_RESULT(ic));

    DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
    DEBUGpic16_emitcode ("; ","result %s, left %s, right %s",
			 pic16_AopType(AOP_TYPE(IC_RESULT(ic))),
			 pic16_AopType(AOP_TYPE(IC_LEFT(ic))),
			 pic16_AopType(AOP_TYPE(IC_RIGHT(ic))));

    /* will try to generate an increment */
    /* if the right side is not a literal 
       we cannot */
    if (AOP_TYPE(IC_RIGHT(ic)) != AOP_LIT)
        return FALSE ;
    
    DEBUGpic16_emitcode ("; ","%s  %d",__FUNCTION__,__LINE__);
    /* if the literal value of the right hand side
       is greater than 1 then it is faster to add */
    if ((icount = (unsigned int) floatFromVal (AOP(IC_RIGHT(ic))->aopu.aop_lit)) > 1)		// this was > 2 why? VR
        return FALSE ;
    
    /* if increment 16 bits in register */
    if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) &&
        (icount == 1)) {

      int offset = MSB16;

      pic16_emitpcode(POC_INCF, pic16_popGet(AOP(IC_RESULT(ic)),LSB));
      //pic16_emitcode("incf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),LSB,FALSE,FALSE));

      while(--size) {
	emitSKPNZ;
	pic16_emitpcode(POC_INCF, pic16_popGet(AOP(IC_RESULT(ic)),offset++));
	//pic16_emitcode(" incf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),offset++,FALSE,FALSE));
      }

      return TRUE;
    }
    
    DEBUGpic16_emitcode ("; ","%s  %d",__FUNCTION__,__LINE__);
    /* if left is in accumulator  - probably a bit operation*/				// VR - why this is a bit operation?!
    if( strcmp(pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE),"a")  &&
	(AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) ) {
      
      pic16_emitpcode(POC_BCF, pic16_popGet(AOP(IC_RESULT(ic)),0));
      pic16_emitcode("bcf","(%s >> 3), (%s & 7)",
	       AOP(IC_RESULT(ic))->aopu.aop_dir,
	       AOP(IC_RESULT(ic))->aopu.aop_dir);
      if(icount)
	pic16_emitpcode(POC_XORLW,pic16_popGetLit(1));
      //pic16_emitcode("xorlw","1");
      else
	pic16_emitpcode(POC_ANDLW,pic16_popGetLit(1));
      //pic16_emitcode("andlw","1");

      emitSKPZ;
      pic16_emitpcode(POC_BSF, pic16_popGet(AOP(IC_RESULT(ic)),0));
      pic16_emitcode("bsf","(%s >> 3), (%s & 7)",
	       AOP(IC_RESULT(ic))->aopu.aop_dir,
	       AOP(IC_RESULT(ic))->aopu.aop_dir);

      return TRUE;
    }



    /* if the sizes are greater than 1 then we cannot */
    if (AOP_SIZE(IC_RESULT(ic)) > 1 ||
        AOP_SIZE(IC_LEFT(ic)) > 1   )
        return FALSE ;
    
    /* If we are incrementing the same register by two: */

    if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
	
      while (icount--) 
	pic16_emitpcode(POC_INCF, pic16_popGet(AOP(IC_RESULT(ic)),0));
      //pic16_emitcode("incf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
	
      return TRUE ;
    }
    
    DEBUGpic16_emitcode ("; ","couldn't increment ");

    return FALSE ;
}

/*-----------------------------------------------------------------*/
/* pic16_outBitAcc - output a bit in acc                                 */
/*-----------------------------------------------------------------*/
void pic16_outBitAcc(operand *result)
{
    symbol *tlbl = newiTempLabel(NULL);
    /* if the result is a bit */
    DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);

    if (AOP_TYPE(result) == AOP_CRY){
        pic16_aopPut(AOP(result),"a",0);
    }
    else {
        pic16_emitcode("jz","%05d_DS_",tlbl->key+100);
        pic16_emitcode("mov","a,#01");
        pic16_emitcode("","%05d_DS_:",tlbl->key+100);
        pic16_outAcc(result);
    }
}

/*-----------------------------------------------------------------*/
/* pic16_genPlusBits - generates code for addition of two bits           */
/*-----------------------------------------------------------------*/
void pic16_genPlusBits (iCode *ic)
{

  DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);

  DEBUGpic16_emitcode ("; ","result %s, left %s, right %s",
		       pic16_AopType(AOP_TYPE(IC_RESULT(ic))),
		       pic16_AopType(AOP_TYPE(IC_LEFT(ic))),
		       pic16_AopType(AOP_TYPE(IC_RIGHT(ic))));
  /*
    The following block of code will add two bits. 
    Note that it'll even work if the destination is
    the carry (C in the status register).
    It won't work if the 'Z' bit is a source or destination.
  */

  /* If the result is stored in the accumulator (w) */
  //if(strcmp(pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE),"a") == 0 ) {
  switch(AOP_TYPE(IC_RESULT(ic))) {
  case AOP_ACC:
    pic16_emitpcode(POC_CLRF, pic16_popCopyReg(&pic16_pc_wreg));
    pic16_emitpcode(POC_BTFSC, pic16_popGet(AOP(IC_RIGHT(ic)),0));
    pic16_emitpcode(POC_XORLW, pic16_popGetLit(1));
    pic16_emitpcode(POC_BTFSC, pic16_popGet(AOP(IC_LEFT(ic)),0));
    pic16_emitpcode(POC_XORLW, pic16_popGetLit(1));

    pic16_emitcode("clrw","");
    pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
		   AOP(IC_RIGHT(ic))->aopu.aop_dir,
		   AOP(IC_RIGHT(ic))->aopu.aop_dir);
    pic16_emitcode("xorlw","1");
    pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
		   AOP(IC_LEFT(ic))->aopu.aop_dir,
		   AOP(IC_LEFT(ic))->aopu.aop_dir);
    pic16_emitcode("xorlw","1");
    break;
  case AOP_REG:
    pic16_emitpcode(POC_MOVLW, pic16_popGetLit(0));
    pic16_emitpcode(POC_BTFSC, pic16_popGet(AOP(IC_RIGHT(ic)),0));
    pic16_emitpcode(POC_XORLW, pic16_popGetLit(1));
    pic16_emitpcode(POC_BTFSC, pic16_popGet(AOP(IC_LEFT(ic)),0));
    pic16_emitpcode(POC_XORLW, pic16_popGetLit(1));
    pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(IC_RESULT(ic)),0));
    break;
  default:
    pic16_emitpcode(POC_MOVLW, pic16_popGet(AOP(IC_RESULT(ic)),0));
    pic16_emitpcode(POC_BCF,   pic16_popGet(AOP(IC_RESULT(ic)),0));
    pic16_emitpcode(POC_BTFSC, pic16_popGet(AOP(IC_RIGHT(ic)),0));
    pic16_emitpcode(POC_XORWF, pic16_popGet(AOP(IC_RESULT(ic)),0));
    pic16_emitpcode(POC_BTFSC, pic16_popGet(AOP(IC_LEFT(ic)),0));
    pic16_emitpcode(POC_XORWF, pic16_popGet(AOP(IC_RESULT(ic)),0));

    pic16_emitcode("movlw","(1 << (%s & 7))",
		   AOP(IC_RESULT(ic))->aopu.aop_dir,
		   AOP(IC_RESULT(ic))->aopu.aop_dir);
    pic16_emitcode("bcf","(%s >> 3), (%s & 7)",
		   AOP(IC_RESULT(ic))->aopu.aop_dir,
		   AOP(IC_RESULT(ic))->aopu.aop_dir);
    pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
		   AOP(IC_RIGHT(ic))->aopu.aop_dir,
		   AOP(IC_RIGHT(ic))->aopu.aop_dir);
    pic16_emitcode("xorwf","(%s >>3),f",
		   AOP(IC_RESULT(ic))->aopu.aop_dir);
    pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
		   AOP(IC_LEFT(ic))->aopu.aop_dir,
		   AOP(IC_LEFT(ic))->aopu.aop_dir);
    pic16_emitcode("xorwf","(%s>>3),f",
		   AOP(IC_RESULT(ic))->aopu.aop_dir);
    break;
  }

}

#if 0
/* This is the original version of this code.
 *
 * This is being kept around for reference, 
 * because I am not entirely sure I got it right...
 */
static void adjustArithmeticResult(iCode *ic)
{
    if (AOP_SIZE(IC_RESULT(ic)) == 3 && 
	AOP_SIZE(IC_LEFT(ic)) == 3   &&
	!pic16_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))))
	pic16_aopPut(AOP(IC_RESULT(ic)),
	       pic16_aopGet(AOP(IC_LEFT(ic)),2,FALSE,FALSE),
	       2);

    if (AOP_SIZE(IC_RESULT(ic)) == 3 && 
	AOP_SIZE(IC_RIGHT(ic)) == 3   &&
	!pic16_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_RIGHT(ic))))
	pic16_aopPut(AOP(IC_RESULT(ic)),
	       pic16_aopGet(AOP(IC_RIGHT(ic)),2,FALSE,FALSE),
	       2);
    
    if (AOP_SIZE(IC_RESULT(ic)) == 3 &&
	AOP_SIZE(IC_LEFT(ic)) < 3    &&
	AOP_SIZE(IC_RIGHT(ic)) < 3   &&
	!pic16_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))) &&
	!pic16_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_RIGHT(ic)))) {
	char buffer[5];
	sprintf(buffer,"#%d",pointerCode(getSpec(operandType(IC_LEFT(ic)))));
	pic16_aopPut(AOP(IC_RESULT(ic)),buffer,2);
    }
}
//#else
/* This is the pure and virtuous version of this code.
 * I'm pretty certain it's right, but not enough to toss the old 
 * code just yet...
 */
static void adjustArithmeticResult(iCode *ic)
{
    if (opIsGptr(IC_RESULT(ic)) &&
    	opIsGptr(IC_LEFT(ic))   &&
	!pic16_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))))
    {
	pic16_aopPut(AOP(IC_RESULT(ic)),
	       pic16_aopGet(AOP(IC_LEFT(ic)), GPTRSIZE - 1,FALSE,FALSE),
	       GPTRSIZE - 1);
    }

    if (opIsGptr(IC_RESULT(ic)) &&
        opIsGptr(IC_RIGHT(ic))   &&
	!pic16_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_RIGHT(ic))))
    {
	pic16_aopPut(AOP(IC_RESULT(ic)),
	       pic16_aopGet(AOP(IC_RIGHT(ic)),GPTRSIZE - 1,FALSE,FALSE),
	       GPTRSIZE - 1);
    }

    if (opIsGptr(IC_RESULT(ic)) 	   &&
        AOP_SIZE(IC_LEFT(ic)) < GPTRSIZE   &&
        AOP_SIZE(IC_RIGHT(ic)) < GPTRSIZE  &&
	 !pic16_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))) &&
	 !pic16_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_RIGHT(ic)))) {
	 char buffer[5];
	 sprintf(buffer,"#%d",pointerCode(getSpec(operandType(IC_LEFT(ic)))));
	 pic16_aopPut(AOP(IC_RESULT(ic)),buffer,GPTRSIZE - 1);
     }
}
#endif

/*-----------------------------------------------------------------*/
/* genAddlit - generates code for addition                         */
/*-----------------------------------------------------------------*/
static void genAddLit2byte (operand *result, int offr, int lit)
{

  switch(lit & 0xff) {
  case 0:
    break;
  case 1:
    pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),offr));
    break;
  case 0xff:
    pic16_emitpcode(POC_DECF, pic16_popGet(AOP(result),offr));
    break;
  default:
    pic16_emitpcode(POC_MOVLW,pic16_popGetLit(lit&0xff));
    pic16_emitpcode(POC_ADDWF,pic16_popGet(AOP(result),offr));
  }

}

static void emitMOVWF(operand *reg, int offset)
{
  if(!reg)
    return;

  if (AOP_TYPE(reg) == AOP_ACC) {
    DEBUGpic16_emitcode ("; ***","%s  %d ignoring mov into W",__FUNCTION__,__LINE__);
    return;
  }

  pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(reg),offset));

}

static void genAddLit (iCode *ic, int lit)
{

  int size,same;
  int lo;

  operand *result;
  operand *left;

  DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);


  left = IC_LEFT(ic);
  result = IC_RESULT(ic);
  same = pic16_sameRegs(AOP(left), AOP(result));
  size = pic16_getDataSize(result);

  if(same) {

    /* Handle special cases first */
    if(size == 1) 
      genAddLit2byte (result, 0, lit);
     
    else if(size == 2) {
      int hi = 0xff & (lit >> 8);
      lo = lit & 0xff;

      switch(hi) {
      case 0: 

	/* lit = 0x00LL */
	DEBUGpic16_emitcode ("; hi = 0","%s  %d",__FUNCTION__,__LINE__);
	switch(lo) {
	case 0:
	  break;
	case 1:
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),0));
	  emitSKPNZ;
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),MSB16));
	  break;
	case 0xff:
	  pic16_emitpcode(POC_DECF, pic16_popGet(AOP(result),0));
	  pic16_emitpcode(POC_INCFSZW, pic16_popGet(AOP(result),0));
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),MSB16));

	  break;
	default:
	  pic16_emitpcode(POC_MOVLW,pic16_popGetLit(lit&0xff));
	  pic16_emitpcode(POC_ADDWF,pic16_popGet(AOP(result),0));
	  emitSKPNC;
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),MSB16));


	}
	break;

      case 1:
	/* lit = 0x01LL */
	DEBUGpic16_emitcode ("; hi = 1","%s  %d",__FUNCTION__,__LINE__);
	switch(lo) {
	case 0:  /* 0x0100 */
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),MSB16));
	  break;
	case 1:  /* 0x0101  */
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),MSB16));
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),0));
	  emitSKPNZ;
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),MSB16));
	  break;
	case 0xff: /* 0x01ff */
	  pic16_emitpcode(POC_DECF, pic16_popGet(AOP(result),0));
	  pic16_emitpcode(POC_INCFSZW, pic16_popGet(AOP(result),0));
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),MSB16));
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),MSB16));
	}	  
	break;

      case 0xff:
	DEBUGpic16_emitcode ("; hi = ff","%s  %d",__FUNCTION__,__LINE__);
	/* lit = 0xffLL */
	switch(lo) {
	case 0:  /* 0xff00 */
	  pic16_emitpcode(POC_DECF, pic16_popGet(AOP(result),MSB16));
	  break;
	case 1:  /*0xff01 */
	  pic16_emitpcode(POC_INCFSZ, pic16_popGet(AOP(result),0));
	  pic16_emitpcode(POC_DECF, pic16_popGet(AOP(result),MSB16));
	  break;
/*	case 0xff: * 0xffff *
	  pic16_emitpcode(POC_INCFSZW, pic16_popGet(AOP(result),0,FALSE,FALSE));
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),MSB16,FALSE,FALSE));
	  pic16_emitpcode(POC_DECF, pic16_popGet(AOP(result),0,FALSE,FALSE));
	  break;
*/
	default:
	  pic16_emitpcode(POC_MOVLW,pic16_popGetLit(lo));
	  pic16_emitpcode(POC_ADDWF,pic16_popGet(AOP(result),0));
	  emitSKPC;
	  pic16_emitpcode(POC_DECF, pic16_popGet(AOP(result),MSB16));
	  
	}

	break;
	
      default:
	DEBUGpic16_emitcode ("; hi is generic","%d   %s  %d",hi,__FUNCTION__,__LINE__);

	/* lit = 0xHHLL */
	switch(lo) {
	case 0:  /* 0xHH00 */
	  genAddLit2byte (result, MSB16, hi);
	  break;
	case 1:  /* 0xHH01 */
	  pic16_emitpcode(POC_MOVLW,pic16_popGetLit((hi+1)&0xff));
	  pic16_emitpcode(POC_INCFSZ, pic16_popGet(AOP(result),0));
	  pic16_emitpcode(POC_MOVLW,pic16_popGetLit(hi));
	  pic16_emitpcode(POC_ADDWF,pic16_popGet(AOP(result),MSB16));
	  break;
/*	case 0xff: * 0xHHff *
	  pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(result),0,FALSE,FALSE));
	  pic16_emitpcode(POC_DECF, pic16_popGet(AOP(result),MSB16,FALSE,FALSE));
	  pic16_emitpcode(POC_MOVLW,pic16_popGetLit(hi));
	  pic16_emitpcode(POC_ADDWF,pic16_popGet(AOP(result),MSB16,FALSE,FALSE));
	  break;
*/	default:  /* 0xHHLL */
	  pic16_emitpcode(POC_MOVLW,pic16_popGetLit(lo));
	  pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(result),0));
	  pic16_emitpcode(POC_MOVLW,pic16_popGetLit(hi));
	  emitSKPNC;
	  pic16_emitpcode(POC_MOVLW,pic16_popGetLit((hi+1) & 0xff));
	  pic16_emitpcode(POC_ADDWF,pic16_popGet(AOP(result),MSB16));
	  break;
	}

      }
    } else {
      int carry_info = 0;
      int offset = 0;
      /* size > 2 */
      DEBUGpic16_emitcode (";  add lit to long","%s  %d",__FUNCTION__,__LINE__);

      while(size--) {
	lo = BYTEofLONG(lit,0);

	if(carry_info) {
	  switch(lo) {
	  case 0:
	    switch(carry_info) {
	    case 1:
	      emitSKPNZ;
	      pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),offset));
	      break;
	    case 2:
	      pic16_emitpcode(POC_RLCFW, pic16_popGet(AOP(result),offset));
	      pic16_emitpcode(POC_ANDLW, pic16_popGetLit(1));
	      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(result),offset));
	      break;
	    default: /* carry_info = 3  */
	      emitSKPNC;
	      pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),offset));
	      carry_info = 1;
	      break;
	    }
	    break;
	  case 0xff:
	    pic16_emitpcode(POC_MOVLW,pic16_popGetLit(lo));
	    if(carry_info==1) 
	      emitSKPZ;
	    else
	      emitSKPC;
	    pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(result),offset));
	    break;
	  default:
	    pic16_emitpcode(POC_MOVLW,pic16_popGetLit(lo));
	    if(carry_info==1) 
	      emitSKPNZ;
	    else
	      emitSKPNC;
	    pic16_emitpcode(POC_MOVLW,pic16_popGetLit(lo+1));
	    pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(result),offset));
	    carry_info=2;
	    break;
	  }
	}else {
	  /* no carry info from previous step */
	  /* this means this is the first time to add */
	  switch(lo) {
	  case 0:
	    break;
	  case 1:
	    pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),offset));
	    carry_info=1;
	    break;
	  default:
	    pic16_emitpcode(POC_MOVLW,pic16_popGetLit(lo));
	    pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(result),offset));
	    if(lit <0x100) 
	      carry_info = 3;  /* Were adding only one byte and propogating the carry */
	    else
	      carry_info = 2;
	    break;
	  }
	}
	offset++;
	lit >>= 8;
      }
    
/*
      lo = BYTEofLONG(lit,0);

      if(lit < 0x100) {
	if(lo) {
	  if(lo == 1) {
	    pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),0,FALSE,FALSE));
	    emitSKPNZ;
	  } else {
	    pic16_emitpcode(POC_MOVLW,pic16_popGetLit(lo));
	    pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(result),0,FALSE,FALSE));
	    emitSKPNC;
	  }
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),1,FALSE,FALSE));
	  emitSKPNZ;
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),2,FALSE,FALSE));
	  emitSKPNZ;
	  pic16_emitpcode(POC_INCF, pic16_popGet(AOP(result),3,FALSE,FALSE));

	} 
      } 
    }

*/
    }
  } else {
    int offset = 1;
    DEBUGpic16_emitcode (";  left and result aren't same","%s  %d",__FUNCTION__,__LINE__);

    if(size == 1) {

      if(AOP_TYPE(left) == AOP_ACC) {
	/* left addend is already in accumulator */
	switch(lit & 0xff) {
	case 0:
	  //pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),0,FALSE,FALSE));
	  emitMOVWF(result,0);
	  break;
	default:
	  pic16_emitpcode(POC_ADDLW, pic16_popGetLit(lit & 0xff));
	  //pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),0,FALSE,FALSE));
	  emitMOVWF(result,0);
	}
      } else {
	/* left addend is in a register */
	switch(lit & 0xff) {
	case 0:
	  pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
	  emitMOVWF(result, 0);
	  //pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),0,FALSE,FALSE));
	  emitMOVWF(result,0);
	  break;
	case 1:
	  pic16_emitpcode(POC_INCFW, pic16_popGet(AOP(left),0));
	  //pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),0,FALSE,FALSE));
	  emitMOVWF(result,0);
	  break;
	case 0xff:
	  pic16_emitpcode(POC_DECFW, pic16_popGet(AOP(left),0));
	  //pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),0,FALSE,FALSE));
	  emitMOVWF(result,0);
	  break;
	default:
	  pic16_emitpcode(POC_MOVLW, pic16_popGetLit(lit & 0xff));
	  pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));
	  //pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),0,FALSE,FALSE));
	  emitMOVWF(result,0);
	}
      }

    } else {
      int clear_carry=0;

      /* left is not the accumulator */
      if(lit & 0xff) {
	pic16_emitpcode(POC_MOVLW, pic16_popGetLit(lit & 0xff));
	pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));
      } else {
	pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
	/* We don't know the state of the carry bit at this point */
	clear_carry = 1;
      }
      //pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),0,FALSE,FALSE));
      emitMOVWF(result,0);
      while(--size) {
      
	lit >>= 8;
	if(lit & 0xff) {
	  if(clear_carry) {
	    /* The ls byte of the lit must've been zero - that 
	       means we don't have to deal with carry */

	    pic16_emitpcode(POC_MOVLW, pic16_popGetLit(lit & 0xff));
	    pic16_emitpcode(POC_ADDFW,  pic16_popGet(AOP(left),offset));
	    pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(left),offset));

	    clear_carry = 0;

	  } else {
	    pic16_emitpcode(POC_MOVLW, pic16_popGetLit(lit & 0xff));
	    //pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),offset,FALSE,FALSE));
	    emitMOVWF(result,offset);
	    pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),offset));
	    emitSKPNC;
	    pic16_emitpcode(POC_INCFSZW,pic16_popGet(AOP(left),offset));
	    pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(result),offset));
	  }

	} else {
	  pic16_emitpcode(POC_CLRF,  pic16_popGet(AOP(result),offset));
	  pic16_emitpcode(POC_RLCF,  pic16_popGet(AOP(result),offset));
	  pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),offset));
	  pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(result),offset));
	}
	offset++;
      }
    }
  }
}

/*-----------------------------------------------------------------*/
/* pic16_genPlus - generates code for addition                     */
/*-----------------------------------------------------------------*/
void pic16_genPlus (iCode *ic)
{
  int size, offset = 0;
  operand *result, *left, *right;
  
  /* special cases :- */
  DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);


#if 1
	result = IC_RESULT(ic);
	left = IC_LEFT(ic);
	right = IC_RIGHT(ic);
  pic16_aopOp (left,ic,FALSE);
  pic16_aopOp (right,ic,FALSE);
  pic16_aopOp (result,ic,TRUE);
  DEBUGpic16_pic16_AopType(__LINE__,left, right, result);

#else
  pic16_aopOp (IC_LEFT(ic),ic,FALSE);
  pic16_aopOp (IC_RIGHT(ic),ic,FALSE);
  pic16_aopOp (IC_RESULT(ic),ic,TRUE);
  DEBUGpic16_pic16_AopType(__LINE__,IC_LEFT(ic),IC_RIGHT(ic),IC_RESULT(ic));
#endif


  /* if literal, literal on the right or
     if left requires ACC or right is already
     in ACC */

  if (AOP_TYPE(IC_LEFT(ic)) == AOP_LIT) {
    operand *t = IC_RIGHT(ic);
    IC_RIGHT(ic) = IC_LEFT(ic);
    IC_LEFT(ic) = t;
  }

  /* if both left & right are in bit space */
  if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY &&
      AOP_TYPE(IC_RIGHT(ic)) == AOP_CRY) {
    pic16_genPlusBits (ic);
    goto release ;
  }

  /* if left in bit space & right literal */
  if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY &&
      AOP_TYPE(IC_RIGHT(ic)) == AOP_LIT) {
    /* if result in bit space */
    if(AOP_TYPE(IC_RESULT(ic)) == AOP_CRY){
      if((unsigned long)floatFromVal(AOP(IC_RIGHT(ic))->aopu.aop_lit) != 0L) {
	pic16_emitpcode(POC_MOVLW, pic16_popGet(AOP(IC_RESULT(ic)),0));
	if (!pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) )
	  pic16_emitpcode(POC_BTFSC, pic16_popGet(AOP(IC_LEFT(ic)),0));
	pic16_emitpcode(POC_XORWF, pic16_popGet(AOP(IC_RESULT(ic)),0));
      }
    } else {
      size = pic16_getDataSize(IC_RESULT(ic));
      while (size--) {
	MOVA(pic16_aopGet(AOP(IC_RIGHT(ic)),offset,FALSE,FALSE));  
	pic16_emitcode("addc","a,#00  ;%d",__LINE__);
	pic16_aopPut(AOP(IC_RESULT(ic)),"a",offset++);
      }
    }
    goto release ;
  }

  /* if I can do an increment instead
     of add then GOOD for ME */
  if (pic16_genPlusIncr (ic) == TRUE)
    goto release;   

  size = pic16_getDataSize(IC_RESULT(ic));

  if(AOP(IC_RIGHT(ic))->type == AOP_LIT) {
    /* Add a literal to something else */
    //bool know_W=0;
    unsigned lit = (unsigned) floatFromVal(AOP(IC_RIGHT(ic))->aopu.aop_lit);
    //      unsigned l1=0;

    //      offset = 0;
    DEBUGpic16_emitcode(";","adding lit to something. size %d",size);

    genAddLit (ic,  lit);
    goto release;

  } else if(AOP_TYPE(IC_RIGHT(ic)) == AOP_CRY) {

    pic16_emitcode(";bitadd","right is bit: %s",pic16_aopGet(AOP(IC_RIGHT(ic)),0,FALSE,FALSE));
    pic16_emitcode(";bitadd","left is bit: %s",pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
    pic16_emitcode(";bitadd","result is bit: %s",pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));

    /* here we are adding a bit to a char or int */
    if(size == 1) {
      if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {

	pic16_emitpcode(POC_BTFSC , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	pic16_emitpcode(POC_INCF ,  pic16_popGet(AOP(IC_RESULT(ic)),0));

	pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
		       AOP(IC_RIGHT(ic))->aopu.aop_dir,
		       AOP(IC_RIGHT(ic))->aopu.aop_dir);
	pic16_emitcode(" incf","%s,f", pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
      } else {

	if(AOP_TYPE(IC_LEFT(ic)) == AOP_ACC) {
	  pic16_emitpcode(POC_BTFSC , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	  pic16_emitpcode(POC_XORLW , pic16_popGetLit(1));

	  pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
			 AOP(IC_RIGHT(ic))->aopu.aop_dir,
			 AOP(IC_RIGHT(ic))->aopu.aop_dir);
	  pic16_emitcode(" xorlw","1");
	} else {
	  pic16_emitpcode(POC_MOVFW , pic16_popGet(AOP(IC_LEFT(ic)),0));
	  pic16_emitpcode(POC_BTFSC , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	  pic16_emitpcode(POC_INCFW , pic16_popGet(AOP(IC_LEFT(ic)),0));

	  pic16_emitcode("movf","%s,w", pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
	  pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
			 AOP(IC_RIGHT(ic))->aopu.aop_dir,
			 AOP(IC_RIGHT(ic))->aopu.aop_dir);
	  pic16_emitcode(" incf","%s,w", pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
	}
	  
	if(AOP_TYPE(IC_RESULT(ic)) != AOP_ACC) {
	    
	  if(AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) {
	    pic16_emitpcode(POC_ANDLW , pic16_popGetLit(1));
	    pic16_emitpcode(POC_BCF ,   pic16_popGet(AOP(IC_RESULT(ic)),0));
	    emitSKPZ;
	    pic16_emitpcode(POC_BSF ,   pic16_popGet(AOP(IC_RESULT(ic)),0));
	  } else {
	    pic16_emitpcode(POC_MOVWF ,   pic16_popGet(AOP(IC_RESULT(ic)),0));
	    pic16_emitcode("movwf","%s", pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
	  }
	}
      }

    } else {
      int offset = 1;
      DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
      if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
	emitCLRZ;
	pic16_emitpcode(POC_BTFSC, pic16_popGet(AOP(IC_RIGHT(ic)),0));
	pic16_emitpcode(POC_INCF,  pic16_popGet(AOP(IC_RESULT(ic)),0));

	pic16_emitcode("clrz","");

	pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
		       AOP(IC_RIGHT(ic))->aopu.aop_dir,
		       AOP(IC_RIGHT(ic))->aopu.aop_dir);
	pic16_emitcode(" incf","%s,f", pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));

      } else {

	pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(IC_LEFT(ic)),0));
	pic16_emitpcode(POC_BTFSC, pic16_popGet(AOP(IC_RIGHT(ic)),0));
	pic16_emitpcode(POC_INCFW, pic16_popGet(AOP(IC_LEFT(ic)),0));
	//pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(IC_RIGHT(ic)),0,FALSE,FALSE));
	emitMOVWF(IC_RIGHT(ic),0);

	pic16_emitcode("movf","%s,w", pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
	pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
		       AOP(IC_RIGHT(ic))->aopu.aop_dir,
		       AOP(IC_RIGHT(ic))->aopu.aop_dir);
	pic16_emitcode(" incf","%s,w", pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
	pic16_emitcode("movwf","%s", pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));

      }

      while(--size){
	emitSKPZ;
	pic16_emitpcode(POC_INCF,  pic16_popGet(AOP(IC_RESULT(ic)),offset++));
	//pic16_emitcode(" incf","%s,f", pic16_aopGet(AOP(IC_RIGHT(ic)),offset++,FALSE,FALSE));
      }

    }
      
  } else {
    DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);    

    /* Add the first bytes */

    if(strcmp(pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE),"a") == 0 ) {
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(IC_RIGHT(ic)),0));
      pic16_emitpcode(POC_MOVWF,pic16_popGet(AOP(IC_RESULT(ic)),0));
    } else {

      if ( AOP_TYPE(IC_LEFT(ic)) == AOP_ACC) {
	pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(IC_RIGHT(ic)),0));
	if ( AOP_TYPE(IC_RESULT(ic)) != AOP_ACC)
	  pic16_emitpcode(POC_MOVWF,pic16_popGet(AOP(IC_RESULT(ic)),0));
      } else {

	pic16_emitpcode(POC_MOVFW,pic16_popGet(AOP(IC_RIGHT(ic)),0));

	if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) )
	  pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(IC_LEFT(ic)),0));
	else {
	  PIC_OPCODE poc = POC_ADDFW;

	  if ((AOP_TYPE(IC_LEFT(ic)) == AOP_PCODE) && (
	      (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_LITERAL) || 
	      (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_IMMEDIATE)))
	    poc = POC_ADDLW;
	  pic16_emitpcode(poc, pic16_popGet(AOP(IC_LEFT(ic)),0));
	  if ( AOP_TYPE(IC_RESULT(ic)) != AOP_ACC)
	    pic16_emitpcode(POC_MOVWF,pic16_popGet(AOP(IC_RESULT(ic)),0));
	}
      }
    }

    size = min( AOP_SIZE(IC_RESULT(ic)), AOP_SIZE(IC_RIGHT(ic))) - 1;
    offset = 1;


    while(size--){
      if (!pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
	pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(IC_LEFT(ic)),offset));
	pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(IC_RESULT(ic)),offset));

	pic16_emitcode("movf","%s,w",  pic16_aopGet(AOP(IC_LEFT(ic)),offset,FALSE,FALSE));
	pic16_emitcode("movwf","%s",  pic16_aopGet(AOP(IC_RESULT(ic)),offset,FALSE,FALSE));
      }

      pic16_emitpcode(POC_MOVFW,   pic16_popGet(AOP(IC_RIGHT(ic)),offset));
      emitSKPNC;
      pic16_emitpcode(POC_INCFSZW, pic16_popGet(AOP(IC_RIGHT(ic)),offset));
      pic16_emitpcode(POC_ADDWF,   pic16_popGet(AOP(IC_RESULT(ic)),offset));

      /*
	pic16_emitcode("movf","%s,w",  pic16_aopGet(AOP(IC_RIGHT(ic)),offset,FALSE,FALSE));
	emitSKPNC;
	pic16_emitcode("incfsz","%s,w",pic16_aopGet(AOP(IC_RIGHT(ic)),offset,FALSE,FALSE));
	pic16_emitcode("addwf","%s,f", pic16_aopGet(AOP(IC_RESULT(ic)),offset,FALSE,FALSE));
      */

      offset++;
    }

  }

  if (AOP_SIZE(IC_RESULT(ic)) > AOP_SIZE(IC_RIGHT(ic))) {
    int sign =  !(SPEC_USIGN(getSpec(operandType(IC_LEFT(ic)))) |
		  SPEC_USIGN(getSpec(operandType(IC_RIGHT(ic)))) );


    /* Need to extend result to higher bytes */
    size = AOP_SIZE(IC_RESULT(ic)) - AOP_SIZE(IC_RIGHT(ic)) - 1;

    /* First grab the carry from the lower bytes */
    pic16_emitpcode(POC_CLRF, pic16_popGet(AOP(IC_RESULT(ic)),offset));
    pic16_emitpcode(POC_RLCF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));


    if(sign) {
      /* Now this is really horrid. Gotta check the sign of the addends and propogate
       * to the result */

      pic16_emitpcode(POC_BTFSC, pic16_newpCodeOpBit(pic16_aopGet(AOP(IC_LEFT(ic)),offset-1,FALSE,FALSE),7,0));
      pic16_emitpcode(POC_DECF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));
      pic16_emitpcode(POC_BTFSC, pic16_newpCodeOpBit(pic16_aopGet(AOP(IC_RIGHT(ic)),offset-1,FALSE,FALSE),7,0));
      pic16_emitpcode(POC_DECF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));

      /* if chars or ints or being signed extended to longs: */
      if(size) {
	pic16_emitpcode(POC_MOVLW, pic16_popGetLit(0));
	pic16_emitpcode(POC_BTFSC, pic16_newpCodeOpBit(pic16_aopGet(AOP(IC_RESULT(ic)),offset,FALSE,FALSE),7,0));
	pic16_emitpcode(POC_MOVLW, pic16_popGetLit(0xff));
      }
    }

    offset++;
    while(size--) {
      
      if(sign)
	pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(IC_RESULT(ic)),offset));
      else
	pic16_emitpcode(POC_CLRF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));

      offset++;
    }
  }


  //adjustArithmeticResult(ic);

 release:
  pic16_freeAsmop(IC_LEFT(ic),NULL,ic,(RESULTONSTACK(ic) ? FALSE : TRUE));
  pic16_freeAsmop(IC_RIGHT(ic),NULL,ic,(RESULTONSTACK(ic) ? FALSE : TRUE));
  pic16_freeAsmop(IC_RESULT(ic),NULL,ic,TRUE);
}

/*-----------------------------------------------------------------*/
/* pic16_genMinusDec :- does subtraction with decrement if possible     */
/*-----------------------------------------------------------------*/
bool pic16_genMinusDec (iCode *ic)
{
    unsigned int icount ;
    unsigned int size = pic16_getDataSize(IC_RESULT(ic));

    DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
    /* will try to generate an increment */
    /* if the right side is not a literal 
    we cannot */
    if ((AOP_TYPE(IC_RIGHT(ic)) != AOP_LIT) || 
	(AOP_TYPE(IC_LEFT(ic)) == AOP_CRY) || 
	(AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) )
        return FALSE ;

    DEBUGpic16_emitcode ("; lit val","%d",(unsigned int) floatFromVal (AOP(IC_RIGHT(ic))->aopu.aop_lit));

    /* if the literal value of the right hand side
    is greater than 4 then it is not worth it */
    if ((icount = (unsigned int) floatFromVal (AOP(IC_RIGHT(ic))->aopu.aop_lit)) > 2)
        return FALSE ;

    /* if decrement 16 bits in register */
    if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) &&
        (size > 1) &&
        (icount == 1)) {

      if(size == 2) { 
	pic16_emitpcode(POC_DECF,    pic16_popGet(AOP(IC_RESULT(ic)),LSB));
	pic16_emitpcode(POC_INCFSZW, pic16_popGet(AOP(IC_RESULT(ic)),LSB));
	pic16_emitpcode(POC_INCF,    pic16_popGet(AOP(IC_RESULT(ic)),MSB16));
	pic16_emitpcode(POC_DECF,    pic16_popGet(AOP(IC_RESULT(ic)),MSB16));

	pic16_emitcode("decf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),LSB,FALSE,FALSE));
	pic16_emitcode("incfsz","%s,w",pic16_aopGet(AOP(IC_RESULT(ic)),LSB,FALSE,FALSE));
	pic16_emitcode(" decf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),MSB16,FALSE,FALSE));
      } else {
	/* size is 3 or 4 */
	pic16_emitpcode(POC_MOVLW,  pic16_popGetLit(0xff));
	pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(IC_RESULT(ic)),LSB));
	emitSKPNC;
	pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(IC_RESULT(ic)),MSB16));
	emitSKPNC;
	pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(IC_RESULT(ic)),MSB24));

	pic16_emitcode("movlw","0xff");
	pic16_emitcode("addwf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),LSB,FALSE,FALSE));

	emitSKPNC;
	pic16_emitcode("addwf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),MSB16,FALSE,FALSE));
	emitSKPNC;
	pic16_emitcode("addwf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),MSB24,FALSE,FALSE));

	if(size > 3) {
	  emitSKPNC;
	  pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(IC_RESULT(ic)),MSB32));

	  pic16_emitcode("skpnc","");
	  emitSKPNC;
	  pic16_emitcode("addwf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),MSB32,FALSE,FALSE));
	}

      }

      return TRUE;

    }

    /* if the sizes are greater than 1 then we cannot */
    if (AOP_SIZE(IC_RESULT(ic)) > 1 ||
        AOP_SIZE(IC_LEFT(ic)) > 1   )
        return FALSE ;

    /* we can if the aops of the left & result match or
    if they are in registers and the registers are the
    same */
    if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic)))) {

      while (icount--) 
	pic16_emitpcode(POC_DECF, pic16_popGet(AOP(IC_RESULT(ic)),0));

	//pic16_emitcode ("decf","%s,f",pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));

        return TRUE ;
    }

    DEBUGpic16_emitcode ("; returning"," result=%s, left=%s",
		   pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE),
		   pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
    if(size==1) {

      pic16_emitcode("decf","%s,w",pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
      pic16_emitcode("movwf","%s",pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));

      pic16_emitpcode(POC_DECFW,  pic16_popGet(AOP(IC_LEFT(ic)),0));
      pic16_emitpcode(POC_MOVWF,  pic16_popGet(AOP(IC_RESULT(ic)),0));

      return TRUE;
    }

    return FALSE ;
}

/*-----------------------------------------------------------------*/
/* pic16_addSign - propogate sign bit to higher bytes                    */
/*-----------------------------------------------------------------*/
void pic16_addSign(operand *result, int offset, int sign)
{
  int size = (pic16_getDataSize(result) - offset);
  DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);

  if(size > 0){
    if(sign && offset) {

      if(size == 1) {
	pic16_emitpcode(POC_CLRF,pic16_popGet(AOP(result),offset));
	pic16_emitpcode(POC_BTFSC,pic16_newpCodeOpBit(pic16_aopGet(AOP(result),offset-1,FALSE,FALSE),7,0));
	pic16_emitpcode(POC_DECF, pic16_popGet(AOP(result),offset));
      } else {

	pic16_emitpcode(POC_MOVLW, pic16_popGetLit(0));
	pic16_emitpcode(POC_BTFSC, pic16_newpCodeOpBit(pic16_aopGet(AOP(result),offset-1,FALSE,FALSE),7,0));
	pic16_emitpcode(POC_MOVLW, pic16_popGetLit(0xff));
	while(size--)
	  pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),size));

      }
    } else
      while(size--)
	pic16_emitpcode(POC_CLRF,pic16_popGet(AOP(result),offset++));
  }
}

/*-----------------------------------------------------------------*/
/* pic16_genMinusBits - generates code for subtraction  of two bits      */
/*-----------------------------------------------------------------*/
void pic16_genMinusBits (iCode *ic)
{
    symbol *lbl = newiTempLabel(NULL);
    DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
    if (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY){
        pic16_emitcode("mov","c,%s",AOP(IC_LEFT(ic))->aopu.aop_dir);
        pic16_emitcode("jnb","%s,%05d_DS_",AOP(IC_RIGHT(ic))->aopu.aop_dir,(lbl->key+100));
        pic16_emitcode("cpl","c");
        pic16_emitcode("","%05d_DS_:",(lbl->key+100));
        pic16_outBitC(IC_RESULT(ic));
    }
    else{
        pic16_emitcode("mov","c,%s",AOP(IC_RIGHT(ic))->aopu.aop_dir);
        pic16_emitcode("subb","a,acc");
        pic16_emitcode("jnb","%s,%05d_DS_",AOP(IC_LEFT(ic))->aopu.aop_dir,(lbl->key+100));
        pic16_emitcode("inc","a");
        pic16_emitcode("","%05d_DS_:",(lbl->key+100));
        pic16_aopPut(AOP(IC_RESULT(ic)),"a",0);
        pic16_addSign(IC_RESULT(ic), MSB16, SPEC_USIGN(getSpec(operandType(IC_RESULT(ic)))));
    }
}

/*-----------------------------------------------------------------*/
/* pic16_genMinus - generates code for subtraction                       */
/*-----------------------------------------------------------------*/
void pic16_genMinus (iCode *ic)
{
  int size, offset = 0, same=0;
  unsigned long lit = 0L;

  DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  pic16_aopOp (IC_LEFT(ic),ic,FALSE);
  pic16_aopOp (IC_RIGHT(ic),ic,FALSE);
  pic16_aopOp (IC_RESULT(ic),ic,TRUE);

  if (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY  &&
      AOP_TYPE(IC_RIGHT(ic)) == AOP_LIT) {
    operand *t = IC_RIGHT(ic);
    IC_RIGHT(ic) = IC_LEFT(ic);
    IC_LEFT(ic) = t;
  }

  DEBUGpic16_emitcode ("; ","result %s, left %s, right %s",
		   pic16_AopType(AOP_TYPE(IC_RESULT(ic))),
		   pic16_AopType(AOP_TYPE(IC_LEFT(ic))),
		   pic16_AopType(AOP_TYPE(IC_RIGHT(ic))));

  /* special cases :- */
  /* if both left & right are in bit space */
  if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY &&
      AOP_TYPE(IC_RIGHT(ic)) == AOP_CRY) {
    pic16_genPlusBits (ic);
    goto release ;
  }

  /* if I can do an decrement instead
     of subtract then GOOD for ME */
  //  if (pic16_genMinusDec (ic) == TRUE)
  //    goto release;   

  size = pic16_getDataSize(IC_RESULT(ic));   
  same = pic16_sameRegs(AOP(IC_RIGHT(ic)), AOP(IC_RESULT(ic)));

  if(AOP(IC_RIGHT(ic))->type == AOP_LIT) {
    /* Add a literal to something else */

    lit = (unsigned long)floatFromVal(AOP(IC_RIGHT(ic))->aopu.aop_lit);
    lit = - (long)lit;

    genAddLit ( ic,  lit);
    
#if 0
    /* add the first byte: */
    pic16_emitcode("movlw","0x%x", lit & 0xff);
    pic16_emitcode("addwf","%s,f", pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
    pic16_emitpcode(POC_MOVLW,  pic16_popGetLit(lit & 0xff));
    pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(IC_LEFT(ic)),0));


    offset = 1;
    size--;

    while(size-- > 0) {

      lit >>= 8;

      if(lit & 0xff) {

	if((lit & 0xff) == 0xff) {
	  pic16_emitpcode(POC_MOVLW,  pic16_popGetLit(0xff));
	  emitSKPC;
	  pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(IC_LEFT(ic)),offset));
	} else {
	  pic16_emitpcode(POC_MOVLW,  pic16_popGetLit(lit & 0xff));
	  emitSKPNC;
	  pic16_emitpcode(POC_MOVLW,  pic16_popGetLit((lit+1) & 0xff));
	  pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(IC_LEFT(ic)),offset));
	}

      } else {
	/* do the rlf known zero trick here */
	pic16_emitpcode(POC_MOVLW,  pic16_popGetLit(1));
	emitSKPNC;
	pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(IC_LEFT(ic)),offset));
      }
      offset++;
    }
#endif
  } else if(AOP_TYPE(IC_RIGHT(ic)) == AOP_CRY) {
    // bit subtraction

    pic16_emitcode(";bitsub","right is bit: %s",pic16_aopGet(AOP(IC_RIGHT(ic)),0,FALSE,FALSE));
    pic16_emitcode(";bitsub","left is bit: %s",pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
    pic16_emitcode(";bitsub","result is bit: %s",pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));

    /* here we are subtracting a bit from a char or int */
    if(size == 1) {
      if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {

	pic16_emitpcode(POC_BTFSC , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	pic16_emitpcode(POC_DECF ,  pic16_popGet(AOP(IC_RESULT(ic)),0));

	pic16_emitcode("btfsc","(%s >> 3), (%s & 7)",
		 AOP(IC_RIGHT(ic))->aopu.aop_dir,
		 AOP(IC_RIGHT(ic))->aopu.aop_dir);
	pic16_emitcode(" incf","%s,f", pic16_aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
      } else {

	if(AOP_TYPE(IC_LEFT(ic)) == AOP_ACC) {
	  pic16_emitpcode(POC_BTFSC , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	  pic16_emitpcode(POC_XORLW , pic16_popGetLit(1));
	}else  if( (AOP_TYPE(IC_LEFT(ic)) == AOP_IMMD) ||
	      (AOP_TYPE(IC_LEFT(ic)) == AOP_LIT) ) {

	  lit = (unsigned long)floatFromVal(AOP(IC_LEFT(ic))->aopu.aop_lit);

	  if(AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) {
	    if (pic16_sameRegs(AOP(IC_RIGHT(ic)), AOP(IC_RESULT(ic))) ) {
	      if(lit & 1) {
		pic16_emitpcode(POC_MOVLW , pic16_popGet(AOP(IC_RIGHT(ic)),0));
		pic16_emitpcode(POC_XORWF , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	      }
	    }else{
	      pic16_emitpcode(POC_BCF ,     pic16_popGet(AOP(IC_RESULT(ic)),0));
	      if(lit & 1) 
		pic16_emitpcode(POC_BTFSS , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	      else
		pic16_emitpcode(POC_BTFSC , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	      pic16_emitpcode(POC_BSF ,     pic16_popGet(AOP(IC_RESULT(ic)),0));
	    }
	    goto release;
	  } else {
	    pic16_emitpcode(POC_MOVLW , pic16_popGetLit(lit & 0xff));
	    pic16_emitpcode(POC_BTFSC , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	    pic16_emitpcode(POC_MOVLW , pic16_popGetLit((lit-1) & 0xff));
	    pic16_emitpcode(POC_MOVWF , pic16_popGet(AOP(IC_RESULT(ic)),0));

	  }

	} else {
	  pic16_emitpcode(POC_MOVFW , pic16_popGet(AOP(IC_LEFT(ic)),0));
	  pic16_emitpcode(POC_BTFSC , pic16_popGet(AOP(IC_RIGHT(ic)),0));
	  pic16_emitpcode(POC_DECFW , pic16_popGet(AOP(IC_LEFT(ic)),0));
	}
	  
	if(AOP_TYPE(IC_RESULT(ic)) != AOP_ACC) {
	    
	  pic16_emitpcode(POC_MOVWF ,   pic16_popGet(AOP(IC_RESULT(ic)),0));

	} else  {
	  pic16_emitpcode(POC_ANDLW , pic16_popGetLit(1));
/*
	  pic16_emitpcode(POC_BCF ,   pic16_popGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
	  emitSKPZ;
	  pic16_emitpcode(POC_BSF ,   pic16_popGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
*/
	}

      }

    }
  } else   if(// (AOP_TYPE(IC_LEFT(ic)) == AOP_IMMD) || 
	      (AOP(IC_LEFT(ic))->type == AOP_LIT) &&
	      (AOP_TYPE(IC_RIGHT(ic)) != AOP_ACC)) {

    lit = (unsigned long)floatFromVal(AOP(IC_LEFT(ic))->aopu.aop_lit);
    DEBUGpic16_emitcode ("; left is lit","line %d result %s, left %s, right %s",__LINE__,
		   pic16_AopType(AOP_TYPE(IC_RESULT(ic))),
		   pic16_AopType(AOP_TYPE(IC_LEFT(ic))),
		   pic16_AopType(AOP_TYPE(IC_RIGHT(ic))));


    if( (size == 1) && ((lit & 0xff) == 0) ) {
      /* res = 0 - right */
      if (pic16_sameRegs(AOP(IC_RIGHT(ic)), AOP(IC_RESULT(ic))) ) {
	pic16_emitpcode(POC_COMF,  pic16_popGet(AOP(IC_RIGHT(ic)),0));
	pic16_emitpcode(POC_INCF,  pic16_popGet(AOP(IC_RIGHT(ic)),0));
      } else { 
	pic16_emitpcode(POC_COMFW,  pic16_popGet(AOP(IC_RIGHT(ic)),0));
	pic16_emitpcode(POC_MOVWF,  pic16_popGet(AOP(IC_RESULT(ic)),0));
	pic16_emitpcode(POC_INCF,   pic16_popGet(AOP(IC_RESULT(ic)),0));
      }
      goto release;
    }

    pic16_emitpcode(POC_MOVFW,  pic16_popGet(AOP(IC_RIGHT(ic)),0));
    pic16_emitpcode(POC_SUBLW, pic16_popGetLit(lit & 0xff));    
    pic16_emitpcode(POC_MOVWF,pic16_popGet(AOP(IC_RESULT(ic)),0));


    offset = 1;
    while(--size) {
      lit >>= 8;

      if(size == 1) {
	/* This is the last byte in a multibyte subtraction 
	 * There are a couple of tricks we can do by not worrying about 
	 * propogating the carry */
	if(lit == 0xff) {
	  /* 0xff - x == ~x */
	  if(same) {
	    pic16_emitpcode(POC_COMF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));
	    emitSKPC;
	    pic16_emitpcode(POC_DECF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));
	  } else {
	    pic16_emitpcode(POC_COMFW, pic16_popGet(AOP(IC_RIGHT(ic)),offset));
	    pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(IC_RESULT(ic)),offset));
	    emitSKPC;
	    pic16_emitpcode(POC_DECF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));
	  }
	} else {
	    pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(IC_RIGHT(ic)),offset));
	    emitSKPC;
	    pic16_emitpcode(POC_INCFW, pic16_popGet(AOP(IC_RIGHT(ic)),offset));
	    pic16_emitpcode(POC_SUBLW, pic16_popGetLit(lit & 0xff));
	    pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(IC_RESULT(ic)),offset));
	}

	goto release;
      }

      if(same) {

	if(lit & 0xff) {
	  pic16_emitpcode(POC_MOVLW, pic16_popGetLit(lit & 0xff));
	  emitSKPC;
	  pic16_emitpcode(POC_MOVLW, pic16_popGetLit((lit & 0xff)-1));
	  pic16_emitpcode(POC_SUBWF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));
	} else {
	  emitSKPNC;
	  pic16_emitpcode(POC_SUBWF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));

	}
      } else {

	if(lit & 0xff) {
	  pic16_emitpcode(POC_MOVLW, pic16_popGetLit(lit & 0xff));
	  pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(IC_RESULT(ic)),offset));
	} else
	  pic16_emitpcode(POC_CLRF, pic16_popGet(AOP(IC_RESULT(ic)),offset));

	pic16_emitpcode(POC_MOVFW,  pic16_popGet(AOP(IC_RIGHT(ic)),offset));
	emitSKPC;
	pic16_emitpcode(POC_INCFSZW,pic16_popGet(AOP(IC_RIGHT(ic)),offset));
	pic16_emitpcode(POC_SUBWF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));
      }
    }
  

  } else {

    DEBUGpic16_emitcode ("; ","line %d result %s, left %s, right %s",__LINE__,
		   pic16_AopType(AOP_TYPE(IC_RESULT(ic))),
		   pic16_AopType(AOP_TYPE(IC_LEFT(ic))),
		   pic16_AopType(AOP_TYPE(IC_RIGHT(ic))));

    if(strcmp(pic16_aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE),"a") == 0 ) {
      DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
      pic16_emitpcode(POC_SUBFW, pic16_popGet(AOP(IC_RIGHT(ic)),0));
      pic16_emitpcode(POC_SUBLW, pic16_popGetLit(0));
      pic16_emitpcode(POC_MOVWF,pic16_popGet(AOP(IC_RESULT(ic)),0));
    } else {

      if ( AOP_TYPE(IC_LEFT(ic)) == AOP_ACC) {
	pic16_emitpcode(POC_SUBFW, pic16_popGet(AOP(IC_RIGHT(ic)),0));
	pic16_emitpcode(POC_SUBLW, pic16_popGetLit(0));
	if ( AOP_TYPE(IC_RESULT(ic)) != AOP_ACC)
	  pic16_emitpcode(POC_MOVWF,pic16_popGet(AOP(IC_RESULT(ic)),0));
      } else {

	DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
	if(AOP_TYPE(IC_RIGHT(ic)) != AOP_ACC) 
	  pic16_emitpcode(POC_MOVFW,pic16_popGet(AOP(IC_RIGHT(ic)),0));

	if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) )
	  pic16_emitpcode(POC_SUBWF, pic16_popGet(AOP(IC_LEFT(ic)),0));
	else {
	  if( (AOP_TYPE(IC_LEFT(ic)) == AOP_IMMD) ||
	      (AOP_TYPE(IC_LEFT(ic)) == AOP_LIT) ) {
	    pic16_emitpcode(POC_SUBLW, pic16_popGet(AOP(IC_LEFT(ic)),0));
	  } else {
	    pic16_emitpcode(POC_SUBFW, pic16_popGet(AOP(IC_LEFT(ic)),0));
	  }
	  if ( AOP_TYPE(IC_RESULT(ic)) != AOP_ACC) {
	    if ( AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) {
	      pic16_emitpcode(POC_BCF ,   pic16_popGet(AOP(IC_RESULT(ic)),0));
	      emitSKPZ;
	      pic16_emitpcode(POC_BSF ,   pic16_popGet(AOP(IC_RESULT(ic)),0));
	    }else
	      pic16_emitpcode(POC_MOVWF,pic16_popGet(AOP(IC_RESULT(ic)),0));
	  }
	}
      }
    }

    /*
      pic16_emitpcode(POC_MOVFW,  pic16_popGet(AOP(IC_RIGHT(ic)),0,FALSE,FALSE));

      if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
      pic16_emitpcode(POC_SUBFW,  pic16_popGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
      } else {
      pic16_emitpcode(POC_SUBFW,  pic16_popGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
      pic16_emitpcode(POC_MOVWF,  pic16_popGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
      }
    */
    offset = 1;
    size--;

    while(size--){
      if (!pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
	pic16_emitpcode(POC_MOVFW,  pic16_popGet(AOP(IC_LEFT(ic)),offset));
	pic16_emitpcode(POC_MOVWF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));
      }
      pic16_emitpcode(POC_MOVFW,  pic16_popGet(AOP(IC_RIGHT(ic)),offset));
      emitSKPC;
      pic16_emitpcode(POC_INCFSZW,pic16_popGet(AOP(IC_RIGHT(ic)),offset));
      pic16_emitpcode(POC_SUBWF,  pic16_popGet(AOP(IC_RESULT(ic)),offset));

      offset++;
    }

  }


  //    adjustArithmeticResult(ic);
        
 release:
  pic16_freeAsmop(IC_LEFT(ic),NULL,ic,(RESULTONSTACK(ic) ? FALSE : TRUE));
  pic16_freeAsmop(IC_RIGHT(ic),NULL,ic,(RESULTONSTACK(ic) ? FALSE : TRUE));
  pic16_freeAsmop(IC_RESULT(ic),NULL,ic,TRUE);
}
/*-----------------------------------------------------------------*
 * pic_genUMult8XLit_16 - unsigned multiplication of two 8-bit numbers.
 * 
 * 
 *-----------------------------------------------------------------*/
void pic16_genUMult8XLit_16 (operand *left,
			     operand *right,
			     operand *result,
			     pCodeOpReg *result_hi)

{

  unsigned int lit;
  unsigned int i,have_first_bit;
  int same;
  pCodeOp *temp;

  if (AOP_TYPE(right) != AOP_LIT){
    fprintf(stderr,"%s %d - right operand is not a literal\n",__FILE__,__LINE__);
    exit(1);
  }


  if(!result_hi) {
    result_hi = PCOR(pic16_popGet(AOP(result),1));
  }

  lit = (unsigned int)floatFromVal(AOP(right)->aopu.aop_lit);
  lit &= 0xff;
  pic16_emitcode(";","Unrolled 8 X 8 multiplication");

  same = pic16_sameRegs(AOP(left), AOP(result));

  if(same) {
    switch(lit) {
    case 0:
      pic16_emitpcode(POC_CLRF,  pic16_popGet(AOP(left),0));
      return;
    case 2:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      return;
    case 3:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      return;
    case 4:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      return;
    case 5:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 2*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 3*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 5*F
      return;
    case 6:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      return;
    case 7:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 2*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 3*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 5*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 7*F
      return;
    case 8:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 2*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 3*F
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 5*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 8*F
      return;
    case 9:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      return;
    case 10:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 2*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 3*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 5*F
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      return;
    case 11:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 2*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 3*F
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 5*F
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 8*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 11*F
      return;
    case 12:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));
      return;
    case 13:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 2*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 3*F
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 5*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 8*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 13*F
      return;
    case 14:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 2*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 3*F
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 5*F
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 8*F
      pic16_emitpcode(POC_ADDFW, pic16_popGet(AOP(left),0));  // W = 11*F
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(left),0));  // F = 14*F
      return;
    case 15:
      temp = pic16_popGetTempReg();
      if(!temp) {
	fprintf(stderr,"ERROR: unable to allocate register. %s:%d\n",__FUNCTION__,__LINE__);
	exit(1);
      }
      pic16_emitpcode(POC_SWAPFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_MOVWF,  temp);
      pic16_emitpcode(POC_ANDLW,  pic16_popGetLit(0xf0));
      pic16_emitpcode(POC_MOVWF,  pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_SWAPFW, temp);
      pic16_emitpcode(POC_SUBWF,  pic16_popGet(AOP(left),0));
      pic16_popReleaseTempReg(temp);
      return;
    case 16:
      pic16_emitpcode(POC_SWAPFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ANDLW,  pic16_popGetLit(0xf0));
      pic16_emitpcode(POC_MOVWF,  pic16_popGet(AOP(left),0));
      return;
    case 17:
      pic16_emitpcode(POC_SWAPFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ANDLW,  pic16_popGetLit(0xf0));
      pic16_emitpcode(POC_ADDWF,  pic16_popGet(AOP(left),0));
      return;
    case 32:
      pic16_emitpcode(POC_SWAPF,  pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_RLCFW,  pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ANDLW,  pic16_popGetLit(0xe0));
      pic16_emitpcode(POC_MOVWF,  pic16_popGet(AOP(left),0));
      return;
    case 64:
      pic16_emitpcode(POC_SWAPF,  pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_RLCF,   pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_RLCFW,  pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_ANDLW,  pic16_popGetLit(0xc0));
      pic16_emitpcode(POC_MOVWF,  pic16_popGet(AOP(left),0));
      return;
    case 128:
      pic16_emitpcode(POC_RRCFW,  pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_CLRF,   pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_RRCF,   pic16_popGet(AOP(left),0));
      return;

    }
  } else {

    switch(lit) {
    case 0:
      pic16_emitpcode(POC_CLRF,  pic16_popGet(AOP(result),0));
      pic16_emitpcode(POC_CLRF,  pic16_popCopyReg(result_hi));
      return;
    case 2:
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),0));
      pic16_emitpcode(POC_ADDWF, pic16_popGet(AOP(result),0));
      pic16_emitpcode(POC_CLRF,  pic16_popCopyReg(result_hi));
      pic16_emitpcode(POC_RLCF,  pic16_popCopyReg(result_hi));
      return;
    }

  }

  pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
  pic16_emitpcode(POC_CLRF,  pic16_popGet(AOP(result),0));
  pic16_emitpcode(POC_CLRF,  pic16_popCopyReg(result_hi));

  have_first_bit = 0;
  for(i=0; i<8; i++) {

    if(lit & 1) {
      pic16_emitpcode(POC_ADDWF, pic16_popCopyReg(result_hi));
      have_first_bit = 1;
    }

    if(have_first_bit) {
      pic16_emitpcode(POC_RRCF,  pic16_popCopyReg(result_hi));
      pic16_emitpcode(POC_RRCF,  pic16_popGet(AOP(result),0));
    }

    lit >>= 1;
  }

}

/*-----------------------------------------------------------------*
 * genUMult8X8_16 - unsigned multiplication of two 8-bit numbers.
 * 
 * 
 *-----------------------------------------------------------------*/
void pic16_genUMult8X8_16 (operand *left,
			   operand *right,
			   operand *result,
			   pCodeOpReg *result_hi)

{

  int i;
  int looped = 1;

  if(!result_hi) {
    result_hi = PCOR(pic16_popGet(AOP(result),1));
  }

  if (AOP_TYPE(right) == AOP_LIT) {
    pic16_genUMult8XLit_16(left,right,result,result_hi);
    return;
  }

  if(!looped) {
    pic16_emitcode(";","Unrolled 8 X 8 multiplication");

    pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(right),0));
    pic16_emitpcode(POC_CLRF,  pic16_popGet(AOP(result),0));
    pic16_emitpcode(POC_CLRF,  pic16_popCopyReg(result_hi));
    emitCLRC;

    for(i=0; i<8; i++) {
      pic16_emitpcode(POC_BTFSC,  pic16_newpCodeOpBit(pic16_aopGet(AOP(left),0,FALSE,FALSE),i,0));
      pic16_emitpcode(POC_ADDWF, pic16_popCopyReg(result_hi));
      pic16_emitpcode(POC_RRCF,  pic16_popCopyReg(result_hi));
      pic16_emitpcode(POC_RRCF,  pic16_popGet(AOP(result),0));
    }


    /*
      Here's another version that does the same thing and takes the 
      same number of instructions. The one above is slightly better
      because the entry instructions have a higher probability of
      being optimized out.
    */
    /*
      pic16_emitpcode(POC_CLRF,  pic16_popCopyReg(result_hi));
      pic16_emitpcode(POC_RRCFW, pic16_popGet(AOP(left),0));
      pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(result),0));
      pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(right),0));

      for(i=0; i<8; i++) {
      emitSKPNC;
      pic16_emitpcode(POC_ADDWF, pic16_popCopyReg(result_hi));
      pic16_emitpcode(POC_RRCF,  pic16_popCopyReg(result_hi));
      pic16_emitpcode(POC_RRCF,  pic16_popGet(AOP(result),0));
      }
    */

  } else {
    symbol  *tlbl = newiTempLabel(NULL);
    pCodeOp *temp;


    pic16_emitcode(";","Looped 8 X 8 multiplication");

    pic16_emitpcode(POC_CLRF,  pic16_popGet(AOP(result),0));
    pic16_emitpcode(POC_CLRF,  pic16_popCopyReg(result_hi));

    pic16_emitpcode(POC_BSF,   pic16_newpCodeOpBit(pic16_aopGet(AOP(result),0,FALSE,FALSE),7,0));

    pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(right),0));

    temp = pic16_popGetTempReg();
    pic16_emitpcode(POC_MOVWF, pic16_popCopyReg(PCOR(temp)));

    pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));

    pic16_emitpLabel(tlbl->key);

    pic16_emitpcode(POC_RRCF,  pic16_popCopyReg(PCOR(temp)));
    emitSKPNC;
    pic16_emitpcode(POC_ADDWF, pic16_popCopyReg(result_hi));

    pic16_emitpcode(POC_RRCF,  pic16_popCopyReg(result_hi));
    pic16_emitpcode(POC_RRCF,  pic16_popGet(AOP(result),0));

    emitSKPC;
    pic16_emitpcode(POC_GOTO,  pic16_popGetLabel(tlbl->key));

    pic16_popReleaseTempReg(temp);

  }
}

/*-----------------------------------------------------------------*
 * pic16_genSMult8X8_16 - signed multiplication of two 8-bit numbers
 *
 *  this routine will call the unsigned multiply routine and then
 * post-fix the sign bit.
 *-----------------------------------------------------------------*/
void pic16_genSMult8X8_16 (operand *left,
			   operand *right,
			   operand *result,
			   pCodeOpReg *result_hi)
{

  if(!result_hi) {
    result_hi = PCOR(pic16_popGet(AOP(result),1));
  }

  pic16_genUMult8X8_16(left,right,result,result_hi);

  pic16_emitpcode(POC_BTFSC, pic16_newpCodeOpBit(pic16_aopGet(AOP(left),0,FALSE,FALSE),7,0));
  pic16_emitpcode(POC_SUBWF, pic16_popCopyReg(result_hi));
  pic16_emitpcode(POC_MOVFW, pic16_popGet(AOP(left),0));
  pic16_emitpcode(POC_BTFSC, pic16_newpCodeOpBit(pic16_aopGet(AOP(right),0,FALSE,FALSE),7,0));
  pic16_emitpcode(POC_SUBWF, pic16_popGet(AOP(result),1));
  
}

/*-----------------------------------------------------------------*
 * pic16_genMult8X8_8 - multiplication of two 8-bit numbers
 *
 *  this routine will call the unsigned multiply 8X8=>16 routine and
 * then throw away the high byte of the result.
 *
 *-----------------------------------------------------------------*/
void pic16_genMult8X8_8 (operand *left,
			 operand *right,
			 operand *result)
{
  pCodeOp *result_hi = pic16_popGetTempReg();

  if (AOP_TYPE(right) == AOP_LIT)
    pic16_genUMult8XLit_16(left,right,result,PCOR(result_hi));
  else
    pic16_genUMult8X8_16(left,right,result,PCOR(result_hi));

  pic16_popReleaseTempReg(result_hi);
}
#if 0
/*-----------------------------------------------------------------*/
/* constMult - generates code for multiplication by a constant     */
/*-----------------------------------------------------------------*/
void genMultConst(unsigned C)
{

  unsigned lit;
  unsigned sr3; // Shift right 3
  unsigned mask;

  int size = 1;

  /*
    Convert a string of 3 binary 1's in the lit into
    0111 = 1000 - 1;
  */

  mask = 7 << ( (size*8) - 3);
  lit = C;
  sr3 = 0;

  while(mask < (1<<size*8)) {

    if( (mask & lit) == lit) {
      unsigned lsb;

      /* We found 3 (or more) consecutive 1's */

      lsb = mask & ~(mask & (mask-1));  // lsb of mask.

      consecutive_bits = ((lit + lsb) & lit) ^ lit;

      lit ^= consecutive_bits;

      mask <<= 3;

      sr3 |= (consecutive + lsb);

    }

    mask >>= 1;

  }

}

#endif
