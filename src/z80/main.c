/*-------------------------------------------------------------------------
  main.c - Z80 specific definitions.

  Michael Hope <michaelh@juju.net.nz> 2001

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

#include <sys/stat.h>
#include "z80.h"
#include "MySystem.h"
#include "BuildCmd.h"
#include "SDCCutil.h"
#include "dbuf.h"

static char _z80_defaultRules[] =
{
#include "peeph.rul"
#include "peeph-z80.rul"
};

static char _gbz80_defaultRules[] =
{
#include "peeph.rul"
#include "peeph-gbz80.rul"
};

Z80_OPTS z80_opts;

static OPTION _z80_options[] = 
  {
    {  0,   "--callee-saves-bc", &z80_opts.calleeSavesBC, "Force a called function to always save BC" },
    {  0,   "--portmode=",       NULL,                    "Determine PORT I/O mode (z80/z180)" },
    {  0, NULL }
  };

static OPTION _gbz80_options[] = 
  {
    {  0,   "--callee-saves-bc", &z80_opts.calleeSavesBC, "Force a called function to always save BC" },
    {  0, NULL }
  };

typedef enum
  {
    /* Must be first */
    ASM_TYPE_ASXXXX,
    ASM_TYPE_RGBDS,
    ASM_TYPE_ISAS,
    ASM_TYPE_Z80ASM
  }
ASM_TYPE;

static struct
  {
    ASM_TYPE asmType;
    /* determine if we can register a parameter */    
    int regParams;
  }
_G;

static char *_keywords[] =
{
  "sfr",
  "nonbanked",
  "banked",
  "at",       //.p.t.20030714 adding support for 'sfr at ADDR' construct
  "_naked",   //.p.t.20030714 adding support for '_naked' functions
  "critical",
  "interrupt",
  NULL
};

extern PORT gbz80_port;
extern PORT z80_port;

#include "mappings.i"

static builtins _z80_builtins[] = {
  /* Disabled for now.
    { "__builtin_strcpy", "v", 2, {"cg*", "cg*" } },
    { "__builtin_memcpy", "cg*", 3, {"cg*", "cg*", "ui" } },
  */
    { NULL , NULL,0, {NULL}}
};    

static void
_z80_init (void)
{
  z80_opts.sub = SUB_Z80;
  asm_addTree (&_asxxxx_z80);
}

static void
_gbz80_init (void)
{
  z80_opts.sub = SUB_GBZ80;
}

static void
_reset_regparm (void)
{
  _G.regParams = 0;
}

static int
_reg_parm (sym_link * l, bool reentrant)
{
  if (options.noRegParams) 
    {
      return FALSE;
    }
  else 
    {
      if (_G.regParams == 2)
        {
          return FALSE;
        }
      else
        {
          _G.regParams++;
          return TRUE;
        }
    }
}
static int
_process_pragma (const char *sz)
{
  if( startsWith( sz, "bank=" ) || startsWith( sz, "bank " ))
  {
    char buffer[128];
    
    if (sz[4]=='=')
      werror(W_DEPRECATED_PRAGMA, "bank=");
    
    strncpy (buffer, sz + 5, sizeof (buffer));
    buffer[sizeof (buffer) - 1 ] = '\0';
    chomp (buffer);
    if (isdigit ((unsigned char)buffer[0]))
    {

    }
    else if (!strcmp (buffer, "BASE"))
    {
      strcpy (buffer, "HOME");
    }
    if (isdigit ((unsigned char)buffer[0]))
    {
	  /* Arg was a bank number.  Handle in an ASM independent
	     way. */
      char num[128];
      strncpy (num, sz + 5, sizeof (num));
      num[sizeof (num) -1] = '\0';
      chomp (num);

      switch (_G.asmType)
      {
	    case ASM_TYPE_ASXXXX:
	      sprintf (buffer, "CODE_%s", num);
	      break;
	    case ASM_TYPE_RGBDS:
	      sprintf (buffer, "CODE,BANK[%s]", num);
	      break;
	    case ASM_TYPE_ISAS:
	      /* PENDING: what to use for ISAS? */
	      sprintf (buffer, "CODE,BANK(%s)", num);
	      break;
	    default:
	      wassert (0);
      }
    }
    gbz80_port.mem.code_name = Safe_strdup (buffer);
    code->sname = gbz80_port.mem.code_name;
    return 0;
  }
  else if( startsWith( sz, "portmode=" ) || startsWith( sz, "portmode " ))
  { /*.p.t.20030716 - adding pragma to manipulate z80 i/o port addressing modes */
    char bfr[128];

    if (sz[8]=='=')
      werror(W_DEPRECATED_PRAGMA, "portmode=");

    strncpy( bfr, sz + 9, sizeof (bfr));
    bfr[sizeof (bfr) - 1] = '\0';
    chomp( bfr );

    if     ( !strcmp( bfr, "z80"     )){ z80_opts.port_mode =  80; }
    else if( !strcmp( bfr, "z180"    )){ z80_opts.port_mode = 180; }
    else if( !strcmp( bfr, "save"    )){ z80_opts.port_back = z80_opts.port_mode; }
    else if( !strcmp( bfr, "restore" )){ z80_opts.port_mode = z80_opts.port_back; }
    else                                 return( 1 );

    return( 0 );
  }

  return 1;
}

