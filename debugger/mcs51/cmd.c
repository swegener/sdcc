/*-------------------------------------------------------------------------
    cmd.c - source  file for debugger command execution

	      Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

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
-------------------------------------------------------------------------*/

#include "sdcdb.h"
#include "symtab.h"
#include "simi.h"
#include "break.h"
#include "cmd.h"
#include "newalloc.h"

/* default number of lines to list out */
int listLines = 16;

/* mainly used to retain a reference to the active module being
   listed.  May be used as a general context for other commands if
   no better context is available */
static module *list_mod = NULL;

EXTERN_STACK_DCL(callStack,function *,1024);

#if defined(__APPLE__) && defined(__MACH__)
static char *copying=
{" GNU GENERAL PUBLIC LICENSE Version 2"};
static char *warranty=
{" NO WARRANTY"};
#else
static char *copying=
"                   GNU GENERAL PUBLIC LICENSE\n"
"                       Version 2, June 1991\n"
"\n"
" Copyright (C) 1989, 1991 Free Software Foundation, Inc.\n"
" 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA\n"
" Everyone is permitted to copy and distribute verbatim copies\n"
" of this license document, but changing it is not allowed.\n"
"\n"
"                            Preamble\n"
"\n"
"  The licenses for most software are designed to take away your\n"
"freedom to share and change it.  By contrast, the GNU General Public\n"
"License is intended to guarantee your freedom to share and change free\n"
"software--to make sure the software is free for all its users.  This\n"
"General Public License applies to most of the Free Software\n"
"Foundation's software and to any other program whose authors commit to\n"
"using it.  (Some other Free Software Foundation software is covered by\n"
"the GNU Library General Public License instead.)  You can apply it to\n"
"your programs, too.\n"
"\n"
"  When we speak of free software, we are referring to freedom, not\n"
"price.  Our General Public Licenses are designed to make sure that you\n"
"have the freedom to distribute copies of free software (and charge for\n"
"this service if you wish), that you receive source code or can get it\n"
"if you want it, that you can change the software or use pieces of it\n"
"in new free programs; and that you know you can do these things.\n"
"\n"
"  To protect your rights, we need to make restrictions that forbid\n"
"anyone to deny you these rights or to ask you to surrender the rights.\n"
"These restrictions translate to certain responsibilities for you if you\n"
"distribute copies of the software, or if you modify it.\n"
"\n"
"  For example, if you distribute copies of such a program, whether\n"
"gratis or for a fee, you must give the recipients all the rights that\n"
"you have.  You must make sure that they, too, receive or can get the\n"
"source code.  And you must show them these terms so they know their\n"
"rights.\n"
"\n"
"  We protect your rights with two steps: (1) copyright the software, and\n"
"(2) offer you this license which gives you legal permission to copy,\n"
"distribute and/or modify the software.\n"
"\n"
"  Also, for each author's protection and ours, we want to make certain\n"
"that everyone understands that there is no warranty for this free\n"
"software.  If the software is modified by someone else and passed on, we\n"
"want its recipients to know that what they have is not the original, so\n"
"that any problems introduced by others will not reflect on the original\n"
"authors' reputations.\n"
"\n"
"  Finally, any free program is threatened constantly by software\n"
"patents.  We wish to avoid the danger that redistributors of a free\n"
"program will individually obtain patent licenses, in effect making the\n"
"program proprietary.  To prevent this, we have made it clear that any\n"
"patent must be licensed for everyone's free use or not licensed at all.\n"
"\n"
"  The precise terms and conditions for copying, distribution and\n"
"modification follow.\n"
"^L\n"
"                    GNU GENERAL PUBLIC LICENSE\n"
"   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n"
"\n"
"  0. This License applies to any program or other work which contains\n"
"a notice placed by the copyright holder saying it may be distributed\n"
"under the terms of this General Public License.  The \"Program\", below,\n"
"refers to any such program or work, and a \"work based on the Program\"\n"
"means either the Program or any derivative work under copyright law:\n"
"that is to say, a work containing the Program or a portion of it,\n"
"either verbatim or with modifications and/or translated into another\n"
"language.  (Hereinafter, translation is included without limitation in\n"
"the term \"modification\".)  Each licensee is addressed as \"you\".\n"
"\n"
"Activities other than copying, distribution and modification are not\n"
"covered by this License; they are outside its scope.  The act of\n"
"running the Program is not restricted, and the output from the Program\n"
"is covered only if its contents constitute a work based on the\n"
"Program (independent of having been made by running the Program).\n"
"Whether that is true depends on what the Program does.\n"
"\n"
"  1. You may copy and distribute verbatim copies of the Program's\n"
"source code as you receive it, in any medium, provided that you\n"
"conspicuously and appropriately publish on each copy an appropriate\n"
"copyright notice and disclaimer of warranty; keep intact all the\n"
"notices that refer to this License and to the absence of any warranty;\n"
"and give any other recipients of the Program a copy of this License\n"
"along with the Program.\n"
"\n"
"You may charge a fee for the physical act of transferring a copy, and\n"
"you may at your option offer warranty protection in exchange for a fee.\n"
"\n"
"  2. You may modify your copy or copies of the Program or any portion\n"
"of it, thus forming a work based on the Program, and copy and\n"
"distribute such modifications or work under the terms of Section 1\n"
"above, provided that you also meet all of these conditions:\n"
"\n"
"    a) You must cause the modified files to carry prominent notices\n"
"    stating that you changed the files and the date of any change.\n"
"\n"
"    b) You must cause any work that you distribute or publish, that in\n"
"    whole or in part contains or is derived from the Program or any\n"
"    part thereof, to be licensed as a whole at no charge to all third\n"
"    parties under the terms of this License.\n"
"\n"
"    c) If the modified program normally reads commands interactively\n"
"    when run, you must cause it, when started running for such\n"
"    interactive use in the most ordinary way, to print or display an\n"
"    announcement including an appropriate copyright notice and a\n"
"    notice that there is no warranty (or else, saying that you provide\n"
"    a warranty) and that users may redistribute the program under\n"
"    these conditions, and telling the user how to view a copy of this\n"
"    License.  (Exception: if the Program itself is interactive but\n"
"    does not normally print such an announcement, your work based on\n"
"    the Program is not required to print an announcement.)\n"
"\n"
"These requirements apply to the modified work as a whole.  If\n"
"identifiable sections of that work are not derived from the Program,\n"
"and can be reasonably considered independent and separate works in\n"
"themselves, then this License, and its terms, do not apply to those\n"
"sections when you distribute them as separate works.  But when you\n"
"distribute the same sections as part of a whole which is a work based\n"
"on the Program, the distribution of the whole must be on the terms of\n"
"this License, whose permissions for other licensees extend to the\n"
"entire whole, and thus to each and every part regardless of who wrote it.\n"
"\n"
"Thus, it is not the intent of this section to claim rights or contest\n"
"your rights to work written entirely by you; rather, the intent is to\n"
"exercise the right to control the distribution of derivative or\n"
"collective works based on the Program.\n"
"\n"
"In addition, mere aggregation of another work not based on the Program\n"
"with the Program (or with a work based on the Program) on a volume of\n"
"a storage or distribution medium does not bring the other work under\n"
"the scope of this License.\n"
"\n"
"  3. You may copy and distribute the Program (or a work based on it,\n"
"under Section 2) in object code or executable form under the terms of\n"
"Sections 1 and 2 above provided that you also do one of the following:\n"
"\n"
"    a) Accompany it with the complete corresponding machine-readable\n"
"    source code, which must be distributed under the terms of Sections\n"
"    1 and 2 above on a medium customarily used for software interchange; or,\n"
"\n"
"    b) Accompany it with a written offer, valid for at least three\n"
"    years, to give any third party, for a charge no more than your\n"
"    cost of physically performing source distribution, a complete\n"
"    machine-readable copy of the corresponding source code, to be\n"
"    distributed under the terms of Sections 1 and 2 above on a medium\n"
"    customarily used for software interchange; or,\n"
"\n"
"    c) Accompany it with the information you received as to the offer\n"
"    to distribute corresponding source code.  (This alternative is\n"
"    allowed only for noncommercial distribution and only if you\n"
"    received the program in object code or executable form with such\n"
"    an offer, in accord with Subsection b above.)\n"
"\n"
"The source code for a work means the preferred form of the work for\n"
"making modifications to it.  For an executable work, complete source\n"
"code means all the source code for all modules it contains, plus any\n"
"associated interface definition files, plus the scripts used to\n"
"control compilation and installation of the executable.  However, as a\n"
"special exception, the source code distributed need not include\n"
"anything that is normally distributed (in either source or binary\n"
"form) with the major components (compiler, kernel, and so on) of the\n"
"operating system on which the executable runs, unless that component\n"
"itself accompanies the executable.\n"
"\n"
"If distribution of executable or object code is made by offering\n"
"access to copy from a designated place, then offering equivalent\n"
"access to copy the source code from the same place counts as\n"
"distribution of the source code, even though third parties are not\n"
"compelled to copy the source along with the object code.\n"
"^L\n"
"  4. You may not copy, modify, sublicense, or distribute the Program\n"
"except as expressly provided under this License.  Any attempt\n"
"otherwise to copy, modify, sublicense or distribute the Program is\n"
"void, and will automatically terminate your rights under this License.\n"
"However, parties who have received copies, or rights, from you under\n"
"this License will not have their licenses terminated so long as such\n"
"parties remain in full compliance.\n"
"\n"
"  5. You are not required to accept this License, since you have not\n"
"signed it.  However, nothing else grants you permission to modify or\n"
"distribute the Program or its derivative works.  These actions are\n"
"prohibited by law if you do not accept this License.  Therefore, by\n"
"modifying or distributing the Program (or any work based on the\n"
"Program), you indicate your acceptance of this License to do so, and\n"
"all its terms and conditions for copying, distributing or modifying\n"
"the Program or works based on it.\n"
"\n"
"  6. Each time you redistribute the Program (or any work based on the\n"
"Program), the recipient automatically receives a license from the\n"
"original licensor to copy, distribute or modify the Program subject to\n"
"these terms and conditions.  You may not impose any further\n"
"restrictions on the recipients' exercise of the rights granted herein.\n"
"You are not responsible for enforcing compliance by third parties to\n"
"this License.\n"
"\n"
"  7. If, as a consequence of a court judgment or allegation of patent\n"
"infringement or for any other reason (not limited to patent issues),\n"
"conditions are imposed on you (whether by court order, agreement or\n"
"otherwise) that contradict the conditions of this License, they do not\n"
"excuse you from the conditions of this License.  If you cannot\n"
"distribute so as to satisfy simultaneously your obligations under this\n"
"License and any other pertinent obligations, then as a consequence you\n"
"may not distribute the Program at all.  For example, if a patent\n"
"license would not permit royalty-free redistribution of the Program by\n"
"all those who receive copies directly or indirectly through you, then\n"
"the only way you could satisfy both it and this License would be to\n"
"refrain entirely from distribution of the Program.\n"
"\n"
"If any portion of this section is held invalid or unenforceable under\n"
"any particular circumstance, the balance of the section is intended to\n"
"apply and the section as a whole is intended to apply in other\n"
"circumstances.\n"
"\n"
"It is not the purpose of this section to induce you to infringe any\n"
"patents or other property right claims or to contest validity of any\n"
"such claims; this section has the sole purpose of protecting the\n"
"integrity of the free software distribution system, which is\n"
"implemented by public license practices.  Many people have made\n"
"generous contributions to the wide range of software distributed\n"
"through that system in reliance on consistent application of that\n"
"system; it is up to the author/donor to decide if he or she is willing\n"
"to distribute software through any other system and a licensee cannot\n"
"impose that choice.\n"
"\n"
"This section is intended to make thoroughly clear what is believed to\n"
"be a consequence of the rest of this License.\n"
"\n"
"  8. If the distribution and/or use of the Program is restricted in\n"
"certain countries either by patents or by copyrighted interfaces, the\n"
"original copyright holder who places the Program under this License\n"
"may add an explicit geographical distribution limitation excluding\n"
"those countries, so that distribution is permitted only in or among\n"
"countries not thus excluded.  In such case, this License incorporates\n"
"the limitation as if written in the body of this License.\n"
"\n"
"  9. The Free Software Foundation may publish revised and/or new versions\n"
"of the General Public License from time to time.  Such new versions will\n"
"be similar in spirit to the present version, but may differ in detail to\n"
"address new problems or concerns.\n"
"\n"
"Each version is given a distinguishing version number.  If the Program\n"
"specifies a version number of this License which applies to it and \"any\n"
"later version\", you have the option of following the terms and conditions\n"
"either of that version or of any later version published by the Free\n"
"Software Foundation.  If the Program does not specify a version number of\n"
"this License, you may choose any version ever published by the Free Software\n"
"Foundation.\n"
"\n"
"  10. If you wish to incorporate parts of the Program into other free\n"
"programs whose distribution conditions are different, write to the author\n"
"to ask for permission.  For software which is copyrighted by the Free\n"
"Software Foundation, write to the Free Software Foundation; we sometimes\n"
"make exceptions for this.  Our decision will be guided by the two goals\n"
"of preserving the free status of all derivatives of our free software and\n"
"of promoting the sharing and reuse of software generally.\n";

