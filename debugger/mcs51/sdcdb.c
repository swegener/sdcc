/*-------------------------------------------------------------------------
  sdcdb.c - main source file for sdcdb debugger
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

char *currModName = NULL;
cdbrecs *recsRoot = NULL ;
set  *modules = NULL;    /* set of all modules */
set  *functions = NULL ; /* set of functions */
set  *symbols = NULL   ; /* set of symbols */
int nStructs = 0 ;
structdef **structs = NULL ; /* all structures */
int nLinkrecs = 0;
linkrec **linkrecs = NULL; /* all linkage editor records */
context *currCtxt = NULL;
short fullname = 0;
short showfull = 0;
short userinterrupt = 0;
char *ssdirl = DATADIR LIB_DIR_SUFFIX ":" DATADIR LIB_DIR_SUFFIX "/small" ;
char *simArgs[40];
int nsimArgs = 0;
char model_str[20];

/* fake filename & lineno to make linker */
char *filename=NULL;
int lineno = 0;
int fatalError = 0;

/* command table */
struct cmdtab
{
    char      *cmd ;  /* command the user will enter */
    int (*cmdfunc)(char *,context *);   /* function to execute when command is entered */
    char *htxt ;    /* short help text */

} cmdTab[] = {
    /* NOTE:- the search is done from the top, so "break" should
       precede the synonym "b" */
    /* break point */
    { "break"    ,  cmdSetUserBp  ,
      "{b}reak\t\t\t [LINE | FILE:LINE | FILE:FUNCTION | FUNCTION]\n",
    },
    { "b"        ,  cmdSetUserBp  , NULL },

    { "clear"    ,  cmdClrUserBp  ,
      "{cl}ear\t\t\t [LINE | FILE:LINE | FILE:FUNCTION | FUNCTION]\n"
    },
    { "cl"       ,  cmdClrUserBp  , NULL },

    { "continue" ,  cmdContinue   ,
      "{c}ontinue\t\t Continue program being debugged, after breakpoint.\n"
    },
    { "c"        ,  cmdContinue   , NULL },

    { "disassemble",cmdDisasmF    , "x disassemble asm commands\n" },
    { "delete" ,  cmdDelUserBp  ,
      "{d}elete n\t\t clears break point number n\n"
    },
    { "display"    ,  cmdDisplay     ,
      "display [<variable>]\t print value of given variable each time the program stops\n"
    },
    { "undisplay"  ,  cmdUnDisplay   ,
      "undisplay [<variable>]\t dont display this variable or all\n"
    },
    { "d"        ,  cmdDelUserBp  , NULL },

    { "info"     ,  cmdInfo       ,
      "info <break stack frame registers all-registers>\n"
      "\t list all break points, call-stack, frame or register information\n"
    },

    { "listasm"  ,  cmdListAsm    ,
      "listasm {la}\t\t list assembler code for the current C line\n"
    },
    { "la"       ,  cmdListAsm    , NULL },
    { "ls"       ,  cmdListSymbols  , "ls,lf,lm\t\t list symbols,functions,modules\n" },
    { "lf"       ,  cmdListFunctions, NULL },
    { "lm"       ,  cmdListModules  , NULL },
    { "list"     ,  cmdListSrc    ,
      "{l}ist\t\t\t [LINE | FILE:LINE | FILE:FUNCTION | FUNCTION]\n"
    },
    { "l"        ,  cmdListSrc    , NULL },
    { "show"     ,  cmdShow       ,
      "show"
      " <copying warranty>\t copying & distribution terms, warranty\n"
    },
    { "set"      ,  cmdSetOption  , "set <srcmode>\t\t toggle between c/asm.\nset variable <var> = >value\t\tset variable to new value\n" },
    { "step"     ,  cmdStep       ,
      "{s}tep\t\t\t Step program until it reaches a different source line.\n"
    },
    { "s"        ,  cmdStep       , NULL },
    { "next"     ,  cmdNext       ,
      "{n}ext\t\t\t Step program, proceeding through subroutine calls.\n"
    },
    { "n"        ,  cmdNext       , NULL },
    { "run"      ,  cmdRun        ,
      "{r}un\t\t\t Start debugged program. \n"
    },
    { "r"        ,  cmdRun        , NULL },
    { "ptype"    ,  cmdPrintType  ,
      "{pt}ype <variable>\t print type information of a variable\n"
    },
    { "pt"       ,  cmdPrintType  , NULL },
    { "print"    ,  cmdPrint      ,
      "{p}rint <variable>\t print value of given variable\n"
    },
    { "output"   ,  cmdOutput      ,
      "output <variable>\t print value of given variable without $ and newline \n"
    },
    { "p"        ,  cmdPrint      , NULL },
    { "file"     ,  cmdFile       ,
      "file <filename>\t\t load symbolic information from <filename>\n"
    },
    { "frame"    ,  cmdFrame      ,
      "{fr}ame\t\t\t print information about the current Stack\n"
    },
    { "finish"   ,  cmdFinish     ,
      "{fi}nish\t\t execute till return of current function\n"
    },
    { "fi"       ,  cmdFinish     , NULL },
    { "where"    ,  cmdWhere      , "where\t\t\t print stack\n" },
    { "fr"       ,  cmdFrame      , NULL },
    { "f"        ,  cmdFrame      , NULL },
    { "x /i"     ,  cmdDisasm1    , "x\t\t disassemble one asm command\n" },
    { "!"        ,  cmdSimulator  ,
      "!<simulator command>\t send a command directly to the simulator\n"
    },
    { "."        ,  cmdSimulator  ,
      ".{cmd}\t switch from simulator or debugger command mode\n"
    },
    { "help"     ,  cmdHelp       ,
      "{h|?}elp\t [CMD_NAME | 0,1,2,3(help page)] (general help or specific help)\n"
    },
    { "?"        ,  cmdHelp       , NULL },
    { "h"        ,  cmdHelp       , NULL },

    { "quit"     ,  cmdQuit       ,
      "{q}uit\t\t\t \"Watch me now. Iam going Down. My name is Bobby Brown\"\n"
    },
    { "q"        ,  cmdQuit       , NULL }
};

