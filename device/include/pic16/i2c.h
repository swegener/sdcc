
/*
 * I2C communications module library header
 *
 * written by Vangelis Rokas, 2005 <vrokas AT otenet.gr>
 *
 * Devices implemented:
 *	PIC18F[24][45][28]
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/*
** $Id$
*/


#ifndef __I2C_H__
#define __I2C_H__

/* link the I/O library */
#pragma library io

#include <pic18fregs.h>


#define _I2CPARAM_SPEC	__data


/* I2C modes of operation */
#define I2C_SLAVE10B_INT	0x0f
#define I2C_SLAVE7B_INT		0x0e
#define I2C_SLAVE_IDLE		0x0b
#define I2C_MASTER		0x08
#define I2C_SLAVE10B		0x07
#define I2C_SLAVE7B		0x06


/* slew rate control */
#define I2C_SLEW_OFF	0xc0
#define I2C_SLEW_ON	0x00

/* macros to generate hardware conditions on I2C module */

/* generate stop condition */
#define I2C_STOP()	SSPCON2bits.PEN=1

/* generate start condition */
#define I2C_START()	SSPCON2bits.SEN=1

/* generate restart condition */
#define I2C_RESTART()	SSPCON2bits.RSEN=1

/* generate not acknoledge condition */
#define I2C_NACK()	SSPCON2bits.ACKDT=1; SSPCON2bits.ACKEN=1

/* generate acknoledge condition */
#define I2C_ACK()	SSPCON2bits.ACKDT=0; SSPCON2bits.ACKEN=1

/* wait until I2C is idle */
#define I2C_IDLE()	while((SSPCON2 & 0x1f) | (SSPSTATbits.R_W));

/* is data ready from I2C module ?? */
#define I2C_DRDY()	(SSPSTATbits.BF)


/* function equivalent to macros for generating hardware conditions */

/* stop */
void i2c_stop(void);

/* start */
void i2c_start(void);

/* restart */
void i2c_restart(void);

/* not acknoledge */
void i2c_nack(void);

/* acknoledge */
void i2c_ack(void);

/* wait until I2C goes idle */
void i2c_idle(void);

/* is character ready in I2C buffer ?? */
unsigned char i2c_drdy(void);

/* read a character from I2C module */
unsigned char i2c_readchar(void);

/* read a string from I2C module */
char i2c_readstr(_I2CPARAM_SPEC unsigned char *ptr, unsigned char len);

/* write a character to I2C module */
char i2c_writechar(unsigned char dat);

/* write a string to I2C module */
char i2c_writestr(unsigned char *ptr);

/* configure I2C port for operation */
void i2c_open(unsigned char mode, unsigned char slew, unsigned char addr_brd);

void i2c_close(void);


#endif	/* __I2C_H__ */
