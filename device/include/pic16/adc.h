
/*
 * A/D conversion module library header
 *
 * written by Vangelis Rokas, 2004 <vrokas AT otenet.gr>
 *
 * Devices implemented:
 *	PIC18F[24][45][28]
 *	PIC18F2455-style
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
 *
 * $Id$
 */

#ifndef __ADC_H__
#define __ADC_H__

/* link I/O libarary */
#pragma library io

/* interrupt on/off flag */
#define ADC_INT_OFF	0x00
#define ADC_INT_ON	0x01


/* output format */
#define ADC_FRM_RJUST	0x80
#define ADC_FRM_LJUST	0x00


/* oscillator frequency */
#define ADC_FOSC_2	0x00
#define ADC_FOSC_4	0x04
#define ADC_FOSC_8	0x01
#define ADC_FOSC_16	0x05
#define ADC_FOSC_32	0x02
#define ADC_FOSC_64	0x06
#define ADC_FOSC_RC	0x07


/* distinguish between 18f242-style and 18f2455-style ADC */

/* ordered by device family */
#if    defined(pic18f1220) || defined(pic18f1320) \
    || defined(pic18f2220) || defined(pic18f2320) || defined(pic18f4220) || defined(pic18f4320) \
    || defined(pic18f2221) || defined(pic18f2321) || defined(pic18f4221) || defined(pic18f4321) \
    || defined(pic18f2420) || defined(pic18f2520) || defined(pic18f4420) || defined(pic18f4520) \
    || defined(pic18f2423) || defined(pic18f2523) || defined(pic18f4423) || defined(pic18f4523) \
    || defined(pic18f2450) || defined(pic18f4450) \
    || defined(pic18f2455) || defined(pic18f2550) || defined(pic18f4455) || defined(pic18f4550) \
    || defined(pic18f2480) || defined(pic18f2580) || defined(pic18f4480) || defined(pic18f4580) \
    || defined(pic18f24j10) || defined(pic18f25j10) || defined(pic18f44j10) || defined(pic18f45j10) \
    || defined(pic18f2525) || defined(pic18f2620) || defined(pic18f4525) || defined(pic18f4620) \
    || defined(pic18f2585) || defined(pic18f2680) || defined(pic18f4585) || defined(pic18f4680) \
    || defined(pic18f2682) || defined(pic18f2685) || defined(pic18f4682) || defined(pic18f4685) \
    || defined(pic18f6520) || defined(pic18f6620) || defined(pic18f6720) \
    || defined(pic18f8520) || defined(pic18f8620) || defined(pic18f8720) \
    || defined(pic18f6585) || defined(pic18f6680) || defined(pic18f8585) || defined(pic18f8680) \

#define __SDCC_ADC_STYLE2455    1

// 97j60 family
#elif  defined(pic18f66j60) || defined(pic18f66j65) || defined(pic18f67j60) \
    || defined(pic18f86j60) || defined(pic18f86j65) || defined(pic18f87j60) \
    || defined(pic18f96j60) || defined(pic18f96j65) || defined(pic18f97j60) \

#define __SDCC_ADC_STYLE97J60   1

// small ADC device?
#elif  defined(pic18f242) || defined(pic18f252) || defined(pic18f442) || defined(pic18f452) \
    || defined(pic18f248) || defined(pic18f258) || defined(pic18f448) || defined(pic18f458)

#define __SDCC_ADC_STYLE242     1

#else // unknown device

#error Device ADC style is unknown, please update your adc.h manually and/or inform the maintainer!

#endif  // !large ADC device


/* channel selection */
#if defined(__SDCC_ADC_STYLE2455) || defined(__SDCC_ADC_STYLE97J60)
#define ADC_CHN_0               0x00
#define ADC_CHN_1               0x01
#define ADC_CHN_2               0x02
#define ADC_CHN_3               0x03
#define ADC_CHN_4               0x04
#define ADC_CHN_5               0x05
#define ADC_CHN_6               0x06
#define ADC_CHN_7               0x07
#define ADC_CHN_8               0x08
#define ADC_CHN_9               0x09
#define ADC_CHN_10              0x0a
#define ADC_CHN_11              0x0b
#define ADC_CHN_12              0x0c
#if defined(__SDCC_ADC_STYLE97J60) // 97j60 family has 2 more ADC ports
#define ADC_CHN_13              0x0d
#define ADC_CHN_14              0x0e
#define ADC_CHN_15              0x0f
#endif

#else   /* all other devices */

