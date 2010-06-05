/*-------------------------------------------------------------------------
   usart.h - USART communications module library header

   Copyright (C) 2005, Vangelis Rokas <vrokas AT otenet.gr>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2.1, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#ifndef __USART_H__
#define __USART_H__

#pragma library io


#define RAM_SCLS	__data

/* configuration bit masks for open function */
#define USART_TX_INT_ON   0xff
#define USART_TX_INT_OFF  0x7f
#define USART_RX_INT_ON   0xff
#define USART_RX_INT_OFF  0xbf
#define USART_BRGH_HIGH   0xff
#define USART_BRGH_LOW    0xef
#define USART_CONT_RX     0xff
#define USART_SINGLE_RX   0xf7
#define USART_SYNC_MASTER 0xff
#define USART_SYNC_SLAVE  0xfb
#define USART_NINE_BIT    0xff
#define USART_EIGHT_BIT   0xfd
#define USART_SYNCH_MODE  0xff
#define USART_ASYNCH_MODE 0xfe

/* status bits */
union USART
{
  unsigned char val;
  struct
  {
    unsigned RX_NINE:1;
    unsigned TX_NINE:1;
    unsigned FRAME_ERROR:1;
    unsigned OVERRUN_ERROR:1;
    unsigned fill:4;
  };
};

void usart_open(unsigned char config, unsigned int spbrg) __wparam;
void usart_close(void);

unsigned char usart_busy(void) __naked;
unsigned char usart_drdy(void) __naked;

unsigned char usart_getc(void);
void usart_gets(RAM_SCLS char *buffer, unsigned char len);

void usart_putc(unsigned char data) __wparam __naked;
void usart_puts(char *data);


void usart_baud(unsigned char baudconfig) __wparam;

#endif
