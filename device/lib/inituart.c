/* Uart initialization for putchar() and getchar()

   Written By -  Jesus Calvino-Fraga (October/2006)

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
*/

#ifdef SDCC_mcs51
#include <8051.h>

void inituart (unsigned char t1_reload)
{
	TR1=0;
	TMOD=(TMOD&0x0f)|0x20;
	PCON|=0x80;
	TH1=TL1=t1_reload;
	TR1=1;
	SCON=0x52;
}

#endif