static char *warranty=
"                            NO WARRANTY\n"
"\n"
"  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY\n"
"FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN\n"
"OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES\n"
"PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED\n"
"OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\n"
"MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS\n"
"TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE\n"
"PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,\n"
"REPAIR OR CORRECTION.\n"
"\n"
"  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\n"
"WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR\n"
"REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,\n"
"INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING\n"
"OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED\n"
"TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY\n"
"YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER\n"
"PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE\n"
"POSSIBILITY OF SUCH DAMAGES.\n";
#endif

static void printTypeInfo(link *);
static void printValAggregates (symbol *,link *,char,unsigned int);
static void setSymValue(symbol *sym, char *val, context *cctxt);

int srcMode = SRC_CMODE ;
static set  *dispsymbols = NULL   ; /* set of displayable symbols */

/*-----------------------------------------------------------------*/
/* funcWithName - returns function with name                       */
/*-----------------------------------------------------------------*/
DEFSETFUNC(funcWithName)
{
    function *func = item;
    V_ARG(char *,name);
    V_ARG(function **,funcp);

    if (*funcp)
	return 0;

    if (strcmp(func->sym->name,name) == 0) {
	*funcp = func;
	return 1;
    }
    
    return 0;
}

/*-----------------------------------------------------------------*/
/* setBPatModLine - set break point at the line specified for the  */
/*-----------------------------------------------------------------*/
static void setBPatModLine (module *mod, int line)
{
  int next_line;

    /* look for the first executable line after the line
       specified & get the break point there */    
    if (srcMode == SRC_CMODE && line > mod->ncLines) {
	fprintf(stderr,"No line %d in file \"%s\".\n",
		line,mod->c_name);
	return ;
    }
    
    if (srcMode == SRC_AMODE && line > mod->nasmLines) {
	fprintf(stderr,"No line %d in file \"%s\".\n",
		line,mod->asm_name);
	return ;
    }

    next_line = line;
    for ( ; next_line < (srcMode == SRC_CMODE ? mod->ncLines : mod->nasmLines ) ; 
	  next_line++ ) {
	if (srcMode == SRC_CMODE) {
	    if (mod->cLines[next_line]->addr) {
		setBreakPoint (mod->cLines[next_line]->addr, CODE, USER, 
			       userBpCB, mod->c_name, next_line);
		return;
//		break;
	    }
	}
	else {
	   if (mod->asmLines[next_line]->addr) {
	       setBreakPoint (mod->asmLines[next_line]->addr, CODE, USER, 
			      userBpCB, mod->asm_name, next_line);
		return;
//	       break;
	   } 
	}
    }

    	fprintf(stderr,"No line %d or after in file \"%s\"..\n",
        		line,mod->c_name);

    return;
}

/*-----------------------------------------------------------------*/
/* clearBPatModLine - clr break point at the line specified        */
/*-----------------------------------------------------------------*/
static void clearBPatModLine (module *mod, int line)
{
    /* look for the first executable line after the line
       specified & get the break point there */
    if (srcMode == SRC_CMODE && line > mod->ncLines) {
	fprintf(stderr,"No line %d in file \"%s\".\n",
		line,mod->c_name);
	return ;
    }
    
    if (srcMode == SRC_AMODE && line > mod->ncLines) {
	fprintf(stderr,"No line %d in file \"%s\".\n",
		line,mod->c_name);
	return ;
    }    
    
    for ( ; line < (srcMode == SRC_CMODE ? mod->ncLines : mod->nasmLines ) ; 
	  line++ ) {
	if (srcMode == SRC_CMODE) 
	    if (mod->cLines[line]->addr) {
		clearUSERbp (mod->cLines[line]->addr);			  
		break;
	    }
	else
	    if (mod->asmLines[line]->addr) {
		clearUSERbp (mod->asmLines[line]->addr);			  
		break;
	    }
    }

    return;
}

/*-----------------------------------------------------------------*/
/* moduleLineWithAddr - finds and returns a line  with a given address */
/*-----------------------------------------------------------------*/
DEFSETFUNC(moduleLineWithAddr)
{
    module *mod = item;
    int i;

    V_ARG(unsigned int,addr);
    V_ARG(module **,rmod);
    V_ARG(int *,line);

    if (*rmod)
        return 0;

    for (i=0; i < mod->nasmLines; i++ ) 
    {
        if ( mod->asmLines[i]->addr == addr)
        {
            *rmod = mod ;
            if (line )
                *line = mod->ncLines;
            return 1;
        }
    }

    return 0;
}

/*-----------------------------------------------------------------*/
/* funcWithNameModule - returns functions with a name module combo */
/*-----------------------------------------------------------------*/
DEFSETFUNC(funcWithNameModule) 
{
    function *func = item;
    V_ARG(char *,fname);
    V_ARG(char *,mname);
    V_ARG(function **,funcp);

    if (*funcp)
        return 0;

    if (strcmp(func->sym->name,fname) == 0 &&
	strcmp(func->mod->c_name,mname) == 0) {
	*funcp = func;
	return 1;
    }

    return 0;
}

/*-----------------------------------------------------------------*/
/* funcInAddr - given an address returns the function              */
/*-----------------------------------------------------------------*/
DEFSETFUNC(funcInAddr)
{
    function *func = item;
    V_ARG(unsigned int,addr);
    V_ARG(function **,funcp);

    if (*funcp)
	return 0;

    /* in the address range */
    if (func->sym->addr <= addr &&
	func->sym->eaddr >= addr) {
	
	*funcp = func;
	return 1;
    }

    return 0;	    
}

/*-----------------------------------------------------------------*/
/* setStepBp - will set STEP Bp @ function entry points            */
/*-----------------------------------------------------------------*/
DEFSETFUNC(setStepBp)
{
    function *func = item;
    
    if (func->sym && func->sym->addr ) {
	
	/* set the entry break point */
	setBreakPoint (func->sym->addr , CODE , STEP , 
		       stepBpCB ,func->mod->c_name , func->entryline);

	return 1;
    }

    return 0;
}

/*-----------------------------------------------------------------*/
/* setStepEPBp - sets a given type of bp @ the execution point     */
/*-----------------------------------------------------------------*/
DEFSETFUNC(setStepEPBp)
{
    exePoint *ep = item;
    V_ARG(int,bptype);
    V_ARG(char *,mname);
   
    setBreakPoint (ep->addr, CODE, bptype, 
		   stepBpCB, mname, ep->line);
    return 1;
}

