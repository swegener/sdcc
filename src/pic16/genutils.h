
/*
** $Id$
*/

#ifndef __GENUTILS_H__
#define __GENUTILS_H__


#include "common.h"


/*
 * The various GEN_xxxxx macros handle which functions
 * should be included in the gen.c source. We are going to use
 * our own functions here so, they must be commentted out from
 * gen.c
 */

#define GEN_Not
void pic16_genNot(iCode *ic);

#define GEN_Cpl
void pic16_genCpl(iCode *ic);


/*
 * global function definitions
 */
void DEBUGpic16_pic16_AopType(int line_no, operand *left, operand *right, operand *result);


#endif	/* __GENUTILS_H__ */
