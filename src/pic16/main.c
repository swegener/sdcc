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
//#include "gen.h"


static char _defaultRules[] =
{
#include "peeph.rul"
};

/* list of key words used by pic16 */
static char *_pic16_keywords[] =
{
  "at",
  "bit",
  "code",
  "critical",
  "data",
  "far",
  "idata",
  "interrupt",
  "near",
  "pdata",
  "reentrant",
  "sfr",
  "sbit",
  "using",
  "xdata",
  "_data",
  "_code",
  "_generic",
  "_near",
  "_xdata",
  "_pdata",
  "_idata",
  NULL
};


extern char *pic16_processor_base_name(void);

void  pic16_pCodeInitRegisters(void);

void pic16_assignRegisters (eBBlock ** ebbs, int count);

static int regParmFlg = 0;	/* determine if we can register a parameter */

static void
_pic16_init (void)
{
  asm_addTree (&asm_asxxxx_mapping);
  pic16_pCodeInitRegisters();
  maxInterrupts = 2;
}

static void
_pic16_reset_regparm ()
{
  regParmFlg = 0;
}

static int
_pic16_regparm (sym_link * l)
{
  /* for this processor it is simple
     can pass only the first parameter in a register */
  //if (regParmFlg)
  //  return 0;

  regParmFlg++;// = 1;
  return 1;
}

static int
_process_pragma(const char *sz)
{
  static const char *WHITE = " \t";
  char	*ptr = strtok((char *)sz, WHITE);

  if (startsWith (ptr, "memmap"))
    {
      char	*start;
      char	*end;
      char	*type;
      char	*alias;

      start = strtok((char *)NULL, WHITE);
      end = strtok((char *)NULL, WHITE);
      type = strtok((char *)NULL, WHITE);
      alias = strtok((char *)NULL, WHITE);

      if (start != (char *)NULL
	  && end != (char *)NULL
	  && type != (char *)NULL) {
	value		*startVal = constVal(start);
	value		*endVal = constVal(end);
	value		*aliasVal;
	memRange	r;

	if (alias == (char *)NULL) {
	  aliasVal = constVal(0);
	} else {
	  aliasVal = constVal(alias);
	}

	r.start_address = (int)floatFromVal(startVal);
	r.end_address = (int)floatFromVal(endVal);
	r.alias = (int)floatFromVal(aliasVal);
	r.bank = (r.start_address >> 7) & 0xf;

	if (strcmp(type, "RAM") == 0) {
	  pic16_addMemRange(&r, 0);
	} else if (strcmp(type, "SFR") == 0) {
	  pic16_addMemRange(&r, 1);
	} else {
	  return 1;
	}
      }

      return 0;
    } else if (startsWith (ptr, "maxram")) {
      char *maxRAM = strtok((char *)NULL, WHITE);

      if (maxRAM != (char *)NULL) {
	int	maxRAMaddress;
	value 	*maxRAMVal;

	maxRAMVal = constVal(maxRAM);
	maxRAMaddress = (int)floatFromVal(maxRAMVal);
	pic16_setMaxRAM(maxRAMaddress);
      }
	
      return 0;
    }
  return 1;
}

static bool
_pic16_parseOptions (int *pargc, char **argv, int *i)
{
  /* TODO: allow port-specific command line options to specify
   * segment names here.
   */
  return FALSE;
}

static void
_pic16_finaliseOptions (void)
{
	port->mem.default_local_map = data;
	port->mem.default_globl_map = data;

	setMainValue("mcu", pic16_processor_base_name() );
	addSet(&preArgvSet, Safe_strdup("-DMCU={mcu}"));
}


/* all the rest is commented ifdef'd out */
#if 0
  /* Hack-o-matic: if we are using the flat24 model,
   * adjust pointer sizes.
   */
  if (options.model == MODEL_FLAT24)
    {

      fprintf (stderr, "*** WARNING: you should use the '-mds390' option "
	       "for DS80C390 support. This code generator is "
	       "badly out of date and probably broken.\n");

      port->s.fptr_size = 3;
      port->s.gptr_size = 4;
      port->stack.isr_overhead++;	/* Will save dpx on ISR entry. */
#if 1
      port->stack.call_overhead++;	/* This acounts for the extra byte 
					 * of return addres on the stack.
					 * but is ugly. There must be a 
					 * better way.
					 */
#endif
      fReturn = fReturn390;
      fReturnSize = 5;
    }

  if (options.model == MODEL_LARGE)
    {
      port->mem.default_local_map = xdata;
      port->mem.default_globl_map = xdata;
    }
  else
    {
      port->mem.default_local_map = data;
      port->mem.default_globl_map = data;
    }

  if (options.stack10bit)
    {
      if (options.model != MODEL_FLAT24)
	{
	  fprintf (stderr,
		   "*** warning: 10 bit stack mode is only supported in flat24 model.\n");
	  fprintf (stderr, "\t10 bit stack mode disabled.\n");
	  options.stack10bit = 0;
	}
      else
	{
	  /* Fixup the memory map for the stack; it is now in
	   * far space and requires a FPOINTER to access it.
	   */
	  istack->fmap = 1;
	  istack->ptrType = FPOINTER;
	}
    }
