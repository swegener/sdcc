; /*-------------------------------------------------------------------------
; 
;   crtstart.asm :- C run-time: startup                
; 
;    This library is free software; you can redistribute it and/or modify it
;    under the terms of the GNU Library General Public License as published by the
;    Free Software Foundation; either version 2, or (at your option) any
;    later version.
;    
;    This library is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU Library General Public License for more details.
;    
;    You should have received a copy of the GNU Library General Public License
;    along with this program; if not, write to the Free Software
;    Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;    
;    In other words, you are welcome to use, share and improve this program.
;    You are forbidden to forbid anyone else to use, share and improve
;    what you give them.   Help stamp out software-hoarding!  
; -------------------------------------------------------------------------*/

	.area CSEG    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)

	.globl __start__stack
	
	.area GSINIT0 (CODE)

__sdcc_gsinit_startup::
        mov     sp,#__start__stack - 1

	.area GSINIT2 (CODE)
	
        lcall   __sdcc_external_startup
        mov     a,dpl
        jz      __sdcc_init_data
        ljmp    __sdcc_program_startup
__sdcc_init_data:
