/*
 * Simulator of microcontrollers (stypes.h)
 *
 * Copyright (C) 1997,16 Drotos Daniel, Talker Bt.
 * 
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 */

/* This file is part of microcontroller simulator: ucsim.

UCSIM is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

UCSIM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UCSIM; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA. */
/*@1@*/

#ifndef STYPES_HEADER
#define STYPES_HEADER

#include "ddconfig.h"

//typedef int8_t TYPE_BYTE;
//typedef uint8_t TYPE_UBYTE;
//typedef int16_t TYPE_WORD;
//typedef uint16_t TYPE_UWORD;
//typedef int32_t TYPE_DWORD;
//typedef uint32_t TYPE_UDWORD;

typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned long	ulong;

typedef   signed TYPE_BYTE  i8_t;
typedef unsigned TYPE_BYTE  u8_t;
typedef   signed TYPE_WORD  i16_t;
typedef unsigned TYPE_WORD  u16_t;
typedef   signed TYPE_DWORD i32_t;
typedef unsigned TYPE_DWORD u32_t;
typedef   signed TYPE_QWORD i64_t;
typedef unsigned TYPE_QWORD u64_t;

typedef i64_t		t_addr;		/* 64 bit max */
typedef u32_t		t_mem;		/* 32 bit max */
typedef i32_t		t_smem;		/* signed 32 bit memory */

enum {
  max_mem_size= 0x40000000		/* 1 GB */
};

struct id_element
{
  int id;
  const char *id_string;
};

enum error_type {
  err_unknown  = 0x01,
  err_error    = 0x02,
  err_warning  = 0x04
};

// table of dissassembled instructions
struct dis_entry
{
  /*uint64_t*/long long code, mask; // max 8 byte of code
  char  branch;
  uchar length;
  const char *mnemonic;
  bool is_call;
};

// table entry of SFR and BIT names
struct name_entry
{
  int		cpu_type;
  t_addr	addr;
  const char	*name;
};


struct cpu_entry
{
  const char *type_str;
  int  type;
  int  technology;
};

enum cpu_type {
  CPU_51	= 0x0001,
  CPU_31	= 0x0002,
  CPU_52	= 0x0004,
  CPU_32	= 0x0008,
  CPU_51R	= 0x0010,
  CPU_89C51R	= 0x0020,
  CPU_251	= 0x0040,
  CPU_DS390	= 0x0080,
  CPU_DS390F	= 0x0100,
  CPU_C521	= 0x0200,
  CPU_ALL_51	= (CPU_51|CPU_31),
  CPU_ALL_52	= (CPU_52|CPU_32|CPU_51R|CPU_89C51R|CPU_251|CPU_DS390|CPU_DS390F|
		   CPU_C521),

  CPU_AVR	= 0x0001,
  CPU_ALL_AVR	= (CPU_AVR),

  CPU_Z80	= 0x0001,
  CPU_Z180	= 0x0002,
  CPU_LR35902   = 0x0004,
  CPU_R2K       = 0x0008,
  CPU_R3KA      = 0x0010,
  CPU_ALL_Z80   = (CPU_Z80|CPU_Z180|CPU_R2K|CPU_LR35902|CPU_R3KA),

  CPU_XA	= 0x0001,
  CPU_ALL_XA	= (CPU_XA),

  CPU_HC08      = 0x0001,
  CPU_HCS08     = 0x0002,
  CPU_ALL_HC08  = (CPU_HC08|CPU_HCS08),

  CPU_STM8	= 0x0001,
  CPU_ALL_STM8	= (CPU_STM8),

  CPU_ST7       = 0x0001,
  CPU_ALL_ST7   = (CPU_ST7),

  CPU_CMOS	= 0x0001,
  CPU_HMOS	= 0x0002
};

/* Classes of memories, this is index on the list */
enum mem_class
{
  MEM_ROM= 0,
  MEM_XRAM,
  MEM_IRAM,
  MEM_SFR,
  MEM_DUMMY,
  MEM_IXRAM,
  MEM_TYPES
};