/*-----------------------------------------------------------------*/
/* gc_strdup - make a string duplicate garbage collector aware     */
/*-----------------------------------------------------------------*/
char *gc_strdup(const char *s)
{
    char *ret;
    ret = Safe_malloc(strlen(s)+1);
    strcpy(ret, s);
    return ret;
}

/*-----------------------------------------------------------------*/
/* alloccpy - allocate copy and return a new string                */
/*-----------------------------------------------------------------*/
char *alloccpy ( char *s, int size)
{
    char *d;

    if (!size)
  return NULL;

    d = Safe_malloc(size+1);
    memcpy(d,s,size);
    d[size] = '\0';

    return d;
}

/*-----------------------------------------------------------------*/
/* resize - resizes array of type with new size                    */
/*-----------------------------------------------------------------*/
void **resize (void **array, int newSize)
{
    void **vptr;

    if (array)
  vptr = Safe_realloc(array,newSize*(sizeof(void **)));
    else
  vptr = calloc(1, sizeof(void **));

    if (!vptr) {
  fprintf(stderr,"sdcdb: out of memory \n");
  exit(1);
    }

    return vptr;

}

/*-----------------------------------------------------------------*/
/* readCdb - reads the cdb files & puts the records into cdbLine   */
/*           linked list                                           */
/*-----------------------------------------------------------------*/
static int readCdb (FILE *file)
{
    cdbrecs *currl ;
    char buffer[1024];
    char *bp ;

    if (!(bp = fgets(buffer,sizeof(buffer),file)))
      return 0;

    currl = Safe_calloc(1,sizeof(cdbrecs));
    recsRoot = currl ;

    while (1) {

  /* make sure this is a cdb record */
  if (strchr("STLFM",*bp) && *(bp+1) == ':') {
      /* depending on the record type */

      switch (*bp) {
      case 'S':
    /* symbol record */
    currl->type = SYM_REC;
    break;
      case 'T':
    currl->type = STRUCT_REC;
    break;
      case 'L':
    currl->type = LNK_REC;
    break;
      case 'F':
    currl->type = FUNC_REC;
    break;
      case 'M':
    currl->type = MOD_REC ;
      }

      bp += 2;
      currl->line = Safe_malloc(strlen(bp));
      strncpy(currl->line,bp,strlen(bp)-1);
      currl->line[strlen(bp)-1] = '\0';
  }

  if (!(bp = fgets(buffer,sizeof(buffer),file)))
      break;

  if (feof(file))
      break;

  currl->next = Safe_calloc(1,sizeof(cdbrecs));
  currl = currl->next;
    }

    return (recsRoot->line ? 1 : 0);
}

