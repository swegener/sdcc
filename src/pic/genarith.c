/*-------------------------------------------------------------------------
  genarith.c - source file for code generation - arithmetic 
  
  Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)
         and -  Jean-Louis VERN.jlvern@writeme.com (1999)
  Bug Fixes  -  Wojciech Stryjewski  wstryj1@tiger.lsu.edu (1999 v2.1.9a)
  PIC port   -  Scott Dattalo scott@dattalo.com (2000)
  
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
  000123 mlh  Moved aopLiteral to SDCCglue.c to help the split
      Made everything static
-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SDCCglobl.h"
#include "newalloc.h"

#if defined(_MSC_VER) && (_MSC_VER < 1300)
#define __FUNCTION__    __FILE__
#endif

#include "common.h"
#include "SDCCpeeph.h"
#include "ralloc.h"
#include "pcode.h"
#include "gen.h"


#define BYTEofLONG(l,b) ( (l>> (b<<3)) & 0xff)
void DEBUGpic14_AopType(int line_no, operand *left, operand *right, operand *result);

const char *AopType(short type)
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

void DebugAop(asmop *aop)
{
  if(!aop)
    return;
  printf("%s\n",AopType(aop->type));
  printf(" current offset: %d\n",aop->coff);
  printf("           size: %d\n",aop->size);

  switch(aop->type) {
  case AOP_LIT:
    printf("          name: %s\n",aop->aopu.aop_lit->name);
    break;
  case AOP_REG:
    printf("          name: %s\n",aop->aopu.aop_reg[0]->name);
    break;
  case AOP_CRY:
  case AOP_DIR:
    printf("          name: %s\n",aop->aopu.aop_dir);
    break;
  case AOP_DPTR:
  case AOP_DPTR2:
  case AOP_R0:
  case AOP_R1:
  case AOP_ACC:
    printf("not supported\n");
    break;
  case AOP_STK:
    printf("   Stack offset: %d\n",aop->aopu.aop_stk);
    break;
  case AOP_IMMD:
    printf("     immediate: %s\n",aop->aopu.aop_immd);
    break;
  case AOP_STR:
    printf("    aop_str[0]: %s\n",aop->aopu.aop_str[0]);
    break;
  case AOP_PCODE:
    //printpCode(stdout,aop->aopu.pcop);
    break;
  }
}

const char *pCodeOpType(  pCodeOp *pcop)
{
  
  if(pcop) {
    
    switch(pcop->type) {
      
    case  PO_NONE:
      return "PO_NONE";
    case  PO_W:
      return  "PO_W";
    case  PO_STATUS:
      return  "PO_STATUS";
    case  PO_FSR:
      return  "PO_FSR";
    case  PO_INDF:
      return  "PO_INDF";
    case  PO_INTCON:
      return  "PO_INTCON";
    case  PO_GPR_REGISTER:
      return  "PO_GPR_REGISTER";
    case  PO_GPR_POINTER:
      return  "PO_GPR_POINTER";
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
    case  PO_LITERAL:
      return  "PO_LITERAL";
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
/* genPlusIncr :- does addition with increment if possible         */
/*-----------------------------------------------------------------*/
bool genPlusIncr (iCode *ic)
{
  unsigned int icount ;
  unsigned int size = pic14_getDataSize(IC_RESULT(ic));
  
  DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  DEBUGpic14_emitcode ("; ","result %s, left %s, right %s",
    AopType(AOP_TYPE(IC_RESULT(ic))),
    AopType(AOP_TYPE(IC_LEFT(ic))),
    AopType(AOP_TYPE(IC_RIGHT(ic))));
  
  /* will try to generate an increment */
  /* if the right side is not a literal 
  we cannot */
  if (AOP_TYPE(IC_RIGHT(ic)) != AOP_LIT)
    return FALSE ;
  
  DEBUGpic14_emitcode ("; ","%s  %d",__FUNCTION__,__LINE__);
  /* if the literal value of the right hand side
  is greater than 1 then it is faster to add */
  if ((icount = (unsigned int) floatFromVal (AOP(IC_RIGHT(ic))->aopu.aop_lit)) > 2)
    return FALSE ;
  
  /* if increment 16 bits in register */
  if (pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) &&
    (icount == 1)) {
    
    int offset = MSB16;
    
    emitpcode(POC_INCF, popGet(AOP(IC_RESULT(ic)),LSB));
    //pic14_emitcode("incf","%s,f",aopGet(AOP(IC_RESULT(ic)),LSB,FALSE,FALSE));
    
    while(--size) {
      emitSKPNZ;
      emitpcode(POC_INCF, popGet(AOP(IC_RESULT(ic)),offset++));
      //pic14_emitcode(" incf","%s,f",aopGet(AOP(IC_RESULT(ic)),offset++,FALSE,FALSE));
    }
    
    return TRUE;
  }
  
  DEBUGpic14_emitcode ("; ","%s  %d",__FUNCTION__,__LINE__);
  /* if left is in accumulator  - probably a bit operation*/
  if( strcmp(aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE),"a")  &&
    (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) ) {
    
    emitpcode(POC_BCF, popGet(AOP(IC_RESULT(ic)),0));
    pic14_emitcode("bcf","(%s >> 3), (%s & 7)",
      AOP(IC_RESULT(ic))->aopu.aop_dir,
      AOP(IC_RESULT(ic))->aopu.aop_dir);
    if(icount)
      emitpcode(POC_XORLW,popGetLit(1));
    //pic14_emitcode("xorlw","1");
    else
      emitpcode(POC_ANDLW,popGetLit(1));
    //pic14_emitcode("andlw","1");
    
    emitSKPZ;
    emitpcode(POC_BSF, popGet(AOP(IC_RESULT(ic)),0));
    pic14_emitcode("bsf","(%s >> 3), (%s & 7)",
      AOP(IC_RESULT(ic))->aopu.aop_dir,
      AOP(IC_RESULT(ic))->aopu.aop_dir);
    
    return TRUE;
  }
  
  
  
  /* if the sizes are greater than 1 then we cannot */
  if (AOP_SIZE(IC_RESULT(ic)) > 1 ||
    AOP_SIZE(IC_LEFT(ic)) > 1   )
    return FALSE ;
  
  /* If we are incrementing the same register by two: */
  
  if (pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
    
    while (icount--) 
      emitpcode(POC_INCF, popGet(AOP(IC_RESULT(ic)),0));
    //pic14_emitcode("incf","%s,f",aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
    
    return TRUE ;
  }
  
  DEBUGpic14_emitcode ("; ","couldn't increment ");
  
  return FALSE ;
}

/*-----------------------------------------------------------------*/
/* pic14_outBitAcc - output a bit in acc                                 */
/*-----------------------------------------------------------------*/
void pic14_outBitAcc(operand *result)
{
  symbol *tlbl = newiTempLabel(NULL);
  /* if the result is a bit */
  DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  
  if (AOP_TYPE(result) == AOP_CRY){
    aopPut(AOP(result),"a",0);
  }
  else {
    pic14_emitcode("jz","%05d_DS_",tlbl->key+100);
    pic14_emitcode("mov","a,#01");
    pic14_emitcode("","%05d_DS_:",tlbl->key+100);
    pic14_outAcc(result);
  }
}