#endif


static void
_pic16_setDefaultOptions (void)
{
}

static const char *
_pic16_getRegName (struct regs *reg)
{
  if (reg)
    return reg->name;
  return "err";
}


static void
_pic16_genAssemblerPreamble (FILE * of)
{
  char * name = pic16_processor_base_name();

  if(!name) {

    name = "p18f452";
    fprintf(stderr,"WARNING: No Pic has been selected, defaulting to %s\n",name);
  }

  fprintf (of, "\tlist\tp=%s\n",&name[1]);
  fprintf (of, "\tinclude \"%s.inc\"\n",name);

  fprintf (of, "\t__config _CONFIG1H,0x%x\n",pic16_getConfigWord(0x300001));
  fprintf (of, "\t__config _CONFIG2L,0x%x\n",pic16_getConfigWord(0x300002));
  fprintf (of, "\t__config _CONFIG2H,0x%x\n",pic16_getConfigWord(0x300003));
  fprintf (of, "\t__config _CONFIG3H,0x%x\n",pic16_getConfigWord(0x300005));
  fprintf (of, "\t__config _CONFIG4L,0x%x\n",pic16_getConfigWord(0x300006));
  fprintf (of, "\t__config _CONFIG5L,0x%x\n",pic16_getConfigWord(0x300008));
  fprintf (of, "\t__config _CONFIG5H,0x%x\n",pic16_getConfigWord(0x300009));
  fprintf (of, "\t__config _CONFIG6L,0x%x\n",pic16_getConfigWord(0x30000a));
  fprintf (of, "\t__config _CONFIG6H,0x%x\n",pic16_getConfigWord(0x30000b));
  fprintf (of, "\t__config _CONFIG7L,0x%x\n",pic16_getConfigWord(0x30000c));
  fprintf (of, "\t__config _CONFIG7H,0x%x\n",pic16_getConfigWord(0x30000d));

  fprintf (of, "\tradix dec\n");
}

/* Generate interrupt vector table. */
static int
_pic16_genIVT (FILE * of, symbol ** interrupts, int maxInterrupts)
{
#if 0
  int i;
#endif

#if 0
	if (options.model != MODEL_FLAT24) {
	/* Let the default code handle it. */
	  return FALSE;
	}
#endif

	/* PIC18F family has only two interrupts, the high and the low
	 * priority interrupts, which reside at 0x0008 and 0x0018 respectively - VR */

	fprintf(of, "; RESET vector\n");
	fprintf(of, "\tgoto\t__sdcc_gsinit_startup\n");
	fprintf(of, "\tres 4\n");


	fprintf(of, "; High priority interrupt vector 0x0008\n");
	if(interrupts[1]) {
		fprintf(of, "\tgoto\t%s\n", interrupts[1]->rname);
		fprintf(of, "\tres\t12\n"); 
	} else {
		fprintf(of, "\tretfie\n");
		fprintf(of, "\tres\t14\n");
	}

	fprintf(of, "; Low priority interrupt vector 0x0018\n");
	if(interrupts[2]) {
		fprintf(of, "\tgoto\t%s\n", interrupts[2]->rname);
	} else {
		fprintf(of, "\tretfie\n");
	}
#if 0
  /* now for the other interrupts */
  for (i = 0; i < maxInterrupts; i++)
    {
	fprintf(of, "; %s priority interrupt vector 0x%s\n", (i==0)?"high":"low", (i==0)?"0008":"0018");
      if (interrupts[i])
	{
	  fprintf (of, "\tgoto\t%s\n\tres\t4\n", interrupts[i]->rname);
	}
      else
	{
	  fprintf (of, "\tretfie\n\tres\t7\n");
	}
    }
#endif

  return TRUE;
}

