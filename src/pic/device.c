/*-------------------------------------------------------------------------

   device.c - Accomodates subtle variations in PIC devices
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
		{"p16f627", "16f627", "pic16f627", "f627"}, /* processor name */
		(memRange *)NULL,
		(memRange *)NULL,
		0,                /* max ram address (calculated) */
		0x1ff,            /* default max ram address */
		0x80,             /* Bank Mask */
	},
	
	{
	{"p16f628", "16f628", "pic16f628", "f628"},
		(memRange *)NULL,
		(memRange *)NULL,
		0,
		0x1ff,
		0x80,
	},
	
	{
		{"p16f84", "16f84", "pic16f84", "f84"},
			(memRange *)NULL,
			(memRange *)NULL,
			0,
			0xcf,
			0x80,
	},
	
	{
		{"p16f873", "16f873", "pic16f873", "f873"},
			(memRange *)NULL,
			(memRange *)NULL,
			0,
			0x1ff,
			0x180,
	},

	{
		{"p16f877", "16f877", "pic16f877", "f877"},
			(memRange *)NULL,
			(memRange *)NULL,
			0,
			0x1ff,
			0x180,
	},
	
	{
		{"p16f819", "16f819", "pic16f819", "f819"},
			(memRange *)NULL,
			(memRange *)NULL,
			0,
			0x1ff,
			0x80,
	},

};

static int num_of_supported_PICS = sizeof(Pics)/sizeof(PIC_device);

static PIC_device *pic=NULL;

AssignedMemory *finalMapping=NULL;

#define CONFIG_WORD_ADDRESS 0x2007
#define DEFAULT_CONFIG_WORD 0x3fff

static unsigned int config_word = DEFAULT_CONFIG_WORD;

void addMemRange(memRange *r, int type)
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
				finalMapping[i | alias].isValid = 1;
				finalMapping[i | alias].alias = r->alias;
				finalMapping[i | alias].bank  = r->bank;
				if(type) {
					/* hack for now */
					finalMapping[i | alias].isSFR  = 1;
				} else {
					finalMapping[i | alias].isSFR  = 0;
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

void setMaxRAM(int size)
{
	int i;
	pic->maxRAMaddress = size;
	
	if (pic->maxRAMaddress < 0) {
		fprintf(stderr, "invalid \"#pragma maxram 0x%x\" setting\n",
			pic->maxRAMaddress);
		return;
	}
	
	finalMapping = Safe_calloc(1+pic->maxRAMaddress,
		sizeof(AssignedMemory));
	
	/* Now initialize the finalMapping array */
	
	for(i=0; i<=pic->maxRAMaddress; i++) {
		finalMapping[i].reg = NULL;
		finalMapping[i].isValid = 0;
		finalMapping[i].bank = (i>>7);
	}
}

/*-----------------------------------------------------------------*
*-----------------------------------------------------------------*/

int isREGinBank(regs *reg, int bank)
{
	
	if(!reg || !pic)
		return 0;
	
	if((int)((reg->address | reg->alias) & pic->bankMask & bank) == bank)
		return 1;
	
	return 0;
}

/*-----------------------------------------------------------------*
*-----------------------------------------------------------------*/
int REGallBanks(regs *reg)
{
	
	if(!reg || !pic)
		return 0;
	
	return ((reg->address | reg->alias) & pic->bankMask);
	
}

/*-----------------------------------------------------------------*
*-----------------------------------------------------------------*/

/*
*  dump_map -- debug stuff
*/

void dump_map(void)
{
	int i;
	
	for(i=0; i<=pic->maxRAMaddress; i++) {
		//fprintf(stdout , "addr 0x%02x is %s\n", i, ((finalMapping[i].isValid) ? "valid":"invalid"));
		
		if(finalMapping[i].isValid) {
			fprintf(stderr,"addr: 0x%02x",i);
			if(finalMapping[i].isSFR)
				fprintf(stderr," isSFR");
			if(finalMapping[i].reg) 
				fprintf( stderr, "  reg %s", finalMapping[i].reg->name);
			fprintf(stderr, "\n");
		}
	}
	
}

void dump_sfr(FILE *of)
{
	
	int start=-1;
	int addr=0;
	int bank_base;
	static int udata_flag=0;
	
	//dump_map();   /* display the register map */
	//fprintf(stdout,";dump_sfr  \n");
	if (pic->maxRAMaddress < 0) {
		fprintf(stderr, "missing \"#pragma maxram\" setting\n");
		return;
	}
	
	do {
		
		if(finalMapping[addr].reg && !finalMapping[addr].reg->isEmitted) {
			
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
				
				if ( (start != addr) && (!udata_flag) ) {
					udata_flag = 1;
					//fprintf(of,"\tudata\n");
				}
				
				for( ; start < addr; start++) {
					if((finalMapping[start].reg) && 
						(!finalMapping[start].reg->isEmitted) &&
						(!finalMapping[start].instance) && 
						(!finalMapping[start].isSFR)) {
						
						if (finalMapping[start].reg->isFixed) {
							unsigned i;
							for (i=0; i<finalMapping[start].reg->size; i++) {
								fprintf(of,"%s\tEQU\t0x%04x\n",
									finalMapping[start].reg->name, 
									finalMapping[start].reg->address+i);
							}
						} else {
							fprintf(of,"%s\tres\t%i\n",
								finalMapping[start].reg->name, 
								finalMapping[start].reg->size);
						}
						finalMapping[start].reg->isEmitted = 1;
					}
				}
				
				start = -1;
			}
			
		}
		
		addr++;
		
	} while(addr <= pic->maxRAMaddress);
	
	
}

