/*-------------------------------------------------------------------------

  main.c - pic16 specific general functions.

   Written by - Scott Dattalo scott@dattalo.com
   Ported to PIC16 by - Martin Dubuc m.debuc@rogers.com

   Note that mlh prepended _pic16_ on the static functions.  Makes
   it easier to set a breakpoint using the debugger.


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

#include "common.h"
#include "main.h"
#include "ralloc.h"
#include "device.h"
#include "SDCCutil.h"
#include "glue.h"
#include "pcode.h"
#include "SDCCargs.h"
#include "dbuf_string.h"


static char _defaultRules[] =
{
#include "peeph.rul"
};

/* list of key words used by pic16 */
static char *_pic16_keywords[] =
{
  "at",
  "code",
  "critical",
  "register",
  "data",
  "far",
  "interrupt",
  "near",
  //"pdata",
  "reentrant",
  "sfr",
  "sfr16",
  "using",
  "_data",
  "_code",
  "_generic",
  "_near",
  //"_pdata",
  "_naked",
  "shadowregs",
  "wparam",
  "prodlp",
  "prodhp",
  "fsr0lp",
  "fixed16x16",

//  "bit",
//  "idata",
//  "sbit",
//  "xdata",
//  "_xdata",
//  "_idata",
  NULL
};


pic16_sectioninfo_t pic16_sectioninfo;

extern char *pic16_processor_base_name(void);

void  pic16_pCodeInitRegisters(void);

void pic16_assignRegisters (ebbIndex *);

static int regParmFlg = 0;  /* determine if we can register a parameter */

pic16_options_t pic16_options;

extern set *includeDirsSet;
extern set *dataDirsSet;
extern set *libFilesSet;

/* Also defined in gen.h, but the #include is commented out */
/* for an unknowned reason. - EEP */
void pic16_emitDebuggerSymbol (char *);

extern void pic16_emitConfigRegs(FILE *of);
extern void pic16_emitIDRegs(FILE *of);



static void
_pic16_init (void)
{
  asm_addTree (&asm_asxxxx_mapping);
  pic16_pCodeInitRegisters();
  maxInterrupts = 2;
}

static void
_pic16_reset_regparm (void)
{
  regParmFlg = 0;
}

static int
_pic16_regparm (sym_link * l, bool reentrant)
{
  /* force all parameters via SEND/RECEIVE */
  if(0 /*pic16_options.ip_stack*/) {
    /* for this processor it is simple
     * can pass only the first parameter in a register */
    if(regParmFlg)return 0;
      regParmFlg++;
      return 1; //regParmFlg;
  } else {
    /* otherwise pass all arguments in registers via SEND/RECEIVE */
    regParmFlg++;// = 1;
    return regParmFlg;
  }
}


int initsfpnt=0;        /* set to 1 if source provides a pragma for stack
                 * so glue() later emits code to initialize stack/frame pointers */
set *absSymSet;

set *sectNames=NULL;            /* list of section listed in pragma directives */
set *sectSyms=NULL;         /* list of symbols set in a specific section */
set *wparamList=NULL;

#if 0
/* This is an experimental code for #pragma inline
   and is temporarily disabled for 2.5.0 release */
set *asmInlineMap=NULL;
#endif  /* 0 */

struct {
  unsigned ignore: 1;
  unsigned want_libc: 1;
  unsigned want_libm: 1;
  unsigned want_libio: 1;
  unsigned want_libdebug: 1;
} libflags = { 0, 0, 0, 0, 0 };


enum {
  P_STACK = 1,
  P_CODE,
  P_UDATA,
  P_LIBRARY
};