/*-----------------------------------------------------------------*/
/* searchDirsFname - search directory list & return the filename   */
/*-----------------------------------------------------------------*/
char *searchDirsFname (char *fname)
{
    char *dirs , *sdirs;
    FILE *rfile = NULL;
    char buffer[128];

    /* first try the current directory */
    if ((rfile = fopen(fname,"r"))) {
  fclose(rfile);
  return strdup(fname) ;
    }

    if (!ssdirl)
  return strdup(fname);

    /* make a copy of the source directories */
    dirs = sdirs = strdup(ssdirl);

    /* assume that the separator is ':'
       and try for each directory in the search list */
    dirs = strtok(dirs,":");
    while (dirs) {
  if (dirs[strlen(dirs)] == '/')
      sprintf(buffer,"%s%s",dirs,fname);
  else
      sprintf(buffer,"%s/%s",dirs,fname);
  if ((rfile = fopen(buffer,"r")))
      break ;
  dirs = strtok(NULL,":");
    }

    free(sdirs);
    if (rfile) {
  fclose(rfile);
  return strdup(buffer);
    } else
  return strdup(fname);
}

/*-----------------------------------------------------------------*/
/* searchDirsFopen - go thru list of directories for filename given*/
/*-----------------------------------------------------------------*/
FILE *searchDirsFopen(char *fname)
{
    char *dirs , *sdirs;
    FILE *rfile = NULL;
    char buffer[128];

    /* first try the current directory */
    if ((rfile = fopen(fname,"r")))
  return rfile;

    if (!ssdirl)
  return NULL;
    /* make a copy of the source directories */
    dirs = sdirs = strdup(ssdirl);

    /* assume that the separator is ':'
       and try for each directory in the search list */
    dirs = strtok(dirs,":");
    while (dirs) {
  sprintf(buffer,"%s/%s",dirs,fname);
  if ((rfile = fopen(buffer,"r")))
      break ;
  dirs = strtok(NULL,":");
    }

    free(sdirs);
    return rfile ;

}

/*-----------------------------------------------------------------*/
/* loadFile - loads a file into module buffer                      */
/*-----------------------------------------------------------------*/
srcLine **loadFile (char *name, int *nlines)
{
    FILE *mfile ;
    char buffer[512];
    char *bp;
    srcLine **slines = NULL;


    if (!(mfile = searchDirsFopen(name))) {
  fprintf(stderr,"sdcdb: cannot open module %s -- use '--directory=<source directory> option\n",name);
  return NULL;
    }

    while ((bp = fgets(buffer,sizeof(buffer),mfile))) {
  (*nlines)++;

  slines = (srcLine **)resize((void **)slines,*nlines);

  slines[(*nlines)-1] = Safe_calloc(1,sizeof(srcLine));
  slines[(*nlines)-1]->src = alloccpy(bp,strlen(bp));
    }

    fclose(mfile);
    return slines;
}


