/*-------------------------------------------------------------------------
  SDCCmain.c - main file               

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

#include "common.h"
#include <ctype.h>
#include "spawn.h"

/* This is a bit messy.  We cant include unistd.h as it defines
   'link' which we also use.
*/
int access(const char *path, int mode);
#define X_OK 1
int unlink(const char *path);

extern void		initSymt		();
extern void		initMem			();
extern void		initExpr		();
extern void             initiCode               ();
extern void             initCSupport            ();
extern void             initPeepHole            ();
extern void		createObject	();
extern int		yyparse			();
extern void             glue ();
extern struct value    *constVal(char *s);
extern double            floatFromVal(struct value *);
extern int              fatalError ;
FILE  *srcFile         ;/* source file          */
FILE  *cdbFile = NULL  ;/* debugger information output file */
char  *fullSrcFileName ;/* full name for the source file */
char  *srcFileName     ;/* source file name with the .c stripped */
char  *moduleName      ;/* module name is srcFilename stripped of any path */
const char *preArgv[128]	   ;/* pre-processor arguments	*/
int   currRegBank = 0 ;
struct optimize optimize ;
struct options  options ;
char *VersionString = SDCC_VERSION_STR /*"Version 2.1.8a"*/;
short preProcOnly = 0;
short noAssemble = 0;
char *linkOptions[128];
const char *asmOptions[128];
char *libFiles[128] ;
int nlibFiles = 0;
char *libPaths[128] ;
int nlibPaths = 0;
char *relFiles[128];
int nrelFiles = 0;
bool verboseExec = FALSE;
//extern int wait (int *);
char    *preOutName;

#define OPTION_LARGE_MODEL "-model-large"
#define OPTION_SMALL_MODEL "-model-small"
#define OPTION_STACK_AUTO  "-stack-auto"
#define OPTION_XSTACK      "-xstack"
#define OPTION_GENERIC     "-generic"
#define OPTION_NO_GCSE     "-nogcse"
#define OPTION_NO_LOOP_INV "-noinvariant"
#define OPTION_NO_LOOP_IND "-noinduction"
#define OPTION_NO_JTBOUND  "-nojtbound"
#define OPTION_NO_LOOPREV  "-noloopreverse"
#define OPTION_XREGS       "-regextend"
#define OPTION_COMP_ONLY   "-compile-only"
#define OPTION_DUMP_RAW    "-dumpraw"
#define OPTION_DUMP_GCSE   "-dumpgcse"
#define OPTION_DUMP_LOOP   "-dumploop"
#define OPTION_DUMP_KILL   "-dumpdeadcode"
#define OPTION_DUMP_RANGE  "-dumpliverange"
#define OPTION_DUMP_PACK   "-dumpregpack"
#define OPTION_DUMP_RASSGN "-dumpregassign"
#define OPTION_DUMP_ALL    "-dumpall"
#define OPTION_XRAM_LOC    "-xram-loc"
#define OPTION_IRAM_SIZE   "-iram-size"
#define OPTION_XSTACK_LOC  "-xstack-loc"
#define OPTION_CODE_LOC    "-code-loc"
#define OPTION_STACK_LOC   "-stack-loc"
#define OPTION_DATA_LOC    "-data-loc"
#define OPTION_IDATA_LOC   "-idata-loc"
#define OPTION_PEEP_FILE   "-peep-file"
#define OPTION_LIB_PATH    "-lib-path"
#define OPTION_INTLONG_RENT "-int-long-reent"
#define OPTION_FLOAT_RENT  "-float-reent"
#define OPTION_OUT_FMT_IHX "-out-fmt-ihx"
#define OPTION_OUT_FMT_S19 "-out-fmt-s19"
#define OPTION_CYCLOMATIC  "-cyclomatic"
#define OPTION_NOOVERLAY   "-nooverlay"
#define OPTION_MAINRETURN  "-main-return"
#define OPTION_NOPEEP      "-no-peep"
#define OPTION_ASMPEEP     "-peep-asm"
#define OPTION_DEBUG       "-debug"
#define OPTION_VERSION     "-version"
#define OPTION_STKAFTRDATA "-stack-after-data"
#define OPTION_PREPROC_ONLY "-preprocessonly"
#define OPTION_HELP         "-help"
#define OPTION_CALLEE_SAVES "-callee-saves"
#define OPTION_NOREGPARMS   "-noregparms"

