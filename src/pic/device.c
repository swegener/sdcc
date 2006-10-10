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
#include <stdlib.h>
#include <ctype.h>

#include "common.h"   // Include everything in the SDCC src directory
#include "newalloc.h"


#include "main.h"
#include "pcode.h"
#include "ralloc.h"
#include "device.h"

#if defined(__BORLANDC__) || defined(_MSC_VER)
#define STRCASECMP stricmp
#define STRNCASECMP strnicmp
#else
#define STRCASECMP strcasecmp
#define STRNCASECMP strncasecmp
#endif

extern int Gstack_base_addr;
extern int Gstack_size;

#define MAX_PICLIST 200
static PIC_device *Pics[MAX_PICLIST];
static int num_of_supported_PICS = 0;

static PIC_device *pic=NULL;

int maxRAMaddress = 0;
AssignedMemory *finalMapping=NULL;

#define CONFIG_WORD_ADDRESS 0x2007
#define CONFIG2_WORD_ADDRESS 0x2008
#define DEFAULT_CONFIG_WORD 0x3fff
#define DEFAULT_CONFIG2_WORD 0x3ffc

#define DEVICE_FILE_NAME "pic14devices.txt"
#define PIC14_STRING_LEN 256
#define SPLIT_WORDS_MAX 16

static unsigned int config_word = DEFAULT_CONFIG_WORD;
static unsigned int config2_word = DEFAULT_CONFIG2_WORD;

extern int pic14_is_shared (regs *reg);
extern void emitSymbolToFile (FILE *of, const char *name, const char *section_type, int size, int addr, int useEQU, int globalize);


/* parse a value from the configuration file */
static int parse_config_value(char *str)
{
	if (str[strlen(str) - 1] == 'K')
		return atoi(str) * 1024;        /* like "1K" */
		
	else if (STRNCASECMP(str, "0x", 2) == 0)
		return strtol(str+2, NULL, 16); /* like "0x400" */
	
	else
		return atoi(str);               /* like "1024" */
}


/* split a line into words */
static int split_words(char result_word[SPLIT_WORDS_MAX][PIC14_STRING_LEN], char *str)
{
	char *pos = str;
	int num_words = 0;
	int ccount;
	
	while (*pos != '\0' && num_words < SPLIT_WORDS_MAX) {
		/* remove leading spaces */
		while (isspace(*pos) || *pos == ',')
			pos++;
	
		/* copy everything up until the first space or comma */
		for (ccount = 0; *pos != '\0' && !isspace(*pos) && *pos != ',' && ccount < PIC14_STRING_LEN-1; ccount++, pos++)
			result_word[num_words][ccount] = *pos;
		result_word[num_words][ccount] = '\0';
		
		num_words++;
	}
	
	return num_words;
}


/* remove annoying prefixes from the processor name */
static char *sanitise_processor_name(char *name)
{
	char *proc_pos = name;

	if (name == NULL)
		return NULL;
	
	if (STRNCASECMP(proc_pos, "pic", 3) == 0)
		proc_pos += 3;

	else if (tolower(*proc_pos) == 'p')
		proc_pos += 1;
				
	return proc_pos;
}


/* create a structure for a pic processor */
static PIC_device *create_pic(char *pic_name, int maxram, int bankmsk, int confsiz, int program, int data, int eeprom, int io)
{
	PIC_device *new_pic;
	char *simple_pic_name = sanitise_processor_name(pic_name);
	
	new_pic = Safe_calloc(1, sizeof(PIC_device));
	new_pic->name = Safe_calloc(strlen(simple_pic_name)+1, sizeof(char));
	strcpy(new_pic->name, simple_pic_name);
			
	new_pic->defMaxRAMaddrs = maxram;
	new_pic->bankMask = bankmsk;
	new_pic->hasSecondConfigReg = confsiz > 1;
	
	new_pic->programMemSize = program;
	new_pic->dataMemSize = data;
	new_pic->eepromMemSize = eeprom;
	new_pic->ioPins = io;
	
	Pics[num_of_supported_PICS] = new_pic;
	num_of_supported_PICS++;
			
	return new_pic;
}