static int
do_pragma(int id, const char *name, const char *cp)
{
  struct pragma_token_s token;
  int err = 0;
  int processed = 1;

  init_pragma_token(&token);

  switch (id)
    {
    /* #pragma stack [stack-position] [stack-len] */
    case  P_STACK:
      {
        unsigned int stackPos, stackLen;
        regs *reg;
        symbol *sym;

        cp = get_pragma_token(cp, &token);
        if (TOKEN_INT != token.type)
          {
            err = 1;
            break;
          }
        stackPos = token.val.int_val;

        cp = get_pragma_token(cp, &token);
        if (TOKEN_INT != token.type)
          {
            err = 1;
            break;
          }
        stackLen = token.val.int_val;

        cp = get_pragma_token(cp, &token);
        if (TOKEN_EOL != token.type)
          {
            err = 1;
            break;
          }

        if (stackLen < 1) {
          stackLen = 64;
          fprintf(stderr, "%s:%d: warning: setting stack to default size %d (0x%04x)\n",
                  filename, lineno, stackLen, stackLen);
        }

        /* check sanity of stack */
        if ((stackPos >> 8) != ((stackPos + stackLen - 1) >> 8)) {
          fprintf (stderr, "%s:%u: warning: stack [0x%03X,0x%03X] crosses memory bank boundaries (not fully tested)\n",
                  filename, lineno, stackPos, stackPos + stackLen - 1);
        }

        if (pic16) {
          if (stackPos < pic16->acsSplitOfs) {
            fprintf (stderr, "%s:%u: warning: stack [0x%03X, 0x%03X] intersects with the access bank [0x000,0x%03x] -- this is highly discouraged!\n",
                  filename, lineno, stackPos, stackPos + stackLen - 1, pic16->acsSplitOfs);
          }

          if (stackPos+stackLen > 0xF00 + pic16->acsSplitOfs) {
            fprintf (stderr, "%s:%u: warning: stack [0x%03X,0x%03X] intersects with special function registers [0x%03X,0xFFF]-- this is highly discouraged!\n",
                   filename, lineno, stackPos, stackPos + stackLen - 1, 0xF00 + pic16->acsSplitOfs);
          }

          if (stackPos+stackLen > pic16->RAMsize) {
            fprintf (stderr, "%s:%u: error: stack [0x%03X,0x%03X] is placed outside available memory [0x000,0x%03X]!\n",
                  filename, lineno, stackPos, stackPos + stackLen - 1, pic16->RAMsize-1);
            err = 1;
            break;
          }
        }

        reg = newReg(REG_SFR, PO_SFR_REGISTER, stackPos, "_stack", stackLen-1, 0, NULL);
        addSet(&pic16_fix_udata, reg);

        reg = newReg(REG_SFR, PO_SFR_REGISTER, stackPos + stackLen-1, "_stack_end", 1, 0, NULL);
        addSet(&pic16_fix_udata, reg);

        sym = newSymbol("stack", 0);
        sprintf(sym->rname, "_%s", sym->name);
        addSet(&publics, sym);

        sym = newSymbol("stack_end", 0);
        sprintf(sym->rname, "_%s", sym->name);
        addSet(&publics, sym);

        initsfpnt = 1;    // force glue() to initialize stack/frame pointers */
      }
      break;

    /* #pragma code [symbol] [location] */
    case P_CODE:
      {
        absSym *absS;

        cp = get_pragma_token(cp, &token);
        if (TOKEN_STR != token.type)
          goto code_err;

        absS = Safe_calloc(1, sizeof(absSym));
        sprintf(absS->name, "_%s", get_pragma_string(&token));

        cp = get_pragma_token(cp, &token);
        if (TOKEN_INT != token.type)
          {
          code_err:
            //fprintf (stderr, "%s:%d: #pragma code [symbol] [location] -- symbol or location missing\n", filename, lineno);
            err = 1;
            break;
          }
        absS->address = token.val.int_val;

        cp = get_pragma_token(cp, &token);
        if (TOKEN_EOL != token.type)
          {
            err = 1;
            break;
          }

        if ((absS->address % 2) != 0) {
          absS->address--;
          fprintf(stderr, "%s:%d: warning: code memory locations should be word aligned, will locate to 0x%06x instead\n",
                  filename, lineno, absS->address);
        }

        addSet(&absSymSet, absS);
//      fprintf(stderr, "%s:%d symbol %s will be placed in location 0x%06x in code memory\n",
//        __FILE__, __LINE__, symname, absS->address);
      }
      break;

    /* #pragma udata [section-name] [symbol] */
    case P_UDATA:
      {
        char *sectname;
        const char *symname;
        symbol *nsym;
        sectSym *ssym;
        sectName *snam;
        int found = 0;

        cp = get_pragma_token(cp, &token);
        if (TOKEN_STR == token.type)
          sectname = Safe_strdup(get_pragma_string(&token));
        else
          {
            err = 1;
            break;
          }

        cp = get_pragma_token(cp, &token);
        if (TOKEN_STR == token.type)
          symname = get_pragma_string(&token);
        else
          {
            //fprintf (stderr, "%s:%d: #pragma udata [section-name] [symbol] -- section-name or symbol missing!\n", filename, lineno);
            err = 1;
            symname = NULL;
          }

        while (symname)
          {
            ssym = Safe_calloc(1, sizeof(sectSym));
            ssym->name = Safe_calloc(1, strlen(symname) + 2);
            sprintf(ssym->name, "%s%s", port->fun_prefix, symname);
            ssym->reg = NULL;

            addSet(&sectSyms, ssym);

            nsym = newSymbol((char *)symname, 0);
            strcpy(nsym->rname, ssym->name);

#if 0
            checkAddSym(&publics, nsym);
#endif

            found = 0;
            for (snam = setFirstItem(sectNames);snam;snam=setNextItem(sectNames))
              {
                if (!strcmp(sectname, snam->name))
                  {
                    found=1;
                    break;
                  }
              }

            if(!found)
              {
                snam = Safe_calloc(1, sizeof(sectName));
                snam->name = Safe_strdup(sectname);
                snam->regsSet = NULL;

                addSet(&sectNames, snam);
              }

            ssym->section = snam;

#if 0
            fprintf(stderr, "%s:%d placing symbol %s at section %s (%p)\n", __FILE__, __LINE__,
               ssym->name, snam->name, snam);
#endif

            cp = get_pragma_token(cp, &token);
            if (TOKEN_STR == token.type)
              symname = get_pragma_string(&token);
            else if (TOKEN_EOL == token.type)
              symname = NULL;
            else
              {
                err = 1;
                symname = NULL;
              }
          }

          Safe_free(sectname);
      }
      break;

    /* #pragma library library_module */
    case P_LIBRARY:
      {
        const char *lmodule;

        cp = get_pragma_token(cp, &token);
        if (TOKEN_EOL != token.type)
          {
            lmodule = get_pragma_string(&token);

            /* lmodule can be:
             * c    link the C library
             * math link the math library
             * io   link the IO library
             * debug    link the debug libary
             * anything else, will link as-is */

            if(!strcmp(lmodule, "c"))
              libflags.want_libc = 1;
            else if(!strcmp(lmodule, "math"))
              libflags.want_libm = 1;
            else if(!strcmp(lmodule, "io"))
              libflags.want_libio = 1;
            else if(!strcmp(lmodule, "debug"))
              libflags.want_libdebug = 1;
            else if(!strcmp(lmodule, "ignore"))
              libflags.ignore = 1;
            else
              {
                if(!libflags.ignore)
                  {
                    fprintf(stderr, "link library %s\n", lmodule);
                    addSetHead(&libFilesSet, (char *)lmodule);
                  }
              }
          }
        else
          {
            err = 1;
            break;
          }

        cp = get_pragma_token(cp, &token);
        if (TOKEN_EOL != token.type)
          {
            err = 1;
            break;
          }
      }
      break;

#if 0
  /* This is an experimental code for #pragma inline
     and is temporarily disabled for 2.5.0 release */
    case P_INLINE:
      {
        char *tmp = strtok((char *)NULL, WHITECOMMA);

        while(tmp) {
          addSet(&asmInlineMap, Safe_strdup( tmp ));
          tmp = strtok((char *)NULL, WHITECOMMA);
        }

        {
          char *s;

          for(s = setFirstItem(asmInlineMap); s ; s = setNextItem(asmInlineMap)) {
            debugf("inline asm: `%s'\n", s);
          }
        }
      }
      break;
#endif  /* 0 */

    default:
      processed = 0;
      break;
  }

  get_pragma_token(cp, &token);

  if (1 == err)
    werror(W_BAD_PRAGMA_ARGUMENTS, name);

  free_pragma_token(&token);
  return processed;
}

