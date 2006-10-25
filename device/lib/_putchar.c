/* Default polling putchar() using to the serial port

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

extern bit uart_init_flag;
void inituart(unsigned char t1_reload);

void putchar (char c)
{
	if(!uart_init_flag) inituart(0xff);
	if (c=='\n')
	{
		while (!TI);
		TI=0;
		SBUF='\r';
	}
	while (!TI);
	TI=0;
	SBUF=c;
}

#endif