/* mark some registers as being duplicated across banks */
static void register_map(int num_words, char word[SPLIT_WORDS_MAX][PIC14_STRING_LEN])
{
	memRange r;
	int pcount;
	
	if (num_words < 3) {
		fprintf(stderr, "WARNING: not enough values in %s regmap directive\n", DEVICE_FILE_NAME);
		return;
	}
	
	r.alias = parse_config_value(word[1]);

	for (pcount = 2; pcount < num_words; pcount++) {
	    
		r.start_address = parse_config_value(word[pcount]);
		r.end_address = parse_config_value(word[pcount]);
		r.bank = (r.start_address >> 7) & 3;
		
		addMemRange(&r, 1);
	}
}


/* define ram areas - may be duplicated across banks */
static void ram_map(int num_words, char word[SPLIT_WORDS_MAX][PIC14_STRING_LEN])
{
	memRange r;
	
	if (num_words < 4) {
		fprintf(stderr, "WARNING: not enough values in %s memmap directive\n", DEVICE_FILE_NAME);
		return;
	}
	
	r.start_address = parse_config_value(word[1]);
	r.end_address = parse_config_value(word[2]);
	r.alias = parse_config_value(word[3]);
	r.bank = (r.start_address >> 7) & 3;
		
	addMemRange(&r, 0);
}

extern set *includeDirsSet;
extern set *userIncDirsSet;
extern set *libDirsSet;
extern set *libPathsSet;

/* read the file with all the pic14 definitions and pick out the definition for a processor
 * if specified. if pic_name is NULL reads everything */
