/*-------------------------------------------------------------------------
 genutils.c - source file for code generation for pic16
 	code generation utility functions

	Created by Vangelis Rokas (vrokas@otenet.gr) [Nov-2003]

	Based on :

  Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)
         and -  Jean-Louis VERN.jlvern@writeme.com (1999)
  Bug Fixes  -  Wojciech Stryjewski  wstryj1@tiger.lsu.edu (1999 v2.1.9a)
  PIC port   -  Scott Dattalo scott@dattalo.com (2000)
  PIC16 port -  Martin Dubuc m.dubuc@rogers.com (2002)
  
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

/**********************************************************
 * Here is a list with completed genXXXXX functions
 *
 * genNot
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SDCCglobl.h"
#include "newalloc.h"

#include "common.h"
#include "SDCCpeeph.h"
#include "ralloc.h"
#include "pcode.h"
#include "gen.h"

#include "genutils.h"

#if 1
#define pic16_emitcode	DEBUGpic16_emitcode
#endif

#if defined(GEN_Not)
/*-----------------------------------------------------------------*/
/* pic16_genNot - generate code for ! operation                    */
/*-----------------------------------------------------------------*/
void pic16_genNot (iCode *ic)
{
  int size;

/*
 * result[AOP_CRY,AOP_REG]  = ! left[AOP_CRY, AOP_REG]
 */

	DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
	/* assign asmOps to operand & result */
	pic16_aopOp (IC_LEFT(ic),ic,FALSE);
	pic16_aopOp (IC_RESULT(ic),ic,TRUE);
	DEBUGpic16_pic16_AopType(__LINE__,IC_LEFT(ic),NULL,IC_RESULT(ic));

	/* if in bit space then a special case */
	if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY) {
		if (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY) {
			pic16_emitpcode(POC_MOVLW,pic16_popGet(AOP(IC_LEFT(ic)),0));
			pic16_emitpcode(POC_XORWF,pic16_popGet(AOP(IC_RESULT(ic)),0));
		} else {
			pic16_emitpcode(POC_CLRF,pic16_popGet(AOP(IC_RESULT(ic)),0));
			pic16_emitpcode(POC_BTFSS,pic16_popGet(AOP(IC_LEFT(ic)),0));
			pic16_emitpcode(POC_INCF,pic16_popGet(AOP(IC_RESULT(ic)),0));
		}
		goto release;
	}

	size = AOP_SIZE(IC_LEFT(ic));
	if(size == 1) {
		pic16_emitpcode(POC_COMFW,pic16_popGet(AOP(IC_LEFT(ic)),0));
		pic16_emitpcode(POC_ANDLW,pic16_popGetLit(1));
		pic16_emitpcode(POC_MOVWF,pic16_popGet(AOP(IC_RESULT(ic)),0));
		goto release;
	}

release:    
	/* release the aops */
	pic16_freeAsmop(IC_LEFT(ic),NULL,ic,(RESULTONSTACK(ic) ? 0 : 1));
	pic16_freeAsmop(IC_RESULT(ic),NULL,ic,TRUE);
}

#endif	/* defined(GEN_Not) */



#if defined(GEN_Cpl)
/*-----------------------------------------------------------------*/
/* pic16_genCpl - generate code for complement                     */
/*-----------------------------------------------------------------*/
void pic16_genCpl (iCode *ic)
{
  int offset = 0;
  int size ;

/*
 * result[CRY,REG] = ~left[CRY,REG]
 */
 

	DEBUGpic16_emitcode ("; ***","%s  %d",__FUNCTION__,__LINE__);
	/* assign asmOps to operand & result */
	pic16_aopOp (IC_LEFT(ic),ic,FALSE);
	pic16_aopOp (IC_RESULT(ic),ic,TRUE);
	DEBUGpic16_pic16_AopType(__LINE__,IC_LEFT(ic),NULL,IC_RESULT(ic));

	/* if both are in bit space then 
	   a special case */
	if (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY &&
		AOP_TYPE(IC_LEFT(ic)) == AOP_CRY ) { 

		/* FIXME */
		pic16_emitcode("mov","c,%s",IC_LEFT(ic)->aop->aopu.aop_dir); 
		pic16_emitcode("cpl","c"); 
		pic16_emitcode("mov","%s,c",IC_RESULT(ic)->aop->aopu.aop_dir); 
		goto release; 
	} 

	size = AOP_SIZE(IC_RESULT(ic));
	while (size--) {

		if (pic16_sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
			pic16_emitpcode(POC_COMF,  pic16_popGet(AOP(IC_LEFT(ic)), offset));
		} else {
			pic16_emitpcode(POC_COMFW, pic16_popGet(AOP(IC_LEFT(ic)),offset));
			pic16_emitpcode(POC_MOVWF, pic16_popGet(AOP(IC_RESULT(ic)),offset));
		}
		offset++;
	}

release:
    /* release the aops */
    pic16_freeAsmop(IC_LEFT(ic),NULL,ic,(RESULTONSTACK(ic) ? 0 : 1));
    pic16_freeAsmop(IC_RESULT(ic),NULL,ic,TRUE);
}
#endif	/* defined(GEN_Cpl) */

