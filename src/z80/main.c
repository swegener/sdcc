#include "z80.h"
#include "MySystem.h"
#include "BuildCmd.h"

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

typedef enum
  {
    /* Must be first */
    ASM_TYPE_ASXXXX,
    ASM_TYPE_RGBDS,
    ASM_TYPE_ISAS
  }
ASM_TYPE;

static struct
  {
    ASM_TYPE asmType;
  }
_G;

static char *_keywords[] =
{
  "sfr",
  "nonbanked",
  "banked",
  NULL
};

extern PORT gbz80_port;
extern PORT z80_port;

#include "mappings.i"

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

static int regParmFlg = 0;	/* determine if we can register a parameter */

static void
_reset_regparm ()
{
  regParmFlg = 0;
}

static int
_reg_parm (sym_link * l)
{
  if (options.noRegParams) 
    {
      return FALSE;
    }
  else 
    {
      if (regParmFlg == 2)
        {
          return FALSE;
        }
      else
        {
          regParmFlg++;
          return TRUE;
        }
    }
}

static bool
_startsWith (const char *sz, const char *key)
{
  return !strncmp (sz, key, strlen (key));
}

static void
_chomp (char *sz)
{
  char *nl;
  while ((nl = strrchr (sz, '\n')))
    *nl = '\0';
}

static int
_process_pragma (const char *sz)
{
  if (_startsWith (sz, "bank="))
    {
      char buffer[128];
      strcpy (buffer, sz + 5);
      _chomp (buffer);
      if (isdigit (buffer[0]))
	{

	}
      else if (!strcmp (buffer, "BASE"))
	{
	  strcpy (buffer, "HOME");
	}
      if (isdigit (buffer[0]))
	{
	  /* Arg was a bank number.  Handle in an ASM independent
	     way. */
	  char num[128];
	  strcpy (num, sz + 5);
	  _chomp (num);

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
      gbz80_port.mem.code_name = gc_strdup (buffer);
      code->sname = gbz80_port.mem.code_name;
      return 0;
    }
  return 1;
}

static const char *_gbz80_rgbasmCmd[] =
{
  "rgbasm", "-o$1.o", "$1.asm", NULL
};

static const char *_gbz80_rgblinkCmd[] =
{
  "xlink", "-tg", "-n$1.sym", "-m$1.map", "-zFF", "$1.lnk", NULL
};

static void
_gbz80_rgblink (void)
{
  FILE *lnkfile;
  const char *sz;

  int i;
  sz = srcFileName;
  if (!sz)
    sz = "a";

  /* first we need to create the <filename>.lnk file */
  sprintf (scratchFileName, "%s.lnk", sz);
  if (!(lnkfile = fopen (scratchFileName, "w")))
    {
      werror (E_FILE_OPEN_ERR, scratchFileName);
      exit (1);
    }

  fprintf (lnkfile, "[Objects]\n");

  if (srcFileName)
    fprintf (lnkfile, "%s.o\n", sz);

  for (i = 0; i < nrelFiles; i++)
    fprintf (lnkfile, "%s\n", relFiles[i]);

  fprintf (lnkfile, "\n[Libraries]\n");
  /* additional libraries if any */
  for (i = 0; i < nlibFiles; i++)
    fprintf (lnkfile, "%s\n", libFiles[i]);


  fprintf (lnkfile, "\n[Output]\n" "%s.gb", sz);

  fclose (lnkfile);

  buildCmdLine (buffer,port->linker.cmd, sz, NULL, NULL, NULL);
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
	      gbz80_port.mem.code_name = gc_strdup (buffer);
	      return TRUE;
	    case 'a':
	      /* RAM bank */
	      sprintf (buffer, "DATA_%u", bank);
	      gbz80_port.mem.data_name = gc_strdup (buffer);
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
	}
    }
  return FALSE;
}

static void
_finaliseOptions (void)
{
  port->mem.default_local_map = data;
  port->mem.default_globl_map = data;
  if (_G.asmType == ASM_TYPE_ASXXXX && IS_GB)
    asm_addTree (&_asxxxx_gb);
}

static void
_setDefaultOptions (void)
{
  options.genericPtr = 1;	/* default on */
  options.nopeep = 0;
  options.stackAuto = 1;
  options.mainreturn = 1;
  /* first the options part */
  options.intlong_rent = 1;
  options.noRegParams = 1;
  /* Default code and data locations. */
  options.code_loc = 0x200;
  options.data_loc = 0x8000;

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

  if (TARGET_IS_Z80) 
    {
      sprintf(buffer, "%s_rr%s_%s", original,
              options.profile ? "f" : "x",
              options.noRegParams ? "s" : "bds"
              );
    }
  else 
    {
      strcpy(buffer, original);
    }

  return gc_strdup(buffer);
}

static const char *
_getRegName (struct regs *reg)
{
  if (reg)
    return reg->name;
  assert (0);
  return "err";
}

/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_z80_linkCmd[] =
{
    "link-z80", 
    "-n",                       // Don't echo output
    "-c",                       // Command line input
    "--",                       // Again, command line input...
    "-b_CODE=0x200",            // Code starts at 0x200
    "-b_DATA=0x8000",           // RAM starts at 0x8000
    "-j",                       // Output a symbol file as well
    "-k" SDCC_LIB_DIR "/z80",   // Library path
    "-lz80.lib",                // Library to use
    "-i",                       // Output Intel IHX
    "$1.ihx",                   // Output to
    SDCC_LIB_DIR "/z80/crt0.o", // Link in crt0 first
    "$1.o",                     // Actual code
    NULL
};