/*-----------------------------------------------------------------*/
/* genPlusBits - generates code for addition of two bits           */
/*-----------------------------------------------------------------*/
void genPlusBits (iCode *ic)
{
  
  DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  
  DEBUGpic14_emitcode ("; ","result %s, left %s, right %s",
           AopType(AOP_TYPE(IC_RESULT(ic))),
           AopType(AOP_TYPE(IC_LEFT(ic))),
           AopType(AOP_TYPE(IC_RIGHT(ic))));
  /*
    The following block of code will add two bits. 
    Note that it'll even work if the destination is
    the carry (C in the status register).
    It won't work if the 'Z' bit is a source or destination.
  */
  
  /* If the result is stored in the accumulator (w) */
  //if(strcmp(aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE),"a") == 0 ) {
  switch(AOP_TYPE(IC_RESULT(ic))) {
  case AOP_ACC:
    emitpcode(POC_CLRW, NULL);
    emitpcode(POC_BTFSC, popGet(AOP(IC_RIGHT(ic)),0));
    emitpcode(POC_XORLW, popGetLit(1));
    emitpcode(POC_BTFSC, popGet(AOP(IC_LEFT(ic)),0));
    emitpcode(POC_XORLW, popGetLit(1));
    
    pic14_emitcode("clrw","");
    pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
       AOP(IC_RIGHT(ic))->aopu.aop_dir,
       AOP(IC_RIGHT(ic))->aopu.aop_dir);
    pic14_emitcode("xorlw","1");
    pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
       AOP(IC_LEFT(ic))->aopu.aop_dir,
       AOP(IC_LEFT(ic))->aopu.aop_dir);
    pic14_emitcode("xorlw","1");
    break;
  case AOP_REG:
    emitpcode(POC_MOVLW, popGetLit(0));
    emitpcode(POC_BTFSC, popGet(AOP(IC_RIGHT(ic)),0));
    emitpcode(POC_XORLW, popGetLit(1));
    emitpcode(POC_BTFSC, popGet(AOP(IC_LEFT(ic)),0));
    emitpcode(POC_XORLW, popGetLit(1));
    emitpcode(POC_MOVWF, popGet(AOP(IC_RESULT(ic)),0));
    break;
    /*  case AOP_CRY:
    if(pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic)))) {

    } else {

    }
    break;*/
  default:
    if(pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic)))) {
      DebugAop(AOP(IC_LEFT(ic)));
      emitpcode(POC_MOVLW, popGet(AOP(IC_LEFT(ic)),0));
      emitpcode(POC_BTFSC, popGet(AOP(IC_RIGHT(ic)),0));
      emitpcode(POC_XORWF, popGet(AOP(IC_LEFT(ic)),0));
    } else {
      emitpcode(POC_MOVLW, popGet(AOP(IC_RESULT(ic)),0));
      emitpcode(POC_BCF,   popGet(AOP(IC_RESULT(ic)),0));
      emitpcode(POC_BTFSC, popGet(AOP(IC_RIGHT(ic)),0));
      emitpcode(POC_XORWF, popGet(AOP(IC_RESULT(ic)),0));
      emitpcode(POC_BTFSC, popGet(AOP(IC_LEFT(ic)),0));
      emitpcode(POC_XORWF, popGet(AOP(IC_RESULT(ic)),0));
    }


    pic14_emitcode("movlw","(1 << (%s & 7))",
       AOP(IC_RESULT(ic))->aopu.aop_dir,
       AOP(IC_RESULT(ic))->aopu.aop_dir);
    pic14_emitcode("bcf","(%s >> 3), (%s & 7)",
       AOP(IC_RESULT(ic))->aopu.aop_dir,
       AOP(IC_RESULT(ic))->aopu.aop_dir);
    pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
       AOP(IC_RIGHT(ic))->aopu.aop_dir,
       AOP(IC_RIGHT(ic))->aopu.aop_dir);
    pic14_emitcode("xorwf","(%s >>3),f",
       AOP(IC_RESULT(ic))->aopu.aop_dir);
    pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
       AOP(IC_LEFT(ic))->aopu.aop_dir,
       AOP(IC_LEFT(ic))->aopu.aop_dir);
    pic14_emitcode("xorwf","(%s>>3),f",
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
    !pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))))
    aopPut(AOP(IC_RESULT(ic)),
    aopGet(AOP(IC_LEFT(ic)),2,FALSE,FALSE),
    2);
  
  if (AOP_SIZE(IC_RESULT(ic)) == 3 && 
    AOP_SIZE(IC_RIGHT(ic)) == 3   &&
    !pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_RIGHT(ic))))
    aopPut(AOP(IC_RESULT(ic)),
    aopGet(AOP(IC_RIGHT(ic)),2,FALSE,FALSE),
    2);
  
  if (AOP_SIZE(IC_RESULT(ic)) == 3 &&
    AOP_SIZE(IC_LEFT(ic)) < 3    &&
    AOP_SIZE(IC_RIGHT(ic)) < 3   &&
    !pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))) &&
    !pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_RIGHT(ic)))) {
    char buffer[5];
    sprintf(buffer,"#%d",pointerCode(getSpec(operandType(IC_LEFT(ic)))));
    aopPut(AOP(IC_RESULT(ic)),buffer,2);
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
    !pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))))
  {
    aopPut(AOP(IC_RESULT(ic)),
      aopGet(AOP(IC_LEFT(ic)), GPTRSIZE - 1,FALSE,FALSE),
      GPTRSIZE - 1);
  }
  
  if (opIsGptr(IC_RESULT(ic)) &&
    opIsGptr(IC_RIGHT(ic))   &&
    !pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_RIGHT(ic))))
  {
    aopPut(AOP(IC_RESULT(ic)),
      aopGet(AOP(IC_RIGHT(ic)),GPTRSIZE - 1,FALSE,FALSE),
      GPTRSIZE - 1);
  }
  
  if (opIsGptr(IC_RESULT(ic))      &&
    AOP_SIZE(IC_LEFT(ic)) < GPTRSIZE   &&
    AOP_SIZE(IC_RIGHT(ic)) < GPTRSIZE  &&
    !pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))) &&
    !pic14_sameRegs(AOP(IC_RESULT(ic)),AOP(IC_RIGHT(ic)))) {
    char buffer[5];
    sprintf(buffer,"#%d",pointerCode(getSpec(operandType(IC_LEFT(ic)))));
    aopPut(AOP(IC_RESULT(ic)),buffer,GPTRSIZE - 1);
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
    emitpcode(POC_INCF, popGet(AOP(result),offr));
    break;
  case 0xff:
    emitpcode(POC_DECF, popGet(AOP(result),offr));
    break;
  default:
    emitpcode(POC_MOVLW,popGetLit(lit&0xff));
    emitpcode(POC_ADDWF,popGet(AOP(result),offr));
  }
  
}

static void emitMOVWF(operand *reg, int offset)
{
  if(!reg)
    return;
  
  if (AOP_TYPE(reg) == AOP_ACC) {
    DEBUGpic14_emitcode ("; ***","%s  %d ignoring mov into W",__FUNCTION__,__LINE__);
    return;
  }
  
  emitpcode(POC_MOVWF, popGet(AOP(reg),offset));
  
}

