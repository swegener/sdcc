/*-------------------------------------------------------------------------

   device.c - Accomodates subtle variations in PIC16 devices
   Written By -  Scott Dattalo scott@dattalo.com
   Ported to PIC16 By -  Martin Dubuc m.dubuc@rogers.com

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

#include <stdio.h>

#include "common.h"   // Include everything in the SDCC src directory
#include "newalloc.h"


#include "pcode.h"
#include "ralloc.h"
#include "device.h"

#if defined(__BORLANDC__) || defined(_MSC_VER)
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

static PIC_device Pics[] = {
  {
    {"p18f242", "18f242", "pic18f242", "f242"},
    (memRange *)NULL,
    (memRange *)NULL,
    0,
    0x300,
  },

  {
    {"p18f252", "18f252", "pic18f252", "f252"},
    (memRange *)NULL,
    (memRange *)NULL,
    0,
    0x600,
  },

  {
    {"p18f442", "18f442", "pic18f442", "f442"},
    (memRange *)NULL,
    (memRange *)NULL,
    0,
    0x300,
  },

  {
    {"p18f452", "18f452", "pic18f452", "f452"},
    (memRange *)NULL,
    (memRange *)NULL,
    0,
    0x600,
  },

};

static int num_of_supported_PICS = sizeof(Pics)/sizeof(PIC_device);

#define DEFAULT_PIC "f452"

static PIC_device *pic=NULL;

AssignedMemory *pic16_finalMapping=NULL;

#define DEFAULT_CONFIG_BYTE 0xff

#define CONFIG1H_WORD_ADDRESS 0x300001
#define DEFAULT_CONFIG1H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG2L_WORD_ADDRESS 0x300002
#define DEFAULT_CONFIG2L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG2H_WORD_ADDRESS 0x300003
#define DEFAULT_CONFIG2H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG3H_WORD_ADDRESS 0x300005
#define DEFAULT_CONFIG3H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG4L_WORD_ADDRESS 0x300006
#define DEFAULT_CONFIG4L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG5L_WORD_ADDRESS 0x300008
#define DEFAULT_CONFIG5L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG5H_WORD_ADDRESS 0x300009
#define DEFAULT_CONFIG5H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG6L_WORD_ADDRESS 0x30000a
#define DEFAULT_CONFIG6L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG6H_WORD_ADDRESS 0x30000b
#define DEFAULT_CONFIG6H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG7L_WORD_ADDRESS 0x30000c
#define DEFAULT_CONFIG7L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG7H_WORD_ADDRESS 0x30000d
#define DEFAULT_CONFIG7H_WORD DEFAULT_CONFIG_BYTE

static unsigned int config1h_word = DEFAULT_CONFIG1H_WORD;
static unsigned int config2l_word = DEFAULT_CONFIG2L_WORD;
static unsigned int config2h_word = DEFAULT_CONFIG2H_WORD;
static unsigned int config3h_word = DEFAULT_CONFIG3H_WORD;
static unsigned int config4l_word = DEFAULT_CONFIG4L_WORD;
static unsigned int config5l_word = DEFAULT_CONFIG5L_WORD;
static unsigned int config5h_word = DEFAULT_CONFIG5H_WORD;
static unsigned int config6l_word = DEFAULT_CONFIG6L_WORD;
static unsigned int config6h_word = DEFAULT_CONFIG6H_WORD;
static unsigned int config7l_word = DEFAULT_CONFIG7L_WORD;
static unsigned int config7h_word = DEFAULT_CONFIG7H_WORD;

void pic16_addMemRange(memRange *r, int type)
{
  int i;
  int alias = r->alias;

  if (pic->maxRAMaddress < 0) {
    fprintf(stderr, "missing \"#pragma maxram\" setting\n");
    return;
  }

  do {
    for (i=r->start_address; i<= r->end_address; i++) {
      if ((i|alias) <= pic->maxRAMaddress) {
	pic16_finalMapping[i | alias].isValid = 1;
	pic16_finalMapping[i | alias].alias = r->alias;
	pic16_finalMapping[i | alias].bank  = r->bank;
	if(type) {
	  /* hack for now */
	  pic16_finalMapping[i | alias].isSFR  = 1;
	} else {
	  pic16_finalMapping[i | alias].isSFR  = 0;
	}
      } else {
	fprintf(stderr, "WARNING: %s:%s memory at 0x%x is beyond max ram = 0x%x\n",
		__FILE__,__FUNCTION__,(i|alias), pic->maxRAMaddress);
      }
    }

    /* Decrement alias */
    if (alias) {
      alias -= ((alias & (alias - 1)) ^ alias);
    } else {
      alias--;
    }

  } while (alias >= 0);
}