static struct pragma_s pragma_tbl[] = {
  { "stack",   P_STACK,   0, do_pragma },
  { "code",    P_CODE,    0, do_pragma },
  { "udata",   P_UDATA,   0, do_pragma },
  { "library", P_LIBRARY, 0, do_pragma },
/*{ "inline",  P_INLINE,  0, do_pragma }, */
  { NULL,      0,         0, NULL },
  };

static int
_process_pragma(const char *s)
{
  return process_pragma_tbl(pragma_tbl, s);
}

#define REP_UDATA         "--preplace-udata-with="

#define STACK_MODEL       "--pstack-model="
#define OPT_BANKSEL       "--obanksel="

#define ALT_ASM           "--asm="
#define ALT_LINK          "--link="

#define IVT_LOC           "--ivt-loc="
#define NO_DEFLIBS        "--nodefaultlibs"
#define MPLAB_COMPAT      "--mplab-comp"

#define USE_CRT           "--use-crt="

#define OFMSG_LRSUPPORT   "--flr-support"

#define NO_OPTIMIZE_GOTO  "--no-optimize-goto"
#define OPTIMIZE_CMP      "--optimize-cmp"
#define OPTIMIZE_DF       "--optimize-df"

char *alt_asm = NULL;
char *alt_link = NULL;

int pic16_mplab_comp = 0;
extern int pic16_debug_verbose;
extern int pic16_ralloc_debug;
extern int pic16_pcode_verbose;

int pic16_enable_peeps = 0;

OPTION pic16_optionsTable[]= {
    /* code generation options */
    { 0, STACK_MODEL,        NULL, "use stack model 'small' (default) or 'large'"},
#if XINST
    { 'y', "--extended",     &pic16_options.xinst, "enable Extended Instruction Set/Literal Offset Addressing mode"},
#endif
    { 0, "--pno-banksel",    &pic16_options.no_banksel, "do not generate BANKSEL assembler directives"},

    /* optimization options */
    { 0, OPT_BANKSEL,       &pic16_options.opt_banksel, "set banksel optimization level (default=0 no)", CLAT_INTEGER },
    { 0, "--denable-peeps", &pic16_enable_peeps, "explicit enable of peepholes"},
    { 0, NO_OPTIMIZE_GOTO,  NULL, "do NOT use (conditional) BRA instead of GOTO"},
    { 0, OPTIMIZE_CMP,      NULL, "try to optimize some compares"},
    { 0, OPTIMIZE_DF,       NULL, "thoroughly analyze data flow (memory and time intensive!)"},

    /* assembling options */
    { 0, ALT_ASM,           &alt_asm, "Use alternative assembler", CLAT_STRING},
    { 0, MPLAB_COMPAT,      &pic16_mplab_comp, "enable compatibility mode for MPLAB utilities (MPASM/MPLINK)"},

    /* linking options */
    { 0, ALT_LINK,          &alt_link, "Use alternative linker", CLAT_STRING },
    { 0, REP_UDATA,         &pic16_sectioninfo.at_udata, "Place udata variables at another section: udata_acs, udata_ovr, udata_shr", CLAT_STRING },
    { 0, IVT_LOC,           NULL, "Set address of interrupt vector table."},
    { 0, NO_DEFLIBS,        &pic16_options.nodefaultlibs,   "do not link default libraries when linking"},
    { 0, USE_CRT,           NULL, "use <crt-o> run-time initialization module"},
    { 0, "--no-crt",        &pic16_options.no_crt, "do not link any default run-time initialization module"},

    /* debugging options */
    { 0, "--debug-xtra",    &pic16_debug_verbose, "show more debug info in assembly output"},
    { 0, "--debug-ralloc",  &pic16_ralloc_debug, "dump register allocator debug file *.d"},
    { 0, "--pcode-verbose", &pic16_pcode_verbose, "dump pcode related info"},
    { 0, "--calltree",      &pic16_options.dumpcalltree, "dump call tree in .calltree file"},
    { 0, "--gstack",        &pic16_options.gstack, "trace stack pointer push/pop to overflow"},
    { 0, NULL,              NULL, NULL}
};