static const char *_gbz80_rgbasmCmd[] =
{
  "rgbasm", "-o\"$1.o\"", "\"$1.asm\"", NULL
};

static const char *_gbz80_rgblinkCmd[] =
{
  "xlink", "-tg", "-n\"$1.sym\"", "-m\"$1.map\"", "-zFF", "\"$1.lnk\"", NULL
};

static void
_gbz80_rgblink (void)
{
  FILE *lnkfile;

  /* first we need to create the <filename>.lnk file */
  sprintf (scratchFileName, "%s.lnk", dstFileName);
  if (!(lnkfile = fopen (scratchFileName, "w")))
    {
      werror (E_FILE_OPEN_ERR, scratchFileName);
      exit (1);
    }

  fprintf (lnkfile, "[Objects]\n");

  fprintf (lnkfile, "%s.o\n", dstFileName);

  fputStrSet(lnkfile, relFilesSet);

  fprintf (lnkfile, "\n[Libraries]\n");
  /* additional libraries if any */
  fputStrSet(lnkfile, libFilesSet);

  fprintf (lnkfile, "\n[Output]\n" "%s.gb", dstFileName);

  fclose (lnkfile);

  buildCmdLine (buffer,port->linker.cmd, dstFileName, NULL, NULL, NULL);
  /* call the linker */
  if (my_system (buffer))
    {
      perror ("Cannot exec linker");
      exit (1);
    }
}

static bool
_parseOptions (int *pargc, char **argv, int *i)
{
  if (argv[*i][0] == '-')
    {
      if (argv[*i][1] == 'b' && IS_GB)
	{
	  int bank = atoi (argv[*i] + 3);
	  char buffer[128];
	  switch (argv[*i][2])
	    {
	    case 'o':
	      /* ROM bank */
	      sprintf (buffer, "CODE_%u", bank);
	      gbz80_port.mem.code_name = Safe_strdup (buffer);
	      return TRUE;
	    case 'a':
	      /* RAM bank */
	      sprintf (buffer, "DATA_%u", bank);
	      gbz80_port.mem.data_name = Safe_strdup (buffer);
	      return TRUE;
	    }
	}
      else if (!strncmp (argv[*i], "--asm=", 6))
	{
	  if (!strcmp (argv[*i], "--asm=rgbds"))
	    {
	      asm_addTree (&_rgbds_gb);
	      gbz80_port.assembler.cmd = _gbz80_rgbasmCmd;
	      gbz80_port.linker.cmd = _gbz80_rgblinkCmd;
	      gbz80_port.linker.do_link = _gbz80_rgblink;
	      _G.asmType = ASM_TYPE_RGBDS;
	      return TRUE;
	    }
	  else if (!strcmp (argv[*i], "--asm=asxxxx"))
	    {
	      _G.asmType = ASM_TYPE_ASXXXX;
	      return TRUE;
	    }
	  else if (!strcmp (argv[*i], "--asm=isas"))
	    {
	      asm_addTree (&_isas_gb);
	      /* Munge the function prefix */
	      gbz80_port.fun_prefix = "";
	      _G.asmType = ASM_TYPE_ISAS;
	      return TRUE;
	    }
	  else if (!strcmp (argv[*i], "--asm=z80asm"))
	    {
              port->assembler.externGlobal = TRUE;
	      asm_addTree (&_z80asm_z80);
	      _G.asmType = ASM_TYPE_ISAS;
	      return TRUE;
	    }
	}
      else if (!strncmp (argv[*i], "--portmode=", 11))
	{
	  if (!strcmp (argv[*i], "--portmode=z80"))
	    {
	      z80_opts.port_mode =  80;
	      return TRUE;
	    }
	  else if (!strcmp (argv[*i], "--portmode=z180"))
	    {
	      z80_opts.port_mode =  180;
	      return TRUE;
	    }
	 }
    }
  return FALSE;
}