#define ADC_CHN_1		0x00
#define ADC_CHN_2		0x01
#define ADC_CHN_3		0x03
#define ADC_CHN_4		0x04
#define ADC_CHN_5		0x05
#define ADC_CHN_6		0x06
#define ADC_CHN_7		0x07

#endif  // ADC_STYLE


/* reference and pin configuration */
#if defined(__SDCC_ADC_STYLE2455) || defined(__SDCC_ADC_STYLE97J60)

// 97j60 family has 2 more possible ADC configs
#if defined(__SDCC_ADC_STYLE97J60) // 97j60 family has 2 more ADC ports
#define ADC_CFG_16A_0R  0x00
#define ADC_CFG_16A_1R  0x10
#define ADC_CFG_16A_2R  0x30
#define ADC_CFG_15A_0R  0x00	// can switch only from 14 analog ports to 16 enabled analog ports
#define ADC_CFG_15A_1R  0x10
#define ADC_CFG_15A_2R  0x30
#define ADC_CFG_14A_0R  0x01
#define ADC_CFG_14A_1R  0x11
#define ADC_CFG_14A_2R  0x31
#define ADC_CFG_13A_0R  0x02
#define ADC_CFG_13A_1R  0x12
#define ADC_CFG_13A_2R  0x32
#else
#define ADC_CFG_13A_0R  0x01
#define ADC_CFG_13A_1R  0x11
#define ADC_CFG_13A_2R  0x31
#endif
#define ADC_CFG_12A_0R  0x03
#define ADC_CFG_12A_1R  0x13
#define ADC_CFG_12A_2R  0x33
#define ADC_CFG_11A_0R  0x04
#define ADC_CFG_11A_1R  0x14
#define ADC_CFG_11A_2R  0x34
#define ADC_CFG_10A_0R  0x05
#define ADC_CFG_10A_1R  0x15
#define ADC_CFG_10A_2R  0x35
#define ADC_CFG_09A_0R  0x06
#define ADC_CFG_09A_1R  0x16
#define ADC_CFG_09A_2R  0x36
#define ADC_CFG_08A_0R  0x07
#define ADC_CFG_08A_1R  0x17
#define ADC_CFG_08A_2R  0x37
#define ADC_CFG_07A_0R  0x08
#define ADC_CFG_07A_1R  0x18
#define ADC_CFG_07A_2R  0x38
#define ADC_CFG_06A_0R  0x09
#define ADC_CFG_06A_1R  0x19
#define ADC_CFG_06A_2R  0x39
#define ADC_CFG_05A_0R  0x0a
#define ADC_CFG_05A_1R  0x1a
#define ADC_CFG_05A_2R  0x3a
#define ADC_CFG_04A_0R  0x0b
#define ADC_CFG_04A_1R  0x1b
#define ADC_CFG_04A_2R  0x3b
#define ADC_CFG_03A_0R  0x0c
#define ADC_CFG_03A_1R  0x1c
#define ADC_CFG_03A_2R  0x3c
#define ADC_CFG_02A_0R  0x0d
#define ADC_CFG_02A_1R  0x1d
#define ADC_CFG_02A_2R  0x3d
#define ADC_CFG_01A_0R  0x0e
#define ADC_CFG_01A_1R  0x1e
#define ADC_CFG_01A_2R  0x3e
#define ADC_CFG_00A_0R  0x0f

#else   /* all other devices */

#define ADC_CFG_8A_0R	0x00
#define ADC_CFG_7A_1R	0x01
#define ADC_CFG_5A_0R	0x02
#define ADC_CFG_4A_1R	0x03
#define ADC_CFG_3A_0R	0x04
#define ADC_CFG_2A_1R	0x05
#define ADC_CFG_0A_0R	0x06
#define ADC_CFG_6A_2R	0x08
#define ADC_CFG_6A_0R	0x09
#define ADC_CFG_5A_1R	0x0a
#define ADC_CFG_4A_2R	0x0b
#define ADC_CFG_3A_2R	0x0c
#define ADC_CFG_2A_2R	0x0d
#define ADC_CFG_1A_0R	0x0e
#define ADC_CFG_1A_2R	0x0f

#endif // ADC_STYLE

/* initialize AD module */
void adc_open(unsigned char channel, unsigned char fosc, unsigned char pcfg, unsigned char config);

/* shutdown AD module */
void adc_close(void);

/* begin a conversion */
void adc_conv(void);

/* return 1 if AD is performing a conversion, 0 if done */
char adc_busy(void) __naked;

/* get value of conversion */
int adc_read(void) __naked;

/* setup conversion channel */
void adc_setchannel(unsigned char channel) __naked;

#endif