/*-----------------------------------------------------------------*/
/* setNextEPBp - sets a given type of bp @ the execution point     */
/*-----------------------------------------------------------------*/
DEFSETFUNC(setNextEPBp)
{
    exePoint *ep = item;
    V_ARG(int,bptype);
    V_ARG(char *,mname);
   
    setBreakPoint (ep->addr, CODE, bptype, 
		   nextBpCB, mname, ep->line);
    return 1;
}

/*-----------------------------------------------------------------*/
/* lineAtAddr - for execution points returns the one with addr     */
/*-----------------------------------------------------------------*/
DEFSETFUNC(lineAtAddr)
{
    exePoint *ep = item;
    V_ARG(unsigned int,addr);
    V_ARG(int *,line);
    V_ARG(int *,block);
    V_ARG(int *,level);

    /* address must be an exact match */
    if (ep->addr == addr) {
	*line = ep->line;
	if (block)
	    *block = ep->block ;
	if (level)
	    *level = ep->level ;
	return 1;
    }

    return 0;
    
}

/*-----------------------------------------------------------------*/
/* lineNearAddr - for execution points returns the one with addr   */
/*-----------------------------------------------------------------*/
DEFSETFUNC(lineNearAddr)
{
    exePoint *ep = item;
    V_ARG(unsigned int,addr);
    V_ARG(int *,line);
    V_ARG(int *,block);
    V_ARG(int *,level);

    /* the line in which the address is */
    if (ep->addr <= addr) {
	*line = ep->line;
	if (block)
	    *block = ep->block ;
	if (level)
	    *level = ep->level ;
	return 1;
    }

    return 0;
    
}

/*-----------------------------------------------------------------*/
/* discoverContext - find out the current context of the bp        */
/*-----------------------------------------------------------------*/
context *discoverContext (unsigned addr)
{
    function *func = NULL;
    module   *mod  = NULL;
    int line = 0;

    /* find the function we are in */
    if (!applyToSet(functions,funcInAddr,addr,&func)) {
        if (!applyToSet(functions,funcWithName,"main") ||
            !applyToSet(modules,moduleLineWithAddr,addr,&mod,&line))
        {
            fprintf(stderr, "Error?:discoverContext: cannot apply to set!\n");
            return NULL;
        }
        currCtxt->func = func;
        currCtxt->addr = addr;
        currCtxt->modName = mod->name;
        currCtxt->cline = func->exitline;
    }
    else
    {
        currCtxt->func = func;
        currCtxt->addr = func->laddr = addr;
        currCtxt->modName = func->modName;
    
        /* find the c line number */
        if(applyToSet(func->cfpoints,lineNearAddr,addr,
                  &line,&currCtxt->block,&currCtxt->level)) 
            currCtxt->cline = func->lline = line;
        else
            currCtxt->cline = func->exitline;
    }    
    /* find the asm line number */
    line = 0;
    if (applyToSet(func->afpoints,lineAtAddr,addr,
		   &line,NULL,NULL))
	currCtxt->asmline = line;       
    else
	currCtxt->asmline = -1;
        
    return currCtxt ;
}


/*-----------------------------------------------------------------*/
/* simGo - send 'go' cmd to simulator and wait till a break occurs */
/*-----------------------------------------------------------------*/
void simGo (unsigned int gaddr)
{   
    unsigned int addr ;
    context *ctxt;
    int rv;
    static int initial_break_flag = 0;

 top:    
    if ( userinterrupt )
    {
        userinterrupt = 0;
        return;
    }
    addr = simGoTillBp (gaddr);

    /* got the pc for the break point now first
       discover the program context i.e. module, function 
       linenumber of the source etc, etc etc */
    ctxt = discoverContext (addr);
    
    /* dispatch all the break point call back functions */
    rv = dispatchCB (addr,ctxt);    
 ret:    

    /* the dispatch call back function will return
       non-zero if an user break point has been hit
       if not then we continue with the execution 
       of the program */
    if (!rv) {
      if (!initial_break_flag) {
        initial_break_flag = 1;  // kludge to stop only at first run
        fprintf(stdout, "Stopping at entry.  You can now list and set breakpoints\n");
      }
      else {
       	gaddr = -1;
       	goto top ;
      }

// notes: kpb
// I took this out, after running "run" it would just keep re-running
// even after a lot of break points hit.  For some reason above code
// not triggering(dispatchCB).  This seems to be by design, startup adds
// a bunch of breakpoints-but they are not USER breakpoints.  Perhaps the
// debugger changed with its implementation of "go"("run").  It seems we
// need to add a "next" or "step" followed by a "run"...
// I added a "step" in simi.c when we want a resume function, this seems
// to work.

// still there is question of how do we stop it initially, since
// it must be started before it can get a context.  If so, we would
// want it to just run up to an initial entry point you'd think...
// I don't see why we can't set breakpoints before an initial run,
// this does not seem right to me.

// line #'s are a bit off too.

#if 0
	gaddr = -1;
	goto top ;
#endif
    }
    
}

static int printAsmLine( function *func, module *m, long saddr, long eaddr)
{
    int i,j,delta;
    int symaddr;
    int lastaddr = saddr+1;
    char *symname;

    if ( func )
    {
        symaddr = func->sym->addr;
        symname = func->sym->name;
    }
    else
    {
        symaddr = saddr;
        symname = "" ;
    }
    for (j=0,i=0; i < m->nasmLines; i++ ) 
    {
        if ( saddr >= 0 && m->asmLines[i]->addr < saddr)
        {
                continue;
        }
        if ( eaddr >= 0 && m->asmLines[i]->addr > eaddr)
        {
                continue;
        }
        if ( func && 
            (m->asmLines[i]->addr < func->sym->addr ||
             m->asmLines[i]->addr > func->sym->eaddr ))
        {
            continue;
        } 
        delta = m->asmLines[i]->addr - symaddr;
        if ( delta >= 0 )
        {
            j++;
            lastaddr = m->asmLines[i]->addr;
            printf("0x%08x <%s",lastaddr,symname);
            if (delta > 0) printf("+%d",delta);
            printf(">:\t%s",m->asmLines[i]->src);            
        }
    }
    return lastaddr;
}

/*-----------------------------------------------------------------*/
/* cmdDisasm - disassemble  asm instruction                        */
/*-----------------------------------------------------------------*/
static int cmdDisasm (char *s, context *cctxt, int args)
{
    function *func = NULL;
    long  saddr = -1;
    long  eaddr = -1;
    int   found = 0;
    module *modul;
    /* white space skip */

    if ( args > 0 )
    {
        while (*s && isspace(*s)) s++;

        if ( isdigit(*s))
        {
            saddr = strtol(s,&s,0);
            if ( args > 1 )
            {
                while (*s && isspace(*s)) s++;

                if ( isdigit(*s))
                    eaddr = strtol(s,0,0);
            }
            else
                eaddr = saddr+1;
        }
    }

    if ( eaddr == -1 )
    {       
        /* no start or only start so dump function */
        if ( saddr == -1 )
        {
            func = cctxt->func;
        }
        else
        {
            applyToSet(functions,funcInAddr,saddr,&func);
        }
        if ( func )
        {
            printf("Dump of assembler code for function %s:\n",func->sym->name);
            printAsmLine(func,func->mod,-1,-1);
            printf("End of assembler dump.\n");
            return 0; 
        }
        else
        {
            if (applyToSet(modules,moduleLineWithAddr,saddr,&modul,NULL))
            {
                eaddr = saddr + 5;
                printf("Dump of assembler code:\n");
                printAsmLine(NULL,modul,saddr,eaddr);
                printf("End of assembler dump.\n");
                return 0; 
            }
        }
    }
    else
    {
        if ( args > 1 )
            printf("Dump of assembler code from 0x%08x to 0x%08x:\n",saddr,eaddr);
        found = 0;
        while ( saddr < eaddr )
        {
            func = NULL;
            if (applyToSet(functions,funcInAddr,saddr,&func))
            {
                found = 1;
                modul = func->mod;
            }
            else
            {
                if ( found )
                    break;
                if (!applyToSet(modules,moduleLineWithAddr,saddr,&modul,NULL))
                    break;
            }
            saddr = printAsmLine(func,modul,saddr,eaddr) + 1;
        }
        if( saddr >= eaddr)
        {
            if ( args > 1 )
                printf("End of assembler dump.\n");
            return 0; 
        }
        
    }
    fprintf(stderr,"No function contains specified address.\n");
    return 0; 
}
/*-----------------------------------------------------------------*/
/* cmdDisasm1 - disassemble one asm instruction                    */
/*-----------------------------------------------------------------*/
int cmdDisasm1 (char *s, context *cctxt)
{
    return cmdDisasm( s, cctxt, 1);
}

/*-----------------------------------------------------------------*/
/* cmdDisasmF - disassemble asm instructions                       */
/*-----------------------------------------------------------------*/
int cmdDisasmF(char *s, context *cctxt)
{
    return cmdDisasm( s, cctxt, 2);
}

