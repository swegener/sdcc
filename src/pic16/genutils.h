
/*
** $Id$
*/

#ifndef __GENUTILS_H__
#define __GENUTILS_H__


#include "common.h"


/*
 * The various GEN_xxxxx macros handle which functions
 * should be included in the gen.c source. We are going to use
 * our own functions here so, they must be commented out from
 * gen.c
 */

#define GEN_Not
void pic16_genNot(iCode *ic);

#define GEN_Cpl
void pic16_genCpl(iCode *ic);


/*
 * global function definitions
 */
void pic16_DumpValue(char *prefix, value *val);
void pic16_DumpPcodeOp(char *prefix, pCodeOp *pcop);
void pic16_DumpAop(char *prefix, asmop *aop);
void pic16_DumpSymbol(char *prefix, symbol *sym);
void pic16_DumpOp(char *prefix, operand *op);

void gpsimio2_pcop(pCodeOp *pcop);
void gpsimio2_lit(unsigned char lit);

void gpsimDebug_StackDump(char *fname, int line, char *info);

#define debugf(frm, rest)       _debugf(__FILE__, __LINE__, frm, rest)
void _debugf(char *f, int l, char *frm, ...);

#endif	/* __GENUTILS_H__ */
