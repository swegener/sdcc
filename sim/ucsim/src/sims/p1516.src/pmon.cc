#include "pmon.h"

t_mem pmon[]= {
0x00000000, 0x01f2f03c, /* 01f2f03c //C 00000 jmp	cold_start */
0x0000f000, 0x01f2f025, /* 01f2f025 //C 0f000 _f000:	jmp	callin */
0x0000f001, 0x01f2f016, /* 01f2f016 //C 0f001 _f001:	jmp	enter_by_uart */
0x0000f002, 0x00f00e00, /* 00f00e00 //C 0f002 _f002:	ret */
0x0000f003, 0x01f2f840, /* 01f2f840 //C 0f003 _f003:	jmp	isdigit */
0x0000f004, 0x01f2f82b, /* 01f2f82b //C 0f004 _f004:	jmp	dtoi */
0x0000f005, 0x01f2f03c, /* 01f2f03c //C 0f005 _f005:	jmp	cold_start */
0x0000f006, 0x01f2f39e, /* 01f2f39e //C 0f006 _f006:	jmp	strchr */
0x0000f007, 0x01f2f3db, /* 01f2f3db //C 0f007 _f007:	jmp	streq */
0x0000f008, 0x01f2f441, /* 01f2f441 //C 0f008 _f008:	jmp	check_uart */
0x0000f009, 0x01f2f3eb, /* 01f2f3eb //C 0f009 _f009:	jmp	hexchar2value */
0x0000f00a, 0x01f2f41a, /* 01f2f41a //C 0f00a _f00a:	jmp	value2hexchar */
0x0000f00b, 0x01f2f41e, /* 01f2f41e //C 0f00b _f00b:	jmp	htoi */
0x0000f00c, 0x01f2f3e3, /* 01f2f3e3 //C 0f00c _f00c:	jmp	strieq */
0x0000f00d, 0x01f2f449, /* 01f2f449 //C 0f00d _f00d:	jmp	read */
0x0000f00e, 0x01f2f44c, /* 01f2f44c //C 0f00e _f00e:	jmp	putchar */
0x0000f00f, 0x01f2f453, /* 01f2f453 //C 0f00f _f00f:	jmp	prints */
0x0000f010, 0x01f2f476, /* 01f2f476 //C 0f010 _f010:	jmp	printsnl */
0x0000f011, 0x01f2f47d, /* 01f2f47d //C 0f011 _f011:	jmp	print_vhex */
0x0000f012, 0x01f2f467, /* 01f2f467 //C 0f012 _f012:	jmp	pes */
0x0000f013, 0x01f2f4fb, /* 01f2f4fb //C 0f013 _f013:	jmp	printd */
0x0000f014, 0x01f2f500, /* 01f2f500 //C 0f014 _f014:	jmp	printf */
0x0000f015, 0x01f2f564, /* 01f2f564 //C 0f015 _f015:	jmp	pesf */
0x0000f016, 0x0d0d0000, /* 0d0d0000 //C 0f016 push	r0 */
0x0000f017, 0x020e0000, /* 020e0000 //C 0f017 getf	r0 */
0x0000f018, 0x0d0d0000, /* 0d0d0000 //C 0f018 push	r0 */
0x0000f019, 0x0700ff42, /* 0700ff42 //C 0f019 ld	r0,UART_RSTAT */
0x0000f01a, 0x010c0001, /* 010c0001 //C 0f01a test	r0,1 */
0x0000f01b, 0x21f2f020, /* 21f2f020 //C 0f01b jnz	ebu_callin */
0x0000f01c, 0x0f0d0000, /* 0f0d0000 //C 0f01c pop	r0 */
0x0000f01d, 0x020f0000, /* 020f0000 //C 0f01d setf	r0 */
0x0000f01e, 0x0f0d0000, /* 0f0d0000 //C 0f01e pop	r0 */
0x0000f01f, 0x00f00e00, /* 00f00e00 //C 0f01f ret */
0x0000f020, 0x0700ff40, /* 0700ff40 //C 0f020 ld	r0,UART_DR */
0x0000f021, 0x0f0d0000, /* 0f0d0000 //C 0f021 pop	r0 */
0x0000f022, 0x020f0000, /* 020f0000 //C 0f022 setf	r0 */
0x0000f023, 0x0f0d0000, /* 0f0d0000 //C 0f023 pop	r0 */
0x0000f024, 0x01f2f025, /* 01f2f025 //C 0f024 jmp	callin */
0x0000f025, 0x0600f5da, /* 0600f5da //C 0f025 st	r0,reg0 */
0x0000f026, 0x0610f5db, /* 0610f5db //C 0f026 st	r1,reg1 */
0x0000f027, 0x0620f5dc, /* 0620f5dc //C 0f027 st	r2,reg2 */
0x0000f028, 0x0630f5dd, /* 0630f5dd //C 0f028 st	r3,reg3 */
0x0000f029, 0x0640f5de, /* 0640f5de //C 0f029 st	r4,reg4 */
0x0000f02a, 0x0650f5df, /* 0650f5df //C 0f02a st	r5,reg5 */
0x0000f02b, 0x0660f5e0, /* 0660f5e0 //C 0f02b st	r6,reg6 */
0x0000f02c, 0x0670f5e1, /* 0670f5e1 //C 0f02c st	r7,reg7 */
0x0000f02d, 0x0680f5e2, /* 0680f5e2 //C 0f02d st	r8,reg8 */
0x0000f02e, 0x0690f5e3, /* 0690f5e3 //C 0f02e st	r9,reg9 */
0x0000f02f, 0x06a0f5e4, /* 06a0f5e4 //C 0f02f st	r10,reg10 */
0x0000f030, 0x06b0f5e5, /* 06b0f5e5 //C 0f030 st	r11,reg11 */
0x0000f031, 0x06c0f5e6, /* 06c0f5e6 //C 0f031 st	r12,reg12 */
0x0000f032, 0x06d0f5e7, /* 06d0f5e7 //C 0f032 st	r13,reg13 */
0x0000f033, 0x06e0f5e8, /* 06e0f5e8 //C 0f033 st	r14,reg14 */
0x0000f034, 0x06e0f5e9, /* 06e0f5e9 //C 0f034 st	r14,reg15 */
0x0000f035, 0x020e0000, /* 020e0000 //C 0f035 getf	r0 */
0x0000f036, 0x0600f5ea, /* 0600f5ea //C 0f036 st	r0,regf */
0x0000f037, 0x01020001, /* 01020001 //C 0f037 mvzl	r0,1 */
0x0000f038, 0x0600f5d8, /* 0600f5d8 //C 0f038 st	r0,called */
0x0000f039, 0x01f2f046, /* 01f2f046 //C 0f039 jmp	common_start */
0x0000f03a, 0x01f2f046, /* 01f2f046 //C 0f03a jmp	common_start */
0x0000f03b, 0x01f2f03c, /* 01f2f03c //C 0f03b jmp	cold_start */
0x0000f03c, 0x01020000, /* 01020000 //C 0f03c mvzl	r0,0 */
0x0000f03d, 0x0600f5d8, /* 0600f5d8 //C 0f03d st	r0,called */
0x0000f03e, 0x0102f03b, /* 0102f03b //C 0f03e mvzl	r0,def_zero	; restore jmp to monitor at zero */
0x0000f03f, 0x0e000000, /* 0e000000 //C 0f03f ld	r0,r0 */
0x0000f040, 0x06000000, /* 06000000 //C 0f040 st	r0,0 */
0x0000f041, 0x01020000, /* 01020000 //C 0f041 mvzl	r0,0		; def values of some regs */
0x0000f042, 0x0600f5ea, /* 0600f5ea //C 0f042 st	r0,regf		; FALGS= 0 */
0x0000f043, 0x0102f7ff, /* 0102f7ff //C 0f043 mvzl	r0,0xf7ff	; R13= 0xf7ff */
0x0000f044, 0x0600f5e7, /* 0600f5e7 //C 0f044 st	r0,reg13 */
0x0000f045, 0x01f2f046, /* 01f2f046 //C 0f045 jmp	common_start: */
0x0000f046, 0x01d2f82a, /* 01d2f82a //C 0f046 mvzl	sp,stack_end */
0x0000f047, 0x01020001, /* 01020001 //C 0f047 mvzl	r0,1 */
0x0000f048, 0x0600f5d7, /* 0600f5d7 //C 0f048 st	r0,echo */
0x0000f049, 0x01020000, /* 01020000 //C 0f049 mvzl	r0,0 */
0x0000f04a, 0x020f0000, /* 020f0000 //C 0f04a setf	r0 */
0x0000f04b, 0x0710ff41, /* 0710ff41 //C 0f04b ld	r1,UART_CTRL	; check if transmit is enabled */
0x0000f04c, 0x011c0002, /* 011c0002 //C 0f04c test	r1,2 */
0x0000f04d, 0x11f2f051, /* 11f2f051 //C 0f04d jz	tr_is_off */
0x0000f04e, 0x0710ff43, /* 0710ff43 //C 0f04e ld	r1,UART_TSTAT	; if transmit is ongoing */
0x0000f04f, 0x011c0001, /* 011c0001 //C 0f04f test	r1,1		; wait it to finish */
0x0000f050, 0x11f2f04e, /* 11f2f04e //C 0f050 jz	wait_uart_tr */
0x0000f051, 0x01120003, /* 01120003 //C 0f051 mvzl	r1,3		; turn on rx and tx */
0x0000f052, 0x0610ff41, /* 0610ff41 //C 0f052 st	r1,UART_CTRL */
0x0000f053, 0x0102000a, /* 0102000a //C 0f053 mvzl	r0,LF */
0x0000f054, 0x0400f44c, /* 0400f44c //C 0f054 call	putchar */
0x0000f055, 0x0102f5eb, /* 0102f5eb //C 0f055 mvzl	r0,msg_start */
0x0000f056, 0x0400f476, /* 0400f476 //C 0f056 call	printsnl */
0x0000f057, 0x0700f5d8, /* 0700f5d8 //C 0f057 ld	r0,called */
0x0000f058, 0x020b0000, /* 020b0000 //C 0f058 sz	r0 */
0x0000f059, 0x11f2f063, /* 11f2f063 //C 0f059 jz	no_called_from */
0x0000f05a, 0x0102000a, /* 0102000a //C 0f05a mvzl	r0,LF */
0x0000f05b, 0x0400f44c, /* 0400f44c //C 0f05b call	putchar */
0x0000f05c, 0x0102f5f9, /* 0102f5f9 //C 0f05c mvzl	r0,msg_stopat */
0x0000f05d, 0x0400f453, /* 0400f453 //C 0f05d call	prints */
0x0000f05e, 0x0700f5e8, /* 0700f5e8 //C 0f05e ld	r0,reg14 */
0x0000f05f, 0x01120004, /* 01120004 //C 0f05f mvzl	r1,4 */
0x0000f060, 0x0400f47d, /* 0400f47d //C 0f060 call	print_vhex */
0x0000f061, 0x0102000a, /* 0102000a //C 0f061 mvzl	r0,LF */
0x0000f062, 0x0400f44c, /* 0400f44c //C 0f062 call	putchar */
0x0000f063, 0x0400f065, /* 0400f065 //C 0f063 call	setup_line */
0x0000f064, 0x01f2f06e, /* 01f2f06e //C 0f064 jmp	main */
0x0000f065, 0x0ded0000, /* 0ded0000 //C 0f065 push	lr */
0x0000f066, 0x01120000, /* 01120000 //C 0f066 mvzl	r1,0		; lptr= 0 */
0x0000f067, 0x0610f5d0, /* 0610f5d0 //C 0f067 st	r1,line_ptr */
0x0000f068, 0x0610f56c, /* 0610f56c //C 0f068 st	r1,line		; line[0]= 0 */
0x0000f069, 0x01120000, /* 01120000 //C 0f069 mvzl	r1,0		; at_eol= 0 */
0x0000f06a, 0x0610f5d1, /* 0610f5d1 //C 0f06a st	r1,at_eol */
0x0000f06b, 0x0102f607, /* 0102f607 //C 0f06b mvzl	r0,prompt */
0x0000f06c, 0x0400f453, /* 0400f453 //C 0f06c call	prints */
0x0000f06d, 0x0ffd0000, /* 0ffd0000 //C 0f06d pop	pc */
0x0000f06e, 0x0400f441, /* 0400f441 //C 0f06e call	check_uart */
0x0000f06f, 0x41f2f075, /* 41f2f075 //C 0f06f C0 jmp	no_input */
0x0000f070, 0x0400f449, /* 0400f449 //C 0f070 call	read */
0x0000f071, 0x0400f076, /* 0400f076 //C 0f071 call	proc_input */
0x0000f072, 0x41f2f075, /* 41f2f075 //C 0f072 C0 jmp	no_line */
0x0000f073, 0x0400f0a7, /* 0400f0a7 //C 0f073 call	proc_line */
0x0000f074, 0x0400f065, /* 0400f065 //C 0f074 call	setup_line */
0x0000f075, 0x01f2f06e, /* 01f2f06e //C 0f075 jmp	main */
0x0000f076, 0x0ded0000, /* 0ded0000 //C 0f076 push	lr */
0x0000f077, 0x0108000a, /* 0108000a //C 0f077 cmp	r0,LF */
0x0000f078, 0x11f2f0a1, /* 11f2f0a1 //C 0f078 EQ jmp	got_eol */
0x0000f079, 0x0108000d, /* 0108000d //C 0f079 cmp	r0,CR */
0x0000f07a, 0x11f2f0a1, /* 11f2f0a1 //C 0f07a EQ jmp	got_eol */
0x0000f07b, 0x01080008, /* 01080008 //C 0f07b cmp	r0,8 */
0x0000f07c, 0x11f2f080, /* 11f2f080 //C 0f07c jz	got_BS */
0x0000f07d, 0x0108007f, /* 0108007f //C 0f07d cmp	r0,127 */
0x0000f07e, 0x11f2f080, /* 11f2f080 //C 0f07e jz	got_DEL */
0x0000f07f, 0x01f2f08c, /* 01f2f08c //C 0f07f jmp	got_char */
0x0000f080, 0x0700f5d0, /* 0700f5d0 //C 0f080 ld	r0,line_ptr */
0x0000f081, 0x020b0000, /* 020b0000 //C 0f081 sz	r0 */
0x0000f082, 0x11f2f08a, /* 11f2f08a //C 0f082 jz	got_done */
0x0000f083, 0x01060001, /* 01060001 //C 0f083 sub	r0,1 */
0x0000f084, 0x0600f5d0, /* 0600f5d0 //C 0f084 st	r0,line_ptr */
0x0000f085, 0x0112f56c, /* 0112f56c //C 0f085 mvzl	r1,line */
0x0000f086, 0x01220000, /* 01220000 //C 0f086 mvzl	r2,0 */
0x0000f087, 0x08210000, /* 08210000 //C 0f087 st	r2,r1,r0 */
0x0000f088, 0x0102f603, /* 0102f603 //C 0f088 mvzl	r0,msg_BS */
0x0000f089, 0x0400f453, /* 0400f453 //C 0f089 call	prints */
0x0000f08a, 0x020d0000, /* 020d0000 //C 0f08a clc */
0x0000f08b, 0x01f2f0a6, /* 01f2f0a6 //C 0f08b jmp	proc_input_ret */
0x0000f08c, 0x0108001f, /* 0108001f //C 0f08c cmp	r0,31		; refuse control chars */
0x0000f08d, 0xa1f2f0a6, /* a1f2f0a6 //C 0f08d LS jmp	proc_input_ret */
0x0000f08e, 0x0108007e, /* 0108007e //C 0f08e cmp	r0,126		; refuse graph chars */
0x0000f08f, 0x91f2f0a6, /* 91f2f0a6 //C 0f08f HI jmp	proc_input_ret */
0x0000f090, 0x01220000, /* 01220000 //C 0f090 mvzl	r2,0		; at_aol= 0 */
0x0000f091, 0x0620f5d1, /* 0620f5d1 //C 0f091 st	r2,at_eol */
0x0000f092, 0x0112f5d0, /* 0112f5d0 //C 0f092 mvzl	r1,line_ptr	; line[line_ptr]= char */
0x0000f093, 0x0e310000, /* 0e310000 //C 0f093 ld	r3,r1 */
0x0000f094, 0x0122f56c, /* 0122f56c //C 0f094 mvzl	r2,line */
0x0000f095, 0x08020300, /* 08020300 //C 0f095 st	r0,r2,r3 */
0x0000f096, 0x013a0001, /* 013a0001 //C 0f096 plus	r3,1		; line_ptr++ */
0x0000f097, 0x0c310000, /* 0c310000 //C 0f097 st	r3,r1 */
0x0000f098, 0x01420000, /* 01420000 //C 0f098 mvzl	r4,0 */
0x0000f099, 0x09438200, /* 09438200 //C 0f099 st	r4,r3+,r2	; line[line_ptr]= 0 */
0x0000f09a, 0x08430200, /* 08430200 //C 0f09a st	r4,r3,r2	; double 0 at end, needed by tokenizer */
0x0000f09b, 0x0142f5d7, /* 0142f5d7 //C 0f09b mvzl	r4,echo		; check if echo is turned on */
0x0000f09c, 0x0e440000, /* 0e440000 //C 0f09c ld	r4,r4 */
0x0000f09d, 0x024b0000, /* 024b0000 //C 0f09d sz	r4 */
0x0000f09e, 0x2400f44c, /* 2400f44c //C 0f09e NZ call	putchar		; echo */
0x0000f09f, 0x020d0000, /* 020d0000 //C 0f09f clc */
0x0000f0a0, 0x01f2f0a6, /* 01f2f0a6 //C 0f0a0 jmp	proc_input_ret */
0x0000f0a1, 0x0112f5d1, /* 0112f5d1 //C 0f0a1 mvzl	r1,at_eol */
0x0000f0a2, 0x0e110000, /* 0e110000 //C 0f0a2 ld	r1,r1 */
0x0000f0a3, 0x021b0000, /* 021b0000 //C 0f0a3 sz	r1		; Z=0 at eol -> skip, not ready */
0x0000f0a4, 0x220d0000, /* 220d0000 //C 0f0a4 Z0 clc */
0x0000f0a5, 0x120c0000, /* 120c0000 //C 0f0a5 Z1 sec */
0x0000f0a6, 0x0ffd0000, /* 0ffd0000 //C 0f0a6 pop	pc */
0x0000f0a7, 0x0ded0000, /* 0ded0000 //C 0f0a7 push	lr */
0x0000f0a8, 0x0102000a, /* 0102000a //C 0f0a8 mvzl	r0,LF */
0x0000f0a9, 0x0400f44c, /* 0400f44c //C 0f0a9 call	putchar */
0x0000f0aa, 0x0700f56c, /* 0700f56c //C 0f0aa ld	r0,line */
0x0000f0ab, 0x020b0000, /* 020b0000 //C 0f0ab sz	r0 */
0x0000f0ac, 0x11f2f0b4, /* 11f2f0b4 //C 0f0ac jz	proc_line_ret */
0x0000f0ad, 0x0400f0d0, /* 0400f0d0 //C 0f0ad call	tokenize */
0x0000f0ae, 0x0400f0f0, /* 0400f0f0 //C 0f0ae call	find_cmd */
0x0000f0af, 0x41f2f0b2, /* 41f2f0b2 //C 0f0af C0 jmp	cmd_not_found */
0x0000f0b0, 0x05000000, /* 05000000 //C 0f0b0 call	r0,0 */
0x0000f0b1, 0x01f2f0b4, /* 01f2f0b4 //C 0f0b1 jmp	proc_line_ret */
0x0000f0b2, 0x0102f0bc, /* 0102f0bc //C 0f0b2 mvzl	r0,snotfound */
0x0000f0b3, 0x0400f476, /* 0400f476 //C 0f0b3 call	printsnl */
0x0000f0b4, 0x01120001, /* 01120001 //C 0f0b4 mvzl	r1,1		; at_eol= 1 */
0x0000f0b5, 0x0610f5d1, /* 0610f5d1 //C 0f0b5 st	r1,at_eol */
0x0000f0b6, 0x0ffd0000, /* 0ffd0000 //C 0f0b6 pop	pc */
0x0000f0b7, 0x00000047, /* 00000047 //C 0f0b7 db	71 */
0x0000f0b8, 0x0000006f, /* 0000006f //C 0f0b8 db	111 */
0x0000f0b9, 0x00000074, /* 00000074 //C 0f0b9 db	116 */
0x0000f0ba, 0x0000003a, /* 0000003a //C 0f0ba db	58 */
0x0000f0bb, 0x00000000, /* 00000000 //C 0f0bb db */
0x0000f0bc, 0x00000055, /* 00000055 //C 0f0bc db	85 */
0x0000f0bd, 0x0000006e, /* 0000006e //C 0f0bd db	110 */
0x0000f0be, 0x0000006b, /* 0000006b //C 0f0be db	107 */
0x0000f0bf, 0x0000006e, /* 0000006e //C 0f0bf db	110 */
0x0000f0c0, 0x0000006f, /* 0000006f //C 0f0c0 db	111 */
0x0000f0c1, 0x00000077, /* 00000077 //C 0f0c1 db	119 */
0x0000f0c2, 0x0000006e, /* 0000006e //C 0f0c2 db	110 */
0x0000f0c3, 0x00000020, /* 00000020 //C 0f0c3 db	32 */
0x0000f0c4, 0x00000063, /* 00000063 //C 0f0c4 db	99 */
0x0000f0c5, 0x0000006f, /* 0000006f //C 0f0c5 db	111 */
0x0000f0c6, 0x0000006d, /* 0000006d //C 0f0c6 db	109 */
0x0000f0c7, 0x0000006d, /* 0000006d //C 0f0c7 db	109 */
0x0000f0c8, 0x00000061, /* 00000061 //C 0f0c8 db	97 */
0x0000f0c9, 0x0000006e, /* 0000006e //C 0f0c9 db	110 */
0x0000f0ca, 0x00000064, /* 00000064 //C 0f0ca db	100 */
0x0000f0cb, 0x00000000, /* 00000000 //C 0f0cb db */
0x0000f0cc, 0x0ded0000, /* 0ded0000 //C 0f0cc push	lr */
0x0000f0cd, 0x0112f609, /* 0112f609 //C 0f0cd mvzl	r1,delimiters */
0x0000f0ce, 0x0400f39e, /* 0400f39e //C 0f0ce call	strchr */
0x0000f0cf, 0x0ffd0000, /* 0ffd0000 //C 0f0cf pop	pc */
0x0000f0d0, 0x0ded0000, /* 0ded0000 //C 0f0d0 push	lr */
0x0000f0d1, 0x0142f5d2, /* 0142f5d2 //C 0f0d1 mvzl	r4,words	; array of result */
0x0000f0d2, 0x0152f56c, /* 0152f56c //C 0f0d2 mvzl	r5,line		; address of next char */
0x0000f0d3, 0x01620000, /* 01620000 //C 0f0d3 mvzl	r6,0		; nuof words found */
0x0000f0d4, 0x01720000, /* 01720000 //C 0f0d4 mvzl	r7,0		; bool in_word */
0x0000f0d5, 0x0e050000, /* 0e050000 //C 0f0d5 ld	r0,r5		; pick a char */
0x0000f0d6, 0x020b0000, /* 020b0000 //C 0f0d6 sz	r0		; check end */
0x0000f0d7, 0x11f2f0e1, /* 11f2f0e1 //C 0f0d7 jz	tok_delimiter	; found end, pretend delim */
0x0000f0d8, 0x0400f0cc, /* 0400f0cc //C 0f0d8 call	is_delimiter */
0x0000f0d9, 0x31f2f0e1, /* 31f2f0e1 //C 0f0d9 C1 jmp	tok_delimiter */
0x0000f0da, 0x027b0000, /* 027b0000 //C 0f0da sz	r7 */
0x0000f0db, 0x21f2f0e6, /* 21f2f0e6 //C 0f0db jnz	tok_next	; still inside word */
0x0000f0dc, 0x01720001, /* 01720001 //C 0f0dc mvzl	r7,1		; in_word=true */
0x0000f0dd, 0x09568400, /* 09568400 //C 0f0dd st	r5,r6+,r4	; record word address */
0x0000f0de, 0x01680005, /* 01680005 //C 0f0de cmp	r6,MAX_WORDS	; If no more space */
0x0000f0df, 0x11f2f0ea, /* 11f2f0ea //C 0f0df EQ jmp	tok_ret		; then return */
0x0000f0e0, 0x01f2f0e6, /* 01f2f0e6 //C 0f0e0 jmp	tok_next */
0x0000f0e1, 0x027b0000, /* 027b0000 //C 0f0e1 sz	r7 */
0x0000f0e2, 0x11f2f0e6, /* 11f2f0e6 //C 0f0e2 jz	tok_next	; still between words */
0x0000f0e3, 0x01720000, /* 01720000 //C 0f0e3 mvzl	r7,0		; in_word=false */
0x0000f0e4, 0x01120000, /* 01120000 //C 0f0e4 mvzl	r1,0		; put a 0 at the end of word */
0x0000f0e5, 0x08150100, /* 08150100 //C 0f0e5 st	r1,r5,r1 */
0x0000f0e6, 0x020b0000, /* 020b0000 //C 0f0e6 sz	r0		; check EOL */
0x0000f0e7, 0x11f2f0ea, /* 11f2f0ea //C 0f0e7 jz	tok_ret		; jump out if char==0 */
0x0000f0e8, 0x01540001, /* 01540001 //C 0f0e8 add	r5,1 */
0x0000f0e9, 0x01f2f0d5, /* 01f2f0d5 //C 0f0e9 jmp	tok_cycle */
0x0000f0ea, 0x01120000, /* 01120000 //C 0f0ea mvzl	r1,0 */
0x0000f0eb, 0x01680005, /* 01680005 //C 0f0eb cmp	r6,MAX_WORDS */
0x0000f0ec, 0x11f2f0ef, /* 11f2f0ef //C 0f0ec jz	tok_end */
0x0000f0ed, 0x09168400, /* 09168400 //C 0f0ed st	r1,r6+,r4 */
0x0000f0ee, 0x01f2f0ea, /* 01f2f0ea //C 0f0ee jmp	tok_ret */
0x0000f0ef, 0x0ffd0000, /* 0ffd0000 //C 0f0ef pop	pc */
0x0000f0f0, 0x0ded0000, /* 0ded0000 //C 0f0f0 push	lr */
0x0000f0f1, 0x0d1d0000, /* 0d1d0000 //C 0f0f1 push	r1 */
0x0000f0f2, 0x0d2d0000, /* 0d2d0000 //C 0f0f2 push	r2 */
0x0000f0f3, 0x0d3d0000, /* 0d3d0000 //C 0f0f3 push	r3 */
0x0000f0f4, 0x0dad0000, /* 0dad0000 //C 0f0f4 push	r10 */
0x0000f0f5, 0x0700f5d2, /* 0700f5d2 //C 0f0f5 ld	r0,words	; R0= 1st word of command */
0x0000f0f6, 0x020b0000, /* 020b0000 //C 0f0f6 sz	r0 */
0x0000f0f7, 0x11f2f127, /* 11f2f127 //C 0f0f7 jz	find_cmd_false */
0x0000f0f8, 0x0e100000, /* 0e100000 //C 0f0f8 ld	r1,r0		; 1st char of word1 */
0x0000f0f9, 0x0e200001, /* 0e200001 //C 0f0f9 ld	r2,r0,1		; 2nd char */
0x0000f0fa, 0x0e300002, /* 0e300002 //C 0f0fa ld	r3,r0,2		; 3rd char */
0x0000f0fb, 0x011fffdf, /* 011fffdf //C 0f0fb and	r1,0xffdf	; upcase 1st char */
0x0000f0fc, 0x01180052, /* 01180052 //C 0f0fc cmp	r1,'R' */
0x0000f0fd, 0x21f2f113, /* 21f2f113 //C 0f0fd jnz	find_not_rx */
0x0000f0fe, 0x0128002f, /* 0128002f //C 0f0fe cmp	r2,'/'		; '0'-1 */
0x0000f0ff, 0xa1f2f113, /* a1f2f113 //C 0f0ff LS jmp	find_not_rx */
0x0000f100, 0x01280039, /* 01280039 //C 0f100 cmp	r2,'9' */
0x0000f101, 0x91f2f113, /* 91f2f113 //C 0f101 HI jmp	find_not_rx */
0x0000f102, 0x023b0000, /* 023b0000 //C 0f102 sz	r3 */
0x0000f103, 0x11f2f10e, /* 11f2f10e //C 0f103 jz	find_rx_09 */
0x0000f104, 0x01280031, /* 01280031 //C 0f104 cmp	r2,'1'		; first char must be '1' */
0x0000f105, 0x21f2f113, /* 21f2f113 //C 0f105 jnz	find_not_rx */
0x0000f106, 0x0138002f, /* 0138002f //C 0f106 cmp	r3,'/'		; '0'-1 */
0x0000f107, 0xa1f2f113, /* a1f2f113 //C 0f107 LS jmp	find_not_rx */
0x0000f108, 0x01380035, /* 01380035 //C 0f108 cmp	r3,'5' */
0x0000f109, 0x91f2f113, /* 91f2f113 //C 0f109 HI jmp	find_not_rx */
0x0000f10a, 0x01360030, /* 01360030 //C 0f10a sub	r3,'0' */
0x0000f10b, 0x0134000a, /* 0134000a //C 0f10b add	r3,10 */
0x0000f10c, 0x0630f5d9, /* 0630f5d9 //C 0f10c st	r3,nuof_reg */
0x0000f10d, 0x01f2f110, /* 01f2f110 //C 0f10d jmp	find_rx */
0x0000f10e, 0x01260030, /* 01260030 //C 0f10e sub	r2,'0' */
0x0000f10f, 0x0620f5d9, /* 0620f5d9 //C 0f10f st	r2,nuof_reg */
0x0000f110, 0x0102f35d, /* 0102f35d //C 0f110 mvzl	r0,cmd_rx */
0x0000f111, 0x020c0000, /* 020c0000 //C 0f111 sec */
0x0000f112, 0x01f2f132, /* 01f2f132 //C 0f112 jmp	find_cmd_ret */
0x0000f113, 0x01a2f625, /* 01a2f625 //C 0f113 mvzl	r10,commands */
0x0000f114, 0x0e2a0000, /* 0e2a0000 //C 0f114 ld	r2,r10		; R2= cmd addr */
0x0000f115, 0x022b0000, /* 022b0000 //C 0f115 sz	r2 */
0x0000f116, 0x11f2f127, /* 11f2f127 //C 0f116 jz	find_cmd_false */
0x0000f117, 0x01a40001, /* 01a40001 //C 0f117 add	r10,1 */
0x0000f118, 0x00100a00, /* 00100a00 //C 0f118 mov	r1,r10		; R1= cmd string */
0x0000f119, 0x01a40001, /* 01a40001 //C 0f119 add	r10,1 */
0x0000f11a, 0x0e3a0000, /* 0e3a0000 //C 0f11a ld	r3,r10 */
0x0000f11b, 0x023b0000, /* 023b0000 //C 0f11b sz	r3 */
0x0000f11c, 0x21f2f119, /* 21f2f119 //C 0f11c jnz	find_cmd_fw */
0x0000f11d, 0x01a40001, /* 01a40001 //C 0f11d add	r10,1 */
0x0000f11e, 0x0400f3db, /* 0400f3db //C 0f11e call	streq */
0x0000f11f, 0x41f2f114, /* 41f2f114 //C 0f11f C0 jmp	find_cmd_cyc */
0x0000f120, 0x00000200, /* 00000200 //C 0f120 mov	r0,r2 */
0x0000f121, 0x020c0000, /* 020c0000 //C 0f121 sec */
0x0000f122, 0x01f2f132, /* 01f2f132 //C 0f122 jmp	find_cmd_ret */
0x0000f123, 0x0000002f, /* 0000002f //C 0f123 db	47 */
0x0000f124, 0x0000002f, /* 0000002f //C 0f124 db	47 */
0x0000f125, 0x00000043, /* 00000043 //C 0f125 db	67 */
0x0000f126, 0x00000000, /* 00000000 //C 0f126 db */
0x0000f127, 0x01040001, /* 01040001 //C 0f127 add	r0,1		; check second word */
0x0000f128, 0x0e100000, /* 0e100000 //C 0f128 ld	r1,r0		; for //C command */
0x0000f129, 0x021b0000, /* 021b0000 //C 0f129 sz	r1 */
0x0000f12a, 0x11f2f130, /* 11f2f130 //C 0f12a jz	find_cmd_very_false */
0x0000f12b, 0x0102f123, /* 0102f123 //C 0f12b mvzl	r0,c_cmd_name */
0x0000f12c, 0x0400f3db, /* 0400f3db //C 0f12c call	streq */
0x0000f12d, 0x21f2f130, /* 21f2f130 //C 0f12d jnz	find_cmd_very_false */
0x0000f12e, 0x0122f1e5, /* 0122f1e5 //C 0f12e mvzl	r2,cmd_c */
0x0000f12f, 0x01f2f120, /* 01f2f120 //C 0f12f jmp	find_cmd_true */
0x0000f130, 0x020d0000, /* 020d0000 //C 0f130 clc */
0x0000f131, 0x01020000, /* 01020000 //C 0f131 mvzl	r0,0 */
0x0000f132, 0x0fad0000, /* 0fad0000 //C 0f132 pop	r10 */
0x0000f133, 0x0f3d0000, /* 0f3d0000 //C 0f133 pop	r3 */
0x0000f134, 0x0f2d0000, /* 0f2d0000 //C 0f134 pop	r2 */
0x0000f135, 0x0f1d0000, /* 0f1d0000 //C 0f135 pop	r1 */
0x0000f136, 0x0ffd0000, /* 0ffd0000 //C 0f136 pop	pc */
0x0000f137, 0x0ded0000, /* 0ded0000 //C 0f137 push	lr */
0x0000f138, 0x0122f5d2, /* 0122f5d2 //C 0f138 mvzl	r2,words */
0x0000f139, 0x01020000, /* 01020000 //C 0f139 mvzl	r0,0 */
0x0000f13a, 0x0e420001, /* 0e420001 //C 0f13a ld	r4,r2,1		; addr */
0x0000f13b, 0x0e520002, /* 0e520002 //C 0f13b ld	r5,r2,2		; value */
0x0000f13c, 0x024b0000, /* 024b0000 //C 0f13c sz 	r4 */
0x0000f13d, 0x11f2f163, /* 11f2f163 //C 0f13d jz	m_ret */
0x0000f13e, 0x00000400, /* 00000400 //C 0f13e mov	r0,r4 */
0x0000f13f, 0x0400f41e, /* 0400f41e //C 0f13f call	htoi */
0x0000f140, 0x00400100, /* 00400100 //C 0f140 mov	r4,r1 */
0x0000f141, 0x31f2f145, /* 31f2f145 //C 0f141 C1 jmp	m_addr_ok */
0x0000f142, 0x0102f164, /* 0102f164 //C 0f142 mvzl	r0,m_err_addr */
0x0000f143, 0x0400f476, /* 0400f476 //C 0f143 call	printsnl */
0x0000f144, 0x01f2f163, /* 01f2f163 //C 0f144 jmp	m_ret */
0x0000f145, 0x025b0000, /* 025b0000 //C 0f145 sz	r5 */
0x0000f146, 0x11f2f159, /* 11f2f159 //C 0f146 jz	m_read */
0x0000f147, 0x0132f000, /* 0132f000 //C 0f147 mvzl	r3,the_begin */
0x0000f148, 0x00380400, /* 00380400 //C 0f148 cmp	r3,r4 */
0x0000f149, 0x91f2f151, /* 91f2f151 //C 0f149 HI jmp	m_addrv_ok */
0x0000f14a, 0x0132f848, /* 0132f848 //C 0f14a mvzl	r3,the_end */
0x0000f14b, 0x00380400, /* 00380400 //C 0f14b cmp	r3,r4 */
0x0000f14c, 0x91f2f14e, /* 91f2f14e //C 0f14c HI jmp	m_addrv_nok */
0x0000f14d, 0x01f2f151, /* 01f2f151 //C 0f14d jmp	m_addrv_ok */
0x0000f14e, 0x0102f172, /* 0102f172 //C 0f14e mvzl	r0,m_err_addrv */
0x0000f14f, 0x0400f476, /* 0400f476 //C 0f14f call	printsnl */
0x0000f150, 0x01f2f163, /* 01f2f163 //C 0f150 jmp	m_ret */
0x0000f151, 0x00000500, /* 00000500 //C 0f151 mov	r0,r5 */
0x0000f152, 0x0400f41e, /* 0400f41e //C 0f152 call	htoi */
0x0000f153, 0x00500100, /* 00500100 //C 0f153 mov	r5,r1 */
0x0000f154, 0x31f2f158, /* 31f2f158 //C 0f154 C1 jmp	m_value_ok */
0x0000f155, 0x0102f184, /* 0102f184 //C 0f155 mvzl	r0,m_err_value */
0x0000f156, 0x0400f476, /* 0400f476 //C 0f156 call	printsnl */
0x0000f157, 0x01f2f163, /* 01f2f163 //C 0f157 jmp	m_ret */
0x0000f158, 0x0c540000, /* 0c540000 //C 0f158 st	r5,r4 */
0x0000f159, 0x00000400, /* 00000400 //C 0f159 mov	r0,r4 */
0x0000f15a, 0x01120004, /* 01120004 //C 0f15a mvzl	r1,4 */
0x0000f15b, 0x0400f47d, /* 0400f47d //C 0f15b call	print_vhex */
0x0000f15c, 0x01020020, /* 01020020 //C 0f15c mvzl	r0,0x20 */
0x0000f15d, 0x0400f44c, /* 0400f44c //C 0f15d call	putchar */
0x0000f15e, 0x0e040000, /* 0e040000 //C 0f15e ld	r0,r4 */
0x0000f15f, 0x01120004, /* 01120004 //C 0f15f mvzl	r1,4 */
0x0000f160, 0x0400f47d, /* 0400f47d //C 0f160 call	print_vhex */
0x0000f161, 0x0102000a, /* 0102000a //C 0f161 mvzl	r0,LF */
0x0000f162, 0x0400f44c, /* 0400f44c //C 0f162 call	putchar */
0x0000f163, 0x0ffd0000, /* 0ffd0000 //C 0f163 pop	pc */
0x0000f164, 0x00000041, /* 00000041 //C 0f164 db	65 */
0x0000f165, 0x00000064, /* 00000064 //C 0f165 db	100 */
0x0000f166, 0x00000064, /* 00000064 //C 0f166 db	100 */
0x0000f167, 0x00000072, /* 00000072 //C 0f167 db	114 */
0x0000f168, 0x00000065, /* 00000065 //C 0f168 db	101 */
0x0000f169, 0x00000073, /* 00000073 //C 0f169 db	115 */
0x0000f16a, 0x00000073, /* 00000073 //C 0f16a db	115 */
0x0000f16b, 0x00000020, /* 00000020 //C 0f16b db	32 */
0x0000f16c, 0x00000065, /* 00000065 //C 0f16c db	101 */
0x0000f16d, 0x00000072, /* 00000072 //C 0f16d db	114 */
0x0000f16e, 0x00000072, /* 00000072 //C 0f16e db	114 */
0x0000f16f, 0x0000006f, /* 0000006f //C 0f16f db	111 */
0x0000f170, 0x00000072, /* 00000072 //C 0f170 db	114 */
0x0000f171, 0x00000000, /* 00000000 //C 0f171 db */
0x0000f172, 0x0000004d, /* 0000004d //C 0f172 db	77 */
0x0000f173, 0x0000006f, /* 0000006f //C 0f173 db	111 */
0x0000f174, 0x0000006e, /* 0000006e //C 0f174 db	110 */
0x0000f175, 0x00000069, /* 00000069 //C 0f175 db	105 */
0x0000f176, 0x00000074, /* 00000074 //C 0f176 db	116 */
0x0000f177, 0x0000006f, /* 0000006f //C 0f177 db	111 */
0x0000f178, 0x00000072, /* 00000072 //C 0f178 db	114 */
0x0000f179, 0x00000027, /* 00000027 //C 0f179 db	39 */
0x0000f17a, 0x00000073, /* 00000073 //C 0f17a db	115 */
0x0000f17b, 0x00000020, /* 00000020 //C 0f17b db	32 */
0x0000f17c, 0x00000061, /* 00000061 //C 0f17c db	97 */
0x0000f17d, 0x00000064, /* 00000064 //C 0f17d db	100 */
0x0000f17e, 0x00000064, /* 00000064 //C 0f17e db	100 */
0x0000f17f, 0x00000072, /* 00000072 //C 0f17f db	114 */
0x0000f180, 0x00000065, /* 00000065 //C 0f180 db	101 */
0x0000f181, 0x00000073, /* 00000073 //C 0f181 db	115 */
0x0000f182, 0x00000073, /* 00000073 //C 0f182 db	115 */
0x0000f183, 0x00000000, /* 00000000 //C 0f183 db */
0x0000f184, 0x00000056, /* 00000056 //C 0f184 db	86 */
0x0000f185, 0x00000061, /* 00000061 //C 0f185 db	97 */
0x0000f186, 0x0000006c, /* 0000006c //C 0f186 db	108 */
0x0000f187, 0x00000075, /* 00000075 //C 0f187 db	117 */
0x0000f188, 0x00000065, /* 00000065 //C 0f188 db	101 */
0x0000f189, 0x00000020, /* 00000020 //C 0f189 db	32 */
0x0000f18a, 0x00000065, /* 00000065 //C 0f18a db	101 */
0x0000f18b, 0x00000072, /* 00000072 //C 0f18b db	114 */
0x0000f18c, 0x00000072, /* 00000072 //C 0f18c db	114 */
0x0000f18d, 0x0000006f, /* 0000006f //C 0f18d db	111 */
0x0000f18e, 0x00000072, /* 00000072 //C 0f18e db	114 */
0x0000f18f, 0x00000000, /* 00000000 //C 0f18f db */
0x0000f190, 0x0ded0000, /* 0ded0000 //C 0f190 push	lr */
0x0000f191, 0x0122f5d2, /* 0122f5d2 //C 0f191 mvzl	r2,words */
0x0000f192, 0x01020000, /* 01020000 //C 0f192 mvzl	r0,0 */
0x0000f193, 0x0e420001, /* 0e420001 //C 0f193 ld	r4,r2,1		; addr */
0x0000f194, 0x0e520002, /* 0e520002 //C 0f194 ld	r5,r2,2		; value */
0x0000f195, 0x024b0000, /* 024b0000 //C 0f195 sz 	r4 */
0x0000f196, 0x11f2f163, /* 11f2f163 //C 0f196 jz	m_ret */
0x0000f197, 0x00000400, /* 00000400 //C 0f197 mov	r0,r4 */
0x0000f198, 0x0400f41e, /* 0400f41e //C 0f198 call	htoi */
0x0000f199, 0x00400100, /* 00400100 //C 0f199 mov	r4,r1 */
0x0000f19a, 0x31f2f19e, /* 31f2f19e //C 0f19a C1 jmp	sm_addr_ok */
0x0000f19b, 0x0102f164, /* 0102f164 //C 0f19b mvzl	r0,m_err_addr */
0x0000f19c, 0x0400f476, /* 0400f476 //C 0f19c call	printsnl */
0x0000f19d, 0x01f2f1b2, /* 01f2f1b2 //C 0f19d jmp	sm_ret */
0x0000f19e, 0x025b0000, /* 025b0000 //C 0f19e sz	r5 */
0x0000f19f, 0x11f2f1b2, /* 11f2f1b2 //C 0f19f jz	sm_ret */
0x0000f1a0, 0x0132f000, /* 0132f000 //C 0f1a0 mvzl	r3,the_begin */
0x0000f1a1, 0x00380400, /* 00380400 //C 0f1a1 cmp	r3,r4 */
0x0000f1a2, 0x91f2f1aa, /* 91f2f1aa //C 0f1a2 HI jmp	sm_addrv_ok */
0x0000f1a3, 0x0132f848, /* 0132f848 //C 0f1a3 mvzl	r3,the_end */
0x0000f1a4, 0x00380400, /* 00380400 //C 0f1a4 cmp	r3,r4 */
0x0000f1a5, 0x91f2f1a7, /* 91f2f1a7 //C 0f1a5 HI jmp	sm_addrv_nok */
0x0000f1a6, 0x01f2f1aa, /* 01f2f1aa //C 0f1a6 jmp	sm_addrv_ok */
0x0000f1a7, 0x0102f172, /* 0102f172 //C 0f1a7 mvzl	r0,m_err_addrv */
0x0000f1a8, 0x0400f476, /* 0400f476 //C 0f1a8 call	printsnl */
0x0000f1a9, 0x01f2f1b2, /* 01f2f1b2 //C 0f1a9 jmp	sm_ret */
0x0000f1aa, 0x00000500, /* 00000500 //C 0f1aa mov	r0,r5 */
0x0000f1ab, 0x0400f41e, /* 0400f41e //C 0f1ab call	htoi */
0x0000f1ac, 0x00500100, /* 00500100 //C 0f1ac mov	r5,r1 */
0x0000f1ad, 0x31f2f1b1, /* 31f2f1b1 //C 0f1ad C1 jmp	sm_value_ok */
0x0000f1ae, 0x0102f184, /* 0102f184 //C 0f1ae mvzl	r0,m_err_value */
0x0000f1af, 0x0400f476, /* 0400f476 //C 0f1af call	printsnl */
0x0000f1b0, 0x01f2f1b2, /* 01f2f1b2 //C 0f1b0 jmp	sm_ret */
0x0000f1b1, 0x0c540000, /* 0c540000 //C 0f1b1 st	r5,r4 */
0x0000f1b2, 0x0ffd0000, /* 0ffd0000 //C 0f1b2 pop	pc */
0x0000f1b3, 0x0ded0000, /* 0ded0000 //C 0f1b3 push	lr */
0x0000f1b4, 0x0122f5d2, /* 0122f5d2 //C 0f1b4 mvzl	r2,words */
0x0000f1b5, 0x0e020001, /* 0e020001 //C 0f1b5 ld	r0,r2,1		; start address */
0x0000f1b6, 0x0400f41e, /* 0400f41e //C 0f1b6 call	htoi */
0x0000f1b7, 0x00300100, /* 00300100 //C 0f1b7 mov	r3,r1 */
0x0000f1b8, 0x0e020002, /* 0e020002 //C 0f1b8 ld	r0,r2,2		; end address */
0x0000f1b9, 0x020b0000, /* 020b0000 //C 0f1b9 sz	r0 */
0x0000f1ba, 0x21f2f1be, /* 21f2f1be //C 0f1ba jnz	d_end_ok */
0x0000f1bb, 0x00400300, /* 00400300 //C 0f1bb mov	r4,r3 */
0x0000f1bc, 0x01440010, /* 01440010 //C 0f1bc add	r4,0x10 */
0x0000f1bd, 0x01f2f1c0, /* 01f2f1c0 //C 0f1bd jmp	d_chk_end */
0x0000f1be, 0x0400f41e, /* 0400f41e //C 0f1be call	htoi */
0x0000f1bf, 0x00400100, /* 00400100 //C 0f1bf mov	r4,r1 */
0x0000f1c0, 0x00380400, /* 00380400 //C 0f1c0 cmp	r3,r4		; check if start>end */
0x0000f1c1, 0x91f2f1d0, /* 91f2f1d0 //C 0f1c1 HI jmp d_bad */
0x0000f1c2, 0x00000300, /* 00000300 //C 0f1c2 mov	r0,r3		; print address */
0x0000f1c3, 0x01120004, /* 01120004 //C 0f1c3 mvzl	r1,4 */
0x0000f1c4, 0x0400f47d, /* 0400f47d //C 0f1c4 call	print_vhex */
0x0000f1c5, 0x01020020, /* 01020020 //C 0f1c5 mvzl	r0,0x20		; print one space */
0x0000f1c6, 0x0400f44c, /* 0400f44c //C 0f1c6 call	putchar */
0x0000f1c7, 0x0e030000, /* 0e030000 //C 0f1c7 ld	r0,r3		; load data */
0x0000f1c8, 0x01120004, /* 01120004 //C 0f1c8 mvzl	r1,4		; print data */
0x0000f1c9, 0x0400f47d, /* 0400f47d //C 0f1c9 call	print_vhex */
0x0000f1ca, 0x0102000a, /* 0102000a //C 0f1ca mvzl	r0,LF		; print new line */
0x0000f1cb, 0x0400f44c, /* 0400f44c //C 0f1cb call	putchar */
0x0000f1cc, 0x00380400, /* 00380400 //C 0f1cc cmp	r3,r4 */
0x0000f1cd, 0x11f2f1d2, /* 11f2f1d2 //C 0f1cd jz	d_ret */
0x0000f1ce, 0x01340001, /* 01340001 //C 0f1ce add	r3,1 */
0x0000f1cf, 0x01f2f1c2, /* 01f2f1c2 //C 0f1cf jmp	d_cyc */
0x0000f1d0, 0x0102f1d3, /* 0102f1d3 //C 0f1d0 mvzl	r0,d_err_bad */
0x0000f1d1, 0x0400f476, /* 0400f476 //C 0f1d1 call	printsnl */
0x0000f1d2, 0x0ffd0000, /* 0ffd0000 //C 0f1d2 pop	pc */
0x0000f1d3, 0x00000057, /* 00000057 //C 0f1d3 db	87 */
0x0000f1d4, 0x00000072, /* 00000072 //C 0f1d4 db	114 */
0x0000f1d5, 0x0000006f, /* 0000006f //C 0f1d5 db	111 */
0x0000f1d6, 0x0000006e, /* 0000006e //C 0f1d6 db	110 */
0x0000f1d7, 0x00000067, /* 00000067 //C 0f1d7 db	103 */
0x0000f1d8, 0x00000020, /* 00000020 //C 0f1d8 db	32 */
0x0000f1d9, 0x00000065, /* 00000065 //C 0f1d9 db	101 */
0x0000f1da, 0x0000006e, /* 0000006e //C 0f1da db	110 */
0x0000f1db, 0x00000064, /* 00000064 //C 0f1db db	100 */
0x0000f1dc, 0x00000020, /* 00000020 //C 0f1dc db	32 */
0x0000f1dd, 0x00000061, /* 00000061 //C 0f1dd db	97 */
0x0000f1de, 0x00000064, /* 00000064 //C 0f1de db	100 */
0x0000f1df, 0x00000064, /* 00000064 //C 0f1df db	100 */
0x0000f1e0, 0x00000072, /* 00000072 //C 0f1e0 db	114 */
0x0000f1e1, 0x00000065, /* 00000065 //C 0f1e1 db	101 */
0x0000f1e2, 0x00000073, /* 00000073 //C 0f1e2 db	115 */
0x0000f1e3, 0x00000073, /* 00000073 //C 0f1e3 db	115 */
0x0000f1e4, 0x00000000, /* 00000000 //C 0f1e4 db */
0x0000f1e5, 0x00f00e00, /* 00f00e00 //C 0f1e5 ret */
0x0000f1e6, 0x0ded0000, /* 0ded0000 //C 0f1e6 push	lr */
0x0000f1e7, 0x01a20000, /* 01a20000 //C 0f1e7 mvzl	r10,0		; state (nr of words) */
0x0000f1e8, 0x01820000, /* 01820000 //C 0f1e8 mvzl	r8,0		; value */
0x0000f1e9, 0x0162003f, /* 0162003f //C 0f1e9 mvzl	r6,'?'		; Record type */
0x0000f1ea, 0x01c20000, /* 01c20000 //C 0f1ea mvzl	r12,0		; Checksum */
0x0000f1eb, 0x0400f441, /* 0400f441 //C 0f1eb call	check_uart */
0x0000f1ec, 0x41f2f1eb, /* 41f2f1eb //C 0f1ec C0 jmp	l_cyc */
0x0000f1ed, 0x0400f449, /* 0400f449 //C 0f1ed call	read */
0x0000f1ee, 0x00b00000, /* 00b00000 //C 0f1ee mov	r11,r0		; Copy of char in R11 */
0x0000f1ef, 0x0108000a, /* 0108000a //C 0f1ef cmp	r0,10		; check EOL chars */
0x0000f1f0, 0x11f2f23c, /* 11f2f23c //C 0f1f0 jz	l_eol */
0x0000f1f1, 0x0108000d, /* 0108000d //C 0f1f1 cmp	r0,13 */
0x0000f1f2, 0x11f2f23c, /* 11f2f23c //C 0f1f2 jz	l_eol */
0x0000f1f3, 0x01a80000, /* 01a80000 //C 0f1f3 cmp	r10,0 */
0x0000f1f4, 0x21f2f204, /* 21f2f204 //C 0f1f4 jnz	l_no0 */
0x0000f1f5, 0x0400f3eb, /* 0400f3eb //C 0f1f5 call	hexchar2value */
0x0000f1f6, 0x41f2f1fe, /* 41f2f1fe //C 0f1f6 C0 jmp	l_eof_0 */
0x0000f1f7, 0x02880000, /* 02880000 //C 0f1f7 shl	r8		; shift val(char) into value */
0x0000f1f8, 0x02880000, /* 02880000 //C 0f1f8 shl	r8 */
0x0000f1f9, 0x02880000, /* 02880000 //C 0f1f9 shl	r8 */
0x0000f1fa, 0x02880000, /* 02880000 //C 0f1fa shl	r8 */
0x0000f1fb, 0x010b000f, /* 010b000f //C 0f1fb btst	r0,0xf */
0x0000f1fc, 0x008d0000, /* 008d0000 //C 0f1fc or	r8,r0 */
0x0000f1fd, 0x01f2f1eb, /* 01f2f1eb //C 0f1fd jmp	l_cyc */
0x0000f1fe, 0x01a20001, /* 01a20001 //C 0f1fe mvzl	r10,1		; state0 -> state1 */
0x0000f1ff, 0x0162003f, /* 0162003f //C 0f1ff mvzl	r6,'?'		; No //C yet */
0x0000f200, 0x01b8002f, /* 01b8002f //C 0f200 cmp	r11,'/'		; is it start of // */
0x0000f201, 0x11720001, /* 11720001 //C 0f201 z1 mvzl	r7,1		; Yes, first / found */
0x0000f202, 0x21720000, /* 21720000 //C 0f202 z0 mvzl	r7,0		; No '/' yet */
0x0000f203, 0x01f2f1eb, /* 01f2f1eb //C 0f203 jmp	l_cyc */
0x0000f204, 0x01a80001, /* 01a80001 //C 0f204 cmp	r10,1 */
0x0000f205, 0x21f2f21f, /* 21f2f21f //C 0f205 jnz	l_no1 */
0x0000f206, 0x027b0000, /* 027b0000 //C 0f206 sz	r7 */
0x0000f207, 0x21f2f20c, /* 21f2f20c //C 0f207 jnz	l_s1_2nd */
0x0000f208, 0x0108002f, /* 0108002f //C 0f208 cmp	r0,'/' */
0x0000f209, 0x21f2f1eb, /* 21f2f1eb //C 0f209 jnz	l_cyc */
0x0000f20a, 0x01720001, /* 01720001 //C 0f20a mvzl	r7,1 */
0x0000f20b, 0x01f2f1eb, /* 01f2f1eb //C 0f20b jmp	l_cyc */
0x0000f20c, 0x0108002f, /* 0108002f //C 0f20c cmp	r0,'/' */
0x0000f20d, 0x11f2f1eb, /* 11f2f1eb //C 0f20d jz	l_cyc */
0x0000f20e, 0x01080043, /* 01080043 //C 0f20e cmp	r0,'C' */
0x0000f20f, 0x11f2f212, /* 11f2f212 //C 0f20f jz	l_s1_C */
0x0000f210, 0x01080049, /* 01080049 //C 0f210 cmp	r0,'I' */
0x0000f211, 0x21f2f217, /* 21f2f217 //C 0f211 jnz	l_s1_noC */
0x0000f212, 0x00600000, /* 00600000 //C 0f212 mov	r6,r0		; record type is in r0, store in r6 */
0x0000f213, 0x01a20002, /* 01a20002 //C 0f213 mvzl	r10,2 */
0x0000f214, 0x01920000, /* 01920000 //C 0f214 mvzl	r9,0		; address= 0 */
0x0000f215, 0x01520000, /* 01520000 //C 0f215 mvzl	r5,0		; where we are in word: before */
0x0000f216, 0x01f2f1eb, /* 01f2f1eb //C 0f216 jmp	l_cyc */
0x0000f217, 0x01080045, /* 01080045 //C 0f217 cmp	r0,'E' */
0x0000f218, 0x21f2f21c, /* 21f2f21c //C 0f218 jnz	l_s1_noE */
0x0000f219, 0x01620045, /* 01620045 //C 0f219 mvzl	r6,'E' */
0x0000f21a, 0x01a20003, /* 01a20003 //C 0f21a mvzl	r10,3 */
0x0000f21b, 0x01f2f1eb, /* 01f2f1eb //C 0f21b jmp	l_cyc */
0x0000f21c, 0x0400f44c, /* 0400f44c //C 0f21c call	putchar		; print record type */
0x0000f21d, 0x01a2000f, /* 01a2000f //C 0f21d mvzl	r10,0xf		; special state: skip everything */
0x0000f21e, 0x01f2f1eb, /* 01f2f1eb //C 0f21e jmp	l_cyc */
0x0000f21f, 0x01a80002, /* 01a80002 //C 0f21f cmp	r10,2 */
0x0000f220, 0x21f2f234, /* 21f2f234 //C 0f220 jnz	l_no2 */
0x0000f221, 0x01580000, /* 01580000 //C 0f221 cmp	r5,0 */
0x0000f222, 0x21f2f22d, /* 21f2f22d //C 0f222 jnz	l_s2_no0 */
0x0000f223, 0x0400f3eb, /* 0400f3eb //C 0f223 call	hexchar2value */
0x0000f224, 0x41f2f1eb, /* 41f2f1eb //C 0f224 C0 jmp	l_cyc */
0x0000f225, 0x01520001, /* 01520001 //C 0f225 mvzl	r5,1 */
0x0000f226, 0x02980000, /* 02980000 //C 0f226 shl	r9 */
0x0000f227, 0x02980000, /* 02980000 //C 0f227 shl	r9 */
0x0000f228, 0x02980000, /* 02980000 //C 0f228 shl	r9 */
0x0000f229, 0x02980000, /* 02980000 //C 0f229 shl	r9 */
0x0000f22a, 0x010b000f, /* 010b000f //C 0f22a btst	r0,0xf */
0x0000f22b, 0x009d0000, /* 009d0000 //C 0f22b or	r9,r0 */
0x0000f22c, 0x01f2f1eb, /* 01f2f1eb //C 0f22c jmp	l_cyc */
0x0000f22d, 0x01580001, /* 01580001 //C 0f22d cmp	r5,1 */
0x0000f22e, 0x21f2f233, /* 21f2f233 //C 0f22e jnz	l_s2_no1 */
0x0000f22f, 0x0400f3eb, /* 0400f3eb //C 0f22f call	hexchar2value */
0x0000f230, 0x31f2f226, /* 31f2f226 //C 0f230 C1 jmp	l_s2_got */
0x0000f231, 0x01520002, /* 01520002 //C 0f231 mvzl	r5,2 */
0x0000f232, 0x01f2f1eb, /* 01f2f1eb //C 0f232 jmp	l_cyc */
0x0000f233, 0x01f2f1eb, /* 01f2f1eb //C 0f233 jmp	l_cyc */
0x0000f234, 0x01a80003, /* 01a80003 //C 0f234 cmp	r10,3 */
0x0000f235, 0x21f2f237, /* 21f2f237 //C 0f235 jnz	l_no3 */
0x0000f236, 0x01f2f1eb, /* 01f2f1eb //C 0f236 jmp	l_cyc		; do nothing, just wait EOL */
0x0000f237, 0x01a8000f, /* 01a8000f //C 0f237 cmp	r10,0xf */
0x0000f238, 0x01f2f23a, /* 01f2f23a //C 0f238 jmp	l_nof */
0x0000f239, 0x01f2f1eb, /* 01f2f1eb //C 0f239 jmp	l_cyc		; just skip */
0x0000f23a, 0x01f2f1eb, /* 01f2f1eb //C 0f23a jmp	l_cyc */
0x0000f23b, 0x01f2f254, /* 01f2f254 //C 0f23b jmp	l_ret */
0x0000f23c, 0x01a80000, /* 01a80000 //C 0f23c cmp	r10,0		; in state0 */
0x0000f23d, 0x11f2f250, /* 11f2f250 //C 0f23d jz	l_back_to_0	; just restart */
0x0000f23e, 0x01a80001, /* 01a80001 //C 0f23e cmp	r10,1		; in state1 */
0x0000f23f, 0x11f2f250, /* 11f2f250 //C 0f23f jz	l_back_to_0 	;l_cyc ;l_bad ; garbage */
0x0000f240, 0x01a80002, /* 01a80002 //C 0f240 cmp	r10,2		; in state2 */
0x0000f241, 0x11f2f248, /* 11f2f248 //C 0f241 jz	l_proc		; process record */
0x0000f242, 0x01a80003, /* 01a80003 //C 0f242 cmp	r10,3		; in state3 */
0x0000f243, 0x11f2f254, /* 11f2f254 //C 0f243 jz	l_ret		; eol in end record: finish */
0x0000f244, 0x01a8000f, /* 01a8000f //C 0f244 cmp	r10,0xf		; in state skip */
0x0000f245, 0x11f2f250, /* 11f2f250 //C 0f245 jz	l_back_to_0	; reset state for new line */
0x0000f246, 0x01f2f1eb, /* 01f2f1eb //C 0f246 jmp	l_cyc */
0x0000f247, 0x01f2f254, /* 01f2f254 //C 0f247 jmp	l_ret */
0x0000f248, 0x01680043, /* 01680043 //C 0f248 cmp	r6,'C'		; is it a C or I record? */
0x0000f249, 0x1c890000, /* 1c890000 //C 0f249 z st	r8,r9		; then store */
0x0000f24a, 0x10c40800, /* 10c40800 //C 0f24a z add	r12,r8		; and add to checksum */
0x0000f24b, 0x01680049, /* 01680049 //C 0f24b cmp	r6,'I' */
0x0000f24c, 0x1c890000, /* 1c890000 //C 0f24c z st	r8,r9 */
0x0000f24d, 0x10c40800, /* 10c40800 //C 0f24d z add	r12,r8 */
0x0000f24e, 0x00000600, /* 00000600 //C 0f24e mov	r0,r6		; echo record type */
0x0000f24f, 0x0400f44c, /* 0400f44c //C 0f24f call	putchar */
0x0000f250, 0x01a20000, /* 01a20000 //C 0f250 mvzl	r10,0 */
0x0000f251, 0x01820000, /* 01820000 //C 0f251 mvzl	r8,0 */
0x0000f252, 0x0162003f, /* 0162003f //C 0f252 mvzl	r6,'?' */
0x0000f253, 0x01f2f1eb, /* 01f2f1eb //C 0f253 jmp	l_cyc */
0x0000f254, 0x0102000a, /* 0102000a //C 0f254 mvzl	r0,LF */
0x0000f255, 0x0400f44c, /* 0400f44c //C 0f255 call	putchar */
0x0000f256, 0x00000c00, /* 00000c00 //C 0f256 mov	r0,r12 */
0x0000f257, 0x01120004, /* 01120004 //C 0f257 mvzl	r1,4 */
0x0000f258, 0x0400f47d, /* 0400f47d //C 0f258 call	print_vhex */
0x0000f259, 0x0102000a, /* 0102000a //C 0f259 mvzl	r0,LF */
0x0000f25a, 0x0400f44c, /* 0400f44c //C 0f25a call	putchar */
0x0000f25b, 0x0ffd0000, /* 0ffd0000 //C 0f25b pop	pc */
0x0000f25c, 0x0ded0000, /* 0ded0000 //C 0f25c push	lr */
0x0000f25d, 0x0122f5d2, /* 0122f5d2 //C 0f25d mvzl	r2,words */
0x0000f25e, 0x0e020001, /* 0e020001 //C 0f25e ld	r0,r2,1		; address */
0x0000f25f, 0x020b0000, /* 020b0000 //C 0f25f sz	r0 */
0x0000f260, 0x11f2f283, /* 11f2f283 //C 0f260 jz	g_no_addr */
0x0000f261, 0x0400f41e, /* 0400f41e //C 0f261 call	htoi */
0x0000f262, 0x00b00100, /* 00b00100 //C 0f262 mov	r11,r1 */
0x0000f263, 0x0102f296, /* 0102f296 //C 0f263 mvzl	r0,d_msg_run */
0x0000f264, 0x0400f453, /* 0400f453 //C 0f264 call	prints */
0x0000f265, 0x00000b00, /* 00000b00 //C 0f265 mov	r0,r11 */
0x0000f266, 0x01120004, /* 01120004 //C 0f266 mvzl	r1,4 */
0x0000f267, 0x0400f47d, /* 0400f47d //C 0f267 call	print_vhex */
0x0000f268, 0x0102000a, /* 0102000a //C 0f268 mvzl	r0,LF */
0x0000f269, 0x0400f44c, /* 0400f44c //C 0f269 call	putchar */
0x0000f26a, 0x06b0f5e9, /* 06b0f5e9 //C 0f26a st	r11,reg15 */
0x0000f26b, 0x0122ff43, /* 0122ff43 //C 0f26b mvzl	r2,UART_TSTAT */
0x0000f26c, 0x0e920000, /* 0e920000 //C 0f26c ld	r9,r2 */
0x0000f26d, 0x019c0001, /* 019c0001 //C 0f26d test	r9,1 */
0x0000f26e, 0x11f2f26c, /* 11f2f26c //C 0f26e jz	g_wait_tc */
0x0000f26f, 0x07000000, /* 07000000 //C 0f26f ld	r0,0 */
0x0000f270, 0x0600f5d8, /* 0600f5d8 //C 0f270 st	r0,called */
0x0000f271, 0x0700f5ea, /* 0700f5ea //C 0f271 ld	r0,regf */
0x0000f272, 0x020f0000, /* 020f0000 //C 0f272 setf	r0 */
0x0000f273, 0x0700f5da, /* 0700f5da //C 0f273 ld	r0,reg0 */
0x0000f274, 0x0710f5db, /* 0710f5db //C 0f274 ld	r1,reg1 */
0x0000f275, 0x0720f5dc, /* 0720f5dc //C 0f275 ld	r2,reg2 */
0x0000f276, 0x0730f5dd, /* 0730f5dd //C 0f276 ld	r3,reg3 */
0x0000f277, 0x0740f5de, /* 0740f5de //C 0f277 ld	r4,reg4 */
0x0000f278, 0x0750f5df, /* 0750f5df //C 0f278 ld	r5,reg5 */
0x0000f279, 0x0760f5e0, /* 0760f5e0 //C 0f279 ld	r6,reg6 */
0x0000f27a, 0x0770f5e1, /* 0770f5e1 //C 0f27a ld	r7,reg7 */
0x0000f27b, 0x0780f5e2, /* 0780f5e2 //C 0f27b ld	r8,reg8 */
0x0000f27c, 0x0790f5e3, /* 0790f5e3 //C 0f27c ld	r9,reg9 */
0x0000f27d, 0x07a0f5e4, /* 07a0f5e4 //C 0f27d ld	r10,reg10 */
0x0000f27e, 0x07b0f5e5, /* 07b0f5e5 //C 0f27e ld	r11,reg11 */
0x0000f27f, 0x07c0f5e6, /* 07c0f5e6 //C 0f27f ld	r12,reg12 */
0x0000f280, 0x07d0f5e7, /* 07d0f5e7 //C 0f280 ld	r13,reg13 */
0x0000f281, 0x07e0f5e8, /* 07e0f5e8 //C 0f281 ld	r14,reg14 */
0x0000f282, 0x07f0f5e9, /* 07f0f5e9 //C 0f282 ld	r15,reg15 */
0x0000f283, 0x0700f5d8, /* 0700f5d8 //C 0f283 ld	r0,called */
0x0000f284, 0x020b0000, /* 020b0000 //C 0f284 sz	r0 */
0x0000f285, 0x11f2f288, /* 11f2f288 //C 0f285 jz	g_err */
0x0000f286, 0x07b0f5e9, /* 07b0f5e9 //C 0f286 ld	r11,reg15 */
0x0000f287, 0x01f2f263, /* 01f2f263 //C 0f287 jmp	g_go11 */
0x0000f288, 0x0102f28b, /* 0102f28b //C 0f288 mvzl	r0,g_err_addr */
0x0000f289, 0x0400f476, /* 0400f476 //C 0f289 call	printsnl */
0x0000f28a, 0x0ffd0000, /* 0ffd0000 //C 0f28a pop	pc */
0x0000f28b, 0x0000004e, /* 0000004e //C 0f28b db	78 */
0x0000f28c, 0x0000006f, /* 0000006f //C 0f28c db	111 */
0x0000f28d, 0x00000020, /* 00000020 //C 0f28d db	32 */
0x0000f28e, 0x00000061, /* 00000061 //C 0f28e db	97 */
0x0000f28f, 0x00000064, /* 00000064 //C 0f28f db	100 */
0x0000f290, 0x00000064, /* 00000064 //C 0f290 db	100 */
0x0000f291, 0x00000072, /* 00000072 //C 0f291 db	114 */
0x0000f292, 0x00000065, /* 00000065 //C 0f292 db	101 */
0x0000f293, 0x00000073, /* 00000073 //C 0f293 db	115 */
0x0000f294, 0x00000073, /* 00000073 //C 0f294 db	115 */
0x0000f295, 0x00000000, /* 00000000 //C 0f295 db */
0x0000f296, 0x00000052, /* 00000052 //C 0f296 db	82 */
0x0000f297, 0x00000075, /* 00000075 //C 0f297 db	117 */
0x0000f298, 0x0000006e, /* 0000006e //C 0f298 db	110 */
0x0000f299, 0x00000020, /* 00000020 //C 0f299 db	32 */
0x0000f29a, 0x00000000, /* 00000000 //C 0f29a db */
0x0000f29b, 0x0ded0000, /* 0ded0000 //C 0f29b push	lr */
0x0000f29c, 0x0122f67a, /* 0122f67a //C 0f29c mvzl	r2,helps */
0x0000f29d, 0x01320000, /* 01320000 //C 0f29d mvzl	r3,0 */
0x0000f29e, 0x0b038200, /* 0b038200 //C 0f29e ld	r0,r3+,r2	; pick a char */
0x0000f29f, 0x020b0000, /* 020b0000 //C 0f29f sz	r0		; is it eos? */
0x0000f2a0, 0x21f2f2a4, /* 21f2f2a4 //C 0f2a0 jnz	h_print */
0x0000f2a1, 0x0b038200, /* 0b038200 //C 0f2a1 ld	r0,r3+,r2	; get first char of next string */
0x0000f2a2, 0x020b0000, /* 020b0000 //C 0f2a2 sz	r0 */
0x0000f2a3, 0x11f2f2a6, /* 11f2f2a6 //C 0f2a3 jz	h_eof */
0x0000f2a4, 0x0400f44c, /* 0400f44c //C 0f2a4 call	putchar */
0x0000f2a5, 0x01f2f29e, /* 01f2f29e //C 0f2a5 jmp	h_cyc */
0x0000f2a6, 0x0ffd0000, /* 0ffd0000 //C 0f2a6 pop	pc */
0x0000f2a7, 0x0ded0000, /* 0ded0000 //C 0f2a7 push	lr */
0x0000f2a8, 0x0d1d0000, /* 0d1d0000 //C 0f2a8 push	r1 */
0x0000f2a9, 0x00100000, /* 00100000 //C 0f2a9 mov	r1,r0 */
0x0000f2aa, 0x01180010, /* 01180010 //C 0f2aa cmp	r1,16		; go out if nr>16 */
0x0000f2ab, 0x91f2f2df, /* 91f2f2df //C 0f2ab HI jmp	prn_ret */
0x0000f2ac, 0x0118000f, /* 0118000f //C 0f2ac cmp	r1,15		; nr=Flag? */
0x0000f2ad, 0xa1f2f2b4, /* a1f2f2b4 //C 0f2ad LS jmp	prn_015 */
0x0000f2ae, 0x01020046, /* 01020046 //C 0f2ae mvzl	r0,'F' */
0x0000f2af, 0x0400f44c, /* 0400f44c //C 0f2af call	putchar */
0x0000f2b0, 0x01020020, /* 01020020 //C 0f2b0 mvzl	r0,32 */
0x0000f2b1, 0x0400f44c, /* 0400f44c //C 0f2b1 call	putchar */
0x0000f2b2, 0x0400f44c, /* 0400f44c //C 0f2b2 call	putchar */
0x0000f2b3, 0x01f2f2df, /* 01f2f2df //C 0f2b3 jmp	prn_ret */
0x0000f2b4, 0x0118000f, /* 0118000f //C 0f2b4 cmp	r1,15 */
0x0000f2b5, 0x21f2f2bd, /* 21f2f2bd //C 0f2b5 jnz	prn_no15 */
0x0000f2b6, 0x0102f2b9, /* 0102f2b9 //C 0f2b6 mvzl	r0,prn_PC */
0x0000f2b7, 0x0400f453, /* 0400f453 //C 0f2b7 call	prints */
0x0000f2b8, 0x01f2f2df, /* 01f2f2df //C 0f2b8 jmp	prn_ret */
0x0000f2b9, 0x00000050, /* 00000050 //C 0f2b9 db	80 */
0x0000f2ba, 0x00000043, /* 00000043 //C 0f2ba db	67 */
0x0000f2bb, 0x00000020, /* 00000020 //C 0f2bb db	32 */
0x0000f2bc, 0x00000000, /* 00000000 //C 0f2bc db */
0x0000f2bd, 0x0118000e, /* 0118000e //C 0f2bd cmp	r1,14 */
0x0000f2be, 0x21f2f2c6, /* 21f2f2c6 //C 0f2be jnz	prn_no14 */
0x0000f2bf, 0x0102f2c2, /* 0102f2c2 //C 0f2bf mvzl	r0,prn_LR */
0x0000f2c0, 0x0400f453, /* 0400f453 //C 0f2c0 call	prints */
0x0000f2c1, 0x01f2f2df, /* 01f2f2df //C 0f2c1 jmp	prn_ret */
0x0000f2c2, 0x0000004c, /* 0000004c //C 0f2c2 db	76 */
0x0000f2c3, 0x00000052, /* 00000052 //C 0f2c3 db	82 */
0x0000f2c4, 0x00000020, /* 00000020 //C 0f2c4 db	32 */
0x0000f2c5, 0x00000000, /* 00000000 //C 0f2c5 db */
0x0000f2c6, 0x0118000d, /* 0118000d //C 0f2c6 cmp	r1,13 */
0x0000f2c7, 0x21f2f2cf, /* 21f2f2cf //C 0f2c7 jnz	prn_no13 */
0x0000f2c8, 0x0102f2cb, /* 0102f2cb //C 0f2c8 mvzl	r0,prn_SP */
0x0000f2c9, 0x0400f453, /* 0400f453 //C 0f2c9 call	prints */
0x0000f2ca, 0x01f2f2df, /* 01f2f2df //C 0f2ca jmp	prn_ret */
0x0000f2cb, 0x00000053, /* 00000053 //C 0f2cb db	83 */
0x0000f2cc, 0x00000050, /* 00000050 //C 0f2cc db	80 */
0x0000f2cd, 0x00000020, /* 00000020 //C 0f2cd db	32 */
0x0000f2ce, 0x00000000, /* 00000000 //C 0f2ce db */
0x0000f2cf, 0x01020052, /* 01020052 //C 0f2cf mvzl	r0,'R' */
0x0000f2d0, 0x0400f44c, /* 0400f44c //C 0f2d0 call	putchar */
0x0000f2d1, 0x01180009, /* 01180009 //C 0f2d1 cmp	r1,9 */
0x0000f2d2, 0x91f2f2d9, /* 91f2f2d9 //C 0f2d2 HI jmp	prn_1015 */
0x0000f2d3, 0x00000100, /* 00000100 //C 0f2d3 mov	r0,r1 */
0x0000f2d4, 0x01040030, /* 01040030 //C 0f2d4 add	r0,'0' */
0x0000f2d5, 0x0400f44c, /* 0400f44c //C 0f2d5 call	putchar */
0x0000f2d6, 0x01020020, /* 01020020 //C 0f2d6 mvzl	r0,32 */
0x0000f2d7, 0x0400f44c, /* 0400f44c //C 0f2d7 call	putchar */
0x0000f2d8, 0x01f2f2df, /* 01f2f2df //C 0f2d8 jmp	prn_ret */
0x0000f2d9, 0x01020031, /* 01020031 //C 0f2d9 mvzl	r0,'1' */
0x0000f2da, 0x0400f44c, /* 0400f44c //C 0f2da call	putchar */
0x0000f2db, 0x00000100, /* 00000100 //C 0f2db mov	r0,r1 */
0x0000f2dc, 0x0106000a, /* 0106000a //C 0f2dc sub	r0,10 */
0x0000f2dd, 0x01040030, /* 01040030 //C 0f2dd add	r0,'0' */
0x0000f2de, 0x0400f44c, /* 0400f44c //C 0f2de call	putchar */
0x0000f2df, 0x0f1d0000, /* 0f1d0000 //C 0f2df pop	r1 */
0x0000f2e0, 0x0ffd0000, /* 0ffd0000 //C 0f2e0 pop	pc */
0x0000f2e1, 0x0ded0000, /* 0ded0000 //C 0f2e1 push	lr */
0x0000f2e2, 0x0d1d0000, /* 0d1d0000 //C 0f2e2 push	r1 */
0x0000f2e3, 0x01080010, /* 01080010 //C 0f2e3 cmp	r0,16 */
0x0000f2e4, 0x91f2f2e9, /* 91f2f2e9 //C 0f2e4 HI jmp	prv_ret */
0x0000f2e5, 0x0112f5da, /* 0112f5da //C 0f2e5 mvzl	r1,reg0 */
0x0000f2e6, 0x0a010000, /* 0a010000 //C 0f2e6 ld	r0,r1,r0 */
0x0000f2e7, 0x01120004, /* 01120004 //C 0f2e7 mvzl	r1,4 */
0x0000f2e8, 0x0400f47d, /* 0400f47d //C 0f2e8 call	print_vhex */
0x0000f2e9, 0x0f1d0000, /* 0f1d0000 //C 0f2e9 pop	r1 */
0x0000f2ea, 0x0ffd0000, /* 0ffd0000 //C 0f2ea pop	pc */
0x0000f2eb, 0x0ded0000, /* 0ded0000 //C 0f2eb push	lr */
0x0000f2ec, 0x0dad0000, /* 0dad0000 //C 0f2ec push	r10 */
0x0000f2ed, 0x0d0d0000, /* 0d0d0000 //C 0f2ed push	r0 */
0x0000f2ee, 0x00000a00, /* 00000a00 //C 0f2ee mov	r0,r10 */
0x0000f2ef, 0x0400f2a7, /* 0400f2a7 //C 0f2ef call	print_reg_name */
0x0000f2f0, 0x01020020, /* 01020020 //C 0f2f0 mvzl	r0,32 */
0x0000f2f1, 0x0400f44c, /* 0400f44c //C 0f2f1 call	putchar */
0x0000f2f2, 0x00000a00, /* 00000a00 //C 0f2f2 mov	r0,r10 */
0x0000f2f3, 0x0400f2e1, /* 0400f2e1 //C 0f2f3 call	print_reg_value */
0x0000f2f4, 0x0102000a, /* 0102000a //C 0f2f4 mvzl	r0,LF */
0x0000f2f5, 0x0400f44c, /* 0400f44c //C 0f2f5 call	putchar */
0x0000f2f6, 0x0f0d0000, /* 0f0d0000 //C 0f2f6 pop	r0 */
0x0000f2f7, 0x0fad0000, /* 0fad0000 //C 0f2f7 pop	r10 */
0x0000f2f8, 0x0ffd0000, /* 0ffd0000 //C 0f2f8 pop	pc */
0x0000f2f9, 0x0ded0000, /* 0ded0000 //C 0f2f9 push	lr */
0x0000f2fa, 0x0400f44c, /* 0400f44c //C 0f2fa call	putchar */
0x0000f2fb, 0x001c0200, /* 001c0200 //C 0f2fb test	r1,r2 */
0x0000f2fc, 0x21020031, /* 21020031 //C 0f2fc NZ mvzl r0,'1' */
0x0000f2fd, 0x11020030, /* 11020030 //C 0f2fd Z  mvzl r0,'0' */
0x0000f2fe, 0x0400f44c, /* 0400f44c //C 0f2fe call	putchar */
0x0000f2ff, 0x00000300, /* 00000300 //C 0f2ff mov	r0,r3 */
0x0000f300, 0x0400f44c, /* 0400f44c //C 0f300 call	putchar */
0x0000f301, 0x0ffd0000, /* 0ffd0000 //C 0f301 pop	pc */
0x0000f302, 0x0ded0000, /* 0ded0000 //C 0f302 push	lr */
0x0000f303, 0x0700f5d8, /* 0700f5d8 //C 0f303 ld	r0,called */
0x0000f304, 0x020b0000, /* 020b0000 //C 0f304 sz	r0 */
0x0000f305, 0x11f2f30e, /* 11f2f30e //C 0f305 jz	r_not_called */
0x0000f306, 0x0102f32b, /* 0102f32b //C 0f306 mvzl	r0,msg_r_called */
0x0000f307, 0x0400f453, /* 0400f453 //C 0f307 call	prints */
0x0000f308, 0x0700f5e9, /* 0700f5e9 //C 0f308 ld	r0,reg15 */
0x0000f309, 0x01120004, /* 01120004 //C 0f309 mvzl	r1,4 */
0x0000f30a, 0x0400f47d, /* 0400f47d //C 0f30a call	print_vhex */
0x0000f30b, 0x0102000a, /* 0102000a //C 0f30b mvzl	r0,LF */
0x0000f30c, 0x0400f44c, /* 0400f44c //C 0f30c call	putchar */
0x0000f30d, 0x01f2f310, /* 01f2f310 //C 0f30d jmp	r_start */
0x0000f30e, 0x0102f341, /* 0102f341 //C 0f30e mvzl	r0,msg_r_notcalled */
0x0000f30f, 0x0400f453, /* 0400f453 //C 0f30f call	prints */
0x0000f310, 0x01a20000, /* 01a20000 //C 0f310 mvzl	r10,0 */
0x0000f311, 0x0400f2eb, /* 0400f2eb //C 0f311 call	print_reg_name_value */
0x0000f312, 0x01a40001, /* 01a40001 //C 0f312 add	r10,1 */
0x0000f313, 0x01a80011, /* 01a80011 //C 0f313 cmp	r10,17 */
0x0000f314, 0x21f2f311, /* 21f2f311 //C 0f314 jnz	r_cyc */
0x0000f315, 0x0710f5ea, /* 0710f5ea //C 0f315 ld	r1,regf */
0x0000f316, 0x01320020, /* 01320020 //C 0f316 mvzl	r3,32 */
0x0000f317, 0x01020055, /* 01020055 //C 0f317 mvzl	r0,'U' */
0x0000f318, 0x01220020, /* 01220020 //C 0f318 mvzl	r2,0x20 */
0x0000f319, 0x0400f2f9, /* 0400f2f9 //C 0f319 call	print_flag */
0x0000f31a, 0x01020050, /* 01020050 //C 0f31a mvzl	r0,'P' */
0x0000f31b, 0x01220010, /* 01220010 //C 0f31b mvzl	r2,0x10 */
0x0000f31c, 0x0400f2f9, /* 0400f2f9 //C 0f31c call	print_flag */
0x0000f31d, 0x0102004f, /* 0102004f //C 0f31d mvzl	r0,'O' */
0x0000f31e, 0x01220008, /* 01220008 //C 0f31e mvzl	r2,8 */
0x0000f31f, 0x0400f2f9, /* 0400f2f9 //C 0f31f call	print_flag */
0x0000f320, 0x0102005a, /* 0102005a //C 0f320 mvzl	r0,'Z' */
0x0000f321, 0x01220004, /* 01220004 //C 0f321 mvzl	r2,4 */
0x0000f322, 0x0400f2f9, /* 0400f2f9 //C 0f322 call	print_flag */
0x0000f323, 0x01020043, /* 01020043 //C 0f323 mvzl	r0,'C' */
0x0000f324, 0x01220002, /* 01220002 //C 0f324 mvzl	r2,2 */
0x0000f325, 0x0400f2f9, /* 0400f2f9 //C 0f325 call	print_flag */
0x0000f326, 0x01020053, /* 01020053 //C 0f326 mvzl	r0,'S' */
0x0000f327, 0x01220001, /* 01220001 //C 0f327 mvzl	r2,1 */
0x0000f328, 0x0132000a, /* 0132000a //C 0f328 mvzl	r3,LF */
0x0000f329, 0x0400f2f9, /* 0400f2f9 //C 0f329 call	print_flag */
0x0000f32a, 0x0ffd0000, /* 0ffd0000 //C 0f32a pop	pc */
0x0000f32b, 0x0000004d, /* 0000004d //C 0f32b db	77 */
0x0000f32c, 0x0000006f, /* 0000006f //C 0f32c db	111 */
0x0000f32d, 0x0000006e, /* 0000006e //C 0f32d db	110 */
0x0000f32e, 0x00000069, /* 00000069 //C 0f32e db	105 */
0x0000f32f, 0x00000074, /* 00000074 //C 0f32f db	116 */
0x0000f330, 0x0000006f, /* 0000006f //C 0f330 db	111 */
0x0000f331, 0x00000072, /* 00000072 //C 0f331 db	114 */
0x0000f332, 0x00000020, /* 00000020 //C 0f332 db	32 */
0x0000f333, 0x00000063, /* 00000063 //C 0f333 db	99 */
0x0000f334, 0x00000061, /* 00000061 //C 0f334 db	97 */
0x0000f335, 0x0000006c, /* 0000006c //C 0f335 db	108 */
0x0000f336, 0x0000006c, /* 0000006c //C 0f336 db	108 */
0x0000f337, 0x00000065, /* 00000065 //C 0f337 db	101 */
0x0000f338, 0x00000064, /* 00000064 //C 0f338 db	100 */
0x0000f339, 0x00000020, /* 00000020 //C 0f339 db	32 */
0x0000f33a, 0x00000066, /* 00000066 //C 0f33a db	102 */
0x0000f33b, 0x00000072, /* 00000072 //C 0f33b db	114 */
0x0000f33c, 0x0000006f, /* 0000006f //C 0f33c db	111 */
0x0000f33d, 0x0000006d, /* 0000006d //C 0f33d db	109 */
0x0000f33e, 0x0000003a, /* 0000003a //C 0f33e db	58 */
0x0000f33f, 0x00000020, /* 00000020 //C 0f33f db	32 */
0x0000f340, 0x00000000, /* 00000000 //C 0f340 db */
0x0000f341, 0x0000004d, /* 0000004d //C 0f341 db	77 */
0x0000f342, 0x0000006f, /* 0000006f //C 0f342 db	111 */
0x0000f343, 0x0000006e, /* 0000006e //C 0f343 db	110 */
0x0000f344, 0x00000069, /* 00000069 //C 0f344 db	105 */
0x0000f345, 0x00000074, /* 00000074 //C 0f345 db	116 */
0x0000f346, 0x0000006f, /* 0000006f //C 0f346 db	111 */
0x0000f347, 0x00000072, /* 00000072 //C 0f347 db	114 */
0x0000f348, 0x00000020, /* 00000020 //C 0f348 db	32 */
0x0000f349, 0x0000006e, /* 0000006e //C 0f349 db	110 */
0x0000f34a, 0x0000006f, /* 0000006f //C 0f34a db	111 */
0x0000f34b, 0x00000074, /* 00000074 //C 0f34b db	116 */
0x0000f34c, 0x00000020, /* 00000020 //C 0f34c db	32 */
0x0000f34d, 0x00000063, /* 00000063 //C 0f34d db	99 */
0x0000f34e, 0x00000061, /* 00000061 //C 0f34e db	97 */
0x0000f34f, 0x0000006c, /* 0000006c //C 0f34f db	108 */
0x0000f350, 0x0000006c, /* 0000006c //C 0f350 db	108 */
0x0000f351, 0x00000065, /* 00000065 //C 0f351 db	101 */
0x0000f352, 0x00000064, /* 00000064 //C 0f352 db	100 */
0x0000f353, 0x00000020, /* 00000020 //C 0f353 db	32 */
0x0000f354, 0x00000062, /* 00000062 //C 0f354 db	98 */
0x0000f355, 0x00000079, /* 00000079 //C 0f355 db	121 */
0x0000f356, 0x00000020, /* 00000020 //C 0f356 db	32 */
0x0000f357, 0x00000075, /* 00000075 //C 0f357 db	117 */
0x0000f358, 0x00000073, /* 00000073 //C 0f358 db	115 */
0x0000f359, 0x00000065, /* 00000065 //C 0f359 db	101 */
0x0000f35a, 0x00000072, /* 00000072 //C 0f35a db	114 */
0x0000f35b, 0x0000000a, /* 0000000a //C 0f35b db	10 */
0x0000f35c, 0x00000000, /* 00000000 //C 0f35c db */
0x0000f35d, 0x0ded0000, /* 0ded0000 //C 0f35d push	lr */
0x0000f35e, 0x07a0f5d9, /* 07a0f5d9 //C 0f35e ld	r10,nuof_reg	; Reg num is in R10 */
0x0000f35f, 0x01a80010, /* 01a80010 //C 0f35f cmp	r10,16 */
0x0000f360, 0xa1f2f375, /* a1f2f375 //C 0f360 LS jmp	rx_nr_ok */
0x0000f361, 0x0102f364, /* 0102f364 //C 0f361 mvzl	r0,rx_err_nr */
0x0000f362, 0x0400f476, /* 0400f476 //C 0f362 call	printsnl */
0x0000f363, 0x01f2f391, /* 01f2f391 //C 0f363 jmp	rx_ret */
0x0000f364, 0x0000004e, /* 0000004e //C 0f364 db	78 */
0x0000f365, 0x0000006f, /* 0000006f //C 0f365 db	111 */
0x0000f366, 0x00000020, /* 00000020 //C 0f366 db	32 */
0x0000f367, 0x00000073, /* 00000073 //C 0f367 db	115 */
0x0000f368, 0x00000075, /* 00000075 //C 0f368 db	117 */
0x0000f369, 0x00000063, /* 00000063 //C 0f369 db	99 */
0x0000f36a, 0x00000068, /* 00000068 //C 0f36a db	104 */
0x0000f36b, 0x00000020, /* 00000020 //C 0f36b db	32 */
0x0000f36c, 0x00000072, /* 00000072 //C 0f36c db	114 */
0x0000f36d, 0x00000065, /* 00000065 //C 0f36d db	101 */
0x0000f36e, 0x00000067, /* 00000067 //C 0f36e db	103 */
0x0000f36f, 0x00000069, /* 00000069 //C 0f36f db	105 */
0x0000f370, 0x00000073, /* 00000073 //C 0f370 db	115 */
0x0000f371, 0x00000074, /* 00000074 //C 0f371 db	116 */
0x0000f372, 0x00000065, /* 00000065 //C 0f372 db	101 */
0x0000f373, 0x00000072, /* 00000072 //C 0f373 db	114 */
0x0000f374, 0x00000000, /* 00000000 //C 0f374 db */
0x0000f375, 0x0122f5d2, /* 0122f5d2 //C 0f375 mvzl	r2,words */
0x0000f376, 0x0e420001, /* 0e420001 //C 0f376 ld	r4,r2,1		; get aof first parameter */
0x0000f377, 0x024b0000, /* 024b0000 //C 0f377 sz	r4		; is it NULL? */
0x0000f378, 0x11f2f390, /* 11f2f390 //C 0f378 jz	rx_print */
0x0000f379, 0x00000400, /* 00000400 //C 0f379 mov	r0,r4 */
0x0000f37a, 0x0400f41e, /* 0400f41e //C 0f37a call	htoi */
0x0000f37b, 0x00500100, /* 00500100 //C 0f37b mov	r5,r1		; Value is in R5 */
0x0000f37c, 0x31f2f38c, /* 31f2f38c //C 0f37c C1 jmp	rx_val_ok */
0x0000f37d, 0x0102f380, /* 0102f380 //C 0f37d mvzl	r0,rx_err_val */
0x0000f37e, 0x0400f476, /* 0400f476 //C 0f37e call	printsnl */
0x0000f37f, 0x01f2f391, /* 01f2f391 //C 0f37f jmp	rx_ret */
0x0000f380, 0x00000056, /* 00000056 //C 0f380 db	86 */
0x0000f381, 0x00000061, /* 00000061 //C 0f381 db	97 */
0x0000f382, 0x0000006c, /* 0000006c //C 0f382 db	108 */
0x0000f383, 0x00000075, /* 00000075 //C 0f383 db	117 */
0x0000f384, 0x00000065, /* 00000065 //C 0f384 db	101 */
0x0000f385, 0x00000020, /* 00000020 //C 0f385 db	32 */
0x0000f386, 0x00000065, /* 00000065 //C 0f386 db	101 */
0x0000f387, 0x00000072, /* 00000072 //C 0f387 db	114 */
0x0000f388, 0x00000072, /* 00000072 //C 0f388 db	114 */
0x0000f389, 0x0000006f, /* 0000006f //C 0f389 db	111 */
0x0000f38a, 0x00000072, /* 00000072 //C 0f38a db	114 */
0x0000f38b, 0x00000000, /* 00000000 //C 0f38b db */
0x0000f38c, 0x01a80010, /* 01a80010 //C 0f38c cmp	r10,16		; Flag reg? */
0x0000f38d, 0x115f003f, /* 115f003f //C 0f38d EQ and	r5,0x3f */
0x0000f38e, 0x0102f5da, /* 0102f5da //C 0f38e mvzl	r0,reg0 */
0x0000f38f, 0x08500a00, /* 08500a00 //C 0f38f st	r5,r0,r10 */
0x0000f390, 0x0400f2eb, /* 0400f2eb //C 0f390 call	print_reg_name_value */
0x0000f391, 0x0ffd0000, /* 0ffd0000 //C 0f391 pop	pc */
0x0000f392, 0x0102000d, /* 0102000d //C 0f392 mvzl	r0,13 */
0x0000f393, 0x0600f5d9, /* 0600f5d9 //C 0f393 st	r0,nuof_reg */
0x0000f394, 0x01f2f35d, /* 01f2f35d //C 0f394 jmp	cmd_rx */
0x0000f395, 0x0102000e, /* 0102000e //C 0f395 mvzl	r0,14 */
0x0000f396, 0x0600f5d9, /* 0600f5d9 //C 0f396 st	r0,nuof_reg */
0x0000f397, 0x01f2f35d, /* 01f2f35d //C 0f397 jmp	cmd_rx */
0x0000f398, 0x0102000f, /* 0102000f //C 0f398 mvzl	r0,15 */
0x0000f399, 0x0600f5d9, /* 0600f5d9 //C 0f399 st	r0,nuof_reg */
0x0000f39a, 0x01f2f35d, /* 01f2f35d //C 0f39a jmp	cmd_rx */
0x0000f39b, 0x01020010, /* 01020010 //C 0f39b mvzl	r0,16 */
0x0000f39c, 0x0600f5d9, /* 0600f5d9 //C 0f39c st	r0,nuof_reg */
0x0000f39d, 0x01f2f35d, /* 01f2f35d //C 0f39d jmp	cmd_rx */
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
0x0000f3b0, 0x0d2d0000, /* 0d2d0000 //C 0f3b0 push	r2 */
0x0000f3b1, 0x0d4d0000, /* 0d4d0000 //C 0f3b1 push	r4 */
0x0000f3b2, 0x0d5d0000, /* 0d5d0000 //C 0f3b2 push	r5 */
0x0000f3b3, 0x0d6d0000, /* 0d6d0000 //C 0f3b3 push	r6 */
0x0000f3b4, 0x0e200000, /* 0e200000 //C 0f3b4 ld	r2,r0		; Got one-one char */
0x0000f3b5, 0x0e610000, /* 0e610000 //C 0f3b5 ld	r6,r1		; from two strings */
0x0000f3b6, 0x023b0000, /* 023b0000 //C 0f3b6 sz	r3		; Prepare for comparing */
0x0000f3b7, 0x112d0020, /* 112d0020 //C 0f3b7 Z1 or	r2,0x20		; if insensitive case */
0x0000f3b8, 0x023b0000, /* 023b0000 //C 0f3b8 sz	r3 */
0x0000f3b9, 0x116d0020, /* 116d0020 //C 0f3b9 Z1 or	r6,0x20 */
0x0000f3ba, 0x00280600, /* 00280600 //C 0f3ba cmp	r2,r6		; compare them */
0x0000f3bb, 0x21f2f3cd, /* 21f2f3cd //C 0f3bb jnz	streq_no	; if differs: strings are not equal */
0x0000f3bc, 0x0e200000, /* 0e200000 //C 0f3bc ld	r2,r0		; Pick original (non-prepared) */
0x0000f3bd, 0x0e610000, /* 0e610000 //C 0f3bd ld	r6,r1		; chars to check EOS */
0x0000f3be, 0x022b0000, /* 022b0000 //C 0f3be sz	r2		; convert them to boolean */
0x0000f3bf, 0x21220001, /* 21220001 //C 0f3bf Z0 mvzl	r2,1		; values in R2,R6 */
0x0000f3c0, 0x11220000, /* 11220000 //C 0f3c0 Z1 mvzl	r2,0		; and copy in R4,R5 */
0x0000f3c1, 0x00400200, /* 00400200 //C 0f3c1 mov	r4,r2 */
0x0000f3c2, 0x026b0000, /* 026b0000 //C 0f3c2 sz	r6 */
0x0000f3c3, 0x21620001, /* 21620001 //C 0f3c3 Z0 mvzl	r6,1 */
0x0000f3c4, 0x11620000, /* 11620000 //C 0f3c4 Z1 mvzl r6,0 */
0x0000f3c5, 0x00500600, /* 00500600 //C 0f3c5 mov	r5,r6 */
0x0000f3c6, 0x004d0500, /* 004d0500 //C 0f3c6 or	r4,r5		; if both are EOS: equal */
0x0000f3c7, 0x11f2f3cf, /* 11f2f3cf //C 0f3c7 jz	streq_yes */
0x0000f3c8, 0x002f0600, /* 002f0600 //C 0f3c8 and 	r2,r6		; just one is EOS: not equal */
0x0000f3c9, 0x11f2f3cd, /* 11f2f3cd //C 0f3c9 jz	streq_no */
0x0000f3ca, 0x010a0001, /* 010a0001 //C 0f3ca plus	r0,1		; non are EOS: go to check next char */
0x0000f3cb, 0x011a0001, /* 011a0001 //C 0f3cb plus	r1,1 */
0x0000f3cc, 0x01f2f3b4, /* 01f2f3b4 //C 0f3cc jmp	streq_cyc */
0x0000f3cd, 0x020d0000, /* 020d0000 //C 0f3cd clc			; False result */
0x0000f3ce, 0x01f2f3d0, /* 01f2f3d0 //C 0f3ce jmp	streq_ret */
0x0000f3cf, 0x020c0000, /* 020c0000 //C 0f3cf sec			; True result */
0x0000f3d0, 0x0e610000, /* 0e610000 //C 0f3d0 ld	r6,r1		; forward R1 to EOS */
0x0000f3d1, 0x026b0000, /* 026b0000 //C 0f3d1 sz	r6 */
0x0000f3d2, 0x11f2f3d5, /* 11f2f3d5 //C 0f3d2 jz	streq_ret_ret */
0x0000f3d3, 0x01140001, /* 01140001 //C 0f3d3 inc	r1 */
0x0000f3d4, 0x01f2f3d0, /* 01f2f3d0 //C 0f3d4 jmp	streq_ret */
0x0000f3d5, 0x0f6d0000, /* 0f6d0000 //C 0f3d5 pop	r6 */
0x0000f3d6, 0x0f5d0000, /* 0f5d0000 //C 0f3d6 pop	r5 */
0x0000f3d7, 0x0f4d0000, /* 0f4d0000 //C 0f3d7 pop	r4 */
0x0000f3d8, 0x0f2d0000, /* 0f2d0000 //C 0f3d8 pop	r2 */
0x0000f3d9, 0x0f0d0000, /* 0f0d0000 //C 0f3d9 pop	r0 */
0x0000f3da, 0x0ffd0000, /* 0ffd0000 //C 0f3da pop	pc */
0x0000f3db, 0x0ded0000, /* 0ded0000 //C 0f3db push	lr */
0x0000f3dc, 0x0d1d0000, /* 0d1d0000 //C 0f3dc push	r1 */
0x0000f3dd, 0x0d3d0000, /* 0d3d0000 //C 0f3dd push	r3 */
0x0000f3de, 0x01320001, /* 01320001 //C 0f3de mvzl	r3,1 */
0x0000f3df, 0x0400f3ae, /* 0400f3ae //C 0f3df call	str_cmp_eq */
0x0000f3e0, 0x0f3d0000, /* 0f3d0000 //C 0f3e0 pop	r3 */
0x0000f3e1, 0x0f1d0000, /* 0f1d0000 //C 0f3e1 pop	r1 */
0x0000f3e2, 0x0ffd0000, /* 0ffd0000 //C 0f3e2 pop	pc */
0x0000f3e3, 0x0ded0000, /* 0ded0000 //C 0f3e3 push	lr */
0x0000f3e4, 0x0d1d0000, /* 0d1d0000 //C 0f3e4 push	r1 */
0x0000f3e5, 0x0d3d0000, /* 0d3d0000 //C 0f3e5 push	r3 */
0x0000f3e6, 0x01320000, /* 01320000 //C 0f3e6 mvzl	r3,0 */
0x0000f3e7, 0x0400f3ae, /* 0400f3ae //C 0f3e7 call	str_cmp_eq */
0x0000f3e8, 0x0f3d0000, /* 0f3d0000 //C 0f3e8 pop	r3 */
0x0000f3e9, 0x0f1d0000, /* 0f1d0000 //C 0f3e9 pop	r1 */
0x0000f3ea, 0x0ffd0000, /* 0ffd0000 //C 0f3ea pop	pc */
0x0000f3eb, 0x0108002f, /* 0108002f //C 0f3eb cmp	r0,'/' */
0x0000f3ec, 0xa1f2f401, /* a1f2f401 //C 0f3ec LS jmp	hc2v_nok */
0x0000f3ed, 0x01080039, /* 01080039 //C 0f3ed cmp	r0,'9' */
0x0000f3ee, 0xa1f2f3fe, /* a1f2f3fe //C 0f3ee LS jmp	hc2v_0_9 */
0x0000f3ef, 0x01080040, /* 01080040 //C 0f3ef cmp	r0,'@' */
0x0000f3f0, 0xa1f2f401, /* a1f2f401 //C 0f3f0 LS jmp	hc2v_nok */
0x0000f3f1, 0x01080046, /* 01080046 //C 0f3f1 cmp	r0,'F' */
0x0000f3f2, 0xa1f2f3fb, /* a1f2f3fb //C 0f3f2 LS jmp	hc2v_A_F */
0x0000f3f3, 0x01080060, /* 01080060 //C 0f3f3 cmp	r0,'`' */
0x0000f3f4, 0xa1f2f401, /* a1f2f401 //C 0f3f4 LS jmp	hc2v_nok */
0x0000f3f5, 0x01080066, /* 01080066 //C 0f3f5 cmp	r0,'f' */
0x0000f3f6, 0xa1f2f3f8, /* a1f2f3f8 //C 0f3f6 LS jmp	hc2v_a_f */
0x0000f3f7, 0x01f2f401, /* 01f2f401 //C 0f3f7 jmp	hc2v_nok */
0x0000f3f8, 0x0104000a, /* 0104000a //C 0f3f8 add	r0,10 */
0x0000f3f9, 0x01060061, /* 01060061 //C 0f3f9 sub	r0,'a' */
0x0000f3fa, 0x01f2f3ff, /* 01f2f3ff //C 0f3fa jmp	hc2v_ok */
0x0000f3fb, 0x0104000a, /* 0104000a //C 0f3fb add	r0,10 */
0x0000f3fc, 0x01060041, /* 01060041 //C 0f3fc sub	r0,'A' */
0x0000f3fd, 0x01f2f3ff, /* 01f2f3ff //C 0f3fd jmp	hc2v_ok */
0x0000f3fe, 0x01060030, /* 01060030 //C 0f3fe sub	r0,'0' */
0x0000f3ff, 0x020c0000, /* 020c0000 //C 0f3ff sec */
0x0000f400, 0x00f00e00, /* 00f00e00 //C 0f400 ret */
0x0000f401, 0x020d0000, /* 020d0000 //C 0f401 clc */
0x0000f402, 0x00f00e00, /* 00f00e00 //C 0f402 ret */
0x0000f403, 0x0d1d0000, /* 0d1d0000 //C 0f403 push	r1 */
0x0000f404, 0x010f000f, /* 010f000f //C 0f404 and	r0,0xf */
0x0000f405, 0x0112f409, /* 0112f409 //C 0f405 mvzl	r1,v2hc_table */
0x0000f406, 0x0a010000, /* 0a010000 //C 0f406 ld	r0,r1,r0 */
0x0000f407, 0x0f1d0000, /* 0f1d0000 //C 0f407 pop	r1 */
0x0000f408, 0x00f00e00, /* 00f00e00 //C 0f408 ret */
0x0000f409, 0x00000030, /* 00000030 //C 0f409 db	48 */
0x0000f40a, 0x00000031, /* 00000031 //C 0f40a db	49 */
0x0000f40b, 0x00000032, /* 00000032 //C 0f40b db	50 */
0x0000f40c, 0x00000033, /* 00000033 //C 0f40c db	51 */
0x0000f40d, 0x00000034, /* 00000034 //C 0f40d db	52 */
0x0000f40e, 0x00000035, /* 00000035 //C 0f40e db	53 */
0x0000f40f, 0x00000036, /* 00000036 //C 0f40f db	54 */
0x0000f410, 0x00000037, /* 00000037 //C 0f410 db	55 */
0x0000f411, 0x00000038, /* 00000038 //C 0f411 db	56 */
0x0000f412, 0x00000039, /* 00000039 //C 0f412 db	57 */
0x0000f413, 0x00000041, /* 00000041 //C 0f413 db	65 */
0x0000f414, 0x00000042, /* 00000042 //C 0f414 db	66 */
0x0000f415, 0x00000043, /* 00000043 //C 0f415 db	67 */
0x0000f416, 0x00000044, /* 00000044 //C 0f416 db	68 */
0x0000f417, 0x00000045, /* 00000045 //C 0f417 db	69 */
0x0000f418, 0x00000046, /* 00000046 //C 0f418 db	70 */
0x0000f419, 0x00000000, /* 00000000 //C 0f419 db */
0x0000f41a, 0x0ded0000, /* 0ded0000 //C 0f41a push	lr */
0x0000f41b, 0x0400f403, /* 0400f403 //C 0f41b call	value2Hexchar */
0x0000f41c, 0x010d0020, /* 010d0020 //C 0f41c or	r0,0x20 */
0x0000f41d, 0x0ffd0000, /* 0ffd0000 //C 0f41d pop	pc */
0x0000f41e, 0x0ded0000, /* 0ded0000 //C 0f41e push	lr */
0x0000f41f, 0x0d2d0000, /* 0d2d0000 //C 0f41f push	r2 */
0x0000f420, 0x0d3d0000, /* 0d3d0000 //C 0f420 push	r3 */
0x0000f421, 0x01120000, /* 01120000 //C 0f421 mvzl	r1,0		; return value */
0x0000f422, 0x01320000, /* 01320000 //C 0f422 mvzl	r3,0		; index */
0x0000f423, 0x0b238000, /* 0b238000 //C 0f423 ld	r2,r3+,r0	; pick a char */
0x0000f424, 0x022b0000, /* 022b0000 //C 0f424 sz	r2		; check eof string */
0x0000f425, 0x11f2f43b, /* 11f2f43b //C 0f425 jz	htoi_eos */
0x0000f426, 0x0128002e, /* 0128002e //C 0f426 cmp	r2,'.'		; skip separators */
0x0000f427, 0x11f2f423, /* 11f2f423 //C 0f427 jz	htoi_cyc */
0x0000f428, 0x0128005f, /* 0128005f //C 0f428 cmp	r2,'_' */
0x0000f429, 0x11f2f423, /* 11f2f423 //C 0f429 jz	htoi_cyc */
0x0000f42a, 0x0128002d, /* 0128002d //C 0f42a cmp	r2,'-' */
0x0000f42b, 0x11f2f423, /* 11f2f423 //C 0f42b jz	htoi_cyc */
0x0000f42c, 0x0d0d0000, /* 0d0d0000 //C 0f42c push	r0 */
0x0000f42d, 0x00000200, /* 00000200 //C 0f42d mov	r0,r2 */
0x0000f42e, 0x0400f3eb, /* 0400f3eb //C 0f42e call	hexchar2value */
0x0000f42f, 0x00200000, /* 00200000 //C 0f42f mov	r2,r0 */
0x0000f430, 0x0f0d0000, /* 0f0d0000 //C 0f430 pop	r0 */
0x0000f431, 0x41f2f439, /* 41f2f439 //C 0f431 C0 jmp	htoi_nok */
0x0000f432, 0x02180000, /* 02180000 //C 0f432 shl	r1 */
0x0000f433, 0x02180000, /* 02180000 //C 0f433 shl	r1 */
0x0000f434, 0x02180000, /* 02180000 //C 0f434 shl	r1 */
0x0000f435, 0x02180000, /* 02180000 //C 0f435 shl	r1 */
0x0000f436, 0x012f000f, /* 012f000f //C 0f436 and	r2,0xf */
0x0000f437, 0x001d0200, /* 001d0200 //C 0f437 or	r1,r2 */
0x0000f438, 0x01f2f423, /* 01f2f423 //C 0f438 jmp	htoi_cyc */
0x0000f439, 0x020d0000, /* 020d0000 //C 0f439 clc */
0x0000f43a, 0x01f2f43e, /* 01f2f43e //C 0f43a jmp	htoi_ret */
0x0000f43b, 0x01380001, /* 01380001 //C 0f43b cmp	r3,1 */
0x0000f43c, 0x920d0000, /* 920d0000 //C 0f43c HI clc */
0x0000f43d, 0xa20c0000, /* a20c0000 //C 0f43d LS sec */
0x0000f43e, 0x0f3d0000, /* 0f3d0000 //C 0f43e pop	r3 */
0x0000f43f, 0x0f2d0000, /* 0f2d0000 //C 0f43f pop	r2 */
0x0000f440, 0x0ffd0000, /* 0ffd0000 //C 0f440 pop	pc */
0x0000f441, 0x0d0d0000, /* 0d0d0000 //C 0f441 push	r0 */
0x0000f442, 0x0700ff42, /* 0700ff42 //C 0f442 ld	r0,UART_RSTAT */
0x0000f443, 0x010c0001, /* 010c0001 //C 0f443 test	r0,1		; check if queue is not empty */
0x0000f444, 0x020d0000, /* 020d0000 //C 0f444 clc */
0x0000f445, 0x220c0000, /* 220c0000 //C 0f445 Z0 sec */
0x0000f446, 0x0f0d0000, /* 0f0d0000 //C 0f446 pop	r0 */
0x0000f447, 0x30f00e00, /* 30f00e00 //C 0f447 C1 ret */
0x0000f448, 0x00f00e00, /* 00f00e00 //C 0f448 ret */
0x0000f449, 0x0700ff40, /* 0700ff40 //C 0f449 ld	r0,UART_DR */
0x0000f44a, 0x0600ff46, /* 0600ff46 //C 0f44a st	r0,UART_IRA */
0x0000f44b, 0x00f00e00, /* 00f00e00 //C 0f44b ret */
0x0000f44c, 0x0d9d0000, /* 0d9d0000 //C 0f44c push	r9 */
0x0000f44d, 0x0790ff43, /* 0790ff43 //C 0f44d ld	r9,UART_TSTAT */
0x0000f44e, 0x019c0001, /* 019c0001 //C 0f44e test	r9,1 */
0x0000f44f, 0x11f2f44d, /* 11f2f44d //C 0f44f jz	wait_tc */
0x0000f450, 0x0600ff40, /* 0600ff40 //C 0f450 st	r0,UART_DR */
0x0000f451, 0x0f9d0000, /* 0f9d0000 //C 0f451 pop	r9 */
0x0000f452, 0x00f00e00, /* 00f00e00 //C 0f452 ret */
0x0000f453, 0x0ded0000, /* 0ded0000 //C 0f453 push	lr */
0x0000f454, 0x0d0d0000, /* 0d0d0000 //C 0f454 push	r0 */
0x0000f455, 0x0d3d0000, /* 0d3d0000 //C 0f455 push	r3 */
0x0000f456, 0x0d4d0000, /* 0d4d0000 //C 0f456 push	r4 */
0x0000f457, 0x0d2d0000, /* 0d2d0000 //C 0f457 push	r2 */
0x0000f458, 0x01420000, /* 01420000 //C 0f458 mvzl	r4,0 */
0x0000f459, 0x020b0000, /* 020b0000 //C 0f459 sz	r0 */
0x0000f45a, 0x1102f612, /* 1102f612 //C 0f45a Z1 mvzl	r0,null_str */
0x0000f45b, 0x00200000, /* 00200000 //C 0f45b mov	r2,r0 */
0x0000f45c, 0x0b048200, /* 0b048200 //C 0f45c ld	r0,r4+,r2 */
0x0000f45d, 0x020b0000, /* 020b0000 //C 0f45d sz	r0 */
0x0000f45e, 0x11f2f461, /* 11f2f461 //C 0f45e jz	prints_done */
0x0000f45f, 0x0400f44c, /* 0400f44c //C 0f45f call	putchar */
0x0000f460, 0x01f2f45c, /* 01f2f45c //C 0f460 jmp	prints_go */
0x0000f461, 0x0f2d0000, /* 0f2d0000 //C 0f461 pop	r2 */
0x0000f462, 0x0f4d0000, /* 0f4d0000 //C 0f462 pop	r4 */
0x0000f463, 0x0f3d0000, /* 0f3d0000 //C 0f463 pop	r3 */
0x0000f464, 0x0f0d0000, /* 0f0d0000 //C 0f464 pop	r0 */
0x0000f465, 0x0ffd0000, /* 0ffd0000 //C 0f465 pop	pc */
0x0000f466, 0x00000000, /* 00000000 //C 0f466 dd	0 */
0x0000f467, 0x0ded0000, /* 0ded0000 //C 0f467 push	lr */
0x0000f468, 0x0d0d0000, /* 0d0d0000 //C 0f468 push	r0 */
0x0000f469, 0x0d1d0000, /* 0d1d0000 //C 0f469 push	r1 */
0x0000f46a, 0x00100e00, /* 00100e00 //C 0f46a mov	r1,lr */
0x0000f46b, 0x0e010000, /* 0e010000 //C 0f46b ld	r0,r1 */
0x0000f46c, 0x01140001, /* 01140001 //C 0f46c inc	r1 */
0x0000f46d, 0x020b0000, /* 020b0000 //C 0f46d sz	r0 */
0x0000f46e, 0x11f2f471, /* 11f2f471 //C 0f46e jz	pes_done */
0x0000f46f, 0x0400f44c, /* 0400f44c //C 0f46f call	putchar */
0x0000f470, 0x01f2f46b, /* 01f2f46b //C 0f470 jmp	pes_next */
0x0000f471, 0x0610f466, /* 0610f466 //C 0f471 st	r1,pes_ret_to */
0x0000f472, 0x0f1d0000, /* 0f1d0000 //C 0f472 pop	r1 */
0x0000f473, 0x0f0d0000, /* 0f0d0000 //C 0f473 pop	r0 */
0x0000f474, 0x0fed0000, /* 0fed0000 //C 0f474 pop	lr */
0x0000f475, 0x07f0f466, /* 07f0f466 //C 0f475 ld	pc,pes_ret_to */
0x0000f476, 0x0ded0000, /* 0ded0000 //C 0f476 push	lr */
0x0000f477, 0x0400f453, /* 0400f453 //C 0f477 call	prints */
0x0000f478, 0x0d0d0000, /* 0d0d0000 //C 0f478 push	r0 */
0x0000f479, 0x0102000a, /* 0102000a //C 0f479 mvzl	r0,LF */
0x0000f47a, 0x0400f44c, /* 0400f44c //C 0f47a call	putchar */
0x0000f47b, 0x0f0d0000, /* 0f0d0000 //C 0f47b pop	r0 */
0x0000f47c, 0x0ffd0000, /* 0ffd0000 //C 0f47c pop	pc */
0x0000f47d, 0x0ded0000, /* 0ded0000 //C 0f47d push	lr */
0x0000f47e, 0x0d0d0000, /* 0d0d0000 //C 0f47e push	r0 */
0x0000f47f, 0x0d1d0000, /* 0d1d0000 //C 0f47f push	r1 */
0x0000f480, 0x0d2d0000, /* 0d2d0000 //C 0f480 push	r2 */
0x0000f481, 0x0d3d0000, /* 0d3d0000 //C 0f481 push	r3 */
0x0000f482, 0x0d4d0000, /* 0d4d0000 //C 0f482 push	r4 */
0x0000f483, 0x00300000, /* 00300000 //C 0f483 mov	r3,r0 */
0x0000f484, 0x01220000, /* 01220000 //C 0f484 mvzl	r2,0 */
0x0000f485, 0x01420001, /* 01420001 //C 0f485 mvzl	r4,1 */
0x0000f486, 0x01020000, /* 01020000 //C 0f486 mvzl	r0,0 */
0x0000f487, 0x02380000, /* 02380000 //C 0f487 shl	r3 */
0x0000f488, 0x02070000, /* 02070000 //C 0f488 rol	r0 */
0x0000f489, 0x02380000, /* 02380000 //C 0f489 shl	r3 */
0x0000f48a, 0x02070000, /* 02070000 //C 0f48a rol	r0 */
0x0000f48b, 0x02380000, /* 02380000 //C 0f48b shl	r3 */
0x0000f48c, 0x02070000, /* 02070000 //C 0f48c rol	r0 */
0x0000f48d, 0x02380000, /* 02380000 //C 0f48d shl	r3 */
0x0000f48e, 0x02070000, /* 02070000 //C 0f48e rol	r0 */
0x0000f48f, 0x0400f403, /* 0400f403 //C 0f48f call	value2Hexchar */
0x0000f490, 0x0400f44c, /* 0400f44c //C 0f490 call	putchar */
0x0000f491, 0x01240001, /* 01240001 //C 0f491 add	r2,1 */
0x0000f492, 0x01280008, /* 01280008 //C 0f492 cmp	r2,8 */
0x0000f493, 0x11f2f49d, /* 11f2f49d //C 0f493 jz	print_vhex_ret */
0x0000f494, 0x021b0000, /* 021b0000 //C 0f494 sz	r1 */
0x0000f495, 0x11f2f49b, /* 11f2f49b //C 0f495 jz	print_vhex_nosep */
0x0000f496, 0x00480100, /* 00480100 //C 0f496 cmp	r4,r1 */
0x0000f497, 0x21f2f49b, /* 21f2f49b //C 0f497 jnz	print_vhex_nosep */
0x0000f498, 0x0102005f, /* 0102005f //C 0f498 mvzl	r0,'_' */
0x0000f499, 0x0400f44c, /* 0400f44c //C 0f499 call	putchar */
0x0000f49a, 0x01420000, /* 01420000 //C 0f49a mvzl	r4,0 */
0x0000f49b, 0x01440001, /* 01440001 //C 0f49b add	r4,1 */
0x0000f49c, 0x01f2f486, /* 01f2f486 //C 0f49c jmp	print_vhex_cyc */
0x0000f49d, 0x0f4d0000, /* 0f4d0000 //C 0f49d pop	r4 */
0x0000f49e, 0x0f3d0000, /* 0f3d0000 //C 0f49e pop	r3 */
0x0000f49f, 0x0f2d0000, /* 0f2d0000 //C 0f49f pop	r2 */
0x0000f4a0, 0x0f1d0000, /* 0f1d0000 //C 0f4a0 pop	r1 */
0x0000f4a1, 0x0f0d0000, /* 0f0d0000 //C 0f4a1 pop	r0 */
0x0000f4a2, 0x0ffd0000, /* 0ffd0000 //C 0f4a2 pop	pc */
0x0000f4a3, 0x0ded0000, /* 0ded0000 //C 0f4a3 push	lr */
0x0000f4a4, 0x0d4d0000, /* 0d4d0000 //C 0f4a4 push	r4 */
0x0000f4a5, 0x021b0000, /* 021b0000 //C 0f4a5 sz	r1 */
0x0000f4a6, 0x21f2f4aa, /* 21f2f4aa //C 0f4a6 NZ jmp	div_dok */
0x0000f4a7, 0x00200000, /* 00200000 //C 0f4a7 mov	r2,r0		; div by zero */
0x0000f4a8, 0x01320000, /* 01320000 //C 0f4a8 mvzl	r3,0 */
0x0000f4a9, 0x01f2f4bb, /* 01f2f4bb //C 0f4a9 jmp	div_ret */
0x0000f4aa, 0x01220000, /* 01220000 //C 0f4aa mvzl	r2,0		; Q= 0 */
0x0000f4ab, 0x01320000, /* 01320000 //C 0f4ab mvzl	r3,0		; R= 0 */
0x0000f4ac, 0x01418000, /* 01418000 //C 0f4ac mvh	r4,0x80000000	; m= 1<<31 */
0x0000f4ad, 0x01400000, /* 01400000 //C 0f4ad mvl	r4,0x80000000 */
0x0000f4ae, 0x024b0000, /* 024b0000 //C 0f4ae sz	r4 */
0x0000f4af, 0x11f2f4bb, /* 11f2f4bb //C 0f4af Z jmp	div_ret */
0x0000f4b0, 0x02380000, /* 02380000 //C 0f4b0 shl	r3		; r<<= 1 */
0x0000f4b1, 0x000c0400, /* 000c0400 //C 0f4b1 test	r0,r4		; if (n&m) */
0x0000f4b2, 0x213d0001, /* 213d0001 //C 0f4b2 NZ or	r3,1		; r|= 1 */
0x0000f4b3, 0x00380100, /* 00380100 //C 0f4b3 cmp	r3,r1		; if (r>=d) */
0x0000f4b4, 0x41f2f4b9, /* 41f2f4b9 //C 0f4b4 LO jmp	div_cyc_next */
0x0000f4b5, 0x00360100, /* 00360100 //C 0f4b5 sub	r3,r1		;r-= d */
0x0000f4b6, 0x002d0400, /* 002d0400 //C 0f4b6 or	r2,r4		;q|= m */
0x0000f4b7, 0x01f2f4b9, /* 01f2f4b9 //C 0f4b7 jmp	div_cyc_next */
0x0000f4b8, 0x01f2f4ae, /* 01f2f4ae //C 0f4b8 jmp	div_cyc */
0x0000f4b9, 0x02490000, /* 02490000 //C 0f4b9 shr	r4		; m>>= 1 */
0x0000f4ba, 0x01f2f4ae, /* 01f2f4ae //C 0f4ba jmp	div_cyc */
0x0000f4bb, 0x0f4d0000, /* 0f4d0000 //C 0f4bb pop	r4 */
0x0000f4bc, 0x0ffd0000, /* 0ffd0000 //C 0f4bc pop	pc */
0x0000f4bd, 0x0ded0000, /* 0ded0000 //C 0f4bd push	lr */
0x0000f4be, 0x0d0d0000, /* 0d0d0000 //C 0f4be push	r0 */
0x0000f4bf, 0x0d1d0000, /* 0d1d0000 //C 0f4bf push	r1 */
0x0000f4c0, 0x0d2d0000, /* 0d2d0000 //C 0f4c0 push	r2 */
0x0000f4c1, 0x0d3d0000, /* 0d3d0000 //C 0f4c1 push	r3 */
0x0000f4c2, 0x0dad0000, /* 0dad0000 //C 0f4c2 push	r10 */
0x0000f4c3, 0x0dbd0000, /* 0dbd0000 //C 0f4c3 push	r11 */
0x0000f4c4, 0x0dcd0000, /* 0dcd0000 //C 0f4c4 push	r12 */
0x0000f4c5, 0x01c2f4e5, /* 01c2f4e5 //C 0f4c5 mvzl	r12,itoa_buffer	; pointer to output buffer */
0x0000f4c6, 0x01b2f4f0, /* 01b2f4f0 //C 0f4c6 mvzl	r11,itoa_divs	; pointer to dividers */
0x0000f4c7, 0x01a20000, /* 01a20000 //C 0f4c7 mvzl	r10,0		; bool: first non-zero char found */
0x0000f4c8, 0x0e1b0000, /* 0e1b0000 //C 0f4c8 ld	r1,r11		; get next divider */
0x0000f4c9, 0x021b0000, /* 021b0000 //C 0f4c9 sz	r1		; if 0, then */
0x0000f4ca, 0x11f2f4dd, /* 11f2f4dd //C 0f4ca jz	itoa_ret	; finish */
0x0000f4cb, 0x01180001, /* 01180001 //C 0f4cb cmp	r1,1		; last divider? */
0x0000f4cc, 0x11a20001, /* 11a20001 //C 0f4cc EQ mvzl	r10,1		; always print last char */
0x0000f4cd, 0x0400f4a3, /* 0400f4a3 //C 0f4cd call	div		; R2,R3= R0/R1 */
0x0000f4ce, 0x022b0000, /* 022b0000 //C 0f4ce sz	r2		; is the result zero? */
0x0000f4cf, 0x11f2f4da, /* 11f2f4da //C 0f4cf jz	itoa_f0 */
0x0000f4d0, 0x01a20001, /* 01a20001 //C 0f4d0 mvzl	r10,1		; non-zero: start to print */
0x0000f4d1, 0x00000200, /* 00000200 //C 0f4d1 mov	r0,r2		; convert result to ASCII char */
0x0000f4d2, 0x0400f41a, /* 0400f41a //C 0f4d2 call	value2hexchar */
0x0000f4d3, 0x0c0c0000, /* 0c0c0000 //C 0f4d3 st	r0,r12		; and store it in buffer */
0x0000f4d4, 0x01c40001, /* 01c40001 //C 0f4d4 inc	r12		; inc buf ptr */
0x0000f4d5, 0x01020000, /* 01020000 //C 0f4d5 mvzl	r0,0		; put 0 after last char */
0x0000f4d6, 0x0c0c0000, /* 0c0c0000 //C 0f4d6 st	r0,r12 */
0x0000f4d7, 0x00000300, /* 00000300 //C 0f4d7 mov	r0,r3		; continue with the reminder */
0x0000f4d8, 0x01b40001, /* 01b40001 //C 0f4d8 inc	r11		; and next divider */
0x0000f4d9, 0x01f2f4c8, /* 01f2f4c8 //C 0f4d9 jmp	itoa_cyc */
0x0000f4da, 0x02ab0000, /* 02ab0000 //C 0f4da sz	r10		; just zeros so far? */
0x0000f4db, 0x21f2f4d1, /* 21f2f4d1 //C 0f4db jnz	itoa_store	; no, print */
0x0000f4dc, 0x01f2f4d7, /* 01f2f4d7 //C 0f4dc jmp	itoa_next */
0x0000f4dd, 0x0fcd0000, /* 0fcd0000 //C 0f4dd pop	r12 */
0x0000f4de, 0x0fbd0000, /* 0fbd0000 //C 0f4de pop	r11 */
0x0000f4df, 0x0fad0000, /* 0fad0000 //C 0f4df pop	r10 */
0x0000f4e0, 0x0f3d0000, /* 0f3d0000 //C 0f4e0 pop	r3 */
0x0000f4e1, 0x0f2d0000, /* 0f2d0000 //C 0f4e1 pop	r2 */
0x0000f4e2, 0x0f1d0000, /* 0f1d0000 //C 0f4e2 pop	r1 */
0x0000f4e3, 0x0f0d0000, /* 0f0d0000 //C 0f4e3 pop	r0 */
0x0000f4e4, 0x0ffd0000, /* 0ffd0000 //C 0f4e4 pop	pc */
0x0000f4e5, 0x00000000, /* 00000000 //C 0f4e5 itoa_buffer:	ds	11 */
0x0000f4f0, 0x3b9aca00, /* 3b9aca00 //C 0f4f0 dd	1000000000 */
0x0000f4f1, 0x05f5e100, /* 05f5e100 //C 0f4f1 dd	100000000 */
0x0000f4f2, 0x00989680, /* 00989680 //C 0f4f2 dd	10000000 */
0x0000f4f3, 0x000f4240, /* 000f4240 //C 0f4f3 dd	1000000 */
0x0000f4f4, 0x000186a0, /* 000186a0 //C 0f4f4 dd	100000 */
0x0000f4f5, 0x00002710, /* 00002710 //C 0f4f5 dd	10000 */
0x0000f4f6, 0x000003e8, /* 000003e8 //C 0f4f6 dd	1000 */
0x0000f4f7, 0x00000064, /* 00000064 //C 0f4f7 dd	100 */
0x0000f4f8, 0x0000000a, /* 0000000a //C 0f4f8 dd	10 */
0x0000f4f9, 0x00000001, /* 00000001 //C 0f4f9 dd	1 */
0x0000f4fa, 0x00000000, /* 00000000 //C 0f4fa dd	0 */
0x0000f4fb, 0x0ded0000, /* 0ded0000 //C 0f4fb push	lr */
0x0000f4fc, 0x0400f4bd, /* 0400f4bd //C 0f4fc call	itoa */
0x0000f4fd, 0x0102f4e5, /* 0102f4e5 //C 0f4fd mvzl	r0,itoa_buffer */
0x0000f4fe, 0x0400f453, /* 0400f453 //C 0f4fe call	prints */
0x0000f4ff, 0x0ffd0000, /* 0ffd0000 //C 0f4ff pop	pc */
0x0000f500, 0x0ded0000, /* 0ded0000 //C 0f500 push	lr */
0x0000f501, 0x0d0d0000, /* 0d0d0000 //C 0f501 push	r0 */
0x0000f502, 0x0d1d0000, /* 0d1d0000 //C 0f502 push	r1 */
0x0000f503, 0x0610f5db, /* 0610f5db //C 0f503 st	r1,reg1 */
0x0000f504, 0x0620f5dc, /* 0620f5dc //C 0f504 st	r2,reg2 */
0x0000f505, 0x0630f5dd, /* 0630f5dd //C 0f505 st	r3,reg3 */
0x0000f506, 0x0640f5de, /* 0640f5de //C 0f506 st	r4,reg4 */
0x0000f507, 0x0650f5df, /* 0650f5df //C 0f507 st	r5,reg5 */
0x0000f508, 0x0660f5e0, /* 0660f5e0 //C 0f508 st	r6,reg6 */
0x0000f509, 0x0670f5e1, /* 0670f5e1 //C 0f509 st	r7,reg7 */
0x0000f50a, 0x0680f5e2, /* 0680f5e2 //C 0f50a st	r8,reg8 */
0x0000f50b, 0x0690f5e3, /* 0690f5e3 //C 0f50b st	r9,reg9 */
0x0000f50c, 0x06a0f5e4, /* 06a0f5e4 //C 0f50c st	r10,reg10 */
0x0000f50d, 0x06b0f5e5, /* 06b0f5e5 //C 0f50d st	r11,reg11 */
0x0000f50e, 0x06c0f5e6, /* 06c0f5e6 //C 0f50e st	r12,reg12 */
0x0000f50f, 0x00200000, /* 00200000 //C 0f50f mov	r2,r0		; pointer to format string */
0x0000f510, 0x0112f5db, /* 0112f5db //C 0f510 mvzl	r1,reg1		; pointer to params */
0x0000f511, 0x0e020000, /* 0e020000 //C 0f511 ld	r0,r2		; get next char */
0x0000f512, 0x020b0000, /* 020b0000 //C 0f512 sz	r0		; is it EOS? */
0x0000f513, 0x11f2f561, /* 11f2f561 //C 0f513 jz	printf_ret */
0x0000f514, 0x0108005c, /* 0108005c //C 0f514 cmp	r0,'\\' */
0x0000f515, 0x21f2f538, /* 21f2f538 //C 0f515 jnz	printf_notescape */
0x0000f516, 0x01240001, /* 01240001 //C 0f516 inc	r2 */
0x0000f517, 0x0e020000, /* 0e020000 //C 0f517 ld	r0,r2 */
0x0000f518, 0x020b0000, /* 020b0000 //C 0f518 sz	r0 */
0x0000f519, 0x11f2f561, /* 11f2f561 //C 0f519 jz	printf_ret */
0x0000f51a, 0x01080061, /* 01080061 //C 0f51a cmp	r0,'a' */
0x0000f51b, 0x11020007, /* 11020007 //C 0f51b Z mvzl	r0,7 */
0x0000f51c, 0x11f2f55e, /* 11f2f55e //C 0f51c Z jmp	printf_print */
0x0000f51d, 0x01080062, /* 01080062 //C 0f51d cmp	r0,'b' */
0x0000f51e, 0x11020008, /* 11020008 //C 0f51e Z mvzl	r0,8 */
0x0000f51f, 0x11f2f55e, /* 11f2f55e //C 0f51f Z jmp	printf_print */
0x0000f520, 0x01080065, /* 01080065 //C 0f520 cmp	r0,'e' */
0x0000f521, 0x1102001b, /* 1102001b //C 0f521 Z mvzl	r0,0x1b */
0x0000f522, 0x11f2f55e, /* 11f2f55e //C 0f522 Z jmp	printf_print */
0x0000f523, 0x01080066, /* 01080066 //C 0f523 cmp	r0,'f' */
0x0000f524, 0x1102000c, /* 1102000c //C 0f524 Z mvzl	r0,0xc */
0x0000f525, 0x11f2f55e, /* 11f2f55e //C 0f525 Z jmp	printf_print */
0x0000f526, 0x0108006e, /* 0108006e //C 0f526 cmp	r0,'n' */
0x0000f527, 0x1102000a, /* 1102000a //C 0f527 Z mvzl	r0,0xa */
0x0000f528, 0x11f2f55e, /* 11f2f55e //C 0f528 Z jmp	printf_print */
0x0000f529, 0x01080072, /* 01080072 //C 0f529 cmp	r0,'r' */
0x0000f52a, 0x1102000d, /* 1102000d //C 0f52a Z mvzl	r0,0xd */
0x0000f52b, 0x11f2f55e, /* 11f2f55e //C 0f52b Z jmp	printf_print */
0x0000f52c, 0x01080074, /* 01080074 //C 0f52c cmp	r0,'t' */
0x0000f52d, 0x11020009, /* 11020009 //C 0f52d Z mvzl	r0,9 */
0x0000f52e, 0x11f2f55e, /* 11f2f55e //C 0f52e Z jmp	printf_print */
0x0000f52f, 0x01080076, /* 01080076 //C 0f52f cmp	r0,'v' */
0x0000f530, 0x1102000b, /* 1102000b //C 0f530 Z mvzl	r0,0xb */
0x0000f531, 0x11f2f55e, /* 11f2f55e //C 0f531 Z jmp	printf_print */
0x0000f532, 0x0108005c, /* 0108005c //C 0f532 cmp	r0,0x5c */
0x0000f533, 0x11f2f55e, /* 11f2f55e //C 0f533 Z jmp	printf_print */
0x0000f534, 0x01080030, /* 01080030 //C 0f534 cmp	r0,'0' */
0x0000f535, 0x11020000, /* 11020000 //C 0f535 Z mvzl	r0,0 */
0x0000f536, 0x11f2f55e, /* 11f2f55e //C 0f536 Z jmp	printf_print */
0x0000f537, 0x01f2f55e, /* 01f2f55e //C 0f537 jmp	printf_print */
0x0000f538, 0x01080025, /* 01080025 //C 0f538 cmp	r0,'%'		; is it a format char? */
0x0000f539, 0x21f2f55e, /* 21f2f55e //C 0f539 jnz	printf_print */
0x0000f53a, 0x01240001, /* 01240001 //C 0f53a inc	r2		; go to format char */
0x0000f53b, 0x0e020000, /* 0e020000 //C 0f53b ld	r0,r2 */
0x0000f53c, 0x022b0000, /* 022b0000 //C 0f53c sz	r2		; is it EOS? */
0x0000f53d, 0x11f2f561, /* 11f2f561 //C 0f53d jz	printf_ret */
0x0000f53e, 0x01080025, /* 01080025 //C 0f53e cmp	r0,'%'		; % is used to print % */
0x0000f53f, 0x11f2f55e, /* 11f2f55e //C 0f53f jz	printf_print */
0x0000f540, 0x01080075, /* 01080075 //C 0f540 cmp	r0,'u'		; u,d print in decimal */
0x0000f541, 0x11f2f544, /* 11f2f544 //C 0f541 jz	printf_d */
0x0000f542, 0x01080064, /* 01080064 //C 0f542 cmp	r0,'d' */
0x0000f543, 0x21f2f548, /* 21f2f548 //C 0f543 jnz	printf_notd */
0x0000f544, 0x0e010000, /* 0e010000 //C 0f544 ld	r0,r1 */
0x0000f545, 0x01140001, /* 01140001 //C 0f545 inc	r1 */
0x0000f546, 0x0400f4fb, /* 0400f4fb //C 0f546 call	printd */
0x0000f547, 0x01f2f55f, /* 01f2f55f //C 0f547 jmp	printf_next */
0x0000f548, 0x01080078, /* 01080078 //C 0f548 cmp	r0,'x' */
0x0000f549, 0x21f2f551, /* 21f2f551 //C 0f549 jnz	printf_notx */
0x0000f54a, 0x0e010000, /* 0e010000 //C 0f54a ld	r0,r1 */
0x0000f54b, 0x01140001, /* 01140001 //C 0f54b inc	r1 */
0x0000f54c, 0x0d1d0000, /* 0d1d0000 //C 0f54c push	r1 */
0x0000f54d, 0x01120000, /* 01120000 //C 0f54d mvzl	r1,0 */
0x0000f54e, 0x0400f47d, /* 0400f47d //C 0f54e call	print_vhex */
0x0000f54f, 0x0f1d0000, /* 0f1d0000 //C 0f54f pop	r1 */
0x0000f550, 0x01f2f55f, /* 01f2f55f //C 0f550 jmp	printf_next */
0x0000f551, 0x01080073, /* 01080073 //C 0f551 cmp	r0,'s' */
0x0000f552, 0x21f2f557, /* 21f2f557 //C 0f552 jnz	printf_nots */
0x0000f553, 0x0e010000, /* 0e010000 //C 0f553 ld	r0,r1 */
0x0000f554, 0x01140001, /* 01140001 //C 0f554 inc	r1 */
0x0000f555, 0x0400f453, /* 0400f453 //C 0f555 call	prints */
0x0000f556, 0x01f2f55f, /* 01f2f55f //C 0f556 jmp	printf_next */
0x0000f557, 0x01080063, /* 01080063 //C 0f557 cmp	r0,'c' */
0x0000f558, 0x21f2f55d, /* 21f2f55d //C 0f558 jnz	printf_notc */
0x0000f559, 0x0e010000, /* 0e010000 //C 0f559 ld	r0,r1 */
0x0000f55a, 0x01140001, /* 01140001 //C 0f55a inc	r1 */
0x0000f55b, 0x0400f44c, /* 0400f44c //C 0f55b call	putchar */
0x0000f55c, 0x01f2f55f, /* 01f2f55f //C 0f55c jmp	printf_next */
0x0000f55d, 0x01f2f55f, /* 01f2f55f //C 0f55d jmp	printf_next */
0x0000f55e, 0x0400f44c, /* 0400f44c //C 0f55e call	putchar		; print actual char */
0x0000f55f, 0x01240001, /* 01240001 //C 0f55f inc	r2		; inc string ptr */
0x0000f560, 0x01f2f511, /* 01f2f511 //C 0f560 jmp	printf_cyc */
0x0000f561, 0x0f1d0000, /* 0f1d0000 //C 0f561 pop	r1 */
0x0000f562, 0x0f0d0000, /* 0f0d0000 //C 0f562 pop	r0 */
0x0000f563, 0x0ffd0000, /* 0ffd0000 //C 0f563 pop	pc */
0x0000f564, 0x0d0d0000, /* 0d0d0000 //C 0f564 push	r0 */
0x0000f565, 0x0d2d0000, /* 0d2d0000 //C 0f565 push	r2 */
0x0000f566, 0x00000e00, /* 00000e00 //C 0f566 mov	r0,LR */
0x0000f567, 0x0400f500, /* 0400f500 //C 0f567 call	printf */
0x0000f568, 0x0620f5dc, /* 0620f5dc //C 0f568 st	r2,reg2 */
0x0000f569, 0x0f2d0000, /* 0f2d0000 //C 0f569 pop	r2 */
0x0000f56a, 0x0f0d0000, /* 0f0d0000 //C 0f56a pop	r0 */
0x0000f56b, 0x07f0f5dc, /* 07f0f5dc //C 0f56b ld	PC,reg2 */
0x0000f56c, 0x00000000, /* 00000000 //C 0f56c line:		ds	100		; line buffer */
0x0000f5d0, 0x00000000, /* 00000000 //C 0f5d0 line_ptr:	ds	1		; line pointer (index) */
0x0000f5d1, 0x00000000, /* 00000000 //C 0f5d1 at_eol:		ds	1		; bool, true if EOL arrived */
0x0000f5d2, 0x00000000, /* 00000000 //C 0f5d2 words:		ds	5		; Tokens of line */
0x0000f5d7, 0x00000000, /* 00000000 //C 0f5d7 echo:		ds	1		; bool, do echo or not */
0x0000f5d8, 0x00000000, /* 00000000 //C 0f5d8 dd	0 */
0x0000f5d9, 0x00000000, /* 00000000 //C 0f5d9 dd	0 */
0x0000f5da, 0x00000000, /* 00000000 //C 0f5da dd	0 */
0x0000f5db, 0x00000000, /* 00000000 //C 0f5db dd	0 */
0x0000f5dc, 0x00000000, /* 00000000 //C 0f5dc dd	0 */
0x0000f5dd, 0x00000000, /* 00000000 //C 0f5dd dd	0 */
0x0000f5de, 0x00000000, /* 00000000 //C 0f5de dd	0 */
0x0000f5df, 0x00000000, /* 00000000 //C 0f5df dd	0 */
0x0000f5e0, 0x00000000, /* 00000000 //C 0f5e0 dd	0 */
0x0000f5e1, 0x00000000, /* 00000000 //C 0f5e1 dd	0 */
0x0000f5e2, 0x00000000, /* 00000000 //C 0f5e2 dd	0 */
0x0000f5e3, 0x00000000, /* 00000000 //C 0f5e3 dd	0 */
0x0000f5e4, 0x00000000, /* 00000000 //C 0f5e4 dd	0 */
0x0000f5e5, 0x00000000, /* 00000000 //C 0f5e5 dd	0 */
0x0000f5e6, 0x00000000, /* 00000000 //C 0f5e6 dd	0 */
0x0000f5e7, 0x00000000, /* 00000000 //C 0f5e7 dd	0 */
0x0000f5e8, 0x00000000, /* 00000000 //C 0f5e8 dd	0 */
0x0000f5e9, 0x00000000, /* 00000000 //C 0f5e9 dd	0 */
0x0000f5ea, 0x00000000, /* 00000000 //C 0f5ea dd	0 */
0x0000f5eb, 0x00000050, /* 00000050 //C 0f5eb db	80 */
0x0000f5ec, 0x0000004d, /* 0000004d //C 0f5ec db	77 */
0x0000f5ed, 0x0000006f, /* 0000006f //C 0f5ed db	111 */
0x0000f5ee, 0x0000006e, /* 0000006e //C 0f5ee db	110 */
0x0000f5ef, 0x00000069, /* 00000069 //C 0f5ef db	105 */
0x0000f5f0, 0x00000074, /* 00000074 //C 0f5f0 db	116 */
0x0000f5f1, 0x0000006f, /* 0000006f //C 0f5f1 db	111 */
0x0000f5f2, 0x00000072, /* 00000072 //C 0f5f2 db	114 */
0x0000f5f3, 0x00000020, /* 00000020 //C 0f5f3 db	32 */
0x0000f5f4, 0x00000076, /* 00000076 //C 0f5f4 db	118 */
0x0000f5f5, 0x00000031, /* 00000031 //C 0f5f5 db	49 */
0x0000f5f6, 0x0000002e, /* 0000002e //C 0f5f6 db	46 */
0x0000f5f7, 0x00000030, /* 00000030 //C 0f5f7 db	48 */
0x0000f5f8, 0x00000000, /* 00000000 //C 0f5f8 db */
0x0000f5f9, 0x00000053, /* 00000053 //C 0f5f9 db	83 */
0x0000f5fa, 0x00000074, /* 00000074 //C 0f5fa db	116 */
0x0000f5fb, 0x0000006f, /* 0000006f //C 0f5fb db	111 */
0x0000f5fc, 0x00000070, /* 00000070 //C 0f5fc db	112 */
0x0000f5fd, 0x00000020, /* 00000020 //C 0f5fd db	32 */
0x0000f5fe, 0x00000061, /* 00000061 //C 0f5fe db	97 */
0x0000f5ff, 0x00000074, /* 00000074 //C 0f5ff db	116 */
0x0000f600, 0x0000003a, /* 0000003a //C 0f600 db	58 */
0x0000f601, 0x00000020, /* 00000020 //C 0f601 db	32 */
0x0000f602, 0x00000000, /* 00000000 //C 0f602 db */
0x0000f603, 0x00000008, /* 00000008 //C 0f603 db	8 */
0x0000f604, 0x00000020, /* 00000020 //C 0f604 db	32 */
0x0000f605, 0x00000008, /* 00000008 //C 0f605 db	8 */
0x0000f606, 0x00000000, /* 00000000 //C 0f606 db	0 */
0x0000f607, 0x0000003a, /* 0000003a //C 0f607 db	58 */
0x0000f608, 0x00000000, /* 00000000 //C 0f608 db */
0x0000f609, 0x00000020, /* 00000020 //C 0f609 db	32 */
0x0000f60a, 0x0000003b, /* 0000003b //C 0f60a db	59 */
0x0000f60b, 0x00000009, /* 00000009 //C 0f60b db	9 */
0x0000f60c, 0x0000000b, /* 0000000b //C 0f60c db	11 */
0x0000f60d, 0x0000002c, /* 0000002c //C 0f60d db	44 */
0x0000f60e, 0x0000003d, /* 0000003d //C 0f60e db	61 */
0x0000f60f, 0x0000005b, /* 0000005b //C 0f60f db	91 */
0x0000f610, 0x0000005d, /* 0000005d //C 0f610 db	93 */
0x0000f611, 0x00000000, /* 00000000 //C 0f611 db */
0x0000f612, 0x00000028, /* 00000028 //C 0f612 db	40 */
0x0000f613, 0x0000006e, /* 0000006e //C 0f613 db	110 */
0x0000f614, 0x00000075, /* 00000075 //C 0f614 db	117 */
0x0000f615, 0x0000006c, /* 0000006c //C 0f615 db	108 */
0x0000f616, 0x0000006c, /* 0000006c //C 0f616 db	108 */
0x0000f617, 0x00000029, /* 00000029 //C 0f617 db	41 */
0x0000f618, 0x00000000, /* 00000000 //C 0f618 db */
0x0000f619, 0x00000000, /* 00000000 //C 0f619 db	0 */
0x0000f61a, 0x00000000, /* 00000000 //C 0f61a db	0 */
0x0000f61b, 0x00000000, /* 00000000 //C 0f61b sc_buffer:	ds	10 */
0x0000f625, 0x0000f190, /* 0000f190 //C 0f625 dd	cmd_sm */
0x0000f626, 0x00000073, /* 00000073 //C 0f626 db	115 */
0x0000f627, 0x0000006d, /* 0000006d //C 0f627 db	109 */
0x0000f628, 0x00000000, /* 00000000 //C 0f628 db */
0x0000f629, 0x0000f137, /* 0000f137 //C 0f629 dd	cmd_m */
0x0000f62a, 0x0000006d, /* 0000006d //C 0f62a db	109 */
0x0000f62b, 0x00000000, /* 00000000 //C 0f62b db */
0x0000f62c, 0x0000f137, /* 0000f137 //C 0f62c dd	cmd_m */
0x0000f62d, 0x0000006d, /* 0000006d //C 0f62d db	109 */
0x0000f62e, 0x00000065, /* 00000065 //C 0f62e db	101 */
0x0000f62f, 0x0000006d, /* 0000006d //C 0f62f db	109 */
0x0000f630, 0x00000000, /* 00000000 //C 0f630 db */
0x0000f631, 0x0000f1b3, /* 0000f1b3 //C 0f631 dd	cmd_d */
0x0000f632, 0x00000064, /* 00000064 //C 0f632 db	100 */
0x0000f633, 0x00000000, /* 00000000 //C 0f633 db */
0x0000f634, 0x0000f1b3, /* 0000f1b3 //C 0f634 dd	cmd_d */
0x0000f635, 0x00000064, /* 00000064 //C 0f635 db	100 */
0x0000f636, 0x00000075, /* 00000075 //C 0f636 db	117 */
0x0000f637, 0x0000006d, /* 0000006d //C 0f637 db	109 */
0x0000f638, 0x00000070, /* 00000070 //C 0f638 db	112 */
0x0000f639, 0x00000000, /* 00000000 //C 0f639 db */
0x0000f63a, 0x0000f1e6, /* 0000f1e6 //C 0f63a dd	cmd_l */
0x0000f63b, 0x0000006c, /* 0000006c //C 0f63b db	108 */
0x0000f63c, 0x00000000, /* 00000000 //C 0f63c db */
0x0000f63d, 0x0000f1e6, /* 0000f1e6 //C 0f63d dd	cmd_l */
0x0000f63e, 0x0000006c, /* 0000006c //C 0f63e db	108 */
0x0000f63f, 0x0000006f, /* 0000006f //C 0f63f db	111 */
0x0000f640, 0x00000061, /* 00000061 //C 0f640 db	97 */
0x0000f641, 0x00000064, /* 00000064 //C 0f641 db	100 */
0x0000f642, 0x00000000, /* 00000000 //C 0f642 db */
0x0000f643, 0x0000f25c, /* 0000f25c //C 0f643 dd	cmd_g */
0x0000f644, 0x00000067, /* 00000067 //C 0f644 db	103 */
0x0000f645, 0x00000000, /* 00000000 //C 0f645 db */
0x0000f646, 0x0000f25c, /* 0000f25c //C 0f646 dd	cmd_g */
0x0000f647, 0x00000067, /* 00000067 //C 0f647 db	103 */
0x0000f648, 0x0000006f, /* 0000006f //C 0f648 db	111 */
0x0000f649, 0x00000000, /* 00000000 //C 0f649 db */
0x0000f64a, 0x0000f25c, /* 0000f25c //C 0f64a dd	cmd_g */
0x0000f64b, 0x00000072, /* 00000072 //C 0f64b db	114 */
0x0000f64c, 0x00000075, /* 00000075 //C 0f64c db	117 */
0x0000f64d, 0x0000006e, /* 0000006e //C 0f64d db	110 */
0x0000f64e, 0x00000000, /* 00000000 //C 0f64e db */
0x0000f64f, 0x0000f29b, /* 0000f29b //C 0f64f dd	cmd_h */
0x0000f650, 0x0000003f, /* 0000003f //C 0f650 db	63 */
0x0000f651, 0x00000000, /* 00000000 //C 0f651 db */
0x0000f652, 0x0000f29b, /* 0000f29b //C 0f652 dd	cmd_h */
0x0000f653, 0x00000068, /* 00000068 //C 0f653 db	104 */
0x0000f654, 0x00000000, /* 00000000 //C 0f654 db */
0x0000f655, 0x0000f29b, /* 0000f29b //C 0f655 dd	cmd_h */
0x0000f656, 0x00000068, /* 00000068 //C 0f656 db	104 */
0x0000f657, 0x00000065, /* 00000065 //C 0f657 db	101 */
0x0000f658, 0x0000006c, /* 0000006c //C 0f658 db	108 */
0x0000f659, 0x00000070, /* 00000070 //C 0f659 db	112 */
0x0000f65a, 0x00000000, /* 00000000 //C 0f65a db */
0x0000f65b, 0x0000f302, /* 0000f302 //C 0f65b dd	cmd_r */
0x0000f65c, 0x00000072, /* 00000072 //C 0f65c db	114 */
0x0000f65d, 0x00000000, /* 00000000 //C 0f65d db */
0x0000f65e, 0x0000f302, /* 0000f302 //C 0f65e dd	cmd_r */
0x0000f65f, 0x00000072, /* 00000072 //C 0f65f db	114 */
0x0000f660, 0x00000065, /* 00000065 //C 0f660 db	101 */
0x0000f661, 0x00000067, /* 00000067 //C 0f661 db	103 */
0x0000f662, 0x00000000, /* 00000000 //C 0f662 db */
0x0000f663, 0x0000f302, /* 0000f302 //C 0f663 dd	cmd_r */
0x0000f664, 0x00000072, /* 00000072 //C 0f664 db	114 */
0x0000f665, 0x00000065, /* 00000065 //C 0f665 db	101 */
0x0000f666, 0x00000067, /* 00000067 //C 0f666 db	103 */
0x0000f667, 0x00000073, /* 00000073 //C 0f667 db	115 */
0x0000f668, 0x00000000, /* 00000000 //C 0f668 db */
0x0000f669, 0x0000f392, /* 0000f392 //C 0f669 dd	cmd_sp */
0x0000f66a, 0x00000073, /* 00000073 //C 0f66a db	115 */
0x0000f66b, 0x00000070, /* 00000070 //C 0f66b db	112 */
0x0000f66c, 0x00000000, /* 00000000 //C 0f66c db */
0x0000f66d, 0x0000f395, /* 0000f395 //C 0f66d dd	cmd_lr */
0x0000f66e, 0x0000006c, /* 0000006c //C 0f66e db	108 */
0x0000f66f, 0x00000072, /* 00000072 //C 0f66f db	114 */
0x0000f670, 0x00000000, /* 00000000 //C 0f670 db */
0x0000f671, 0x0000f398, /* 0000f398 //C 0f671 dd	cmd_pc */
0x0000f672, 0x00000070, /* 00000070 //C 0f672 db	112 */
0x0000f673, 0x00000063, /* 00000063 //C 0f673 db	99 */
0x0000f674, 0x00000000, /* 00000000 //C 0f674 db */
0x0000f675, 0x0000f39b, /* 0000f39b //C 0f675 dd	cmd_f */
0x0000f676, 0x00000066, /* 00000066 //C 0f676 db	102 */
0x0000f677, 0x00000000, /* 00000000 //C 0f677 db */
0x0000f678, 0x00000000, /* 00000000 //C 0f678 dd	0 */
0x0000f679, 0x00000000, /* 00000000 //C 0f679 dd	0 */
0x0000f67a, 0x0000006d, /* 0000006d //C 0f67a db	109 */
0x0000f67b, 0x0000005b, /* 0000005b //C 0f67b db	91 */
0x0000f67c, 0x00000065, /* 00000065 //C 0f67c db	101 */
0x0000f67d, 0x0000006d, /* 0000006d //C 0f67d db	109 */
0x0000f67e, 0x0000005d, /* 0000005d //C 0f67e db	93 */
0x0000f67f, 0x00000020, /* 00000020 //C 0f67f db	32 */
0x0000f680, 0x00000061, /* 00000061 //C 0f680 db	97 */
0x0000f681, 0x00000064, /* 00000064 //C 0f681 db	100 */
0x0000f682, 0x00000064, /* 00000064 //C 0f682 db	100 */
0x0000f683, 0x00000072, /* 00000072 //C 0f683 db	114 */
0x0000f684, 0x00000020, /* 00000020 //C 0f684 db	32 */
0x0000f685, 0x0000005b, /* 0000005b //C 0f685 db	91 */
0x0000f686, 0x00000076, /* 00000076 //C 0f686 db	118 */
0x0000f687, 0x00000061, /* 00000061 //C 0f687 db	97 */
0x0000f688, 0x0000006c, /* 0000006c //C 0f688 db	108 */
0x0000f689, 0x0000005d, /* 0000005d //C 0f689 db	93 */
0x0000f68a, 0x00000020, /* 00000020 //C 0f68a db	32 */
0x0000f68b, 0x00000020, /* 00000020 //C 0f68b db	32 */
0x0000f68c, 0x00000047, /* 00000047 //C 0f68c db	71 */
0x0000f68d, 0x00000065, /* 00000065 //C 0f68d db	101 */
0x0000f68e, 0x00000074, /* 00000074 //C 0f68e db	116 */
0x0000f68f, 0x0000002f, /* 0000002f //C 0f68f db	47 */
0x0000f690, 0x00000073, /* 00000073 //C 0f690 db	115 */
0x0000f691, 0x00000065, /* 00000065 //C 0f691 db	101 */
0x0000f692, 0x00000074, /* 00000074 //C 0f692 db	116 */
0x0000f693, 0x00000020, /* 00000020 //C 0f693 db	32 */
0x0000f694, 0x0000006d, /* 0000006d //C 0f694 db	109 */
0x0000f695, 0x00000065, /* 00000065 //C 0f695 db	101 */
0x0000f696, 0x0000006d, /* 0000006d //C 0f696 db	109 */
0x0000f697, 0x0000006f, /* 0000006f //C 0f697 db	111 */
0x0000f698, 0x00000072, /* 00000072 //C 0f698 db	114 */
0x0000f699, 0x00000079, /* 00000079 //C 0f699 db	121 */
0x0000f69a, 0x0000000a, /* 0000000a //C 0f69a db	10 */
0x0000f69b, 0x00000000, /* 00000000 //C 0f69b db */
0x0000f69c, 0x00000064, /* 00000064 //C 0f69c db	100 */
0x0000f69d, 0x0000005b, /* 0000005b //C 0f69d db	91 */
0x0000f69e, 0x00000075, /* 00000075 //C 0f69e db	117 */
0x0000f69f, 0x0000006d, /* 0000006d //C 0f69f db	109 */
0x0000f6a0, 0x00000070, /* 00000070 //C 0f6a0 db	112 */
0x0000f6a1, 0x0000005d, /* 0000005d //C 0f6a1 db	93 */
0x0000f6a2, 0x00000020, /* 00000020 //C 0f6a2 db	32 */
0x0000f6a3, 0x00000073, /* 00000073 //C 0f6a3 db	115 */
0x0000f6a4, 0x00000074, /* 00000074 //C 0f6a4 db	116 */
0x0000f6a5, 0x00000061, /* 00000061 //C 0f6a5 db	97 */
0x0000f6a6, 0x00000072, /* 00000072 //C 0f6a6 db	114 */
0x0000f6a7, 0x00000074, /* 00000074 //C 0f6a7 db	116 */
0x0000f6a8, 0x00000020, /* 00000020 //C 0f6a8 db	32 */
0x0000f6a9, 0x00000065, /* 00000065 //C 0f6a9 db	101 */
0x0000f6aa, 0x0000006e, /* 0000006e //C 0f6aa db	110 */
0x0000f6ab, 0x00000064, /* 00000064 //C 0f6ab db	100 */
0x0000f6ac, 0x00000020, /* 00000020 //C 0f6ac db	32 */
0x0000f6ad, 0x00000020, /* 00000020 //C 0f6ad db	32 */
0x0000f6ae, 0x00000044, /* 00000044 //C 0f6ae db	68 */
0x0000f6af, 0x00000075, /* 00000075 //C 0f6af db	117 */
0x0000f6b0, 0x0000006d, /* 0000006d //C 0f6b0 db	109 */
0x0000f6b1, 0x00000070, /* 00000070 //C 0f6b1 db	112 */
0x0000f6b2, 0x00000020, /* 00000020 //C 0f6b2 db	32 */
0x0000f6b3, 0x0000006d, /* 0000006d //C 0f6b3 db	109 */
0x0000f6b4, 0x00000065, /* 00000065 //C 0f6b4 db	101 */
0x0000f6b5, 0x0000006d, /* 0000006d //C 0f6b5 db	109 */
0x0000f6b6, 0x0000006f, /* 0000006f //C 0f6b6 db	111 */
0x0000f6b7, 0x00000072, /* 00000072 //C 0f6b7 db	114 */
0x0000f6b8, 0x00000079, /* 00000079 //C 0f6b8 db	121 */
0x0000f6b9, 0x00000020, /* 00000020 //C 0f6b9 db	32 */
0x0000f6ba, 0x00000063, /* 00000063 //C 0f6ba db	99 */
0x0000f6bb, 0x0000006f, /* 0000006f //C 0f6bb db	111 */
0x0000f6bc, 0x0000006e, /* 0000006e //C 0f6bc db	110 */
0x0000f6bd, 0x00000074, /* 00000074 //C 0f6bd db	116 */
0x0000f6be, 0x00000065, /* 00000065 //C 0f6be db	101 */
0x0000f6bf, 0x0000006e, /* 0000006e //C 0f6bf db	110 */
0x0000f6c0, 0x00000074, /* 00000074 //C 0f6c0 db	116 */
0x0000f6c1, 0x0000000a, /* 0000000a //C 0f6c1 db	10 */
0x0000f6c2, 0x00000000, /* 00000000 //C 0f6c2 db */
0x0000f6c3, 0x0000006c, /* 0000006c //C 0f6c3 db	108 */
0x0000f6c4, 0x0000005b, /* 0000005b //C 0f6c4 db	91 */
0x0000f6c5, 0x0000006f, /* 0000006f //C 0f6c5 db	111 */
0x0000f6c6, 0x00000061, /* 00000061 //C 0f6c6 db	97 */
0x0000f6c7, 0x00000064, /* 00000064 //C 0f6c7 db	100 */
0x0000f6c8, 0x0000005d, /* 0000005d //C 0f6c8 db	93 */
0x0000f6c9, 0x00000020, /* 00000020 //C 0f6c9 db	32 */
0x0000f6ca, 0x00000020, /* 00000020 //C 0f6ca db	32 */
0x0000f6cb, 0x00000020, /* 00000020 //C 0f6cb db	32 */
0x0000f6cc, 0x00000020, /* 00000020 //C 0f6cc db	32 */
0x0000f6cd, 0x00000020, /* 00000020 //C 0f6cd db	32 */
0x0000f6ce, 0x00000020, /* 00000020 //C 0f6ce db	32 */
0x0000f6cf, 0x00000020, /* 00000020 //C 0f6cf db	32 */
0x0000f6d0, 0x00000020, /* 00000020 //C 0f6d0 db	32 */
0x0000f6d1, 0x00000020, /* 00000020 //C 0f6d1 db	32 */
0x0000f6d2, 0x00000020, /* 00000020 //C 0f6d2 db	32 */
0x0000f6d3, 0x00000020, /* 00000020 //C 0f6d3 db	32 */
0x0000f6d4, 0x00000020, /* 00000020 //C 0f6d4 db	32 */
0x0000f6d5, 0x0000004c, /* 0000004c //C 0f6d5 db	76 */
0x0000f6d6, 0x0000006f, /* 0000006f //C 0f6d6 db	111 */
0x0000f6d7, 0x00000061, /* 00000061 //C 0f6d7 db	97 */
0x0000f6d8, 0x00000064, /* 00000064 //C 0f6d8 db	100 */
0x0000f6d9, 0x00000020, /* 00000020 //C 0f6d9 db	32 */
0x0000f6da, 0x00000068, /* 00000068 //C 0f6da db	104 */
0x0000f6db, 0x00000065, /* 00000065 //C 0f6db db	101 */
0x0000f6dc, 0x00000078, /* 00000078 //C 0f6dc db	120 */
0x0000f6dd, 0x00000020, /* 00000020 //C 0f6dd db	32 */
0x0000f6de, 0x00000066, /* 00000066 //C 0f6de db	102 */
0x0000f6df, 0x00000069, /* 00000069 //C 0f6df db	105 */
0x0000f6e0, 0x0000006c, /* 0000006c //C 0f6e0 db	108 */
0x0000f6e1, 0x00000065, /* 00000065 //C 0f6e1 db	101 */
0x0000f6e2, 0x00000020, /* 00000020 //C 0f6e2 db	32 */
0x0000f6e3, 0x00000074, /* 00000074 //C 0f6e3 db	116 */
0x0000f6e4, 0x0000006f, /* 0000006f //C 0f6e4 db	111 */
0x0000f6e5, 0x00000020, /* 00000020 //C 0f6e5 db	32 */
0x0000f6e6, 0x0000006d, /* 0000006d //C 0f6e6 db	109 */
0x0000f6e7, 0x00000065, /* 00000065 //C 0f6e7 db	101 */
0x0000f6e8, 0x0000006d, /* 0000006d //C 0f6e8 db	109 */
0x0000f6e9, 0x0000006f, /* 0000006f //C 0f6e9 db	111 */
0x0000f6ea, 0x00000072, /* 00000072 //C 0f6ea db	114 */
0x0000f6eb, 0x00000079, /* 00000079 //C 0f6eb db	121 */
0x0000f6ec, 0x0000000a, /* 0000000a //C 0f6ec db	10 */
0x0000f6ed, 0x00000000, /* 00000000 //C 0f6ed db */
0x0000f6ee, 0x00000067, /* 00000067 //C 0f6ee db	103 */
0x0000f6ef, 0x0000005b, /* 0000005b //C 0f6ef db	91 */
0x0000f6f0, 0x0000006f, /* 0000006f //C 0f6f0 db	111 */
0x0000f6f1, 0x0000005d, /* 0000005d //C 0f6f1 db	93 */
0x0000f6f2, 0x0000007c, /* 0000007c //C 0f6f2 db	124 */
0x0000f6f3, 0x00000072, /* 00000072 //C 0f6f3 db	114 */
0x0000f6f4, 0x00000075, /* 00000075 //C 0f6f4 db	117 */
0x0000f6f5, 0x0000006e, /* 0000006e //C 0f6f5 db	110 */
0x0000f6f6, 0x00000020, /* 00000020 //C 0f6f6 db	32 */
0x0000f6f7, 0x0000005b, /* 0000005b //C 0f6f7 db	91 */
0x0000f6f8, 0x00000061, /* 00000061 //C 0f6f8 db	97 */
0x0000f6f9, 0x00000064, /* 00000064 //C 0f6f9 db	100 */
0x0000f6fa, 0x00000064, /* 00000064 //C 0f6fa db	100 */
0x0000f6fb, 0x00000072, /* 00000072 //C 0f6fb db	114 */
0x0000f6fc, 0x0000005d, /* 0000005d //C 0f6fc db	93 */
0x0000f6fd, 0x00000020, /* 00000020 //C 0f6fd db	32 */
0x0000f6fe, 0x00000020, /* 00000020 //C 0f6fe db	32 */
0x0000f6ff, 0x00000020, /* 00000020 //C 0f6ff db	32 */
0x0000f700, 0x00000052, /* 00000052 //C 0f700 db	82 */
0x0000f701, 0x00000075, /* 00000075 //C 0f701 db	117 */
0x0000f702, 0x0000006e, /* 0000006e //C 0f702 db	110 */
0x0000f703, 0x00000020, /* 00000020 //C 0f703 db	32 */
0x0000f704, 0x00000066, /* 00000066 //C 0f704 db	102 */
0x0000f705, 0x00000072, /* 00000072 //C 0f705 db	114 */
0x0000f706, 0x0000006f, /* 0000006f //C 0f706 db	111 */
0x0000f707, 0x0000006d, /* 0000006d //C 0f707 db	109 */
0x0000f708, 0x00000020, /* 00000020 //C 0f708 db	32 */
0x0000f709, 0x00000061, /* 00000061 //C 0f709 db	97 */
0x0000f70a, 0x00000064, /* 00000064 //C 0f70a db	100 */
0x0000f70b, 0x00000064, /* 00000064 //C 0f70b db	100 */
0x0000f70c, 0x00000072, /* 00000072 //C 0f70c db	114 */
0x0000f70d, 0x00000065, /* 00000065 //C 0f70d db	101 */
0x0000f70e, 0x00000073, /* 00000073 //C 0f70e db	115 */
0x0000f70f, 0x00000073, /* 00000073 //C 0f70f db	115 */
0x0000f710, 0x0000000a, /* 0000000a //C 0f710 db	10 */
0x0000f711, 0x00000000, /* 00000000 //C 0f711 db */
0x0000f712, 0x00000072, /* 00000072 //C 0f712 db	114 */
0x0000f713, 0x0000005b, /* 0000005b //C 0f713 db	91 */
0x0000f714, 0x00000065, /* 00000065 //C 0f714 db	101 */
0x0000f715, 0x00000067, /* 00000067 //C 0f715 db	103 */
0x0000f716, 0x0000005b, /* 0000005b //C 0f716 db	91 */
0x0000f717, 0x00000073, /* 00000073 //C 0f717 db	115 */
0x0000f718, 0x0000005d, /* 0000005d //C 0f718 db	93 */
0x0000f719, 0x0000005d, /* 0000005d //C 0f719 db	93 */
0x0000f71a, 0x00000020, /* 00000020 //C 0f71a db	32 */
0x0000f71b, 0x00000020, /* 00000020 //C 0f71b db	32 */
0x0000f71c, 0x00000020, /* 00000020 //C 0f71c db	32 */
0x0000f71d, 0x00000020, /* 00000020 //C 0f71d db	32 */
0x0000f71e, 0x00000020, /* 00000020 //C 0f71e db	32 */
0x0000f71f, 0x00000020, /* 00000020 //C 0f71f db	32 */
0x0000f720, 0x00000020, /* 00000020 //C 0f720 db	32 */
0x0000f721, 0x00000020, /* 00000020 //C 0f721 db	32 */
0x0000f722, 0x00000020, /* 00000020 //C 0f722 db	32 */
0x0000f723, 0x00000020, /* 00000020 //C 0f723 db	32 */
0x0000f724, 0x00000050, /* 00000050 //C 0f724 db	80 */
0x0000f725, 0x00000072, /* 00000072 //C 0f725 db	114 */
0x0000f726, 0x00000069, /* 00000069 //C 0f726 db	105 */
0x0000f727, 0x0000006e, /* 0000006e //C 0f727 db	110 */
0x0000f728, 0x00000074, /* 00000074 //C 0f728 db	116 */
0x0000f729, 0x00000020, /* 00000020 //C 0f729 db	32 */
0x0000f72a, 0x00000072, /* 00000072 //C 0f72a db	114 */
0x0000f72b, 0x00000065, /* 00000065 //C 0f72b db	101 */
0x0000f72c, 0x00000067, /* 00000067 //C 0f72c db	103 */
0x0000f72d, 0x00000069, /* 00000069 //C 0f72d db	105 */
0x0000f72e, 0x00000073, /* 00000073 //C 0f72e db	115 */
0x0000f72f, 0x00000074, /* 00000074 //C 0f72f db	116 */
0x0000f730, 0x00000065, /* 00000065 //C 0f730 db	101 */
0x0000f731, 0x00000072, /* 00000072 //C 0f731 db	114 */
0x0000f732, 0x00000073, /* 00000073 //C 0f732 db	115 */
0x0000f733, 0x0000000a, /* 0000000a //C 0f733 db	10 */
0x0000f734, 0x00000000, /* 00000000 //C 0f734 db */
0x0000f735, 0x00000072, /* 00000072 //C 0f735 db	114 */
0x0000f736, 0x00000058, /* 00000058 //C 0f736 db	88 */
0x0000f737, 0x00000020, /* 00000020 //C 0f737 db	32 */
0x0000f738, 0x0000005b, /* 0000005b //C 0f738 db	91 */
0x0000f739, 0x00000076, /* 00000076 //C 0f739 db	118 */
0x0000f73a, 0x00000061, /* 00000061 //C 0f73a db	97 */
0x0000f73b, 0x0000006c, /* 0000006c //C 0f73b db	108 */
0x0000f73c, 0x0000005d, /* 0000005d //C 0f73c db	93 */
0x0000f73d, 0x00000020, /* 00000020 //C 0f73d db	32 */
0x0000f73e, 0x00000020, /* 00000020 //C 0f73e db	32 */
0x0000f73f, 0x00000020, /* 00000020 //C 0f73f db	32 */
0x0000f740, 0x00000020, /* 00000020 //C 0f740 db	32 */
0x0000f741, 0x00000020, /* 00000020 //C 0f741 db	32 */
0x0000f742, 0x00000020, /* 00000020 //C 0f742 db	32 */
0x0000f743, 0x00000020, /* 00000020 //C 0f743 db	32 */
0x0000f744, 0x00000020, /* 00000020 //C 0f744 db	32 */
0x0000f745, 0x00000020, /* 00000020 //C 0f745 db	32 */
0x0000f746, 0x00000020, /* 00000020 //C 0f746 db	32 */
0x0000f747, 0x00000047, /* 00000047 //C 0f747 db	71 */
0x0000f748, 0x00000065, /* 00000065 //C 0f748 db	101 */
0x0000f749, 0x00000074, /* 00000074 //C 0f749 db	116 */
0x0000f74a, 0x0000002f, /* 0000002f //C 0f74a db	47 */
0x0000f74b, 0x00000073, /* 00000073 //C 0f74b db	115 */
0x0000f74c, 0x00000065, /* 00000065 //C 0f74c db	101 */
0x0000f74d, 0x00000074, /* 00000074 //C 0f74d db	116 */
0x0000f74e, 0x00000020, /* 00000020 //C 0f74e db	32 */
0x0000f74f, 0x00000052, /* 00000052 //C 0f74f db	82 */
0x0000f750, 0x00000058, /* 00000058 //C 0f750 db	88 */
0x0000f751, 0x0000000a, /* 0000000a //C 0f751 db	10 */
0x0000f752, 0x00000000, /* 00000000 //C 0f752 db */
0x0000f753, 0x00000073, /* 00000073 //C 0f753 db	115 */
0x0000f754, 0x00000070, /* 00000070 //C 0f754 db	112 */
0x0000f755, 0x00000020, /* 00000020 //C 0f755 db	32 */
0x0000f756, 0x0000005b, /* 0000005b //C 0f756 db	91 */
0x0000f757, 0x00000076, /* 00000076 //C 0f757 db	118 */
0x0000f758, 0x00000061, /* 00000061 //C 0f758 db	97 */
0x0000f759, 0x0000006c, /* 0000006c //C 0f759 db	108 */
0x0000f75a, 0x0000005d, /* 0000005d //C 0f75a db	93 */
0x0000f75b, 0x00000020, /* 00000020 //C 0f75b db	32 */
0x0000f75c, 0x00000020, /* 00000020 //C 0f75c db	32 */
0x0000f75d, 0x00000020, /* 00000020 //C 0f75d db	32 */
0x0000f75e, 0x00000020, /* 00000020 //C 0f75e db	32 */
0x0000f75f, 0x00000020, /* 00000020 //C 0f75f db	32 */
0x0000f760, 0x00000020, /* 00000020 //C 0f760 db	32 */
0x0000f761, 0x00000020, /* 00000020 //C 0f761 db	32 */
0x0000f762, 0x00000020, /* 00000020 //C 0f762 db	32 */
0x0000f763, 0x00000020, /* 00000020 //C 0f763 db	32 */
0x0000f764, 0x00000020, /* 00000020 //C 0f764 db	32 */
0x0000f765, 0x00000047, /* 00000047 //C 0f765 db	71 */
0x0000f766, 0x00000065, /* 00000065 //C 0f766 db	101 */
0x0000f767, 0x00000074, /* 00000074 //C 0f767 db	116 */
0x0000f768, 0x0000002f, /* 0000002f //C 0f768 db	47 */
0x0000f769, 0x00000073, /* 00000073 //C 0f769 db	115 */
0x0000f76a, 0x00000065, /* 00000065 //C 0f76a db	101 */
0x0000f76b, 0x00000074, /* 00000074 //C 0f76b db	116 */
0x0000f76c, 0x00000020, /* 00000020 //C 0f76c db	32 */
0x0000f76d, 0x00000052, /* 00000052 //C 0f76d db	82 */
0x0000f76e, 0x00000031, /* 00000031 //C 0f76e db	49 */
0x0000f76f, 0x00000033, /* 00000033 //C 0f76f db	51 */
0x0000f770, 0x0000000a, /* 0000000a //C 0f770 db	10 */
0x0000f771, 0x00000000, /* 00000000 //C 0f771 db */
0x0000f772, 0x0000006c, /* 0000006c //C 0f772 db	108 */
0x0000f773, 0x00000072, /* 00000072 //C 0f773 db	114 */
0x0000f774, 0x00000020, /* 00000020 //C 0f774 db	32 */
0x0000f775, 0x0000005b, /* 0000005b //C 0f775 db	91 */
0x0000f776, 0x00000076, /* 00000076 //C 0f776 db	118 */
0x0000f777, 0x00000061, /* 00000061 //C 0f777 db	97 */
0x0000f778, 0x0000006c, /* 0000006c //C 0f778 db	108 */
0x0000f779, 0x0000005d, /* 0000005d //C 0f779 db	93 */
0x0000f77a, 0x00000020, /* 00000020 //C 0f77a db	32 */
0x0000f77b, 0x00000020, /* 00000020 //C 0f77b db	32 */
0x0000f77c, 0x00000020, /* 00000020 //C 0f77c db	32 */
0x0000f77d, 0x00000020, /* 00000020 //C 0f77d db	32 */
0x0000f77e, 0x00000020, /* 00000020 //C 0f77e db	32 */
0x0000f77f, 0x00000020, /* 00000020 //C 0f77f db	32 */
0x0000f780, 0x00000020, /* 00000020 //C 0f780 db	32 */
0x0000f781, 0x00000020, /* 00000020 //C 0f781 db	32 */
0x0000f782, 0x00000020, /* 00000020 //C 0f782 db	32 */
0x0000f783, 0x00000020, /* 00000020 //C 0f783 db	32 */
0x0000f784, 0x00000047, /* 00000047 //C 0f784 db	71 */
0x0000f785, 0x00000065, /* 00000065 //C 0f785 db	101 */
0x0000f786, 0x00000074, /* 00000074 //C 0f786 db	116 */
0x0000f787, 0x0000002f, /* 0000002f //C 0f787 db	47 */
0x0000f788, 0x00000073, /* 00000073 //C 0f788 db	115 */
0x0000f789, 0x00000065, /* 00000065 //C 0f789 db	101 */
0x0000f78a, 0x00000074, /* 00000074 //C 0f78a db	116 */
0x0000f78b, 0x00000020, /* 00000020 //C 0f78b db	32 */
0x0000f78c, 0x00000052, /* 00000052 //C 0f78c db	82 */
0x0000f78d, 0x00000031, /* 00000031 //C 0f78d db	49 */
0x0000f78e, 0x00000034, /* 00000034 //C 0f78e db	52 */
0x0000f78f, 0x0000000a, /* 0000000a //C 0f78f db	10 */
0x0000f790, 0x00000000, /* 00000000 //C 0f790 db */
0x0000f791, 0x00000070, /* 00000070 //C 0f791 db	112 */
0x0000f792, 0x00000063, /* 00000063 //C 0f792 db	99 */
0x0000f793, 0x00000020, /* 00000020 //C 0f793 db	32 */
0x0000f794, 0x0000005b, /* 0000005b //C 0f794 db	91 */
0x0000f795, 0x00000076, /* 00000076 //C 0f795 db	118 */
0x0000f796, 0x00000061, /* 00000061 //C 0f796 db	97 */
0x0000f797, 0x0000006c, /* 0000006c //C 0f797 db	108 */
0x0000f798, 0x0000005d, /* 0000005d //C 0f798 db	93 */
0x0000f799, 0x00000020, /* 00000020 //C 0f799 db	32 */
0x0000f79a, 0x00000020, /* 00000020 //C 0f79a db	32 */
0x0000f79b, 0x00000020, /* 00000020 //C 0f79b db	32 */
0x0000f79c, 0x00000020, /* 00000020 //C 0f79c db	32 */
0x0000f79d, 0x00000020, /* 00000020 //C 0f79d db	32 */
0x0000f79e, 0x00000020, /* 00000020 //C 0f79e db	32 */
0x0000f79f, 0x00000020, /* 00000020 //C 0f79f db	32 */
0x0000f7a0, 0x00000020, /* 00000020 //C 0f7a0 db	32 */
0x0000f7a1, 0x00000020, /* 00000020 //C 0f7a1 db	32 */
0x0000f7a2, 0x00000020, /* 00000020 //C 0f7a2 db	32 */
0x0000f7a3, 0x00000047, /* 00000047 //C 0f7a3 db	71 */
0x0000f7a4, 0x00000065, /* 00000065 //C 0f7a4 db	101 */
0x0000f7a5, 0x00000074, /* 00000074 //C 0f7a5 db	116 */
0x0000f7a6, 0x0000002f, /* 0000002f //C 0f7a6 db	47 */
0x0000f7a7, 0x00000073, /* 00000073 //C 0f7a7 db	115 */
0x0000f7a8, 0x00000065, /* 00000065 //C 0f7a8 db	101 */
0x0000f7a9, 0x00000074, /* 00000074 //C 0f7a9 db	116 */
0x0000f7aa, 0x00000020, /* 00000020 //C 0f7aa db	32 */
0x0000f7ab, 0x00000052, /* 00000052 //C 0f7ab db	82 */
0x0000f7ac, 0x00000031, /* 00000031 //C 0f7ac db	49 */
0x0000f7ad, 0x00000035, /* 00000035 //C 0f7ad db	53 */
0x0000f7ae, 0x0000000a, /* 0000000a //C 0f7ae db	10 */
0x0000f7af, 0x00000000, /* 00000000 //C 0f7af db */
0x0000f7b0, 0x00000066, /* 00000066 //C 0f7b0 db	102 */
0x0000f7b1, 0x00000020, /* 00000020 //C 0f7b1 db	32 */
0x0000f7b2, 0x0000005b, /* 0000005b //C 0f7b2 db	91 */
0x0000f7b3, 0x00000076, /* 00000076 //C 0f7b3 db	118 */
0x0000f7b4, 0x00000061, /* 00000061 //C 0f7b4 db	97 */
0x0000f7b5, 0x0000006c, /* 0000006c //C 0f7b5 db	108 */
0x0000f7b6, 0x0000005d, /* 0000005d //C 0f7b6 db	93 */
0x0000f7b7, 0x00000020, /* 00000020 //C 0f7b7 db	32 */
0x0000f7b8, 0x00000020, /* 00000020 //C 0f7b8 db	32 */
0x0000f7b9, 0x00000020, /* 00000020 //C 0f7b9 db	32 */
0x0000f7ba, 0x00000020, /* 00000020 //C 0f7ba db	32 */
0x0000f7bb, 0x00000020, /* 00000020 //C 0f7bb db	32 */
0x0000f7bc, 0x00000020, /* 00000020 //C 0f7bc db	32 */
0x0000f7bd, 0x00000020, /* 00000020 //C 0f7bd db	32 */
0x0000f7be, 0x00000020, /* 00000020 //C 0f7be db	32 */
0x0000f7bf, 0x00000020, /* 00000020 //C 0f7bf db	32 */
0x0000f7c0, 0x00000020, /* 00000020 //C 0f7c0 db	32 */
0x0000f7c1, 0x00000020, /* 00000020 //C 0f7c1 db	32 */
0x0000f7c2, 0x00000047, /* 00000047 //C 0f7c2 db	71 */
0x0000f7c3, 0x00000065, /* 00000065 //C 0f7c3 db	101 */
0x0000f7c4, 0x00000074, /* 00000074 //C 0f7c4 db	116 */
0x0000f7c5, 0x0000002f, /* 0000002f //C 0f7c5 db	47 */
0x0000f7c6, 0x00000073, /* 00000073 //C 0f7c6 db	115 */
0x0000f7c7, 0x00000065, /* 00000065 //C 0f7c7 db	101 */
0x0000f7c8, 0x00000074, /* 00000074 //C 0f7c8 db	116 */
0x0000f7c9, 0x00000020, /* 00000020 //C 0f7c9 db	32 */
0x0000f7ca, 0x00000066, /* 00000066 //C 0f7ca db	102 */
0x0000f7cb, 0x0000006c, /* 0000006c //C 0f7cb db	108 */
0x0000f7cc, 0x00000061, /* 00000061 //C 0f7cc db	97 */
0x0000f7cd, 0x00000067, /* 00000067 //C 0f7cd db	103 */
0x0000f7ce, 0x00000073, /* 00000073 //C 0f7ce db	115 */
0x0000f7cf, 0x0000000a, /* 0000000a //C 0f7cf db	10 */
0x0000f7d0, 0x00000000, /* 00000000 //C 0f7d0 db */
0x0000f7d1, 0x00000068, /* 00000068 //C 0f7d1 db	104 */
0x0000f7d2, 0x0000005b, /* 0000005b //C 0f7d2 db	91 */
0x0000f7d3, 0x00000065, /* 00000065 //C 0f7d3 db	101 */
0x0000f7d4, 0x0000006c, /* 0000006c //C 0f7d4 db	108 */
0x0000f7d5, 0x00000070, /* 00000070 //C 0f7d5 db	112 */
0x0000f7d6, 0x0000005d, /* 0000005d //C 0f7d6 db	93 */
0x0000f7d7, 0x0000002c, /* 0000002c //C 0f7d7 db	44 */
0x0000f7d8, 0x0000003f, /* 0000003f //C 0f7d8 db	63 */
0x0000f7d9, 0x00000020, /* 00000020 //C 0f7d9 db	32 */
0x0000f7da, 0x00000020, /* 00000020 //C 0f7da db	32 */
0x0000f7db, 0x00000020, /* 00000020 //C 0f7db db	32 */
0x0000f7dc, 0x00000020, /* 00000020 //C 0f7dc db	32 */
0x0000f7dd, 0x00000020, /* 00000020 //C 0f7dd db	32 */
0x0000f7de, 0x00000020, /* 00000020 //C 0f7de db	32 */
0x0000f7df, 0x00000020, /* 00000020 //C 0f7df db	32 */
0x0000f7e0, 0x00000020, /* 00000020 //C 0f7e0 db	32 */
0x0000f7e1, 0x00000020, /* 00000020 //C 0f7e1 db	32 */
0x0000f7e2, 0x00000020, /* 00000020 //C 0f7e2 db	32 */
0x0000f7e3, 0x00000048, /* 00000048 //C 0f7e3 db	72 */
0x0000f7e4, 0x00000065, /* 00000065 //C 0f7e4 db	101 */
0x0000f7e5, 0x0000006c, /* 0000006c //C 0f7e5 db	108 */
0x0000f7e6, 0x00000070, /* 00000070 //C 0f7e6 db	112 */
0x0000f7e7, 0x0000000a, /* 0000000a //C 0f7e7 db	10 */
0x0000f7e8, 0x00000000, /* 00000000 //C 0f7e8 db */
0x0000f7e9, 0x00000000, /* 00000000 //C 0f7e9 dd	0 */
0x0000f7ea, 0x00000000, /* 00000000 //C 0f7ea ds	0x40 */
0x0000f82a, 0x00000000, /* 00000000 //C 0f82a ds	1 */
0x0000f82b, 0x0ded0000, /* 0ded0000 //C 0f82b push	lr */
0x0000f82c, 0x0d2d0000, /* 0d2d0000 //C 0f82c push	r2 */
0x0000f82d, 0x0d3d0000, /* 0d3d0000 //C 0f82d push	r3 */
0x0000f82e, 0x01120000, /* 01120000 //C 0f82e mvzl	r1,0		; tmp value */
0x0000f82f, 0x00200000, /* 00200000 //C 0f82f mov	r2,r0		; address in r2 */
0x0000f830, 0x01320000, /* 01320000 //C 0f830 mvzl	r3,0		; index */
0x0000f831, 0x0b038200, /* 0b038200 //C 0f831 ld	r0,r3+,r2	; pick a char */
0x0000f832, 0x020b0000, /* 020b0000 //C 0f832 sz	r0		; end of string? */
0x0000f833, 0x11f2f83a, /* 11f2f83a //C 0f833 jz	dtoi_true	; normal exit */
0x0000f834, 0x0400f840, /* 0400f840 //C 0f834 call	isdigit	; check ascii char */
0x0000f835, 0x41f2f83c, /* 41f2f83c //C 0f835 jnc	dtoi_false	; exit if not a number */
0x0000f836, 0x01060030, /* 01060030 //C 0f836 sub	r0,'0'		; convert char to number */
0x0000f837, 0x0119000a, /* 0119000a //C 0f837 mul	r1,10		; shift tmp */
0x0000f838, 0x00140000, /* 00140000 //C 0f838 add	r1,r0		; add actual number */
0x0000f839, 0x01f2f831, /* 01f2f831 //C 0f839 jmp	dtoi_cyc */
0x0000f83a, 0x020c0000, /* 020c0000 //C 0f83a sec */
0x0000f83b, 0x01f2f83d, /* 01f2f83d //C 0f83b jmp	dtoi_ret */
0x0000f83c, 0x020d0000, /* 020d0000 //C 0f83c clc */
0x0000f83d, 0x0f3d0000, /* 0f3d0000 //C 0f83d pop	r3 */
0x0000f83e, 0x0f2d0000, /* 0f2d0000 //C 0f83e pop	r2 */
0x0000f83f, 0x0ffd0000, /* 0ffd0000 //C 0f83f pop	pc */
0x0000f840, 0x01080030, /* 01080030 //C 0f840 cmp	R0,'0' */
0x0000f841, 0x41f2f846, /* 41f2f846 //C 0f841 ULT jmp	isd_false */
0x0000f842, 0x01080039, /* 01080039 //C 0f842 cmp	R0,'9' */
0x0000f843, 0x91f2f846, /* 91f2f846 //C 0f843 UGT jmp	isd_false */
0x0000f844, 0x020c0000, /* 020c0000 //C 0f844 sec */
0x0000f845, 0x00f00e00, /* 00f00e00 //C 0f845 ret */
0x0000f846, 0x020d0000, /* 020d0000 //C 0f846 clc */
0x0000f847, 0x00f00e00, /* 00f00e00 //C 0f847 ret */
0x0000f848, 0x00000000, /* 00000000 //C 0f848  */
0xffffffff, 0xffffffff
};