/*-----------------------------------------------------------------*/
/* loadModules - reads the source files into module structure      */
/*-----------------------------------------------------------------*/
static void loadModules ()
{
    cdbrecs *loop;
    module *currMod;
    char *rs;

    /* go thru the records & find out the module
       records & load the modules specified */
    for ( loop = recsRoot ; loop ; loop = loop->next ) {

  switch (loop->type) {
  /* for module records do */
  case MOD_REC:
      currMod = parseModule(loop->line,TRUE);
      currModName = currMod->name ;

      currMod->cfullname = searchDirsFname(currMod->c_name);

      /* load it into buffer */
      currMod->cLines = loadFile (currMod->c_name,
          &currMod->ncLines);

      /* do the same for the assembler file */
      currMod->afullname = searchDirsFname(currMod->asm_name);
      currMod->asmLines=loadFile (currMod->asm_name,
          &currMod->nasmLines);
      break;

  /* if this is a function record */
  case FUNC_REC:
      parseFunc(loop->line);
      break;

  /* if this is a structure record */
  case STRUCT_REC:
      parseStruct(loop->line);
      break;

  /* if symbol then parse the symbol */
  case  SYM_REC:
      parseSymbol(loop->line,&rs);
      break;

  case LNK_REC:
      parseLnkRec(loop->line);
      break;
  }
    }
}

/*-----------------------------------------------------------------*/
/* functionPoints - determine the execution points within a func   */
/*-----------------------------------------------------------------*/
static void functionPoints ()
{
    function *func;
    symbol *sym;

    /* for all functions do */
    for ( func = setFirstItem(functions); func;
    func = setNextItem(functions)) {
  int j ;
  module *mod;

  sym = func->sym;

  Dprintf(D_sdcdb, ("func '%s' has entry '%x' exit '%x'\n",
         func->sym->name,
         func->sym->addr,
         func->sym->eaddr));

  if (!func->sym->addr && !func->sym->eaddr)
      continue ;

  /* for all source lines in the module find
     the ones with address >= start and <= end
     and put them in the point */
  mod = NULL ;
  if (! applyToSet(modules,moduleWithName,func->modName,&mod))
      continue ;
  func->mod = mod ;
  func->entryline= INT_MAX;
  func->exitline =  0;
  func->aentryline = INT_MAX ;
  func->aexitline = 0;

  /* do it for the C Lines first */
  for ( j = 0 ; j < mod->ncLines ; j++ ) {
      if (mod->cLines[j]->addr >= sym->addr &&
    mod->cLines[j]->addr <= sym->eaddr ) {

    exePoint *ep ;

    /* add it to the execution point */
    if (func->entryline > j)
        func->entryline = j;

    if (func->exitline < j)
        func->exitline = j;

    ep = Safe_calloc(1,sizeof(exePoint));
    ep->addr =  mod->cLines[j]->addr ;
    ep->line = j;
    ep->block= mod->cLines[j]->block;
    ep->level= mod->cLines[j]->level;
    addSet(&func->cfpoints,ep);
      }
  }

  /* do the same for asm execution points */
  for ( j = 0 ; j < mod->nasmLines ; j++ ) {
      if (mod->asmLines[j]->addr >= sym->addr &&
    mod->asmLines[j]->addr <= sym->eaddr ) {

    exePoint *ep ;
    /* add it to the execution point */
    if (func->aentryline > j)
        func->aentryline = j;

    if (func->aexitline < j)
        func->aexitline = j;

    /* add it to the execution point */
    ep = Safe_calloc(1,sizeof(exePoint));
    ep->addr =  mod->asmLines[j]->addr ;
    ep->line = j;
    addSet(&func->afpoints,ep);
      }
  }

#ifdef SDCDB_DEBUG
  Dprintf(D_sdcdb, ("function '%s' has the following C exePoints\n",
         func->sym->name));
  {
      exePoint *ep;

      for (ep = setFirstItem(func->cfpoints); ep;
     ep = setNextItem(func->cfpoints))
     Dprintf(D_sdcdb, ("{%x,%d} %s",
         ep->addr,ep->line,mod->cLines[ep->line]->src));

      Dprintf(D_sdcdb, (" and the following ASM exePoints\n"));
      for (ep = setFirstItem(func->afpoints); ep;
           ep = setNextItem(func->afpoints))
        Dprintf (D_sdcdb, ("{%x,%d} %s",
            ep->addr,ep->line,mod->asmLines[ep->line]->src));
  }
#endif
    }
}


