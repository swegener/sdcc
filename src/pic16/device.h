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

/* memRange - a structure to define a range of valid memory addresses 
 * 
 * The Memory of most PICs (and other micros) is a collection of
 * disjoint chunks. The memRange structure will define the start
 * and end address of one of these chunks. The memory map of a
 * particular device is a collection of memRange struct's.
 */

typedef struct memRange {
  int start_address;      /* first address in range */
  int end_address;        /* last */
  int alias;              /* bit mask defining how/if memory range is aliased 
			   * e.g. alias = 0x80 means start_address is identical
			   * to the memory location at (0x80 | start_address) */
  int bank;               /* PIC memory bank this range occupies */

} memRange;

/* AssignedMemory - A structure to keep track of the memory that has been used.
 *
 * When a register gets assigned an address this struct is used to
 * keep track of a few details about the register. There is one of
 * these structures for each memory location in the device.
 */

typedef struct AssignedMemory {
  regs *reg;        /* Pointer to the register (NULL if this is an invalid address) */
  int  instance;    /* the i'th byte of a multibyte register */
  int  alias;       /* Bit mapping of aliased addresses (see memRange) */
  int  bank;        /* Memory bank of this register */
  int  isValid:1;   /* True if the address is legal */
  int  isSFR:1;     /* True if the address is that of a Special Function reg */
  int  isEmitted:1; /* True if the register has been written to a cBlock */

} AssignedMemory;


/*
 * pic16_finalMapping - Dynamically allocated array that records the register assignments
 */

//extern AssignedMemory *pic16_finalMapping;

/*
 * pic16_finalMappingSize - Size of register assignments that pic16_finalMapping can hold
 */

//extern int pic16_finalMappingSize;


#define PROCESSOR_NAMES    4
/* Processor unique attributes */
typedef struct PIC16_device {
  char *name[PROCESSOR_NAMES];/* aliases for the processor name */

  memRange *ram;              /* RAM memory map */
  memRange *sfr;              /* SFR memory map */

  int maxRAMaddress;          /* maximum value for a data address */
  int bankMask;               /* Bitmask that is ANDed with address to extract banking bits */
  int RAMsize;		      /* size of Data RAM - VR 031120 */
  int extMIface;               /* device has external memory interface */
} PIC16_device;

/* Given a pointer to a register, this macro returns the bank that it is in */
#define REG_ADDR(r)        ((r)->isBitField ? (((r)->address)>>3) : (r)->address)
//#define REG_BANK(r)        (pic16_finalMapping[REG_ADDR(r)].bank)
//#define REG_isALIASED(r)   (pic16_finalMapping[REG_ADDR(r)].alias != 0)
//#define REG_isVALID(r)     (pic16_finalMapping[REG_ADDR(r)].isValid)


typedef struct {
	int gen_banksel;
	int opt_banksel;
	int omit_configw;
	int omit_ivt;
	int leave_reset;
	int enable_stack;
	int stack_model;
} pic16_options_t;

#define USE_STACK	(pic16_options.enable_stack)
#define STACK_MODEL_SMALL	(pic16_options.stack_model == 0)
#define STACK_MODEL_LARGE	(pic16_options.stack_model == 1)


extern pic16_options_t pic16_options;
extern PIC16_device *pic16;

/****************************************/
void pic16_assignConfigWordValue(int address, int value);
int pic16_getConfigWord(int address);
int pic16_isREGinBank(regs *reg, int bank);
int pic16_REGallBanks(regs *reg);
void pic16_addMemRange(memRange *r, int type);
void pic16_setMaxRAM(int size);

void checkAddReg(set **set, regs *reg);


#endif  /* __DEVICE_H__ */