void pic16_setMaxRAM(int size)
{
  int i;
  pic->maxRAMaddress = size;

  if (pic->maxRAMaddress < 0) {
    fprintf(stderr, "invalid \"#pragma maxram 0x%x\" setting\n",
	    pic->maxRAMaddress);
    return;
  }

  pic16_finalMapping = Safe_calloc(1+pic->maxRAMaddress,
			     sizeof(AssignedMemory));

  /* Now initialize the pic16_finalMapping array */

  for(i=0; i<=pic->maxRAMaddress; i++) {
    pic16_finalMapping[i].reg = NULL;
    pic16_finalMapping[i].isValid = 0;
  }
}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/

int pic16_isREGinBank(regs *reg, int bank)
{

  if(!reg || !pic)
    return 0;

  if(pic16_finalMapping[reg->address].bank == bank)
    return 1;

  return 0;
}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/
int pic16_REGallBanks(regs *reg)
{

  if(!reg || !pic)
    return 0;

  if (reg->address > pic->maxRAMaddress)
    return 0;

  return 1;

}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/

/*
 *  pic16_dump_map -- debug stuff
 */

void pic16_dump_map(void)
{
  int i;

  for(i=0; i<=pic->maxRAMaddress; i++) {
    //fprintf(stdout , "addr 0x%02x is %s\n", i, ((pic16_finalMapping[i].isValid) ? "valid":"invalid"));

    if(pic16_finalMapping[i].isValid) {
      fprintf(stderr,"addr: 0x%02x",i);
      if(pic16_finalMapping[i].isSFR)
	fprintf(stderr," isSFR");
      if(pic16_finalMapping[i].reg) 
	fprintf( stderr, "  reg %s", pic16_finalMapping[i].reg->name);
      fprintf(stderr, "\n");
    }
  }

}

void pic16_dump_cblock(FILE *of)
{
  int start=-1;
  int addr=0;
  int bank_base;

  //pic16_dump_map();   /* display the register map */

  if (pic->maxRAMaddress < 0) {
    fprintf(stderr, "missing \"#pragma maxram\" setting\n");
    return;
  }

  do {

    if(pic16_finalMapping[addr].reg && !pic16_finalMapping[addr].reg->isEmitted) {

      if(start<0)
	start = addr;
    } else {
      if(start>=0) {

	/* clear the lower 7-bits of the start address of the first
	 * variable declared in this bank. The upper bits for the mid
	 * range pics are the bank select bits.
	 */

	bank_base = start & 0xfff8;

	/* The bank number printed in the cblock comment tacitly
	 * assumes that the first register in the contiguous group
	 * of registers represents the bank for the whole group */

        if ((pic16_finalMapping[start].bank == 0 && start <= 0x7f) ||
            pic16_finalMapping[start].isSFR)
	  fprintf(of,"  cblock  0X%04X\t; Access Bank\n",start);
        else
	  fprintf(of,"  cblock  0X%04X\t; Bank %d\n",start,pic16_finalMapping[start].bank);

	for( ; start < addr; start++) {
	  if((pic16_finalMapping[start].reg) && !pic16_finalMapping[start].reg->isEmitted ) {
	    fprintf(of,"\t%s",pic16_finalMapping[start].reg->name);

	    /* If this register is aliased in multiple banks, then
	     * mangle the variable name with the alias address: */
	    if(pic16_finalMapping[start].alias & start)
	      fprintf(of,"_%x",bank_base);

	    if(pic16_finalMapping[start].instance)
	      fprintf(of,"_%d",pic16_finalMapping[start].instance);

	    
	    fputc('\n',of);

	    //pic16_finalMapping[start].reg->isEmitted = 1;
	  }
	}

	fprintf(of,"  endc\n");

	start = -1;
      }

    }

    addr++;

  } while(addr <= pic->maxRAMaddress);
  

}

/*-----------------------------------------------------------------*
 *  void pic16_list_valid_pics(int ncols, int list_alias)
 *
 * Print out a formatted list of valid PIC devices
 *
 * ncols - number of columns in the list.
 *
 * list_alias - if non-zero, print all of the supported aliases
 *              for a device (e.g. F84, 16F84, etc...)
 *-----------------------------------------------------------------*/