static PIC_device *find_device(char *pic_name)
{
	FILE *pic_file;
	char pic_buf[PIC14_STRING_LEN];
	char *pic_buf_pos;
	int found_processor = FALSE;
	int done = FALSE;
	char processor_name[SPLIT_WORDS_MAX][PIC14_STRING_LEN];
	int num_processor_names = 0;
	int pic_maxram = 0;
	int pic_bankmsk = 0;
	int pic_confsiz = 0;
	int pic_program = 0;
	int pic_data = 0;
	int pic_eeprom = 0;
	int pic_io = 0;
	char *simple_pic_name;
	char *dir;
	char filename[512];
	int len = 512;
	
	/* allow abbreviations of the form "f877" - convert to "16f877" */
	simple_pic_name = sanitise_processor_name(pic_name);
	num_of_supported_PICS = 0;
	
	/* open the piclist file */
	/* first scan all include directories */
	pic_file = NULL;
	//fprintf( stderr, "%s: searching %s\n", __FUNCTION__, DEVICE_FILE_NAME );
	for (dir = setFirstItem(includeDirsSet);
		!pic_file && dir;
		dir = setNextItem(includeDirsSet))
	{
	  //fprintf( stderr, "searching1 %s\n", dir );
	  SNPRINTF(&filename[0], len, "%s%s%s", dir, DIR_SEPARATOR_STRING, DEVICE_FILE_NAME);
	  pic_file = fopen( filename, "rt" );
	  if (pic_file) break;
	} // for
	for (dir = setFirstItem(userIncDirsSet);
		!pic_file && dir;
		dir = setNextItem(userIncDirsSet))
	{
	  //fprintf( stderr, "searching2 %s\n", dir );
	  SNPRINTF(&filename[0], len, "%s%s%s", dir, DIR_SEPARATOR_STRING, DEVICE_FILE_NAME);
	  pic_file = fopen( filename, "rt" );
	  if (pic_file) break;
	} // for
	for (dir = setFirstItem(libDirsSet);
		!pic_file && dir;
		dir = setNextItem(libDirsSet))
	{
	  //fprintf( stderr, "searching3 %s\n", dir );
	  SNPRINTF(&filename[0], len, "%s%s%s", dir, DIR_SEPARATOR_STRING, DEVICE_FILE_NAME);
	  pic_file = fopen( filename, "rt" );
	  if (pic_file) break;
	} // for
	for (dir = setFirstItem(libPathsSet);
		!pic_file && dir;
		dir = setNextItem(libPathsSet))
	{
	  //fprintf( stderr, "searching4 %s\n", dir );
	  SNPRINTF(&filename[0], len, "%s%s%s", dir, DIR_SEPARATOR_STRING, DEVICE_FILE_NAME);
	  pic_file = fopen( filename, "rt" );
	  if (pic_file) break;
	} // for
	if (!pic_file) {
	  pic_file = fopen(DATADIR LIB_DIR_SUFFIX DIR_SEPARATOR_STRING "pic" DIR_SEPARATOR_STRING DEVICE_FILE_NAME, "rt");
	}
	if (pic_file == NULL) {
		/* this second attempt is used when initially building the libraries */
		pic_file = fopen(".." DIR_SEPARATOR_STRING ".." DIR_SEPARATOR_STRING ".." DIR_SEPARATOR_STRING ".." 
				DIR_SEPARATOR_STRING "src" DIR_SEPARATOR_STRING "pic" DIR_SEPARATOR_STRING 
				DEVICE_FILE_NAME, "rt");
		if (pic_file == NULL) {
			fprintf(stderr, "can't find %s\n", DATADIR LIB_DIR_SUFFIX DIR_SEPARATOR_STRING "pic" 
					DIR_SEPARATOR_STRING DEVICE_FILE_NAME);
			return NULL;
		}
	}
	
	/* read line by line */
	pic_buf[sizeof(pic_buf)-1] = '\0';
	while (fgets(pic_buf, sizeof(pic_buf)-1, pic_file) != NULL && !done) {
		
		/* remove trailing spaces */
		while (isspace(pic_buf[strlen(pic_buf)-1]))
			pic_buf[strlen(pic_buf)-1] = '\0';
		
		/* remove leading spaces */
		for (pic_buf_pos = pic_buf; isspace(*pic_buf_pos); pic_buf_pos++)
		{}
		
		/* ignore comment / empty lines */
		if (*pic_buf_pos != '\0' && *pic_buf_pos != '#') {
			
			/* split into fields */
			char pic_word[SPLIT_WORDS_MAX][PIC14_STRING_LEN];
			int num_pic_words;
			int wcount;
			
			num_pic_words = split_words(pic_word, pic_buf_pos);
			
			if (STRCASECMP(pic_word[0], "processor") == 0) {
			
				if (pic_name == NULL) {
					/* this is the mode where we read all the processors in - store the names for now */
					if (num_processor_names > 0) {
						/* store away all the previous processor definitions */
						int dcount;
						
						for (dcount = 1; dcount < num_processor_names; dcount++)
							create_pic(processor_name[dcount], pic_maxram, pic_bankmsk,
							           pic_confsiz, pic_program, pic_data, pic_eeprom, pic_io);
					}
					
					num_processor_names = split_words(processor_name, pic_buf_pos);
				}
				else {
					/* if we've just completed reading a processor definition stop now */
					if (found_processor)
						done = TRUE;
					else {
						/* check if this processor name is a match */
						for (wcount = 1; wcount < num_pic_words; wcount++) {
							
							/* skip uninteresting prefixes */
							char *found_name = sanitise_processor_name(pic_word[wcount]);

							if (STRCASECMP(found_name, simple_pic_name) == 0)
								found_processor = TRUE;
						}
					}
				}
			}
			
			else {
				if (found_processor || pic_name == NULL) {
					/* only parse a processor section if we've found the one we want */
					if (STRCASECMP(pic_word[0], "maxram") == 0 && num_pic_words > 1) {
						pic_maxram = parse_config_value(pic_word[1]);
						setMaxRAM(pic_maxram);
					}
					else if (STRCASECMP(pic_word[0], "bankmsk") == 0 && num_pic_words > 1)
						pic_bankmsk = parse_config_value(pic_word[1]);
			
					else if (STRCASECMP(pic_word[0], "confsiz") == 0 && num_pic_words > 1)
						pic_confsiz = parse_config_value(pic_word[1]);
			
					else if (STRCASECMP(pic_word[0], "program") == 0 && num_pic_words > 1)
						pic_program = parse_config_value(pic_word[1]);
			
					else if (STRCASECMP(pic_word[0], "data") == 0 && num_pic_words > 1)
						pic_data = parse_config_value(pic_word[1]);
			
					else if (STRCASECMP(pic_word[0], "eeprom") == 0 && num_pic_words > 1)
						pic_eeprom = parse_config_value(pic_word[1]);
			
					else if (STRCASECMP(pic_word[0], "io") == 0 && num_pic_words > 1)
						pic_io = parse_config_value(pic_word[1]);
			
					else if (STRCASECMP(pic_word[0], "regmap") == 0 && num_pic_words > 2) {
						if (found_processor)
							register_map(num_pic_words, pic_word);
					}
					else if (STRCASECMP(pic_word[0], "memmap") == 0 && num_pic_words > 2) {
						if (found_processor)
							ram_map(num_pic_words, pic_word);
					}
					else {
						fprintf(stderr, "WARNING: %s: bad syntax `%s'\n", DEVICE_FILE_NAME, pic_word[0]);
					}
				}
			}
		}
	}
	
	fclose(pic_file);

	/* if we're in read-the-lot mode then create the final processor definition */
	if (pic_name == NULL) {
	    
		if (num_processor_names > 0) {
			/* store away all the previous processor definitions */
			int dcount;
		
			for (dcount = 1; dcount < num_processor_names; dcount++)
				create_pic(processor_name[dcount], pic_maxram, pic_bankmsk,
				           pic_confsiz, pic_program, pic_data, pic_eeprom, pic_io);
		}
	}
	else {
		/* in search mode */
		if (found_processor) {
			/* create a new pic entry */
			return create_pic(pic_name, pic_maxram, pic_bankmsk,
			                  pic_confsiz, pic_program, pic_data, pic_eeprom, pic_io);
		}
	}
	
	return NULL;
}

