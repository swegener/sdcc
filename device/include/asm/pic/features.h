/*
 * PIC14 port features.
 *
 * This file is part of the GNU PIC Library.
 *
 * June, 2006: adopted for pic14
 * 	Raphael Neider <rneider at web.de>
 * 
 * January, 2004
 * The GNU PIC Library is maintained by,
 * 	Vangelis Rokas <vrokas@otenet.gr>
 *
 * $Id$
 *
 */
#ifndef __PIC14_ASM_FEATURES_H
#define __PIC14_ASM_FEATURES_H   1

#define _REENTRANT

#define _IL_REENTRANT
#define _FS_REENTRANT
#define _MATH_REENTRANT

#define _CODE	__code
#define _DATA	__data
#define _AUTOMEM
#define _STATMEM

#endif	/* __PIC14_ASM_FEATURES_H */