#define ISOPT(str)  !strncmp(argv[ *i ], str, strlen(str) )

static bool
_pic16_parseOptions (int *pargc, char **argv, int *i)
{
  int j=0;
  char *stkmodel;

  /* TODO: allow port-specific command line options to specify
   * segment names here.
   */

    /* check for arguments that have associated an integer variable */
    while(pic16_optionsTable[j].pparameter) {
      if(ISOPT( pic16_optionsTable[j].longOpt )) {
        (*(int *)pic16_optionsTable[j].pparameter)++;
        return TRUE;
      }
      j++;
    }

    if(ISOPT(STACK_MODEL)) {
      stkmodel = getStringArg(STACK_MODEL, argv, i, *pargc);
      if(!STRCASECMP(stkmodel, "small"))pic16_options.stack_model = 0;
      else if(!STRCASECMP(stkmodel, "large"))pic16_options.stack_model = 1;
      else {
        fprintf(stderr, "Unknown stack model: %s", stkmodel);
        exit(EXIT_FAILURE);
      }
      return TRUE;
    }

    if(ISOPT(IVT_LOC)) {
      pic16_options.ivt_loc = getIntArg(IVT_LOC, argv, i, *pargc);
      fprintf(stderr, "%s:%d setting interrupt vector addresses 0x%x\n", __FILE__, __LINE__, pic16_options.ivt_loc);
      return TRUE;
    }

    if(ISOPT(USE_CRT)) {
      pic16_options.no_crt = 0;
      pic16_options.crt_name = Safe_strdup( getStringArg(USE_CRT, argv, i, *pargc) );

      return TRUE;
    }

#if 0
    if(ISOPT(OFMSG_LRSUPPORT)) {
      pic16_options.opt_flags |= OF_LR_SUPPORT;
      return TRUE;
    }
#endif

    if (ISOPT(NO_OPTIMIZE_GOTO)) {
      pic16_options.opt_flags |= OF_NO_OPTIMIZE_GOTO;
      return TRUE;
    }

    if(ISOPT(OPTIMIZE_CMP)) {
      pic16_options.opt_flags |= OF_OPTIMIZE_CMP;
      return TRUE;
    }

    if (ISOPT(OPTIMIZE_DF)) {
      pic16_options.opt_flags |= OF_OPTIMIZE_DF;
      return TRUE;
    }


  return FALSE;
}

extern void pic16_init_pic(const char *name);

static void _pic16_initPaths(void)
{
    set *pic16libDirsSet=NULL;

    if (!options.nostdlib) {
        struct dbuf_s pic16libDir;

        dbuf_init(&pic16libDir, 128);
        dbuf_makePath(&pic16libDir, LIB_DIR_SUFFIX, "pic16");
        pic16libDirsSet = appendStrSet(dataDirsSet, NULL, dbuf_c_str(&pic16libDir));
        dbuf_destroy(&pic16libDir);
        mergeSets(&pic16libDirsSet, libDirsSet);
        libDirsSet = pic16libDirsSet;
    }

    /* now that we have the paths set up... */
    pic16_init_pic(port->processor);
}

extern set *linkOptionsSet;
char *msprintf(hTab *pvals, const char *pformat, ...);
int my_system(const char *cmd);

/* forward declarations */
extern const char *pic16_linkCmd[];
extern const char *pic16_asmCmd[];
extern set *asmOptionsSet;