static const char *_preCmd[] = {
    "sdcpp", "-version", "-Wall", "-lang-c++", "-DSDCC=1", 
    "-I" SDCC_INCLUDE_DIR, "$l", "$1", "$2", NULL
};

extern PORT mcs51_port;
extern PORT z80_port;

PORT *port;

static PORT *_ports[] = {
    &mcs51_port,
    &z80_port
};

#define NUM_PORTS (sizeof(_ports)/sizeof(_ports[0]))

/** Sets the port to the one given by the command line option.
    @param		The name minus the option (eg 'mcs51')
    @return 		0 on success.
*/
static int _setPort(const char *name)
{
    int i;
    for (i=0; i<NUM_PORTS; i++) {
	if (!strcmp(_ports[i]->target, name)) {
	    port = _ports[i];
	    return 0;
	}
    }
    /* Error - didnt find */
    return 1;
}

static void _buildCmdLine(char *into, char **args, const char **cmds, 
			  const char *p1, const char *p2, 
			  const char *p3, const char **list)
{
    const char *p, *from;

    while (*cmds) {
	*args = into;
	args++;

	from = *cmds;
	cmds++;
	*into = '\0';

	/* See if it has a '$' anywhere - if not, just copy */
	if ((p = strchr(from, '$'))) {
	    strncpy(into, from, p - from);
	    from = p+2;
	    p++;
	    switch (*p) {
	    case '1':
		if (p1)
		    strcat(into, p1);
		break;
	    case '2':
		if (p2)
		    strcat(into, p2);
		break;
	    case '3':
		if (p3)
		    strcat(into, p3);
		break;
	    case 'l': {
		const char **tmp = list;
		if (tmp) {
		    while (*tmp) {
			strcpy(into, *tmp);
			into += strlen(into)+1;
			*args = into;
			args++;
			tmp++;
		    }
		}
		break;
	    }
	    default:
		assert(0);
	    }
	}
	strcat(into, from);
	if (strlen(into) == 0)
	    args--;
	into += strlen(into)+1;
    }
    *args = NULL;
}

/*-----------------------------------------------------------------*/
/* printVersionInfo - prints the version info			   */
/*-----------------------------------------------------------------*/
void	printVersionInfo ()
{
    int i;
    
    fprintf (stderr,
	     "SDCC : ");
    for (i=0; i<NUM_PORTS; i++)
	fprintf(stderr, "%s%s", i==0 ? "" : "/", _ports[i]->target);

    fprintf(stderr, " %s `"
#ifdef __CYGWIN32__
		" (CYGWIN32)\n"
#else
# ifdef __DJGPP__
		" (DJGPP) \n"
# else
		" (UNIX) \n"
# endif
#endif
	    , VersionString
	    );
}

/*-----------------------------------------------------------------*/
/* printUsage - prints command line syntax			   */
/*-----------------------------------------------------------------*/
void	printUsage ()
{
	printVersionInfo ();
	fprintf (stderr,
		 "Usage : [options] filename\n"
		 "Options :-\n"
		 "\t-m<proc>             -     Target processor <proc>.  Default %s\n"
		 "\t                           Try --version for supported values of <proc>\n"
		 "\t--model-large        -     Large Model\n"
		 "\t--model-small        -     Small Model (default)\n"
		 "\t--stack-auto         -     Stack automatic variables\n"
		 "\t--xstack             -     Use external stack\n"
		 "\t--xram-loc <nnnn>    -     External Ram start location\n"
		 "\t--xstack-loc <nnnn>  -     Xternal Stack Location\n"
		 "\t--code-loc <nnnn>    -     Code Segment Location\n"
		 "\t--stack-loc <nnnn>   -     Stack pointer initial value\n"
		 "\t--data-loc <nnnn>    -     Direct data start location\n"
		 "\t--idata-loc <nnnn>   -     Indirect data start location\n"
		 "\t--iram-size <nnnn>   -     Internal Ram size\n"
		 "\t--nojtbound          -     Don't generate boundary check for jump tables\n"
		 "\t--generic            -     All unqualified ptrs converted to '_generic'\n"
		 "PreProcessor Options :-\n"
		 "\t-Dmacro		-	Define Macro\n"		 
		 "\t-Ipath		-	Include \"*.h\" path\n"
		 "Note: this is a complete list of options see docs for details\n",
		 _ports[0]->target
		 );		
	exit (0);
}