static void genAddLit (iCode *ic, int lit)
{
  
  int size,same;
  int lo;
  
  operand *result;
  operand *left;
  
  DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  
  
  left = IC_LEFT(ic);
  result = IC_RESULT(ic);
  same = pic14_sameRegs(AOP(left), AOP(result));
  size = pic14_getDataSize(result);
  
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
        DEBUGpic14_emitcode ("; hi = 0","%s  %d",__FUNCTION__,__LINE__);
        switch(lo) {
        case 0:
          break;
        case 1:
          emitpcode(POC_INCF, popGet(AOP(result),0));
          emitSKPNZ;
          emitpcode(POC_INCF, popGet(AOP(result),MSB16));
          break;
        case 0xff:
          emitpcode(POC_DECF, popGet(AOP(result),0));
          emitpcode(POC_INCFSZW, popGet(AOP(result),0));
          emitpcode(POC_INCF, popGet(AOP(result),MSB16));
          
          break;
        default:
          emitpcode(POC_MOVLW,popGetLit(lit&0xff));
          emitpcode(POC_ADDWF,popGet(AOP(result),0));
          emitSKPNC;
          emitpcode(POC_INCF, popGet(AOP(result),MSB16));
          
          
        }
        break;
        
        case 1:
          /* lit = 0x01LL */
          DEBUGpic14_emitcode ("; hi = 1","%s  %d",__FUNCTION__,__LINE__);
          switch(lo) {
          case 0:  /* 0x0100 */
            emitpcode(POC_INCF, popGet(AOP(result),MSB16));
            break;
          case 1:  /* 0x0101  */
            emitpcode(POC_INCF, popGet(AOP(result),MSB16));
            emitpcode(POC_INCF, popGet(AOP(result),0));
            emitSKPNZ;
            emitpcode(POC_INCF, popGet(AOP(result),MSB16));
            break;
          case 0xff: /* 0x01ff */
            emitpcode(POC_DECF, popGet(AOP(result),0));
            emitpcode(POC_INCFSZW, popGet(AOP(result),0));
            emitpcode(POC_INCF, popGet(AOP(result),MSB16));
            emitpcode(POC_INCF, popGet(AOP(result),MSB16));
          }   
          break;
          
          case 0xff:
            DEBUGpic14_emitcode ("; hi = ff","%s  %d",__FUNCTION__,__LINE__);
            /* lit = 0xffLL */
            switch(lo) {
            case 0:  /* 0xff00 */
              emitpcode(POC_DECF, popGet(AOP(result),MSB16));
              break;
            case 1:  /*0xff01 */
              emitpcode(POC_INCFSZ, popGet(AOP(result),0));
              emitpcode(POC_DECF, popGet(AOP(result),MSB16));
              break;
              /*  case 0xff: * 0xffff *
              emitpcode(POC_INCFSZW, popGet(AOP(result),0,FALSE,FALSE));
              emitpcode(POC_INCF, popGet(AOP(result),MSB16,FALSE,FALSE));
              emitpcode(POC_DECF, popGet(AOP(result),0,FALSE,FALSE));
              break;
              */
            default:
              emitpcode(POC_MOVLW,popGetLit(lo));
              emitpcode(POC_ADDWF,popGet(AOP(result),0));
              emitSKPC;
              emitpcode(POC_DECF, popGet(AOP(result),MSB16));
              
            }
            
            break;
            
            default:
              DEBUGpic14_emitcode ("; hi is generic","%d   %s  %d",hi,__FUNCTION__,__LINE__);
              
              /* lit = 0xHHLL */
              switch(lo) {
              case 0:  /* 0xHH00 */
                genAddLit2byte (result, MSB16, hi);
                break;
              case 1:  /* 0xHH01 */
                emitpcode(POC_MOVLW,popGetLit((hi+1)&0xff));
                emitpcode(POC_INCFSZ, popGet(AOP(result),0));
                emitpcode(POC_MOVLW,popGetLit(hi));
                emitpcode(POC_ADDWF,popGet(AOP(result),MSB16));
                break;
                /*  case 0xff: * 0xHHff *
                emitpcode(POC_MOVFW, popGet(AOP(result),0,FALSE,FALSE));
                emitpcode(POC_DECF, popGet(AOP(result),MSB16,FALSE,FALSE));
                emitpcode(POC_MOVLW,popGetLit(hi));
                emitpcode(POC_ADDWF,popGet(AOP(result),MSB16,FALSE,FALSE));
                break;
                */  default:  /* 0xHHLL */
                emitpcode(POC_MOVLW,popGetLit(lo));
                emitpcode(POC_ADDWF, popGet(AOP(result),0));
                emitpcode(POC_MOVLW,popGetLit(hi));
                emitSKPNC;
                emitpcode(POC_MOVLW,popGetLit((hi+1) & 0xff));
                emitpcode(POC_ADDWF,popGet(AOP(result),MSB16));
                break;
              }
              
    }
  } else {
    int carry_info = 0;
    int offset = 0;
    /* size > 2 */
    DEBUGpic14_emitcode (";  add lit to long","%s  %d",__FUNCTION__,__LINE__);
    
    while(size--) {
      lo = BYTEofLONG(lit,0);
      
      if(carry_info) {
        switch(lo) {
        case 0:
          switch(carry_info) {
          case 1:
            emitSKPNZ;
            emitpcode(POC_INCF, popGet(AOP(result),offset));
            break;
          case 2:
            emitpcode(POC_RLFW, popGet(AOP(result),offset));
            emitpcode(POC_ANDLW,popGetLit(1));
            emitpcode(POC_ADDWF, popGet(AOP(result),offset));
            break;
          default: /* carry_info = 3  */
            emitSKPNC;
            emitpcode(POC_INCF, popGet(AOP(result),offset));
            carry_info = 1;
            break;
          }
          break;
          case 0xff:
            emitpcode(POC_MOVLW,popGetLit(lo));
            if(carry_info==1) 
              emitSKPZ;
            else
              emitSKPC;
            emitpcode(POC_ADDWF, popGet(AOP(result),offset));
            break;
          default:
            emitpcode(POC_MOVLW,popGetLit(lo));
            if(carry_info==1) 
              emitSKPNZ;
            else
              emitSKPNC;
            emitpcode(POC_MOVLW,popGetLit(lo+1));
            emitpcode(POC_ADDWF, popGet(AOP(result),offset));
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
          emitpcode(POC_INCF, popGet(AOP(result),offset));
          carry_info=1;
          break;
        default:
          emitpcode(POC_MOVLW,popGetLit(lo));
          emitpcode(POC_ADDWF, popGet(AOP(result),offset));
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
          emitpcode(POC_INCF, popGet(AOP(result),0,FALSE,FALSE));
          emitSKPNZ;
        } else {
          emitpcode(POC_MOVLW,popGetLit(lo));
          emitpcode(POC_ADDWF, popGet(AOP(result),0,FALSE,FALSE));
          emitSKPNC;
        }
        emitpcode(POC_INCF, popGet(AOP(result),1,FALSE,FALSE));
        emitSKPNZ;
        emitpcode(POC_INCF, popGet(AOP(result),2,FALSE,FALSE));
        emitSKPNZ;
        emitpcode(POC_INCF, popGet(AOP(result),3,FALSE,FALSE));

        } 
      } 
    }
      
    */
  }
  } else {
    int offset = 1;
    DEBUGpic14_emitcode (";  left and result aren't same","%s  %d",__FUNCTION__,__LINE__);
    
    if(size == 1) {
      
      if(AOP_TYPE(left) == AOP_ACC) {
        /* left addend is already in accumulator */
        switch(lit & 0xff) {
        case 0:
          //emitpcode(POC_MOVWF, popGet(AOP(result),0,FALSE,FALSE));
          emitMOVWF(result,0);
          break;
        default:
          emitpcode(POC_ADDLW, popGetLit(lit & 0xff));
          //emitpcode(POC_MOVWF, popGet(AOP(result),0,FALSE,FALSE));
          emitMOVWF(result,0);
        }
      } else {
        /* left addend is in a register */
        switch(lit & 0xff) {
        case 0:
          emitpcode(POC_MOVFW, popGet(AOP(left),0));
          emitMOVWF(result, 0);
          //emitpcode(POC_MOVWF, popGet(AOP(result),0,FALSE,FALSE));
          emitMOVWF(result,0);
          break;
        case 1:
          emitpcode(POC_INCFW, popGet(AOP(left),0));
          //emitpcode(POC_MOVWF, popGet(AOP(result),0,FALSE,FALSE));
          emitMOVWF(result,0);
          break;
        case 0xff:
          emitpcode(POC_DECFW, popGet(AOP(left),0));
          //emitpcode(POC_MOVWF, popGet(AOP(result),0,FALSE,FALSE));
          emitMOVWF(result,0);
          break;
        default:
          emitpcode(POC_MOVLW, popGetLit(lit & 0xff));
          emitpcode(POC_ADDFW, popGet(AOP(left),0));
          //emitpcode(POC_MOVWF, popGet(AOP(result),0,FALSE,FALSE));
          emitMOVWF(result,0);
        }
      }
      
    } else {
      int clear_carry=0;
      
      /* left is not the accumulator */
      if(lit & 0xff) {
        emitpcode(POC_MOVLW, popGetLit(lit & 0xff));
        emitpcode(POC_ADDFW, popGet(AOP(left),0));
      } else {
        emitpcode(POC_MOVFW, popGet(AOP(left),0));
        /* We don't know the state of the carry bit at this point */
        clear_carry = 1;
      }
      //emitpcode(POC_MOVWF, popGet(AOP(result),0,FALSE,FALSE));
      emitMOVWF(result,0);
      while(--size) {
        
        lit >>= 8;
        if(lit & 0xff) {
          if(clear_carry) {
          /* The ls byte of the lit must've been zero - that 
            means we don't have to deal with carry */
            
            emitpcode(POC_MOVLW, popGetLit(lit & 0xff));
            emitpcode(POC_ADDFW,  popGet(AOP(left),offset));
            emitpcode(POC_MOVWF, popGet(AOP(left),offset));
            
            clear_carry = 0;
            
          } else {
            emitpcode(POC_MOVLW, popGetLit(lit & 0xff));
            //emitpcode(POC_MOVWF, popGet(AOP(result),offset,FALSE,FALSE));
            emitMOVWF(result,offset);
            emitpcode(POC_MOVFW, popGet(AOP(left),offset));
            emitSKPNC;
            emitpcode(POC_INCFSZW,popGet(AOP(left),offset));
            emitpcode(POC_ADDWF,  popGet(AOP(result),offset));
          }
          
        } else {
          emitpcode(POC_CLRF,  popGet(AOP(result),offset));
          emitpcode(POC_RLF,   popGet(AOP(result),offset));
          emitpcode(POC_MOVFW, popGet(AOP(left),offset));
          emitpcode(POC_ADDWF, popGet(AOP(result),offset));
        }
        offset++;
      }
    }
  }
}

