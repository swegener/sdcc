/*-------------------------------------------------------------------------
  lkmem.c - Create a memory summary file with extension .mem

   Written By -  Jesus Calvino-Fraga, jesusc@ieee.org (2002)

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
#include <string.h>
#include <stdlib.h>
#include "aslink.h"

int summary(struct area * areap) 
{
	#define EQ(A,B) !strcmpi((A),(B))
	#define MIN_STACK 16
	#define REPORT_ERROR(A, H) \
	{\
		fprintf(of, "%s%s", (H)?"*** ERROR: ":"", (A)); \
		fprintf(stderr, "%s%s", (H)?"\n?ASlink-Error-":"",(A)); \
		toreturn=1; \
	}

	#define REPORT_WARNING(A, H) \
	{ \
		fprintf(of, "%s%s", (H)?"*** WARNING: ":"", (A)); \
		fprintf(stderr, "%s%s",(H)?"\n?ASlink-Warning-":"", (A)); \
	}

	char buff[128];
	int j, toreturn=0;
	unsigned int Total_Last=0, k; 

	struct area * xp;
	FILE * of;
	
	/*Artifacts used for printing*/
	char start[15], end[15], size[15], max[15];
	char format[]="   %-16.16s %-8.8s %-8.8s %-8.8s %-8.8s\n";
	char line[]="---------------------";

	typedef struct
	{
		unsigned long Start;
		unsigned long Size;
		unsigned long Max;
		char Name[NCPS];
		unsigned long flag;
	} _Mem;

	unsigned int dram[0x100];
	_Mem Ram[]={
		{0,		8,	8,	 "REG_BANK_0", 0x0001},
		{0x8,	8,	8,	 "REG_BANK_1", 0x0002},
		{0x10,	8,	8,	 "REG_BANK_2", 0x0004},
		{0x18,	8,	8,	 "REG_BANK_3", 0x0008},
		{0x20,	0,	16,	 "BSEG_BYTES", 0x0010},
		{0,		0,	128, "UNUSED",     0x0000},
		{0x7f,	0,	128, "DATA",       0x0020},
		{0,		0,	128, "TOTAL:",     0x0000}
	};
	
	_Mem IRam= {0xff,   0,   128, "INDIRECT RAM",		0x0080};
	_Mem Stack={0xff,   0,     1, "STACK",				0x0000};
	_Mem XRam= {0xffff, 0, 65536, "EXTERNAL RAM",		0x0100};
	_Mem Rom=  {0xffff, 0, 65536, "ROM/EPROM/FLASH",	0x0200};
	
	if(rflag) /*For the DS390*/
	{
		XRam.Max=0x1000000; /*24 bits*/
		XRam.Start=0xffffff;
		Rom.Max=0x1000000;
		Rom.Start=0xffffff;
	}

	if((iram_size<=0)||(iram_size>0x100)) /*Default: 8052 like memory*/
	{
		Ram[5].Max=0x80;
		Ram[6].Max=0x80;
		Ram[7].Max=0x80;
		IRam.Max=0x80;
		iram_size=0x100;
	}
	else if(iram_size<0x80)
	{
		Ram[5].Max=iram_size;
		Ram[6].Max=iram_size;
		Ram[7].Max=iram_size;
		IRam.Max=0;
	}
	else
	{
		Ram[5].Max=0x80;
		Ram[6].Max=0x80;
		Ram[7].Max=0x80;
		IRam.Max=iram_size-0x80;
	}

	for(j=0; j<(int)iram_size; j++) dram[j]=0;
	for(; j<0x100; j++) dram[j]=0x8000; /*Memory not available*/

	/* Open Memory Summary File*/
	of = afile(linkp->f_idp, "mem", 1);
	if (of == NULL)
	{
		lkexit(1);
	}

	xp=areap;
	while (xp)
	{
		/**/ if (EQ(xp->a_id, "REG_BANK_0"))
		{
			Ram[0].Size=xp->a_size;
		}
		else if (EQ(xp->a_id, "REG_BANK_1"))
		{
			Ram[1].Size=xp->a_size;
		}
		else if (EQ(xp->a_id, "REG_BANK_2"))
		{
			Ram[2].Size=xp->a_size;
		}
		else if (EQ(xp->a_id, "REG_BANK_3"))
		{
			Ram[3].Size=xp->a_size;
		}
		else if (EQ(xp->a_id, "BSEG_BYTES"))
		{
			Ram[4].Size=xp->a_size;
		}
		else if ( EQ(xp->a_id, "DSEG") || EQ(xp->a_id, "OSEG") )
		{
			Ram[6].Size+=xp->a_size;
			if(xp->a_addr<Ram[6].Start) Ram[6].Start=xp->a_addr;
		}

		else if( EQ(xp->a_id, "CSEG") || EQ(xp->a_id, "GSINIT") ||
				 EQ(xp->a_id, "GSFINAL") || EQ(xp->a_id, "HOME") )
		{
			Rom.Size+=xp->a_size;
			if(xp->a_addr<Rom.Start) Rom.Start=xp->a_addr;
		}
		
		else if (EQ(xp->a_id, "SSEG"))
		{
			Stack.Size+=xp->a_size;
			if(xp->a_addr<Stack.Start) Stack.Start=xp->a_addr;
		}

		else if (EQ(xp->a_id, "XSEG") || EQ(xp->a_id, "XISEG")) 
		{
			XRam.Size+=xp->a_size;
			if(xp->a_addr<XRam.Start) XRam.Start=xp->a_addr;
		}

		else if (EQ(xp->a_id, "ISEG"))
		{
			IRam.Size+=xp->a_size;
			if(xp->a_addr<IRam.Start) IRam.Start=xp->a_addr;
		}
		xp=xp->a_ap;
	}

	for(j=0; j<7; j++)
		for(k=Ram[j].Start; (k<(Ram[j].Start+Ram[j].Size))&&(k<0x100); k++)
			dram[k]|=Ram[j].flag; /*Mark as used*/
	
	for(k=IRam.Start; (k<(IRam.Start+IRam.Size))&&(k<0x100); k++)
		dram[k]|=IRam.flag; /*Mark as used*/

	/*Compute the amount of unused memory in direct data Ram.  This is the
	gap between the last register bank or bit segment and the data segment.*/
	for(k=Ram[6].Start-1; (dram[k]==0) && (k>0); k--);
	Ram[5].Start=k+1;
	Ram[5].Size=Ram[6].Start-Ram[5].Start; /*It may be zero (which is good!)*/

	/*Compute the data Ram totals*/
	for(j=0; j<7; j++)
	{
		if(Ram[7].Start>Ram[j].Start) Ram[7].Start=Ram[j].Start;
		Ram[7].Size+=Ram[j].Size;
	}
	Total_Last=Ram[6].Size+Ram[6].Start-1;

	/*Report the Ram totals*/
	fprintf(of, "Direct Internal RAM:\n");
	fprintf(of, format, "Name", "Start", "End", "Size", "Max");

	for(j=0; j<8; j++)
	{
		if((j==0) || (j==7)) fprintf(of, format, line, line, line, line, line);
		if((j!=5) || (Ram[j].Size>0))
		{
			sprintf(start, "0x%02lx", Ram[j].Start);
			if(Ram[j].Size==0)
				end[0]=0;/*Empty string*/
			else
				sprintf(end,  "0x%02lx", j==7?Total_Last:Ram[j].Size+Ram[j].Start-1);
			sprintf(size, "%5lu", Ram[j].Size);
			sprintf(max, "%5lu", Ram[j].Max);
			fprintf(of, format, Ram[j].Name, start, end, size, max);
		}
	}

	for(k=Ram[6].Start; (k<(Ram[6].Start+Ram[6].Size))&&(k<0x100); k++)
	{
		if(dram[k]!=Ram[6].flag)
		{
			sprintf(buff, "Internal memory overlap starting at 0x%02x.\n", k);
			REPORT_ERROR(buff, 1);
			break;
		}
	}

	if(Ram[4].Size>Ram[4].Max)
	{
		k=Ram[4].Size-Ram[4].Max;
		sprintf(buff, "Insufficient bit addressable memory.  "
					"%d byte%s short.\n", k, (k==1)?"":"s");
		REPORT_ERROR(buff, 1);
	}

	if(Ram[5].Size!=0)
	{
		sprintf(buff, "%ld bytes in DRAM wasted.  "
		            "SDCC link could use: --data-loc 0x%02lx\n",
					Ram[5].Size, Ram[6].Start-Ram[5].Size);
		REPORT_WARNING(buff, 1);
	}

	if((Ram[6].Start+Ram[6].Size)>Ram[6].Max)
	{
		k=(Ram[6].Start+Ram[6].Size)-Ram[6].Max;
		sprintf(buff, "Insufficient DRAM memory.  "
					"%d byte%s short.\n", k, (k==1)?"":"s");
		REPORT_ERROR(buff, 1);
	}

	/*Report the position of the begining of the stack*/
	fprintf(of, "\nStack starts at: 0x%02lx", Stack.Start);

	/*Check that the stack pointer is landing in a safe place:*/
	// if (!flat24Mode) TODO: bypass stack check for ds390
	{
		if( (dram[Stack.Start] & 0x8000) == 0x8000 )
		{
			fprintf(of, ".\n");
			sprintf(buff, "Stack set to unavailable memory.\n");
			REPORT_ERROR(buff, 1);
		}
		else if(dram[Stack.Start+1])
		{
			fprintf(of, ".\n");
			sprintf(buff, "Stack overlaps area ");
			REPORT_ERROR(buff, 1);
			for(j=0; j<7; j++)
			{
        	                if(dram[Stack.Start+1]&Ram[j].flag)
				{
					sprintf(buff, "'%s'\n", Ram[j].Name);
					break;
				}
			}
			if(dram[Stack.Start]&IRam.flag)
			{
				sprintf(buff, "'%s'\n", IRam.Name);
			}
			REPORT_ERROR(buff, 0);
		}
		else
		{
			for(j=Stack.Start, k=0; (j<(int)iram_size)&&(dram[j]==0); j++, k++);
			fprintf(of, " with %d bytes available\n", k);
			if (k<MIN_STACK)
			{
				sprintf(buff, "Only %d byte%s available for stack.\n",
					k, (k==1)?"":"s");
				REPORT_WARNING(buff, 1);
			}
		}
	}

	fprintf(of, "\nOther memory:\n");
	fprintf(of, format, "Name", "Start", "End", "Size", "Max");
	fprintf(of, format, line, line, line, line, line);

	/*Report IRam totals:*/
	sprintf(start, "0x%02lx", IRam.Start);
	if(IRam.Size==0)
		end[0]=0;/*Empty string*/
	else
		sprintf(end,  "0x%02lx", IRam.Size+IRam.Start-1);
	sprintf(size, "%5lu", IRam.Size);
	sprintf(max, "%5lu", IRam.Max);
	fprintf(of, format, IRam.Name, start, end, size, max);

	/*Report XRam totals:*/
	sprintf(start, "0x%04lx", XRam.Start);
	if(XRam.Size==0)
		end[0]=0;/*Empty string*/
	else
		sprintf(end,  "0x%04lx", XRam.Size+XRam.Start-1);
	sprintf(size, "%5lu", XRam.Size);
	sprintf(max, "%5lu", XRam.Max);
	fprintf(of, format, XRam.Name, start, end, size, max);

	/*Report Rom/Flash totals:*/
	sprintf(start, "0x%04lx", Rom.Start);
	if(Rom.Size==0)
		end[0]=0;/*Empty string*/
	else
		sprintf(end,  "0x%04lx", Rom.Size+Rom.Start-1);
	sprintf(size, "%5lu", Rom.Size);
	sprintf(max, "%5lu", Rom.Max);
	fprintf(of, format, Rom.Name, start, end, size, max);

	/*Report any excess:*/
	if((IRam.Start+IRam.Size)>(IRam.Max+0x80))
	{
		sprintf(buff, "Insufficient INDIRECT RAM memory.\n");
		REPORT_ERROR(buff, 1);
	}
	if((XRam.Start+XRam.Size)>XRam.Max)
	{
		sprintf(buff, "Insufficient EXTERNAL RAM memory.\n");
		REPORT_ERROR(buff, 1);
	}
	if((Rom.Start+Rom.Size)>Rom.Max)
	{
		sprintf(buff, "Insufficient ROM/EPROM/FLASH memory.\n");
		REPORT_ERROR(buff, 1);
	}

	fclose(of);
	return toreturn;		
}