/*-----------------------------------------------------------------*/
/* parseWithComma - separates string with comma                    */
/*-----------------------------------------------------------------*/
void parseWithComma (char **dest,char *src)
{
    int i = 0;
    
    strtok(src,"\n \t");
    /* skip the initial white spaces */
    while (isspace(*src)) src++;
    dest[i++] = src;
    while (*src) {
	if (*src == ',') {
	    *src = '\0';
	    src++;
	    if (*src)
		dest[i++] = src;
	    continue ;
	}
	src++;
    }
}

/*-----------------------------------------------------------------*/
/* setDefaultOptions - sets the default options                    */
/*-----------------------------------------------------------------*/
static void setDefaultOptions()
{
    int i ;

    for ( i = 0 ; i < 128 ; i++)
	preArgv[i] = linkOptions [i] =
	    asmOptions[i] = relFiles[i] = libFiles[i] =
	    libPaths[i] = NULL ;

    /* first the options part */
    options.stack_loc = 0; /* stack pointer initialised to 0 */
    options.xstack_loc= 0; /* xternal stack starts at 0 */
    options.code_loc  = 0; /* code starts at 0 */
    options.data_loc  = 0x0030; /* data starts at 0x0030 */
    options.xdata_loc = 0;
    options.idata_loc = 0x80;
    options.genericPtr = 1;   /* default on */
    options.nopeep    = 0;

    /* now for the optimizations */
    /* turn on the everything */
    optimize.global_cse = 1;    
    optimize.label1 = 1;
    optimize.label2 = 1;
    optimize.label3 = 1;
    optimize.label4 = 1;    
    optimize.loopInvariant = 1;
    optimize.loopInduction = 1;

    port->setDefaultOptions();
}

/*-----------------------------------------------------------------*/
/* processFile - determines the type of file from the extension    */
/*-----------------------------------------------------------------*/
static void processFile (char *s)
{
    char *fext = NULL;
    
    /* get the file extension */
    fext = s + strlen(s);
    while ((fext != s) && *fext != '.') fext--;
    
    /* now if no '.' then we don't know what the file type is
       so give a warning and return */
    if (fext == s) {
	werror(W_UNKNOWN_FEXT,s);
	return ;
    }

    /* otherwise depending on the file type */
    if (strcmp(fext,".c") == 0 || strcmp(fext,".C") == 0) {
	/* source file name : not if we already have a 
	   source file */
	if (srcFileName) {
	    werror(W_TOO_MANY_SRC,s);
	    return ;
	}

	/* the only source file */       
	if (!(srcFile = fopen((fullSrcFileName = s),"r"))) {
	    werror(E_FILE_OPEN_ERR,s);
	    exit (1);
	}
	
	/* copy the file name into the buffer */
	strcpy(buffer,s);
	
	/* get rid of the "." */
	strtok(buffer,".");
	ALLOC_ATOMIC(srcFileName,strlen(buffer)+1);
	strcpy(srcFileName,buffer);

	/* get rid of any path information 
	   for the module name; do this by going
	   backwards till we get to either '/' or '\' or ':'
	   or start of buffer */
	fext = buffer + strlen(buffer);
	while (fext != buffer && 
	       *(fext -1) != '\\'  &&
	       *(fext-1) != '/'   &&
	       *(fext-1) != ':')
	    fext--;
	ALLOC_ATOMIC(moduleName,strlen(fext)+1);
	strcpy(moduleName,fext);
	
	return ;
    }

    /* if the extention is type .rel or .r or .REL or .R 
       addtional object file will be passed to the linker */
    if (strcmp(fext,".r") == 0 || strcmp(fext,".rel") == 0 ||
	strcmp(fext,".R") == 0 || strcmp(fext,".REL") == 0) {
	
	relFiles[nrelFiles++] = s;
	return ;
    }

    /* if .lib or .LIB */
    if (strcmp(fext,".lib") == 0 || strcmp(fext,".LIB") == 0) {
	libFiles[nlibFiles++] = s;
	return;
    }

    werror(W_UNKNOWN_FEXT,s);
  
}