/*-----------------------------------------------------------------*/
/* genPlus - generates code for addition                           */
/*-----------------------------------------------------------------*/
void genPlus (iCode *ic)
{
  int size, offset = 0;
  
  /* special cases :- */
  DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  
  aopOp (IC_LEFT(ic),ic,FALSE);
  aopOp (IC_RIGHT(ic),ic,FALSE);
  aopOp (IC_RESULT(ic),ic,TRUE);
  
  DEBUGpic14_AopType(__LINE__,IC_LEFT(ic),IC_RIGHT(ic),IC_RESULT(ic));
  
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
    genPlusBits (ic);
    goto release ;
  }
  
  /* if left in bit space & right literal */
  if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY &&
      AOP_TYPE(IC_RIGHT(ic)) == AOP_LIT) {
    /* if result in bit space */
    if(AOP_TYPE(IC_RESULT(ic)) == AOP_CRY){
      if((unsigned long)floatFromVal(AOP(IC_RIGHT(ic))->aopu.aop_lit) != 0L) {
  emitpcode(POC_MOVLW, popGet(AOP(IC_RESULT(ic)),0));
  if (!pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) )
    emitpcode(POC_BTFSC, popGet(AOP(IC_LEFT(ic)),0));
  emitpcode(POC_XORWF, popGet(AOP(IC_RESULT(ic)),0));
      }
    } else {
      size = pic14_getDataSize(IC_RESULT(ic));
      while (size--) {
  MOVA(aopGet(AOP(IC_RIGHT(ic)),offset,FALSE,FALSE));  
  pic14_emitcode("addc","a,#00  ;%d",__LINE__);
  aopPut(AOP(IC_RESULT(ic)),"a",offset++);
      }
    }
    goto release ;
  }
  
  /* if I can do an increment instead
     of add then GOOD for ME */
  if (genPlusIncr (ic) == TRUE)
    goto release;   
  
  size = pic14_getDataSize(IC_RESULT(ic));
  
  if(AOP(IC_RIGHT(ic))->type == AOP_LIT) {
    /* Add a literal to something else */
    //bool know_W=0;
    unsigned lit = (unsigned) floatFromVal(AOP(IC_RIGHT(ic))->aopu.aop_lit);
    //      unsigned l1=0;
    
    //      offset = 0;
    DEBUGpic14_emitcode(";","adding lit to something. size %d",size);
    
    genAddLit (ic,  lit);
    goto release;
    
  } else if(AOP_TYPE(IC_RIGHT(ic)) == AOP_CRY) {
    
    pic14_emitcode(";bitadd","right is bit: %s",aopGet(AOP(IC_RIGHT(ic)),0,FALSE,FALSE));
    pic14_emitcode(";bitadd","left is bit: %s",aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
    pic14_emitcode(";bitadd","result is bit: %s",aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
    
    /* here we are adding a bit to a char or int */
    if(size == 1) {
      if (pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
        
  emitpcode(POC_BTFSC , popGet(AOP(IC_RIGHT(ic)),0));
  emitpcode(POC_INCF ,  popGet(AOP(IC_RESULT(ic)),0));
        
  pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
           AOP(IC_RIGHT(ic))->aopu.aop_dir,
           AOP(IC_RIGHT(ic))->aopu.aop_dir);
  pic14_emitcode(" incf","%s,f", aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
      } else {
        
  if(AOP_TYPE(IC_LEFT(ic)) == AOP_ACC) {
    emitpcode(POC_BTFSC , popGet(AOP(IC_RIGHT(ic)),0));
    emitpcode(POC_XORLW , popGetLit(1));
          
    pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
       AOP(IC_RIGHT(ic))->aopu.aop_dir,
       AOP(IC_RIGHT(ic))->aopu.aop_dir);
    pic14_emitcode(" xorlw","1");
  } else {
    emitpcode(POC_MOVFW , popGet(AOP(IC_LEFT(ic)),0));
    emitpcode(POC_BTFSC , popGet(AOP(IC_RIGHT(ic)),0));
    emitpcode(POC_INCFW , popGet(AOP(IC_LEFT(ic)),0));
          
    pic14_emitcode("movf","%s,w", aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
    pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
       AOP(IC_RIGHT(ic))->aopu.aop_dir,
       AOP(IC_RIGHT(ic))->aopu.aop_dir);
    pic14_emitcode(" incf","%s,w", aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
  }
        
  if(AOP_TYPE(IC_RESULT(ic)) != AOP_ACC) {
          
    if(AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) {
      emitpcode(POC_ANDLW , popGetLit(1));
      emitpcode(POC_BCF ,   popGet(AOP(IC_RESULT(ic)),0));
      emitSKPZ;
      emitpcode(POC_BSF ,   popGet(AOP(IC_RESULT(ic)),0));
    } else {
      emitpcode(POC_MOVWF ,   popGet(AOP(IC_RESULT(ic)),0));
      pic14_emitcode("movwf","%s", aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
    }
  }
      }
      
    } else {
      int offset = 1;
      DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
      if (pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
  emitCLRZ;
  emitpcode(POC_BTFSC, popGet(AOP(IC_RIGHT(ic)),0));
  emitpcode(POC_INCF,  popGet(AOP(IC_RESULT(ic)),0));
        
  pic14_emitcode("clrz","");
        
  pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
           AOP(IC_RIGHT(ic))->aopu.aop_dir,
           AOP(IC_RIGHT(ic))->aopu.aop_dir);
  pic14_emitcode(" incf","%s,f", aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
        
      } else {
        
  emitpcode(POC_MOVFW, popGet(AOP(IC_LEFT(ic)),0));
  emitpcode(POC_BTFSC, popGet(AOP(IC_RIGHT(ic)),0));
  emitpcode(POC_INCFW, popGet(AOP(IC_LEFT(ic)),0));
  //emitpcode(POC_MOVWF, popGet(AOP(IC_RIGHT(ic)),0,FALSE,FALSE));
  emitMOVWF(IC_RIGHT(ic),0);
        
  pic14_emitcode("movf","%s,w", aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
  pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
           AOP(IC_RIGHT(ic))->aopu.aop_dir,
           AOP(IC_RIGHT(ic))->aopu.aop_dir);
  pic14_emitcode(" incf","%s,w", aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
  pic14_emitcode("movwf","%s", aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
        
      }
      
      while(--size){
  emitSKPZ;
  emitpcode(POC_INCF,  popGet(AOP(IC_RESULT(ic)),offset++));
  //pic14_emitcode(" incf","%s,f", aopGet(AOP(IC_RIGHT(ic)),offset++,FALSE,FALSE));
      }
      
    }
    
  } else {
    DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);    
    
    /* Add the first bytes */
    
    if(strcmp(aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE),"a") == 0 ) {
      emitpcode(POC_ADDFW, popGet(AOP(IC_RIGHT(ic)),0));
      emitpcode(POC_MOVWF,popGet(AOP(IC_RESULT(ic)),0));
    } else {
      
      if ( AOP_TYPE(IC_LEFT(ic)) == AOP_ACC) {
  emitpcode(POC_ADDFW, popGet(AOP(IC_RIGHT(ic)),0));
  if ( AOP_TYPE(IC_RESULT(ic)) != AOP_ACC)
    emitpcode(POC_MOVWF,popGet(AOP(IC_RESULT(ic)),0));
      } else {
        
  emitpcode(POC_MOVFW,popGet(AOP(IC_RIGHT(ic)),0));
        
  if (pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) )
    emitpcode(POC_ADDWF, popGet(AOP(IC_LEFT(ic)),0));
  else {
    PIC_OPCODE poc = POC_ADDFW;
          
    if ((AOP_TYPE(IC_LEFT(ic)) == AOP_PCODE) && (
                   (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_LITERAL) || 
                   (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_IMMEDIATE)))
      poc = POC_ADDLW;
    emitpcode(poc, popGet(AOP(IC_LEFT(ic)),0));
    if ( AOP_TYPE(IC_RESULT(ic)) != AOP_ACC)
      emitpcode(POC_MOVWF,popGet(AOP(IC_RESULT(ic)),0));
  }
      }
    }
    
    size = min( AOP_SIZE(IC_RESULT(ic)), AOP_SIZE(IC_RIGHT(ic))) - 1;
    offset = 1;
    
    
    if(size){
      if (pic14_sameRegs(AOP(IC_RIGHT(ic)), AOP(IC_RESULT(ic)))) {
  if ((AOP_TYPE(IC_LEFT(ic)) == AOP_PCODE) && (
                 (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_LITERAL) || 
                 (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_IMMEDIATE))) {
    while(size--){
      emitpcode(POC_MOVFW,   popGet(AOP(IC_RIGHT(ic)),offset));
      emitSKPNC;
      emitpcode(POC_INCFSZW, popGet(AOP(IC_RIGHT(ic)),offset));
      emitpcode(POC_ADDLW,   popGet(AOP(IC_LEFT(ic)),offset));
      emitpcode(POC_MOVWF,   popGet(AOP(IC_RESULT(ic)),offset));
      offset++;
    }
  } else {
    while(size--){
      emitpcode(POC_MOVFW,   popGet(AOP(IC_LEFT(ic)),offset));
      emitSKPNC;
      emitpcode(POC_INCFSZW, popGet(AOP(IC_LEFT(ic)),offset));
      emitpcode(POC_ADDWF,   popGet(AOP(IC_RESULT(ic)),offset));
      offset++;
    }
  }
      } else {
  PIC_OPCODE poc = POC_MOVFW;
  if ((AOP_TYPE(IC_LEFT(ic)) == AOP_PCODE) && (
                 (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_LITERAL) || 
                 (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_IMMEDIATE)))
    poc = POC_MOVLW;
  while(size--){
    if (!pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
      emitpcode(poc, popGet(AOP(IC_LEFT(ic)),offset));
      emitpcode(POC_MOVWF, popGet(AOP(IC_RESULT(ic)),offset));
    }
    emitpcode(POC_MOVFW,   popGet(AOP(IC_RIGHT(ic)),offset));
    emitSKPNC;
    emitpcode(POC_INCFSZW, popGet(AOP(IC_RIGHT(ic)),offset));
    emitpcode(POC_ADDWF,   popGet(AOP(IC_RESULT(ic)),offset));
    offset++;
  }
      }
    }
  }
  
  if (AOP_SIZE(IC_RESULT(ic)) > AOP_SIZE(IC_RIGHT(ic))) {
    int sign =  !(SPEC_USIGN(getSpec(operandType(IC_LEFT(ic)))) |
      SPEC_USIGN(getSpec(operandType(IC_RIGHT(ic)))) );
    
    
    /* Need to extend result to higher bytes */
    size = AOP_SIZE(IC_RESULT(ic)) - AOP_SIZE(IC_RIGHT(ic)) - 1;
    
    /* First grab the carry from the lower bytes */
    if (AOP_SIZE(IC_LEFT(ic)) > AOP_SIZE(IC_RIGHT(ic))) { 
      int leftsize = AOP_SIZE(IC_LEFT(ic)) - AOP_SIZE(IC_RIGHT(ic));
      PIC_OPCODE poc = POC_MOVFW;
      if ((AOP_TYPE(IC_LEFT(ic)) == AOP_PCODE) && (
               (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_LITERAL) || 
               (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_IMMEDIATE)))
  poc = POC_MOVLW;
      while(leftsize-- > 0) {
  emitpcode(poc, popGet(AOP(IC_LEFT(ic)),offset));
  emitpcode(POC_MOVWF, popGet(AOP(IC_RESULT(ic)),offset));
  emitSKPNC;
  emitpcode(POC_INCF, popGet(AOP(IC_RESULT(ic)),offset));
  offset++;
  if (size)
    size--;
  else
    break;
      }
    } else {
      emitpcode(POC_CLRF, popGet(AOP(IC_RESULT(ic)),offset));
      emitpcode(POC_RLF,  popGet(AOP(IC_RESULT(ic)),offset));
    }
    
    
    if(sign) {
      /* Now this is really horrid. Gotta check the sign of the addends and propogate
       * to the result */
      
      emitpcode(POC_BTFSC, newpCodeOpBit(aopGet(AOP(IC_LEFT(ic)),offset-1,FALSE,FALSE),7,0));
      emitpcode(POC_DECF,  popGet(AOP(IC_RESULT(ic)),offset));
      emitpcode(POC_BTFSC, newpCodeOpBit(aopGet(AOP(IC_RIGHT(ic)),offset-1,FALSE,FALSE),7,0));
      emitpcode(POC_DECF,  popGet(AOP(IC_RESULT(ic)),offset));
      
      /* if chars or ints or being signed extended to longs: */
      if(size) {
  emitpcode(POC_MOVLW, popGetLit(0));
  emitpcode(POC_BTFSC, newpCodeOpBit(aopGet(AOP(IC_RESULT(ic)),offset,FALSE,FALSE),7,0));
  emitpcode(POC_MOVLW, popGetLit(0xff));
      }
    }
    
    offset++;
    while(size--) {
      
      if(sign)
  emitpcode(POC_MOVWF, popGet(AOP(IC_RESULT(ic)),offset));
      else
  emitpcode(POC_CLRF,  popGet(AOP(IC_RESULT(ic)),offset));
      
      offset++;
    }
  }
  
  
  //adjustArithmeticResult(ic);
  
 release:
  freeAsmop(IC_LEFT(ic),NULL,ic,(RESULTONSTACK(ic) ? FALSE : TRUE));
  freeAsmop(IC_RIGHT(ic),NULL,ic,(RESULTONSTACK(ic) ? FALSE : TRUE));
  freeAsmop(IC_RESULT(ic),NULL,ic,TRUE);
}

/*-----------------------------------------------------------------*/
/* genMinusDec :- does subtraction with decrement if possible     */
/*-----------------------------------------------------------------*/
bool genMinusDec (iCode *ic)
{
  unsigned int icount ;
  unsigned int size = pic14_getDataSize(IC_RESULT(ic));
  
  DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  /* will try to generate an increment */
  /* if the right side is not a literal 
  we cannot */
  if ((AOP_TYPE(IC_RIGHT(ic)) != AOP_LIT) || 
    (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY) || 
    (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) )
    return FALSE ;
  
  DEBUGpic14_emitcode ("; lit val","%d",(unsigned int) floatFromVal (AOP(IC_RIGHT(ic))->aopu.aop_lit));
  
  /* if the literal value of the right hand side
  is greater than 4 then it is not worth it */
  if ((icount = (unsigned int) floatFromVal (AOP(IC_RIGHT(ic))->aopu.aop_lit)) > 2)
    return FALSE ;
  
  /* if decrement 16 bits in register */
  if (pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) &&
    (size > 1) &&
    (icount == 1)) {
    
    if(size == 2) { 
      emitpcode(POC_DECF,    popGet(AOP(IC_RESULT(ic)),LSB));
      emitpcode(POC_INCFSZW, popGet(AOP(IC_RESULT(ic)),LSB));
      emitpcode(POC_INCF,    popGet(AOP(IC_RESULT(ic)),MSB16));
      emitpcode(POC_DECF,    popGet(AOP(IC_RESULT(ic)),MSB16));
      
      pic14_emitcode("decf","%s,f",aopGet(AOP(IC_RESULT(ic)),LSB,FALSE,FALSE));
      pic14_emitcode("incfsz","%s,w",aopGet(AOP(IC_RESULT(ic)),LSB,FALSE,FALSE));
      pic14_emitcode(" decf","%s,f",aopGet(AOP(IC_RESULT(ic)),MSB16,FALSE,FALSE));
    } else {
      /* size is 3 or 4 */
      emitpcode(POC_MOVLW,  popGetLit(0xff));
      emitpcode(POC_ADDWF,  popGet(AOP(IC_RESULT(ic)),LSB));
      emitSKPNC;
      emitpcode(POC_ADDWF,  popGet(AOP(IC_RESULT(ic)),MSB16));
      emitSKPNC;
      emitpcode(POC_ADDWF,  popGet(AOP(IC_RESULT(ic)),MSB24));
      
      pic14_emitcode("movlw","0xff");
      pic14_emitcode("addwf","%s,f",aopGet(AOP(IC_RESULT(ic)),LSB,FALSE,FALSE));
      
      emitSKPNC;
      pic14_emitcode("addwf","%s,f",aopGet(AOP(IC_RESULT(ic)),MSB16,FALSE,FALSE));
      emitSKPNC;
      pic14_emitcode("addwf","%s,f",aopGet(AOP(IC_RESULT(ic)),MSB24,FALSE,FALSE));
      
      if(size > 3) {
        emitSKPNC;
        emitpcode(POC_ADDWF,  popGet(AOP(IC_RESULT(ic)),MSB32));
        
        pic14_emitcode("skpnc","");
        emitSKPNC;
        pic14_emitcode("addwf","%s,f",aopGet(AOP(IC_RESULT(ic)),MSB32,FALSE,FALSE));
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
  if (pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic)))) {
    
    while (icount--) 
      emitpcode(POC_DECF, popGet(AOP(IC_RESULT(ic)),0));
    
    //pic14_emitcode ("decf","%s,f",aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
    
    return TRUE ;
  }
  
  DEBUGpic14_emitcode ("; returning"," result=%s, left=%s",
    aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE),
    aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
  if(size==1) {
    
    pic14_emitcode("decf","%s,w",aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
    pic14_emitcode("movwf","%s",aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
    
    emitpcode(POC_DECFW,  popGet(AOP(IC_LEFT(ic)),0));
    emitpcode(POC_MOVWF,  popGet(AOP(IC_RESULT(ic)),0));
    
    return TRUE;
  }
  
  return FALSE ;
}

/*-----------------------------------------------------------------*/
/* addSign - propogate sign bit to higher bytes                    */
/*-----------------------------------------------------------------*/
void addSign(operand *result, int offset, int sign)
{
  int size = (pic14_getDataSize(result) - offset);
  DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  
  if(size > 0){
    if(sign && offset) {
      
      if(size == 1) {
        emitpcode(POC_CLRF,popGet(AOP(result),offset));
        emitpcode(POC_BTFSC,newpCodeOpBit(aopGet(AOP(result),offset-1,FALSE,FALSE),7,0));
        emitpcode(POC_DECF, popGet(AOP(result),offset));
      } else {
        
        emitpcode(POC_MOVLW, popGetLit(0));
        emitpcode(POC_BTFSC, newpCodeOpBit(aopGet(AOP(result),offset-1,FALSE,FALSE),7,0));
        emitpcode(POC_MOVLW, popGetLit(0xff));
        while(size--)
          emitpcode(POC_MOVWF, popGet(AOP(result),size));
        
      }
    } else
      while(size--)
        emitpcode(POC_CLRF,popGet(AOP(result),offset++));
  }
}

/*-----------------------------------------------------------------*/
/* genMinusBits - generates code for subtraction  of two bits      */
/*-----------------------------------------------------------------*/
void genMinusBits (iCode *ic)
{
  symbol *lbl = newiTempLabel(NULL);
  DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  if (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY){
    pic14_emitcode("mov","c,%s",AOP(IC_LEFT(ic))->aopu.aop_dir);
    pic14_emitcode("jnb","%s,%05d_DS_",AOP(IC_RIGHT(ic))->aopu.aop_dir,(lbl->key+100));
    pic14_emitcode("cpl","c");
    pic14_emitcode("","%05d_DS_:",(lbl->key+100));
    pic14_outBitC(IC_RESULT(ic));
  }
  else{
    pic14_emitcode("mov","c,%s",AOP(IC_RIGHT(ic))->aopu.aop_dir);
    pic14_emitcode("subb","a,acc");
    pic14_emitcode("jnb","%s,%05d_DS_",AOP(IC_LEFT(ic))->aopu.aop_dir,(lbl->key+100));
    pic14_emitcode("inc","a");
    pic14_emitcode("","%05d_DS_:",(lbl->key+100));
    aopPut(AOP(IC_RESULT(ic)),"a",0);
    addSign(IC_RESULT(ic), MSB16, SPEC_USIGN(getSpec(operandType(IC_RESULT(ic)))));
  }
}

/*-----------------------------------------------------------------*/
/* genMinus - generates code for subtraction                       */
/*-----------------------------------------------------------------*/
void genMinus (iCode *ic)
{
  int size, offset = 0, same=0;
  unsigned long lit = 0L;
  
  DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
  aopOp (IC_LEFT(ic),ic,FALSE);
  aopOp (IC_RIGHT(ic),ic,FALSE);
  aopOp (IC_RESULT(ic),ic,TRUE);
  
  if (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY  &&
    AOP_TYPE(IC_RIGHT(ic)) == AOP_LIT) {
    operand *t = IC_RIGHT(ic);
    IC_RIGHT(ic) = IC_LEFT(ic);
    IC_LEFT(ic) = t;
  }
  
  DEBUGpic14_emitcode ("; ","result %s, left %s, right %s",
    AopType(AOP_TYPE(IC_RESULT(ic))),
    AopType(AOP_TYPE(IC_LEFT(ic))),
    AopType(AOP_TYPE(IC_RIGHT(ic))));
  
  /* special cases :- */
  /* if both left & right are in bit space */
  if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY &&
    AOP_TYPE(IC_RIGHT(ic)) == AOP_CRY) {
    genPlusBits (ic);
    goto release ;
  }
  
  /* if I can do an decrement instead
  of subtract then GOOD for ME */
  //  if (genMinusDec (ic) == TRUE)
  //    goto release;   
  
  size = pic14_getDataSize(IC_RESULT(ic));   
  same = pic14_sameRegs(AOP(IC_RIGHT(ic)), AOP(IC_RESULT(ic)));
  
  if(AOP(IC_RIGHT(ic))->type == AOP_LIT) {
    /* Add a literal to something else */
    
    lit = (unsigned long)floatFromVal(AOP(IC_RIGHT(ic))->aopu.aop_lit);
    lit = - (long)lit;
    
    genAddLit ( ic,  lit);
    
#if 0
    /* add the first byte: */
    pic14_emitcode("movlw","0x%x", lit & 0xff);
    pic14_emitcode("addwf","%s,f", aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
    emitpcode(POC_MOVLW,  popGetLit(lit & 0xff));
    emitpcode(POC_ADDWF,  popGet(AOP(IC_LEFT(ic)),0));
    
    
    offset = 1;
    size--;
    
    while(size-- > 0) {
      
      lit >>= 8;
      
      if(lit & 0xff) {
        
        if((lit & 0xff) == 0xff) {
          emitpcode(POC_MOVLW,  popGetLit(0xff));
          emitSKPC;
          emitpcode(POC_ADDWF,  popGet(AOP(IC_LEFT(ic)),offset));
        } else {
          emitpcode(POC_MOVLW,  popGetLit(lit & 0xff));
          emitSKPNC;
          emitpcode(POC_MOVLW,  popGetLit((lit+1) & 0xff));
          emitpcode(POC_ADDWF,  popGet(AOP(IC_LEFT(ic)),offset));
        }
        
      } else {
        /* do the rlf known zero trick here */
        emitpcode(POC_MOVLW,  popGetLit(1));
        emitSKPNC;
        emitpcode(POC_ADDWF,  popGet(AOP(IC_LEFT(ic)),offset));
      }
      offset++;
    }
#endif
  } else if(AOP_TYPE(IC_RIGHT(ic)) == AOP_CRY) {
    // bit subtraction
    
    pic14_emitcode(";bitsub","right is bit: %s",aopGet(AOP(IC_RIGHT(ic)),0,FALSE,FALSE));
    pic14_emitcode(";bitsub","left is bit: %s",aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE));
    pic14_emitcode(";bitsub","result is bit: %s",aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
    
    /* here we are subtracting a bit from a char or int */
    if(size == 1) {
      if (pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
        
        emitpcode(POC_BTFSC , popGet(AOP(IC_RIGHT(ic)),0));
        emitpcode(POC_DECF ,  popGet(AOP(IC_RESULT(ic)),0));
        
        pic14_emitcode("btfsc","(%s >> 3), (%s & 7)",
          AOP(IC_RIGHT(ic))->aopu.aop_dir,
          AOP(IC_RIGHT(ic))->aopu.aop_dir);
        pic14_emitcode(" incf","%s,f", aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
      } else {
        
        if(AOP_TYPE(IC_LEFT(ic)) == AOP_ACC) {
          emitpcode(POC_BTFSC , popGet(AOP(IC_RIGHT(ic)),0));
          emitpcode(POC_XORLW , popGetLit(1));
        }else  if( (AOP_TYPE(IC_LEFT(ic)) == AOP_IMMD) ||
          (AOP_TYPE(IC_LEFT(ic)) == AOP_LIT) ) {
          
          lit = (unsigned long)floatFromVal(AOP(IC_LEFT(ic))->aopu.aop_lit);
          
          if(AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) {
            if (pic14_sameRegs(AOP(IC_RIGHT(ic)), AOP(IC_RESULT(ic))) ) {
              if(lit & 1) {
                emitpcode(POC_MOVLW , popGet(AOP(IC_RIGHT(ic)),0));
                emitpcode(POC_XORWF , popGet(AOP(IC_RIGHT(ic)),0));
              }
            }else{
              emitpcode(POC_BCF ,     popGet(AOP(IC_RESULT(ic)),0));
              if(lit & 1) 
                emitpcode(POC_BTFSS , popGet(AOP(IC_RIGHT(ic)),0));
              else
                emitpcode(POC_BTFSC , popGet(AOP(IC_RIGHT(ic)),0));
              emitpcode(POC_BSF ,     popGet(AOP(IC_RESULT(ic)),0));
            }
            goto release;
          } else {
            emitpcode(POC_MOVLW , popGetLit(lit & 0xff));
            emitpcode(POC_BTFSC , popGet(AOP(IC_RIGHT(ic)),0));
            emitpcode(POC_MOVLW , popGetLit((lit-1) & 0xff));
            emitpcode(POC_MOVWF , popGet(AOP(IC_RESULT(ic)),0));
            
          }
          
        } else {
          emitpcode(POC_MOVFW , popGet(AOP(IC_LEFT(ic)),0));
          emitpcode(POC_BTFSC , popGet(AOP(IC_RIGHT(ic)),0));
          emitpcode(POC_DECFW , popGet(AOP(IC_LEFT(ic)),0));
        }
        
        if(AOP_TYPE(IC_RESULT(ic)) != AOP_ACC) {
          
          emitpcode(POC_MOVWF ,   popGet(AOP(IC_RESULT(ic)),0));
          
        } else  {
          emitpcode(POC_ANDLW , popGetLit(1));
          /*
          emitpcode(POC_BCF ,   popGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
          emitSKPZ;
          emitpcode(POC_BSF ,   popGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE));
          */
        }
        
      }
      
    }
  } else   if(// (AOP_TYPE(IC_LEFT(ic)) == AOP_IMMD) || 
    (AOP(IC_LEFT(ic))->type == AOP_LIT) &&
    (AOP_TYPE(IC_RIGHT(ic)) != AOP_ACC)) {
    
    lit = (unsigned long)floatFromVal(AOP(IC_LEFT(ic))->aopu.aop_lit);
    DEBUGpic14_emitcode ("; left is lit","line %d result %s, left %s, right %s",__LINE__,
      AopType(AOP_TYPE(IC_RESULT(ic))),
      AopType(AOP_TYPE(IC_LEFT(ic))),
      AopType(AOP_TYPE(IC_RIGHT(ic))));
    
    
    if( (size == 1) && ((lit & 0xff) == 0) ) {
      /* res = 0 - right */
      if (pic14_sameRegs(AOP(IC_RIGHT(ic)), AOP(IC_RESULT(ic))) ) {
        emitpcode(POC_COMF,  popGet(AOP(IC_RIGHT(ic)),0));
        emitpcode(POC_INCF,  popGet(AOP(IC_RIGHT(ic)),0));
      } else { 
        emitpcode(POC_COMFW,  popGet(AOP(IC_RIGHT(ic)),0));
        emitpcode(POC_MOVWF,  popGet(AOP(IC_RESULT(ic)),0));
        emitpcode(POC_INCF,   popGet(AOP(IC_RESULT(ic)),0));
      }
      goto release;
    }
    
    emitpcode(POC_MOVFW,  popGet(AOP(IC_RIGHT(ic)),0));
    emitpcode(POC_SUBLW, popGetLit(lit & 0xff));    
    emitpcode(POC_MOVWF,popGet(AOP(IC_RESULT(ic)),0));
    
    
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
            emitpcode(POC_COMF,  popGet(AOP(IC_RESULT(ic)),offset));
            emitSKPC;
            emitpcode(POC_DECF,  popGet(AOP(IC_RESULT(ic)),offset));
          } else {
            emitpcode(POC_COMFW, popGet(AOP(IC_RIGHT(ic)),offset));
            emitpcode(POC_MOVWF, popGet(AOP(IC_RESULT(ic)),offset));
            emitSKPC;
            emitpcode(POC_DECF,  popGet(AOP(IC_RESULT(ic)),offset));
          }
        } else {
          emitpcode(POC_MOVFW, popGet(AOP(IC_RIGHT(ic)),offset));
          emitSKPC;
          emitpcode(POC_INCFW, popGet(AOP(IC_RIGHT(ic)),offset));
          emitpcode(POC_SUBLW, popGetLit(lit & 0xff));
          emitpcode(POC_MOVWF, popGet(AOP(IC_RESULT(ic)),offset));
        }
        
        goto release;
      }
      
      if(same) {
        
        if(lit & 0xff) {
          emitpcode(POC_MOVLW, popGetLit(lit & 0xff));
          emitSKPC;
          emitpcode(POC_MOVLW, popGetLit((lit & 0xff)-1));
          emitpcode(POC_SUBWF,  popGet(AOP(IC_RESULT(ic)),offset));
        } else {
          emitSKPNC;
          emitpcode(POC_SUBWF,  popGet(AOP(IC_RESULT(ic)),offset));
          
        }
      } else {
        
        if(lit & 0xff) {
          emitpcode(POC_MOVLW, popGetLit(lit & 0xff));
          emitpcode(POC_MOVWF, popGet(AOP(IC_RESULT(ic)),offset));
        } else
          emitpcode(POC_CLRF, popGet(AOP(IC_RESULT(ic)),offset));
        
        emitpcode(POC_MOVFW,  popGet(AOP(IC_RIGHT(ic)),offset));
        emitSKPC;
        emitpcode(POC_INCFSZW,popGet(AOP(IC_RIGHT(ic)),offset));
        emitpcode(POC_SUBWF,  popGet(AOP(IC_RESULT(ic)),offset));
      }
    }
    
    
  } else {
    
    DEBUGpic14_emitcode ("; ","line %d result %s, left %s, right %s",__LINE__,
      AopType(AOP_TYPE(IC_RESULT(ic))),
      AopType(AOP_TYPE(IC_LEFT(ic))),
      AopType(AOP_TYPE(IC_RIGHT(ic))));
    
    if(strcmp(aopGet(AOP(IC_LEFT(ic)),0,FALSE,FALSE),"a") == 0 ) {
      DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
      emitpcode(POC_SUBFW, popGet(AOP(IC_RIGHT(ic)),0));
      emitpcode(POC_SUBLW, popGetLit(0));
      emitpcode(POC_MOVWF,popGet(AOP(IC_RESULT(ic)),0));
    } else {
      
      if ( AOP_TYPE(IC_LEFT(ic)) == AOP_ACC) {
        emitpcode(POC_SUBFW, popGet(AOP(IC_RIGHT(ic)),0));
        emitpcode(POC_SUBLW, popGetLit(0));
        if ( AOP_TYPE(IC_RESULT(ic)) != AOP_ACC)
          emitpcode(POC_MOVWF,popGet(AOP(IC_RESULT(ic)),0));
      } else {
        
        DEBUGpic14_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
        if(AOP_TYPE(IC_RIGHT(ic)) != AOP_ACC) 
          emitpcode(POC_MOVFW,popGet(AOP(IC_RIGHT(ic)),0));
        
        if (pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) )
          emitpcode(POC_SUBWF, popGet(AOP(IC_LEFT(ic)),0));
        else {
          if( (AOP_TYPE(IC_LEFT(ic)) == AOP_IMMD) ||
            (AOP_TYPE(IC_LEFT(ic)) == AOP_LIT) ) {
            emitpcode(POC_SUBLW, popGet(AOP(IC_LEFT(ic)),0));
          } else {
            emitpcode(POC_SUBFW, popGet(AOP(IC_LEFT(ic)),0));
          }
          if ( AOP_TYPE(IC_RESULT(ic)) != AOP_ACC) {
            if ( AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) {
              emitpcode(POC_BCF ,   popGet(AOP(IC_RESULT(ic)),0));
              emitSKPZ;
              emitpcode(POC_BSF ,   popGet(AOP(IC_RESULT(ic)),0));
            }else
              emitpcode(POC_MOVWF,popGet(AOP(IC_RESULT(ic)),0));
          }
        }
      }
    }
    
    size = min( AOP_SIZE(IC_RESULT(ic)), AOP_SIZE(IC_RIGHT(ic))) - 1;
    offset = 1;
    
    if(size){
      if (pic14_sameRegs(AOP(IC_RIGHT(ic)), AOP(IC_RESULT(ic)))) {
        if ((AOP_TYPE(IC_LEFT(ic)) == AOP_PCODE) && (
          (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_LITERAL) || 
          (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_IMMEDIATE))) {
          while(size--){
            emitpcode(POC_MOVFW,   popGet(AOP(IC_RIGHT(ic)),offset));
            emitSKPC;
            emitpcode(POC_INCFSZW, popGet(AOP(IC_RIGHT(ic)),offset));
            emitpcode(POC_SUBLW,   popGet(AOP(IC_LEFT(ic)),offset));
            emitpcode(POC_MOVWF,   popGet(AOP(IC_RESULT(ic)),offset));
            offset++;
          }
        } else {
          while(size--){
            emitpcode(POC_MOVFW,   popGet(AOP(IC_RIGHT(ic)),offset));
            emitSKPC;
            emitpcode(POC_INCFSZW, popGet(AOP(IC_RIGHT(ic)),offset));
            emitpcode(POC_SUBFW,   popGet(AOP(IC_LEFT(ic)),offset));
            emitpcode(POC_MOVWF,   popGet(AOP(IC_RESULT(ic)),offset));
            offset++;
          }
        }
      } else {
        PIC_OPCODE poc = POC_MOVFW;
        if ((AOP_TYPE(IC_LEFT(ic)) == AOP_PCODE) && (
          (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_LITERAL) || 
          (AOP(IC_LEFT(ic))->aopu.pcop->type == PO_IMMEDIATE)))
          poc = POC_MOVLW;
        while(size--){
          if (!pic14_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
            emitpcode(POC_MOVFW,  popGet(AOP(IC_LEFT(ic)),offset));
            emitpcode(POC_MOVWF,  popGet(AOP(IC_RESULT(ic)),offset));
          }
          emitpcode(POC_MOVFW,  popGet(AOP(IC_RIGHT(ic)),offset));
          emitSKPC;
          emitpcode(POC_INCFSZW,popGet(AOP(IC_RIGHT(ic)),offset));
          emitpcode(POC_SUBWF,  popGet(AOP(IC_RESULT(ic)),offset));
          offset++;
        }
      }
    }
  }
  
  //    adjustArithmeticResult(ic);
  
