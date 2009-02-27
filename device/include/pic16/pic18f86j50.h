/*
 * pic18f86j50.h - device specific declarations
 */

#ifndef __PIC18F86J50_H__
#define __PIC18F86J50_H__ 1

#include "pic18f87j50.h"

#undef __CONFIG1L
#undef __CONFIG1H
#undef __CONFIG2L
#undef __CONFIG2H
#undef __CONFIG3L
#undef __CONFIG3H

// Configuration Bits
#define	__CONFIG1L          	0xFFF8
#define	__CONFIG1H          	0xFFF9
#define	__CONFIG2L          	0xFFFA
#define	__CONFIG2H          	0xFFFB
#define	__CONFIG3L          	0xFFFC
#define	__CONFIG3H          	0xFFFD

#endif