/* custom function to link objects */
static void _pic16_linkEdit(void)
{
  hTab *linkValues=NULL;
  char lfrm[1024];
  char *lcmd;
  char temp[1024];
  set *tSet=NULL;
  int ret;

    /*
     * link command format:
     * {linker} {incdirs} {lflags} -o {outfile} {spec_ofiles} {ofiles} {libs}
     *
     */
    sprintf(lfrm, "{linker} {incdirs} {lflags} -w -r -o {outfile} {user_ofile} {ofiles} {spec_ofiles} {libs}");

    shash_add(&linkValues, "linker", pic16_linkCmd[0]);

    mergeSets(&tSet, libDirsSet);
    mergeSets(&tSet, libPathsSet);

    shash_add(&linkValues, "incdirs", joinStrSet( appendStrSet(tSet, "-I\"", "\"")));
    shash_add(&linkValues, "lflags", joinStrSet(linkOptionsSet));

    shash_add(&linkValues, "outfile", fullDstFileName ? fullDstFileName : dstFileName);

    if(fullSrcFileName) {
        sprintf(temp, "%s.o", fullDstFileName ? fullDstFileName : dstFileName);
//      addSetHead(&relFilesSet, Safe_strdup(temp));
                shash_add(&linkValues, "user_ofile", temp);
    }

    if(!pic16_options.no_crt)
          shash_add(&linkValues, "spec_ofiles", pic16_options.crt_name);

    shash_add(&linkValues, "ofiles", joinStrSet(relFilesSet));

    if(!libflags.ignore) {
      if(libflags.want_libc)
        addSet(&libFilesSet, Safe_strdup("libc18f.lib"));

          if(libflags.want_libm)
            addSet(&libFilesSet, Safe_strdup("libm18f.lib"));

          if(libflags.want_libio) {
            sprintf(temp, "libio%s.lib", pic16->name[1]);   /* build libio18f452.lib name */
            addSet(&libFilesSet, Safe_strdup(temp));
          }

          if(libflags.want_libdebug)
            addSet(&libFilesSet, Safe_strdup("libdebug.lib"));
        }

    shash_add(&linkValues, "libs", joinStrSet(libFilesSet));

    lcmd = msprintf(linkValues, lfrm);

    ret = my_system( lcmd );

    Safe_free( lcmd );

    if(ret)
        exit(1);
}


static void
_pic16_finaliseOptions (void)
{
  port->mem.default_local_map = data;
  port->mem.default_globl_map = data;

  /* peepholes are disabled for the time being */
  options.nopeep = 1;

  /* explicit enable peepholes for testing */
  if (pic16_enable_peeps)
    options.nopeep = 0;

  options.all_callee_saves = 1;       // always callee saves

#if 0
  options.float_rent = 1;
  options.intlong_rent = 1;
#endif

  setMainValue("mcu", pic16->name[2] );
  addSet(&preArgvSet, Safe_strdup("-D{mcu}"));

  setMainValue("mcu1", pic16->name[1] );
  addSet(&preArgvSet, Safe_strdup("-D__{mcu1}"));

  if (!pic16_options.nodefaultlibs)
    {
      char devlib[512];

      /* now add the library for the device */
      sprintf(devlib, "libdev%s.lib", pic16->name[1]);   /* e.g., libdev18f452.lib */
      addSet(&libFilesSet, Safe_strdup(devlib));

      /* add the internal SDCC library */
      addSet(&libFilesSet, Safe_strdup( "libsdcc.lib" ));
    }

  if (alt_asm && alt_asm[0] != '\0')
    {
      size_t len = strlen(alt_asm);
      char *cmd = malloc(len + 3);

      if (0)
        {
          // fails for --asm="/bin/sh script.sh --options"
          cmd[0] = '"';
          memcpy(&cmd[1], alt_asm, len);
          cmd[len + 1] = '"';
          cmd[len + 2] = '\0';
        }
      else
        {
          // requires --asm="'my prog with spaces'"
          memcpy(&cmd[0], alt_asm, len);
          cmd[len] = 0;
        }
      pic16_asmCmd[0] = cmd;
    }

  if (alt_link && alt_link[0] != '\0')
    {
      size_t len = strlen(alt_link);
      char *cmd = malloc(len + 3);

      if (0)
        {
          // fails for --link="/bin/sh script.sh --options"
          cmd[0] = '"';
          memcpy(&cmd[1], alt_link, len);
          cmd[len + 1] = '"';
          cmd[len + 2] = '\0';
        }
      else
        {
          // requires --link="'my prog with spaces'"
          memcpy(&cmd[0], alt_link, len);
          cmd[len] = 0;
        }
      pic16_linkCmd[0] = cmd;
    }

  if (!pic16_options.no_crt)
    {
      pic16_options.omit_ivt = 1;
      pic16_options.leave_reset = 0;
    }

  if (options.model == MODEL_SMALL)
    {
      addSet(&asmOptionsSet, Safe_strdup("-DSDCC_MODEL_SMALL"));
    }
  else if(options.model == MODEL_LARGE)
    {
      char buf[128];

      addSet(&asmOptionsSet, Safe_strdup("-DSDCC_MODEL_LARGE"));

      sprintf(buf, "-D%s -D__%s", pic16->name[2], pic16->name[1]);
      *(strrchr(buf, 'f')) = 'F';
      addSet(&asmOptionsSet, Safe_strdup(buf));
    }

  if (STACK_MODEL_LARGE)
    {
      addSet(&preArgvSet, Safe_strdup("-DSTACK_MODEL_LARGE"));
      addSet(&asmOptionsSet, Safe_strdup("-DSTACK_MODEL_LARGE"));
    }
  else
    {
      addSet(&preArgvSet, Safe_strdup("-DSTACK_MODEL_SMALL"));
      addSet(&asmOptionsSet, Safe_strdup("-DSTACK_MODEL_SMALL"));
    }
}