static bool
_hasNativeMulFor (iCode *ic, sym_link *left, sym_link *right)
{
  //  sym_link *test = NULL;
  //  value *val;

  fprintf(stderr,"checking for native mult\n");

  if ( ic->op != '*')
    {
      return FALSE;
    }

  return TRUE;
/*
  if ( IS_LITERAL (left))
    {
      fprintf(stderr,"left is lit\n");
      test = left;
      val = OP_VALUE (IC_LEFT (ic));
    }
  else if ( IS_LITERAL (right))
    {
      fprintf(stderr,"right is lit\n");
      test = left;
      val = OP_VALUE (IC_RIGHT (ic));
    }
  else
    {
      fprintf(stderr,"oops, neither is lit so no\n");
      return FALSE;
    }

  if ( getSize (test) <= 2)
    {
      fprintf(stderr,"yep\n");
      return TRUE;
    }
  fprintf(stderr,"nope\n");

  return FALSE;
*/
}

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

/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_linkCmd[] =
{
  "gplink", "\"$1.o\"", "-o $1", "$l", NULL
};

/* Sigh. This really is not good. For now, I recommend:
 * sdcc -S -mpic16 file.c
 * the -S option does not compile or link
 */
static const char *_asmCmd[] =
{
  "gpasm", "-c  -I/usr/local/share/gputils/header", "\"$1.asm\"", NULL

};

/* Globals */
PORT pic16_port =
{
  TARGET_ID_PIC16,
  "pic16",
  "MCU PIC16",			/* Target name */
  "p18f442",                    /* Processor */
  {
    pic16glue,
    TRUE,			/* Emit glue around main */
    MODEL_SMALL | MODEL_LARGE | MODEL_FLAT24,
    MODEL_SMALL
  },
  {
    _asmCmd,
    NULL,
    NULL,
    NULL,
	//"-plosgffc",          /* Options with debug */
	//"-plosgff",           /* Options without debug */
    0,
    ".asm",
    NULL			/* no do_assemble function */
  },
  {
    _linkCmd,
    NULL,
    NULL,
    ".rel"
  },
  {
    _defaultRules
  },
  {
	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
    1, 2, 2, 4, 2, 2, 2, 1, 4, 4
	/* TSD - I changed the size of gptr from 3 to 1. However, it should be
	   2 so that we can accomodate the PIC's with 4 register banks (like the
	   16f877)
	 */
  },
  {
    "XSEG    (XDATA)",		// xstack
    "STACK   (DATA)",		// istack
    "CSEG    (CODE)",		// code
    "DSEG    (DATA)",		// data
    "ISEG    (DATA)",		// idata
    "XSEG    (XDATA)",		// xdata
    "BSEG    (BIT)",		// bit
    "RSEG    (DATA)",		// reg
    "GSINIT  (CODE)",		// static
    "OSEG    (OVR,DATA)",	// overlay
    "GSFINAL (CODE)",		// post static
    "HOME	 (CODE)",	// home
    NULL,			// xidata
    NULL,			// xinit
    NULL,			// default location for auto vars
    NULL,			// default location for global vars
    1				// code is read only
  },
  { NULL, NULL },
  {
    +1, 1, 4, 1, 1, 0
  },
    /* pic16 has an 8 bit mul */
  {
    1, -1
  },
  "_",
  _pic16_init,
  _pic16_parseOptions,
  NULL,
  _pic16_finaliseOptions,
  _pic16_setDefaultOptions,
  pic16_assignRegisters,
  _pic16_getRegName,
  _pic16_keywords,
  _pic16_genAssemblerPreamble,
  NULL,				/* no genAssemblerEnd */
  _pic16_genIVT,
  NULL, // _pic16_genXINIT
  _pic16_reset_regparm,
  _pic16_regparm,
  _process_pragma,				/* process a pragma */
  NULL,
  _hasNativeMulFor,
  hasExtBitOp,			/* hasExtBitOp */
  oclsExpense,			/* oclsExpense */
  FALSE,
  TRUE,				/* little endian */
  0,				/* leave lt */
  0,				/* leave gt */
  1,				/* transform <= to ! > */
  1,				/* transform >= to ! < */
  1,				/* transform != to !(a == b) */
  0,				/* leave == */
  FALSE,                        /* No array initializer support. */
  0,                            /* no CSE cost estimation yet */
  NULL, 			/* no builtin functions */
  GPOINTER,			/* treat unqualified pointers as "generic" pointers */
  1,				/* reset labelKey to 1 */
  1,				/* globals & local static allowed */
  PORT_MAGIC
};
