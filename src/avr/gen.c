/*-------------------------------------------------------------------------
  gen.c - source file for code generation for AVR
  
  Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)
         and -  Jean-Louis VERN.jlvern@writeme.com (1999)
  Bug Fixes  -  Wojciech Stryjewski  wstryj1@tiger.lsu.edu (1999 v2.1.9a)
  
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

#ifdef HAVE_SYS_ISA_DEFS_H
#include <sys/isa_defs.h>
#else
#ifdef HAVE_ENDIAN_H
#include <endian.h>
#else
#ifndef __BORLANDC__
#warning "Cannot determine ENDIANESS of this machine assuming LITTLE_ENDIAN"
#warning "If you running sdcc on an INTEL 80x86 Platform you are okay"
#endif
#endif
#endif

#include "common.h"
#include "SDCCpeeph.h"
#include "ralloc.h"
#include "gen.h"

char *aopLiteral (value *val, int offset);

/* this is the down and dirty file with all kinds of 
   kludgy & hacky stuff. This is what it is all about
   CODE GENERATION for a specific MCU . some of the
   routines may be reusable, will have to see */

static char *zero = "#0x00";
static char *one  = "#0x01";

static struct {
    short r0Pushed;
    short r1Pushed;
    short accInUse;
    short inLine;
    short debugLine;
    short nRegsSaved;
    set *sendSet;
} _G;

extern FILE *codeOutFile;

static lineNode *lineHead = NULL;
static lineNode *lineCurr = NULL;

/*-----------------------------------------------------------------*/
/* emitcode - writes the code into a file : for now it is simple    */
/*-----------------------------------------------------------------*/
static void emitcode (char *inst,char *fmt, ...)
{
    va_list ap;
    char lb[MAX_INLINEASM];  
    char *lbp = lb;

    va_start(ap,fmt);   

    if (inst && *inst) {
	if (fmt && *fmt)
	    sprintf(lb,"%s\t",inst);
	else
	    sprintf(lb,"%s",inst);
        vsprintf(lb+(strlen(lb)),fmt,ap);
    }  else
        vsprintf(lb,fmt,ap);

    while (isspace(*lbp)) lbp++;

    if (lbp && *lbp) 
        lineCurr = (lineCurr ?
                    connectLine(lineCurr,newLineNode(lb)) :
                    (lineHead = newLineNode(lb)));
    lineCurr->isInline = _G.inLine;
    lineCurr->isDebug  = _G.debugLine;
    va_end(ap);
}

/*-----------------------------------------------------------------*/
/* genAVRCode - generate code forAVR  based controllers            */
/*-----------------------------------------------------------------*/
void genAVRCode (iCode *lic)
{
    iCode *ic;
    int cln = 0;

    lineHead = lineCurr = NULL;

    /* if debug information required */
/*     if (options.debug && currFunc) { */
    if (currFunc) {
	cdbSymbol(currFunc,cdbFile,FALSE,TRUE);
	_G.debugLine = 1;
	if (IS_STATIC(currFunc->etype))
	    emitcode("","F%s$%s$0$0 ==.",moduleName,currFunc->name); 
	else
	    emitcode("","G$%s$0$0 ==.",currFunc->name);
	_G.debugLine = 0;
    }
 
    for (ic = lic ; ic ; ic = ic->next ) {
	piCode(ic,stdout);
    }

    /* now we are ready to call the 
       peep hole optimizer */
    if (!options.nopeep)
	peepHole (&lineHead);

    /* now do the actual printing */
    printLine (lineHead,codeOutFile);
    return;
}