static void
_pic16_setDefaultOptions (void)
{
  options.stackAuto = 0;        /* implicit declaration */
  /* port is not capable yet to allocate separate registers
   * dedicated for passing certain parameters */

  /* initialize to defaults section locations, names and addresses */
  pic16_sectioninfo.at_udata    = "udata";

  /* set pic16 port options to defaults */
  pic16_options.no_banksel = 0;
  pic16_options.opt_banksel = 0;
  pic16_options.omit_configw = 0;
  pic16_options.omit_ivt = 0;
  pic16_options.leave_reset = 0;
  pic16_options.stack_model = 0;            /* 0 for 'small', 1 for 'large' */
  pic16_options.ivt_loc = 0x000000;
  pic16_options.nodefaultlibs = 0;
  pic16_options.dumpcalltree = 0;
  pic16_options.crt_name = "crt0i.o";       /* the default crt to link */
  pic16_options.no_crt = 0;         /* use crt by default */
  pic16_options.ip_stack = 1;       /* set to 1 to enable ipop/ipush for stack */
  pic16_options.gstack = 0;
  pic16_options.debgen = 0;
}

static const char *
_pic16_getRegName (struct regs *reg)
{
  if (reg)
    return reg->name;
  return "err";
}


#if 1
static  char *_pic16_mangleFunctionName(char *sz)
{
//  fprintf(stderr, "mangled function name: %s\n", sz);

  return sz;
}
#endif


static void
_pic16_genAssemblerPreamble (FILE * of)
{
  char *name = pic16_processor_base_name();

    if(!name) {
        name = "p18f452";
        fprintf(stderr,"WARNING: No Pic has been selected, defaulting to %s\n",name);
    }

    fprintf (of, "\tlist\tp=%s\n",&name[1]);
    if (pic16_mplab_comp) {
      // provide ACCESS macro used during SFR accesses
      fprintf (of, "\tinclude <p%s.inc>\n", &name[1]);
    }

    if(!pic16_options.omit_configw) {
        pic16_emitConfigRegs(of);
        fprintf(of, "\n");
        pic16_emitIDRegs(of);
    }

  fprintf (of, "\tradix dec\n");
}

/* Generate interrupt vector table. */
static int
_pic16_genIVT (struct dbuf_s * oBuf, symbol ** interrupts, int maxInterrupts)
{
#if 1
    /* PIC18F family has only two interrupts, the high and the low
     * priority interrupts, which reside at 0x0008 and 0x0018 respectively - VR */

    if((!pic16_options.omit_ivt) || (pic16_options.omit_ivt && pic16_options.leave_reset)) {
        dbuf_printf(oBuf, "; RESET vector\n");
        dbuf_printf(oBuf, "\tgoto\t__sdcc_gsinit_startup\n");
    }

    if(!pic16_options.omit_ivt) {
        dbuf_printf(oBuf, "\tres 4\n");


        dbuf_printf(oBuf, "; High priority interrupt vector 0x0008\n");
        if(interrupts[1]) {
            dbuf_printf(oBuf, "\tgoto\t%s\n", interrupts[1]->rname);
            dbuf_printf(oBuf, "\tres\t12\n");
        } else {
            dbuf_printf(oBuf, "\tretfie\n");
            dbuf_printf(oBuf, "\tres\t14\n");
        }

        dbuf_printf(oBuf, "; Low priority interrupt vector 0x0018\n");
        if(interrupts[2]) {
            dbuf_printf(oBuf, "\tgoto\t%s\n", interrupts[2]->rname);
        } else {
            dbuf_printf(oBuf, "\tretfie\n");
        }
    }
#endif
  return TRUE;
}

/* return True if the port can handle the type,
 * False to convert it to function call */