static void
_setValues(void)
{
  const char *s;

  if (options.nostdlib == FALSE)
    {
      const char *s;
      char path[PATH_MAX];
      struct dbuf_s dbuf;

      dbuf_init(&dbuf, PATH_MAX);

      for (s = setFirstItem(libDirsSet); s != NULL; s = setNextItem(libDirsSet))
        {
          buildCmdLine2(path, sizeof path, "-k\"%s" DIR_SEPARATOR_STRING "{port}\" ", s);
          dbuf_append(&dbuf, path, strlen(path));
        }
      buildCmdLine2(path, sizeof path, "-l\"{port}.lib\"", s);
      dbuf_append(&dbuf, path, strlen(path));

      setMainValue ("z80libspec", dbuf_c_str(&dbuf));
      dbuf_destroy(&dbuf);

      for (s = setFirstItem(libDirsSet); s != NULL; s = setNextItem(libDirsSet))
        {
          struct stat stat_buf;

          buildCmdLine2(path, sizeof path, "%s" DIR_SEPARATOR_STRING "{port}" DIR_SEPARATOR_STRING "crt0{objext}", s);
          if (stat(path, &stat_buf) == 0)
            break;
        }

      if (s == NULL)
        setMainValue ("z80crt0", "\"crt0{objext}\"");
      else
        {
          char *buf;
          size_t len = strlen(path) + 3;

          buf = Safe_alloc(len);
          SNPRINTF(buf, len, "\"%s\"", path);
          setMainValue("z80crt0", buf);
          Safe_free(buf);
        } 
    }
  else
    {
      setMainValue ("z80libspec", "");
      setMainValue ("z80crt0", "");
    }

  setMainValue ("z80extralibfiles", (s = joinStrSet(libFilesSet)));
  Safe_free((void *)s);
  setMainValue ("z80extralibpaths", (s = joinStrSet(libPathsSet)));
  Safe_free((void *)s);

  if (IS_GB)
    {
      setMainValue ("z80outputtypeflag", "-z");
      setMainValue ("z80outext", ".gb");
    }
  else
    {
      setMainValue ("z80outputtypeflag", "-i");
      setMainValue ("z80outext", ".ihx");
    }

  setMainValue ("stdobjdstfilename" , "{dstfilename}{objext}");
  setMainValue ("stdlinkdstfilename", "{dstfilename}{z80outext}");

  setMainValue ("z80extraobj", (s = joinStrSet(relFilesSet)));
  Safe_free((void *)s);

  sprintf (buffer, "-b_CODE=0x%04X -b_DATA=0x%04X", options.code_loc, options.data_loc);
  setMainValue ("z80bases", buffer);
}

static void
_finaliseOptions (void)
{
  port->mem.default_local_map = data;
  port->mem.default_globl_map = data;
  if (_G.asmType == ASM_TYPE_ASXXXX && IS_GB)
    asm_addTree (&_asxxxx_gb);

  _setValues();
}

static void
_setDefaultOptions (void)
{
  options.nopeep = 0;
  options.stackAuto = 1;
  options.mainreturn = 1;
  /* first the options part */
  options.intlong_rent = 1;
  options.float_rent = 1;
  options.noRegParams = 1;
  /* Default code and data locations. */
  options.code_loc = 0x200;

  if (IS_GB) 
    {
      options.data_loc = 0xC000;
    }
  else
    {
      options.data_loc = 0x8000;
    }

  optimize.global_cse = 1;
  optimize.label1 = 1;
  optimize.label2 = 1;
  optimize.label3 = 1;
  optimize.label4 = 1;
  optimize.loopInvariant = 1;
  optimize.loopInduction = 1;
}