void pic16_list_valid_pics(int ncols, int list_alias)
{
  int col,longest;
  int i,j,k,l;

  if(list_alias)
    list_alias = sizeof(Pics[0].name) / sizeof(Pics[0].name[0]);

  /* decrement the column number if it's greater than zero */
  ncols = (ncols > 1) ? ncols-1 : 4;

  /* Find the device with the longest name */
  for(i=0,longest=0; i<num_of_supported_PICS; i++) {
    for(j=0; j<=list_alias; j++) {
      k = strlen(Pics[i].name[j]);
      if(k>longest)
	longest = k;
    }
  }

  col = 0;

  for(i=0;  i < num_of_supported_PICS; i++) {
    j = 0;
    do {

      fprintf(stderr,"%s", Pics[i].name[j]);
      if(col<ncols) {
	l = longest + 2 - strlen(Pics[i].name[j]);
	for(k=0; k<l; k++)
	  fputc(' ',stderr);

	col++;

      } else {
	fputc('\n',stderr);
	col = 0;
      }

    } while(++j<list_alias);

  }
  if(col != ncols)
    fputc('\n',stderr);

}

/*-----------------------------------------------------------------*
 *  
 *-----------------------------------------------------------------*/
PIC_device *pic16_find_device(char *name)
{

  int i,j;

  if(!name)
    return NULL;

  for(i = 0; i<num_of_supported_PICS; i++) {

    for(j=0; j<PROCESSOR_NAMES; j++)
      if(!STRCASECMP(Pics[i].name[j], name) )
	return &Pics[i];
  }

  /* not found */
  return NULL; 
}

/*-----------------------------------------------------------------*
 *  
 *-----------------------------------------------------------------*/
void pic16_init_pic(char *pic_type)
{
  pic = pic16_find_device(pic_type);

  if(!pic) {
    if(pic_type)
      fprintf(stderr, "'%s' was not found.\n", pic_type);
    else
      fprintf(stderr, "No processor has been specified (use -pPROCESSOR_NAME)\n");

    fprintf(stderr,"Valid devices are:\n");

    pic16_list_valid_pics(4,0);
    exit(1);
  }

  pic->maxRAMaddress = -1;
}

/*-----------------------------------------------------------------*
 *  
 *-----------------------------------------------------------------*/
int pic16_picIsInitialized(void)
{
  if(pic && pic->maxRAMaddress > 0)
    return 1;

  return 0;

}

/*-----------------------------------------------------------------*
 *  char *pic16_processor_base_name(void) - Include file is derived from this.
 *-----------------------------------------------------------------*/
char *pic16_processor_base_name(void)
{

  if(!pic)
    return NULL;

  return pic->name[0];
}

static int isSFR(int address)
{

  if( (address > pic->maxRAMaddress) || !pic16_finalMapping[address].isSFR)
    return 0;

  return 1;

}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/
static int validAddress(int address, int reg_size)
{
  int i;

  if (pic->maxRAMaddress < 0) {
    fprintf(stderr, "missing \"#pragma maxram\" setting\n");
    return 0;
  }
//  fprintf(stderr, "validAddress: Checking 0x%04x (max=0x%04x) (reg_size = %d)\n",address, pic->maxRAMaddress, reg_size);
  if(address > pic->maxRAMaddress)
    return 0;

  for (i=0; i<reg_size; i++)
    if(!pic16_finalMapping[address + i].isValid || 
       pic16_finalMapping[address+i].reg ||
       pic16_finalMapping[address+i].isSFR )
      return 0;

  return 1;
}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/
static void mapRegister(regs *reg)
{

  int i;
  int alias;

  if(!reg || !reg->size) {
    fprintf(stderr,"WARNING: %s:%s:%d Bad register\n",__FILE__,__FUNCTION__,__LINE__);
    return;
  }

  if (pic->maxRAMaddress < 0) {
    fprintf(stderr, "missing \"#pragma maxram\" setting\n");
    return;
  }

  for(i=0; i<reg->size; i++) {

    alias = pic16_finalMapping[reg->address].alias;
    reg->alias = alias;

    do {

//	fprintf(stdout,"mapping %s to address 0x%02x, reg size = %d\n",reg->name, (reg->address+alias+i),reg->size);

      pic16_finalMapping[reg->address + alias + i].reg = reg;
      pic16_finalMapping[reg->address + alias + i].instance = i;

      /* Decrement alias */
      if(alias)
	alias -= ((alias & (alias - 1)) ^ alias);
      else
	alias--;

    } while (alias>=0);
  }

  //  fprintf(stderr,"%s - %s addr = 0x%03x, size %d\n",__FUNCTION__,reg->name, reg->address,reg->size);

  reg->isMapped = 1;

}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/
static int assignRegister(regs *reg, int start_address)
{
  int i;

	//fprintf(stderr,"%s -  %s start_address = 0x%03x\n",__FUNCTION__,reg->name, start_address);
  if(reg->isFixed) {

    if (validAddress(reg->address,reg->size)) {
      //fprintf(stderr,"%s -  %s address = 0x%03x\n",__FUNCTION__,reg->name, reg->address);
      mapRegister(reg);
      return reg->address;
    }

    if( isSFR(reg->address)) {
      mapRegister(reg);
      return reg->address;
    }

    //fprintf(stderr, "WARNING: Ignoring Out of Range register assignment at fixed address %d, %s\n",
    //    reg->address, reg->name);

  } else {

    /* This register does not have a fixed address requirement
     * so we'll search through all availble ram address and
     * assign the first one */

    for (i=start_address; i<=pic->maxRAMaddress; i++) {

      if (validAddress(i,reg->size)) {
//	fprintf(stderr, "found valid address = 0x%04x\n", i);
	reg->address = i;
	mapRegister(reg);
	return i;
      }
    }

    fprintf(stderr, "WARNING: No more RAM available for %s\n",reg->name);

  }

  return -1;
}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/
void pic16_assignFixedRegisters(set *regset)
{
  regs *reg;

  for (reg = setFirstItem(regset) ; reg ; 
       reg = setNextItem(regset)) {

    if(reg->isFixed) 
      assignRegister(reg,0);
  }

}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/
void pic16_assignRelocatableRegisters(set *regset, int used)
{

  regs *reg;
  int address = 0;

  for (reg = setFirstItem(regset) ; reg ; 
       reg = setNextItem(regset)) {

    //fprintf(stdout,"assigning %s isFixed=%d, wasUsed=%d\n",reg->name,reg->isFixed,reg->wasUsed);

    if((!reg->isFixed) && (used || reg->wasUsed))
      address = assignRegister(reg,address);

  }

}


