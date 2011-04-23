/* lknoice.c */

/*
 *  Copyright (C) 1989-2009  Alan R. Baldwin
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 *
 *
 * Extensions to produce NoICE debug files
 *
 * 31-Oct-1997 by John Hartman
 * 30-Jan-98 JLH add page to DefineNoICE for 8051
 *  2-Feb-98 JLH Allow optional .nest on local vars - C scoping rules...
 */

#include "aslink.h"

static void DefineGlobal( char *name, a_uint value, int page );
static void DefineScoped( char *name, a_uint value, int page );
static void DefineFile( char *name, a_uint value, int page );
static void DefineFunction( char *name, a_uint value, int page );
static void DefineStaticFunction( char *name, a_uint value, int page );
static void DefineEndFunction( a_uint value, int page );
static void DefineLine( char *lineString, a_uint value, int page );
static void PagedAddress( a_uint value, int page );

/*)Function	VOID	NoICEfopen()
 * 
 *	The function NoICEfile() opens the NoICE output file
 *	and sets the map flag, mflag, to create a map file.
 *	NoICE processing is performed during map generation.
 *
 *	local variables:
 *		none
 *
 *	global variables:
 *		int	jflag		NoICE Debug flag
 *		FILE *	jfp		NoICE Debug File handle
 *		struct lfile *linkp	Pointer to the Linker output file name
 *		int	mflag		Map output flag
 *
 *	functions called:
 *		FILE *	afile()		lkmain.c
 *		VOID	lkexit()	lkmain.c
 *
 *	side effects:
 *		The NoICE output file is opened.
 *		Failure to open the file will
 *		terminate the linker.
 */

VOID NoICEfopen(void)
{
	if (jflag) {
		jfp = afile(linkp->f_idp, "noi", 1);
		if (jfp == NULL) {
			lkexit(1);
		}
		mflag = 1;
	}
}

/*
 * Called from lstarea in lklist.c for each symbol.
 *
 * Generates appropriate NoICE commands into output file, if any is open
 *
 */

void DefineNoICE( char *name, a_uint value, int page )
{
        char token1[NCPS];                      /* parse for file.function.symbol */
        char token2[NCPS];
        char token3[NCPS];
        char sep1, sep2;
        int  j, level;

        /* no output if file is not open */
        if (jfp == NULL) return;

        j = sscanf( name, "%[^.]%c%[^.]%c%s", token1, &sep1, token2, &sep2, token3 );
        switch (j)
        {
                /* file.function.symbol, or file.function..SPECIAL */
                case 5:
                        DefineFile( token1, 0, 0 );
                        if (token3[0] == '.')
                        {
                                if (symeq( token3, ".FN", 1 ) != 0)
                                {
                                        /* Global function */
                                        DefineFunction( token2, value, page );
                                }
                                else if (symeq( token3, ".SFN", 1 ) != 0)
                                {
                                        /* Static (file-scope) function */
                                        DefineStaticFunction( token2, value, page );
                                }
                                else if (symeq( token3, ".EFN", 1 ) != 0)
                                {
                                        /* End of function */
                                        DefineEndFunction( value, page );
                                }
                        }
                        else
                        {
                                /* Function-scope var. */
                                DefineFunction( token2, 0, 0 );

                                /* Look for optional level integer */
                                j = sscanf( token3, "%[^.]%c%u", token1, &sep1, &level );
                                if ((j == 3) && (level != 0))
                                {
                                        sprintf( &token1[ strlen(token1) ], "_%u", level );
                                }
                                DefineScoped( token1, value, page );
                        }
                        break;

                /* either file.symbol or file.line# */
                case 3:
                        DefineFile( token1, 0, 0 );
                        if ((token2[0] >= '0') && (token2[0] <= '9'))
                        {
                                /* Line number */
                                DefineLine( token2, value, page );
                        }
                        else
                        {
                                /* File-scope symbol.  (Kill any function) */
                                DefineEndFunction( 0, 0 );
                                DefineScoped( token2, value, page );
                        }
                        break;

                /* NoICE file.func. is illegal */
                case 4:
                        break;

                /* NoICE symbol. is illegal */
                case 2:
                        break;

                /* just a symbol */
                case 1:
                        DefineGlobal( token1, value, page );
                        break;
        }
}

static char currentFile[NCPS];
static char currentFunction[NCPS];

/*
 * static function:
 * Define "name" as a global symbol
 */

void DefineGlobal( char *name, a_uint value, int page )
{
        fprintf( jfp, "DEF %s ", name );
        PagedAddress( value, page );
}

/*
 * static function:
 * Define "name" as a static (scoped) symbol
 */

void DefineScoped( char *name, a_uint value, int page )
{
        fprintf( jfp, "DEFS %s ", name );
        PagedAddress( value, page );
}

/*
 * static function:
 * Define "name" as the current file
 */

void DefineFile( char *name, a_uint value, int page )
{
        if (symeq( name, currentFile, 1 ) == 0)
        {
                strcpy( currentFile, name );
                if (value != 0)
                {
                        fprintf( jfp, "FILE %s ", name );
                        PagedAddress( value, page );
                }
                else
                {
                        fprintf( jfp, "FILE %s\n", name );
                }
        }
}

/*
 * static function:
 * Define "name" as the current function
 */

void DefineFunction( char *name, a_uint value, int page )
{
        if (symeq( name, currentFunction, 1 ) == 0)
        {
                strcpy( currentFunction, name );
                if (value != 0)
                {
                        fprintf( jfp, "DEF %s ", name );
                        PagedAddress( value, page );
                        fprintf( jfp, "FUNC %s ", name );
                        PagedAddress( value, page );
                }
                else
                {
                        fprintf( jfp, "FUNC %s\n", name );
                }
        }
}

/*
 * static function:
 * Define "name" as the current static (scoped) function
 */

void DefineStaticFunction( char *name, a_uint value, int page )
{
        if (symeq( name, currentFunction, 1 ) == 0)
        {
                strcpy( currentFunction, name );
                if (value != 0)
                {
                        fprintf( jfp, "DEFS %s ", name );
                        PagedAddress( value, page );
                        fprintf( jfp, "SFUNC %s ", name );
                        PagedAddress( value, page );
                }
                else
                {
                        fprintf( jfp, "SFUNC %s\n", name );
                }
        }
}

/*
 * static function:
 * Define the end of the current function
 */

void DefineEndFunction( a_uint value, int page )
{
        if (currentFunction[0] != 0)
        {
                if (value != 0)
                {
                        fprintf( jfp, "ENDF " );
                        PagedAddress( value, page );
                }
                else
                {
                        fprintf( jfp, "ENDF\n" );
                }

                currentFunction[0] = 0;
        }
}

/*
 * static function:
 * Define "lineNumber" as a line in the current file
 */

void DefineLine( char *lineString, a_uint value, int page )
{
        int indigit, lineNumber;

        lineNumber = 0;
        while( (indigit=digit( *lineString++, 10 )) >= 0)
        {
                lineNumber = 10*lineNumber + indigit;
        }
        fprintf( jfp, "LINE %u ", lineNumber );
        PagedAddress( value, page );
}

/*
 */

void PagedAddress( a_uint value, int page )
{
        fprintf( jfp, "%X:0x%X\n", page, value );
}
