/*-------------------------------------------------------------------------
   errno.h: Error codes used in the math functions

    Ported to PIC16 port by Vangelis Rokas, 2004 (vrokas@otenet.gr)
    
    Copyright (C) 2001  Jesus Calvino-Fraga, jesusc@ieee.org 

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
-------------------------------------------------------------------------*/

/*
** $Id$
*/

#ifndef _PIC16_ERRNO_H
#define _PIC16_ERRNO_H

extern int errno;

/* Error Codes: */

#define EDOM        33  /* Math argument out of domain of functions */
#define ERANGE      34  /* Math result not representable */

#endif  /* _PIC16_ERRNO_H */