static void _addToList(const char **list, const char *str)
{
    /* This is the bad way to do things :) */
    while (*list)
	list++;
    *list = strdup(str);
    if (!*list) {
	werror(E_OUT_OF_MEM,__FILE__, 0);
	exit (1);
    }
    *(++list) = NULL;
}

/*-----------------------------------------------------------------*/
/* parseCmdLine - parses the command line and sets the options     */
/*-----------------------------------------------------------------*/
int   parseCmdLine ( int argc, char **argv )
{
    int i ;      
    char cdbfnbuf[50];

    /* go thru all whole command line	*/
    for ( i = 1; i < argc; i++ ) {
	if ( i >= argc )
	    break ;

	/* options */
	if (argv[i][0] == '-' && argv[i][1] == '-') {
	    
	    if (strcmp(&argv[i][1],OPTION_HELP) == 0) {
		printUsage();
                exit(0);
	    }	    

	    if (strcmp(&argv[i][1],OPTION_XREGS) == 0) {
		options.regExtend = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_LARGE_MODEL) == 0) {
		options.model = 1;
                continue;
	    }
	    
	    if (strcmp(&argv[i][1],OPTION_SMALL_MODEL) == 0) {
		options.model = 0;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_STACK_AUTO) == 0) {
		options.stackAuto = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_DUMP_RAW) == 0) {
		options.dump_raw = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_CYCLOMATIC) == 0) {
		options.cyclomatic = 1;
                continue;
	    }
	    
	    if (strcmp(&argv[i][1],OPTION_DUMP_GCSE) == 0) {
		options.dump_gcse = 1;
                continue;
	    }
	    
	    if (strcmp(&argv[i][1],OPTION_DUMP_LOOP) == 0) {
		options.dump_loop = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_DUMP_KILL) == 0) {
		options.dump_kill = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_INTLONG_RENT) == 0) {
		options.intlong_rent = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_FLOAT_RENT) == 0) {
		options.float_rent = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_DUMP_RANGE) == 0) {
		options.dump_range = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_DUMP_PACK) == 0) {
		options.dump_pack = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_DUMP_RASSGN) == 0) {
		options.dump_rassgn = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_OUT_FMT_IHX) == 0) {
		options.out_fmt = 0;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_OUT_FMT_S19) == 0) {
		options.out_fmt = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_NOOVERLAY) == 0) {
		options.noOverlay = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_STKAFTRDATA) == 0) {
		options.stackOnData = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_PREPROC_ONLY) == 0) {
		preProcOnly = 1;
                continue;
	    }

	    
	    if (strcmp(&argv[i][1],OPTION_DUMP_ALL) == 0) {
		options.dump_rassgn = 
                options.dump_pack   =
                options.dump_range  = 
                options.dump_kill   = 
                options.dump_loop   = 
                options.dump_gcse   = 
                options.dump_raw    = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_COMP_ONLY) == 0) {
		options.cc_only = 1;
                continue;
	    }
   
	    if (strcmp(&argv[i][1],OPTION_GENERIC) == 0) {
		options.genericPtr = 1;
                continue;
	    }
	    
	    if (strcmp(&argv[i][1],OPTION_NOPEEP) == 0) {
		options.nopeep = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_ASMPEEP) == 0) {
		options.asmpeep = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_DEBUG) == 0) {
		options.debug = 1;		
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_NOREGPARMS) == 0) {
		options.noregparms = 1;		
                continue;
	    }
	    
	    if (strcmp(&argv[i][1],OPTION_PEEP_FILE) == 0) {
		if (argv[i][1+strlen(OPTION_PEEP_FILE)]) 
		    options.peep_file = 
			&argv[i][1+strlen(OPTION_PEEP_FILE)];
		else
		    options.peep_file = argv[++i];
		continue ;
	    }

	    if (strcmp(&argv[i][1],OPTION_LIB_PATH) == 0) {
		if (argv[i][1+strlen(OPTION_LIB_PATH)]) 
		    libPaths[nlibPaths++] = 
			&argv[i][1+strlen(OPTION_PEEP_FILE)];
		else
		    libPaths[nlibPaths++] = argv[++i];
		continue ;
	    }

	    if (strcmp(&argv[i][1],OPTION_XSTACK_LOC) == 0) {
		
		if (argv[i][1+strlen(OPTION_XSTACK_LOC)])
		    options.xstack_loc = 
			(int) floatFromVal(constVal(&argv[i][1+strlen(OPTION_XSTACK_LOC)]));
		    else
			options.xstack_loc = 
			    (int) floatFromVal(constVal(argv[++i]));
		continue ;
	    }

	    if (strcmp(&argv[i][1],OPTION_XSTACK) == 0) {
		options.useXstack = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_MAINRETURN) == 0) {
		options.mainreturn = 1;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_CALLEE_SAVES) == 0) {
		if (argv[i][1+strlen(OPTION_CALLEE_SAVES)])
		    parseWithComma(options.calleeSaves
				   ,&argv[i][1+strlen(OPTION_CALLEE_SAVES)]);
		else
		    parseWithComma(options.calleeSaves,argv[++i]);
                continue;
	    }
	    
	    if (strcmp(&argv[i][1],OPTION_STACK_LOC) == 0) {
		
		if (argv[i][1+strlen(OPTION_STACK_LOC)])
		    options.stack_loc = 
			(int) floatFromVal(constVal(&argv[i][1+strlen(OPTION_STACK_LOC)]));
		    else
			options.stack_loc = 
			    (int) floatFromVal(constVal(argv[++i]));
		continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_XRAM_LOC) == 0) {
		
		if (argv[i][1+strlen(OPTION_XRAM_LOC)])
		    options.xdata_loc = 
			(unsigned int) floatFromVal(constVal(&argv[i][1+strlen(OPTION_XRAM_LOC)]));
		    else
			options.xdata_loc = 
			    (unsigned int) floatFromVal(constVal(argv[++i]));
		continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_IRAM_SIZE) == 0) {
		
		if (argv[i][1+strlen(OPTION_IRAM_SIZE)])
		    options.iram_size = 
			(int) floatFromVal(constVal(&argv[i][1+strlen(OPTION_IRAM_SIZE)]));
		    else
			options.iram_size = 
			    (int) floatFromVal(constVal(argv[++i]));
		continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_VERSION) == 0) {		
		printVersionInfo();
		exit(0);
		continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_DATA_LOC) == 0) {
		
		if (argv[i][1+strlen(OPTION_DATA_LOC)])
		    options.data_loc = 
			(int) floatFromVal(constVal(&argv[i][1+strlen(OPTION_DATA_LOC)]));
		    else
			options.data_loc = 
			    (int) floatFromVal(constVal(argv[++i]));
		continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_IDATA_LOC) == 0) {
		
		if (argv[i][1+strlen(OPTION_IDATA_LOC)])
		    options.idata_loc = 
			(int) floatFromVal(constVal(&argv[i][1+strlen(OPTION_IDATA_LOC)]));
		    else
			options.idata_loc = 
			    (int) floatFromVal(constVal(argv[++i]));
		continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_CODE_LOC) == 0) {
		
		if (argv[i][1+strlen(OPTION_CODE_LOC)])
		    options.code_loc = 
			(int) floatFromVal(constVal(&argv[i][1+strlen(OPTION_CODE_LOC)]));
		    else
			options.code_loc = 
			    (int) floatFromVal(constVal(argv[++i]));
		continue;
	    }

	    
	    if (strcmp(&argv[i][1],OPTION_NO_JTBOUND) == 0) {
		optimize.noJTabBoundary = 1;
                continue;
	    }
 
	    if (strcmp(&argv[i][1],OPTION_NO_GCSE) == 0) {
		optimize.global_cse = 0;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_NO_LOOP_INV) == 0) {
		optimize.loopInvariant = 0;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_NO_LOOP_IND) == 0) {
		optimize.loopInduction = 0;
                continue;
	    }

	    if (strcmp(&argv[i][1],OPTION_NO_LOOPREV) == 0) {
		optimize.noLoopReverse = 1;
                continue;
	    }

	    if (!port->parseOption(&argc, argv))
		werror(W_UNKNOWN_OPTION,argv[i]);
	}      

	/* these are undocumented options */
	/* if preceded by '/' then turn off certain optmizations, used
	   for debugging only these are also the legacy options from
	   version 1.xx will be removed gradually */
	if ( *argv[i] == '/') {
	    switch (argv[i][1]) {
		
	    case 'p':
		optimize.ptrArithmetic=0;
		break;

	    case 'L':
		switch (argv[i][2]) {
		case '\0':
		    optimize.label1 = 
			optimize.label2 =
			optimize.label3 = 
			optimize.label4 = 0 ;
		    break;
		case '1':
		    optimize.label1 = 0;
		    break;
		case '2':
		    optimize.label2 = 0;
		    break;
		case '3':
		    optimize.label3 = 0;
		    break;
		case '4':
		    optimize.label4 = 0;
		    break;
		}
		break;
		
	    case 'l' :
		switch (argv[i][2]) {		    
		case 'i' :
		    optimize.loopInvariant = 0;
		    break;
		case 'n' :
		    optimize.loopInduction = 0;
		    break;


		}
		break ;
	    case 'g' :
		optimize.global_cse = 0;
		break;

	    }
	    continue ;
	}

	/* if preceded by  '-' then option */
	if ( *argv[i] == '-' ) {
	    switch (argv[i][1]) {
	    case 'h'	:
		printUsage();
		exit(0);
		break;
		
	    case 'E':
		preProcOnly = 1;
		break;

	    case 'm':
		/* Used to select the port */
		if (_setPort(argv[i] + 2)) {
		    werror(W_UNSUPP_OPTION,"-m","Unrecognised processor");
		}
		break;
	    
	    case 'a'	: 
		werror(W_UNSUPP_OPTION,"-a","use --stack-auto instead");
		break ;
		
	    case 'g'	:
		werror(W_UNSUPP_OPTION,"-g","use --generic instead");
		break;
				
	    case 'X'	:	/* use external stack		*/
		werror(W_UNSUPP_OPTION,"-X","use --xstack-loc instead");
		break ;
		
	    case 'x'	:
		werror(W_UNSUPP_OPTION,"-x","use --xstack instead");
		break;
		
	    case 'p'	:	/* stack pointer intial value */
	    case 'P'	:
		werror(W_UNSUPP_OPTION,"-p","use --stack-loc instead");
		break ;
				
	    case 'i'	:
		werror(W_UNSUPP_OPTION,"-i","use --idata-loc instead");
		break ;
		
	    case 'r'	:
		werror(W_UNSUPP_OPTION,"-r","use --xdata-loc instead");
		break ;
		
	    case 's'	:
		werror(W_UNSUPP_OPTION,"-s","use --code-loc instead");
		break ;		
				
	    case 'c':	   
		options.cc_only = 1;
		break;	

	    case 'Y':
		werror(W_UNSUPP_OPTION,"-Y","use -I instead");
		break;
		
	    case 'L' :
		if (argv[i][2])
		    libPaths[nlibPaths++] = &argv[i][2];
		else
		    libPaths[nlibPaths++] = argv[++i];
		break;
		
	    case 'W':
		/* linker options */
		if (argv[i][2] == 'l') {
		    if (argv[i][3])
			parseWithComma(linkOptions,&argv[i][3]);
		    else
			parseWithComma(linkOptions,argv[++i]);
		} else {
		    /* assembler options */
		    if (argv[i][2] == 'a') {
			if (argv[i][3])
			    parseWithComma(asmOptions,&argv[i][3]);
			else
			    parseWithComma(asmOptions,argv[++i]);
			
		    } else {
			werror(W_UNKNOWN_OPTION,argv[i]);		       
		    }
		}
		break;
	    case 'S':
		noAssemble = 1;
		break;

	    case 'v':
#if FEATURE_VERBOSE_EXEC
		verboseExec = TRUE;
#else
		printVersionInfo();
		exit(0);
#endif
		break;

		/* preprocessor options */		
	    case 'd':
	    case 'D':
	    case 'I':
	    case 'M':
	    case 'C':
	    case 'A':
	    case 'U':
		{
		    char sOpt = argv[i][1] ;
		    char *rest ;
		    
		    if ( argv[i][2] == ' ' || argv[i][2] == '\0') {
			i++ ;
			rest = argv[i] ;
		    }
		    else
			rest = &argv[i][2] ;
		    
		    if ( argv[i][1] == 'Y' )
			argv[i][1] = 'I';
		    if (argv[i][1] == 'M')
			preProcOnly = 1;

		    sprintf(buffer, "-%c%s", sOpt, rest);
		    _addToList(preArgv, buffer);
		}
		break ;

	    default:
		if (!port->parseOption(&argc, argv))
		    werror(W_UNKNOWN_OPTION,argv[i]);
	    }
	    continue ;
	}

	if (!port->parseOption(&argc, argv)) {
	    /* no option must be a filename */
	    processFile(argv[i]);
	}
    }	

    /* set up external stack location if not explicitly specified */
    if ( !options.xstack_loc )
	options.xstack_loc = options.xdata_loc ;

    /* if debug option is set the open the cdbFile */
    if (/* options.debug && */ srcFileName) {
	sprintf(cdbfnbuf,"%s.cdb",srcFileName);
	if ((cdbFile = fopen(cdbfnbuf,"w")) == NULL)
	    werror(E_FILE_OPEN_ERR,cdbfnbuf);
	else {
	    /* add a module record */
	    fprintf(cdbFile,"M:%s\n",moduleName);
	}
    }
    port->finaliseOptions();
    return 0;
}

