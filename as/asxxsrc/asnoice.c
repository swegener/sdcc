/* asnoice.c - Extensions to CUG 292 assembler ASxxxx to produce NoICE debug files

   Copyright (C) 1989-1995 Alan R. Baldwin
   721 Berkeley St., Kent, Ohio 44240

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/*
 * Extensions to CUG 292 assembler ASxxxx to produce NoICE debug files
 *
 * 3-Nov-1997 by John Hartman
 */

#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <ctype.h>
#include "asm.h"

/* Return basic file name without path or extension.
   If spacesToUnderscores != 0 then spaces are converted to underscores */

char* BaseFileName( int fileNumber, int spacesToUnderscores )
{
        static int prevFile = -1;
        static char baseName[ PATH_MAX ];

        char *p1, *p2;

        if (fileNumber != prevFile)
        {
                prevFile = fileNumber;

                p1 = srcfn[prevFile];

                /* issue a FILE command with full path and extension */
                fprintf( ofp, ";!FILE %s\n", p1 );

                /* Name starts after any colon or backslash (DOS) */
                p2 = strrchr( p1, '\\' );
                if (p2 == NULL) p2 = strrchr( p1, '/' );
                if (p2 == NULL) p2 = strrchr( p1, ':' );
                if (p2 == NULL) p2 = p1-1;
                strcpy( baseName, p2+1 );

                /* Name ends at any separator */
                p2 = strrchr( baseName, FSEPX );
                if (p2 != NULL) *p2 = 0;
                /* SD comment this out since not a ANSI Function */
                /* strupr( baseName ); */

                if (spacesToUnderscores)
                {
                  /* Convert spaces to underscores */
                  for (p1 = baseName; *p1; ++p1)
                    if (isspace(*p1))
                      *p1 = '_';
                }
        }
        return baseName;
}

/* Define a symbol for current location:  FILE.line# */
void DefineNoICE_Line()
{
        char name[ NCPS ];
        struct sym *pSym;

        /* symbol is FILE.nnn */
        sprintf( name, "%s.%u", BaseFileName( cfile, 0 ), srcline[ cfile ] );

        pSym = lookup( name );
        pSym->s_type = S_USER;
        pSym->s_area = dot.s_area;
        pSym->s_addr = laddr;
        pSym->s_flag |= S_GBL;
}

/* Define a symbol for current location:  A$FILE$line# */
void DefineCDB_Line()
{
        char name[ NCPS ];
        struct sym *pSym;

        /* symbol is FILE.nnn */
        sprintf( name, "A$%s$%u", BaseFileName( cfile, 1 ), srcline[ cfile ] );

        pSym = lookup( name );
        pSym->s_type = S_USER;
        pSym->s_area = dot.s_area;
        pSym->s_addr = laddr;
        pSym->s_flag |= S_GBL;
}

#if 0
OLD VERSION
/* Define a symbol for current location:  FILE.line# */
void DefineNoICE_Line()
{
        static int prevFile = -1;
        static struct area *pPrevArea = NULL;
        static char baseName[ PATH_MAX ];

        int j;
        char *p1, *p2;

        /* Get outfilename without extension for use as base symbol name */
        if (baseName[0] == 0)
        {
                p1 = srcfn[0];
                p2 = baseName;
                while ((*p1 != 0) && (*p1 != FSEPX))
                {
                        *p2++ = *p1++;
                }
                *p2 = 0;
                /* SD Commented this out since it is not a
                   ASNI Function */
                /* strupr( baseName ); */
        }

        if ((cfile != prevFile) || (dot.s_area != pPrevArea))
        {
                prevFile = cfile;
                pPrevArea = dot.s_area;

                /* file or area change: issue FILE command with base @ */
                fprintf( ofp, ";!FILE %s %s_%s\n", srcfn[ cfile ],
                         baseName,
                         dot.s_area->a_id );
        }

        fprintf( ofp, ";!LINE %u. 0x%X\n", srcline[ cfile ], laddr );
}

#endif
