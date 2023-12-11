/*-------------------------------------------------------------------------
   setjmp.h - header file for setjmp & longjmp ANSI routines

   Copyright (C) 2023 Jonathon Hall <dabigjhall@gmail.com>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
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

#ifndef __SETJMP_H__
#define __SETJMP_H__

#define __STDC_VERSION_SETJMP_H__ __STDC_VERSION__

/*
 * return address - 3 (TOS)
 * return address stack index - 1 (STKPTR)
 * stack pointer - 1 (small) / 2 (large) (FSR1)
 * frame pointer - 1 (small) / 2 (large) (FSR2)
 */
#if defined(__SDCC_MODEL_LARGE)
#define __SETJMP_H_STACK_PTR_SIZE 2
#else
#define __SETJMP_H_STACK_PTR_SIZE 1
#endif
typedef unsigned char jmp_buf[2 * __SETJMP_H_STACK_PTR_SIZE + 4];

int __setjmp (jmp_buf);

/* C99 might require setjmp to be a macro. The standard seems self-contradicting on this issue. */
/* However, it is clear that the standards allow setjmp to be a macro. */
#define setjmp(jump_buf) __setjmp(jump_buf)

#ifndef __SDCC_HIDE_LONGJMP
_Noreturn void longjmp(jmp_buf, int);
#endif

#undef __SETJMP_H_STACK_PTR_SIZE

#endif /* __SETJMP_H__ */
