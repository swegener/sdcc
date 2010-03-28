;--------------------------------------------------------------------------
;  shift.s
;
;  Copyright (C) 2001, Michael Hope
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2.1, or (at your option) any
;  later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License 
;  along with this library; see the file COPYING. If not, write to the
;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;  As a special exception, if you link this library with other files,
;  some of which are compiled with SDCC, to produce an executable,
;  this library does not by itself cause the resulting executable to
;  be covered by the GNU General Public License. This exception does
;  not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

        ;;
__rrulong_rrx_s::
        ld      hl,#2
        add     hl,sp

        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        inc     hl
        ld      c,(hl)
        inc     hl
        ld      b,(hl)
        inc     hl
        ld      a,(hl)

        ld      l,c
        ld      h,b
1$:
        or      a,a
        ret     Z

        rr      h
        rr      l
        rr      d
        rr      e

        dec     a
        jp      1$

__rrslong_rrx_s::
        ld      hl,#2
        add     hl,sp

        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        inc     hl
        ld      c,(hl)
        inc     hl
        ld      b,(hl)
        inc     hl
        ld      a,(hl)

        ld      l,c
        ld      h,b
1$:
        or      a,a
        ret     Z

        sra     h
        rr      l
        rr      d
        rr      e

        dec     a
        jp      1$
 
__rlslong_rrx_s::
__rlulong_rrx_s::
        ld      hl,#2
        add     hl,sp

        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        inc     hl
        ld      c,(hl)
        inc     hl
        ld      b,(hl)
        inc     hl
        ld      a,(hl)

        ld      l,c
        ld      h,b
1$:
        or      a,a
        ret     Z

        rl      e
        rl      d
        rl      l
        rl      h

        dec     a
        jp      1$