/*-----------------------------------------------------------------*/
/* my_system - will call a program with arguments                  */
/*-----------------------------------------------------------------*/
char *try_dir[]= {SRCDIR "/bin",PREFIX "/bin", NULL};
int my_system (const char *cmd, char **cmd_argv)
{    

    char *dir, *got= NULL; int i= 0;
    while (!got && try_dir[i]) {
	dir= (char*)malloc(strlen(try_dir[i])+strlen(cmd)+10);
	strcpy(dir, try_dir[i]); strcat(dir, "/"); strcat(dir, cmd);
	if (access(dir, X_OK) == 0)
	    got= strdup(dir);
	free(dir);
	i++;
    }
#if FEATURE_VERBOSE_EXEC
    if (verboseExec) {
	char **pCmd = cmd_argv;
	while (*pCmd) {
	    printf("%s ", *pCmd);
	    pCmd++;
	}
	printf("\n");
    }
#endif
    if (got)
      i= spawnv(P_WAIT,got,cmd_argv) == -1;
    else
      i= spawnvp(P_WAIT,cmd,cmd_argv) == -1;
    if (i) {
	perror("Cannot exec process ");
	return -1;
    }
    
    return 0;
}

/*-----------------------------------------------------------------*/
/* linkEdit : - calls the linkage editor  with options             */
/*-----------------------------------------------------------------*/
static void linkEdit (char **envp)
{
    FILE *lnkfile ;
    char *argv[128];

    int i;
    if (!srcFileName)
	srcFileName = "temp";

    /* first we need to create the <filename>.lnk file */
    sprintf(buffer,"%s.lnk",srcFileName);
    if (!(lnkfile = fopen(buffer,"w"))) {
	werror(E_FILE_OPEN_ERR,buffer);
	exit(1);
    }

    /* now write the options */
    fprintf(lnkfile,"-mux%c\n", (options.out_fmt ? 's' : 'i'));

    /* if iram size specified */
    if (options.iram_size)
	fprintf(lnkfile,"-a 0x%04x\n",options.iram_size);
    
    /*if (options.debug) */
    fprintf(lnkfile,"-z\n");
    /* code segment start */
    fprintf (lnkfile,"-b CODE = 0x%04x\n",options.code_loc);
    /* data segment start */
    fprintf (lnkfile,"-b DSEG = 0x%04x\n",options.data_loc);
    /* xdata start */
    fprintf (lnkfile,"-b XSEG = 0x%04x\n",options.xdata_loc);
    /* indirect data */
    fprintf (lnkfile,"-b ISEG = 0x%04x\n",options.idata_loc);
    /* bit segment start */
    fprintf (lnkfile,"-b BSEG = 0x%04x\n",0);
    
    /* add the extra linker options */
    for (i=0; linkOptions[i] ; i++)
	fprintf(lnkfile,"%s\n",linkOptions[i]);

    /* standard library path */
    fprintf (lnkfile,"-k %s/%s\n",SDCC_LIB_DIR/*STD_LIB_PATH*/,
	     ( (options.model==0) ? "small": "large"));
	    
    /* other library paths if specified */
    for (i = 0 ; i < nlibPaths ; i++ )
	fprintf (lnkfile,"-k %s\n",libPaths[i]);
        
    /* standard library files */
    fprintf (lnkfile,"-l %s\n",STD_LIB);
    fprintf (lnkfile,"-l %s\n",STD_INT_LIB);
    fprintf (lnkfile,"-l %s\n",STD_LONG_LIB);
    fprintf (lnkfile,"-l %s\n",STD_FP_LIB);

    /* additional libraries if any */
    for (i = 0 ; i < nlibFiles; i++)
	fprintf (lnkfile,"-l %s\n",libFiles[i]);

    /* put in the object files */
    if (strcmp(srcFileName,"temp"))
	fprintf (lnkfile,"%s ",srcFileName);

    for (i = 0 ; i < nrelFiles ; i++ )
	fprintf (lnkfile,"%s\n",relFiles[i]);

    fprintf (lnkfile,"\n-e\n");
    fclose(lnkfile);

    _buildCmdLine(buffer, argv, port->linker.cmd, srcFileName, NULL, NULL, NULL);

    /* call the linker */
    if (my_system(argv[0], argv)) {
	perror("Cannot exec linker");
	exit(1);
    }

    if (strcmp(srcFileName,"temp") == 0) {
	/* rename "temp.cdb" to "firstRelFile.cdb" */
	char *f = strtok(strdup(relFiles[0]),".");
	f = strcat(f,".cdb");
	rename("temp.cdb",f);       
	srcFileName = NULL;
    }
}