/* Mangaling format:
    _fun_policy_params
    where:
      policy is the function policy
      params is the parameter format

   policy format:
    rsp
    where:
      r is 'r' for reentrant, 's' for static functions
      s is 'c' for callee saves, 'r' for caller saves
      p is 'p' for profiling on, 'x' for profiling off
    examples:
      rr - reentrant, caller saves
   params format:
    A combination of register short names and s to signify stack variables.
    examples:
      bds - first two args appear in BC and DE, the rest on the stack
      s - all arguments are on the stack.
*/
static char *
_mangleSupportFunctionName(char *original)
{
  char buffer[128];

  sprintf(buffer, "%s_rr%s_%s", original,
          options.profile ? "f" : "x",
          options.noRegParams ? "s" : "bds"
          );

  return Safe_strdup(buffer);
}

static const char *
_getRegName (struct regs *reg)
{
  if (reg)
    {
      return reg->name;
    }
  /*  assert (0); */
  return "err";
}

static bool
_hasNativeMulFor (iCode *ic, sym_link *left, sym_link *right)
{
  sym_link *test = NULL;
  value *val;

  if ( ic->op != '*')
    {
      return FALSE;
    }

  if ( IS_LITERAL (left))
    {
      test = left;
      val = OP_VALUE (IC_LEFT (ic));
    }
  else if ( IS_LITERAL (right))
    {
      test = left;
      val = OP_VALUE (IC_RIGHT (ic));
    }
  else
    {
      return FALSE;
    }

  if ( getSize (test) <= 2)
    {
      return TRUE;
    }

  return FALSE;
}

/* Indicate which extended bit operations this port supports */
static bool
hasExtBitOp (int op, int size)
{
  if (op == GETHBIT)
    return TRUE;
  else
    return FALSE;
}

/* Indicate the expense of an access to an output storage class */
static int
oclsExpense (struct memmap *oclass)
{
  if (IN_FARSPACE(oclass))
    return 1;
    
  return 0;
}


#define LINKCMD "link-{port} -nf {dstfilename}"
/*
#define LINKCMD \
    "link-{port} -n -c -- {z80bases} -m -j" \
    " {z80libspec}" \
    " {z80extralibfiles} {z80extralibpaths}" \
    " {z80outputtypeflag} \"{linkdstfilename}\"" \
    " {z80crt0}" \
    " \"{dstfilename}{objext}\"" \
    " {z80extraobj}"
*/

#define ASMCMD \
    "as-{port} -plosgff \"{objdstfilename}\" \"{dstfilename}{asmext}\""

/* Globals */
PORT z80_port =
{
  TARGET_ID_Z80,
  "z80",
  "Zilog Z80",			/* Target name */
  NULL,				/* Processor name */
  {
    glue,
    FALSE,
    MODEL_MEDIUM | MODEL_SMALL,
    MODEL_SMALL
  },
  {
    NULL,
    ASMCMD,
    "-plosgff",			/* Options with debug */
    "-plosgff",			/* Options without debug */
    0,
    ".asm"
  },
  {
    NULL,
    LINKCMD,
    NULL,
    ".o",
    1
  },
  {
    _z80_defaultRules
  },
  {
	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
    1, 2, 2, 4, 2, 2, 2, 1, 4, 4
  },
  {
    "XSEG",
    "STACK",
    "CODE",
    "DATA",
    "ISEG",
    NULL, /* pdata */
    "XSEG",
    "BSEG",
    "RSEG",
    "GSINIT",
    "OVERLAY",
    "GSFINAL",
    "HOME",
    NULL, /* xidata */
    NULL, /* xinit */
    NULL, /* const_name */
    NULL,
    NULL,
    1
  },
  { NULL, NULL },
  {
    -1, 0, 0, 4, 0, 2
  },
    /* Z80 has no native mul/div commands */
  {
    0, 2
  },
  {
    z80_emitDebuggerSymbol
  },
  {
    255,        /* maxCount */
    3,          /* sizeofElement */
    /* The rest of these costs are bogus. They approximate */
    /* the behavior of src/SDCCicode.c 1.207 and earlier.  */
    {4,4,4},    /* sizeofMatchJump[] */
    {0,0,0},    /* sizeofRangeCompare[] */
    0,          /* sizeofSubtract */
    3,          /* sizeofDispatch */
  },
  "_",
  _z80_init,
  _parseOptions,
  _z80_options,
  NULL,
  _finaliseOptions,
  _setDefaultOptions,
  z80_assignRegisters,
  _getRegName,
  _keywords,
  0,				/* no assembler preamble */
  NULL,				/* no genAssemblerEnd */
  0,				/* no local IVT generation code */
  0,                            /* no genXINIT code */
  NULL, 			/* genInitStartup */
  _reset_regparm,
  _reg_parm,
  _process_pragma,
  _mangleSupportFunctionName,
  _hasNativeMulFor,
  hasExtBitOp,			/* hasExtBitOp */
  oclsExpense,			/* oclsExpense */
  TRUE,
  TRUE,				/* little endian */
  0,				/* leave lt */
  0,				/* leave gt */
  1,				/* transform <= to ! > */
  1,				/* transform >= to ! < */
  1,				/* transform != to !(a == b) */
  0,				/* leave == */
  TRUE,                         /* Array initializer support. */	
  0,                            /* no CSE cost estimation yet */
  _z80_builtins,		/* no builtin functions */
  GPOINTER,			/* treat unqualified pointers as "generic" pointers */
  1,				/* reset labelKey to 1 */
  1,				/* globals & local static allowed */
  PORT_MAGIC
};