/* sprintf that appends to the string. */
static void
_saprintf(char *pinto, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    vsprintf(pinto + strlen(pinto), format, ap);
    va_end(ap);
}

static void
_z80_link(void)
{
    int i;
    // PENDING
    char buffer[2048];

    sprintf(buffer, 
            "link-z80 "
            "-n "                       // Don't echo output
            "-c "                       // Command line input
            "-- "                       // Again, command line input...
            "-b_CODE=0x%04X "           // Code starts at 0x200
            "-b_DATA=0x%04X "           // RAM starts at 0x8000
            "-j ",                      // Output a symbol file as well
            options.code_loc,
            options.data_loc
            );

    // Add the standard lib in.
    if (options.nostdlib == FALSE) {
        _saprintf(buffer,
                  "-k" SDCC_LIB_DIR "/z80 "   // Library path
                  "-lz80.lib "                // Library to use
                  );
    }

    // Add in the library paths and libraries
    for (i = 0; i < nlibFiles; i++) {
        _saprintf(buffer, "-k%s ", libFiles[i]);
    }
    for (i = 0; i < nlibPaths; i++) {
        _saprintf(buffer, "-l%s ", libPaths[i]);
    }

    _saprintf(buffer,
              "-i "                       // Output Intel IHX
              "%s.ihx ",                  // Output to
              srcFileName
              );

    if (options.nostdlib == FALSE) {
        _saprintf(buffer, 
                  SDCC_LIB_DIR "/z80/crt0.o " // Link in crt0 first
                  );
    }

    _saprintf(buffer,
              "%s.o ",                    // Actual code
              srcFileName
              );

    // Append all the other targets
    for (i = 0; i < nrelFiles; i++) {
        _saprintf(buffer, "%s ", relFiles[i]);
    }

    // Do it.
    if (my_system (buffer)) {
        exit(1);
    }
}

static const char *_z80_asmCmd[] =
{
    "as-z80", "-plosgff", "$1.o", "$1.asm", NULL
};

/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_gbz80_linkCmd[] =
{
    // PENDING
    "link-gbz80", "-nf", "$1", NULL
};

static const char *_gbz80_asmCmd[] =
{
    "as-gbz80", "-plosgff", "$1.o", "$1.asm", NULL
};

/* Globals */
PORT z80_port =
{
  TARGET_ID_Z80,
  "z80",
  "Zilog Z80",			/* Target name */
  {
    FALSE,
    MODEL_MEDIUM | MODEL_SMALL,
    MODEL_SMALL
  },
  {
    _z80_asmCmd,
    "-plosgff",			/* Options with debug */
    "-plosgff",			/* Options without debug */
    0,
    ".asm"
  },
  {
    _z80_linkCmd,
    _z80_link,
    ".o"
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
    "XSEG",
    "BSEG",
    "RSEG",
    "GSINIT",
    "OVERLAY",
    "GSFINAL",
    "HOME",
    NULL,
    NULL,
    1
  },
  {
    -1, 0, 0, 4, 0, 2
  },
    /* Z80 has no native mul/div commands */
  {
    0, 2
  },
  "_",
  _z80_init,
  _parseOptions,
  _finaliseOptions,
  _setDefaultOptions,
  z80_assignRegisters,
  _getRegName,
  _keywords,
  0,				/* no assembler preamble */
  0,				/* no local IVT generation code */
  _reset_regparm,
  _reg_parm,
  _process_pragma,
  _mangleSupportFunctionName,
  TRUE,
  0,				/* leave lt */
  0,				/* leave gt */
  1,				/* transform <= to ! > */
  1,				/* transform >= to ! < */
  1,				/* transform != to !(a == b) */
  0,				/* leave == */
  TRUE,                         /* Array initializer support. */	
  PORT_MAGIC
};

/* Globals */
PORT gbz80_port =
{
  TARGET_ID_GBZ80,
  "gbz80",
  "Gameboy Z80-like",		/* Target name */
  {
    FALSE,
    MODEL_MEDIUM | MODEL_SMALL,
    MODEL_SMALL
  },
  {
    _gbz80_asmCmd,
    "-plosgff",			/* Options with debug */
    "-plosgff",			/* Options without debug */
    1,
    ".asm"
  },
  {
    _gbz80_linkCmd,
    NULL,
    ".o"
  },
  {
    _gbz80_defaultRules
  },
  {
	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
    1, 1, 2, 4, 2, 2, 2, 1, 4, 4
  },
  {
    "XSEG",
    "STACK",
    "CODE",
    "DATA",
    "ISEG",
    "XSEG",
    "BSEG",
    "RSEG",
    "GSINIT",
    "OVERLAY",
    "GSFINAL",
    "HOME",
    NULL,
    NULL,
    1
  },
  {
    -1, 0, 0, 2, 0, 4
  },
    /* gbZ80 has no native mul/div commands */
  {
    0, 2
  },
  "_",
  _gbz80_init,
  _parseOptions,
  _finaliseOptions,
  _setDefaultOptions,
  z80_assignRegisters,
  _getRegName,
  _keywords,
  0,				/* no assembler preamble */
  0,				/* no local IVT generation code */
  _reset_regparm,
  _reg_parm,
  _process_pragma,
  NULL,
  TRUE,
  0,				/* leave lt */
  0,				/* leave gt */
  1,				/* transform <= to ! > */
  1,				/* transform >= to ! < */
  1,				/* transform != to !(a == b) */
  0,				/* leave == */
  FALSE,                        /* No array initializer support. */
  PORT_MAGIC
};