/*-----------------------------------------------------------------*/
/* setEntryExitBP - set the entry & exit Break Points for functions*/
/*-----------------------------------------------------------------*/
DEFSETFUNC(setEntryExitBP)
{
    function *func = item;

    if (func->sym && func->sym->addr && func->sym->eaddr) {

  /* set the entry break point */
  setBreakPoint (func->sym->addr , CODE , FENTRY ,
           fentryCB ,func->mod->c_name , func->entryline);

  /* set the exit break point */
  setBreakPoint (func->sym->eaddr , CODE , FEXIT  ,
           fexitCB  ,func->mod->c_name , func->exitline );
    }

    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdFile - load file into the debugger                           */
/*-----------------------------------------------------------------*/
int cmdFile (char *s,context *cctxt)
{
    FILE *cdbFile;
    char buffer[128];
    char *bp;

    while (isspace(*s)) s++;
    if (!*s) {
  fprintf(stdout,"No exec file now.\nNo symbol file now.\n");
  return 0;
    }

    sprintf(buffer,"%s.cdb",s);
    /* try creating the cdbfile */
    if (!(cdbFile = searchDirsFopen(buffer))) {
      fprintf(stdout,"Cannot open file\"%s\", no symbolic information loaded\n",buffer);
      // return 0;
    }

    /* allocate for context */
    currCtxt = Safe_calloc(1,sizeof(context));

    if (cdbFile) {
      /* readin the debug information */
      if (!readCdb (cdbFile)) {
        fprintf(stdout,"No symbolic information found in file %s.cdb\n",s);
        //return 0;
      }
    }

    /* parse and load the modules required */
    loadModules();

    /* determine the execution points for this
       module */
    functionPoints();

    /* start the simulator & setup connection to it */
    if ( sock == -1 )
        openSimulator((char **)simArgs,nsimArgs);
    fprintf(stdout,"%s",simResponse());
    /* now send the filename to be loaded to the simulator */
    sprintf(buffer,"%s.ihx",s);
    bp=searchDirsFname(buffer);
    simLoadFile(bp);
    free(bp);

    /*set the break points
       required by the debugger . i.e. the function entry
       and function exit break points */
    applyToSet(functions,setEntryExitBP);

    /* ad we are done */
    return 0;
}

/*-----------------------------------------------------------------*/
/* cmdHelp - help command                                          */
/*-----------------------------------------------------------------*/
int cmdHelp (char *s, context *cctxt)
{
    int i ;
    int endline = 999;
    int startline = 0;

    while (isspace(*s))
      ++s;
    if (isdigit(*s)) {
      endline = ((*s - '0') * 20) + 20;
      if (endline > 0)
        startline = endline - 20;
    }
    else if (*s)
    {
        for (i = 0 ; i < (sizeof(cmdTab)/sizeof(struct cmdtab)) ; i++) 
        {
            if ((cmdTab[i].htxt) && !strcmp(cmdTab[i].cmd,s))
                fprintf(stdout,"%s",cmdTab[i].htxt);             
        }
        return 0;
    }

    for (i = 0 ; i < (sizeof(cmdTab)/sizeof(struct cmdtab)) ; i++) {

      /* command string matches */
      
      if ((cmdTab[i].htxt) && (i >= startline))
        fprintf(stdout,"%s",cmdTab[i].htxt);
      if (i == endline)
        break;
    }

    return 0;
}

#define MAX_CMD_LEN 512
static char cmdbuff[MAX_CMD_LEN];
static int sim_cmd_mode = 0;

/*-----------------------------------------------------------------
 interpretCmd - interpret and do the command.  Return 0 to continue,
   return 1 to exit program.
|-----------------------------------------------------------------*/
int interpretCmd (char *s)
{
    static char *pcmd = NULL;
    int i ;
    int rv = 0 ;

    /* if nothing & previous command exists then
       execute the previous command again */
    if (*s == '\n' && pcmd)
      strcpy(s,pcmd);

    /* if previous command exists & is different
       from the current command then copy it */
    if (pcmd) {
      if (strcmp(pcmd,s)) {
         free(pcmd);
         pcmd = strdup(s);
      }
    } else
      pcmd = strdup(s);

    /* lookup the command table and do the task required */
    strtok(s,"\n");

    if (sim_cmd_mode) {
      if (strcmp(s,".") == 0) {
        sim_cmd_mode = 0;
        return 0;
      }
      else if (s[0] == '.') {
        /* kill the preceeding '.' and pass on as SDCDB command */
        char *s1 = s+1;
        char *s2 = s;
        while (*s1 != 0)
          *s2++ = *s1++;
        *s2 = 0;
      } else {
        cmdSimulator (s, currCtxt);
        return 0;
      }
    } else {
      if (strcmp(s,".") ==0) {
        sim_cmd_mode = 1;
        return 0;
      }
    }

    for (i = 0 ; i < (sizeof(cmdTab)/sizeof(struct cmdtab)) ; i++) {

      /* command string matches */
      if (strncmp(s,cmdTab[i].cmd,strlen(cmdTab[i].cmd)) == 0) {
        if (!cmdTab[i].cmdfunc)
          return 1;

        rv = (*cmdTab[i].cmdfunc)(s + strlen(cmdTab[i].cmd),currCtxt);

        /* if full name then give the file name & position */
        if (fullname && showfull && currCtxt && currCtxt->func) {
          showfull = 0;
          if (srcMode == SRC_CMODE)
            fprintf(stdout,"\032\032%s:%d:1:beg:0x%08x\n",
                    currCtxt->func->mod->cfullname,
                    currCtxt->cline,currCtxt->addr);
          else
            fprintf(stdout,"\032\032%s:%d:1:beg:0x%08x\n",
                    currCtxt->func->mod->afullname,
                    currCtxt->asmline,currCtxt->addr);
          displayAll(currCtxt);
        }
        goto ret;
      }
    }

    fprintf(stdout,"Undefined command: \"%s\".  Try \"help\".\n",s);
 ret:
    return rv;
}

/*-----------------------------------------------------------------*/
/* commandLoop - the main command loop                             */
/*-----------------------------------------------------------------*/
void commandLoop()
{
 char *prompt = "(sdcdb) ";
 char *sim_prompt = "(sim) ";

  while (1) {
    if (sim_cmd_mode)
      printf("%s",sim_prompt);
    else
      fprintf(stdout,"%s",prompt);

    fflush(stdout);

    if (fgets(cmdbuff,sizeof(cmdbuff),stdin) == NULL)
      break;

#if 0
    /* make a way to go into "ucSim" mode */
    if (cmdbuff[0] == '$') {
      if (sim_cmd_mode) sim_cmd_mode = 0;
      else sim_cmd_mode = 1;
      continue;
    }
#endif

    if (interpretCmd(cmdbuff))
      break;
  }
}

/*-----------------------------------------------------------------*/
/* printVersionInfo - print the version information                */
/*-----------------------------------------------------------------*/
static void printVersionInfo()
{
    fprintf(stdout,
      "SDCDB is free software and you are welcome to distribute copies of it\n"
      "under certain conditions; type \"show copying\" to see the conditions.\n"
      "There is absolutely no warranty for SDCDB; type \"show warranty\" for details.\n"
      "SDCDB 0.8 . Copyright (C) 1999 Sandeep Dutta (sandeep.dutta@usa.net)\n"
      "Type ? for help\n");

}

/*-----------------------------------------------------------------*/
/* parseCmdLine - parse the commandline arguments                  */
/*-----------------------------------------------------------------*/
static void parseCmdLine (int argc, char **argv)
{
    int i ;
    char *filename = NULL;
    int passon_args_flag = 0;  /* if true, pass on args to simulator */

    Dprintf(D_sdcdb, ("parseCmdLine\n"));

    for ( i = 1; i < argc ; i++) {
  //fprintf(stdout,"%s\n",argv[i]);

  if (passon_args_flag) { /* if true, pass on args to simulator */
    simArgs[nsimArgs++] = strdup(argv[i]);
    continue;
  }

  /* if this is an option */
  if (argv[i][0] == '-') {

      /* if directory then mark directory */
      if (strncmp(argv[i],"--directory=",12) == 0) {
    if (!ssdirl)
        ssdirl = &argv[i][12];
    else {
        char *p = Safe_malloc(strlen(ssdirl)+strlen(&argv[i][12])+2);
        strcat(strcat(strcpy(p,&argv[i][12]),":"),ssdirl);
        ssdirl = p;
    }
    continue;
      }

      if (strncmp(argv[i],"-fullname",9) == 0) {
    fullname = TRUE;
    continue;
      }

      if (strcmp(argv[i],"-cd") == 0) {
    i++;
    chdir(argv[i]);
    continue;
      }

      if (strncmp(argv[i],"-cd=",4) == 0) {
    chdir(argv[i][4]);
    continue;
      }

      /* model string */
      if (strncmp(argv[i],"-m",2) == 0) {
        strncpy(model_str, &argv[i][2], 15);
        if (strcmp(model_str,"avr") == 0)
          simArgs[0] = "savr";
        else if (strcmp(model_str,"xa") == 0)
          simArgs[0] = "sxa";
        else if (strcmp(model_str,"z80") == 0)
          simArgs[0] = "sz80";
        continue ;
      }

      /* -z all remaining options are for simulator */
      if (strcmp(argv[i],"-z") == 0) {
        passon_args_flag = 1;
        continue ;
      }

      /* the simulator arguments */

      /* cpu */
      if (strcmp(argv[i],"-t") == 0 ||
    strcmp(argv[i],"-cpu") == 0) {

        simArgs[nsimArgs++] = "-t";
        simArgs[nsimArgs++] = strdup(argv[++i]);
        continue ;
      }

      /* XTAL Frequency */
      if (strcmp(argv[i],"-X") == 0 ||
    strcmp(argv[i],"-frequency") == 0) {
        simArgs[nsimArgs++] = "-X";
        simArgs[nsimArgs++] = strdup(argv[++i]);
        continue ;
      }

      /* serial port */
      if ( (strcmp(argv[i],"-S") == 0) ||
           (strcmp(argv[i],"-s") == 0)) {
        simArgs[nsimArgs++] = "-s";
        simArgs[nsimArgs++] = strdup(argv[++i]);
        continue ;
      }

      fprintf(stderr,"unknown option %s --- ignored\n",
        argv[i]);

  } else {
      /* must be file name */
      if (filename) {
    fprintf(stderr,"too many filenames .. parameter '%s' ignored\n",
      argv[i]);
    continue ;
      }

      filename = strtok(argv[i],".");

  }
    }

    if (filename)
  cmdFile(filename,NULL);
}

/*-----------------------------------------------------------------*/
/* setsignals -  catch some signals                                */
/*-----------------------------------------------------------------*/
#include <signal.h>
static void
bad_signal(int sig)
{
    if ( simactive )
        closeSimulator();
    exit(1);
}

static void
sigintr(int sig)
{
    /* may be interrupt from user: stop debugger ( also simulator ??) */
    userinterrupt = 1;
}

/* the only child can be the simulator */
static void sigchld(int sig)
{
    /* the only child can be the simulator */
    int status, retpid;
    retpid = wait ( &status );
    /* if ( retpid == simPid ) */
    simactive = 0;
}

static void
setsignals()
{
    signal(SIGHUP , bad_signal);		
    signal(SIGINT , sigintr );	
    signal(SIGTERM, bad_signal);	
    signal(SIGCHLD, sigchld );

    signal(SIGPIPE, SIG_IGN);
    signal(SIGABRT, bad_signal);
    signal(SIGALRM, bad_signal);
    signal(SIGFPE,  bad_signal);
    signal(SIGILL,  bad_signal);
    signal(SIGPIPE, bad_signal);
    signal(SIGQUIT, bad_signal);
    signal(SIGSEGV, bad_signal);
}

/*-----------------------------------------------------------------*/
/* main -                                                          */
/*-----------------------------------------------------------------*/

int main ( int argc, char **argv)
{
    printVersionInfo();
    printf("WARNING: SDCDB is EXPERIMENTAL and NOT A FULLY FUNCTIONING TOOL.\n");

    simArgs[nsimArgs++] = "s51";
    simArgs[nsimArgs++] = "-P";
    simArgs[nsimArgs++] = "-r 9756";
    /* parse command line */

    setsignals();
    parseCmdLine(argc,argv);

    commandLoop();

    return 0;
}