static bool _hasNativeMulFor (iCode *ic, sym_link *left, sym_link *right)
{
  //fprintf(stderr,"checking for native mult for %c (size: %d)\n", ic->op, getSize(OP_SYMBOL(IC_RESULT(ic))->type));
  int symL, symR, symRes, sizeL = 0, sizeR = 0, sizeRes = 0;

  /* left/right are symbols? */
  symL = IS_SYMOP(IC_LEFT(ic));
  symR = IS_SYMOP(IC_RIGHT(ic));
  symRes = IS_SYMOP(IC_RESULT(ic));

  /* --> then determine their sizes */
  sizeL = symL ? getSize(OP_SYM_TYPE(IC_LEFT(ic))) : 4;
  sizeR = symR ? getSize(OP_SYM_TYPE(IC_RIGHT(ic))) : 4;
  sizeRes = symRes ? getSize(OP_SYM_TYPE(IC_RESULT(ic))) : 4;

  /* Checks to enable native multiplication.
   * PICs do not offer native division at all...
   *
   * Ideas:
   * (  i) if result is just one byte, use native MUL
   *       (regardless of the operands)
   * ( ii) if left and right are unsigned 8-bit operands,
   *       use native MUL
   * (iii) if left or right is a literal in the range of [-128..256)
   *       and the other is an unsigned byte, use native MUL
   */
  if (ic->op == '*')
  {
    /* use native mult for `*: <?> x <?> --> {u8_t, s8_t}' */
    if (sizeRes == 1) { return TRUE; }

    /* use native mult for `u8_t x u8_t --> { u16_t, s16_t }' */
    if (sizeL == 1 && symL /*&& SPEC_USIGN(OP_SYM_TYPE(IC_LEFT(ic)))*/) {
      sizeL = 1;
    } else {
      //printf( "%s: left too large (%u) / signed (%u)\n", __FUNCTION__, sizeL, symL && !SPEC_USIGN(OP_SYM_TYPE(IC_LEFT(ic))));
      sizeL = 4;
    }
    if (sizeR == 1 && symR /*&& SPEC_USIGN(OP_SYM_TYPE(IC_RIGHT(ic)))*/) {
      sizeR = 1;
    } else {
      //printf( "%s: right too large (%u) / signed (%u)\n", __FUNCTION__, sizeR, symR && !SPEC_USIGN(OP_SYM_TYPE(IC_RIGHT(ic))));
      sizeR = 4;
    }

    /* also allow literals [-128..256) for left/right operands */
    if (IS_VALOP(IC_LEFT(ic)))
    {
      long l = (long) ulFromVal ( OP_VALUE( IC_LEFT(ic) ) );
      sizeL = 4;
      //printf( "%s: val(left) = %ld\n", __FUNCTION__, l );
      if (l >= -128 && l < 256)
      {
    sizeL = 1;
      } else {
    //printf( "%s: left value %ld outside [-128..256)\n", __FUNCTION__, l );
      }
    }
    if (IS_VALOP( IC_RIGHT(ic) ))
    {
      long l = (long) ulFromVal ( OP_VALUE( IC_RIGHT(ic) ) );
      sizeR = 4;
      //printf( "%s: val(right) = %ld\n", __FUNCTION__, l );
      if (l >= -128 && l < 256)
      {
    sizeR = 1;
      } else {
    //printf( "%s: right value %ld outside [-128..256)\n", __FUNCTION__, l );
      }
    }

    /* use native mult iff left and right are (unsigned) 8-bit operands */
    if (sizeL == 1 && sizeR == 1) { return TRUE; }
  }

  if (ic->op == '/' || ic->op == '%')
  {
    /* We must catch /: {u8_t,s8_t} x {u8_t,s8_t} --> {u8_t,s8_t},
     * because SDCC will call 'divuchar' even for u8_t / s8_t.
     * Example: 128 / -2 becomes 128 / 254 = 0 != -64... */
    if (sizeL == 1 && sizeR == 1) return TRUE;

    /* What about literals? */
    if (IS_VALOP( IC_LEFT(ic) ))
    {
      long l = (long) ulFromVal ( OP_VALUE( IC_LEFT(ic) ) );
      sizeL = 4;
      //printf( "%s: val(left) = %ld\n", __FUNCTION__, l );
      if (l >= -128 && l < 256)
      {
    sizeL = 1;
      } else {
    //printf( "%s: left value %ld outside [-128..256)\n", __FUNCTION__, l );
      }
    }
    if (IS_VALOP( IC_RIGHT(ic) ))
    {
      long l = (long) ulFromVal ( OP_VALUE( IC_RIGHT(ic) ) );
      sizeR = 4;
      //printf( "%s: val(right) = %ld\n", __FUNCTION__, l );
      if (l >= -128 && l < 256)
      {
    sizeR = 1;
      } else {
    //printf( "%s: right value %ld outside [-128..256)\n", __FUNCTION__, l );
      }
    }
    if (sizeL == 1 && sizeR == 1) { return TRUE; }
  }

  return FALSE;
}


#if 0
/* Do CSE estimation */
static bool cseCostEstimation (iCode *ic, iCode *pdic)
{
//    operand *result = IC_RESULT(ic);
//    sym_link *result_type = operandType(result);


    /* VR -- this is an adhoc. Put here after conversation
     * with Erik Epetrich */

    if(ic->op == '<'
        || ic->op == '>'
        || ic->op == EQ_OP) {

        fprintf(stderr, "%d %s\n", __LINE__, __FUNCTION__);
      return 0;
    }

#if 0
    /* if it is a pointer then return ok for now */
    if (IC_RESULT(ic) && IS_PTR(result_type)) return 1;

    /* if bitwise | add & subtract then no since mcs51 is pretty good at it
       so we will cse only if they are local (i.e. both ic & pdic belong to
       the same basic block */
    if (IS_BITWISE_OP(ic) || ic->op == '+' || ic->op == '-') {
        /* then if they are the same Basic block then ok */
        if (ic->eBBlockNum == pdic->eBBlockNum) return 1;
        else return 0;
    }
#endif

    /* for others it is cheaper to do the cse */
    return 1;
}
#endif


/* Indicate which extended bit operations this port supports */
static bool
hasExtBitOp (int op, int size)
{
  if (op == RRC
      || op == RLC
      /* || op == GETHBIT */ /* GETHBIT doesn't look complete for PIC */
     )
    return TRUE;
  else
    return FALSE;
}

/* Indicate the expense of an access to an output storage class */
static int
oclsExpense (struct memmap *oclass)
{
  /* The IN_FARSPACE test is compatible with historical behaviour, */
  /* but I don't think it is applicable to PIC. If so, please feel */
  /* free to remove this test -- EEP */
  if (IN_FARSPACE(oclass))
    return 1;

  return 0;
}