release:
  freeAsmop(IC_LEFT(ic),NULL,ic,(RESULTONSTACK(ic) ? FALSE : TRUE));
  freeAsmop(IC_RIGHT(ic),NULL,ic,(RESULTONSTACK(ic) ? FALSE : TRUE));
  freeAsmop(IC_RESULT(ic),NULL,ic,TRUE);
}
/*-----------------------------------------------------------------*
* genUMult8XLit_16 - unsigned multiplication of two 8-bit numbers.
* 
* 
*-----------------------------------------------------------------*/
void genUMult8XLit_16 (operand *left,
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
    result_hi = PCOR(popGet(AOP(result),1));
  }
  
  lit = (unsigned int)floatFromVal(AOP(right)->aopu.aop_lit);
  lit &= 0xff;
  pic14_emitcode(";","Unrolled 8 X 8 multiplication");
  
  same = pic14_sameRegs(AOP(left), AOP(result));
  
  if(same) {
    switch(lit) {
    case 0:
      emitpcode(POC_CLRF,  popGet(AOP(left),0));
      return;
    case 2:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      return;
    case 3:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      return;
    case 4:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      return;
    case 5:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 2*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 3*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 5*F
      return;
    case 6:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      return;
    case 7:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 2*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 3*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 5*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 7*F
      return;
    case 8:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 2*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 3*F
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 5*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 8*F
      return;
    case 9:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      return;
    case 10:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 2*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 3*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 5*F
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      return;
    case 11:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 2*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 3*F
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 5*F
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 8*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 11*F
      return;
    case 12:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDWF, popGet(AOP(left),0));
      return;
    case 13:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 2*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 3*F
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 5*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 8*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 13*F
      return;
    case 14:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 2*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 3*F
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 5*F
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 8*F
      emitpcode(POC_ADDFW, popGet(AOP(left),0));  // W = 11*F
      emitpcode(POC_ADDWF, popGet(AOP(left),0));  // F = 14*F
      return;
    case 15:
      temp = popGetTempReg();
      if(!temp) {
        fprintf(stderr,"ERROR: unable to allocate register. %s:%d\n",__FUNCTION__,__LINE__);
        exit(1);
      }
      emitpcode(POC_SWAPFW, popGet(AOP(left),0));
      emitpcode(POC_MOVWF,  temp);
      emitpcode(POC_ANDLW,  popGetLit(0xf0));
      emitpcode(POC_MOVWF,  popGet(AOP(left),0));
      emitpcode(POC_SWAPFW, temp);
      emitpcode(POC_SUBWF,  popGet(AOP(left),0));
      popReleaseTempReg(temp);
      return;
    case 16:
      emitpcode(POC_SWAPFW, popGet(AOP(left),0));
      emitpcode(POC_ANDLW,  popGetLit(0xf0));
      emitpcode(POC_MOVWF,  popGet(AOP(left),0));
      return;
    case 17:
      emitpcode(POC_SWAPFW, popGet(AOP(left),0));
      emitpcode(POC_ANDLW,  popGetLit(0xf0));
      emitpcode(POC_ADDWF,  popGet(AOP(left),0));
      return;
    case 32:
      emitpcode(POC_SWAPF,  popGet(AOP(left),0));
      emitpcode(POC_RLFW,   popGet(AOP(left),0));
      emitpcode(POC_ANDLW,  popGetLit(0xe0));
      emitpcode(POC_MOVWF,  popGet(AOP(left),0));
      return;
    case 64:
      emitpcode(POC_SWAPF,  popGet(AOP(left),0));
      emitpcode(POC_RLF,    popGet(AOP(left),0));
      emitpcode(POC_RLFW,   popGet(AOP(left),0));
      emitpcode(POC_ANDLW,  popGetLit(0xc0));
      emitpcode(POC_MOVWF,  popGet(AOP(left),0));
      return;
    case 128:
      emitpcode(POC_RRFW,   popGet(AOP(left),0));
      emitpcode(POC_CLRF,   popGet(AOP(left),0));
      emitpcode(POC_RRF,    popGet(AOP(left),0));
      return;
      
  }
  } else {
    
    switch(lit) {
    case 0:
      emitpcode(POC_CLRF,  popGet(AOP(result),0));
      emitpcode(POC_CLRF,  popCopyReg(result_hi));
      return;
    case 2:
      emitpcode(POC_MOVFW, popGet(AOP(left),0));
      emitpcode(POC_MOVWF, popGet(AOP(result),0));
      emitpcode(POC_ADDWF, popGet(AOP(result),0));
      emitpcode(POC_CLRF,  popCopyReg(result_hi));
      emitpcode(POC_RLF,   popCopyReg(result_hi));
      return;
    }
    
  }
  
  emitpcode(POC_MOVFW, popGet(AOP(left),0));
  emitpcode(POC_CLRF,  popGet(AOP(result),0));
  emitpcode(POC_CLRF,  popCopyReg(result_hi));
  
  have_first_bit = 0;
  for(i=0; i<8; i++) {
    
    if(lit & 1) {
      emitpcode(POC_ADDWF, popCopyReg(result_hi));
      have_first_bit = 1;
    }
    
    if(have_first_bit) {
      emitpcode(POC_RRF,   popCopyReg(result_hi));
      emitpcode(POC_RRF,   popGet(AOP(result),0));
    }
    
    lit >>= 1;
  }
  
}