/*-----------------------------------------------------------------*/
/* assemble - spawns the assembler with arguments                  */
/*-----------------------------------------------------------------*/
static void assemble (char **envp)
{
    char *argv[128];  /* assembler arguments */

    _buildCmdLine(buffer, argv, port->assembler.cmd, srcFileName, NULL, NULL, asmOptions);

    if (my_system(argv[0], argv)) {
	perror("Cannot exec assember");
	exit(1);
    }
}



/*-----------------------------------------------------------------*/
/* preProcess - spawns the preprocessor with arguments		   */
/*-----------------------------------------------------------------*/
static int preProcess (char **envp)
{
    char *argv[128];

    preOutName = NULL;

    /* if using external stack define the macro */
    if ( options.useXstack )
	_addToList(preArgv, "-DSDCC_USE_XSTACK");
    
    /* set the macro for stack autos	*/
    if ( options.stackAuto )
	_addToList(preArgv, "-DSDCC_STACK_AUTO");
    
    /* set the macro for large model	*/
    if ( options.model )
	_addToList(preArgv, "-DSDCC_MODEL_LARGE");
    else
	_addToList(preArgv, "-DSDCC_MODEL_SMALL");
    
    if (!preProcOnly)
	preOutName = strdup(tmpnam(NULL));

    _buildCmdLine(buffer, argv, _preCmd, fullSrcFileName, 
		  preOutName, srcFileName, preArgv);

    if (my_system(argv[0], argv)) {
	unlink (preOutName);
	perror("Cannot exec Preprocessor");
	exit(1);
    }

    if (preProcOnly)
	exit(0);

    yyin = fopen(preOutName,"r");
    if (yyin == NULL) {
	perror("Preproc file not found\n");
	exit(1);
    }
    
    return 0;
}

