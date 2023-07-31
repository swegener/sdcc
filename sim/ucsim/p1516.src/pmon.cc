#include "pmon.h"

t_mem pmon[]= {
0x00000000, 0x01f2f038, /* 01f2f038 //C 00000 jmp	cold_start */
0x0000f000, 0x01f2f021, /* 01f2f021 //C 0f000 _f000:	jmp	callin */
0x0000f001, 0x01f2f012, /* 01f2f012 //C 0f001 _f001:	jmp	enter_by_uart */
0x0000f002, 0x00f00e00, /* 00f00e00 //C 0f002 _f002:	ret */
0x0000f003, 0x00f00e00, /* 00f00e00 //C 0f003 _f003:	ret */
0x0000f004, 0x00f00e00, /* 00f00e00 //C 0f004 _f004:	ret */
0x0000f005, 0x01f2f038, /* 01f2f038 //C 0f005 _f005:	jmp	cold_start */
0x0000f006, 0x01f2f39e, /* 01f2f39e //C 0f006 _f006:	jmp	strchr */
0x0000f007, 0x01f2f3d9, /* 01f2f3d9 //C 0f007 _f007:	jmp	streq */
0x0000f008, 0x01f2f43f, /* 01f2f43f //C 0f008 _f008:	jmp	check_uart */
0x0000f009, 0x01f2f3e7, /* 01f2f3e7 //C 0f009 _f009:	jmp	hexchar2value */
0x0000f00a, 0x01f2f416, /* 01f2f416 //C 0f00a _f00a:	jmp	value2hexchar */
0x0000f00b, 0x01f2f41b, /* 01f2f41b //C 0f00b _f00b:	jmp	htoi */
0x0000f00c, 0x01f2f3e0, /* 01f2f3e0 //C 0f00c _f00c:	jmp	strieq */
0x0000f00d, 0x01f2f447, /* 01f2f447 //C 0f00d _f00d:	jmp	read */
0x0000f00e, 0x01f2f44a, /* 01f2f44a //C 0f00e _f00e:	jmp	putchar */
0x0000f00f, 0x01f2f451, /* 01f2f451 //C 0f00f _f00f:	jmp	prints */
0x0000f010, 0x01f2f465, /* 01f2f465 //C 0f010 _f010:	jmp	printsnl */
0x0000f011, 0x01f2f46d, /* 01f2f46d //C 0f011 _f011:	jmp	print_vhex */
0x0000f012, 0x0d0d0000, /* 0d0d0000 //C 0f012 push	r0 */
0x0000f013, 0x020e0000, /* 020e0000 //C 0f013 getf	r0 */
0x0000f014, 0x0d0d0000, /* 0d0d0000 //C 0f014 push	r0 */
0x0000f015, 0x0700ff42, /* 0700ff42 //C 0f015 ld	r0,UART_RSTAT */
0x0000f016, 0x010c0001, /* 010c0001 //C 0f016 test	r0,1 */
0x0000f017, 0x21f2f01c, /* 21f2f01c //C 0f017 jnz	ebu_callin */
0x0000f018, 0x0f0d0000, /* 0f0d0000 //C 0f018 pop	r0 */
0x0000f019, 0x020f0000, /* 020f0000 //C 0f019 setf	r0 */
0x0000f01a, 0x0f0d0000, /* 0f0d0000 //C 0f01a pop	r0 */
0x0000f01b, 0x00f00e00, /* 00f00e00 //C 0f01b ret */
0x0000f01c, 0x0700ff40, /* 0700ff40 //C 0f01c ld	r0,UART_DR */
0x0000f01d, 0x0f0d0000, /* 0f0d0000 //C 0f01d pop	r0 */
0x0000f01e, 0x020f0000, /* 020f0000 //C 0f01e setf	r0 */
0x0000f01f, 0x0f0d0000, /* 0f0d0000 //C 0f01f pop	r0 */
0x0000f020, 0x01f2f021, /* 01f2f021 //C 0f020 jmp	callin */
0x0000f021, 0x0600f502, /* 0600f502 //C 0f021 st	r0,reg0 */
0x0000f022, 0x0610f503, /* 0610f503 //C 0f022 st	r1,reg1 */
0x0000f023, 0x0620f504, /* 0620f504 //C 0f023 st	r2,reg2 */
0x0000f024, 0x0630f505, /* 0630f505 //C 0f024 st	r3,reg3 */
0x0000f025, 0x0640f506, /* 0640f506 //C 0f025 st	r4,reg4 */
0x0000f026, 0x0650f507, /* 0650f507 //C 0f026 st	r5,reg5 */
0x0000f027, 0x0660f508, /* 0660f508 //C 0f027 st	r6,reg6 */
0x0000f028, 0x0670f509, /* 0670f509 //C 0f028 st	r7,reg7 */
0x0000f029, 0x0680f50a, /* 0680f50a //C 0f029 st	r8,reg8 */
0x0000f02a, 0x0690f50b, /* 0690f50b //C 0f02a st	r9,reg9 */
0x0000f02b, 0x06a0f50c, /* 06a0f50c //C 0f02b st	r10,reg10 */
0x0000f02c, 0x06b0f50d, /* 06b0f50d //C 0f02c st	r11,reg11 */
0x0000f02d, 0x06c0f50e, /* 06c0f50e //C 0f02d st	r12,reg12 */
0x0000f02e, 0x06d0f50f, /* 06d0f50f //C 0f02e st	r13,reg13 */
0x0000f02f, 0x06e0f510, /* 06e0f510 //C 0f02f st	r14,reg14 */
0x0000f030, 0x06e0f511, /* 06e0f511 //C 0f030 st	r14,reg15 */
0x0000f031, 0x020e0000, /* 020e0000 //C 0f031 getf	r0 */
0x0000f032, 0x0600f512, /* 0600f512 //C 0f032 st	r0,regf */
0x0000f033, 0x01020001, /* 01020001 //C 0f033 mvzl	r0,1 */
0x0000f034, 0x0600f500, /* 0600f500 //C 0f034 st	r0,called */
0x0000f035, 0x01f2f042, /* 01f2f042 //C 0f035 jmp	common_start */
0x0000f036, 0x01f2f042, /* 01f2f042 //C 0f036 jmp	common_start */
0x0000f037, 0x01f2f038, /* 01f2f038 //C 0f037 jmp	cold_start */
0x0000f038, 0x01020000, /* 01020000 //C 0f038 mvzl	r0,0 */
0x0000f039, 0x0600f500, /* 0600f500 //C 0f039 st	r0,called */
0x0000f03a, 0x0102f037, /* 0102f037 //C 0f03a mvzl	r0,def_zero	; restore jmp to monitor at zero */
0x0000f03b, 0x0e000000, /* 0e000000 //C 0f03b ld	r0,r0 */
0x0000f03c, 0x06000000, /* 06000000 //C 0f03c st	r0,0 */
0x0000f03d, 0x01020000, /* 01020000 //C 0f03d mvzl	r0,0		; def values of some regs */
0x0000f03e, 0x0600f512, /* 0600f512 //C 0f03e st	r0,regf		; FALGS= 0 */
0x0000f03f, 0x0102f7ff, /* 0102f7ff //C 0f03f mvzl	r0,0xf7ff	; R13= 0xf7ff */
0x0000f040, 0x0600f50f, /* 0600f50f //C 0f040 st	r0,reg13 */
0x0000f041, 0x01f2f042, /* 01f2f042 //C 0f041 jmp	common_start: */
0x0000f042, 0x01d2f752, /* 01d2f752 //C 0f042 mvzl	sp,stack_end */
0x0000f043, 0x01020001, /* 01020001 //C 0f043 mvzl	r0,1 */
0x0000f044, 0x0600f4ff, /* 0600f4ff //C 0f044 st	r0,echo */
0x0000f045, 0x01020000, /* 01020000 //C 0f045 mvzl	r0,0 */
0x0000f046, 0x020f0000, /* 020f0000 //C 0f046 setf	r0 */
0x0000f047, 0x01120003, /* 01120003 //C 0f047 mvzl	r1,3 */
0x0000f048, 0x0610ff41, /* 0610ff41 //C 0f048 st	r1,UART_CTRL */
0x0000f049, 0x0102f513, /* 0102f513 //C 0f049 mvzl	r0,msg_start */
0x0000f04a, 0x0400f465, /* 0400f465 //C 0f04a call	printsnl */
0x0000f04b, 0x0700f500, /* 0700f500 //C 0f04b ld	r0,called */
0x0000f04c, 0x020b0000, /* 020b0000 //C 0f04c sz	r0 */
0x0000f04d, 0x11f2f057, /* 11f2f057 //C 0f04d jz	no_called_from */
0x0000f04e, 0x0102000a, /* 0102000a //C 0f04e mvzl	r0,LF */
0x0000f04f, 0x0400f44a, /* 0400f44a //C 0f04f call	putchar */
0x0000f050, 0x0102f521, /* 0102f521 //C 0f050 mvzl	r0,msg_stopat */
0x0000f051, 0x0400f451, /* 0400f451 //C 0f051 call	prints */
0x0000f052, 0x0700f510, /* 0700f510 //C 0f052 ld	r0,reg14 */
0x0000f053, 0x01120004, /* 01120004 //C 0f053 mvzl	r1,4 */
0x0000f054, 0x0400f46d, /* 0400f46d //C 0f054 call	print_vhex */
0x0000f055, 0x0102000a, /* 0102000a //C 0f055 mvzl	r0,LF */
0x0000f056, 0x0400f44a, /* 0400f44a //C 0f056 call	putchar */
0x0000f057, 0x0400f059, /* 0400f059 //C 0f057 call	setup_line */
0x0000f058, 0x01f2f063, /* 01f2f063 //C 0f058 jmp	main */
0x0000f059, 0x0ded0000, /* 0ded0000 //C 0f059 push	lr */
0x0000f05a, 0x01120000, /* 01120000 //C 0f05a mvzl	r1,0		; lptr= 0 */
0x0000f05b, 0x0610f4f8, /* 0610f4f8 //C 0f05b st	r1,line_ptr */
0x0000f05c, 0x0610f494, /* 0610f494 //C 0f05c st	r1,line		; line[0]= 0 */
0x0000f05d, 0x01120000, /* 01120000 //C 0f05d mvzl	r1,0		; at_eol= 0 */
0x0000f05e, 0x0610f4f9, /* 0610f4f9 //C 0f05e st	r1,at_eol */
0x0000f05f, 0x0102f52f, /* 0102f52f //C 0f05f mvzl	r0,prompt */
0x0000f060, 0x0400f451, /* 0400f451 //C 0f060 call	prints */
0x0000f061, 0x0fed0000, /* 0fed0000 //C 0f061 pop	lr */
0x0000f062, 0x00f00e00, /* 00f00e00 //C 0f062 ret */
0x0000f063, 0x0400f43f, /* 0400f43f //C 0f063 call	check_uart */
0x0000f064, 0x41f2f06a, /* 41f2f06a //C 0f064 C0 jmp	no_input */
0x0000f065, 0x0400f447, /* 0400f447 //C 0f065 call	read */
0x0000f066, 0x0400f06b, /* 0400f06b //C 0f066 call	proc_input */
0x0000f067, 0x41f2f06a, /* 41f2f06a //C 0f067 C0 jmp	no_line */
0x0000f068, 0x0400f09d, /* 0400f09d //C 0f068 call	proc_line */
0x0000f069, 0x0400f059, /* 0400f059 //C 0f069 call	setup_line */
0x0000f06a, 0x01f2f063, /* 01f2f063 //C 0f06a jmp	main */
0x0000f06b, 0x0ded0000, /* 0ded0000 //C 0f06b push	lr */
0x0000f06c, 0x0108000a, /* 0108000a //C 0f06c cmp	r0,LF */
0x0000f06d, 0x11f2f096, /* 11f2f096 //C 0f06d EQ jmp	got_eol */
0x0000f06e, 0x0108000d, /* 0108000d //C 0f06e cmp	r0,CR */
0x0000f06f, 0x11f2f096, /* 11f2f096 //C 0f06f EQ jmp	got_eol */
0x0000f070, 0x01080008, /* 01080008 //C 0f070 cmp	r0,8 */
0x0000f071, 0x11f2f075, /* 11f2f075 //C 0f071 jz	got_BS */
0x0000f072, 0x0108007f, /* 0108007f //C 0f072 cmp	r0,127 */
0x0000f073, 0x11f2f075, /* 11f2f075 //C 0f073 jz	got_DEL */
0x0000f074, 0x01f2f081, /* 01f2f081 //C 0f074 jmp	got_char */
0x0000f075, 0x0700f4f8, /* 0700f4f8 //C 0f075 ld	r0,line_ptr */
0x0000f076, 0x020b0000, /* 020b0000 //C 0f076 sz	r0 */
0x0000f077, 0x11f2f07f, /* 11f2f07f //C 0f077 jz	got_done */
0x0000f078, 0x01060001, /* 01060001 //C 0f078 sub	r0,1 */
0x0000f079, 0x0600f4f8, /* 0600f4f8 //C 0f079 st	r0,line_ptr */
0x0000f07a, 0x0112f494, /* 0112f494 //C 0f07a mvzl	r1,line */
0x0000f07b, 0x01220000, /* 01220000 //C 0f07b mvzl	r2,0 */
0x0000f07c, 0x08210000, /* 08210000 //C 0f07c st	r2,r1,r0 */
0x0000f07d, 0x0102f52b, /* 0102f52b //C 0f07d mvzl	r0,msg_BS */
0x0000f07e, 0x0400f451, /* 0400f451 //C 0f07e call	prints */
0x0000f07f, 0x020d0000, /* 020d0000 //C 0f07f clc */
0x0000f080, 0x01f2f09b, /* 01f2f09b //C 0f080 jmp	proc_input_ret */
0x0000f081, 0x0108001f, /* 0108001f //C 0f081 cmp	r0,31		; refuse control chars */
0x0000f082, 0xa1f2f09b, /* a1f2f09b //C 0f082 LS jmp	proc_input_ret */
0x0000f083, 0x0108007e, /* 0108007e //C 0f083 cmp	r0,126		; refuse graph chars */
0x0000f084, 0x91f2f09b, /* 91f2f09b //C 0f084 HI jmp	proc_input_ret */
0x0000f085, 0x01220000, /* 01220000 //C 0f085 mvzl	r2,0		; at_aol= 0 */
0x0000f086, 0x0620f4f9, /* 0620f4f9 //C 0f086 st	r2,at_eol */
0x0000f087, 0x0112f4f8, /* 0112f4f8 //C 0f087 mvzl	r1,line_ptr	; line[line_ptr]= char */
0x0000f088, 0x0e310000, /* 0e310000 //C 0f088 ld	r3,r1 */
0x0000f089, 0x0122f494, /* 0122f494 //C 0f089 mvzl	r2,line */
0x0000f08a, 0x08020300, /* 08020300 //C 0f08a st	r0,r2,r3 */
0x0000f08b, 0x013a0001, /* 013a0001 //C 0f08b plus	r3,1		; line_ptr++ */
0x0000f08c, 0x0c310000, /* 0c310000 //C 0f08c st	r3,r1 */
0x0000f08d, 0x01420000, /* 01420000 //C 0f08d mvzl	r4,0 */
0x0000f08e, 0x09438200, /* 09438200 //C 0f08e st	r4,r3+,r2	; line[line_ptr]= 0 */
0x0000f08f, 0x08430200, /* 08430200 //C 0f08f st	r4,r3,r2	; double 0 at end, needed by tokenizer */
0x0000f090, 0x0142f4ff, /* 0142f4ff //C 0f090 mvzl	r4,echo		; check if echo is turned on */
0x0000f091, 0x0e440000, /* 0e440000 //C 0f091 ld	r4,r4 */
0x0000f092, 0x024b0000, /* 024b0000 //C 0f092 sz	r4 */
0x0000f093, 0x2400f44a, /* 2400f44a //C 0f093 NZ call	putchar		; echo */
0x0000f094, 0x020d0000, /* 020d0000 //C 0f094 clc */
0x0000f095, 0x01f2f09b, /* 01f2f09b //C 0f095 jmp	proc_input_ret */
0x0000f096, 0x0112f4f9, /* 0112f4f9 //C 0f096 mvzl	r1,at_eol */
0x0000f097, 0x0e110000, /* 0e110000 //C 0f097 ld	r1,r1 */
0x0000f098, 0x021b0000, /* 021b0000 //C 0f098 sz	r1		; Z=0 at eol -> skip, not ready */
0x0000f099, 0x220d0000, /* 220d0000 //C 0f099 Z0 clc */
0x0000f09a, 0x120c0000, /* 120c0000 //C 0f09a Z1 sec */
0x0000f09b, 0x0fed0000, /* 0fed0000 //C 0f09b pop	lr */
0x0000f09c, 0x00f00e00, /* 00f00e00 //C 0f09c ret */
0x0000f09d, 0x0ded0000, /* 0ded0000 //C 0f09d push	lr */
0x0000f09e, 0x0102000a, /* 0102000a //C 0f09e mvzl	r0,LF */
0x0000f09f, 0x0400f44a, /* 0400f44a //C 0f09f call	putchar */
0x0000f0a0, 0x0700f494, /* 0700f494 //C 0f0a0 ld	r0,line */
0x0000f0a1, 0x020b0000, /* 020b0000 //C 0f0a1 sz	r0 */
0x0000f0a2, 0x11f2f0aa, /* 11f2f0aa //C 0f0a2 jz	proc_line_ret */
0x0000f0a3, 0x0400f0c8, /* 0400f0c8 //C 0f0a3 call	tokenize */
0x0000f0a4, 0x0400f0e9, /* 0400f0e9 //C 0f0a4 call	find_cmd */
0x0000f0a5, 0x41f2f0a8, /* 41f2f0a8 //C 0f0a5 C0 jmp	cmd_not_found */
0x0000f0a6, 0x05000000, /* 05000000 //C 0f0a6 call	r0,0 */
0x0000f0a7, 0x01f2f0aa, /* 01f2f0aa //C 0f0a7 jmp	proc_line_ret */
0x0000f0a8, 0x0102f0b3, /* 0102f0b3 //C 0f0a8 mvzl	r0,snotfound */
0x0000f0a9, 0x0400f465, /* 0400f465 //C 0f0a9 call	printsnl */
0x0000f0aa, 0x01120001, /* 01120001 //C 0f0aa mvzl	r1,1		; at_eol= 1 */
0x0000f0ab, 0x0610f4f9, /* 0610f4f9 //C 0f0ab st	r1,at_eol */
0x0000f0ac, 0x0fed0000, /* 0fed0000 //C 0f0ac pop	lr */
0x0000f0ad, 0x00f00e00, /* 00f00e00 //C 0f0ad ret */
0x0000f0ae, 0x00000047, /* 00000047 //C 0f0ae db	71 */
0x0000f0af, 0x0000006f, /* 0000006f //C 0f0af db	111 */
0x0000f0b0, 0x00000074, /* 00000074 //C 0f0b0 db	116 */
0x0000f0b1, 0x0000003a, /* 0000003a //C 0f0b1 db	58 */
0x0000f0b2, 0x00000000, /* 00000000 //C 0f0b2 db */
0x0000f0b3, 0x00000055, /* 00000055 //C 0f0b3 db	85 */
0x0000f0b4, 0x0000006e, /* 0000006e //C 0f0b4 db	110 */
0x0000f0b5, 0x0000006b, /* 0000006b //C 0f0b5 db	107 */
0x0000f0b6, 0x0000006e, /* 0000006e //C 0f0b6 db	110 */
0x0000f0b7, 0x0000006f, /* 0000006f //C 0f0b7 db	111 */
0x0000f0b8, 0x00000077, /* 00000077 //C 0f0b8 db	119 */
0x0000f0b9, 0x0000006e, /* 0000006e //C 0f0b9 db	110 */
0x0000f0ba, 0x00000020, /* 00000020 //C 0f0ba db	32 */
0x0000f0bb, 0x00000063, /* 00000063 //C 0f0bb db	99 */
0x0000f0bc, 0x0000006f, /* 0000006f //C 0f0bc db	111 */
0x0000f0bd, 0x0000006d, /* 0000006d //C 0f0bd db	109 */
0x0000f0be, 0x0000006d, /* 0000006d //C 0f0be db	109 */
0x0000f0bf, 0x00000061, /* 00000061 //C 0f0bf db	97 */
0x0000f0c0, 0x0000006e, /* 0000006e //C 0f0c0 db	110 */
0x0000f0c1, 0x00000064, /* 00000064 //C 0f0c1 db	100 */
0x0000f0c2, 0x00000000, /* 00000000 //C 0f0c2 db */
0x0000f0c3, 0x0ded0000, /* 0ded0000 //C 0f0c3 push	lr */
0x0000f0c4, 0x0112f531, /* 0112f531 //C 0f0c4 mvzl	r1,delimiters */
0x0000f0c5, 0x0400f39e, /* 0400f39e //C 0f0c5 call	strchr */
0x0000f0c6, 0x0fed0000, /* 0fed0000 //C 0f0c6 pop	lr */
0x0000f0c7, 0x00f00e00, /* 00f00e00 //C 0f0c7 ret */
0x0000f0c8, 0x0ded0000, /* 0ded0000 //C 0f0c8 push	lr */
0x0000f0c9, 0x0142f4fa, /* 0142f4fa //C 0f0c9 mvzl	r4,words	; array of result */
0x0000f0ca, 0x0152f494, /* 0152f494 //C 0f0ca mvzl	r5,line		; address of next char */
0x0000f0cb, 0x01620000, /* 01620000 //C 0f0cb mvzl	r6,0		; nuof words found */
0x0000f0cc, 0x01720000, /* 01720000 //C 0f0cc mvzl	r7,0		; bool in_word */
0x0000f0cd, 0x0e050000, /* 0e050000 //C 0f0cd ld	r0,r5		; pick a char */
0x0000f0ce, 0x020b0000, /* 020b0000 //C 0f0ce sz	r0		; check end */
0x0000f0cf, 0x11f2f0d9, /* 11f2f0d9 //C 0f0cf jz	tok_delimiter	; found end, pretend delim */
0x0000f0d0, 0x0400f0c3, /* 0400f0c3 //C 0f0d0 call	is_delimiter */
0x0000f0d1, 0x31f2f0d9, /* 31f2f0d9 //C 0f0d1 C1 jmp	tok_delimiter */
0x0000f0d2, 0x027b0000, /* 027b0000 //C 0f0d2 sz	r7 */
0x0000f0d3, 0x21f2f0de, /* 21f2f0de //C 0f0d3 jnz	tok_next	; still inside word */
0x0000f0d4, 0x01720001, /* 01720001 //C 0f0d4 mvzl	r7,1		; in_word=true */
0x0000f0d5, 0x09568400, /* 09568400 //C 0f0d5 st	r5,r6+,r4	; record word address */
0x0000f0d6, 0x01680005, /* 01680005 //C 0f0d6 cmp	r6,MAX_WORDS	; If no more space */
0x0000f0d7, 0x11f2f0e2, /* 11f2f0e2 //C 0f0d7 EQ jmp	tok_ret		; then return */
0x0000f0d8, 0x01f2f0de, /* 01f2f0de //C 0f0d8 jmp	tok_next */
0x0000f0d9, 0x027b0000, /* 027b0000 //C 0f0d9 sz	r7 */
0x0000f0da, 0x11f2f0de, /* 11f2f0de //C 0f0da jz	tok_next	; still between words */
0x0000f0db, 0x01720000, /* 01720000 //C 0f0db mvzl	r7,0		; in_word=false */
0x0000f0dc, 0x01120000, /* 01120000 //C 0f0dc mvzl	r1,0		; put a 0 at the end of word */
0x0000f0dd, 0x08150100, /* 08150100 //C 0f0dd st	r1,r5,r1 */
0x0000f0de, 0x020b0000, /* 020b0000 //C 0f0de sz	r0		; check EOL */
0x0000f0df, 0x11f2f0e2, /* 11f2f0e2 //C 0f0df jz	tok_ret		; jump out if char==0 */
0x0000f0e0, 0x01540001, /* 01540001 //C 0f0e0 add	r5,1 */
0x0000f0e1, 0x01f2f0cd, /* 01f2f0cd //C 0f0e1 jmp	tok_cycle */
0x0000f0e2, 0x01120000, /* 01120000 //C 0f0e2 mvzl	r1,0 */
0x0000f0e3, 0x01680005, /* 01680005 //C 0f0e3 cmp	r6,MAX_WORDS */
0x0000f0e4, 0x11f2f0e7, /* 11f2f0e7 //C 0f0e4 jz	tok_end */
0x0000f0e5, 0x09168400, /* 09168400 //C 0f0e5 st	r1,r6+,r4 */
0x0000f0e6, 0x01f2f0e2, /* 01f2f0e2 //C 0f0e6 jmp	tok_ret */
0x0000f0e7, 0x0fed0000, /* 0fed0000 //C 0f0e7 pop	lr */
0x0000f0e8, 0x00f00e00, /* 00f00e00 //C 0f0e8 ret */
0x0000f0e9, 0x0ded0000, /* 0ded0000 //C 0f0e9 push	lr */
0x0000f0ea, 0x0d1d0000, /* 0d1d0000 //C 0f0ea push	r1 */
0x0000f0eb, 0x0d2d0000, /* 0d2d0000 //C 0f0eb push	r2 */
0x0000f0ec, 0x0d3d0000, /* 0d3d0000 //C 0f0ec push	r3 */
0x0000f0ed, 0x0dad0000, /* 0dad0000 //C 0f0ed push	r10 */
0x0000f0ee, 0x0700f4fa, /* 0700f4fa //C 0f0ee ld	r0,words	; R0= 1st word of command */
0x0000f0ef, 0x020b0000, /* 020b0000 //C 0f0ef sz	r0 */
0x0000f0f0, 0x11f2f120, /* 11f2f120 //C 0f0f0 jz	find_cmd_false */
0x0000f0f1, 0x0e100000, /* 0e100000 //C 0f0f1 ld	r1,r0		; 1st char of word1 */
0x0000f0f2, 0x0e200001, /* 0e200001 //C 0f0f2 ld	r2,r0,1		; 2nd char */
0x0000f0f3, 0x0e300002, /* 0e300002 //C 0f0f3 ld	r3,r0,2		; 3rd char */
0x0000f0f4, 0x011fffdf, /* 011fffdf //C 0f0f4 and	r1,0xffdf	; upcase 1st char */
0x0000f0f5, 0x01180052, /* 01180052 //C 0f0f5 cmp	r1,'R' */
0x0000f0f6, 0x21f2f10c, /* 21f2f10c //C 0f0f6 jnz	find_not_rx */
0x0000f0f7, 0x0128002f, /* 0128002f //C 0f0f7 cmp	r2,'/'		; '0'-1 */
0x0000f0f8, 0xa1f2f10c, /* a1f2f10c //C 0f0f8 LS jmp	find_not_rx */
0x0000f0f9, 0x01280039, /* 01280039 //C 0f0f9 cmp	r2,'9' */
0x0000f0fa, 0x91f2f10c, /* 91f2f10c //C 0f0fa HI jmp	find_not_rx */
0x0000f0fb, 0x023b0000, /* 023b0000 //C 0f0fb sz	r3 */
0x0000f0fc, 0x11f2f107, /* 11f2f107 //C 0f0fc jz	find_rx_09 */
0x0000f0fd, 0x01280031, /* 01280031 //C 0f0fd cmp	r2,'1'		; first char must be '1' */
0x0000f0fe, 0x21f2f10c, /* 21f2f10c //C 0f0fe jnz	find_not_rx */
0x0000f0ff, 0x0138002f, /* 0138002f //C 0f0ff cmp	r3,'/'		; '0'-1 */
0x0000f100, 0xa1f2f10c, /* a1f2f10c //C 0f100 LS jmp	find_not_rx */
0x0000f101, 0x01380035, /* 01380035 //C 0f101 cmp	r3,'5' */
0x0000f102, 0x91f2f10c, /* 91f2f10c //C 0f102 HI jmp	find_not_rx */
0x0000f103, 0x01360030, /* 01360030 //C 0f103 sub	r3,'0' */
0x0000f104, 0x0134000a, /* 0134000a //C 0f104 add	r3,10 */
0x0000f105, 0x0630f501, /* 0630f501 //C 0f105 st	r3,nuof_reg */
0x0000f106, 0x01f2f109, /* 01f2f109 //C 0f106 jmp	find_rx */
0x0000f107, 0x01260030, /* 01260030 //C 0f107 sub	r2,'0' */
0x0000f108, 0x0620f501, /* 0620f501 //C 0f108 st	r2,nuof_reg */
0x0000f109, 0x0102f35c, /* 0102f35c //C 0f109 mvzl	r0,cmd_rx */
0x0000f10a, 0x020c0000, /* 020c0000 //C 0f10a sec */
0x0000f10b, 0x01f2f12b, /* 01f2f12b //C 0f10b jmp	find_cmd_ret */
0x0000f10c, 0x01a2f54d, /* 01a2f54d //C 0f10c mvzl	r10,commands */
0x0000f10d, 0x0e2a0000, /* 0e2a0000 //C 0f10d ld	r2,r10		; R2= cmd addr */
0x0000f10e, 0x022b0000, /* 022b0000 //C 0f10e sz	r2 */
0x0000f10f, 0x11f2f120, /* 11f2f120 //C 0f10f jz	find_cmd_false */
0x0000f110, 0x01a40001, /* 01a40001 //C 0f110 add	r10,1 */
0x0000f111, 0x00100a00, /* 00100a00 //C 0f111 mov	r1,r10		; R1= cmd string */
0x0000f112, 0x01a40001, /* 01a40001 //C 0f112 add	r10,1 */
0x0000f113, 0x0e3a0000, /* 0e3a0000 //C 0f113 ld	r3,r10 */
0x0000f114, 0x023b0000, /* 023b0000 //C 0f114 sz	r3 */
0x0000f115, 0x21f2f112, /* 21f2f112 //C 0f115 jnz	find_cmd_fw */
0x0000f116, 0x01a40001, /* 01a40001 //C 0f116 add	r10,1 */
0x0000f117, 0x0400f3d9, /* 0400f3d9 //C 0f117 call	streq */
0x0000f118, 0x41f2f10d, /* 41f2f10d //C 0f118 C0 jmp	find_cmd_cyc */
0x0000f119, 0x00000200, /* 00000200 //C 0f119 mov	r0,r2 */
0x0000f11a, 0x020c0000, /* 020c0000 //C 0f11a sec */
0x0000f11b, 0x01f2f12b, /* 01f2f12b //C 0f11b jmp	find_cmd_ret */
0x0000f11c, 0x0000002f, /* 0000002f //C 0f11c db	47 */
0x0000f11d, 0x0000002f, /* 0000002f //C 0f11d db	47 */
0x0000f11e, 0x00000043, /* 00000043 //C 0f11e db	67 */
0x0000f11f, 0x00000000, /* 00000000 //C 0f11f db */
0x0000f120, 0x01040001, /* 01040001 //C 0f120 add	r0,1		; check second word */
0x0000f121, 0x0e100000, /* 0e100000 //C 0f121 ld	r1,r0		; for //C command */
0x0000f122, 0x021b0000, /* 021b0000 //C 0f122 sz	r1 */
0x0000f123, 0x11f2f129, /* 11f2f129 //C 0f123 jz	find_cmd_very_false */
0x0000f124, 0x0102f11c, /* 0102f11c //C 0f124 mvzl	r0,c_cmd_name */
0x0000f125, 0x0400f3d9, /* 0400f3d9 //C 0f125 call	streq */
0x0000f126, 0x21f2f129, /* 21f2f129 //C 0f126 jnz	find_cmd_very_false */
0x0000f127, 0x0122f1e2, /* 0122f1e2 //C 0f127 mvzl	r2,cmd_c */
0x0000f128, 0x01f2f119, /* 01f2f119 //C 0f128 jmp	find_cmd_true */
0x0000f129, 0x020d0000, /* 020d0000 //C 0f129 clc */
0x0000f12a, 0x01020000, /* 01020000 //C 0f12a mvzl	r0,0 */
0x0000f12b, 0x0fad0000, /* 0fad0000 //C 0f12b pop	r10 */
0x0000f12c, 0x0f3d0000, /* 0f3d0000 //C 0f12c pop	r3 */
0x0000f12d, 0x0f2d0000, /* 0f2d0000 //C 0f12d pop	r2 */
0x0000f12e, 0x0f1d0000, /* 0f1d0000 //C 0f12e pop	r1 */
0x0000f12f, 0x0fed0000, /* 0fed0000 //C 0f12f pop	lr */
0x0000f130, 0x00f00e00, /* 00f00e00 //C 0f130 ret */
0x0000f131, 0x0ded0000, /* 0ded0000 //C 0f131 push	lr */
0x0000f132, 0x0122f4fa, /* 0122f4fa //C 0f132 mvzl	r2,words */
0x0000f133, 0x01020000, /* 01020000 //C 0f133 mvzl	r0,0 */
0x0000f134, 0x0e420001, /* 0e420001 //C 0f134 ld	r4,r2,1		; addr */
0x0000f135, 0x0e520002, /* 0e520002 //C 0f135 ld	r5,r2,2		; value */
0x0000f136, 0x024b0000, /* 024b0000 //C 0f136 sz 	r4 */
0x0000f137, 0x11f2f15d, /* 11f2f15d //C 0f137 jz	m_ret */
0x0000f138, 0x00000400, /* 00000400 //C 0f138 mov	r0,r4 */
0x0000f139, 0x0400f41b, /* 0400f41b //C 0f139 call	htoi */
0x0000f13a, 0x00400100, /* 00400100 //C 0f13a mov	r4,r1 */
0x0000f13b, 0x31f2f13f, /* 31f2f13f //C 0f13b C1 jmp	m_addr_ok */
0x0000f13c, 0x0102f15f, /* 0102f15f //C 0f13c mvzl	r0,m_err_addr */
0x0000f13d, 0x0400f465, /* 0400f465 //C 0f13d call	printsnl */
0x0000f13e, 0x01f2f15d, /* 01f2f15d //C 0f13e jmp	m_ret */
0x0000f13f, 0x025b0000, /* 025b0000 //C 0f13f sz	r5 */
0x0000f140, 0x11f2f153, /* 11f2f153 //C 0f140 jz	m_read */
0x0000f141, 0x0132f000, /* 0132f000 //C 0f141 mvzl	r3,the_begin */
0x0000f142, 0x00380400, /* 00380400 //C 0f142 cmp	r3,r4 */
0x0000f143, 0x91f2f14b, /* 91f2f14b //C 0f143 HI jmp	m_addrv_ok */
0x0000f144, 0x0132f761, /* 0132f761 //C 0f144 mvzl	r3,the_end */
0x0000f145, 0x00380400, /* 00380400 //C 0f145 cmp	r3,r4 */
0x0000f146, 0x91f2f148, /* 91f2f148 //C 0f146 HI jmp	m_addrv_nok */
0x0000f147, 0x01f2f14b, /* 01f2f14b //C 0f147 jmp	m_addrv_ok */
0x0000f148, 0x0102f16d, /* 0102f16d //C 0f148 mvzl	r0,m_err_addrv */
0x0000f149, 0x0400f465, /* 0400f465 //C 0f149 call	printsnl */
0x0000f14a, 0x01f2f15d, /* 01f2f15d //C 0f14a jmp	m_ret */
0x0000f14b, 0x00000500, /* 00000500 //C 0f14b mov	r0,r5 */
0x0000f14c, 0x0400f41b, /* 0400f41b //C 0f14c call	htoi */
0x0000f14d, 0x00500100, /* 00500100 //C 0f14d mov	r5,r1 */
0x0000f14e, 0x31f2f152, /* 31f2f152 //C 0f14e C1 jmp	m_value_ok */
0x0000f14f, 0x0102f17f, /* 0102f17f //C 0f14f mvzl	r0,m_err_value */
0x0000f150, 0x0400f465, /* 0400f465 //C 0f150 call	printsnl */
0x0000f151, 0x01f2f15d, /* 01f2f15d //C 0f151 jmp	m_ret */
0x0000f152, 0x0c540000, /* 0c540000 //C 0f152 st	r5,r4 */
0x0000f153, 0x00000400, /* 00000400 //C 0f153 mov	r0,r4 */
0x0000f154, 0x01120004, /* 01120004 //C 0f154 mvzl	r1,4 */
0x0000f155, 0x0400f46d, /* 0400f46d //C 0f155 call	print_vhex */
0x0000f156, 0x01020020, /* 01020020 //C 0f156 mvzl	r0,0x20 */
0x0000f157, 0x0400f44a, /* 0400f44a //C 0f157 call	putchar */
0x0000f158, 0x0e040000, /* 0e040000 //C 0f158 ld	r0,r4 */
0x0000f159, 0x01120004, /* 01120004 //C 0f159 mvzl	r1,4 */
0x0000f15a, 0x0400f46d, /* 0400f46d //C 0f15a call	print_vhex */
0x0000f15b, 0x0102000a, /* 0102000a //C 0f15b mvzl	r0,LF */
0x0000f15c, 0x0400f44a, /* 0400f44a //C 0f15c call	putchar */
0x0000f15d, 0x0fed0000, /* 0fed0000 //C 0f15d pop	lr */
0x0000f15e, 0x00f00e00, /* 00f00e00 //C 0f15e ret */
0x0000f15f, 0x00000041, /* 00000041 //C 0f15f db	65 */
0x0000f160, 0x00000064, /* 00000064 //C 0f160 db	100 */
0x0000f161, 0x00000064, /* 00000064 //C 0f161 db	100 */
0x0000f162, 0x00000072, /* 00000072 //C 0f162 db	114 */
0x0000f163, 0x00000065, /* 00000065 //C 0f163 db	101 */
0x0000f164, 0x00000073, /* 00000073 //C 0f164 db	115 */
0x0000f165, 0x00000073, /* 00000073 //C 0f165 db	115 */
0x0000f166, 0x00000020, /* 00000020 //C 0f166 db	32 */
0x0000f167, 0x00000065, /* 00000065 //C 0f167 db	101 */
0x0000f168, 0x00000072, /* 00000072 //C 0f168 db	114 */
0x0000f169, 0x00000072, /* 00000072 //C 0f169 db	114 */
0x0000f16a, 0x0000006f, /* 0000006f //C 0f16a db	111 */
0x0000f16b, 0x00000072, /* 00000072 //C 0f16b db	114 */
0x0000f16c, 0x00000000, /* 00000000 //C 0f16c db */
0x0000f16d, 0x0000004d, /* 0000004d //C 0f16d db	77 */
0x0000f16e, 0x0000006f, /* 0000006f //C 0f16e db	111 */
0x0000f16f, 0x0000006e, /* 0000006e //C 0f16f db	110 */
0x0000f170, 0x00000069, /* 00000069 //C 0f170 db	105 */
0x0000f171, 0x00000074, /* 00000074 //C 0f171 db	116 */
0x0000f172, 0x0000006f, /* 0000006f //C 0f172 db	111 */
0x0000f173, 0x00000072, /* 00000072 //C 0f173 db	114 */
0x0000f174, 0x00000027, /* 00000027 //C 0f174 db	39 */
0x0000f175, 0x00000073, /* 00000073 //C 0f175 db	115 */
0x0000f176, 0x00000020, /* 00000020 //C 0f176 db	32 */
0x0000f177, 0x00000061, /* 00000061 //C 0f177 db	97 */
0x0000f178, 0x00000064, /* 00000064 //C 0f178 db	100 */
0x0000f179, 0x00000064, /* 00000064 //C 0f179 db	100 */
0x0000f17a, 0x00000072, /* 00000072 //C 0f17a db	114 */
0x0000f17b, 0x00000065, /* 00000065 //C 0f17b db	101 */
0x0000f17c, 0x00000073, /* 00000073 //C 0f17c db	115 */
0x0000f17d, 0x00000073, /* 00000073 //C 0f17d db	115 */
0x0000f17e, 0x00000000, /* 00000000 //C 0f17e db */
0x0000f17f, 0x00000056, /* 00000056 //C 0f17f db	86 */
0x0000f180, 0x00000061, /* 00000061 //C 0f180 db	97 */
0x0000f181, 0x0000006c, /* 0000006c //C 0f181 db	108 */
0x0000f182, 0x00000075, /* 00000075 //C 0f182 db	117 */
0x0000f183, 0x00000065, /* 00000065 //C 0f183 db	101 */
0x0000f184, 0x00000020, /* 00000020 //C 0f184 db	32 */
0x0000f185, 0x00000065, /* 00000065 //C 0f185 db	101 */
0x0000f186, 0x00000072, /* 00000072 //C 0f186 db	114 */
0x0000f187, 0x00000072, /* 00000072 //C 0f187 db	114 */
0x0000f188, 0x0000006f, /* 0000006f //C 0f188 db	111 */
0x0000f189, 0x00000072, /* 00000072 //C 0f189 db	114 */
0x0000f18a, 0x00000000, /* 00000000 //C 0f18a db */
0x0000f18b, 0x0ded0000, /* 0ded0000 //C 0f18b push	lr */
0x0000f18c, 0x0122f4fa, /* 0122f4fa //C 0f18c mvzl	r2,words */
0x0000f18d, 0x01020000, /* 01020000 //C 0f18d mvzl	r0,0 */
0x0000f18e, 0x0e420001, /* 0e420001 //C 0f18e ld	r4,r2,1		; addr */
0x0000f18f, 0x0e520002, /* 0e520002 //C 0f18f ld	r5,r2,2		; value */
0x0000f190, 0x024b0000, /* 024b0000 //C 0f190 sz 	r4 */
0x0000f191, 0x11f2f15d, /* 11f2f15d //C 0f191 jz	m_ret */
0x0000f192, 0x00000400, /* 00000400 //C 0f192 mov	r0,r4 */
0x0000f193, 0x0400f41b, /* 0400f41b //C 0f193 call	htoi */
0x0000f194, 0x00400100, /* 00400100 //C 0f194 mov	r4,r1 */
0x0000f195, 0x31f2f199, /* 31f2f199 //C 0f195 C1 jmp	sm_addr_ok */
0x0000f196, 0x0102f15f, /* 0102f15f //C 0f196 mvzl	r0,m_err_addr */
0x0000f197, 0x0400f465, /* 0400f465 //C 0f197 call	printsnl */
0x0000f198, 0x01f2f1ad, /* 01f2f1ad //C 0f198 jmp	sm_ret */
0x0000f199, 0x025b0000, /* 025b0000 //C 0f199 sz	r5 */
0x0000f19a, 0x11f2f1ad, /* 11f2f1ad //C 0f19a jz	sm_ret */
0x0000f19b, 0x0132f000, /* 0132f000 //C 0f19b mvzl	r3,the_begin */
0x0000f19c, 0x00380400, /* 00380400 //C 0f19c cmp	r3,r4 */
0x0000f19d, 0x91f2f1a5, /* 91f2f1a5 //C 0f19d HI jmp	sm_addrv_ok */
0x0000f19e, 0x0132f761, /* 0132f761 //C 0f19e mvzl	r3,the_end */
0x0000f19f, 0x00380400, /* 00380400 //C 0f19f cmp	r3,r4 */
0x0000f1a0, 0x91f2f1a2, /* 91f2f1a2 //C 0f1a0 HI jmp	sm_addrv_nok */
0x0000f1a1, 0x01f2f1a5, /* 01f2f1a5 //C 0f1a1 jmp	sm_addrv_ok */
0x0000f1a2, 0x0102f16d, /* 0102f16d //C 0f1a2 mvzl	r0,m_err_addrv */
0x0000f1a3, 0x0400f465, /* 0400f465 //C 0f1a3 call	printsnl */
0x0000f1a4, 0x01f2f1ad, /* 01f2f1ad //C 0f1a4 jmp	sm_ret */
0x0000f1a5, 0x00000500, /* 00000500 //C 0f1a5 mov	r0,r5 */
0x0000f1a6, 0x0400f41b, /* 0400f41b //C 0f1a6 call	htoi */
0x0000f1a7, 0x00500100, /* 00500100 //C 0f1a7 mov	r5,r1 */
0x0000f1a8, 0x31f2f1ac, /* 31f2f1ac //C 0f1a8 C1 jmp	sm_value_ok */
0x0000f1a9, 0x0102f17f, /* 0102f17f //C 0f1a9 mvzl	r0,m_err_value */
0x0000f1aa, 0x0400f465, /* 0400f465 //C 0f1aa call	printsnl */
0x0000f1ab, 0x01f2f1ad, /* 01f2f1ad //C 0f1ab jmp	sm_ret */
0x0000f1ac, 0x0c540000, /* 0c540000 //C 0f1ac st	r5,r4 */
0x0000f1ad, 0x0fed0000, /* 0fed0000 //C 0f1ad pop	lr */
0x0000f1ae, 0x00f00e00, /* 00f00e00 //C 0f1ae ret */
0x0000f1af, 0x0ded0000, /* 0ded0000 //C 0f1af push	lr */
0x0000f1b0, 0x0122f4fa, /* 0122f4fa //C 0f1b0 mvzl	r2,words */
0x0000f1b1, 0x0e020001, /* 0e020001 //C 0f1b1 ld	r0,r2,1		; start address */
0x0000f1b2, 0x0400f41b, /* 0400f41b //C 0f1b2 call	htoi */
0x0000f1b3, 0x00300100, /* 00300100 //C 0f1b3 mov	r3,r1 */
0x0000f1b4, 0x0e020002, /* 0e020002 //C 0f1b4 ld	r0,r2,2		; end address */
0x0000f1b5, 0x020b0000, /* 020b0000 //C 0f1b5 sz	r0 */
0x0000f1b6, 0x21f2f1ba, /* 21f2f1ba //C 0f1b6 jnz	d_end_ok */
0x0000f1b7, 0x00400300, /* 00400300 //C 0f1b7 mov	r4,r3 */
0x0000f1b8, 0x01440010, /* 01440010 //C 0f1b8 add	r4,0x10 */
0x0000f1b9, 0x01f2f1bc, /* 01f2f1bc //C 0f1b9 jmp	d_chk_end */
0x0000f1ba, 0x0400f41b, /* 0400f41b //C 0f1ba call	htoi */
0x0000f1bb, 0x00400100, /* 00400100 //C 0f1bb mov	r4,r1 */
0x0000f1bc, 0x00380400, /* 00380400 //C 0f1bc cmp	r3,r4		; check if start>end */
0x0000f1bd, 0x91f2f1cc, /* 91f2f1cc //C 0f1bd HI jmp d_bad */
0x0000f1be, 0x00000300, /* 00000300 //C 0f1be mov	r0,r3		; print address */
0x0000f1bf, 0x01120004, /* 01120004 //C 0f1bf mvzl	r1,4 */
0x0000f1c0, 0x0400f46d, /* 0400f46d //C 0f1c0 call	print_vhex */
0x0000f1c1, 0x01020020, /* 01020020 //C 0f1c1 mvzl	r0,0x20		; print one space */
0x0000f1c2, 0x0400f44a, /* 0400f44a //C 0f1c2 call	putchar */
0x0000f1c3, 0x0e030000, /* 0e030000 //C 0f1c3 ld	r0,r3		; load data */
0x0000f1c4, 0x01120004, /* 01120004 //C 0f1c4 mvzl	r1,4		; print data */
0x0000f1c5, 0x0400f46d, /* 0400f46d //C 0f1c5 call	print_vhex */
0x0000f1c6, 0x0102000a, /* 0102000a //C 0f1c6 mvzl	r0,LF		; print new line */
0x0000f1c7, 0x0400f44a, /* 0400f44a //C 0f1c7 call	putchar */
0x0000f1c8, 0x00380400, /* 00380400 //C 0f1c8 cmp	r3,r4 */
0x0000f1c9, 0x11f2f1ce, /* 11f2f1ce //C 0f1c9 jz	d_ret */
0x0000f1ca, 0x01340001, /* 01340001 //C 0f1ca add	r3,1 */
0x0000f1cb, 0x01f2f1be, /* 01f2f1be //C 0f1cb jmp	d_cyc */
0x0000f1cc, 0x0102f1d0, /* 0102f1d0 //C 0f1cc mvzl	r0,d_err_bad */
0x0000f1cd, 0x0400f465, /* 0400f465 //C 0f1cd call	printsnl */
0x0000f1ce, 0x0fed0000, /* 0fed0000 //C 0f1ce pop	lr */
0x0000f1cf, 0x00f00e00, /* 00f00e00 //C 0f1cf ret */
0x0000f1d0, 0x00000057, /* 00000057 //C 0f1d0 db	87 */
0x0000f1d1, 0x00000072, /* 00000072 //C 0f1d1 db	114 */
0x0000f1d2, 0x0000006f, /* 0000006f //C 0f1d2 db	111 */
0x0000f1d3, 0x0000006e, /* 0000006e //C 0f1d3 db	110 */
0x0000f1d4, 0x00000067, /* 00000067 //C 0f1d4 db	103 */
0x0000f1d5, 0x00000020, /* 00000020 //C 0f1d5 db	32 */
0x0000f1d6, 0x00000065, /* 00000065 //C 0f1d6 db	101 */
0x0000f1d7, 0x0000006e, /* 0000006e //C 0f1d7 db	110 */
0x0000f1d8, 0x00000064, /* 00000064 //C 0f1d8 db	100 */
0x0000f1d9, 0x00000020, /* 00000020 //C 0f1d9 db	32 */
0x0000f1da, 0x00000061, /* 00000061 //C 0f1da db	97 */
0x0000f1db, 0x00000064, /* 00000064 //C 0f1db db	100 */
0x0000f1dc, 0x00000064, /* 00000064 //C 0f1dc db	100 */
0x0000f1dd, 0x00000072, /* 00000072 //C 0f1dd db	114 */
0x0000f1de, 0x00000065, /* 00000065 //C 0f1de db	101 */
0x0000f1df, 0x00000073, /* 00000073 //C 0f1df db	115 */
0x0000f1e0, 0x00000073, /* 00000073 //C 0f1e0 db	115 */
0x0000f1e1, 0x00000000, /* 00000000 //C 0f1e1 db */
0x0000f1e2, 0x00f00e00, /* 00f00e00 //C 0f1e2 ret */
0x0000f1e3, 0x0ded0000, /* 0ded0000 //C 0f1e3 push	lr */
0x0000f1e4, 0x01a20000, /* 01a20000 //C 0f1e4 mvzl	r10,0		; state (nr of words) */
0x0000f1e5, 0x01820000, /* 01820000 //C 0f1e5 mvzl	r8,0		; value */
0x0000f1e6, 0x0162003f, /* 0162003f //C 0f1e6 mvzl	r6,'?'		; Record type */
0x0000f1e7, 0x0400f43f, /* 0400f43f //C 0f1e7 call	check_uart */
0x0000f1e8, 0x41f2f1e7, /* 41f2f1e7 //C 0f1e8 C0 jmp	l_cyc */
0x0000f1e9, 0x0400f447, /* 0400f447 //C 0f1e9 call	read */
0x0000f1ea, 0x00b00000, /* 00b00000 //C 0f1ea mov	r11,r0		; Copy of char in R11 */
0x0000f1eb, 0x0108000a, /* 0108000a //C 0f1eb cmp	r0,10		; check EOL chars */
0x0000f1ec, 0x11f2f238, /* 11f2f238 //C 0f1ec jz	l_eol */
0x0000f1ed, 0x0108000d, /* 0108000d //C 0f1ed cmp	r0,13 */
0x0000f1ee, 0x11f2f238, /* 11f2f238 //C 0f1ee jz	l_eol */
0x0000f1ef, 0x01a80000, /* 01a80000 //C 0f1ef cmp	r10,0 */
0x0000f1f0, 0x21f2f200, /* 21f2f200 //C 0f1f0 jnz	l_no0 */
0x0000f1f1, 0x0400f3e7, /* 0400f3e7 //C 0f1f1 call	hexchar2value */
0x0000f1f2, 0x41f2f1fa, /* 41f2f1fa //C 0f1f2 C0 jmp	l_eof_0 */
0x0000f1f3, 0x02880000, /* 02880000 //C 0f1f3 shl	r8		; shift val(char) into value */
0x0000f1f4, 0x02880000, /* 02880000 //C 0f1f4 shl	r8 */
0x0000f1f5, 0x02880000, /* 02880000 //C 0f1f5 shl	r8 */
0x0000f1f6, 0x02880000, /* 02880000 //C 0f1f6 shl	r8 */
0x0000f1f7, 0x010b000f, /* 010b000f //C 0f1f7 btst	r0,0xf */
0x0000f1f8, 0x008d0000, /* 008d0000 //C 0f1f8 or	r8,r0 */
0x0000f1f9, 0x01f2f1e7, /* 01f2f1e7 //C 0f1f9 jmp	l_cyc */
0x0000f1fa, 0x01a20001, /* 01a20001 //C 0f1fa mvzl	r10,1		; state0 -> state1 */
0x0000f1fb, 0x0162003f, /* 0162003f //C 0f1fb mvzl	r6,'?'		; No //C yet */
0x0000f1fc, 0x01b8002f, /* 01b8002f //C 0f1fc cmp	r11,'/'		; is it start of // */
0x0000f1fd, 0x11720001, /* 11720001 //C 0f1fd z1 mvzl	r7,1		; Yes, first / found */
0x0000f1fe, 0x21720000, /* 21720000 //C 0f1fe z0 mvzl	r7,0		; No '/' yet */
0x0000f1ff, 0x01f2f1e7, /* 01f2f1e7 //C 0f1ff jmp	l_cyc */
0x0000f200, 0x01a80001, /* 01a80001 //C 0f200 cmp	r10,1 */
0x0000f201, 0x21f2f21b, /* 21f2f21b //C 0f201 jnz	l_no1 */
0x0000f202, 0x027b0000, /* 027b0000 //C 0f202 sz	r7 */
0x0000f203, 0x21f2f208, /* 21f2f208 //C 0f203 jnz	l_s1_2nd */
0x0000f204, 0x0108002f, /* 0108002f //C 0f204 cmp	r0,'/' */
0x0000f205, 0x21f2f1e7, /* 21f2f1e7 //C 0f205 jnz	l_cyc */
0x0000f206, 0x01720001, /* 01720001 //C 0f206 mvzl	r7,1 */
0x0000f207, 0x01f2f1e7, /* 01f2f1e7 //C 0f207 jmp	l_cyc */
0x0000f208, 0x0108002f, /* 0108002f //C 0f208 cmp	r0,'/' */
0x0000f209, 0x11f2f1e7, /* 11f2f1e7 //C 0f209 jz	l_cyc */
0x0000f20a, 0x01080043, /* 01080043 //C 0f20a cmp	r0,'C' */
0x0000f20b, 0x11f2f20e, /* 11f2f20e //C 0f20b jz	l_s1_C */
0x0000f20c, 0x01080049, /* 01080049 //C 0f20c cmp	r0,'I' */
0x0000f20d, 0x21f2f213, /* 21f2f213 //C 0f20d jnz	l_s1_noC */
0x0000f20e, 0x00600000, /* 00600000 //C 0f20e mov	r6,r0		; record type is in r0, store in r6 */
0x0000f20f, 0x01a20002, /* 01a20002 //C 0f20f mvzl	r10,2 */
0x0000f210, 0x01920000, /* 01920000 //C 0f210 mvzl	r9,0		; address= 0 */
0x0000f211, 0x01520000, /* 01520000 //C 0f211 mvzl	r5,0		; where we are in word: before */
0x0000f212, 0x01f2f1e7, /* 01f2f1e7 //C 0f212 jmp	l_cyc */
0x0000f213, 0x01080045, /* 01080045 //C 0f213 cmp	r0,'E' */
0x0000f214, 0x21f2f218, /* 21f2f218 //C 0f214 jnz	l_s1_noE */
0x0000f215, 0x01620045, /* 01620045 //C 0f215 mvzl	r6,'E' */
0x0000f216, 0x01a20003, /* 01a20003 //C 0f216 mvzl	r10,3 */
0x0000f217, 0x01f2f1e7, /* 01f2f1e7 //C 0f217 jmp	l_cyc */
0x0000f218, 0x0400f44a, /* 0400f44a //C 0f218 call	putchar		; print record type */
0x0000f219, 0x01a2000f, /* 01a2000f //C 0f219 mvzl	r10,0xf		; special state: skip everything */
0x0000f21a, 0x01f2f1e7, /* 01f2f1e7 //C 0f21a jmp	l_cyc */
0x0000f21b, 0x01a80002, /* 01a80002 //C 0f21b cmp	r10,2 */
0x0000f21c, 0x21f2f230, /* 21f2f230 //C 0f21c jnz	l_no2 */
0x0000f21d, 0x01580000, /* 01580000 //C 0f21d cmp	r5,0 */
0x0000f21e, 0x21f2f229, /* 21f2f229 //C 0f21e jnz	l_s2_no0 */
0x0000f21f, 0x0400f3e7, /* 0400f3e7 //C 0f21f call	hexchar2value */
0x0000f220, 0x41f2f1e7, /* 41f2f1e7 //C 0f220 C0 jmp	l_cyc */
0x0000f221, 0x01520001, /* 01520001 //C 0f221 mvzl	r5,1 */
0x0000f222, 0x02980000, /* 02980000 //C 0f222 shl	r9 */
0x0000f223, 0x02980000, /* 02980000 //C 0f223 shl	r9 */
0x0000f224, 0x02980000, /* 02980000 //C 0f224 shl	r9 */
0x0000f225, 0x02980000, /* 02980000 //C 0f225 shl	r9 */
0x0000f226, 0x010b000f, /* 010b000f //C 0f226 btst	r0,0xf */
0x0000f227, 0x009d0000, /* 009d0000 //C 0f227 or	r9,r0 */
0x0000f228, 0x01f2f1e7, /* 01f2f1e7 //C 0f228 jmp	l_cyc */
0x0000f229, 0x01580001, /* 01580001 //C 0f229 cmp	r5,1 */
0x0000f22a, 0x21f2f22f, /* 21f2f22f //C 0f22a jnz	l_s2_no1 */
0x0000f22b, 0x0400f3e7, /* 0400f3e7 //C 0f22b call	hexchar2value */
0x0000f22c, 0x31f2f222, /* 31f2f222 //C 0f22c C1 jmp	l_s2_got */
0x0000f22d, 0x01520002, /* 01520002 //C 0f22d mvzl	r5,2 */
0x0000f22e, 0x01f2f1e7, /* 01f2f1e7 //C 0f22e jmp	l_cyc */
0x0000f22f, 0x01f2f1e7, /* 01f2f1e7 //C 0f22f jmp	l_cyc */
0x0000f230, 0x01a80003, /* 01a80003 //C 0f230 cmp	r10,3 */
0x0000f231, 0x21f2f233, /* 21f2f233 //C 0f231 jnz	l_no3 */
0x0000f232, 0x01f2f1e7, /* 01f2f1e7 //C 0f232 jmp	l_cyc		; do nothing, just wait EOL */
0x0000f233, 0x01a8000f, /* 01a8000f //C 0f233 cmp	r10,0xf */
0x0000f234, 0x01f2f236, /* 01f2f236 //C 0f234 jmp	l_nof */
0x0000f235, 0x01f2f1e7, /* 01f2f1e7 //C 0f235 jmp	l_cyc		; just skip */
0x0000f236, 0x01f2f1e7, /* 01f2f1e7 //C 0f236 jmp	l_cyc */
0x0000f237, 0x01f2f250, /* 01f2f250 //C 0f237 jmp	l_ret */
0x0000f238, 0x01a80000, /* 01a80000 //C 0f238 cmp	r10,0		; in state0 */
0x0000f239, 0x11f2f24a, /* 11f2f24a //C 0f239 jz	l_back_to_0	; just restart */
0x0000f23a, 0x01a80001, /* 01a80001 //C 0f23a cmp	r10,1		; in state1 */
0x0000f23b, 0x11f2f24a, /* 11f2f24a //C 0f23b jz	l_back_to_0 	;l_cyc ;l_bad ; garbage */
0x0000f23c, 0x01a80002, /* 01a80002 //C 0f23c cmp	r10,2		; in state2 */
0x0000f23d, 0x11f2f244, /* 11f2f244 //C 0f23d jz	l_proc		; process record */
0x0000f23e, 0x01a80003, /* 01a80003 //C 0f23e cmp	r10,3		; in state3 */
0x0000f23f, 0x11f2f250, /* 11f2f250 //C 0f23f jz	l_ret		; eol in end record: finish */
0x0000f240, 0x01a8000f, /* 01a8000f //C 0f240 cmp	r10,0xf		; in state skip */
0x0000f241, 0x11f2f24a, /* 11f2f24a //C 0f241 jz	l_back_to_0	; reset state for new line */
0x0000f242, 0x01f2f1e7, /* 01f2f1e7 //C 0f242 jmp	l_cyc */
0x0000f243, 0x01f2f250, /* 01f2f250 //C 0f243 jmp	l_ret */
0x0000f244, 0x01680043, /* 01680043 //C 0f244 cmp	r6,'C'		; is it a C or I record? */
0x0000f245, 0x1c890000, /* 1c890000 //C 0f245 z st	r8,r9		; then store */
0x0000f246, 0x01680049, /* 01680049 //C 0f246 cmp	r6,'I' */
0x0000f247, 0x1c890000, /* 1c890000 //C 0f247 z st	r8,r9 */
0x0000f248, 0x00000600, /* 00000600 //C 0f248 mov	r0,r6		; echo record type */
0x0000f249, 0x0400f44a, /* 0400f44a //C 0f249 call	putchar */
0x0000f24a, 0x0102002e, /* 0102002e //C 0f24a mvzl	r0,'.' */
0x0000f24b, 0x0400f44a, /* 0400f44a //C 0f24b call	putchar */
0x0000f24c, 0x01a20000, /* 01a20000 //C 0f24c mvzl	r10,0 */
0x0000f24d, 0x01820000, /* 01820000 //C 0f24d mvzl	r8,0 */
0x0000f24e, 0x0162003f, /* 0162003f //C 0f24e mvzl	r6,'?' */
0x0000f24f, 0x01f2f1e7, /* 01f2f1e7 //C 0f24f jmp	l_cyc */
0x0000f250, 0x0102000a, /* 0102000a //C 0f250 mvzl	r0,LF */
0x0000f251, 0x0400f44a, /* 0400f44a //C 0f251 call	putchar */
0x0000f252, 0x0fed0000, /* 0fed0000 //C 0f252 pop	lr */
0x0000f253, 0x00f00e00, /* 00f00e00 //C 0f253 ret */
0x0000f254, 0x0ded0000, /* 0ded0000 //C 0f254 push	lr */
0x0000f255, 0x0122f4fa, /* 0122f4fa //C 0f255 mvzl	r2,words */
0x0000f256, 0x0e020001, /* 0e020001 //C 0f256 ld	r0,r2,1		; address */
0x0000f257, 0x020b0000, /* 020b0000 //C 0f257 sz	r0 */
0x0000f258, 0x11f2f27b, /* 11f2f27b //C 0f258 jz	g_no_addr */
0x0000f259, 0x0400f41b, /* 0400f41b //C 0f259 call	htoi */
0x0000f25a, 0x00b00100, /* 00b00100 //C 0f25a mov	r11,r1 */
0x0000f25b, 0x0102f28f, /* 0102f28f //C 0f25b mvzl	r0,d_msg_run */
0x0000f25c, 0x0400f451, /* 0400f451 //C 0f25c call	prints */
0x0000f25d, 0x00000b00, /* 00000b00 //C 0f25d mov	r0,r11 */
0x0000f25e, 0x01120004, /* 01120004 //C 0f25e mvzl	r1,4 */
0x0000f25f, 0x0400f46d, /* 0400f46d //C 0f25f call	print_vhex */
0x0000f260, 0x0102000a, /* 0102000a //C 0f260 mvzl	r0,LF */
0x0000f261, 0x0400f44a, /* 0400f44a //C 0f261 call	putchar */
0x0000f262, 0x06b0f511, /* 06b0f511 //C 0f262 st	r11,reg15 */
0x0000f263, 0x0122ff43, /* 0122ff43 //C 0f263 mvzl	r2,UART_TSTAT */
0x0000f264, 0x0e920000, /* 0e920000 //C 0f264 ld	r9,r2 */
0x0000f265, 0x019c0001, /* 019c0001 //C 0f265 test	r9,1 */
0x0000f266, 0x11f2f264, /* 11f2f264 //C 0f266 jz	g_wait_tc */
0x0000f267, 0x07000000, /* 07000000 //C 0f267 ld	r0,0 */
0x0000f268, 0x0600f500, /* 0600f500 //C 0f268 st	r0,called */
0x0000f269, 0x0700f512, /* 0700f512 //C 0f269 ld	r0,regf */
0x0000f26a, 0x020f0000, /* 020f0000 //C 0f26a setf	r0 */
0x0000f26b, 0x0700f502, /* 0700f502 //C 0f26b ld	r0,reg0 */
0x0000f26c, 0x0710f503, /* 0710f503 //C 0f26c ld	r1,reg1 */
0x0000f26d, 0x0720f504, /* 0720f504 //C 0f26d ld	r2,reg2 */
0x0000f26e, 0x0730f505, /* 0730f505 //C 0f26e ld	r3,reg3 */
0x0000f26f, 0x0740f506, /* 0740f506 //C 0f26f ld	r4,reg4 */
0x0000f270, 0x0750f507, /* 0750f507 //C 0f270 ld	r5,reg5 */
0x0000f271, 0x0760f508, /* 0760f508 //C 0f271 ld	r6,reg6 */
0x0000f272, 0x0770f509, /* 0770f509 //C 0f272 ld	r7,reg7 */
0x0000f273, 0x0780f50a, /* 0780f50a //C 0f273 ld	r8,reg8 */
0x0000f274, 0x0790f50b, /* 0790f50b //C 0f274 ld	r9,reg9 */
0x0000f275, 0x07a0f50c, /* 07a0f50c //C 0f275 ld	r10,reg10 */
0x0000f276, 0x07b0f50d, /* 07b0f50d //C 0f276 ld	r11,reg11 */
0x0000f277, 0x07c0f50e, /* 07c0f50e //C 0f277 ld	r12,reg12 */
0x0000f278, 0x07d0f50f, /* 07d0f50f //C 0f278 ld	r13,reg13 */
0x0000f279, 0x07e0f510, /* 07e0f510 //C 0f279 ld	r14,reg14 */
0x0000f27a, 0x07f0f511, /* 07f0f511 //C 0f27a ld	r15,reg15 */
0x0000f27b, 0x0700f500, /* 0700f500 //C 0f27b ld	r0,called */
0x0000f27c, 0x020b0000, /* 020b0000 //C 0f27c sz	r0 */
0x0000f27d, 0x11f2f280, /* 11f2f280 //C 0f27d jz	g_err */
0x0000f27e, 0x07b0f511, /* 07b0f511 //C 0f27e ld	r11,reg15 */
0x0000f27f, 0x01f2f25b, /* 01f2f25b //C 0f27f jmp	g_go11 */
0x0000f280, 0x0102f284, /* 0102f284 //C 0f280 mvzl	r0,g_err_addr */
0x0000f281, 0x0400f465, /* 0400f465 //C 0f281 call	printsnl */
0x0000f282, 0x0fed0000, /* 0fed0000 //C 0f282 pop	lr */
0x0000f283, 0x00f00e00, /* 00f00e00 //C 0f283 ret */
0x0000f284, 0x0000004e, /* 0000004e //C 0f284 db	78 */
0x0000f285, 0x0000006f, /* 0000006f //C 0f285 db	111 */
0x0000f286, 0x00000020, /* 00000020 //C 0f286 db	32 */
0x0000f287, 0x00000061, /* 00000061 //C 0f287 db	97 */
0x0000f288, 0x00000064, /* 00000064 //C 0f288 db	100 */
0x0000f289, 0x00000064, /* 00000064 //C 0f289 db	100 */
0x0000f28a, 0x00000072, /* 00000072 //C 0f28a db	114 */
0x0000f28b, 0x00000065, /* 00000065 //C 0f28b db	101 */
0x0000f28c, 0x00000073, /* 00000073 //C 0f28c db	115 */
0x0000f28d, 0x00000073, /* 00000073 //C 0f28d db	115 */
0x0000f28e, 0x00000000, /* 00000000 //C 0f28e db */
0x0000f28f, 0x00000052, /* 00000052 //C 0f28f db	82 */
0x0000f290, 0x00000075, /* 00000075 //C 0f290 db	117 */
0x0000f291, 0x0000006e, /* 0000006e //C 0f291 db	110 */
0x0000f292, 0x00000020, /* 00000020 //C 0f292 db	32 */
0x0000f293, 0x00000000, /* 00000000 //C 0f293 db */
0x0000f294, 0x0ded0000, /* 0ded0000 //C 0f294 push	lr */
0x0000f295, 0x0122f5a2, /* 0122f5a2 //C 0f295 mvzl	r2,helps */
0x0000f296, 0x01320000, /* 01320000 //C 0f296 mvzl	r3,0 */
0x0000f297, 0x0b038200, /* 0b038200 //C 0f297 ld	r0,r3+,r2	; pick a char */
0x0000f298, 0x020b0000, /* 020b0000 //C 0f298 sz	r0		; is it eos? */
0x0000f299, 0x21f2f29d, /* 21f2f29d //C 0f299 jnz	h_print */
0x0000f29a, 0x0b038200, /* 0b038200 //C 0f29a ld	r0,r3+,r2	; get first char of next string */
0x0000f29b, 0x020b0000, /* 020b0000 //C 0f29b sz	r0 */
0x0000f29c, 0x11f2f29f, /* 11f2f29f //C 0f29c jz	h_eof */
0x0000f29d, 0x0400f44a, /* 0400f44a //C 0f29d call	putchar */
0x0000f29e, 0x01f2f297, /* 01f2f297 //C 0f29e jmp	h_cyc */
0x0000f29f, 0x0fed0000, /* 0fed0000 //C 0f29f pop	lr */
0x0000f2a0, 0x00f00e00, /* 00f00e00 //C 0f2a0 ret */
0x0000f2a1, 0x0ded0000, /* 0ded0000 //C 0f2a1 push	lr */
0x0000f2a2, 0x0d1d0000, /* 0d1d0000 //C 0f2a2 push	r1 */
0x0000f2a3, 0x00100000, /* 00100000 //C 0f2a3 mov	r1,r0 */
0x0000f2a4, 0x01180010, /* 01180010 //C 0f2a4 cmp	r1,16		; go out if nr>16 */
0x0000f2a5, 0x91f2f2d9, /* 91f2f2d9 //C 0f2a5 HI jmp	prn_ret */
0x0000f2a6, 0x0118000f, /* 0118000f //C 0f2a6 cmp	r1,15		; nr=Flag? */
0x0000f2a7, 0xa1f2f2ae, /* a1f2f2ae //C 0f2a7 LS jmp	prn_015 */
0x0000f2a8, 0x01020046, /* 01020046 //C 0f2a8 mvzl	r0,'F' */
0x0000f2a9, 0x0400f44a, /* 0400f44a //C 0f2a9 call	putchar */
0x0000f2aa, 0x01020020, /* 01020020 //C 0f2aa mvzl	r0,32 */
0x0000f2ab, 0x0400f44a, /* 0400f44a //C 0f2ab call	putchar */
0x0000f2ac, 0x0400f44a, /* 0400f44a //C 0f2ac call	putchar */
0x0000f2ad, 0x01f2f2d9, /* 01f2f2d9 //C 0f2ad jmp	prn_ret */
0x0000f2ae, 0x0118000f, /* 0118000f //C 0f2ae cmp	r1,15 */
0x0000f2af, 0x21f2f2b7, /* 21f2f2b7 //C 0f2af jnz	prn_no15 */
0x0000f2b0, 0x0102f2b3, /* 0102f2b3 //C 0f2b0 mvzl	r0,prn_PC */
0x0000f2b1, 0x0400f451, /* 0400f451 //C 0f2b1 call	prints */
0x0000f2b2, 0x01f2f2d9, /* 01f2f2d9 //C 0f2b2 jmp	prn_ret */
0x0000f2b3, 0x00000050, /* 00000050 //C 0f2b3 db	80 */
0x0000f2b4, 0x00000043, /* 00000043 //C 0f2b4 db	67 */
0x0000f2b5, 0x00000020, /* 00000020 //C 0f2b5 db	32 */
0x0000f2b6, 0x00000000, /* 00000000 //C 0f2b6 db */
0x0000f2b7, 0x0118000e, /* 0118000e //C 0f2b7 cmp	r1,14 */
0x0000f2b8, 0x21f2f2c0, /* 21f2f2c0 //C 0f2b8 jnz	prn_no14 */
0x0000f2b9, 0x0102f2bc, /* 0102f2bc //C 0f2b9 mvzl	r0,prn_LR */
0x0000f2ba, 0x0400f451, /* 0400f451 //C 0f2ba call	prints */
0x0000f2bb, 0x01f2f2d9, /* 01f2f2d9 //C 0f2bb jmp	prn_ret */
0x0000f2bc, 0x0000004c, /* 0000004c //C 0f2bc db	76 */
0x0000f2bd, 0x00000052, /* 00000052 //C 0f2bd db	82 */
0x0000f2be, 0x00000020, /* 00000020 //C 0f2be db	32 */
0x0000f2bf, 0x00000000, /* 00000000 //C 0f2bf db */
0x0000f2c0, 0x0118000d, /* 0118000d //C 0f2c0 cmp	r1,13 */
0x0000f2c1, 0x21f2f2c9, /* 21f2f2c9 //C 0f2c1 jnz	prn_no13 */
0x0000f2c2, 0x0102f2c5, /* 0102f2c5 //C 0f2c2 mvzl	r0,prn_SP */
0x0000f2c3, 0x0400f451, /* 0400f451 //C 0f2c3 call	prints */
0x0000f2c4, 0x01f2f2d9, /* 01f2f2d9 //C 0f2c4 jmp	prn_ret */
0x0000f2c5, 0x00000053, /* 00000053 //C 0f2c5 db	83 */
0x0000f2c6, 0x00000050, /* 00000050 //C 0f2c6 db	80 */
0x0000f2c7, 0x00000020, /* 00000020 //C 0f2c7 db	32 */
0x0000f2c8, 0x00000000, /* 00000000 //C 0f2c8 db */
0x0000f2c9, 0x01020052, /* 01020052 //C 0f2c9 mvzl	r0,'R' */
0x0000f2ca, 0x0400f44a, /* 0400f44a //C 0f2ca call	putchar */
0x0000f2cb, 0x01180009, /* 01180009 //C 0f2cb cmp	r1,9 */
0x0000f2cc, 0x91f2f2d3, /* 91f2f2d3 //C 0f2cc HI jmp	prn_1015 */
0x0000f2cd, 0x00000100, /* 00000100 //C 0f2cd mov	r0,r1 */
0x0000f2ce, 0x01040030, /* 01040030 //C 0f2ce add	r0,'0' */
0x0000f2cf, 0x0400f44a, /* 0400f44a //C 0f2cf call	putchar */
0x0000f2d0, 0x01020020, /* 01020020 //C 0f2d0 mvzl	r0,32 */
0x0000f2d1, 0x0400f44a, /* 0400f44a //C 0f2d1 call	putchar */
0x0000f2d2, 0x01f2f2d9, /* 01f2f2d9 //C 0f2d2 jmp	prn_ret */
0x0000f2d3, 0x01020031, /* 01020031 //C 0f2d3 mvzl	r0,'1' */
0x0000f2d4, 0x0400f44a, /* 0400f44a //C 0f2d4 call	putchar */
0x0000f2d5, 0x00000100, /* 00000100 //C 0f2d5 mov	r0,r1 */
0x0000f2d6, 0x0106000a, /* 0106000a //C 0f2d6 sub	r0,10 */
0x0000f2d7, 0x01040030, /* 01040030 //C 0f2d7 add	r0,'0' */
0x0000f2d8, 0x0400f44a, /* 0400f44a //C 0f2d8 call	putchar */
0x0000f2d9, 0x0f1d0000, /* 0f1d0000 //C 0f2d9 pop	r1 */
0x0000f2da, 0x0fed0000, /* 0fed0000 //C 0f2da pop	lr */
0x0000f2db, 0x00f00e00, /* 00f00e00 //C 0f2db ret */
0x0000f2dc, 0x0ded0000, /* 0ded0000 //C 0f2dc push	lr */
0x0000f2dd, 0x0d1d0000, /* 0d1d0000 //C 0f2dd push	r1 */
0x0000f2de, 0x01080010, /* 01080010 //C 0f2de cmp	r0,16 */
0x0000f2df, 0x91f2f2e4, /* 91f2f2e4 //C 0f2df HI jmp	prv_ret */
0x0000f2e0, 0x0112f502, /* 0112f502 //C 0f2e0 mvzl	r1,reg0 */
0x0000f2e1, 0x0a010000, /* 0a010000 //C 0f2e1 ld	r0,r1,r0 */
0x0000f2e2, 0x01120004, /* 01120004 //C 0f2e2 mvzl	r1,4 */
0x0000f2e3, 0x0400f46d, /* 0400f46d //C 0f2e3 call	print_vhex */
0x0000f2e4, 0x0f1d0000, /* 0f1d0000 //C 0f2e4 pop	r1 */
0x0000f2e5, 0x0fed0000, /* 0fed0000 //C 0f2e5 pop	lr */
0x0000f2e6, 0x00f00e00, /* 00f00e00 //C 0f2e6 ret */
0x0000f2e7, 0x0ded0000, /* 0ded0000 //C 0f2e7 push	lr */
0x0000f2e8, 0x0dad0000, /* 0dad0000 //C 0f2e8 push	r10 */
0x0000f2e9, 0x0d0d0000, /* 0d0d0000 //C 0f2e9 push	r0 */
0x0000f2ea, 0x00000a00, /* 00000a00 //C 0f2ea mov	r0,r10 */
0x0000f2eb, 0x0400f2a1, /* 0400f2a1 //C 0f2eb call	print_reg_name */
0x0000f2ec, 0x01020020, /* 01020020 //C 0f2ec mvzl	r0,32 */
0x0000f2ed, 0x0400f44a, /* 0400f44a //C 0f2ed call	putchar */
0x0000f2ee, 0x00000a00, /* 00000a00 //C 0f2ee mov	r0,r10 */
0x0000f2ef, 0x0400f2dc, /* 0400f2dc //C 0f2ef call	print_reg_value */
0x0000f2f0, 0x0102000a, /* 0102000a //C 0f2f0 mvzl	r0,LF */
0x0000f2f1, 0x0400f44a, /* 0400f44a //C 0f2f1 call	putchar */
0x0000f2f2, 0x0f0d0000, /* 0f0d0000 //C 0f2f2 pop	r0 */
0x0000f2f3, 0x0fad0000, /* 0fad0000 //C 0f2f3 pop	r10 */
0x0000f2f4, 0x0fed0000, /* 0fed0000 //C 0f2f4 pop	lr */
0x0000f2f5, 0x00f00e00, /* 00f00e00 //C 0f2f5 ret */
0x0000f2f6, 0x0ded0000, /* 0ded0000 //C 0f2f6 push	lr */
0x0000f2f7, 0x0400f44a, /* 0400f44a //C 0f2f7 call	putchar */
0x0000f2f8, 0x001c0200, /* 001c0200 //C 0f2f8 test	r1,r2 */
0x0000f2f9, 0x21020031, /* 21020031 //C 0f2f9 NZ mvzl r0,'1' */
0x0000f2fa, 0x11020030, /* 11020030 //C 0f2fa Z  mvzl r0,'0' */
0x0000f2fb, 0x0400f44a, /* 0400f44a //C 0f2fb call	putchar */
0x0000f2fc, 0x00000300, /* 00000300 //C 0f2fc mov	r0,r3 */
0x0000f2fd, 0x0400f44a, /* 0400f44a //C 0f2fd call	putchar */
0x0000f2fe, 0x0fed0000, /* 0fed0000 //C 0f2fe pop	lr */
0x0000f2ff, 0x00f00e00, /* 00f00e00 //C 0f2ff ret */
0x0000f300, 0x0ded0000, /* 0ded0000 //C 0f300 push	lr */
0x0000f301, 0x0700f500, /* 0700f500 //C 0f301 ld	r0,called */
0x0000f302, 0x020b0000, /* 020b0000 //C 0f302 sz	r0 */
0x0000f303, 0x11f2f30c, /* 11f2f30c //C 0f303 jz	r_not_called */
0x0000f304, 0x0102f32a, /* 0102f32a //C 0f304 mvzl	r0,msg_r_called */
0x0000f305, 0x0400f451, /* 0400f451 //C 0f305 call	prints */
0x0000f306, 0x0700f511, /* 0700f511 //C 0f306 ld	r0,reg15 */
0x0000f307, 0x01120004, /* 01120004 //C 0f307 mvzl	r1,4 */
0x0000f308, 0x0400f46d, /* 0400f46d //C 0f308 call	print_vhex */
0x0000f309, 0x0102000a, /* 0102000a //C 0f309 mvzl	r0,LF */
0x0000f30a, 0x0400f44a, /* 0400f44a //C 0f30a call	putchar */
0x0000f30b, 0x01f2f30e, /* 01f2f30e //C 0f30b jmp	r_start */
0x0000f30c, 0x0102f340, /* 0102f340 //C 0f30c mvzl	r0,msg_r_notcalled */
0x0000f30d, 0x0400f451, /* 0400f451 //C 0f30d call	prints */
0x0000f30e, 0x01a20000, /* 01a20000 //C 0f30e mvzl	r10,0 */
0x0000f30f, 0x0400f2e7, /* 0400f2e7 //C 0f30f call	print_reg_name_value */
0x0000f310, 0x01a40001, /* 01a40001 //C 0f310 add	r10,1 */
0x0000f311, 0x01a80011, /* 01a80011 //C 0f311 cmp	r10,17 */
0x0000f312, 0x21f2f30f, /* 21f2f30f //C 0f312 jnz	r_cyc */
0x0000f313, 0x0710f512, /* 0710f512 //C 0f313 ld	r1,regf */
0x0000f314, 0x01320020, /* 01320020 //C 0f314 mvzl	r3,32 */
0x0000f315, 0x01020055, /* 01020055 //C 0f315 mvzl	r0,'U' */
0x0000f316, 0x01220020, /* 01220020 //C 0f316 mvzl	r2,0x20 */
0x0000f317, 0x0400f2f6, /* 0400f2f6 //C 0f317 call	print_flag */
0x0000f318, 0x01020050, /* 01020050 //C 0f318 mvzl	r0,'P' */
0x0000f319, 0x01220010, /* 01220010 //C 0f319 mvzl	r2,0x10 */
0x0000f31a, 0x0400f2f6, /* 0400f2f6 //C 0f31a call	print_flag */
0x0000f31b, 0x0102004f, /* 0102004f //C 0f31b mvzl	r0,'O' */
0x0000f31c, 0x01220008, /* 01220008 //C 0f31c mvzl	r2,8 */
0x0000f31d, 0x0400f2f6, /* 0400f2f6 //C 0f31d call	print_flag */
0x0000f31e, 0x0102005a, /* 0102005a //C 0f31e mvzl	r0,'Z' */
0x0000f31f, 0x01220004, /* 01220004 //C 0f31f mvzl	r2,4 */
0x0000f320, 0x0400f2f6, /* 0400f2f6 //C 0f320 call	print_flag */
0x0000f321, 0x01020043, /* 01020043 //C 0f321 mvzl	r0,'C' */
0x0000f322, 0x01220002, /* 01220002 //C 0f322 mvzl	r2,2 */
0x0000f323, 0x0400f2f6, /* 0400f2f6 //C 0f323 call	print_flag */
0x0000f324, 0x01020053, /* 01020053 //C 0f324 mvzl	r0,'S' */
0x0000f325, 0x01220001, /* 01220001 //C 0f325 mvzl	r2,1 */
0x0000f326, 0x0132000a, /* 0132000a //C 0f326 mvzl	r3,LF */
0x0000f327, 0x0400f2f6, /* 0400f2f6 //C 0f327 call	print_flag */
0x0000f328, 0x0fed0000, /* 0fed0000 //C 0f328 pop	lr */
0x0000f329, 0x00f00e00, /* 00f00e00 //C 0f329 ret */
0x0000f32a, 0x0000004d, /* 0000004d //C 0f32a db	77 */
0x0000f32b, 0x0000006f, /* 0000006f //C 0f32b db	111 */
0x0000f32c, 0x0000006e, /* 0000006e //C 0f32c db	110 */
0x0000f32d, 0x00000069, /* 00000069 //C 0f32d db	105 */
0x0000f32e, 0x00000074, /* 00000074 //C 0f32e db	116 */
0x0000f32f, 0x0000006f, /* 0000006f //C 0f32f db	111 */
0x0000f330, 0x00000072, /* 00000072 //C 0f330 db	114 */
0x0000f331, 0x00000020, /* 00000020 //C 0f331 db	32 */
0x0000f332, 0x00000063, /* 00000063 //C 0f332 db	99 */
0x0000f333, 0x00000061, /* 00000061 //C 0f333 db	97 */
0x0000f334, 0x0000006c, /* 0000006c //C 0f334 db	108 */
0x0000f335, 0x0000006c, /* 0000006c //C 0f335 db	108 */
0x0000f336, 0x00000065, /* 00000065 //C 0f336 db	101 */
0x0000f337, 0x00000064, /* 00000064 //C 0f337 db	100 */
0x0000f338, 0x00000020, /* 00000020 //C 0f338 db	32 */
0x0000f339, 0x00000066, /* 00000066 //C 0f339 db	102 */
0x0000f33a, 0x00000072, /* 00000072 //C 0f33a db	114 */
0x0000f33b, 0x0000006f, /* 0000006f //C 0f33b db	111 */
0x0000f33c, 0x0000006d, /* 0000006d //C 0f33c db	109 */
0x0000f33d, 0x0000003a, /* 0000003a //C 0f33d db	58 */
0x0000f33e, 0x00000020, /* 00000020 //C 0f33e db	32 */
0x0000f33f, 0x00000000, /* 00000000 //C 0f33f db */
0x0000f340, 0x0000004d, /* 0000004d //C 0f340 db	77 */
0x0000f341, 0x0000006f, /* 0000006f //C 0f341 db	111 */
0x0000f342, 0x0000006e, /* 0000006e //C 0f342 db	110 */
0x0000f343, 0x00000069, /* 00000069 //C 0f343 db	105 */
0x0000f344, 0x00000074, /* 00000074 //C 0f344 db	116 */
0x0000f345, 0x0000006f, /* 0000006f //C 0f345 db	111 */
0x0000f346, 0x00000072, /* 00000072 //C 0f346 db	114 */
0x0000f347, 0x00000020, /* 00000020 //C 0f347 db	32 */
0x0000f348, 0x0000006e, /* 0000006e //C 0f348 db	110 */
0x0000f349, 0x0000006f, /* 0000006f //C 0f349 db	111 */
0x0000f34a, 0x00000074, /* 00000074 //C 0f34a db	116 */
0x0000f34b, 0x00000020, /* 00000020 //C 0f34b db	32 */
0x0000f34c, 0x00000063, /* 00000063 //C 0f34c db	99 */
0x0000f34d, 0x00000061, /* 00000061 //C 0f34d db	97 */
0x0000f34e, 0x0000006c, /* 0000006c //C 0f34e db	108 */
0x0000f34f, 0x0000006c, /* 0000006c //C 0f34f db	108 */
0x0000f350, 0x00000065, /* 00000065 //C 0f350 db	101 */
0x0000f351, 0x00000064, /* 00000064 //C 0f351 db	100 */
0x0000f352, 0x00000020, /* 00000020 //C 0f352 db	32 */
0x0000f353, 0x00000062, /* 00000062 //C 0f353 db	98 */
0x0000f354, 0x00000079, /* 00000079 //C 0f354 db	121 */
0x0000f355, 0x00000020, /* 00000020 //C 0f355 db	32 */
0x0000f356, 0x00000075, /* 00000075 //C 0f356 db	117 */
0x0000f357, 0x00000073, /* 00000073 //C 0f357 db	115 */
0x0000f358, 0x00000065, /* 00000065 //C 0f358 db	101 */
0x0000f359, 0x00000072, /* 00000072 //C 0f359 db	114 */
0x0000f35a, 0x0000000a, /* 0000000a //C 0f35a db	10 */
0x0000f35b, 0x00000000, /* 00000000 //C 0f35b db */
0x0000f35c, 0x0ded0000, /* 0ded0000 //C 0f35c push	lr */
0x0000f35d, 0x07a0f501, /* 07a0f501 //C 0f35d ld	r10,nuof_reg	; Reg num is in R10 */
0x0000f35e, 0x01a80010, /* 01a80010 //C 0f35e cmp	r10,16 */
0x0000f35f, 0xa1f2f374, /* a1f2f374 //C 0f35f LS jmp	rx_nr_ok */
0x0000f360, 0x0102f363, /* 0102f363 //C 0f360 mvzl	r0,rx_err_nr */
0x0000f361, 0x0400f465, /* 0400f465 //C 0f361 call	printsnl */
0x0000f362, 0x01f2f390, /* 01f2f390 //C 0f362 jmp	rx_ret */
0x0000f363, 0x0000004e, /* 0000004e //C 0f363 db	78 */
0x0000f364, 0x0000006f, /* 0000006f //C 0f364 db	111 */
0x0000f365, 0x00000020, /* 00000020 //C 0f365 db	32 */
0x0000f366, 0x00000073, /* 00000073 //C 0f366 db	115 */
0x0000f367, 0x00000075, /* 00000075 //C 0f367 db	117 */
0x0000f368, 0x00000063, /* 00000063 //C 0f368 db	99 */
0x0000f369, 0x00000068, /* 00000068 //C 0f369 db	104 */
0x0000f36a, 0x00000020, /* 00000020 //C 0f36a db	32 */
0x0000f36b, 0x00000072, /* 00000072 //C 0f36b db	114 */
0x0000f36c, 0x00000065, /* 00000065 //C 0f36c db	101 */
0x0000f36d, 0x00000067, /* 00000067 //C 0f36d db	103 */
0x0000f36e, 0x00000069, /* 00000069 //C 0f36e db	105 */
0x0000f36f, 0x00000073, /* 00000073 //C 0f36f db	115 */
0x0000f370, 0x00000074, /* 00000074 //C 0f370 db	116 */
0x0000f371, 0x00000065, /* 00000065 //C 0f371 db	101 */
0x0000f372, 0x00000072, /* 00000072 //C 0f372 db	114 */
0x0000f373, 0x00000000, /* 00000000 //C 0f373 db */
0x0000f374, 0x0122f4fa, /* 0122f4fa //C 0f374 mvzl	r2,words */
0x0000f375, 0x0e420001, /* 0e420001 //C 0f375 ld	r4,r2,1		; get aof first parameter */
0x0000f376, 0x024b0000, /* 024b0000 //C 0f376 sz	r4		; is it NULL? */
0x0000f377, 0x11f2f38f, /* 11f2f38f //C 0f377 jz	rx_print */
0x0000f378, 0x00000400, /* 00000400 //C 0f378 mov	r0,r4 */
0x0000f379, 0x0400f41b, /* 0400f41b //C 0f379 call	htoi */
0x0000f37a, 0x00500100, /* 00500100 //C 0f37a mov	r5,r1		; Value is in R5 */
0x0000f37b, 0x31f2f38b, /* 31f2f38b //C 0f37b C1 jmp	rx_val_ok */
0x0000f37c, 0x0102f37f, /* 0102f37f //C 0f37c mvzl	r0,rx_err_val */
0x0000f37d, 0x0400f465, /* 0400f465 //C 0f37d call	printsnl */
0x0000f37e, 0x01f2f390, /* 01f2f390 //C 0f37e jmp	rx_ret */
0x0000f37f, 0x00000056, /* 00000056 //C 0f37f db	86 */
0x0000f380, 0x00000061, /* 00000061 //C 0f380 db	97 */
0x0000f381, 0x0000006c, /* 0000006c //C 0f381 db	108 */
0x0000f382, 0x00000075, /* 00000075 //C 0f382 db	117 */
0x0000f383, 0x00000065, /* 00000065 //C 0f383 db	101 */
0x0000f384, 0x00000020, /* 00000020 //C 0f384 db	32 */
0x0000f385, 0x00000065, /* 00000065 //C 0f385 db	101 */
0x0000f386, 0x00000072, /* 00000072 //C 0f386 db	114 */
0x0000f387, 0x00000072, /* 00000072 //C 0f387 db	114 */
0x0000f388, 0x0000006f, /* 0000006f //C 0f388 db	111 */
0x0000f389, 0x00000072, /* 00000072 //C 0f389 db	114 */
0x0000f38a, 0x00000000, /* 00000000 //C 0f38a db */
0x0000f38b, 0x01a80010, /* 01a80010 //C 0f38b cmp	r10,16		; Flag reg? */
0x0000f38c, 0x115f003f, /* 115f003f //C 0f38c EQ and	r5,0x3f */
0x0000f38d, 0x0102f502, /* 0102f502 //C 0f38d mvzl	r0,reg0 */
0x0000f38e, 0x08500a00, /* 08500a00 //C 0f38e st	r5,r0,r10 */
0x0000f38f, 0x0400f2e7, /* 0400f2e7 //C 0f38f call	print_reg_name_value */
0x0000f390, 0x0fed0000, /* 0fed0000 //C 0f390 pop	lr */
0x0000f391, 0x00f00e00, /* 00f00e00 //C 0f391 ret */
0x0000f392, 0x0102000d, /* 0102000d //C 0f392 mvzl	r0,13 */
0x0000f393, 0x0600f501, /* 0600f501 //C 0f393 st	r0,nuof_reg */
0x0000f394, 0x01f2f35c, /* 01f2f35c //C 0f394 jmp	cmd_rx */
0x0000f395, 0x0102000e, /* 0102000e //C 0f395 mvzl	r0,14 */
0x0000f396, 0x0600f501, /* 0600f501 //C 0f396 st	r0,nuof_reg */
0x0000f397, 0x01f2f35c, /* 01f2f35c //C 0f397 jmp	cmd_rx */
0x0000f398, 0x0102000f, /* 0102000f //C 0f398 mvzl	r0,15 */
0x0000f399, 0x0600f501, /* 0600f501 //C 0f399 st	r0,nuof_reg */
0x0000f39a, 0x01f2f35c, /* 01f2f35c //C 0f39a jmp	cmd_rx */
0x0000f39b, 0x01020010, /* 01020010 //C 0f39b mvzl	r0,16 */
0x0000f39c, 0x0600f501, /* 0600f501 //C 0f39c st	r0,nuof_reg */
0x0000f39d, 0x01f2f35c, /* 01f2f35c //C 0f39d jmp	cmd_rx */
0x0000f39e, 0x0d1d0000, /* 0d1d0000 //C 0f39e push	r1 */
0x0000f39f, 0x0d2d0000, /* 0d2d0000 //C 0f39f push	r2 */
0x0000f3a0, 0x0e210000, /* 0e210000 //C 0f3a0 ld	r2,r1 */
0x0000f3a1, 0x022b0000, /* 022b0000 //C 0f3a1 sz	r2 */
0x0000f3a2, 0x11f2f3a9, /* 11f2f3a9 //C 0f3a2 jz	strchr_no	; eof string found */
0x0000f3a3, 0x00280000, /* 00280000 //C 0f3a3 cmp	r2,r0		; compare */
0x0000f3a4, 0x11f2f3a7, /* 11f2f3a7 //C 0f3a4 jz	strchr_yes */
0x0000f3a5, 0x011a0001, /* 011a0001 //C 0f3a5 plus	r1,1		; go to next char */
0x0000f3a6, 0x01f2f3a0, /* 01f2f3a0 //C 0f3a6 jmp	strchr_cyc */
0x0000f3a7, 0x020c0000, /* 020c0000 //C 0f3a7 sec */
0x0000f3a8, 0x01f2f3ab, /* 01f2f3ab //C 0f3a8 jmp	strchr_ret */
0x0000f3a9, 0x01120000, /* 01120000 //C 0f3a9 mvzl	r1,0 */
0x0000f3aa, 0x020d0000, /* 020d0000 //C 0f3aa clc */
0x0000f3ab, 0x0f2d0000, /* 0f2d0000 //C 0f3ab pop	r2 */
0x0000f3ac, 0x0f1d0000, /* 0f1d0000 //C 0f3ac pop	r1 */
0x0000f3ad, 0x00f00e00, /* 00f00e00 //C 0f3ad ret */
0x0000f3ae, 0x0ded0000, /* 0ded0000 //C 0f3ae push	lr		; Save used registers */
0x0000f3af, 0x0d0d0000, /* 0d0d0000 //C 0f3af push	r0		; and input parameters */
0x0000f3b0, 0x0d1d0000, /* 0d1d0000 //C 0f3b0 push	r1 */
0x0000f3b1, 0x0d2d0000, /* 0d2d0000 //C 0f3b1 push	r2 */
0x0000f3b2, 0x0d4d0000, /* 0d4d0000 //C 0f3b2 push	r4 */
0x0000f3b3, 0x0d5d0000, /* 0d5d0000 //C 0f3b3 push	r5 */
0x0000f3b4, 0x0d6d0000, /* 0d6d0000 //C 0f3b4 push	r6 */
0x0000f3b5, 0x0e200000, /* 0e200000 //C 0f3b5 ld	r2,r0		; Got one-one char */
0x0000f3b6, 0x0e610000, /* 0e610000 //C 0f3b6 ld	r6,r1		; from two strings */
0x0000f3b7, 0x023b0000, /* 023b0000 //C 0f3b7 sz	r3		; Prepare for comparing */
0x0000f3b8, 0x112d0020, /* 112d0020 //C 0f3b8 Z1 or	r2,0x20		; if insensitive case */
0x0000f3b9, 0x023b0000, /* 023b0000 //C 0f3b9 sz	r3 */
0x0000f3ba, 0x116d0020, /* 116d0020 //C 0f3ba Z1 or	r6,0x20 */
0x0000f3bb, 0x00280600, /* 00280600 //C 0f3bb cmp	r2,r6		; compare them */
0x0000f3bc, 0x21f2f3ce, /* 21f2f3ce //C 0f3bc jnz	streq_no	; if differs: strings are not equal */
0x0000f3bd, 0x0e200000, /* 0e200000 //C 0f3bd ld	r2,r0		; Pick original (non-prepared) */
0x0000f3be, 0x0e610000, /* 0e610000 //C 0f3be ld	r6,r1		; chars to check EOS */
0x0000f3bf, 0x022b0000, /* 022b0000 //C 0f3bf sz	r2		; convert them to boolean */
0x0000f3c0, 0x21220001, /* 21220001 //C 0f3c0 Z0 mvzl	r2,1		; values in R2,R6 */
0x0000f3c1, 0x11220000, /* 11220000 //C 0f3c1 Z1 mvzl	r2,0		; and copy in R4,R5 */
0x0000f3c2, 0x00400200, /* 00400200 //C 0f3c2 mov	r4,r2 */
0x0000f3c3, 0x026b0000, /* 026b0000 //C 0f3c3 sz	r6 */
0x0000f3c4, 0x21620001, /* 21620001 //C 0f3c4 Z0 mvzl	r6,1 */
0x0000f3c5, 0x11620000, /* 11620000 //C 0f3c5 Z1 mvzl r6,0 */
0x0000f3c6, 0x00500600, /* 00500600 //C 0f3c6 mov	r5,r6 */
0x0000f3c7, 0x004d0500, /* 004d0500 //C 0f3c7 or	r4,r5		; if both are EOS: equal */
0x0000f3c8, 0x11f2f3d0, /* 11f2f3d0 //C 0f3c8 jz	streq_yes */
0x0000f3c9, 0x002f0600, /* 002f0600 //C 0f3c9 and 	r2,r6		; just one is EOS: not equal */
0x0000f3ca, 0x11f2f3ce, /* 11f2f3ce //C 0f3ca jz	streq_no */
0x0000f3cb, 0x010a0001, /* 010a0001 //C 0f3cb plus	r0,1		; non are EOS: go to check next char */
0x0000f3cc, 0x011a0001, /* 011a0001 //C 0f3cc plus	r1,1 */
0x0000f3cd, 0x01f2f3b5, /* 01f2f3b5 //C 0f3cd jmp	streq_cyc */
0x0000f3ce, 0x020d0000, /* 020d0000 //C 0f3ce clc			; False result */
0x0000f3cf, 0x01f2f3d1, /* 01f2f3d1 //C 0f3cf jmp	streq_ret */
0x0000f3d0, 0x020c0000, /* 020c0000 //C 0f3d0 sec			; True result */
0x0000f3d1, 0x0f6d0000, /* 0f6d0000 //C 0f3d1 pop	r6 */
0x0000f3d2, 0x0f5d0000, /* 0f5d0000 //C 0f3d2 pop	r5 */
0x0000f3d3, 0x0f4d0000, /* 0f4d0000 //C 0f3d3 pop	r4 */
0x0000f3d4, 0x0f2d0000, /* 0f2d0000 //C 0f3d4 pop	r2 */
0x0000f3d5, 0x0f1d0000, /* 0f1d0000 //C 0f3d5 pop	r1 */
0x0000f3d6, 0x0f0d0000, /* 0f0d0000 //C 0f3d6 pop	r0 */
0x0000f3d7, 0x0fed0000, /* 0fed0000 //C 0f3d7 pop	lr */
0x0000f3d8, 0x00f00e00, /* 00f00e00 //C 0f3d8 ret */
0x0000f3d9, 0x0ded0000, /* 0ded0000 //C 0f3d9 push	lr */
0x0000f3da, 0x0d3d0000, /* 0d3d0000 //C 0f3da push	r3 */
0x0000f3db, 0x01320001, /* 01320001 //C 0f3db mvzl	r3,1 */
0x0000f3dc, 0x0400f3ae, /* 0400f3ae //C 0f3dc call	str_cmp_eq */
0x0000f3dd, 0x0f3d0000, /* 0f3d0000 //C 0f3dd pop	r3 */
0x0000f3de, 0x0fed0000, /* 0fed0000 //C 0f3de pop	lr */
0x0000f3df, 0x00f00e00, /* 00f00e00 //C 0f3df ret */
0x0000f3e0, 0x0ded0000, /* 0ded0000 //C 0f3e0 push	lr */
0x0000f3e1, 0x0d3d0000, /* 0d3d0000 //C 0f3e1 push	r3 */
0x0000f3e2, 0x01320000, /* 01320000 //C 0f3e2 mvzl	r3,0 */
0x0000f3e3, 0x0400f3ae, /* 0400f3ae //C 0f3e3 call	str_cmp_eq */
0x0000f3e4, 0x0f3d0000, /* 0f3d0000 //C 0f3e4 pop	r3 */
0x0000f3e5, 0x0fed0000, /* 0fed0000 //C 0f3e5 pop	lr */
0x0000f3e6, 0x00f00e00, /* 00f00e00 //C 0f3e6 ret */
0x0000f3e7, 0x0108002f, /* 0108002f //C 0f3e7 cmp	r0,'/' */
0x0000f3e8, 0xa1f2f3fd, /* a1f2f3fd //C 0f3e8 LS jmp	hc2v_nok */
0x0000f3e9, 0x01080039, /* 01080039 //C 0f3e9 cmp	r0,'9' */
0x0000f3ea, 0xa1f2f3fa, /* a1f2f3fa //C 0f3ea LS jmp	hc2v_0_9 */
0x0000f3eb, 0x01080040, /* 01080040 //C 0f3eb cmp	r0,'@' */
0x0000f3ec, 0xa1f2f3fd, /* a1f2f3fd //C 0f3ec LS jmp	hc2v_nok */
0x0000f3ed, 0x01080046, /* 01080046 //C 0f3ed cmp	r0,'F' */
0x0000f3ee, 0xa1f2f3f7, /* a1f2f3f7 //C 0f3ee LS jmp	hc2v_A_F */
0x0000f3ef, 0x01080060, /* 01080060 //C 0f3ef cmp	r0,'`' */
0x0000f3f0, 0xa1f2f3fd, /* a1f2f3fd //C 0f3f0 LS jmp	hc2v_nok */
0x0000f3f1, 0x01080066, /* 01080066 //C 0f3f1 cmp	r0,'f' */
0x0000f3f2, 0xa1f2f3f4, /* a1f2f3f4 //C 0f3f2 LS jmp	hc2v_a_f */
0x0000f3f3, 0x01f2f3fd, /* 01f2f3fd //C 0f3f3 jmp	hc2v_nok */
0x0000f3f4, 0x0104000a, /* 0104000a //C 0f3f4 add	r0,10 */
0x0000f3f5, 0x01060061, /* 01060061 //C 0f3f5 sub	r0,'a' */
0x0000f3f6, 0x01f2f3fb, /* 01f2f3fb //C 0f3f6 jmp	hc2v_ok */
0x0000f3f7, 0x0104000a, /* 0104000a //C 0f3f7 add	r0,10 */
0x0000f3f8, 0x01060041, /* 01060041 //C 0f3f8 sub	r0,'A' */
0x0000f3f9, 0x01f2f3fb, /* 01f2f3fb //C 0f3f9 jmp	hc2v_ok */
0x0000f3fa, 0x01060030, /* 01060030 //C 0f3fa sub	r0,'0' */
0x0000f3fb, 0x020c0000, /* 020c0000 //C 0f3fb sec */
0x0000f3fc, 0x00f00e00, /* 00f00e00 //C 0f3fc ret */
0x0000f3fd, 0x020d0000, /* 020d0000 //C 0f3fd clc */
0x0000f3fe, 0x00f00e00, /* 00f00e00 //C 0f3fe ret */
0x0000f3ff, 0x0d1d0000, /* 0d1d0000 //C 0f3ff push	r1 */
0x0000f400, 0x010f000f, /* 010f000f //C 0f400 and	r0,0xf */
0x0000f401, 0x0112f405, /* 0112f405 //C 0f401 mvzl	r1,v2hc_table */
0x0000f402, 0x0a010000, /* 0a010000 //C 0f402 ld	r0,r1,r0 */
0x0000f403, 0x0f1d0000, /* 0f1d0000 //C 0f403 pop	r1 */
0x0000f404, 0x00f00e00, /* 00f00e00 //C 0f404 ret */
0x0000f405, 0x00000030, /* 00000030 //C 0f405 db	48 */
0x0000f406, 0x00000031, /* 00000031 //C 0f406 db	49 */
0x0000f407, 0x00000032, /* 00000032 //C 0f407 db	50 */
0x0000f408, 0x00000033, /* 00000033 //C 0f408 db	51 */
0x0000f409, 0x00000034, /* 00000034 //C 0f409 db	52 */
0x0000f40a, 0x00000035, /* 00000035 //C 0f40a db	53 */
0x0000f40b, 0x00000036, /* 00000036 //C 0f40b db	54 */
0x0000f40c, 0x00000037, /* 00000037 //C 0f40c db	55 */
0x0000f40d, 0x00000038, /* 00000038 //C 0f40d db	56 */
0x0000f40e, 0x00000039, /* 00000039 //C 0f40e db	57 */
0x0000f40f, 0x00000041, /* 00000041 //C 0f40f db	65 */
0x0000f410, 0x00000042, /* 00000042 //C 0f410 db	66 */
0x0000f411, 0x00000043, /* 00000043 //C 0f411 db	67 */
0x0000f412, 0x00000044, /* 00000044 //C 0f412 db	68 */
0x0000f413, 0x00000045, /* 00000045 //C 0f413 db	69 */
0x0000f414, 0x00000046, /* 00000046 //C 0f414 db	70 */
0x0000f415, 0x00000000, /* 00000000 //C 0f415 db */
0x0000f416, 0x0ded0000, /* 0ded0000 //C 0f416 push	lr */
0x0000f417, 0x0400f3ff, /* 0400f3ff //C 0f417 call	value2Hexchar */
0x0000f418, 0x010d0020, /* 010d0020 //C 0f418 or	r0,0x20 */
0x0000f419, 0x0fed0000, /* 0fed0000 //C 0f419 pop	lr */
0x0000f41a, 0x00f00e00, /* 00f00e00 //C 0f41a ret */
0x0000f41b, 0x0ded0000, /* 0ded0000 //C 0f41b push	lr */
0x0000f41c, 0x0d2d0000, /* 0d2d0000 //C 0f41c push	r2 */
0x0000f41d, 0x0d3d0000, /* 0d3d0000 //C 0f41d push	r3 */
0x0000f41e, 0x01120000, /* 01120000 //C 0f41e mvzl	r1,0		; return value */
0x0000f41f, 0x01320000, /* 01320000 //C 0f41f mvzl	r3,0		; index */
0x0000f420, 0x0b238000, /* 0b238000 //C 0f420 ld	r2,r3+,r0	; pick a char */
0x0000f421, 0x022b0000, /* 022b0000 //C 0f421 sz	r2		; check eof string */
0x0000f422, 0x11f2f438, /* 11f2f438 //C 0f422 jz	htoi_eos */
0x0000f423, 0x0128002e, /* 0128002e //C 0f423 cmp	r2,'.'		; skip separators */
0x0000f424, 0x11f2f420, /* 11f2f420 //C 0f424 jz	htoi_cyc */
0x0000f425, 0x0128005f, /* 0128005f //C 0f425 cmp	r2,'_' */
0x0000f426, 0x11f2f420, /* 11f2f420 //C 0f426 jz	htoi_cyc */
0x0000f427, 0x0128002d, /* 0128002d //C 0f427 cmp	r2,'-' */
0x0000f428, 0x11f2f420, /* 11f2f420 //C 0f428 jz	htoi_cyc */
0x0000f429, 0x0d0d0000, /* 0d0d0000 //C 0f429 push	r0 */
0x0000f42a, 0x00000200, /* 00000200 //C 0f42a mov	r0,r2 */
0x0000f42b, 0x0400f3e7, /* 0400f3e7 //C 0f42b call	hexchar2value */
0x0000f42c, 0x00200000, /* 00200000 //C 0f42c mov	r2,r0 */
0x0000f42d, 0x0f0d0000, /* 0f0d0000 //C 0f42d pop	r0 */
0x0000f42e, 0x41f2f436, /* 41f2f436 //C 0f42e C0 jmp	htoi_nok */
0x0000f42f, 0x02180000, /* 02180000 //C 0f42f shl	r1 */
0x0000f430, 0x02180000, /* 02180000 //C 0f430 shl	r1 */
0x0000f431, 0x02180000, /* 02180000 //C 0f431 shl	r1 */
0x0000f432, 0x02180000, /* 02180000 //C 0f432 shl	r1 */
0x0000f433, 0x012f000f, /* 012f000f //C 0f433 and	r2,0xf */
0x0000f434, 0x001d0200, /* 001d0200 //C 0f434 or	r1,r2 */
0x0000f435, 0x01f2f420, /* 01f2f420 //C 0f435 jmp	htoi_cyc */
0x0000f436, 0x020d0000, /* 020d0000 //C 0f436 clc */
0x0000f437, 0x01f2f43b, /* 01f2f43b //C 0f437 jmp	htoi_ret */
0x0000f438, 0x01380001, /* 01380001 //C 0f438 cmp	r3,1 */
0x0000f439, 0x920d0000, /* 920d0000 //C 0f439 HI clc */
0x0000f43a, 0xa20c0000, /* a20c0000 //C 0f43a LS sec */
0x0000f43b, 0x0f3d0000, /* 0f3d0000 //C 0f43b pop	r3 */
0x0000f43c, 0x0f2d0000, /* 0f2d0000 //C 0f43c pop	r2 */
0x0000f43d, 0x0fed0000, /* 0fed0000 //C 0f43d pop	lr */
0x0000f43e, 0x00f00e00, /* 00f00e00 //C 0f43e ret */
0x0000f43f, 0x0d0d0000, /* 0d0d0000 //C 0f43f push	r0 */
0x0000f440, 0x0700ff42, /* 0700ff42 //C 0f440 ld	r0,UART_RSTAT */
0x0000f441, 0x010c0001, /* 010c0001 //C 0f441 test	r0,1		; check if queue is not empty */
0x0000f442, 0x020d0000, /* 020d0000 //C 0f442 clc */
0x0000f443, 0x220c0000, /* 220c0000 //C 0f443 Z0 sec */
0x0000f444, 0x0f0d0000, /* 0f0d0000 //C 0f444 pop	r0 */
0x0000f445, 0x30f00e00, /* 30f00e00 //C 0f445 C1 ret */
0x0000f446, 0x00f00e00, /* 00f00e00 //C 0f446 ret */
0x0000f447, 0x0700ff40, /* 0700ff40 //C 0f447 ld	r0,UART_DR */
0x0000f448, 0x0600ff46, /* 0600ff46 //C 0f448 st	r0,UART_IRA */
0x0000f449, 0x00f00e00, /* 00f00e00 //C 0f449 ret */
0x0000f44a, 0x0d9d0000, /* 0d9d0000 //C 0f44a push	r9 */
0x0000f44b, 0x0790ff43, /* 0790ff43 //C 0f44b ld	r9,UART_TSTAT */
0x0000f44c, 0x019c0001, /* 019c0001 //C 0f44c test	r9,1 */
0x0000f44d, 0x11f2f44b, /* 11f2f44b //C 0f44d jz	wait_tc */
0x0000f44e, 0x0600ff40, /* 0600ff40 //C 0f44e st	r0,UART_DR */
0x0000f44f, 0x0f9d0000, /* 0f9d0000 //C 0f44f pop	r9 */
0x0000f450, 0x00f00e00, /* 00f00e00 //C 0f450 ret */
0x0000f451, 0x0ded0000, /* 0ded0000 //C 0f451 push	lr */
0x0000f452, 0x0d0d0000, /* 0d0d0000 //C 0f452 push	r0 */
0x0000f453, 0x0d3d0000, /* 0d3d0000 //C 0f453 push	r3 */
0x0000f454, 0x0d4d0000, /* 0d4d0000 //C 0f454 push	r4 */
0x0000f455, 0x0d2d0000, /* 0d2d0000 //C 0f455 push	r2 */
0x0000f456, 0x01420000, /* 01420000 //C 0f456 mvzl	r4,0 */
0x0000f457, 0x020b0000, /* 020b0000 //C 0f457 sz	r0 */
0x0000f458, 0x1102f53a, /* 1102f53a //C 0f458 Z1 mvzl	r0,null_str */
0x0000f459, 0x00200000, /* 00200000 //C 0f459 mov	r2,r0 */
0x0000f45a, 0x0b048200, /* 0b048200 //C 0f45a ld	r0,r4+,r2 */
0x0000f45b, 0x020b0000, /* 020b0000 //C 0f45b sz	r0 */
0x0000f45c, 0x11f2f45f, /* 11f2f45f //C 0f45c jz	prints_done */
0x0000f45d, 0x0400f44a, /* 0400f44a //C 0f45d call	putchar */
0x0000f45e, 0x01f2f45a, /* 01f2f45a //C 0f45e jmp	prints_go */
0x0000f45f, 0x0f2d0000, /* 0f2d0000 //C 0f45f pop	r2 */
0x0000f460, 0x0f4d0000, /* 0f4d0000 //C 0f460 pop	r4 */
0x0000f461, 0x0f3d0000, /* 0f3d0000 //C 0f461 pop	r3 */
0x0000f462, 0x0f0d0000, /* 0f0d0000 //C 0f462 pop	r0 */
0x0000f463, 0x0fed0000, /* 0fed0000 //C 0f463 pop	lr */
0x0000f464, 0x00f00e00, /* 00f00e00 //C 0f464 ret */
0x0000f465, 0x0ded0000, /* 0ded0000 //C 0f465 push	lr */
0x0000f466, 0x0400f451, /* 0400f451 //C 0f466 call	prints */
0x0000f467, 0x0d0d0000, /* 0d0d0000 //C 0f467 push	r0 */
0x0000f468, 0x0102000a, /* 0102000a //C 0f468 mvzl	r0,LF */
0x0000f469, 0x0400f44a, /* 0400f44a //C 0f469 call	putchar */
0x0000f46a, 0x0f0d0000, /* 0f0d0000 //C 0f46a pop	r0 */
0x0000f46b, 0x0fed0000, /* 0fed0000 //C 0f46b pop	lr */
0x0000f46c, 0x00f00e00, /* 00f00e00 //C 0f46c ret */
0x0000f46d, 0x0ded0000, /* 0ded0000 //C 0f46d push	lr */
0x0000f46e, 0x0d0d0000, /* 0d0d0000 //C 0f46e push	r0 */
0x0000f46f, 0x0d1d0000, /* 0d1d0000 //C 0f46f push	r1 */
0x0000f470, 0x0d2d0000, /* 0d2d0000 //C 0f470 push	r2 */
0x0000f471, 0x0d3d0000, /* 0d3d0000 //C 0f471 push	r3 */
0x0000f472, 0x0d4d0000, /* 0d4d0000 //C 0f472 push	r4 */
0x0000f473, 0x00300000, /* 00300000 //C 0f473 mov	r3,r0 */
0x0000f474, 0x01220000, /* 01220000 //C 0f474 mvzl	r2,0 */
0x0000f475, 0x01420001, /* 01420001 //C 0f475 mvzl	r4,1 */
0x0000f476, 0x01020000, /* 01020000 //C 0f476 mvzl	r0,0 */
0x0000f477, 0x02380000, /* 02380000 //C 0f477 shl	r3 */
0x0000f478, 0x02070000, /* 02070000 //C 0f478 rol	r0 */
0x0000f479, 0x02380000, /* 02380000 //C 0f479 shl	r3 */
0x0000f47a, 0x02070000, /* 02070000 //C 0f47a rol	r0 */
0x0000f47b, 0x02380000, /* 02380000 //C 0f47b shl	r3 */
0x0000f47c, 0x02070000, /* 02070000 //C 0f47c rol	r0 */
0x0000f47d, 0x02380000, /* 02380000 //C 0f47d shl	r3 */
0x0000f47e, 0x02070000, /* 02070000 //C 0f47e rol	r0 */
0x0000f47f, 0x0400f3ff, /* 0400f3ff //C 0f47f call	value2Hexchar */
0x0000f480, 0x0400f44a, /* 0400f44a //C 0f480 call	putchar */
0x0000f481, 0x01240001, /* 01240001 //C 0f481 add	r2,1 */
0x0000f482, 0x01280008, /* 01280008 //C 0f482 cmp	r2,8 */
0x0000f483, 0x11f2f48d, /* 11f2f48d //C 0f483 jz	print_vhex_ret */
0x0000f484, 0x021b0000, /* 021b0000 //C 0f484 sz	r1 */
0x0000f485, 0x11f2f48b, /* 11f2f48b //C 0f485 jz	print_vhex_nosep */
0x0000f486, 0x00480100, /* 00480100 //C 0f486 cmp	r4,r1 */
0x0000f487, 0x21f2f48b, /* 21f2f48b //C 0f487 jnz	print_vhex_nosep */
0x0000f488, 0x0102005f, /* 0102005f //C 0f488 mvzl	r0,'_' */
0x0000f489, 0x0400f44a, /* 0400f44a //C 0f489 call	putchar */
0x0000f48a, 0x01420000, /* 01420000 //C 0f48a mvzl	r4,0 */
0x0000f48b, 0x01440001, /* 01440001 //C 0f48b add	r4,1 */
0x0000f48c, 0x01f2f476, /* 01f2f476 //C 0f48c jmp	print_vhex_cyc */
0x0000f48d, 0x0f4d0000, /* 0f4d0000 //C 0f48d pop	r4 */
0x0000f48e, 0x0f3d0000, /* 0f3d0000 //C 0f48e pop	r3 */
0x0000f48f, 0x0f2d0000, /* 0f2d0000 //C 0f48f pop	r2 */
0x0000f490, 0x0f1d0000, /* 0f1d0000 //C 0f490 pop	r1 */
0x0000f491, 0x0f0d0000, /* 0f0d0000 //C 0f491 pop	r0 */
0x0000f492, 0x0fed0000, /* 0fed0000 //C 0f492 pop	lr */
0x0000f493, 0x00f00e00, /* 00f00e00 //C 0f493 ret */
0x0000f500, 0x00000000, /* 00000000 //C 0f500 dd	0 */
0x0000f501, 0x00000000, /* 00000000 //C 0f501 dd	0 */
0x0000f502, 0x00000000, /* 00000000 //C 0f502 dd	0 */
0x0000f503, 0x00000000, /* 00000000 //C 0f503 dd	0 */
0x0000f504, 0x00000000, /* 00000000 //C 0f504 dd	0 */
0x0000f505, 0x00000000, /* 00000000 //C 0f505 dd	0 */
0x0000f506, 0x00000000, /* 00000000 //C 0f506 dd	0 */
0x0000f507, 0x00000000, /* 00000000 //C 0f507 dd	0 */
0x0000f508, 0x00000000, /* 00000000 //C 0f508 dd	0 */
0x0000f509, 0x00000000, /* 00000000 //C 0f509 dd	0 */
0x0000f50a, 0x00000000, /* 00000000 //C 0f50a dd	0 */
0x0000f50b, 0x00000000, /* 00000000 //C 0f50b dd	0 */
0x0000f50c, 0x00000000, /* 00000000 //C 0f50c dd	0 */
0x0000f50d, 0x00000000, /* 00000000 //C 0f50d dd	0 */
0x0000f50e, 0x00000000, /* 00000000 //C 0f50e dd	0 */
0x0000f50f, 0x00000000, /* 00000000 //C 0f50f dd	0 */
0x0000f510, 0x00000000, /* 00000000 //C 0f510 dd	0 */
0x0000f511, 0x00000000, /* 00000000 //C 0f511 dd	0 */
0x0000f512, 0x00000000, /* 00000000 //C 0f512 dd	0 */
0x0000f513, 0x00000050, /* 00000050 //C 0f513 db	80 */
0x0000f514, 0x0000004d, /* 0000004d //C 0f514 db	77 */
0x0000f515, 0x0000006f, /* 0000006f //C 0f515 db	111 */
0x0000f516, 0x0000006e, /* 0000006e //C 0f516 db	110 */
0x0000f517, 0x00000069, /* 00000069 //C 0f517 db	105 */
0x0000f518, 0x00000074, /* 00000074 //C 0f518 db	116 */
0x0000f519, 0x0000006f, /* 0000006f //C 0f519 db	111 */
0x0000f51a, 0x00000072, /* 00000072 //C 0f51a db	114 */
0x0000f51b, 0x00000020, /* 00000020 //C 0f51b db	32 */
0x0000f51c, 0x00000076, /* 00000076 //C 0f51c db	118 */
0x0000f51d, 0x00000031, /* 00000031 //C 0f51d db	49 */
0x0000f51e, 0x0000002e, /* 0000002e //C 0f51e db	46 */
0x0000f51f, 0x00000030, /* 00000030 //C 0f51f db	48 */
0x0000f520, 0x00000000, /* 00000000 //C 0f520 db */
0x0000f521, 0x00000053, /* 00000053 //C 0f521 db	83 */
0x0000f522, 0x00000074, /* 00000074 //C 0f522 db	116 */
0x0000f523, 0x0000006f, /* 0000006f //C 0f523 db	111 */
0x0000f524, 0x00000070, /* 00000070 //C 0f524 db	112 */
0x0000f525, 0x00000020, /* 00000020 //C 0f525 db	32 */
0x0000f526, 0x00000061, /* 00000061 //C 0f526 db	97 */
0x0000f527, 0x00000074, /* 00000074 //C 0f527 db	116 */
0x0000f528, 0x0000003a, /* 0000003a //C 0f528 db	58 */
0x0000f529, 0x00000020, /* 00000020 //C 0f529 db	32 */
0x0000f52a, 0x00000000, /* 00000000 //C 0f52a db */
0x0000f52b, 0x00000008, /* 00000008 //C 0f52b db	8 */
0x0000f52c, 0x00000020, /* 00000020 //C 0f52c db	32 */
0x0000f52d, 0x00000008, /* 00000008 //C 0f52d db	8 */
0x0000f52e, 0x00000000, /* 00000000 //C 0f52e db	0 */
0x0000f52f, 0x0000003a, /* 0000003a //C 0f52f db	58 */
0x0000f530, 0x00000000, /* 00000000 //C 0f530 db */
0x0000f531, 0x00000020, /* 00000020 //C 0f531 db	32 */
0x0000f532, 0x0000003b, /* 0000003b //C 0f532 db	59 */
0x0000f533, 0x00000009, /* 00000009 //C 0f533 db	9 */
0x0000f534, 0x0000000b, /* 0000000b //C 0f534 db	11 */
0x0000f535, 0x0000002c, /* 0000002c //C 0f535 db	44 */
0x0000f536, 0x0000003d, /* 0000003d //C 0f536 db	61 */
0x0000f537, 0x0000005b, /* 0000005b //C 0f537 db	91 */
0x0000f538, 0x0000005d, /* 0000005d //C 0f538 db	93 */
0x0000f539, 0x00000000, /* 00000000 //C 0f539 db */
0x0000f53a, 0x00000028, /* 00000028 //C 0f53a db	40 */
0x0000f53b, 0x0000006e, /* 0000006e //C 0f53b db	110 */
0x0000f53c, 0x00000075, /* 00000075 //C 0f53c db	117 */
0x0000f53d, 0x0000006c, /* 0000006c //C 0f53d db	108 */
0x0000f53e, 0x0000006c, /* 0000006c //C 0f53e db	108 */
0x0000f53f, 0x00000029, /* 00000029 //C 0f53f db	41 */
0x0000f540, 0x00000000, /* 00000000 //C 0f540 db */
0x0000f541, 0x00000000, /* 00000000 //C 0f541 db	0 */
0x0000f542, 0x00000000, /* 00000000 //C 0f542 db	0 */
0x0000f54d, 0x0000f18b, /* 0000f18b //C 0f54d dd	cmd_sm */
0x0000f54e, 0x00000073, /* 00000073 //C 0f54e db	115 */
0x0000f54f, 0x0000006d, /* 0000006d //C 0f54f db	109 */
0x0000f550, 0x00000000, /* 00000000 //C 0f550 db */
0x0000f551, 0x0000f131, /* 0000f131 //C 0f551 dd	cmd_m */
0x0000f552, 0x0000006d, /* 0000006d //C 0f552 db	109 */
0x0000f553, 0x00000000, /* 00000000 //C 0f553 db */
0x0000f554, 0x0000f131, /* 0000f131 //C 0f554 dd	cmd_m */
0x0000f555, 0x0000006d, /* 0000006d //C 0f555 db	109 */
0x0000f556, 0x00000065, /* 00000065 //C 0f556 db	101 */
0x0000f557, 0x0000006d, /* 0000006d //C 0f557 db	109 */
0x0000f558, 0x00000000, /* 00000000 //C 0f558 db */
0x0000f559, 0x0000f1af, /* 0000f1af //C 0f559 dd	cmd_d */
0x0000f55a, 0x00000064, /* 00000064 //C 0f55a db	100 */
0x0000f55b, 0x00000000, /* 00000000 //C 0f55b db */
0x0000f55c, 0x0000f1af, /* 0000f1af //C 0f55c dd	cmd_d */
0x0000f55d, 0x00000064, /* 00000064 //C 0f55d db	100 */
0x0000f55e, 0x00000075, /* 00000075 //C 0f55e db	117 */
0x0000f55f, 0x0000006d, /* 0000006d //C 0f55f db	109 */
0x0000f560, 0x00000070, /* 00000070 //C 0f560 db	112 */
0x0000f561, 0x00000000, /* 00000000 //C 0f561 db */
0x0000f562, 0x0000f1e3, /* 0000f1e3 //C 0f562 dd	cmd_l */
0x0000f563, 0x0000006c, /* 0000006c //C 0f563 db	108 */
0x0000f564, 0x00000000, /* 00000000 //C 0f564 db */
0x0000f565, 0x0000f1e3, /* 0000f1e3 //C 0f565 dd	cmd_l */
0x0000f566, 0x0000006c, /* 0000006c //C 0f566 db	108 */
0x0000f567, 0x0000006f, /* 0000006f //C 0f567 db	111 */
0x0000f568, 0x00000061, /* 00000061 //C 0f568 db	97 */
0x0000f569, 0x00000064, /* 00000064 //C 0f569 db	100 */
0x0000f56a, 0x00000000, /* 00000000 //C 0f56a db */
0x0000f56b, 0x0000f254, /* 0000f254 //C 0f56b dd	cmd_g */
0x0000f56c, 0x00000067, /* 00000067 //C 0f56c db	103 */
0x0000f56d, 0x00000000, /* 00000000 //C 0f56d db */
0x0000f56e, 0x0000f254, /* 0000f254 //C 0f56e dd	cmd_g */
0x0000f56f, 0x00000067, /* 00000067 //C 0f56f db	103 */
0x0000f570, 0x0000006f, /* 0000006f //C 0f570 db	111 */
0x0000f571, 0x00000000, /* 00000000 //C 0f571 db */
0x0000f572, 0x0000f254, /* 0000f254 //C 0f572 dd	cmd_g */
0x0000f573, 0x00000072, /* 00000072 //C 0f573 db	114 */
0x0000f574, 0x00000075, /* 00000075 //C 0f574 db	117 */
0x0000f575, 0x0000006e, /* 0000006e //C 0f575 db	110 */
0x0000f576, 0x00000000, /* 00000000 //C 0f576 db */
0x0000f577, 0x0000f294, /* 0000f294 //C 0f577 dd	cmd_h */
0x0000f578, 0x0000003f, /* 0000003f //C 0f578 db	63 */
0x0000f579, 0x00000000, /* 00000000 //C 0f579 db */
0x0000f57a, 0x0000f294, /* 0000f294 //C 0f57a dd	cmd_h */
0x0000f57b, 0x00000068, /* 00000068 //C 0f57b db	104 */
0x0000f57c, 0x00000000, /* 00000000 //C 0f57c db */
0x0000f57d, 0x0000f294, /* 0000f294 //C 0f57d dd	cmd_h */
0x0000f57e, 0x00000068, /* 00000068 //C 0f57e db	104 */
0x0000f57f, 0x00000065, /* 00000065 //C 0f57f db	101 */
0x0000f580, 0x0000006c, /* 0000006c //C 0f580 db	108 */
0x0000f581, 0x00000070, /* 00000070 //C 0f581 db	112 */
0x0000f582, 0x00000000, /* 00000000 //C 0f582 db */
0x0000f583, 0x0000f300, /* 0000f300 //C 0f583 dd	cmd_r */
0x0000f584, 0x00000072, /* 00000072 //C 0f584 db	114 */
0x0000f585, 0x00000000, /* 00000000 //C 0f585 db */
0x0000f586, 0x0000f300, /* 0000f300 //C 0f586 dd	cmd_r */
0x0000f587, 0x00000072, /* 00000072 //C 0f587 db	114 */
0x0000f588, 0x00000065, /* 00000065 //C 0f588 db	101 */
0x0000f589, 0x00000067, /* 00000067 //C 0f589 db	103 */
0x0000f58a, 0x00000000, /* 00000000 //C 0f58a db */
0x0000f58b, 0x0000f300, /* 0000f300 //C 0f58b dd	cmd_r */
0x0000f58c, 0x00000072, /* 00000072 //C 0f58c db	114 */
0x0000f58d, 0x00000065, /* 00000065 //C 0f58d db	101 */
0x0000f58e, 0x00000067, /* 00000067 //C 0f58e db	103 */
0x0000f58f, 0x00000073, /* 00000073 //C 0f58f db	115 */
0x0000f590, 0x00000000, /* 00000000 //C 0f590 db */
0x0000f591, 0x0000f392, /* 0000f392 //C 0f591 dd	cmd_sp */
0x0000f592, 0x00000073, /* 00000073 //C 0f592 db	115 */
0x0000f593, 0x00000070, /* 00000070 //C 0f593 db	112 */
0x0000f594, 0x00000000, /* 00000000 //C 0f594 db */
0x0000f595, 0x0000f395, /* 0000f395 //C 0f595 dd	cmd_lr */
0x0000f596, 0x0000006c, /* 0000006c //C 0f596 db	108 */
0x0000f597, 0x00000072, /* 00000072 //C 0f597 db	114 */
0x0000f598, 0x00000000, /* 00000000 //C 0f598 db */
0x0000f599, 0x0000f398, /* 0000f398 //C 0f599 dd	cmd_pc */
0x0000f59a, 0x00000070, /* 00000070 //C 0f59a db	112 */
0x0000f59b, 0x00000063, /* 00000063 //C 0f59b db	99 */
0x0000f59c, 0x00000000, /* 00000000 //C 0f59c db */
0x0000f59d, 0x0000f39b, /* 0000f39b //C 0f59d dd	cmd_f */
0x0000f59e, 0x00000066, /* 00000066 //C 0f59e db	102 */
0x0000f59f, 0x00000000, /* 00000000 //C 0f59f db */
0x0000f5a0, 0x00000000, /* 00000000 //C 0f5a0 dd	0 */
0x0000f5a1, 0x00000000, /* 00000000 //C 0f5a1 dd	0 */
0x0000f5a2, 0x0000006d, /* 0000006d //C 0f5a2 db	109 */
0x0000f5a3, 0x0000005b, /* 0000005b //C 0f5a3 db	91 */
0x0000f5a4, 0x00000065, /* 00000065 //C 0f5a4 db	101 */
0x0000f5a5, 0x0000006d, /* 0000006d //C 0f5a5 db	109 */
0x0000f5a6, 0x0000005d, /* 0000005d //C 0f5a6 db	93 */
0x0000f5a7, 0x00000020, /* 00000020 //C 0f5a7 db	32 */
0x0000f5a8, 0x00000061, /* 00000061 //C 0f5a8 db	97 */
0x0000f5a9, 0x00000064, /* 00000064 //C 0f5a9 db	100 */
0x0000f5aa, 0x00000064, /* 00000064 //C 0f5aa db	100 */
0x0000f5ab, 0x00000072, /* 00000072 //C 0f5ab db	114 */
0x0000f5ac, 0x00000020, /* 00000020 //C 0f5ac db	32 */
0x0000f5ad, 0x0000005b, /* 0000005b //C 0f5ad db	91 */
0x0000f5ae, 0x00000076, /* 00000076 //C 0f5ae db	118 */
0x0000f5af, 0x00000061, /* 00000061 //C 0f5af db	97 */
0x0000f5b0, 0x0000006c, /* 0000006c //C 0f5b0 db	108 */
0x0000f5b1, 0x0000005d, /* 0000005d //C 0f5b1 db	93 */
0x0000f5b2, 0x00000020, /* 00000020 //C 0f5b2 db	32 */
0x0000f5b3, 0x00000020, /* 00000020 //C 0f5b3 db	32 */
0x0000f5b4, 0x00000047, /* 00000047 //C 0f5b4 db	71 */
0x0000f5b5, 0x00000065, /* 00000065 //C 0f5b5 db	101 */
0x0000f5b6, 0x00000074, /* 00000074 //C 0f5b6 db	116 */
0x0000f5b7, 0x0000002f, /* 0000002f //C 0f5b7 db	47 */
0x0000f5b8, 0x00000073, /* 00000073 //C 0f5b8 db	115 */
0x0000f5b9, 0x00000065, /* 00000065 //C 0f5b9 db	101 */
0x0000f5ba, 0x00000074, /* 00000074 //C 0f5ba db	116 */
0x0000f5bb, 0x00000020, /* 00000020 //C 0f5bb db	32 */
0x0000f5bc, 0x0000006d, /* 0000006d //C 0f5bc db	109 */
0x0000f5bd, 0x00000065, /* 00000065 //C 0f5bd db	101 */
0x0000f5be, 0x0000006d, /* 0000006d //C 0f5be db	109 */
0x0000f5bf, 0x0000006f, /* 0000006f //C 0f5bf db	111 */
0x0000f5c0, 0x00000072, /* 00000072 //C 0f5c0 db	114 */
0x0000f5c1, 0x00000079, /* 00000079 //C 0f5c1 db	121 */
0x0000f5c2, 0x0000000a, /* 0000000a //C 0f5c2 db	10 */
0x0000f5c3, 0x00000000, /* 00000000 //C 0f5c3 db */
0x0000f5c4, 0x00000064, /* 00000064 //C 0f5c4 db	100 */
0x0000f5c5, 0x0000005b, /* 0000005b //C 0f5c5 db	91 */
0x0000f5c6, 0x00000075, /* 00000075 //C 0f5c6 db	117 */
0x0000f5c7, 0x0000006d, /* 0000006d //C 0f5c7 db	109 */
0x0000f5c8, 0x00000070, /* 00000070 //C 0f5c8 db	112 */
0x0000f5c9, 0x0000005d, /* 0000005d //C 0f5c9 db	93 */
0x0000f5ca, 0x00000020, /* 00000020 //C 0f5ca db	32 */
0x0000f5cb, 0x00000073, /* 00000073 //C 0f5cb db	115 */
0x0000f5cc, 0x00000074, /* 00000074 //C 0f5cc db	116 */
0x0000f5cd, 0x00000061, /* 00000061 //C 0f5cd db	97 */
0x0000f5ce, 0x00000072, /* 00000072 //C 0f5ce db	114 */
0x0000f5cf, 0x00000074, /* 00000074 //C 0f5cf db	116 */
0x0000f5d0, 0x00000020, /* 00000020 //C 0f5d0 db	32 */
0x0000f5d1, 0x00000065, /* 00000065 //C 0f5d1 db	101 */
0x0000f5d2, 0x0000006e, /* 0000006e //C 0f5d2 db	110 */
0x0000f5d3, 0x00000064, /* 00000064 //C 0f5d3 db	100 */
0x0000f5d4, 0x00000020, /* 00000020 //C 0f5d4 db	32 */
0x0000f5d5, 0x00000020, /* 00000020 //C 0f5d5 db	32 */
0x0000f5d6, 0x00000044, /* 00000044 //C 0f5d6 db	68 */
0x0000f5d7, 0x00000075, /* 00000075 //C 0f5d7 db	117 */
0x0000f5d8, 0x0000006d, /* 0000006d //C 0f5d8 db	109 */
0x0000f5d9, 0x00000070, /* 00000070 //C 0f5d9 db	112 */
0x0000f5da, 0x00000020, /* 00000020 //C 0f5da db	32 */
0x0000f5db, 0x0000006d, /* 0000006d //C 0f5db db	109 */
0x0000f5dc, 0x00000065, /* 00000065 //C 0f5dc db	101 */
0x0000f5dd, 0x0000006d, /* 0000006d //C 0f5dd db	109 */
0x0000f5de, 0x0000006f, /* 0000006f //C 0f5de db	111 */
0x0000f5df, 0x00000072, /* 00000072 //C 0f5df db	114 */
0x0000f5e0, 0x00000079, /* 00000079 //C 0f5e0 db	121 */
0x0000f5e1, 0x00000020, /* 00000020 //C 0f5e1 db	32 */
0x0000f5e2, 0x00000063, /* 00000063 //C 0f5e2 db	99 */
0x0000f5e3, 0x0000006f, /* 0000006f //C 0f5e3 db	111 */
0x0000f5e4, 0x0000006e, /* 0000006e //C 0f5e4 db	110 */
0x0000f5e5, 0x00000074, /* 00000074 //C 0f5e5 db	116 */
0x0000f5e6, 0x00000065, /* 00000065 //C 0f5e6 db	101 */
0x0000f5e7, 0x0000006e, /* 0000006e //C 0f5e7 db	110 */
0x0000f5e8, 0x00000074, /* 00000074 //C 0f5e8 db	116 */
0x0000f5e9, 0x0000000a, /* 0000000a //C 0f5e9 db	10 */
0x0000f5ea, 0x00000000, /* 00000000 //C 0f5ea db */
0x0000f5eb, 0x0000006c, /* 0000006c //C 0f5eb db	108 */
0x0000f5ec, 0x0000005b, /* 0000005b //C 0f5ec db	91 */
0x0000f5ed, 0x0000006f, /* 0000006f //C 0f5ed db	111 */
0x0000f5ee, 0x00000061, /* 00000061 //C 0f5ee db	97 */
0x0000f5ef, 0x00000064, /* 00000064 //C 0f5ef db	100 */
0x0000f5f0, 0x0000005d, /* 0000005d //C 0f5f0 db	93 */
0x0000f5f1, 0x00000020, /* 00000020 //C 0f5f1 db	32 */
0x0000f5f2, 0x00000020, /* 00000020 //C 0f5f2 db	32 */
0x0000f5f3, 0x00000020, /* 00000020 //C 0f5f3 db	32 */
0x0000f5f4, 0x00000020, /* 00000020 //C 0f5f4 db	32 */
0x0000f5f5, 0x00000020, /* 00000020 //C 0f5f5 db	32 */
0x0000f5f6, 0x00000020, /* 00000020 //C 0f5f6 db	32 */
0x0000f5f7, 0x00000020, /* 00000020 //C 0f5f7 db	32 */
0x0000f5f8, 0x00000020, /* 00000020 //C 0f5f8 db	32 */
0x0000f5f9, 0x00000020, /* 00000020 //C 0f5f9 db	32 */
0x0000f5fa, 0x00000020, /* 00000020 //C 0f5fa db	32 */
0x0000f5fb, 0x00000020, /* 00000020 //C 0f5fb db	32 */
0x0000f5fc, 0x00000020, /* 00000020 //C 0f5fc db	32 */
0x0000f5fd, 0x0000004c, /* 0000004c //C 0f5fd db	76 */
0x0000f5fe, 0x0000006f, /* 0000006f //C 0f5fe db	111 */
0x0000f5ff, 0x00000061, /* 00000061 //C 0f5ff db	97 */
0x0000f600, 0x00000064, /* 00000064 //C 0f600 db	100 */
0x0000f601, 0x00000020, /* 00000020 //C 0f601 db	32 */
0x0000f602, 0x00000068, /* 00000068 //C 0f602 db	104 */
0x0000f603, 0x00000065, /* 00000065 //C 0f603 db	101 */
0x0000f604, 0x00000078, /* 00000078 //C 0f604 db	120 */
0x0000f605, 0x00000020, /* 00000020 //C 0f605 db	32 */
0x0000f606, 0x00000066, /* 00000066 //C 0f606 db	102 */
0x0000f607, 0x00000069, /* 00000069 //C 0f607 db	105 */
0x0000f608, 0x0000006c, /* 0000006c //C 0f608 db	108 */
0x0000f609, 0x00000065, /* 00000065 //C 0f609 db	101 */
0x0000f60a, 0x00000020, /* 00000020 //C 0f60a db	32 */
0x0000f60b, 0x00000074, /* 00000074 //C 0f60b db	116 */
0x0000f60c, 0x0000006f, /* 0000006f //C 0f60c db	111 */
0x0000f60d, 0x00000020, /* 00000020 //C 0f60d db	32 */
0x0000f60e, 0x0000006d, /* 0000006d //C 0f60e db	109 */
0x0000f60f, 0x00000065, /* 00000065 //C 0f60f db	101 */
0x0000f610, 0x0000006d, /* 0000006d //C 0f610 db	109 */
0x0000f611, 0x0000006f, /* 0000006f //C 0f611 db	111 */
0x0000f612, 0x00000072, /* 00000072 //C 0f612 db	114 */
0x0000f613, 0x00000079, /* 00000079 //C 0f613 db	121 */
0x0000f614, 0x0000000a, /* 0000000a //C 0f614 db	10 */
0x0000f615, 0x00000000, /* 00000000 //C 0f615 db */
0x0000f616, 0x00000067, /* 00000067 //C 0f616 db	103 */
0x0000f617, 0x0000005b, /* 0000005b //C 0f617 db	91 */
0x0000f618, 0x0000006f, /* 0000006f //C 0f618 db	111 */
0x0000f619, 0x0000005d, /* 0000005d //C 0f619 db	93 */
0x0000f61a, 0x0000007c, /* 0000007c //C 0f61a db	124 */
0x0000f61b, 0x00000072, /* 00000072 //C 0f61b db	114 */
0x0000f61c, 0x00000075, /* 00000075 //C 0f61c db	117 */
0x0000f61d, 0x0000006e, /* 0000006e //C 0f61d db	110 */
0x0000f61e, 0x00000020, /* 00000020 //C 0f61e db	32 */
0x0000f61f, 0x0000005b, /* 0000005b //C 0f61f db	91 */
0x0000f620, 0x00000061, /* 00000061 //C 0f620 db	97 */
0x0000f621, 0x00000064, /* 00000064 //C 0f621 db	100 */
0x0000f622, 0x00000064, /* 00000064 //C 0f622 db	100 */
0x0000f623, 0x00000072, /* 00000072 //C 0f623 db	114 */
0x0000f624, 0x0000005d, /* 0000005d //C 0f624 db	93 */
0x0000f625, 0x00000020, /* 00000020 //C 0f625 db	32 */
0x0000f626, 0x00000020, /* 00000020 //C 0f626 db	32 */
0x0000f627, 0x00000020, /* 00000020 //C 0f627 db	32 */
0x0000f628, 0x00000052, /* 00000052 //C 0f628 db	82 */
0x0000f629, 0x00000075, /* 00000075 //C 0f629 db	117 */
0x0000f62a, 0x0000006e, /* 0000006e //C 0f62a db	110 */
0x0000f62b, 0x00000020, /* 00000020 //C 0f62b db	32 */
0x0000f62c, 0x00000066, /* 00000066 //C 0f62c db	102 */
0x0000f62d, 0x00000072, /* 00000072 //C 0f62d db	114 */
0x0000f62e, 0x0000006f, /* 0000006f //C 0f62e db	111 */
0x0000f62f, 0x0000006d, /* 0000006d //C 0f62f db	109 */
0x0000f630, 0x00000020, /* 00000020 //C 0f630 db	32 */
0x0000f631, 0x00000061, /* 00000061 //C 0f631 db	97 */
0x0000f632, 0x00000064, /* 00000064 //C 0f632 db	100 */
0x0000f633, 0x00000064, /* 00000064 //C 0f633 db	100 */
0x0000f634, 0x00000072, /* 00000072 //C 0f634 db	114 */
0x0000f635, 0x00000065, /* 00000065 //C 0f635 db	101 */
0x0000f636, 0x00000073, /* 00000073 //C 0f636 db	115 */
0x0000f637, 0x00000073, /* 00000073 //C 0f637 db	115 */
0x0000f638, 0x0000000a, /* 0000000a //C 0f638 db	10 */
0x0000f639, 0x00000000, /* 00000000 //C 0f639 db */
0x0000f63a, 0x00000072, /* 00000072 //C 0f63a db	114 */
0x0000f63b, 0x0000005b, /* 0000005b //C 0f63b db	91 */
0x0000f63c, 0x00000065, /* 00000065 //C 0f63c db	101 */
0x0000f63d, 0x00000067, /* 00000067 //C 0f63d db	103 */
0x0000f63e, 0x0000005b, /* 0000005b //C 0f63e db	91 */
0x0000f63f, 0x00000073, /* 00000073 //C 0f63f db	115 */
0x0000f640, 0x0000005d, /* 0000005d //C 0f640 db	93 */
0x0000f641, 0x0000005d, /* 0000005d //C 0f641 db	93 */
0x0000f642, 0x00000020, /* 00000020 //C 0f642 db	32 */
0x0000f643, 0x00000020, /* 00000020 //C 0f643 db	32 */
0x0000f644, 0x00000020, /* 00000020 //C 0f644 db	32 */
0x0000f645, 0x00000020, /* 00000020 //C 0f645 db	32 */
0x0000f646, 0x00000020, /* 00000020 //C 0f646 db	32 */
0x0000f647, 0x00000020, /* 00000020 //C 0f647 db	32 */
0x0000f648, 0x00000020, /* 00000020 //C 0f648 db	32 */
0x0000f649, 0x00000020, /* 00000020 //C 0f649 db	32 */
0x0000f64a, 0x00000020, /* 00000020 //C 0f64a db	32 */
0x0000f64b, 0x00000020, /* 00000020 //C 0f64b db	32 */
0x0000f64c, 0x00000050, /* 00000050 //C 0f64c db	80 */
0x0000f64d, 0x00000072, /* 00000072 //C 0f64d db	114 */
0x0000f64e, 0x00000069, /* 00000069 //C 0f64e db	105 */
0x0000f64f, 0x0000006e, /* 0000006e //C 0f64f db	110 */
0x0000f650, 0x00000074, /* 00000074 //C 0f650 db	116 */
0x0000f651, 0x00000020, /* 00000020 //C 0f651 db	32 */
0x0000f652, 0x00000072, /* 00000072 //C 0f652 db	114 */
0x0000f653, 0x00000065, /* 00000065 //C 0f653 db	101 */
0x0000f654, 0x00000067, /* 00000067 //C 0f654 db	103 */
0x0000f655, 0x00000069, /* 00000069 //C 0f655 db	105 */
0x0000f656, 0x00000073, /* 00000073 //C 0f656 db	115 */
0x0000f657, 0x00000074, /* 00000074 //C 0f657 db	116 */
0x0000f658, 0x00000065, /* 00000065 //C 0f658 db	101 */
0x0000f659, 0x00000072, /* 00000072 //C 0f659 db	114 */
0x0000f65a, 0x00000073, /* 00000073 //C 0f65a db	115 */
0x0000f65b, 0x0000000a, /* 0000000a //C 0f65b db	10 */
0x0000f65c, 0x00000000, /* 00000000 //C 0f65c db */
0x0000f65d, 0x00000072, /* 00000072 //C 0f65d db	114 */
0x0000f65e, 0x00000058, /* 00000058 //C 0f65e db	88 */
0x0000f65f, 0x00000020, /* 00000020 //C 0f65f db	32 */
0x0000f660, 0x0000005b, /* 0000005b //C 0f660 db	91 */
0x0000f661, 0x00000076, /* 00000076 //C 0f661 db	118 */
0x0000f662, 0x00000061, /* 00000061 //C 0f662 db	97 */
0x0000f663, 0x0000006c, /* 0000006c //C 0f663 db	108 */
0x0000f664, 0x0000005d, /* 0000005d //C 0f664 db	93 */
0x0000f665, 0x00000020, /* 00000020 //C 0f665 db	32 */
0x0000f666, 0x00000020, /* 00000020 //C 0f666 db	32 */
0x0000f667, 0x00000020, /* 00000020 //C 0f667 db	32 */
0x0000f668, 0x00000020, /* 00000020 //C 0f668 db	32 */
0x0000f669, 0x00000020, /* 00000020 //C 0f669 db	32 */
0x0000f66a, 0x00000020, /* 00000020 //C 0f66a db	32 */
0x0000f66b, 0x00000020, /* 00000020 //C 0f66b db	32 */
0x0000f66c, 0x00000020, /* 00000020 //C 0f66c db	32 */
0x0000f66d, 0x00000020, /* 00000020 //C 0f66d db	32 */
0x0000f66e, 0x00000020, /* 00000020 //C 0f66e db	32 */
0x0000f66f, 0x00000047, /* 00000047 //C 0f66f db	71 */
0x0000f670, 0x00000065, /* 00000065 //C 0f670 db	101 */
0x0000f671, 0x00000074, /* 00000074 //C 0f671 db	116 */
0x0000f672, 0x0000002f, /* 0000002f //C 0f672 db	47 */
0x0000f673, 0x00000073, /* 00000073 //C 0f673 db	115 */
0x0000f674, 0x00000065, /* 00000065 //C 0f674 db	101 */
0x0000f675, 0x00000074, /* 00000074 //C 0f675 db	116 */
0x0000f676, 0x00000020, /* 00000020 //C 0f676 db	32 */
0x0000f677, 0x00000052, /* 00000052 //C 0f677 db	82 */
0x0000f678, 0x00000058, /* 00000058 //C 0f678 db	88 */
0x0000f679, 0x0000000a, /* 0000000a //C 0f679 db	10 */
0x0000f67a, 0x00000000, /* 00000000 //C 0f67a db */
0x0000f67b, 0x00000073, /* 00000073 //C 0f67b db	115 */
0x0000f67c, 0x00000070, /* 00000070 //C 0f67c db	112 */
0x0000f67d, 0x00000020, /* 00000020 //C 0f67d db	32 */
0x0000f67e, 0x0000005b, /* 0000005b //C 0f67e db	91 */
0x0000f67f, 0x00000076, /* 00000076 //C 0f67f db	118 */
0x0000f680, 0x00000061, /* 00000061 //C 0f680 db	97 */
0x0000f681, 0x0000006c, /* 0000006c //C 0f681 db	108 */
0x0000f682, 0x0000005d, /* 0000005d //C 0f682 db	93 */
0x0000f683, 0x00000020, /* 00000020 //C 0f683 db	32 */
0x0000f684, 0x00000020, /* 00000020 //C 0f684 db	32 */
0x0000f685, 0x00000020, /* 00000020 //C 0f685 db	32 */
0x0000f686, 0x00000020, /* 00000020 //C 0f686 db	32 */
0x0000f687, 0x00000020, /* 00000020 //C 0f687 db	32 */
0x0000f688, 0x00000020, /* 00000020 //C 0f688 db	32 */
0x0000f689, 0x00000020, /* 00000020 //C 0f689 db	32 */
0x0000f68a, 0x00000020, /* 00000020 //C 0f68a db	32 */
0x0000f68b, 0x00000020, /* 00000020 //C 0f68b db	32 */
0x0000f68c, 0x00000020, /* 00000020 //C 0f68c db	32 */
0x0000f68d, 0x00000047, /* 00000047 //C 0f68d db	71 */
0x0000f68e, 0x00000065, /* 00000065 //C 0f68e db	101 */
0x0000f68f, 0x00000074, /* 00000074 //C 0f68f db	116 */
0x0000f690, 0x0000002f, /* 0000002f //C 0f690 db	47 */
0x0000f691, 0x00000073, /* 00000073 //C 0f691 db	115 */
0x0000f692, 0x00000065, /* 00000065 //C 0f692 db	101 */
0x0000f693, 0x00000074, /* 00000074 //C 0f693 db	116 */
0x0000f694, 0x00000020, /* 00000020 //C 0f694 db	32 */
0x0000f695, 0x00000052, /* 00000052 //C 0f695 db	82 */
0x0000f696, 0x00000031, /* 00000031 //C 0f696 db	49 */
0x0000f697, 0x00000033, /* 00000033 //C 0f697 db	51 */
0x0000f698, 0x0000000a, /* 0000000a //C 0f698 db	10 */
0x0000f699, 0x00000000, /* 00000000 //C 0f699 db */
0x0000f69a, 0x0000006c, /* 0000006c //C 0f69a db	108 */
0x0000f69b, 0x00000072, /* 00000072 //C 0f69b db	114 */
0x0000f69c, 0x00000020, /* 00000020 //C 0f69c db	32 */
0x0000f69d, 0x0000005b, /* 0000005b //C 0f69d db	91 */
0x0000f69e, 0x00000076, /* 00000076 //C 0f69e db	118 */
0x0000f69f, 0x00000061, /* 00000061 //C 0f69f db	97 */
0x0000f6a0, 0x0000006c, /* 0000006c //C 0f6a0 db	108 */
0x0000f6a1, 0x0000005d, /* 0000005d //C 0f6a1 db	93 */
0x0000f6a2, 0x00000020, /* 00000020 //C 0f6a2 db	32 */
0x0000f6a3, 0x00000020, /* 00000020 //C 0f6a3 db	32 */
0x0000f6a4, 0x00000020, /* 00000020 //C 0f6a4 db	32 */
0x0000f6a5, 0x00000020, /* 00000020 //C 0f6a5 db	32 */
0x0000f6a6, 0x00000020, /* 00000020 //C 0f6a6 db	32 */
0x0000f6a7, 0x00000020, /* 00000020 //C 0f6a7 db	32 */
0x0000f6a8, 0x00000020, /* 00000020 //C 0f6a8 db	32 */
0x0000f6a9, 0x00000020, /* 00000020 //C 0f6a9 db	32 */
0x0000f6aa, 0x00000020, /* 00000020 //C 0f6aa db	32 */
0x0000f6ab, 0x00000020, /* 00000020 //C 0f6ab db	32 */
0x0000f6ac, 0x00000047, /* 00000047 //C 0f6ac db	71 */
0x0000f6ad, 0x00000065, /* 00000065 //C 0f6ad db	101 */
0x0000f6ae, 0x00000074, /* 00000074 //C 0f6ae db	116 */
0x0000f6af, 0x0000002f, /* 0000002f //C 0f6af db	47 */
0x0000f6b0, 0x00000073, /* 00000073 //C 0f6b0 db	115 */
0x0000f6b1, 0x00000065, /* 00000065 //C 0f6b1 db	101 */
0x0000f6b2, 0x00000074, /* 00000074 //C 0f6b2 db	116 */
0x0000f6b3, 0x00000020, /* 00000020 //C 0f6b3 db	32 */
0x0000f6b4, 0x00000052, /* 00000052 //C 0f6b4 db	82 */
0x0000f6b5, 0x00000031, /* 00000031 //C 0f6b5 db	49 */
0x0000f6b6, 0x00000034, /* 00000034 //C 0f6b6 db	52 */
0x0000f6b7, 0x0000000a, /* 0000000a //C 0f6b7 db	10 */
0x0000f6b8, 0x00000000, /* 00000000 //C 0f6b8 db */
0x0000f6b9, 0x00000070, /* 00000070 //C 0f6b9 db	112 */
0x0000f6ba, 0x00000063, /* 00000063 //C 0f6ba db	99 */
0x0000f6bb, 0x00000020, /* 00000020 //C 0f6bb db	32 */
0x0000f6bc, 0x0000005b, /* 0000005b //C 0f6bc db	91 */
0x0000f6bd, 0x00000076, /* 00000076 //C 0f6bd db	118 */
0x0000f6be, 0x00000061, /* 00000061 //C 0f6be db	97 */
0x0000f6bf, 0x0000006c, /* 0000006c //C 0f6bf db	108 */
0x0000f6c0, 0x0000005d, /* 0000005d //C 0f6c0 db	93 */
0x0000f6c1, 0x00000020, /* 00000020 //C 0f6c1 db	32 */
0x0000f6c2, 0x00000020, /* 00000020 //C 0f6c2 db	32 */
0x0000f6c3, 0x00000020, /* 00000020 //C 0f6c3 db	32 */
0x0000f6c4, 0x00000020, /* 00000020 //C 0f6c4 db	32 */
0x0000f6c5, 0x00000020, /* 00000020 //C 0f6c5 db	32 */
0x0000f6c6, 0x00000020, /* 00000020 //C 0f6c6 db	32 */
0x0000f6c7, 0x00000020, /* 00000020 //C 0f6c7 db	32 */
0x0000f6c8, 0x00000020, /* 00000020 //C 0f6c8 db	32 */
0x0000f6c9, 0x00000020, /* 00000020 //C 0f6c9 db	32 */
0x0000f6ca, 0x00000020, /* 00000020 //C 0f6ca db	32 */
0x0000f6cb, 0x00000047, /* 00000047 //C 0f6cb db	71 */
0x0000f6cc, 0x00000065, /* 00000065 //C 0f6cc db	101 */
0x0000f6cd, 0x00000074, /* 00000074 //C 0f6cd db	116 */
0x0000f6ce, 0x0000002f, /* 0000002f //C 0f6ce db	47 */
0x0000f6cf, 0x00000073, /* 00000073 //C 0f6cf db	115 */
0x0000f6d0, 0x00000065, /* 00000065 //C 0f6d0 db	101 */
0x0000f6d1, 0x00000074, /* 00000074 //C 0f6d1 db	116 */
0x0000f6d2, 0x00000020, /* 00000020 //C 0f6d2 db	32 */
0x0000f6d3, 0x00000052, /* 00000052 //C 0f6d3 db	82 */
0x0000f6d4, 0x00000031, /* 00000031 //C 0f6d4 db	49 */
0x0000f6d5, 0x00000035, /* 00000035 //C 0f6d5 db	53 */
0x0000f6d6, 0x0000000a, /* 0000000a //C 0f6d6 db	10 */
0x0000f6d7, 0x00000000, /* 00000000 //C 0f6d7 db */
0x0000f6d8, 0x00000066, /* 00000066 //C 0f6d8 db	102 */
0x0000f6d9, 0x00000020, /* 00000020 //C 0f6d9 db	32 */
0x0000f6da, 0x0000005b, /* 0000005b //C 0f6da db	91 */
0x0000f6db, 0x00000076, /* 00000076 //C 0f6db db	118 */
0x0000f6dc, 0x00000061, /* 00000061 //C 0f6dc db	97 */
0x0000f6dd, 0x0000006c, /* 0000006c //C 0f6dd db	108 */
0x0000f6de, 0x0000005d, /* 0000005d //C 0f6de db	93 */
0x0000f6df, 0x00000020, /* 00000020 //C 0f6df db	32 */
0x0000f6e0, 0x00000020, /* 00000020 //C 0f6e0 db	32 */
0x0000f6e1, 0x00000020, /* 00000020 //C 0f6e1 db	32 */
0x0000f6e2, 0x00000020, /* 00000020 //C 0f6e2 db	32 */
0x0000f6e3, 0x00000020, /* 00000020 //C 0f6e3 db	32 */
0x0000f6e4, 0x00000020, /* 00000020 //C 0f6e4 db	32 */
0x0000f6e5, 0x00000020, /* 00000020 //C 0f6e5 db	32 */
0x0000f6e6, 0x00000020, /* 00000020 //C 0f6e6 db	32 */
0x0000f6e7, 0x00000020, /* 00000020 //C 0f6e7 db	32 */
0x0000f6e8, 0x00000020, /* 00000020 //C 0f6e8 db	32 */
0x0000f6e9, 0x00000020, /* 00000020 //C 0f6e9 db	32 */
0x0000f6ea, 0x00000047, /* 00000047 //C 0f6ea db	71 */
0x0000f6eb, 0x00000065, /* 00000065 //C 0f6eb db	101 */
0x0000f6ec, 0x00000074, /* 00000074 //C 0f6ec db	116 */
0x0000f6ed, 0x0000002f, /* 0000002f //C 0f6ed db	47 */
0x0000f6ee, 0x00000073, /* 00000073 //C 0f6ee db	115 */
0x0000f6ef, 0x00000065, /* 00000065 //C 0f6ef db	101 */
0x0000f6f0, 0x00000074, /* 00000074 //C 0f6f0 db	116 */
0x0000f6f1, 0x00000020, /* 00000020 //C 0f6f1 db	32 */
0x0000f6f2, 0x00000066, /* 00000066 //C 0f6f2 db	102 */
0x0000f6f3, 0x0000006c, /* 0000006c //C 0f6f3 db	108 */
0x0000f6f4, 0x00000061, /* 00000061 //C 0f6f4 db	97 */
0x0000f6f5, 0x00000067, /* 00000067 //C 0f6f5 db	103 */
0x0000f6f6, 0x00000073, /* 00000073 //C 0f6f6 db	115 */
0x0000f6f7, 0x0000000a, /* 0000000a //C 0f6f7 db	10 */
0x0000f6f8, 0x00000000, /* 00000000 //C 0f6f8 db */
0x0000f6f9, 0x00000068, /* 00000068 //C 0f6f9 db	104 */
0x0000f6fa, 0x0000002c, /* 0000002c //C 0f6fa db	44 */
0x0000f6fb, 0x0000003f, /* 0000003f //C 0f6fb db	63 */
0x0000f6fc, 0x00000020, /* 00000020 //C 0f6fc db	32 */
0x0000f6fd, 0x00000020, /* 00000020 //C 0f6fd db	32 */
0x0000f6fe, 0x00000020, /* 00000020 //C 0f6fe db	32 */
0x0000f6ff, 0x00000020, /* 00000020 //C 0f6ff db	32 */
0x0000f700, 0x00000020, /* 00000020 //C 0f700 db	32 */
0x0000f701, 0x00000020, /* 00000020 //C 0f701 db	32 */
0x0000f702, 0x00000020, /* 00000020 //C 0f702 db	32 */
0x0000f703, 0x00000020, /* 00000020 //C 0f703 db	32 */
0x0000f704, 0x00000020, /* 00000020 //C 0f704 db	32 */
0x0000f705, 0x00000020, /* 00000020 //C 0f705 db	32 */
0x0000f706, 0x00000020, /* 00000020 //C 0f706 db	32 */
0x0000f707, 0x00000020, /* 00000020 //C 0f707 db	32 */
0x0000f708, 0x00000020, /* 00000020 //C 0f708 db	32 */
0x0000f709, 0x00000020, /* 00000020 //C 0f709 db	32 */
0x0000f70a, 0x00000020, /* 00000020 //C 0f70a db	32 */
0x0000f70b, 0x00000048, /* 00000048 //C 0f70b db	72 */
0x0000f70c, 0x00000065, /* 00000065 //C 0f70c db	101 */
0x0000f70d, 0x0000006c, /* 0000006c //C 0f70d db	108 */
0x0000f70e, 0x00000070, /* 00000070 //C 0f70e db	112 */
0x0000f70f, 0x0000000a, /* 0000000a //C 0f70f db	10 */
0x0000f710, 0x00000000, /* 00000000 //C 0f710 db */
0x0000f711, 0x00000000, /* 00000000 //C 0f711 dd	0 */
0x0000f753, 0x00000045, /* 00000045 //C 0f753 db	69 */
0x0000f754, 0x0000004f, /* 0000004f //C 0f754 db	79 */
0x0000f755, 0x00000046, /* 00000046 //C 0f755 db	70 */
0x0000f756, 0x00000020, /* 00000020 //C 0f756 db	32 */
0x0000f757, 0x00000050, /* 00000050 //C 0f757 db	80 */
0x0000f758, 0x0000004d, /* 0000004d //C 0f758 db	77 */
0x0000f759, 0x0000006f, /* 0000006f //C 0f759 db	111 */
0x0000f75a, 0x0000006e, /* 0000006e //C 0f75a db	110 */
0x0000f75b, 0x00000069, /* 00000069 //C 0f75b db	105 */
0x0000f75c, 0x00000074, /* 00000074 //C 0f75c db	116 */
0x0000f75d, 0x0000006f, /* 0000006f //C 0f75d db	111 */
0x0000f75e, 0x00000072, /* 00000072 //C 0f75e db	114 */
0x0000f75f, 0x0000000a, /* 0000000a //C 0f75f db	10 */
0x0000f760, 0x00000000, /* 00000000 //C 0f760 db */
0xffffffff, 0xffffffff
};