/*-----------------------------------------------------------------*
* genUMult8X8_16 - unsigned multiplication of two 8-bit numbers.
* 
* 
*-----------------------------------------------------------------*/
void genUMult8X8_16 (operand *left,
           operand *right,
           operand *result,
           pCodeOpReg *result_hi)
           
{
  
  int i;
  int looped = 1;
  
  if(!result_hi) {
    result_hi = PCOR(popGet(AOP(result),1));
  }
  
  if (AOP_TYPE(right) == AOP_LIT) {
    genUMult8XLit_16(left,right,result,result_hi);
    return;
  }
  
  if(!looped) {
    pic14_emitcode(";","Unrolled 8 X 8 multiplication");
    
    emitpcode(POC_MOVFW, popGet(AOP(right),0));
    emitpcode(POC_CLRF,  popGet(AOP(result),0));
    emitpcode(POC_CLRF,  popCopyReg(result_hi));
    emitCLRC;
    
    for(i=0; i<8; i++) {
      emitpcode(POC_BTFSC,  newpCodeOpBit(aopGet(AOP(left),0,FALSE,FALSE),i,0));
      emitpcode(POC_ADDWF, popCopyReg(result_hi));
      emitpcode(POC_RRF,   popCopyReg(result_hi));
      emitpcode(POC_RRF,   popGet(AOP(result),0));
    }
    
    
    /*
    Here's another version that does the same thing and takes the 
    same number of instructions. The one above is slightly better
    because the entry instructions have a higher probability of
    being optimized out.
    */
    /*
    emitpcode(POC_CLRF,  popCopyReg(result_hi));
    emitpcode(POC_RRFW,  popGet(AOP(left),0));
    emitpcode(POC_MOVWF, popGet(AOP(result),0));
    emitpcode(POC_MOVFW, popGet(AOP(right),0));
    
      for(i=0; i<8; i++) {
      emitSKPNC;
      emitpcode(POC_ADDWF, popCopyReg(result_hi));
      emitpcode(POC_RRF,   popCopyReg(result_hi));
      emitpcode(POC_RRF,   popGet(AOP(result),0));
      }
    */
    
  } else {
    symbol  *tlbl = newiTempLabel(NULL);
    pCodeOp *temp;
    
    
    pic14_emitcode(";","Looped 8 X 8 multiplication");
    
    emitpcode(POC_CLRF,  popGet(AOP(result),0));
    emitpcode(POC_CLRF,  popCopyReg(result_hi));
    
    emitpcode(POC_BSF,   newpCodeOpBit(aopGet(AOP(result),0,FALSE,FALSE),7,0));
    
    emitpcode(POC_MOVFW, popGet(AOP(right),0));
    
    temp = popGetTempReg();
    emitpcode(POC_MOVWF, popCopyReg(PCOR(temp)));
    
    emitpcode(POC_MOVFW, popGet(AOP(left),0));
    
    emitpLabel(tlbl->key);
    
    emitpcode(POC_RRF,   popCopyReg(PCOR(temp)));
    emitSKPNC;
    emitpcode(POC_ADDWF, popCopyReg(result_hi));
    
    emitpcode(POC_RRF,   popCopyReg(result_hi));
    emitpcode(POC_RRF,   popGet(AOP(result),0));
    
    emitSKPC;
    emitpcode(POC_GOTO,  popGetLabel(tlbl->key));
    
    popReleaseTempReg(temp);
    
  }
}