/** $1 is the input object file (PIC16 specific)    // >>always the basename<<.
    $2 is always the output file.
    $3 -L path and -l libraries
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
const char *pic16_linkCmd[] =
{
  "gplink", "$l", "-w", "-r", "-o \"$2\"", "\"$1\"","$3", NULL
};



/** $1 is always the basename.
    $2 is always the output file.
    $3 varies (nothing currently)
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
const char *pic16_asmCmd[] =
{
  "gpasm", "$l", "$3", "-c", "\"$1.asm\"", "-o \"$2\"", NULL

};

/* Globals */
PORT pic16_port =
{
  TARGET_ID_PIC16,
  "pic16",
  "MCU PIC16",      /* Target name */
  "p18f452",        /* Processor */
  {
    pic16glue,
    TRUE,           /* Emit glue around main */
    MODEL_SMALL | MODEL_LARGE | MODEL_FLAT24,
    MODEL_SMALL
  },
  {
    pic16_asmCmd,   /* assembler command and arguments */
    NULL,           /* alternate macro based form */
    "-g",           /* arguments for debug mode */
    NULL,           /* arguments for normal mode */
    0,              /* print externs as global */
    ".asm",         /* assembler file extension */
    NULL            /* no do_assemble function */
  },
  {
    NULL,           //    pic16_linkCmd,        /* linker command and arguments */
    NULL,           /* alternate macro based form */
    _pic16_linkEdit,        //NULL,         /* no do_link function */
    ".o",           /* extension for object files */
    0               /* no need for linker file */
  },
  {
    _defaultRules
  },
  {
    /* Sizes */
    1,      /* char */
    2,      /* short */
    2,      /* int */
    4,      /* long */
    2,      /* ptr */
    3,      /* fptr, far pointers (see Microchip) */
    3,      /* gptr */
    1,      /* bit */
    4,      /* float */
    4       /* max */
  },

    /* generic pointer tags */
  {
    0x00,   /* far */
    0x80,   /* near */
    0x00,   /* xstack */
    0x00    /* code */
  },

  {
    "XSEG    (XDATA)",      // xstack
    "STACK   (DATA)",       // istack
    "CSEG    (CODE)",       // code
    "DSEG    (DATA)",       // data
    "ISEG    (DATA)",       // idata
    "PSEG    (DATA)",       // pdata
    "XSEG    (XDATA)",      // xdata
    "BSEG    (BIT)",        // bit
    "RSEG    (DATA)",       // reg
    "GSINIT  (CODE)",       // static
    "OSEG    (OVR,DATA)",   // overlay
    "GSFINAL (CODE)",       // post static
    "HOME    (CODE)",       // home
    NULL,                   // xidata
    NULL,                   // xinit
    "CONST   (CODE)",       // const_name - const data (code or not)
    "CABS    (ABS,CODE)",   // cabs_name - const absolute data (code or not)
    "XABS    (ABS,XDATA)",  // xabs_name - absolute xdata
    "IABS    (ABS,DATA)",   // iabs_name - absolute data
    NULL,                   // default location for auto vars
    NULL,                   // default location for global vars
    1                       // code is read only 1=yes
  },
  {
    NULL,       /* genExtraAreaDeclaration */
    NULL        /* genExatrAreaLinkOptions */
  },
  {
    /* stack related information */
    -1,         /* -1 stack grows downwards, +1 upwards */
    1,          /* extra overhead when calling between banks */
    4,          /* extra overhead when the function is an ISR */
    1,          /* extra overhead for a function call */
    1,          /* re-entrant space */
    0           /* 'banked' call overhead, mild overlap with bank_overhead */
  },
    /* pic16 has an 8 bit mul */
  {
     0, -1
  },
  {
    pic16_emitDebuggerSymbol
  },
  {
    255/3,      /* maxCount */
    3,          /* sizeofElement */
    /* The rest of these costs are bogus. They approximate */
    /* the behavior of src/SDCCicode.c 1.207 and earlier.  */
    {4,4,4},    /* sizeofMatchJump[] */
    {0,0,0},    /* sizeofRangeCompare[] */
    0,          /* sizeofSubtract */
    3,          /* sizeofDispatch */
  },
  "_",
  _pic16_init,
  _pic16_parseOptions,
  pic16_optionsTable,
  _pic16_initPaths,
  _pic16_finaliseOptions,
  _pic16_setDefaultOptions,
  pic16_assignRegisters,
  _pic16_getRegName,
  _pic16_keywords,
  _pic16_genAssemblerPreamble,
  NULL,             /* no genAssemblerEnd */
  _pic16_genIVT,
  NULL, // _pic16_genXINIT
  NULL,             /* genInitStartup */
  _pic16_reset_regparm,
  _pic16_regparm,
  _process_pragma,  /* process a pragma */
  _pic16_mangleFunctionName, /* mangles function name */
  _hasNativeMulFor,
  hasExtBitOp,      /* hasExtBitOp */
  oclsExpense,      /* oclsExpense */
  FALSE,
  TRUE,             /* little endian */
  0,                /* leave lt */
  0,                /* leave gt */
  1,                /* transform <= to ! > */
  1,                /* transform >= to ! < */
  1,                /* transform != to !(a == b) */
  0,                /* leave == */
  FALSE,            /* No array initializer support. */
  0,    //cseCostEstimation,            /* !!!no CSE cost estimation yet */
  NULL,             /* no builtin functions */
  GPOINTER,         /* treat unqualified pointers as "generic" pointers */
  1,                /* reset labelKey to 1 */
  1,                /* globals & local static allowed */
  PORT_MAGIC
};
