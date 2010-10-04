/*
 * pic18f65j50.h - device specific declarations
 */

#ifndef __PIC18F65J50_H__
#define __PIC18F65J50_H__ 1

#include "pic18f67j50.h"

// Configuration Bits
#undef __CONFIG1L
#undef __CONFIG1H
#undef __CONFIG2L
#undef __CONFIG2H
#undef __CONFIG3L
#undef __CONFIG3H

#define	__CONFIG1L          	0x7FF8
#define	__CONFIG1H          	0x7FF9
#define	__CONFIG2L          	0x7FFA
#define	__CONFIG2H          	0x7FFB
#define	__CONFIG3L          	0x7FFC
#define	__CONFIG3H          	0x7FFD

#endif