/*-----------------------------------------------------------------*
*  void list_valid_pics(int ncols, int list_alias)
*
* Print out a formatted list of valid PIC devices
*
* ncols - number of columns in the list.
*
* list_alias - if non-zero, print all of the supported aliases
*              for a device (e.g. F84, 16F84, etc...)
*-----------------------------------------------------------------*/
void list_valid_pics(int ncols, int list_alias)
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
PIC_device *find_device(char *name)
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
void init_pic(char *pic_type)
{
	pic = find_device(pic_type);
	
	if(!pic) {
		if(pic_type)
			fprintf(stderr, "'%s' was not found.\n", pic_type);
		else
			fprintf(stderr, "No processor has been specified (use -pPROCESSOR_NAME)\n");
		
		fprintf(stderr,"Valid devices are:\n");
		
		list_valid_pics(4,0);
		exit(1);
	}
	
	pic->maxRAMaddress = -1;
}

/*-----------------------------------------------------------------*
*  
*-----------------------------------------------------------------*/
int picIsInitialized(void)
{
	if(pic && pic->maxRAMaddress > 0)
		return 1;
	
	return 0;
	
}

/*-----------------------------------------------------------------*
*  char *processor_base_name(void) - Include file is derived from this.
*-----------------------------------------------------------------*/
char *processor_base_name(void)
{
	
	if(!pic)
		return NULL;
	
	return pic->name[0];
}

int isSFR(int address)
{
	
	if( (address > pic->maxRAMaddress) || !finalMapping[address].isSFR)
		return 0;
	
	return 1;
	
}

/*-----------------------------------------------------------------*
*-----------------------------------------------------------------*/
int validAddress(int address, int reg_size)
{
	int i;
	
	if (pic->maxRAMaddress < 0) {
		fprintf(stderr, "missing \"#pragma maxram\" setting\n");
		return 0;
	}
	//  fprintf(stderr, "validAddress: Checking 0x%04x\n",address);
	if(address > pic->maxRAMaddress)
		return 0;
	
	for (i=0; i<reg_size; i++)
		if(!finalMapping[address + i].isValid || 
			finalMapping[address+i].reg ||
			finalMapping[address+i].isSFR )
			return 0;
		
		return 1;
}

/*-----------------------------------------------------------------*
*-----------------------------------------------------------------*/
void mapRegister(regs *reg)
{
	
	unsigned i;
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
		
		alias = finalMapping[reg->address].alias;
		reg->alias = alias;
		
		do {
			
			//fprintf(stdout,"mapping %s to address 0x%02x, reg size = %d\n",reg->name, (reg->address+alias+i),reg->size);
			
			finalMapping[reg->address + alias + i].reg = reg;
			finalMapping[reg->address + alias + i].instance = i;
			
			/* Decrement alias */
			if(alias)
				alias -= ((alias & (alias - 1)) ^ alias);
			else
				alias--;
			
		} while (alias>=0);
	}
	
	//fprintf(stderr,"%s - %s addr = 0x%03x, size %d\n",__FUNCTION__,reg->name, reg->address,reg->size);
	
	reg->isMapped = 1;
	
}

/*-----------------------------------------------------------------*
*-----------------------------------------------------------------*/
int assignRegister(regs *reg, int start_address)
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
void assignFixedRegisters(set *regset)
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
void assignRelocatableRegisters(set *regset, int used)
{
	
	regs *reg;
	int address = 0;
	
	for (reg = setFirstItem(regset) ; reg ; 
	reg = setNextItem(regset)) {
		
		//fprintf(stdout,"assigning %s isFixed=%d, wasUsed=%d\n",reg->name,reg->isFixed,reg->wasUsed);
		
		if((!reg->isFixed) && ( used || reg->wasUsed)) {
			/* If register have been reused then shall not print it a second time. */
			set *s;
			int done = 0;
			for (s = regset; s; s = s->next) {
				regs *r;
				r = s->item;
				if (r == reg)
					break;
				if((!r->isFixed) && ( used || r->wasUsed)) {
					if (r->rIdx == reg->rIdx) {
						reg->address = r->address;
						done = 1;
						break;
					}
				}
			}
			if (!done)
				address = assignRegister(reg,address);
		}
	}
	
}


/*-----------------------------------------------------------------*
*  void assignConfigWordValue(int address, int value)
*
* All midrange PICs have one config word at address 0x2007.
* This routine will assign a value to that address.
*
*-----------------------------------------------------------------*/

void assignConfigWordValue(int address, int value)
{
	if(CONFIG_WORD_ADDRESS == address)
		config_word = value;
	
	//fprintf(stderr,"setting config word to 0x%x\n",value);
	
}
/*-----------------------------------------------------------------*
* int getConfigWord(int address)
*
* Get the current value of the config word.
*
*-----------------------------------------------------------------*/

int getConfigWord(int address)
{
	if(CONFIG_WORD_ADDRESS == address)
		return config_word;
	
	else
		return 0;
	
}

/*-----------------------------------------------------------------*
*  
*-----------------------------------------------------------------*/
void setDefMaxRam(void)
{
	unsigned i;
	setMaxRAM(pic->defMaxRAMaddrs); /* Max RAM has not been included, so use default setting */
	/* Validate full memory range for use by general purpose RAM */
	for (i=pic->defMaxRAMaddrs; i--; ) {
		finalMapping[i].bank = (i>>7);
		finalMapping[i].isValid = 1;
	}
}

/*-----------------------------------------------------------------*
*  
*-----------------------------------------------------------------*/
unsigned getMaxRam(void)
{
	return pic->defMaxRAMaddrs;
}