static void _findPort(int argc, char **argv)
{
    argc--;
    while (argc) {
	if (!strncmp(*argv, "-m", 2)) {
	    _setPort(*argv + 2);
	    return;
	}
	argv++;
	argc--;
    }
    /* Use the first in the list */
    port = _ports[0];
}

/* 
 * main routine
 * initialises and calls the parser
 */

int main ( int argc, char **argv , char **envp)
{
    /* turn all optimizations off by default */
    memset(&optimize,0,sizeof(struct optimize));
    
    /*printVersionInfo ();*/

    _findPort(argc, argv);
    setDefaultOptions();
    parseCmdLine(argc,argv);

    /* if no input then printUsage & exit */
    if (!srcFileName && !nrelFiles) {
	printUsage();
	exit(0);
    }
	
    if (srcFileName)
	preProcess(envp) ;

    if (srcFileName) {

	initSymt();
	initMem();	  	    
	initiCode();
	initCSupport ();
	initPeepHole();
	yyparse();

	if (!fatalError) {
	    glue();
	    assemble(envp);
	}
	
    }
    
    if (cdbFile)
	fclose(cdbFile);

    if (!options.cc_only && 
	!fatalError      &&
	!noAssemble      &&
	(srcFileName || nrelFiles))
	linkEdit (envp);

    if (yyin && yyin != stdin)
	fclose(yyin);

    if (preOutName) {
	unlink(preOutName);
	free(preOutName);
    }
    return 0;
    
}