/*-----------------------------------------------------------------*
* genSMult8X8_16 - signed multiplication of two 8-bit numbers
*
*  this routine will call the unsigned multiply routine and then
* post-fix the sign bit.
*-----------------------------------------------------------------*/
void genSMult8X8_16 (operand *left,
           operand *right,
           operand *result,
           pCodeOpReg *result_hi)
{
  
  if(!result_hi) {
    result_hi = PCOR(popGet(AOP(result),1));
  }
  
  genUMult8X8_16(left,right,result,result_hi);
  
  emitpcode(POC_BTFSC, newpCodeOpBit(aopGet(AOP(left),0,FALSE,FALSE),7,0));
  emitpcode(POC_SUBWF, popCopyReg(result_hi));
  emitpcode(POC_MOVFW, popGet(AOP(left),0));
  emitpcode(POC_BTFSC, newpCodeOpBit(aopGet(AOP(right),0,FALSE,FALSE),7,0));
  emitpcode(POC_SUBWF, popGet(AOP(result),1));
  
}

/*-----------------------------------------------------------------*
* genMult8X8_8 - multiplication of two 8-bit numbers
*
*  this routine will call the unsigned multiply 8X8=>16 routine and
* then throw away the high byte of the result.
*
*-----------------------------------------------------------------*/
void genMult8X8_8 (operand *left,
           operand *right,
           operand *result)
{
  pCodeOp *result_hi = popGetTempReg();
  
  if (AOP_TYPE(right) == AOP_LIT)
    genUMult8XLit_16(left,right,result,PCOR(result_hi));
  else
    genUMult8X8_16(left,right,result,PCOR(result_hi));
  
  popReleaseTempReg(result_hi);
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
