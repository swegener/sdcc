/*-------------------------------------------------------------------------

  device.c - Accomodates subtle variations in PIC16 devices

   Written By -  Scott Dattalo scott@dattalo.com

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

/*
  PIC device abstraction

  There are dozens of variations of PIC microcontrollers. This include
  file attempts to abstract those differences so that SDCC can easily
  deal with them.
*/

#ifndef  __DEVICE_H__
#define  __DEVICE_H__

#if defined(__BORLANDC__) || defined(_MSC_VER)
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

#define CONFIGURATION_WORDS	20

typedef struct {
	int sfrLoAddr;
	int sfrHiAddr;
} sfrRangeInfo_t;
	

typedef struct {
	int mask;
	int emit;
	int value;
} configRegInfo_t;

typedef struct {
	int confAddrStart;	/* starting address */
	int confAddrEnd;	/* ending address */
	configRegInfo_t crInfo[ CONFIGURATION_WORDS ];
} configWordsInfo_t;


#define PROCESSOR_NAMES    4
/* Processor unique attributes */
typedef struct PIC16_device {
  char *name[PROCESSOR_NAMES];/* aliases for the processor name */

  int maxRAMaddress;		/* maximum value for a data address */
  int RAMsize;			/* size of Data RAM - VR 031120 */
  int acsSplitOfs;		/* access bank split offset */
  int extMIface;		/* device has external memory interface */
  sfrRangeInfo_t sfrRange;	/* SFR range */
  configWordsInfo_t cwInfo;	/* configuration words info */
} PIC16_device;

/* Given a pointer to a register, this macro returns the bank that it is in */
#define REG_ADDR(r)        ((r)->isBitField ? (((r)->address)>>3) : (r)->address)
//#define REG_BANK(r)        (pic16_finalMapping[REG_ADDR(r)].bank)
//#define REG_isALIASED(r)   (pic16_finalMapping[REG_ADDR(r)].alias != 0)
//#define REG_isVALID(r)     (pic16_finalMapping[REG_ADDR(r)].isValid)


typedef struct {
	int no_banksel;
	int opt_banksel;
	int omit_configw;
	int omit_ivt;
	int leave_reset;
	int stack_model;
	int ivt_loc;
	int nodefaultlibs;
} pic16_options_t;

#define STACK_MODEL_SMALL	(pic16_options.stack_model == 0)
#define STACK_MODEL_LARGE	(pic16_options.stack_model == 1)

extern set *fix_idataSymSet;
extern set *rel_idataSymSet;

extern pic16_options_t pic16_options;
extern PIC16_device *pic16;

/****************************************/
void pic16_assignConfigWordValue(int address, int value);
int pic16_getConfigWord(int address);
int pic16_isREGinBank(regs *reg, int bank);
int pic16_REGallBanks(regs *reg);
void pic16_setMaxRAM(int size);
int PIC16_IS_CONFIG_ADDRESS(int address);

int checkAddReg(set **set, regs *reg);
int checkAddSym(set **set, symbol *reg);

#endif  /* __DEVICE_H__ */