void addMemRange(memRange *r, int type)
{
	int i;
	int alias = r->alias;
	
	if (maxRAMaddress < 0) {
		fprintf(stderr, "missing maxram setting in %s\n", DEVICE_FILE_NAME);
		return;
	}
	
	do {
		for (i=r->start_address; i<= r->end_address; i++) {
			if ((i|alias) <= maxRAMaddress) {
				/* if we haven't seen this address before, enter it */
				if (!finalMapping[i | alias].isValid) {
				finalMapping[i | alias].isValid = 1;
				finalMapping[i | alias].alias = r->alias;
				finalMapping[i | alias].bank  = r->bank;
				if(type) {
					/* hack for now */
					finalMapping[i | alias].isSFR  = 1;
				} else {
					finalMapping[i | alias].isSFR  = 0;
				}
				}
			} else {
				if (getenv("SDCCPICDEBUG")) {
					fprintf(stderr, "WARNING: %s:%s memory at 0x%x is beyond max ram = 0x%x\n",
						__FILE__,__FUNCTION__,(i|alias), maxRAMaddress);
				}
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
	maxRAMaddress = size;
	
	if (maxRAMaddress < 0) {
		fprintf(stderr, "invalid maxram 0x%x setting in %s\n",
			maxRAMaddress, DEVICE_FILE_NAME);
		return;
	}
	
	finalMapping = Safe_calloc(1+maxRAMaddress,
		sizeof(AssignedMemory));
	
	/* Now initialize the finalMapping array */
	
	for(i=0; i<=maxRAMaddress; i++) {
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

int isSFR(int address)
{
	
	if( (address > maxRAMaddress) || !finalMapping[address].isSFR)
		return 0;
	
	return 1;
	
}

/*
*  dump_map -- debug stuff
*/

void dump_map(void)
{
	int i;
	
	for(i=0; i<=maxRAMaddress; i++) {
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
#if 0
	int start=-1;
	int bank_base;
	static int udata_flag=0;
#endif
	int addr=0;
	
	//dump_map();   /* display the register map */
	//fprintf(stdout,";dump_sfr  \n");
	if (maxRAMaddress < 0) {
		fprintf(stderr, "missing maxram setting in %s\n", DEVICE_FILE_NAME);
		return;
	}
	
	for (addr = 0; addr <= maxRAMaddress; addr++)
	{
		regs *reg = finalMapping[addr].reg;
		
		if (reg && !reg->isEmitted)
		{
		  if (pic14_options.isLibrarySource && pic14_is_shared (reg))
		  {
		    /* rely on external declarations for the non-fixed stack */
		    /* Update: We always emit the STACK symbols into a
		     * udata_shr section, so no extern declaration is
		     * required. */
		    //fprintf (of, "\textern\t%s\n", reg->name);
		  } else {
		    emitSymbolToFile (of, reg->name, "udata", reg->size, reg->isFixed ? reg->address : -1, 0, pic14_is_shared (reg));
		  }
		  
		  reg->isEmitted = 1;
		}
	} // for

#if 0
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
							emitSymbolToFile (of, finalMapping[start].reg->name, finalMapping[start].reg->size);
#if 0
							fprintf(of,"%s\tres\t%i\n",
								finalMapping[start].reg->name, 
								finalMapping[start].reg->size);
#endif
						}
						finalMapping[start].reg->isEmitted = 1;
					}
				}
				
				start = -1;
			}
			
		}
		
		addr++;
		
	} while(addr <= maxRAMaddress);


#endif
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
void list_valid_pics(int ncols)
{
	int col=0,longest;
	int i,k,l;
	
	if (num_of_supported_PICS == 0)
		find_device(NULL);          /* load all the definitions */
	
	/* decrement the column number if it's greater than zero */
	ncols = (ncols > 1) ? ncols-1 : 4;
	
	/* Find the device with the longest name */
	for(i=0,longest=0; i<num_of_supported_PICS; i++) {
		k = strlen(Pics[i]->name);
		if(k>longest)
			longest = k;
	}
	
#if 1
	/* heading */
	fprintf(stderr, "\nPIC14 processors and their characteristics:\n\n");
	fprintf(stderr, " processor");
	for(k=0; k<longest-1; k++)
		fputc(' ',stderr);
	fprintf(stderr, "program     RAM      EEPROM    I/O\n");
	fprintf(stderr, "-----------------------------------------------------\n");
	
	for(i=0;  i < num_of_supported_PICS; i++) {
		fprintf(stderr,"  %s", Pics[i]->name);
		l = longest + 2 - strlen(Pics[i]->name);
		for(k=0; k<l; k++)
			fputc(' ',stderr);
	
		fprintf(stderr, "     ");
		if (Pics[i]->programMemSize % 1024 == 0)
			fprintf(stderr, "%4dK", Pics[i]->programMemSize / 1024);
		else
			fprintf(stderr, "%5d", Pics[i]->programMemSize);
	
		fprintf(stderr, "     %5d     %5d     %4d\n", 
			Pics[i]->dataMemSize, Pics[i]->eepromMemSize, Pics[i]->ioPins);
	}

	col = 0;
	
	fprintf(stderr, "\nPIC14 processors supported:\n");
	for(i=0;  i < num_of_supported_PICS; i++) {
			
		fprintf(stderr,"%s", Pics[i]->name);
		if(col<ncols) {
			l = longest + 2 - strlen(Pics[i]->name);
			for(k=0; k<l; k++)
				fputc(' ',stderr);
				
			col++;
				
		} else {
			fputc('\n',stderr);
			col = 0;
		}
		
	}
#endif
	if(col != 0)
		fputc('\n',stderr);
}

/*-----------------------------------------------------------------*
*  
*-----------------------------------------------------------------*/
void init_pic(char *pic_type)
{
	char long_name[PIC14_STRING_LEN];
	
	pic = find_device(pic_type);
	
	if (pic == NULL) {
		/* check for shortened "16xxx" form */
		sprintf(long_name, "16%s", pic_type);
		pic = find_device(long_name);
		if (pic == NULL) {
			if(pic_type != NULL && pic_type[0] != '\0')
				fprintf(stderr, "'%s' was not found.\n", pic_type);
			else
				fprintf(stderr, "No processor has been specified (use -pPROCESSOR_NAME)\n");
		
			list_valid_pics(7);
			exit(1);
		}
	}
}

/*-----------------------------------------------------------------*
*  
*-----------------------------------------------------------------*/
int picIsInitialized(void)
{
	if(pic && maxRAMaddress > 0)
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
	
	return pic->name;
}

/*-----------------------------------------------------------------*
*-----------------------------------------------------------------*/
int validAddress(int address, int reg_size)
{
	int i;
	
	if (maxRAMaddress < 0) {
		fprintf(stderr, "missing maxram setting in %s\n", DEVICE_FILE_NAME);
		return 0;
	}
	//  fprintf(stderr, "validAddress: Checking 0x%04x\n",address);
	assert (reg_size > 0);
	if(address + (reg_size - 1) > maxRAMaddress)
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
	
	if (maxRAMaddress < 0) {
		fprintf(stderr, "missing maxram setting in %s\n", DEVICE_FILE_NAME);
		return;
	}
	
	for(i=0; i<reg->size; i++) {
		
		assert(reg->address >= 0 && reg->address < maxRAMaddress);
		
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
		
		if (getenv("SDCCPICDEBUG")) {
			fprintf(stderr, "WARNING: Ignoring Out of Range register assignment at fixed address %d, %s\n",
			    reg->address, reg->name);
		}
		
	} else {
		
	/* This register does not have a fixed address requirement
	* so we'll search through all availble ram address and
		* assign the first one */
		
		for (i=start_address; i<=maxRAMaddress; i++) {
			
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
		
		//fprintf(stdout,"assigning %s (%d) isFixed=%d, wasUsed=%d\n",reg->name,reg->size,reg->isFixed,reg->wasUsed);
		
		if((!reg->isExtern) && (!reg->isFixed) && ( used || reg->wasUsed)) {
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

/* Keep track of whether we found an assignment to the __config words. */
static int pic14_hasSetConfigWord = 0;

/*-----------------------------------------------------------------*
 *  void assignConfigWordValue(int address, int value)
 *
 * Most midrange PICs have one config word at address 0x2007.
 * Newer PIC14s have a second config word at address 0x2008.
 * This routine will assign values to those addresses.
 *
 *-----------------------------------------------------------------*/

void pic14_assignConfigWordValue(int address, int value)
{
	if (CONFIG_WORD_ADDRESS == address)
		config_word = value;
	
	else if (CONFIG2_WORD_ADDRESS == address)
		config2_word = value;
	
	//fprintf(stderr,"setting config word 0x%x to 0x%x\n", address, value);
	pic14_hasSetConfigWord = 1;
}

/*-----------------------------------------------------------------*
 * int pic14_emitConfigWord (FILE * vFile)
 * 
 * Emit the __config directives iff we found a previous assignment
 * to the config word.
 *-----------------------------------------------------------------*/
extern char *iComments2;
int pic14_emitConfigWord (FILE * vFile)
{
  if (pic14_hasSetConfigWord)
  {
    fprintf (vFile, "%s", iComments2);
    fprintf (vFile, "; config word \n");
    fprintf (vFile, "%s", iComments2);
    if (pic14_getHasSecondConfigReg())
    {
      fprintf (vFile, "\t__config _CONFIG1, 0x%x\n", pic14_getConfigWord(0x2007));
      fprintf (vFile, "\t__config _CONFIG2, 0x%x\n", pic14_getConfigWord(0x2008));
    }
    else
      fprintf (vFile, "\t__config 0x%x\n", pic14_getConfigWord(0x2007));

    return 1;
  }
  return 0;
}

/*-----------------------------------------------------------------*
 * int pic14_getConfigWord(int address)
 *
 * Get the current value of a config word.
 *
 *-----------------------------------------------------------------*/

int pic14_getConfigWord(int address)
{
	switch (address)
	{
	case CONFIG_WORD_ADDRESS:
		return config_word;
	
	case CONFIG2_WORD_ADDRESS:
		return config2_word;
	
	default:
		return 0;
	}
}

/*-----------------------------------------------------------------*
*  
*-----------------------------------------------------------------*/
unsigned pic14_getMaxRam(void)
{
	return pic->defMaxRAMaddrs;
}


/*-----------------------------------------------------------------*
*  int getHasSecondConfigReg(void) - check if the device has a 
*  second config register, rather than just one.
*-----------------------------------------------------------------*/
int pic14_getHasSecondConfigReg(void)
{
	if(!pic)
		return 0;
	else
		return pic->hasSecondConfigReg;
}

/*-----------------------------------------------------------------*
 * Query the size of the sharebank of the selected device.
 * FIXME: Currently always returns 16.
 *-----------------------------------------------------------------*/
int pic14_getSharebankSize(void)
{
	if (pic14_options.stackSize <= 0) {
		// default size: 16 bytes
		return 16;
	} else {
		return pic14_options.stackSize;
	}
}

/*-----------------------------------------------------------------*
 * Query the highest byte address occupied by the sharebank of the
 * selected device.
 * THINK: Might not be needed, if we assign all shareable objects to
 *        a `udata_shr' section and let the linker do the rest...
 * Tried it, but yields `no target memory available' for pic16f877...
 *-----------------------------------------------------------------*/
int pic14_getSharebankAddress(void)
{
	int sharebankAddress = 0x7f;
	if (pic14_options.stackLocation != 0) {
	    // permanent (?) workaround for pic16f84a-like devices with hardly
	    // any memory:
	    // 0x00-0x0B SFR
	    // 0x0C-0x4F memory,
	    // 0x50-0x7F unimplemented (reads as 0),
	    // 0x80-0x8B SFRs (partly mapped to 0x0?)
	    // 0x8c-0xCF mapped to 0x0C-0x4F
	    sharebankAddress = pic14_options.stackLocation + pic14_getSharebankSize() - 1;
	} else {
	    /* If total RAM is less than 0x7f as with 16f84 then reduce
	     * sharebankAddress to fit */
	    if ((unsigned)sharebankAddress > pic14_getMaxRam())
		    sharebankAddress = (int)pic14_getMaxRam();
	}
	return sharebankAddress;
}

