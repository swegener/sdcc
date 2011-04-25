/* lkbank.c */

/*
 *  Copyright (C) 2001-2009  Alan R. Baldwin
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
 */

#include "aslink.h"

/*Module	lkbank.c
 *
 *	The module lkbank.c contains the function newbank() which
 *	creates a bank structure and the function module() which
 *	loads the module name into the current head structure.
 *
 *	lkbank.c contains the following functions:
 *		VOID	lkfopen()
 *		VOID	lkfclose()
 *
 *	lkbank.c contains no local variables.
 */

/*)Function	VOID	lkfopen()
 *
 *	The function lkfopen() will
 *	open output data files.
 *
 *	local variables:
 *		File *	fp		temporary file handle
 *		char *	frmt		temporary file type string
 *
 *
 *	global variables:
 *		int	a_bytes		T line address bytes
 *		char	afspec[]	Filespec from afile()
 *		FILE *	jfp		NoICE output file handle
 *		int	oflag		data output type flag
 *		FILE *	ofp		Output file handle
 *		FILE *	stderr		Standard Error Output handle
 *
 *	functions called:
 *		FILE *	afile()		lkmain.c
 *		int	fprintf()	c_library
 *		VOID	lkexit()	lkmain.c
 *
 *	side effects:
 *		All data output files are opened.
 */

VOID
lkfopen()
{
	char * frmt;
	FILE * fp;

	if (oflag == 0) return;

	fp = stderr;
	/*
	 * Open output file
	 */
	if (oflag == 1) {
		switch(a_bytes) {
		default:
		case 2: frmt = "ihx"; break;
		}
		fp = afile(linkp->f_idp, frmt, 1);
	} else
	if (oflag == 2) {
		switch(a_bytes) {
		default:
		case 2: frmt = "s19"; break;
		}
		fp = afile(linkp->f_idp, frmt, 1);
	} else
	if (oflag == 3) {
		switch(a_bytes) {
		default:
		case 2: frmt = "bin"; break;
		case 3: frmt = "bi3"; break;
		case 4: frmt = "bi4"; break;
		}
		fp = afile(linkp->f_idp, frmt, 2);
	}
	/* sdld specific */
	if (oflag == 4) {
		fp = afile(linkp->f_idp, "elf", 2);
	}
	/* end sdld specific */
	if (fp != stderr) {
		if (fp == NULL) {
			lkexit(ER_FATAL);
		}
#if NOICE
		/*
		 * Include NoICE command to load file
		 */
		if (jfp) {
			fprintf(jfp, "LOAD %s\n", afspec);
		}
#endif
		ofp = fp;
	}
}


/*)Function	VOID	lkfclose()
 *
 *	The function lkfclose() will
 *	close all open data output files.
 *
 *	local variables:
 *
 *	global variables:
 *		FILE *	ofp		Output file handle
 *		FILE *	stderr		Standard Error Output handle
 *
 *	functions called:
 *		int	fclose()	c_library
 *
 *	side effects:
 *		All open data output files are closed.
 */

VOID
lkfclose()
{
	if (ofp != NULL) {
		if (ofp != stderr) {
			fclose(ofp);
		}
		ofp = NULL;
	}
}
