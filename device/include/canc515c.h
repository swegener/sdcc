/*****************************************************************************
|*
|*  MODULE:            canc515c.h
|*
|*    This file contains definitions for the builtin CAN-Bus Controller of
|*    the Siemens c515c controller
|*
****************************************************************************/

#ifndef _CANC515C_H
#define _CANC515C_H

/* define CPU_CLK_10MHZ or CPU_CLK_8MHZ to select the right values for */
/* the bit timing registers */

#define CPU_CLK_10MHZ

/* address of can controller in xmem */
#define CAN_CTRL	0xf700

/* size of message buffer including 1 dummy byte at end */
#define CAN_MSG_SZ	0x10

/* register offset definitions */
#define CR	0
#define SR	1
#define IR	2

#define BT_0	4
#define BT_1	5
#define GMS_0	6
#define GMS_1	7
#define GME_0	8
#define GME_1	9
#define GME_2	0xa
#define GME_3	0xb
#define MSG15MSK_0	0xc
#define MSG15MSK_1	0xd
#define MSG15MSK_2	0xe
#define MSG15MSK_3	0xf

/* register offsets  in message buffer */
#define MCR_0	0
#define MCR_1	1
#define ARB_0	2
#define ARB_1	3
#define ARB_2	4
#define ARB_3	5
#define MCFG	6
/* beginning of message data */
#define DATA	7

/* bits in cntr_x registers */
#define MSGVAL	0x80
#define TXIE	0x20
#define RXIE	0x8
#define INTPND	0x2
#define RMTPND	0x80
#define TXRQST	0x20
#define MSGLST	0x8
#define CPUUPD	0x8
#define NEWDAT	0x2

/* macros for setting and resetting above bits, see Siemens documentation */
#define MCR_BIT_SET(p,x) ((p) = (0xff & ~((x) >> 1)))
#define MCR_BIT_RES(p,x) ((p) = (0xff & ~(x)))

/* direction = transmit in mcfg */
#define DIR_TRANSMIT	0x8

/* constants for bit timing registers */
/* 8 MHZ */
#ifdef CPU_CLK_8MHZ
#define BT_0_125K	0x3
#define BT_1_125K	0x1c
#define BT_0_250K	0x1
#define BT_1_250K	0x1c
#define BT_0_500K	0x0
#define BT_1_500K	0x1c
#define BT_0_1M		0x0
#define BT_1_1M		0x14
#endif
/* dito, 10 MHZ */
#ifdef CPU_CLK_10MHZ
#define BT_0_125K	0x3
#define BT_1_125K	0x1c
#define BT_0_250K	0x1
#define BT_1_250K	0x1c
#define BT_0_500K	0x0
#define BT_1_500K	0x2f
#define BT_0_1M		0x0
#define BT_1_1M		0x25
#endif

/* Control register bits */

#define	CINIT	0x1
#define IE	0x2
#define SIE	0x4
#define EIE	0x8

#define CCE	0x40

/* status register bits */
#define	LEC0	0x1
#define	LEC1	0x2
#define	LEC2	0x4
#define	TXOK	0x8
#define	RXOK	0x10
#define	WAKE	0x20
#define	WARN	0x40
#define	BOFF	0x80


typedef struct can_msg
{
    unsigned char mcr_0;
    unsigned char mcr_1;
    unsigned char arb_0;
    unsigned char arb_1;
    unsigned char arb_2;
    unsigned char arb_3;
    unsigned char mcfg;
    unsigned char data_bytes[8];
    unsigned char dummy;
} *can_msgp;

xdata at CAN_CTRL struct
{
    unsigned	char	cr;
    unsigned	char	sr;
    unsigned	char	ir;
    unsigned	char	res0;
    unsigned	char	bt_0;
    unsigned	char	bt_1;
    unsigned	char	gms_0;
    unsigned	char	gme_1;
    unsigned	char	gme_0;
    unsigned	char	gme_1;
    unsigned	char	gme_2;
    unsigned	char	gme_3;
    unsigned	char	msg15msk_0;
    unsigned	char	msg15msk_1;
    unsigned	char	msg15msk_2;
    unsigned	char	msg15msk_3;
    struct	can_msg msgbufs[15];
} can_ctrl;

#endif /* _CANC515C */