/*-----------------------------------------------------------------*
 *  void pic16_assignConfigWordValue(int address, int value)
 *
 * All high performance RISC CPU PICs have seven config word starting
 * at address 0x300000.
 * This routine will assign a value to that address.
 *
 *-----------------------------------------------------------------*/

void pic16_assignConfigWordValue(int address, int value)
{
  switch(address) {
  case CONFIG1H_WORD_ADDRESS:
    config1h_word = value;
    break;
  case CONFIG2L_WORD_ADDRESS:
    config2l_word = value;
    break;
  case CONFIG2H_WORD_ADDRESS:
    config2h_word = value;
    break;
  case CONFIG3H_WORD_ADDRESS:
    config3h_word = value;
    break;
  case CONFIG4L_WORD_ADDRESS:
    config4l_word = value;
    break;
  case CONFIG5L_WORD_ADDRESS:
    config5l_word = value;
    break;
  case CONFIG5H_WORD_ADDRESS:
    config5h_word = value;
    break;
  case CONFIG6L_WORD_ADDRESS:
    config6l_word = value;
    break;
  case CONFIG6H_WORD_ADDRESS:
    config6h_word = value;
    break;
  case CONFIG7L_WORD_ADDRESS:
    config7l_word = value;
    break;
  case CONFIG7H_WORD_ADDRESS:
    config7h_word = value;
    break;
  }

  //fprintf(stderr,"setting config word to 0x%x\n",value);

}
/*-----------------------------------------------------------------*
 * int pic16_getConfigWord(int address)
 *
 * Get the current value of the config word.
 *
 *-----------------------------------------------------------------*/

int pic16_getConfigWord(int address)
{
  switch(address) {
  case CONFIG1H_WORD_ADDRESS:
    return config1h_word;
  case CONFIG2L_WORD_ADDRESS:
    return config2l_word;
  case CONFIG2H_WORD_ADDRESS:
    return config2h_word;
  case CONFIG3H_WORD_ADDRESS:
    return config3h_word;
  case CONFIG4L_WORD_ADDRESS:
    return config4l_word;
  case CONFIG5L_WORD_ADDRESS:
    return config5l_word;
  case CONFIG5H_WORD_ADDRESS:
    return config5h_word;
  case CONFIG6L_WORD_ADDRESS:
    return config6l_word;
  case CONFIG6H_WORD_ADDRESS:
    return config6h_word;
  case CONFIG7L_WORD_ADDRESS:
    return config7l_word;
  case CONFIG7H_WORD_ADDRESS:
    return config7h_word;
  default:
    return 0;
  }
}