/* Globals */
PORT gbz80_port =
{
  TARGET_ID_GBZ80,
  "gbz80",
  "Gameboy Z80-like",		/* Target name */
  NULL,
  {
    glue,
    FALSE,
    MODEL_MEDIUM | MODEL_SMALL,
    MODEL_SMALL
  },
  {
    NULL,
    ASMCMD,
    "-plosgff",			/* Options with debug */
    "-plosgff",			/* Options without debug */
    0,
    ".asm",
    NULL			/* no do_assemble function */
  },
  {
    NULL,
    LINKCMD,
    NULL,
    ".o",
    1
  },
  {
    _gbz80_defaultRules
  },
  {
    /* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
    1, 2, 2, 4, 2, 2, 2, 1, 4, 4
  },
  {
    "XSEG",
    "STACK",
    "CODE",
    "DATA",
    "ISEG",
    NULL, /* pdata */
    "XSEG",
    "BSEG",
    "RSEG",
    "GSINIT",
    "OVERLAY",
    "GSFINAL",
    "HOME",
    NULL, /* xidata */
    NULL, /* xinit */
    NULL, /* const_name */
    NULL,
    NULL,
    1
  },
  { NULL, NULL },
  {
    -1, 0, 0, 2, 0, 4
  },
    /* gbZ80 has no native mul/div commands */
  {
    0, 2
  },
  {
    z80_emitDebuggerSymbol
  },
  {
    255,        /* maxCount */
    3,          /* sizeofElement */
    /* The rest of these costs are bogus. They approximate */
    /* the behavior of src/SDCCicode.c 1.207 and earlier.  */
    {4,4,4},    /* sizeofMatchJump[] */
    {0,0,0},    /* sizeofRangeCompare[] */
    0,          /* sizeofSubtract */
    3,          /* sizeofDispatch */
  },
  "_",
  _gbz80_init,
  _parseOptions,
  _gbz80_options,
  NULL,
  _finaliseOptions,
  _setDefaultOptions,
  z80_assignRegisters,
  _getRegName,
  _keywords,
  0,				/* no assembler preamble */
  NULL,				/* no genAssemblerEnd */
  0,				/* no local IVT generation code */
  0,                            /* no genXINIT code */
  NULL, 			/* genInitStartup */
  _reset_regparm,
  _reg_parm,
  _process_pragma,
  _mangleSupportFunctionName,
  _hasNativeMulFor,
  hasExtBitOp,			/* hasExtBitOp */
  oclsExpense,			/* oclsExpense */
  TRUE,
  TRUE,				/* little endian */
  0,				/* leave lt */
  0,				/* leave gt */
  1,				/* transform <= to ! > */
  1,				/* transform >= to ! < */
  1,				/* transform != to !(a == b) */
  0,				/* leave == */
  TRUE,                         /* Array initializer support. */
  0,                            /* no CSE cost estimation yet */
  NULL, 			/* no builtin functions */
  GPOINTER,			/* treat unqualified pointers as "generic" pointers */
  1,				/* reset labelKey to 1 */
  1,				/* globals & local static allowed */
  PORT_MAGIC
};