/*-----------------------------------------------------------------*/
/* cmdSetUserBp - set break point at the user specified location   */
/*-----------------------------------------------------------------*/
int cmdSetUserBp (char *s, context *cctxt)
{
    char *bp ;
    function *func = NULL;
	
    /* user break point location specification can be of the following
       forms
       a) <nothing>        - break point at current location
       b) lineno           - number of the current module
       c) filename:lineno  - line number of the given file
       e) filename:function- function X in file Y (useful for static functions)
       f) function         - function entry point
       g) *addr            - break point at address 
    */

    if (!cctxt) {
	fprintf(stdout,"No symbol table is loaded.  Use the \"file\" command.\n");
	return 0;
    }
    /* white space skip */
    while (*s && isspace(*s)) s++;
    
    /* null terminate it after stripping trailing blanks*/
    bp = s + strlen(s);
    while (bp != s && isspace(*bp)) bp--;
    *bp = '\0';

    /* case a) nothing */
    /* if nothing given then current location : we know
       the current execution location from the currentContext */
    if (! *s ) {

	/* if current context is known */
	if (cctxt->func) {
	    if (srcMode == SRC_CMODE)
		/* set the break point */
		setBreakPoint ( cctxt->addr , CODE , USER , userBpCB ,
				cctxt->func->mod->c_name, cctxt->cline);
	    else
		setBreakPoint ( cctxt->addr , CODE , USER , userBpCB ,
				cctxt->func->mod->asm_name, cctxt->asmline);
		
	}
	else
	    fprintf(stderr,"No default breakpoint address now.\n");
			
	goto ret ;
    }
    /* case g) *addr */
    if ( *s == '*' && isdigit(*(s+1)))
    {
        int  line   = 0;
        long braddr = strtol(s+1,0,0);
        if (!applyToSet(functions,funcInAddr,braddr,&func))
        {
            module *modul;
            if (!applyToSet(modules,moduleLineWithAddr,braddr,&modul,&line))
            {
                fprintf(stderr,"Address 0x%08x not exists in code.\n",braddr); 
            }
            else
            {
                setBreakPoint ( braddr , CODE , USER , userBpCB ,
                            modul->c_name,line);
            }
            goto ret ;
        }
		else
        {
            int line = func->exitline;
            applyToSet(func->cfpoints,lineNearAddr,braddr,&line,NULL,NULL);
            setBreakPoint ( braddr , CODE , USER , userBpCB ,
                            func->mod->c_name,line);
        }
	goto ret ;
    }
    /* case b) lineno */
    /* check if line number */
    if (isdigit(*s)) {
	/* get the lineno */
	int line = atoi(s);

	/* if current context not present then we must get the module
	   which has main & set the break point @ line number provided
	   of that module : if current context known then set the bp 
	   at the line number given for the current module 
	*/
	if (cctxt->func) {
	    if (!cctxt->func->mod) {
		if (!applyToSet(functions,funcWithName,"main"))
		    fprintf(stderr,"Function \"main\" not defined.\n");
		else 
		    setBPatModLine(func->mod,line);
	    } else 
		setBPatModLine(cctxt->func->mod,line);
	} else {
		if (list_mod) {
			setBPatModLine(list_mod,line);
		} else {
		  fprintf(stdout,"Sdcdb fails to have module symbol context at %d\n", __LINE__);
		}
	}
	
	goto ret;
    }

    if ((bp = strchr(s,':'))) {
	
	module *mod = NULL;
	*bp = '\0';
	
	if (srcMode == SRC_CMODE) {
	    if (!applyToSet(modules,moduleWithCName,s,&mod)) {
		fprintf (stderr,"No source file named %s.\n",s);
		goto ret;
	    }
	} else {
	    if (!applyToSet(modules,moduleWithAsmName,s,&mod)) {
		fprintf (stderr,"No source file named %s.\n",s);
		goto ret;
	    }
	}
	    	
	/* case c) filename:lineno */
	if (isdigit(*(bp +1))) {	    	    
	 
	    setBPatModLine (mod,atoi(bp+1));	    
	    goto ret;
	    
	}
	/* case d) filename:function */
	if (!applyToSet(functions,funcWithNameModule,bp+1,s,&func)) 
	    fprintf(stderr,"Function \"%s\" not defined.\n",bp+1); 
	else 	    
	    setBPatModLine (mod,
			    (srcMode == SRC_CMODE ? 
			     func->entryline :
			     func->aentryline));
	
	goto ret;
    }
            
    /* case e) function */
    if (!applyToSet(functions,funcWithName,s,&func))
	fprintf(stderr,"Function \"%s\" not defined.\n",s); 
    else
	setBPatModLine(func->mod,
		       (srcMode == SRC_CMODE ?
			func->entryline :
			func->aentryline));

 ret:    
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdListAsm - list assembler source code                         */
/*-----------------------------------------------------------------*/
int cmdListAsm (char *s, context *cctxt)
{
    if (  cctxt && cctxt->func) 
    {
        /* actual line */
        if (printAsmLine(cctxt->func,cctxt->func->mod,
                         (long)cctxt->addr,(long)cctxt->addr))
            return 0; 
    }
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdSetOption - set debugger options                             */
/*-----------------------------------------------------------------*/
int cmdSetOption (char *s, context *cctxt)
{
    while (*s && isspace(*s)) s++;
    if (strncmp(s,"srcmode",7) == 0 ) {
	if (srcMode == SRC_CMODE)
	    srcMode = SRC_AMODE;
	else
	    srcMode = SRC_CMODE;
	fprintf(stderr,"source mode set to '%s'\n", 
		(srcMode == SRC_CMODE ? "C" : "asm"));
	return 0;
    }

    if (strncmp(s,"variable ",9) == 0) 
    {
        symbol *sym ;
        char *val;
        s += 9;
        while (isspace(*s)) s++;
        if (!*s) return 0;

        val = s;
        while (*val && !isspace(*val) && *val != '=') val++;
        while (isspace(*val)) *val++ = '\0';
        if (*val) *val++ = '\0';
        if (*val)
        {
            if ((sym = symLookup(s,cctxt))) 
            {
                setSymValue(sym,val,cctxt);
                return 0;
            }   
            fprintf(stdout,"No symbol \"%s\" in current context.\n",s);
        }
        else
            fprintf(stdout,"No new value for \"%s\".\n",s);
        return 0;       
    }

 
    fprintf(stderr,"'set %s' command not yet implemented\n",s);
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdContinue - continue till next break point                    */
/*-----------------------------------------------------------------*/
int cmdContinue (char *s, context *cctxt)
{
    if (!cctxt || !cctxt->func) {
	fprintf(stdout,"The program is not being run.\n");
	return 0;
    }

    fprintf(stdout,"Continuing.\n");
    simGo(-1);
    showfull = 1;
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdDelUserBp - delete user break point                          */
/*-----------------------------------------------------------------*/
int cmdDelUserBp (char *s, context *cctxt)
{
    int bpnum ;
    while (isspace(*s)) s++;
    
    if (!*s ) {
	if (userBpPresent) {
	    char buffer[10];
	    fprintf (stdout,"Delete all breakpoints? (y or n) ");
	    fflush(stdout);
	    fgets(buffer,sizeof(buffer),stdin);
	    if (toupper(buffer[0]) == 'Y')
		deleteUSERbp(-1);	   
	}
	return 0;
    }
    
    /* determine the break point number */
    if (sscanf(s,"%d",&bpnum) == 1)
	deleteUSERbp(bpnum);

    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdStep - single step thru C source file                        */
/*-----------------------------------------------------------------*/
int cmdStep (char *s, context *cctxt)
{
    function *func = NULL;

    if (!cctxt || !cctxt->func || !cctxt->func->mod) 
	fprintf(stdout,"The program is not being run.\n");
    else {
    int origSrcMode = srcMode;
    if ( *s == 'i' )
        srcMode = SRC_AMODE;
	/* if we are @ the end of a function then set
	   break points at execution points of the
	   function in the call stack... */
	if (cctxt->addr == cctxt->func->sym->eaddr) {
	    if ((func = STACK_PEEK(callStack))) {
		if (srcMode == SRC_CMODE)
		    applyToSet (func->cfpoints,setStepEPBp,STEP,
				func->mod->c_name);	
		else
		    applyToSet (func->afpoints,setStepEPBp,STEP,
				func->mod->asm_name);
	    }
	} else {
	    /* set breakpoints at all function entry points
	       and all exepoints of this functions & for
	       all functions one up in the call stack */
	    
	    /* all function entry points */
	    applyToSet(functions,setStepBp); 
	    
	    if (srcMode == SRC_CMODE) {
		/* for all execution points in this function */
		applyToSet(cctxt->func->cfpoints,setStepEPBp,STEP,
			   cctxt->func->mod->c_name);
		
		/* set a break point @ the current function's
		   exit */
		setBreakPoint (cctxt->func->sym->eaddr, CODE, STEP , 
			       stepBpCB, cctxt->func->mod->c_name, 
			       cctxt->func->exitline);
		
		/* now break point @ callers execution points */
		if ((func = STACK_PPEEK(callStack))) {
		    applyToSet (func->cfpoints,setStepEPBp,STEP,
				func->mod->c_name);	
		    /* set bp @ callers exit point */
		    setBreakPoint (func->sym->eaddr, CODE, STEP , 
				   stepBpCB, func->mod->c_name, 
				   func->exitline);
		}
	    } else {
		/* for all execution points in this function */
		applyToSet(cctxt->func->afpoints,setStepEPBp,STEP,
			   cctxt->func->mod->asm_name);
		
		/* set a break point @ the current function's
		   exit */
		setBreakPoint (cctxt->func->sym->eaddr, CODE, STEP , 
			       stepBpCB, cctxt->func->mod->asm_name, 
			       cctxt->func->aexitline);
		
		/* now break point @ callers execution points */
		if ((func = STACK_PPEEK(callStack))) {
		    
		    applyToSet (func->afpoints,setStepEPBp,STEP,
				func->mod->asm_name);	
		    
		    /* set bp @ callers exit point */
		    setBreakPoint (func->sym->eaddr, CODE, STEP , 
				   stepBpCB, func->mod->asm_name, 
				   func->aexitline);
		}
	    }
	}

    srcMode = origSrcMode;
	simGo(-1);
    showfull = 1;
    }
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdNext - next executable C statement file                      */
/*-----------------------------------------------------------------*/
int cmdNext (char *s, context *cctxt)
{
    function *func = NULL;
    /* next is almost the same as step except we don't
       we don't set break point for all function entry
       points */
    if (!cctxt || !cctxt->func || !cctxt->func->mod) 
	fprintf(stdout,"The program is not being run.\n");
    else {
    int origSrcMode = srcMode;
    if ( *s == 'i' )
        srcMode = SRC_AMODE;
	
	/* if we are @ the end of a function then set
	   break points at execution points of the
	   function in the call stack... */
	if (cctxt->addr == cctxt->func->sym->eaddr) {
	    if ((func = STACK_PEEK(callStack))) {
		if (srcMode == SRC_CMODE)
		    applyToSet (func->cfpoints,setStepEPBp,STEP,
				func->mod->c_name);	
		else
		    applyToSet (func->afpoints,setStepEPBp,STEP,
			       func->mod->asm_name);
	    }
	} else {
	    if (srcMode == SRC_CMODE) {
		/* for all execution points in this function */
		applyToSet(cctxt->func->cfpoints,setNextEPBp,NEXT,
			   cctxt->func->mod->c_name);
		/* set a break point @ the current function's
		   exit */
		setBreakPoint (cctxt->func->sym->eaddr, CODE, NEXT , 
			       nextBpCB, cctxt->func->mod->c_name, 
			       cctxt->func->exitline);
		
		/* now break point @ callers execution points */	
		if ((func = STACK_PPEEK(callStack))) {
		    applyToSet (func->cfpoints,setNextEPBp,NEXT ,
				func->mod->c_name);	
		    /* set bp @ callers exit point */
		    setBreakPoint (func->sym->eaddr, CODE, NEXT , 
				   stepBpCB, func->mod->c_name, 
				   func->exitline);
		}
	    } else {
		/* for all execution points in this function */
		applyToSet(cctxt->func->afpoints,setNextEPBp,NEXT,
			   cctxt->func->mod->asm_name);
		/* set a break point @ the current function's
		   exit */
		setBreakPoint (cctxt->func->sym->eaddr, CODE, NEXT , 
			       nextBpCB, cctxt->func->mod->asm_name, 
			       cctxt->func->aexitline);
		
		/* now break point @ callers execution points */	
		if ((func = STACK_PPEEK(callStack))) {
		    applyToSet (func->cfpoints,setNextEPBp,NEXT ,
				func->mod->asm_name);	
		    /* set bp @ callers exit point */
		    setBreakPoint (func->sym->eaddr, CODE, NEXT , 
				   stepBpCB, func->mod->asm_name, 
				   func->aexitline);
		}
	    }
        srcMode = origSrcMode;
	    simGo(-1);	
        showfull = 1;
	}
    srcMode = origSrcMode;
    }    
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdRun  - run till next break point                             */
/*-----------------------------------------------------------------*/
int cmdRun (char *s, context *cctxt)
{
    char buff[10];
    if (!cctxt || !cctxt->func || !cctxt->func->mod) {
	fprintf(stdout,"Starting program\n");
    if ( ! simactive )
    {
        fprintf(stdout,"No executable file specified.\nUse the \"file\" command.\n");
        return 0;
    }
	simGo(0);
    } else {
	
	fprintf(stdout,
		"The program being debugged has been started already.\n");
	fprintf(stdout,"Start it from the beginning? (y or n) ");
	fflush(stdout);

	fgets(buff,sizeof(buff),stdin);
	if (toupper(buff[0]) == 'Y') {
	    simReset();
	    simGo(0);
	}
    }
    showfull = 1;
    return 0;
}

/*-----------------------------------------------------------------
 cmdListSymbols - list symbols
|-----------------------------------------------------------------*/
int cmdListSymbols (char *s, context *cctxt)
{
    int our_verbose = 0;
    symbol *sy;
    int i;

    if (strstr(s, "v1")) {
      our_verbose = 1;
    } else if (strstr(s, "v2")) {
      our_verbose = 2;
    }

    printf("[symbols]\n");
    sy = setFirstItem(symbols);
    i = 0;
    for (;;) {
      if (sy == NULL)
        break;
      if (our_verbose <= 1)
        printf("<%s>", sy->name);

      if (our_verbose > 1) {
        printf("  %d) name:%s, size:%d, level:%d block:%d\n", i,
          sy->name, sy->size, sy->level, sy->block);
        printf("    isonstack:%d, isfunc:%d, offset:%d addr:%d\n",
          sy->isonstack, sy->isfunc, sy->offset, sy->addr);
        printf("    eaddr:%d, addr_type:%c, type:%x etype:%x\n",
          sy->eaddr, sy->addr_type, sy->type, sy->etype);
        printf("    scopetype:%c, sname:%s, rname:%s addrspace:%c\n",
          sy->scopetype, sy->sname, sy->rname, sy->addrspace);
        printf("    next:%x\n", sy->next);
      }
      ++i;
      sy = setNextItem(symbols);
    }
    printf("   %d symbols\n", i);
    return 0;
}

/*-----------------------------------------------------------------
 cmdListFunctions - list functions.
|-----------------------------------------------------------------*/
int cmdListFunctions (char *s, context *cctxt)
{
    function *f;
    int i;
    int our_verbose = 0;

    if (strstr(s, "v1")) {
      our_verbose = 1;
    } else if (strstr(s, "v2")) {
      our_verbose = 2;
    }

    printf("[functions]\n");
    f = setFirstItem(functions);
    i = 0;
    for (;;) {
      if (f == NULL)
        break;
      if (our_verbose) {
        printf("  %d) sym:%x, fname:%s, modName:%s, mod:%x\n", i,
          f->sym, f->sym->name, f->modName, f->mod);
        printf("    entryline:%d, aentryline:%d, exitline:%d, aexitline:%d\n",
                f->entryline, f->aentryline, f->exitline, f->aexitline);
        printf("    cfpoints:%x, afpoints:%x, laddr:%x, lline:%d\n",
                f->cfpoints, f->afpoints, f->laddr, f->lline);
      }
      else {
        printf("<%s>", f->modName);
      }
      ++i;
      f = setNextItem(functions);
    }
    printf("   %d functions\n", i);
    return 0;
}

/*-----------------------------------------------------------------
 cmdListModules - list functions.
|-----------------------------------------------------------------*/
int cmdListModules (char *s, context *cctxt)
{
    module *m;
    srcLine *cs, *as;
    int i, mi;
    int our_verbose = 0;

    if (strstr(s, "v1")) {
      our_verbose = 1;
    } else if (strstr(s, "v2")) {
      our_verbose = 2;
    }

    printf("[modules]\n");
    m = setFirstItem(modules);
    mi = 0;
    for (;;) {
      if (m == NULL)
        break;

      if (our_verbose >= 0) {
      printf("  %d) cfullname:%s, afullname:%s, name:%s\n", ++mi,
        m->cfullname, m->afullname, m->name);
      printf("    c_name:%s, asm_name:%s, ncLines:%d, nasmLines:%d\n",
              m->c_name, m->asm_name, m->ncLines, m->nasmLines);
      printf("    cLines:%x, asmLines:%x\n",
              m->cLines, m->asmLines);
      }
      if (our_verbose >= 2) {
        if (m->ncLines) {
          printf("    [cLines] ");
          if ( our_verbose)
          for (i=0; i<m->ncLines; i++ ) {
              cs = m->cLines[i];
              printf("   (%d) addr:%x, block:%d, level:%d, src:%s\n",
                 i, cs->addr, cs->block, cs->level, cs->src);
          }
          if (!our_verbose)
              printf("%d records", i);
        }
        if (m->nasmLines) {
          printf("    [asmLines] ");
          if ( our_verbose)
          for (i=0; i<m->nasmLines; i++ ) {
              as = m->asmLines[i];
              printf("   (%d) addr:%x, block:%d, level:%d, src:%s\n",
                 i, as->addr, as->block, as->level, as->src);
          }
          if (!our_verbose)
              printf("%d records", i);
        }
        printf("\n");
      }

      m = setNextItem(modules);
    }
    return 0;
}

/*-----------------------------------------------------------------
 infoSymbols - This is really just a tool to dump all these
   huge program structures out into human readable form.
|-----------------------------------------------------------------*/
static void infoSymbols(context *ctxt)
{
  int our_verbose = 0;

  printf("[context:%x] func:%x modName:%s addr:%x\n",
    ctxt, ctxt->func, ctxt->modName, ctxt->addr);

  printf("  cline:%d asmline:%d block:%d level:%d\n",
    ctxt->cline, ctxt->asmline, ctxt->level);

  printf("[globals] currCtxt:%x, modules:%x, functions:%x symbols:%x\n",
    currCtxt, modules, functions, symbols);
  printf("  nStructs:%d, structs:%x, ssdirl:%s\n",
    nStructs, structs, ssdirl);

  /**************** modules *******************/
  {
    module *m;
    srcLine *cs, *as;
    int i, mi;
    printf("[modules]\n");
    m = setFirstItem(modules);
    mi = 0;
    for (;;) {
      if (m == NULL)
        break;
      printf("  %d) cfullname:%s, afullname:%s, name:%s\n", ++mi,
        m->cfullname, m->afullname, m->name);
      printf("    c_name:%s, asm_name:%s, ncLines:%d, nasmLines:%d\n",
              m->c_name, m->asm_name, m->ncLines, m->nasmLines);
      printf("    cLines:%x, asmLines:%x\n",
              m->cLines, m->asmLines);
      i = 0;
      if (m->cLines) {
        cs = m->cLines[i++];
        printf("    [cLines] ");
        while (cs) {
          if (our_verbose)
            printf("   (%d) addr:%x, block:%d, level:%d, src:%s\n",
               i, cs->addr, cs->block, cs->level, cs->src);
          cs = m->cLines[i++];
        }
        if (!our_verbose)
            printf("%d records", i);
      }
      i = 0;
      if (m->asmLines) {
        as = m->asmLines[i++];
        printf("    [asmLines] ");
        while (as) {
          if (our_verbose)
            printf("   (%d) addr:%x, block:%d, level:%d, src:%s\n",
               i, as->addr, as->block, as->level, as->src);
          as = m->asmLines[i++];
        }
        if (!our_verbose)
            printf("%d records", i);
      }
      printf("\n");

      m = setNextItem(modules);
    }
  }

  /**************** functions *******************/
  {
    function *f;
    int i;
    printf("[functions]\n");
    f = setFirstItem(functions);
    i = 0;
    for (;;) {
      if (f == NULL)
        break;
      if (our_verbose) {
        printf("  %d) sym:%x, modName:%s, mod:%x\n", i,
          f->sym, f->modName, f->mod);
        printf("    entryline:%d, aentryline:%d, exitline:%d, aexitline:%d\n",
                f->entryline, f->aentryline, f->exitline, f->aexitline);
        printf("    cfpoints:%x, afpoints:%x, laddr:%x, lline:%d\n",
                f->cfpoints, f->afpoints, f->laddr, f->lline);
      }
      ++i;
      f = setNextItem(functions);
    }
    if (!our_verbose)
      printf("   %d functions\n", i);
  }

  /**************** symbols *******************/
  {
    symbol *s;
    int i;
    printf("[symbols]\n");
    s = setFirstItem(symbols);
    i = 0;
    for (;;) {
      if (s == NULL)
        break;
      if (our_verbose) {
        printf("  %d) name:%s, size:%d, level:%d block:%d\n", i,
          s->name, s->size, s->level, s->block);
        printf("    isonstack:%d, isfunc:%d, offset:%d addr:%d\n",
          s->isonstack, s->isfunc, s->offset, s->addr);
        printf("    eaddr:%d, addr_type:%c, type:%x etype:%x\n",
          s->eaddr, s->addr_type, s->type, s->etype);
        printf("    scopetype:%c, sname:%s, rname:%s addrspace:%c\n",
          s->scopetype, s->sname, s->rname, s->addrspace);
        printf("    next:%x\n", s->next);
      }
      ++i;
      s = setNextItem(symbols);
    }
    if (!our_verbose)
      printf("   %d symbols\n", i);
  }

}

/*-----------------------------------------------------------------*/
/* infoStack - print call stack information                        */
/*-----------------------------------------------------------------*/
static void infoStack(context *ctxt)
{
    function *func ;
    int i = 0 ;

    STACK_STARTWALK(callStack) ;
    while ((func = STACK_WALK(callStack))) {

	fprintf(stdout,"#%d 0x%04x %s () at %s:%d\n",i++,
		func->laddr,func->sym->name,
		func->mod->c_name,func->lline);
    }

}

/*-----------------------------------------------------------------*/
/* cmdWhere -  where command                                       */
/*-----------------------------------------------------------------*/
int cmdWhere(char *s, context *cctxt)
{
	infoStack(cctxt);
    showfull = 1;
	return 0;
}

/*-----------------------------------------------------------------*/
/* cmdUp -  Up command                                             */
/*-----------------------------------------------------------------*/
int cmdUp(char *s, context *cctxt)
{
	return 0;
}

/*-----------------------------------------------------------------*/
/* cmdDown - down command                                          */
/*-----------------------------------------------------------------*/
int cmdDown(char *s, context *cctxt)
{
	return 0;
}

static int infomode = 0;
/*-----------------------------------------------------------------*/
/* cmdInfo - info command                                          */
/*-----------------------------------------------------------------*/
int cmdInfo (char *s, context *cctxt)
{
    while (isspace(*s)) s++;

    /* list all break points */
    if (strncmp(s,"break",5) == 0) {
	listUSERbp();
	return 0;
    }

    /* info frame same as frame */
    if (strcmp(s,"frame") == 0) {
	cmdFrame (s,cctxt);
    showfull = 1;
	return 0;
    }

    if (strncmp(s,"line",4) == 0) {
    infomode=1;
	cmdListSrc (s+4,cctxt);
	return 0;
    }

    /* info stack display call stack */
    if (strcmp(s,"stack") == 0) {
	infoStack(cctxt);
    showfull = 1;
	return 0;
    }

    /* info stack display call stack */
    if (strcmp(s,"registers") == 0) {
	fprintf(stdout,"%s",simRegs());
	return 0;
    }

    /* info stack display call stack */
    if (strcmp(s,"all-registers") == 0) {
	fprintf(stdout,"%s",simRegs());
	fprintf(stdout,"Special Function Registers:\n");
    sendSim("ds 0x80 0x100\n");
    waitForSim(100,NULL);
    fprintf(stdout,simResponse());
	return 0;
    }

    /* info stack display call stack */
    if (strcmp(s,"symbols") == 0) {
      /* dump out symbols we have read in */
      fprintf(stdout,"Dumping symbols...\n");
      infoSymbols(cctxt);
      return 0;
    }

    if (strcmp(s,"variables") == 0) {
      /* dump out symbols we have read in */
      fprintf(stdout,"Dumping symbols...\n");
      infoSymbols(cctxt);
      return 0;
    }

    fprintf(stdout,"Undefined info command: \"%s\".  Try \"help\n",s);
    return 0;

}

/*-----------------------------------------------------------------*/
/* cmdQuit  - quit debugging                                       */
/*-----------------------------------------------------------------*/
int cmdQuit (char *s, context *cctxt)
{   
    if (simactive)
	closeSimulator();
    return 1;
}

/*-----------------------------------------------------------------*/
/* cmdListSrc  - list src                                          */
/*-----------------------------------------------------------------*/
int cmdListSrc (char *s, context *cctxt)
{   
    static int currline = 0;
    int i =0 ;
    int pline = 0;
    int llines = listLines;

    while (*s && isspace(*s)) s++;
    
    /* if the user has spcified line numer then the line number
       can be of the following formats
       LINE          - just line number
       FILE:LINE     - filename line number
       FUNCTION      - list a function
       FILE:FUNCTION - function in file */

    if (*s) {
	/* case a) LINE */
	if (isdigit(*s)) {
	    sscanf(s,"%d",&pline);
	    if (!cctxt || !cctxt->func || !cctxt->func->mod) {
	      if (!list_mod) {
	        fprintf(stdout,"Sdcdb fails to have a proper context at %d.\n", __LINE__);
	        return 0;
	      }
	    }
	    else
	      list_mod = cctxt->func->mod;
	}
	else {
	    char *bp;
	    function *func = NULL;
	    
	    /* if ':' present then FILE:LINE || FILE:FUNCTION */
	    if ((bp = strchr(s,':'))) {
		*bp = '\0';
		bp ++;
		if (isdigit(*bp)) {
		    /* FILE:LINE */
		    list_mod=NULL;  /* bug fix 2-09-02, moduleWithCName expects mod to be null */
		    if (srcMode == SRC_CMODE) {
			if (!applyToSet(modules,moduleWithCName,s,&list_mod)) {
			    fprintf (stderr,"No c source file named %s.\n",s);
			    return 0;
			}
		    } else {
			if (!applyToSet(modules,moduleWithAsmName,s,&list_mod)) {
			    fprintf (stderr,"No source file named %s.\n",s);
			    return 0;
			}
		    }
		    sscanf(bp,"%d",&pline);
		} else {
		    /* FILE:FUCTION */
		    if (!applyToSet(functions,funcWithNameModule,bp,s,&func)) {
			fprintf(stdout,"Function \"%s\" not defined.\n",bp);
			return 0;
		    }
		    list_mod = func->mod;
		    if (srcMode == SRC_CMODE) {
			pline = func->entryline;
			llines = func->exitline - func->entryline + 1;
		    } else {
			pline = func->aentryline;
			llines = func->aexitline - func->aentryline + 1;
		    }
		}
	    }
	    else {
		/* FUNCTION */
		if (!applyToSet(functions,funcWithName,s,&func)) {
		    fprintf(stderr,"Function \"%s\" not defined.\n",s); 
		    return 0;
		}
		else {
		    list_mod = func->mod;
		    if (srcMode == SRC_CMODE) {
			pline = func->entryline;
			llines = func->exitline - func->entryline + 1; 
		    } else {
			pline = func->aentryline;
			llines = func->aexitline - func->aentryline + 1; 
		    }
		}
	    }
	}
    } else {
	/* if no line specified & we had listed
	   before then continue from that listing */
	if (currline)
	    pline = currline ;
	else {
	    if (!cctxt || !cctxt->func || !cctxt->func->mod) {
	      fprintf(stdout,"Missing context at %d. Try list filename:lineno\n", __LINE__);
	      return 0;
	    }
	    list_mod = cctxt->func->mod;
	    if (srcMode == SRC_CMODE)
		pline = cctxt->cline;
	    else
		pline = cctxt->asmline;
	}
    }

    if (!list_mod) {
      fprintf(stdout,"Sdcdb fails to have a valid module context at %d.\n", __LINE__);
      return 0;
    }
    
    if (llines > listLines) llines = listLines;

    if ( infomode )
    {
	    fprintf(stdout,"Line %d of \"%s\" starts at address 0x%08x.\n",pline,
		    list_mod->c_name, list_mod->cLines[pline]->addr);
        infomode=0;
        return 0;
    }
    for ( i = 0 ; i < llines ; i++ ) {
	if (srcMode == SRC_CMODE) {
	    if ( (pline + i) >= list_mod->ncLines )
		break;
	    fprintf(stdout,"%d\t%s",pline + i,
		    list_mod->cLines[pline +i]->src);
	} else {
	    if ( (pline + i) >= list_mod->nasmLines )
		break;
	    fprintf(stdout,"%d\t%s",pline + i,
		    list_mod->asmLines[pline +i]->src);
	}
    }
    currline = pline + i ;
    return 0;
}

static void setValBasic(symbol *sym, char *val)
{
    union 
    {  	
        float f;     
        unsigned long val;
        long         sval;
        struct {
            short    lo;
            short    hi;
        } i;
        unsigned char b[4];
    }v;

    if (IS_FLOAT(sym->type)) 	
        v.f = strtof(val,NULL);    
    else
	if (IS_PTR(sym->type))
	    v.sval = strtol(val,NULL,0);
	else
    {
	    if (IS_SPEC(sym->type) && IS_INTEGRAL(sym->type)) 
        {
            if (IS_CHAR(sym->etype))
            { 
                v.b[0] = val[0];
            }
            else
                if (IS_INT(sym->etype)) 
                    if (IS_LONG(sym->etype))
                        if (SPEC_USIGN(sym->etype))
                            v.val = strtol(val,NULL,0);
                        else
                            v.sval = strtol(val,NULL,0);
                    else
                        v.i.lo = strtol(val,NULL,0);
                else
                    v.sval = strtol(val,NULL,0);
	    } 
        else
            v.sval = strtol(val,NULL,0);
    }
    simSetValue(sym->addr,sym->addrspace,sym->size,v.sval);	
}

/*-----------------------------------------------------------------*/
/* printValBasic - print value of basic types                      */
/*-----------------------------------------------------------------*/
static void printValBasic(symbol *sym,unsigned addr,char mem, int size)
{
    union {  	
	float f;     
	unsigned long val;
	long         sval;
	struct {
	    short    lo;
	    short    hi;
	} i;
	unsigned char b[4];
    }v;
    union {
	unsigned char b[4];
    }v1;
    
    v.val = simGetValue(addr,mem,size);
    /* if this a floating point number then */
    if (IS_FLOAT(sym->type)) 	
	fprintf(stdout,"%f",v.f);    
    else
	if (IS_PTR(sym->type))
	    fprintf(stdout,"0x%x",v.val);
	else
	    if (IS_SPEC(sym->type) && IS_INTEGRAL(sym->type)) {
		if (IS_CHAR(sym->etype))
        { 
		    if ( isprint(v.val))
                fprintf(stdout,"%d 0x%x '%c'",v.val,v.val,v.val);
            else
                fprintf(stdout,"%d 0x%x '\\%o'",v.val,v.val,v.val);
		}
        else
		    if (IS_INT(sym->etype)) 
			if (IS_LONG(sym->etype))
			    if (SPEC_USIGN(sym->etype))
				fprintf(stdout,"%d 0x%x",v.val,v.val);
			    else
				fprintf(stdout,"%d 0x%x",v.sval,v.sval);
			else
			    fprintf(stdout,"%d 0x%x",v.i.lo,v.i.lo);
		    else
			fprintf(stdout,"0x%x",v.val);
	    } else
		fprintf(stdout,"0x%x",v.val);
		
    
}

/*-----------------------------------------------------------------*/
/* printValFunc  - prints function values                          */
/*-----------------------------------------------------------------*/
static void printValFunc (symbol *sym)
{
    fprintf(stdout,"print function not yet implemented");
}

/*-----------------------------------------------------------------*/
/* printArrayValue - will print the values of array elements       */
/*-----------------------------------------------------------------*/
static void printArrayValue (symbol *sym, char space, unsigned int addr)
{
	link *elem_type = sym->type->next;
	int i;
	
	fprintf(stdout," { ");
	for (i = 0 ; i < DCL_ELEM(sym->type) ; i++) {		
		if (IS_AGGREGATE(elem_type)) {
			printValAggregates(sym,elem_type,space,addr);		       
		} else {
			printValBasic(sym,addr,space,getSize(elem_type));
		}
		addr += getSize(elem_type);
		if (i != DCL_ELEM(sym->type) -1)
			fprintf(stdout,",");
	}

	fprintf(stdout,"}");		
}

/*-----------------------------------------------------------------*/
/* printStructValue - prints structures elements                   */
/*-----------------------------------------------------------------*/
static void printStructValue (symbol *sym,link *type, char space, unsigned int addr) 
{
	symbol *fields = SPEC_STRUCT(type)->fields;

	fprintf(stdout," { ");
	while (fields) {
		fprintf(stdout,"%s = ",fields->name);
		if (IS_AGGREGATE(fields->type)) {
			printValAggregates(fields,fields->type,space, addr);
		} else {
			printValBasic(fields,addr,space,getSize(fields->type));
		}
		addr += getSize(fields->type);
		fields = fields->next;
	}
	fprintf(stdout,"}");
}

/*-----------------------------------------------------------------*/
/* printValAggregates - print value of aggregates                  */
/*-----------------------------------------------------------------*/
static void printValAggregates (symbol *sym, link *type,char space,unsigned int addr)
{

	if (IS_ARRAY(type)) {
		printArrayValue(sym, space, addr);
		return ;
	}

 	if (IS_STRUCT(type)) { 
 		printStructValue(sym,sym->type,space, addr); 
 		return; 
 	} 
}

/*-----------------------------------------------------------------*/
/* setSymValue - set     value of a symbol                         */
/*-----------------------------------------------------------------*/
static void setSymValue(symbol *sym, char *val, context *cctxt)
{
    if (sym->isonstack) 
    {
        symbol *bp = symLookup("bp",cctxt);
        if (!bp) 
        {
            fprintf(stdout,"cannot determine stack frame\n");
            return ;
        }

        sym->addr = simGetValue(bp->addr,bp->addrspace,bp->size)
            + sym->offset ;      
    }
    /* arrays & structures first */
    if (IS_AGGREGATE(sym->type))
	{
    }
    else
	/* functions */
	if (IS_FUNC(sym->type))
    {
    }
	else
    {
        setValBasic(sym,val);
    }
}

/*-----------------------------------------------------------------*/
/* printSymValue - print value of a symbol                         */
/*-----------------------------------------------------------------*/
static void printSymValue (symbol *sym, context *cctxt, int flg, int dnum)
{
    static int stack = 1;
    unsigned long val;
    /* if it is on stack then compute address & fall thru */
    if (sym->isonstack) {
	symbol *bp = symLookup("bp",cctxt);
	if (!bp) {
	    fprintf(stdout,"cannot determine stack frame\n");
	    return ;
	}

	sym->addr = simGetValue(bp->addr,bp->addrspace,bp->size)
	  + sym->offset ;      
    }
    
    /* get the value from the simulator and
       print it */
    switch (flg)
    {
        case 0: 
        default:
            break;
        case 1: 
            fprintf(stdout,"$%d = ",stack++);
            break;
        case 2: 
            fprintf(stdout,"%d: %s = ",dnum,sym->name);
            break;
    }
    /* arrays & structures first */
    if (IS_AGGREGATE(sym->type))
	printValAggregates(sym,sym->type,sym->addrspace,sym->addr);
    else
	/* functions */
	if (IS_FUNC(sym->type))
	    printValFunc(sym);
	else 
	    printValBasic(sym,sym->addr,sym->addrspace,sym->size);
    if ( flg > 0 ) fprintf(stdout,"\n");
	    //fprintf(stdout,"(BASIC %x,%c,%d)\n",sym->addr,sym->addrspace,sym->size);
	
}

/*-----------------------------------------------------------------*/
/* printStructInfo - print out structure information               */
/*-----------------------------------------------------------------*/
static void printStructInfo (structdef *sdef)
{
    symbol *field = sdef->fields ;
    int i = 0 ;
    
    while (field) {
	i += field->offset;
	field = field->next;
    }

    fprintf(stdout,"%s %s {\n",(i ? "struct" : "union" ), sdef->tag);
    field = sdef->fields;
    while (field) {
	printTypeInfo (field->type);
	fprintf(stdout," %s ;\n",field->name);
	field = field->next ;
    }

    fprintf(stdout,"}\n");

}

/*-----------------------------------------------------------------*/
/* printTypeInfo - print out the type information                  */
/*-----------------------------------------------------------------*/
static void printTypeInfo(link *p)
{
    if (!p)
	return ;

    if (IS_DECL(p)) {
	switch (DCL_TYPE(p))  {
	case FUNCTION:
	    printTypeInfo (p->next);
	    fprintf(stdout,"()");
	    break;
	case ARRAY:
	    printTypeInfo (p->next);
	    fprintf(stdout,"[%d]",DCL_ELEM(p));
	    break;
	
	case IPOINTER:
	case PPOINTER:
	case POINTER:
	    printTypeInfo (p->next);
	    fprintf(stdout,"(_near *)");
	    break;

	case FPOINTER:
	    printTypeInfo (p->next);
	    fprintf(stdout,"(_xdata *)");
	    break;

	case CPOINTER:
	    printTypeInfo( p->next);
	    fprintf(stdout,"(_code *)");
	    break;
	    
	case GPOINTER:
	    printTypeInfo( p->next);
	    fprintf(stdout,"(_generic *)");
	    break;	             
	}
    } else {
	switch (SPEC_NOUN(p)) { /* depending on the specifier type */
	case V_INT:
	    (IS_LONG(p) ? fputs("long ",stdout) : 
	     ( IS_SHORT(p) ? fputs("short ",stdout) : 
	       fputs("int ",stdout))) ;
	    break;
	case V_FLOAT:
	     fputs("float ",stdout);
	     break;

	case V_CHAR:
	    fputs ("char ",stdout);
	    break;

	case V_VOID:
	    fputs("void ",stdout);
	    break;

	case V_STRUCT:
	    printStructInfo (SPEC_STRUCT(p));
	    break;

	case V_SBIT:
	    fputs("sbit ",stdout);
	    break;

	case V_BIT:
	    fprintf(stdout,": %d" ,SPEC_BLEN(p));       
	    break;
	}
    }
}

/*-----------------------------------------------------------------*/
/* cmdPrint - print value of variable                              */
/*-----------------------------------------------------------------*/
int cmdPrint (char *s, context *cctxt)
{   
    symbol *sym ;
    char *bp = s+strlen(s) -1;

    while (isspace(*s)) s++;
    if (!*s) return 0;
    while (isspace(*bp)) bp--;
    bp++ ;
    *bp = '\0';

    if ((sym = symLookup(s,cctxt))) {
	printSymValue(sym,cctxt,1,0);
    } else {
	fprintf(stdout,
		"No symbol \"%s\" in current context.\n",	       
		s);
    }
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdOutput - print value of variable without number and newline  */
/*-----------------------------------------------------------------*/
int cmdOutput (char *s, context *cctxt)
{   
    symbol *sym ;
    char *bp = s+strlen(s) -1;

    while (isspace(*s)) s++;
    if (!*s) return 0;
    while (isspace(*bp)) bp--;
    bp++ ;
    *bp = '\0';

    if ((sym = symLookup(s,cctxt))) {
	printSymValue(sym,cctxt,0,0);
    } else {
	fprintf(stdout,
		"No symbol \"%s\" in current context.",	       
		s);
    }
    return 0;
}

typedef struct _dsymbol
{
    char *name;
    int  dnum;
} dsymbol;

/** find display entry with this number */

DEFSETFUNC(dsymWithNumber)
{
    dsymbol *dsym = item;
    V_ARG(int , dnum);
    V_ARG(dsymbol **,dsymp);

    if ( dsym->dnum == dnum )
    {
        *dsymp = dsym;
        return 1;
    }
    return 0;
}

/*-----------------------------------------------------------------*/
/* displayAll  - display all valid variables                       */
/*-----------------------------------------------------------------*/
void displayAll(context *cctxt)
{
    dsymbol *dsym;
    symbol  *sym;
    if ( !dispsymbols )
        return;
    for (dsym = setFirstItem(dispsymbols);
         dsym ;
         dsym = setNextItem(dispsymbols)) 
    {
        if ( (sym = symLookup(dsym->name,cctxt)))
            printSymValue(sym,cctxt,2,dsym->dnum);            
    }
}

/*-----------------------------------------------------------------*/
/* cmdDisplay  - display value of variable                         */
/*-----------------------------------------------------------------*/
int cmdDisplay (char *s, context *cctxt)
{   
    symbol *sym ;
    char *bp = s+strlen(s) -1;
    static int dnum = 1;

    while (isspace(*s)) s++;
    if (!*s)
    {
        displayAll(cctxt);
        return 0;
    }
    while (isspace(*bp)) bp--;
    bp++ ;
    *bp = '\0';

    if ((sym = symLookup(s,cctxt))) 
    {
        dsymbol *dsym = (dsymbol *)Safe_calloc(1,sizeof(dsymbol));
        dsym->dnum = dnum++ ;
        dsym->name = sym->name;
        addSetHead(&dispsymbols,dsym);
    }
    else
    {
        fprintf(stdout,"No symbol \"%s\" in current context.\n",	       
                s);
    }
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdUnDisplay  - undisplay value of variable                              */
/*-----------------------------------------------------------------*/
int cmdUnDisplay (char *s, context *cctxt)
{   
    dsymbol *dsym;
    int dnum;

    while (isspace(*s)) s++;
    if (!*s)
    {
        deleteSet(&dispsymbols);
        return 0;
    }
    while ( s && *s )
    {
        dnum = strtol(s,&s,10);
        if (applyToSetFTrue(dispsymbols,dsymWithNumber,dnum,&dsym)) 
        {
            deleteSetItem(&dispsymbols,dsym);
        } 
        else
        {
            fprintf(stdout,"Arguments must be display numbers.\n");    
        }
    }
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdPrintType - print type of a variable                         */
/*-----------------------------------------------------------------*/
int cmdPrintType (char *s, context *cctxt)
{   
        symbol *sym ;
    char *bp = s+strlen(s) -1;

    while (isspace(*s)) s++;
    if (!*s) return 0;
    while (isspace(*bp)) bp--;
    bp++ ;
    *bp = '\0';

    if ((sym = symLookup(s,cctxt))) {
	printTypeInfo(sym->type);
	fprintf(stdout,"\n");
    } else {
	fprintf(stdout,
		"No symbol \"%s\" in current context.\n",
		s);
    }
    return 0;   
}

/*-----------------------------------------------------------------*/
/* cmdClrUserBp - clear user break point                           */
/*-----------------------------------------------------------------*/
int cmdClrUserBp (char *s, context *cctxt)
{   
    char *bp ;    
    function *func = NULL;
	
    /* clear break point location specification can be of the following
       forms
       a) <nothing>        - break point at current location
       b) lineno           - number of the current module
       c) filename:lineno  - line number of the given file
       e) filename:function- function X in file Y (useful for static functions)
       f) function         - function entry point
    */

    if (!cctxt) {
	fprintf(stdout,"No symbol table is loaded.  Use the \"file\" command.\n");
	return 0;
    }

    /* white space skip */
    while (*s && isspace(*s)) s++;
    
    /* null terminate it after stripping trailing blanks*/
    bp = s + strlen(s);
    while (bp != s && isspace(*bp)) bp--;
    *bp = '\0';

    /* case a) nothing */
    /* if nothing given then current location : we know
       the current execution location from the currentContext */
    if (! *s ) {

	/* if current context is known */
	if (cctxt->func) 
	    /* clear the break point @ current location */
	    clearUSERbp (cctxt->addr);
	else
	    fprintf(stderr,"No default breakpoint address now.\n");
			
	goto ret ;
    }

    /* case b) lineno */
    /* check if line number */
    if (isdigit(*s)) {
	/* get the lineno */
	int line = atoi(s);

	/* if current context not present then we must get the module
	   which has main & set the break point @ line number provided
	   of that module : if current context known then set the bp 
	   at the line number given for the current module 
	*/
	if (cctxt->func) {
	    if (!cctxt->func->mod) {
		if (!applyToSet(functions,funcWithName,"main"))
		    fprintf(stderr,"Function \"main\" not defined.\n");
		else 
		    clearBPatModLine(func->mod,line);
	    } else 
		clearBPatModLine(cctxt->func->mod,line);			
	}
	
	goto ret;
    }

    if ((bp = strchr(s,':'))) {
	
	module *mod = NULL;
	*bp = '\0';
	
	if (!applyToSet(modules,moduleWithCName,s,&mod)) {
	    fprintf (stderr,"No source file named %s.\n",s);
	    goto ret;
	}

	/* case c) filename:lineno */
	if (isdigit(*(bp +1))) {	    	    
	 
	    clearBPatModLine (mod,atoi(bp+1));	    
	    goto ret;
	    
	}
	/* case d) filename:function */
	if (!applyToSet(functions,funcWithNameModule,bp+1,s,&func)) 
	    fprintf(stderr,"Function \"%s\" not defined.\n",bp+1); 
	else
	    clearBPatModLine (mod,func->entryline);
	
	goto ret;
    }
            
    /* case e) function */
    if (!applyToSet(functions,funcWithName,s,&func))
	fprintf(stderr,"Function \"%s\" not defined.\n",s); 
    else
	clearBPatModLine(func->mod,func->entryline);

 ret:    
    return 0;        
}


/*-----------------------------------------------------------------*/
/* cmdSimulator - send command to simulator                        */
/*-----------------------------------------------------------------*/
int cmdSimulator (char *s, context *cctxt)
{   
  char tmpstr[82];

    if (strlen(s) > 80) {
      printf("error 3A\n");
      exit(1);
    }
    strcpy(tmpstr, s);
    strcat(tmpstr, "\n");
    sendSim(tmpstr);
    waitForSim(200,NULL);
    fprintf(stdout,"%s",simResponse());
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdFrame - Frame command                                        */
/*-----------------------------------------------------------------*/
int cmdFrame (char *s, context *cctxt)
{   
    function *func ;

    if ((func = STACK_PEEK(callStack))) {
	fprintf(stdout,"#0  %s () at %s:%d\n",
		func->sym->name,func->mod->c_name,cctxt->cline);

	if (cctxt->cline < func->mod->ncLines)	    
	    fprintf(stdout,"%d\t%s",
		    cctxt->cline,
		    func->mod->cLines[cctxt->cline]->src);
    } else
	fprintf(stdout,"No stack.\n");
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdFinish - exec till end of current function                   */
/*-----------------------------------------------------------------*/
int cmdFinish (char *s, context *ctxt)
{
    if (!ctxt || ! ctxt->func) {
	fprintf(stdout,"The program is not running.\n");
	return 0;
    }

    if (srcMode == SRC_CMODE) {
	setBreakPoint (ctxt->func->sym->eaddr, CODE, STEP, 
		       stepBpCB, ctxt->func->mod->c_name, 
		       ctxt->func->exitline);
    } else {
	setBreakPoint (ctxt->func->sym->eaddr, CODE, STEP, 
		       stepBpCB, ctxt->func->mod->asm_name, 
		       ctxt->func->aexitline);
    }

    simGo(-1);
    return 0;
    
}


/*-----------------------------------------------------------------*/
/* cmdShow - show command                                          */
/*-----------------------------------------------------------------*/
int cmdShow (char *s, context *cctxt)
{
    /* skip white space */
    while (*s && isspace(*s)) s++ ;

    if (strcmp(s,"copying") == 0) {
	fputs(copying,stdout);
	return 0;
    }
    
    if (strcmp(s,"warranty") == 0) {
	fputs(warranty,stdout);
	return 0;
    }

    return 0;
}

