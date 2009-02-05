/* lks19.c

   Copyright (C) 1989-1995 Alan R. Baldwin
   721 Berkeley St., Kent, Ohio 44240

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdio.h>
#include <string.h>
#include "aslink.h"

/*)Module       lks19.c
 *
 *      The module lks19.c contains the function to
 *      output the relocated object code in the
 *      Motorola S19 format.
 *
 *      lks19.c contains the following function:
 *              VOID    s19(i)
 *
 *      lks19.c contains no local variables.
 */

/*)S19 Format
 *      Record Type Field    -  This  field  signifies  the  start  of a
 *                              record and  identifies  the  the  record
 *                              type as follows:
 *
 *                                  Ascii S1 - Data Record
 *                                  Ascii S9 - End of File Record
 *
 *      Record Length Field  -  This  field  specifies the record length
 *                              which includes the  address,  data,  and
 *                              checksum   fields.   The  8  bit  record
 *                              length value is converted to  two  ascii
 *                              characters, high digit first.
 *
 *      Load Address Field   -  This  field  consists  of the four ascii
 *                              characters which result from  converting
 *                              the  the  binary value of the address in
 *                              which to begin loading this record.  The
 *                              order is as follows:
 *
 *                                  High digit of high byte of address.
 *                                  Low digit of high byte of address.
 *                                  High digit of low byte of address.
 *                                  Low digit of low byte of address.
 *
 *                              In an End of File record this field con-
 *                              sists of either four ascii zeros or  the
 *                              program  entry  address.   Currently the
 *                              entry address option is not supported.
 *
 *      Data Field           -  This  field consists of the actual data,
 *                              converted to two ascii characters,  high
 *                              digit first.  There are no data bytes in
 *                              the End of File record.
 *
 *      Checksum Field       -  The  checksum  field is the 8 bit binary
 *                              sum of the record length field, the load
 *                              address field, and the data field.  This
 *                              sum is then  complemented  (1's  comple-
 *                              ment)   and   converted   to  two  ascii
 *                              characters, high digit first.
 */

/*)Function     s19(i)
 *
 *              int     i               0 - process data
 *                                      1 - end of data
 *
 *      The function s19() outputs the relocated data
 *      in the standard Motorola S19 format.
 *
 *      local variables:
 *              Addr_T  chksum          byte checksum
 *
 *      global variables:
 *              int     hilo            byte order
 *              FILE *  ofp             output file handle
 *              int     rtcnt           count of data words
 *              int     rtflg[]         output the data flag
 *              Addr_T  rtval[]         relocated data
 *
 *      functions called:
 *              int     fprintf()       c_library
 *
 *      side effects:
 *              The data is output to the file defined by ofp.
 */

VOID
s19(i)
{
        register Addr_T chksum;

        if (i) {
                if (ap->a_flag & A_NOLOAD)
                        return;
                if (hilo == 0) {
                        chksum = rtval[0];
                        rtval[0] = rtval[1];
                        rtval[1] = chksum;
                }
                for (i = 0, chksum = 1; i < rtcnt; i++) {
                        if (rtflg[i])
                                chksum++;
                }
                fprintf(ofp, "S1%02X", chksum);
                for (i = 0; i < rtcnt ; i++) {
                        if (rtflg[i]) {
                                fprintf(ofp, "%02X", rtval[i]);
                                chksum += rtval[i];
                        }
                }
                fprintf(ofp, "%02X\n", (0-chksum-1) & 0xff);
        } else {
                fprintf(ofp, "S9030000FC\n");
        }
}