#define MEM_SFR_ID	cchars("sfr")
#define MEM_XRAM_ID	cchars("xram")
#define MEM_IXRAM_ID	cchars("ixram")
#define MEM_IRAM_ID	cchars("iram")

// States of simulator
enum sim_state {
  SIM_NONE	= 0,
  SIM_GO	= 0x01,	// Processor is running
  SIM_QUIT	= 0x02	// Program must exit
};

/* States of CPU */
enum cpu_state {
  stGO		= 0,	/* Normal state */
  stIDLE	= 1,	/* Idle mode is active */
  stPD		= 2	/* Power Down mode is active */
};

/* Result of instruction simulation */
enum inst_result {
  resGO		= 0,	/* OK, go on */
  resWDTRESET	= 1,	/* Reseted by WDT */
  resINTERRUPT	= 2,	/* Interrupt accepted */
  resSTOP	= 100,	/* Stop if result greather then this */
  resHALT	= 101,	/* Serious error, halt CPU */
  resINV_ADDR	= 102,	/* Invalid indirect address */
  resSTACK_OV	= 103,	/* Stack overflow */
  resBREAKPOINT	= 104,	/* Breakpoint */
  resUSER	= 105,	/* Stopped by user */
  resINV_INST	= 106,	/* Invalid instruction */
  resBITADDR	= 107,	/* Bit address is uninterpretable */
  resERROR	= 108,	/* Error happened during instruction exec */
  resSTEP	= 109,	/* Step command done, no more exex needed */
  resSIMIF	= 110,	/* Stopped by simulated prog itself through sim interface */
};
  
#define BIT_MASK(bitaddr) (1 << (bitaddr & 0x07))


/* Type of breakpoints */
enum brk_perm
{
  brkFIX,	/* f */
  brkDYNAMIC	/* d */
};

enum brk_type
{
  brkFETCH,	/* f */
  brkEVENT	/* e */
};

enum brk_event
{
  brkNONE,
  brkWXRAM,	/* wx */
  brkRXRAM,	/* rx */
  brkRCODE,	/* rc */
  brkWIRAM,	/* wi */
  brkRIRAM,	/* ri */
  brkWSFR,	/* ws */
  brkRSFR,	/* rs */
  brkREAD,
  brkWRITE,
  brkACCESS
};

/* Interrupt levels */
enum intr_levels {
//IT_NO		= -1, /* not in interroupt service */
  IT_LOW	= 1, /* low level interrupt service */
  IT_HIGH	= 2 /* service of high priority interrupt */
};

/* cathegories of hw elements (peripherials) */
enum hw_cath {
  HW_DUMMY	= 0x0000,
  HW_TIMER	= 0x0002,
  HW_UART	= 0x0004,
  HW_PORT	= 0x0008,
  HW_PCA	= 0x0010,
  HW_INTERRUPT	= 0x0020,
  HW_WDT	= 0x0040,
  HW_SIMIF	= 0x0080,
  HW_RESET	= 0x0100,
  HW_CLOCK	= 0x0200,
};

// Events that can happen in peripherals
enum hw_event {
  EV_OVERFLOW,
  EV_PORT_CHANGED,
  EV_T2_MODE_CHANGED
};

// flags of hw units
enum hw_flags {
  HWF_NONE	= 0,
  HWF_INSIDE	= 0x0001,
  HWF_OUTSIDE	= 0x0002,
  HWF_MISC	= 0x0004
};

/* Letter cases */
enum letter_case {
  case_upper,  /* all is upper case */
  case_lower,  /* all is lower case */
  case_case    /* first letter is upper, others are lower case */
};


/* Simulator interface config memory addresses */
enum simif_cfg {
  simif_on		= 0,  // RW
  simif_run		= 1,  // RW
  simif_start		= 2,  // RW
  simif_stop		= 3,  // RW
  simif_quit		= 4,  // W
  simif_reason		= 5,  // R
  simif_xtal		= 6,  // RW
  simif_ticks		= 7,  // R
  simif_isr_ticks	= 8,  // R
  simif_idle_ticks	= 9,  // R
  simif_real_time	= 10, // R

  simif_nuof		= 11
};

#endif

/* End of stypes.h */
