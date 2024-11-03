#include "pmon.h"

const char * p12cpu_version= "2.2.7";

t_mem pmon[]= {
0x00000000, 0x01f2f03c, /* 01f2f03c //C 00000    24 jmp	cold_start */
0x0000f000, 0x01f2f025, /* 01f2f025 //C 0f000    29 _f000:	jmp	callin */
0x0000f001, 0x01f2f016, /* 01f2f016 //C 0f001    30 _f001:	jmp	enter_by_uart */
0x0000f002, 0x01f2f492, /* 01f2f492 //C 0f002    31 _f002:	jmp	getchar */
0x0000f003, 0x01f2f5fe, /* 01f2f5fe //C 0f003    32 _f003:	jmp	version */
0x0000f004, 0x01f2f55b, /* 01f2f55b //C 0f004    33 _f004:	jmp	itobcd */
0x0000f005, 0x01f2f03c, /* 01f2f03c //C 0f005    34 _f005:	jmp	cold_start */
0x0000f006, 0x01f2f3a4, /* 01f2f3a4 //C 0f006    35 _f006:	jmp	strchr */
0x0000f007, 0x01f2f416, /* 01f2f416 //C 0f007    36 _f007:	jmp	streq */
0x0000f008, 0x01f2f487, /* 01f2f487 //C 0f008    37 _f008:	jmp	check_uart */
0x0000f009, 0x01f2f426, /* 01f2f426 //C 0f009    38 _f009:	jmp	hexchar2value */
0x0000f00a, 0x01f2f455, /* 01f2f455 //C 0f00a    39 _f00a:	jmp	value2hexchar */
0x0000f00b, 0x01f2f459, /* 01f2f459 //C 0f00b    40 _f00b:	jmp	htoi */
0x0000f00c, 0x01f2f41e, /* 01f2f41e //C 0f00c    41 _f00c:	jmp	strieq */
0x0000f00d, 0x01f2f48f, /* 01f2f48f //C 0f00d    42 _f00d:	jmp	read */
0x0000f00e, 0x01f2f496, /* 01f2f496 //C 0f00e    43 _f00e:	jmp	putchar */
0x0000f00f, 0x01f2f49d, /* 01f2f49d //C 0f00f    44 _f00f:	jmp	prints */
0x0000f010, 0x01f2f4d6, /* 01f2f4d6 //C 0f010    45 _f010:	jmp	printsnl */
0x0000f011, 0x01f2f4dd, /* 01f2f4dd //C 0f011    46 _f011:	jmp	print_vhex */
0x0000f012, 0x01f2f4bc, /* 01f2f4bc //C 0f012    47 _f012:	jmp	pes */
0x0000f013, 0x01f2f572, /* 01f2f572 //C 0f013    48 _f013:	jmp	printd */
0x0000f014, 0x01f2f577, /* 01f2f577 //C 0f014    49 _f014:	jmp	printf */
0x0000f015, 0x01f2f5f5, /* 01f2f5f5 //C 0f015    50 _f015:	jmp	pesf */
0x0000f016, 0x0d0d0000, /* 0d0d0000 //C 0f016    53 push	r0 */
0x0000f017, 0x020e0000, /* 020e0000 //C 0f017    54 getf	r0 */
0x0000f018, 0x0d0d0000, /* 0d0d0000 //C 0f018    55 push	r0 */
0x0000f019, 0x0700ff42, /* 0700ff42 //C 0f019    56 ld	r0,UART_RSTAT */
0x0000f01a, 0x010c0001, /* 010c0001 //C 0f01a    57 test	r0,1 */
0x0000f01b, 0x21f2f020, /* 21f2f020 //C 0f01b    58 jnz	ebu_callin */
0x0000f01c, 0x0f0d0000, /* 0f0d0000 //C 0f01c    60 pop	r0 */
0x0000f01d, 0x020f0000, /* 020f0000 //C 0f01d    61 setf	r0 */
0x0000f01e, 0x0f0d0000, /* 0f0d0000 //C 0f01e    62 pop	r0 */
0x0000f01f, 0x00f00e00, /* 00f00e00 //C 0f01f    63 ret */
0x0000f020, 0x0700ff40, /* 0700ff40 //C 0f020    65 ld	r0,UART_DR */
0x0000f021, 0x0f0d0000, /* 0f0d0000 //C 0f021    66 pop	r0 */
0x0000f022, 0x020f0000, /* 020f0000 //C 0f022    67 setf	r0 */
0x0000f023, 0x0f0d0000, /* 0f0d0000 //C 0f023    68 pop	r0 */
0x0000f024, 0x01f2f025, /* 01f2f025 //C 0f024    69 jmp	callin */
0x0000f025, 0x0600f672, /* 0600f672 //C 0f025    72 st	r0,reg0 */
0x0000f026, 0x0610f673, /* 0610f673 //C 0f026    73 st	r1,reg1 */
0x0000f027, 0x0620f674, /* 0620f674 //C 0f027    74 st	r2,reg2 */
0x0000f028, 0x0630f675, /* 0630f675 //C 0f028    75 st	r3,reg3 */
0x0000f029, 0x0640f676, /* 0640f676 //C 0f029    76 st	r4,reg4 */
0x0000f02a, 0x0650f677, /* 0650f677 //C 0f02a    77 st	r5,reg5 */
0x0000f02b, 0x0660f678, /* 0660f678 //C 0f02b    78 st	r6,reg6 */
0x0000f02c, 0x0670f679, /* 0670f679 //C 0f02c    79 st	r7,reg7 */
0x0000f02d, 0x0680f67a, /* 0680f67a //C 0f02d    80 st	r8,reg8 */
0x0000f02e, 0x0690f67b, /* 0690f67b //C 0f02e    81 st	r9,reg9 */
0x0000f02f, 0x06a0f67c, /* 06a0f67c //C 0f02f    82 st	r10,reg10 */
0x0000f030, 0x06b0f67d, /* 06b0f67d //C 0f030    83 st	r11,reg11 */
0x0000f031, 0x06c0f67e, /* 06c0f67e //C 0f031    84 st	r12,reg12 */
0x0000f032, 0x06d0f67f, /* 06d0f67f //C 0f032    85 st	r13,reg13 */
0x0000f033, 0x06e0f680, /* 06e0f680 //C 0f033    86 st	r14,reg14 */
0x0000f034, 0x06e0f681, /* 06e0f681 //C 0f034    87 st	r14,reg15 */
0x0000f035, 0x020e0000, /* 020e0000 //C 0f035    88 getf	r0 */
0x0000f036, 0x0600f682, /* 0600f682 //C 0f036    89 st	r0,regf */
0x0000f037, 0x01020001, /* 01020001 //C 0f037    90 mvzl	r0,1 */
0x0000f038, 0x0600f670, /* 0600f670 //C 0f038    91 st	r0,called */
0x0000f039, 0x01f2f046, /* 01f2f046 //C 0f039    92 jmp	common_start */
0x0000f03a, 0x01f2f046, /* 01f2f046 //C 0f03a    94 jmp	common_start */
0x0000f03b, 0x01f2f03c, /* 01f2f03c //C 0f03b    96 jmp	cold_start */
0x0000f03c, 0x01020000, /* 01020000 //C 0f03c    98 mvzl	r0,0 */
0x0000f03d, 0x0600f670, /* 0600f670 //C 0f03d    99 st	r0,called */
0x0000f03e, 0x0102f03b, /* 0102f03b //C 0f03e   100 mvzl	r0,def_zero	; restore jmp to monitor at zero */
0x0000f03f, 0x0e000000, /* 0e000000 //C 0f03f   101 ld	r0,r0 */
0x0000f040, 0x06000000, /* 06000000 //C 0f040   102 st	r0,0 */
0x0000f041, 0x01020000, /* 01020000 //C 0f041   103 mvzl	r0,0		; def values of some regs */
0x0000f042, 0x0600f682, /* 0600f682 //C 0f042   104 st	r0,regf		; FALGS= 0 */
0x0000f043, 0x0102f7ff, /* 0102f7ff //C 0f043   105 mvzl	r0,0xf7ff	; R13= 0xf7ff */
0x0000f044, 0x0600f67f, /* 0600f67f //C 0f044   106 st	r0,reg13 */
0x0000f045, 0x01f2f046, /* 01f2f046 //C 0f045   107 jmp	common_start */
0x0000f046, 0x01d2f8d5, /* 01d2f8d5 //C 0f046   110 mvzl	sp,stack_end */
0x0000f047, 0x01020001, /* 01020001 //C 0f047   111 mvzl	r0,1 */
0x0000f048, 0x0600f66f, /* 0600f66f //C 0f048   112 st	r0,echo */
0x0000f049, 0x01020000, /* 01020000 //C 0f049   113 mvzl	r0,0 */
0x0000f04a, 0x020f0000, /* 020f0000 //C 0f04a   114 setf	r0 */
0x0000f04b, 0x0710ff41, /* 0710ff41 //C 0f04b   119 ld	r1,UART_CTRL	; check if transmit is enabled */
0x0000f04c, 0x011c0002, /* 011c0002 //C 0f04c   120 test	r1,2 */
0x0000f04d, 0x11f2f051, /* 11f2f051 //C 0f04d   121 jz	tr_is_off */
0x0000f04e, 0x0710ff43, /* 0710ff43 //C 0f04e   123 ld	r1,UART_TSTAT	; if transmit is ongoing */
0x0000f04f, 0x011c0001, /* 011c0001 //C 0f04f   124 test	r1,1		; wait it to finish */
0x0000f050, 0x11f2f04e, /* 11f2f04e //C 0f050   125 jz	wait_uart_tr */
0x0000f051, 0x01120003, /* 01120003 //C 0f051   127 mvzl	r1,3		; turn on rx and tx */
0x0000f052, 0x0610ff41, /* 0610ff41 //C 0f052   128 st	r1,UART_CTRL */
0x0000f053, 0x0102000a, /* 0102000a //C 0f053   131 mvzl	r0,LF */
0x0000f054, 0x0400f496, /* 0400f496 //C 0f054   132 call	putchar */
0x0000f055, 0x06020100, /* 06020100 //C 0f055   133 rds	r0,sver */
0x0000f056, 0x01120001, /* 01120001 //C 0f056   134 mvzl	r1,version_main */
0x0000f057, 0x01220001, /* 01220001 //C 0f057   135 mvzl	r2,version_sub */
0x0000f058, 0x0631c002, /* 0631c002 //C 0f058   136 getbz	r3,r0,2 */
0x0000f059, 0x0641c001, /* 0641c001 //C 0f059   137 getbz	r4,r0,1 */
0x0000f05a, 0x0651c000, /* 0651c000 //C 0f05a   138 getbz	r5,r0,0 */
0x0000f05b, 0x0102f683, /* 0102f683 //C 0f05b   139 mvzl	r0,msg_start */
0x0000f05c, 0x0400f577, /* 0400f577 //C 0f05c   140 call	printf */
0x0000f05d, 0x0700f670, /* 0700f670 //C 0f05d   142 ld	r0,called */
0x0000f05e, 0x020b0000, /* 020b0000 //C 0f05e   143 sz	r0 */
0x0000f05f, 0x11f2f069, /* 11f2f069 //C 0f05f   144 jz	no_called_from */
0x0000f060, 0x0102000a, /* 0102000a //C 0f060   145 mvzl	r0,LF */
0x0000f061, 0x0400f496, /* 0400f496 //C 0f061   146 call	putchar */
0x0000f062, 0x0102f6a4, /* 0102f6a4 //C 0f062   147 mvzl	r0,msg_stopat */
0x0000f063, 0x0400f49d, /* 0400f49d //C 0f063   148 call	prints */
0x0000f064, 0x0700f680, /* 0700f680 //C 0f064   149 ld	r0,reg14 */
0x0000f065, 0x01120004, /* 01120004 //C 0f065   150 mvzl	r1,4 */
0x0000f066, 0x0400f4dd, /* 0400f4dd //C 0f066   151 call	print_vhex */
0x0000f067, 0x0102000a, /* 0102000a //C 0f067   152 mvzl	r0,LF */
0x0000f068, 0x0400f496, /* 0400f496 //C 0f068   153 call	putchar */
0x0000f069, 0x0400f06b, /* 0400f06b //C 0f069   157 call	setup_line */
0x0000f06a, 0x01f2f074, /* 01f2f074 //C 0f06a   161 jmp	main */
0x0000f06b, 0x0ded0000, /* 0ded0000 //C 0f06b   167 push	lr */
0x0000f06c, 0x01120000, /* 01120000 //C 0f06c   168 mvzl	r1,0		; lptr= 0 */
0x0000f06d, 0x0610f668, /* 0610f668 //C 0f06d   169 st	r1,line_ptr */
0x0000f06e, 0x0610f604, /* 0610f604 //C 0f06e   170 st	r1,line		; line[0]= 0 */
0x0000f06f, 0x01120000, /* 01120000 //C 0f06f   171 mvzl	r1,0		; at_eol= 0 */
0x0000f070, 0x0610f669, /* 0610f669 //C 0f070   172 st	r1,at_eol */
0x0000f071, 0x0102f6b2, /* 0102f6b2 //C 0f071   174 mvzl	r0,prompt */
0x0000f072, 0x0400f49d, /* 0400f49d //C 0f072   175 call	prints */
0x0000f073, 0x0ffd0000, /* 0ffd0000 //C 0f073   176 pop	pc */
0x0000f074, 0x0400f487, /* 0400f487 //C 0f074   183 call	check_uart */
0x0000f075, 0x41f2f07b, /* 41f2f07b //C 0f075   184 C0 jmp	no_input */
0x0000f076, 0x0400f48f, /* 0400f48f //C 0f076   186 call	read */
0x0000f077, 0x0400f07c, /* 0400f07c //C 0f077   187 call	proc_input */
0x0000f078, 0x41f2f07b, /* 41f2f07b //C 0f078   188 C0 jmp	no_line */
0x0000f079, 0x0400f0ad, /* 0400f0ad //C 0f079   191 call	proc_line */
0x0000f07a, 0x0400f06b, /* 0400f06b //C 0f07a   192 call	setup_line */
0x0000f07b, 0x01f2f074, /* 01f2f074 //C 0f07b   195 jmp	main */
0x0000f07c, 0x0ded0000, /* 0ded0000 //C 0f07c   201 push	lr */
0x0000f07d, 0x0108000a, /* 0108000a //C 0f07d   202 cmp	r0,LF */
0x0000f07e, 0x11f2f0a7, /* 11f2f0a7 //C 0f07e   203 EQ jmp	got_eol */
0x0000f07f, 0x0108000d, /* 0108000d //C 0f07f   204 cmp	r0,CR */
0x0000f080, 0x11f2f0a7, /* 11f2f0a7 //C 0f080   205 EQ jmp	got_eol */
0x0000f081, 0x01080008, /* 01080008 //C 0f081   206 cmp	r0,8 */
0x0000f082, 0x11f2f086, /* 11f2f086 //C 0f082   207 jz	got_BS */
0x0000f083, 0x0108007f, /* 0108007f //C 0f083   208 cmp	r0,127 */
0x0000f084, 0x11f2f086, /* 11f2f086 //C 0f084   209 jz	got_DEL */
0x0000f085, 0x01f2f092, /* 01f2f092 //C 0f085   210 jmp	got_char */
0x0000f086, 0x0700f668, /* 0700f668 //C 0f086   213 ld	r0,line_ptr */
0x0000f087, 0x020b0000, /* 020b0000 //C 0f087   214 sz	r0 */
0x0000f088, 0x11f2f090, /* 11f2f090 //C 0f088   215 jz	got_done */
0x0000f089, 0x01060001, /* 01060001 //C 0f089   216 sub	r0,1 */
0x0000f08a, 0x0600f668, /* 0600f668 //C 0f08a   217 st	r0,line_ptr */
0x0000f08b, 0x0112f604, /* 0112f604 //C 0f08b   218 mvzl	r1,line */
0x0000f08c, 0x01220000, /* 01220000 //C 0f08c   219 mvzl	r2,0 */
0x0000f08d, 0x08210000, /* 08210000 //C 0f08d   220 st	r2,r1,r0 */
0x0000f08e, 0x0102f6ae, /* 0102f6ae //C 0f08e   221 mvzl	r0,msg_BS */
0x0000f08f, 0x0400f49d, /* 0400f49d //C 0f08f   222 call	prints */
0x0000f090, 0x020d0000, /* 020d0000 //C 0f090   224 clc */
0x0000f091, 0x01f2f0ac, /* 01f2f0ac //C 0f091   225 jmp	proc_input_ret */
0x0000f092, 0x0108001f, /* 0108001f //C 0f092   227 cmp	r0,31		; refuse control chars */
0x0000f093, 0xa1f2f0ac, /* a1f2f0ac //C 0f093   228 LS jmp	proc_input_ret */
0x0000f094, 0x0108007e, /* 0108007e //C 0f094   229 cmp	r0,126		; refuse graph chars */
0x0000f095, 0x91f2f0ac, /* 91f2f0ac //C 0f095   230 HI jmp	proc_input_ret */
0x0000f096, 0x01220000, /* 01220000 //C 0f096   231 mvzl	r2,0		; at_aol= 0 */
0x0000f097, 0x0620f669, /* 0620f669 //C 0f097   232 st	r2,at_eol */
0x0000f098, 0x0112f668, /* 0112f668 //C 0f098   233 mvzl	r1,line_ptr	; line[line_ptr]= char */
0x0000f099, 0x0e310000, /* 0e310000 //C 0f099   234 ld	r3,r1 */
0x0000f09a, 0x0122f604, /* 0122f604 //C 0f09a   235 mvzl	r2,line */
0x0000f09b, 0x08020300, /* 08020300 //C 0f09b   236 st	r0,r2,r3 */
0x0000f09c, 0x013a0001, /* 013a0001 //C 0f09c   238 plus	r3,1		; line_ptr++ */
0x0000f09d, 0x0c310000, /* 0c310000 //C 0f09d   239 st	r3,r1 */
0x0000f09e, 0x01420000, /* 01420000 //C 0f09e   240 mvzl	r4,0 */
0x0000f09f, 0x09438200, /* 09438200 //C 0f09f   241 st	r4,r3+,r2	; line[line_ptr]= 0 */
0x0000f0a0, 0x08430200, /* 08430200 //C 0f0a0   242 st	r4,r3,r2	; double 0 at end, needed by tokenizer */
0x0000f0a1, 0x0142f66f, /* 0142f66f //C 0f0a1   243 mvzl	r4,echo		; check if echo is turned on */
0x0000f0a2, 0x0e440000, /* 0e440000 //C 0f0a2   244 ld	r4,r4 */
0x0000f0a3, 0x024b0000, /* 024b0000 //C 0f0a3   245 sz	r4 */
0x0000f0a4, 0x2400f496, /* 2400f496 //C 0f0a4   246 NZ call	putchar		; echo */
0x0000f0a5, 0x020d0000, /* 020d0000 //C 0f0a5   247 clc */
0x0000f0a6, 0x01f2f0ac, /* 01f2f0ac //C 0f0a6   248 jmp	proc_input_ret */
0x0000f0a7, 0x0112f669, /* 0112f669 //C 0f0a7   250 mvzl	r1,at_eol */
0x0000f0a8, 0x0e110000, /* 0e110000 //C 0f0a8   251 ld	r1,r1 */
0x0000f0a9, 0x021b0000, /* 021b0000 //C 0f0a9   252 sz	r1		; Z=0 at eol -> skip, not ready */
0x0000f0aa, 0x220d0000, /* 220d0000 //C 0f0aa   253 Z0 clc */
0x0000f0ab, 0x120c0000, /* 120c0000 //C 0f0ab   254 Z1 sec */
0x0000f0ac, 0x0ffd0000, /* 0ffd0000 //C 0f0ac   256 pop	pc */
0x0000f0ad, 0x0ded0000, /* 0ded0000 //C 0f0ad   265 push	lr */
0x0000f0ae, 0x0102000a, /* 0102000a //C 0f0ae   267 mvzl	r0,LF */
0x0000f0af, 0x0400f496, /* 0400f496 //C 0f0af   268 call	putchar */
0x0000f0b0, 0x0700f604, /* 0700f604 //C 0f0b0   270 ld	r0,line */
0x0000f0b1, 0x020b0000, /* 020b0000 //C 0f0b1   271 sz	r0 */
0x0000f0b2, 0x11f2f0ba, /* 11f2f0ba //C 0f0b2   272 jz	proc_line_ret */
0x0000f0b3, 0x0400f0d6, /* 0400f0d6 //C 0f0b3   280 call	tokenize */
0x0000f0b4, 0x0400f0f6, /* 0400f0f6 //C 0f0b4   291 call	find_cmd */
0x0000f0b5, 0x41f2f0b8, /* 41f2f0b8 //C 0f0b5   292 C0 jmp	cmd_not_found */
0x0000f0b6, 0x05000000, /* 05000000 //C 0f0b6   295 call	r0,0 */
0x0000f0b7, 0x01f2f0ba, /* 01f2f0ba //C 0f0b7   297 jmp	proc_line_ret */
0x0000f0b8, 0x0102f0c2, /* 0102f0c2 //C 0f0b8   299 mvzl	r0,snotfound */
0x0000f0b9, 0x0400f4d6, /* 0400f4d6 //C 0f0b9   300 call	printsnl */
0x0000f0ba, 0x01120001, /* 01120001 //C 0f0ba   303 mvzl	r1,1		; at_eol= 1 */
0x0000f0bb, 0x0610f669, /* 0610f669 //C 0f0bb   304 st	r1,at_eol */
0x0000f0bc, 0x0ffd0000, /* 0ffd0000 //C 0f0bc   305 pop	pc */
0x0000f0bd, 0x00000047, /* 00000047 //C 0f0bd   307 db	71 */
0x0000f0be, 0x0000006f, /* 0000006f //C 0f0be   307 db	111 */
0x0000f0bf, 0x00000074, /* 00000074 //C 0f0bf   307 db	116 */
0x0000f0c0, 0x0000003a, /* 0000003a //C 0f0c0   307 db	58 */
0x0000f0c1, 0x00000000, /* 00000000 //C 0f0c1   307 db */
0x0000f0c2, 0x00000055, /* 00000055 //C 0f0c2   308 db	85 */
0x0000f0c3, 0x0000006e, /* 0000006e //C 0f0c3   308 db	110 */
0x0000f0c4, 0x0000006b, /* 0000006b //C 0f0c4   308 db	107 */
0x0000f0c5, 0x0000006e, /* 0000006e //C 0f0c5   308 db	110 */
0x0000f0c6, 0x0000006f, /* 0000006f //C 0f0c6   308 db	111 */
0x0000f0c7, 0x00000077, /* 00000077 //C 0f0c7   308 db	119 */
0x0000f0c8, 0x0000006e, /* 0000006e //C 0f0c8   308 db	110 */
0x0000f0c9, 0x00000020, /* 00000020 //C 0f0c9   308 db	32 */
0x0000f0ca, 0x00000063, /* 00000063 //C 0f0ca   308 db	99 */
0x0000f0cb, 0x0000006f, /* 0000006f //C 0f0cb   308 db	111 */
0x0000f0cc, 0x0000006d, /* 0000006d //C 0f0cc   308 db	109 */
0x0000f0cd, 0x0000006d, /* 0000006d //C 0f0cd   308 db	109 */
0x0000f0ce, 0x00000061, /* 00000061 //C 0f0ce   308 db	97 */
0x0000f0cf, 0x0000006e, /* 0000006e //C 0f0cf   308 db	110 */
0x0000f0d0, 0x00000064, /* 00000064 //C 0f0d0   308 db	100 */
0x0000f0d1, 0x00000000, /* 00000000 //C 0f0d1   308 db */
0x0000f0d2, 0x0ded0000, /* 0ded0000 //C 0f0d2   316 push	lr */
0x0000f0d3, 0x0112f6b4, /* 0112f6b4 //C 0f0d3   317 mvzl	r1,delimiters */
0x0000f0d4, 0x0400f3a4, /* 0400f3a4 //C 0f0d4   318 call	strchr */
0x0000f0d5, 0x0ffd0000, /* 0ffd0000 //C 0f0d5   319 pop	pc */
0x0000f0d6, 0x0ded0000, /* 0ded0000 //C 0f0d6   328 push	lr */
0x0000f0d7, 0x0142f66a, /* 0142f66a //C 0f0d7   329 mvzl	r4,words	; array of result */
0x0000f0d8, 0x0152f604, /* 0152f604 //C 0f0d8   330 mvzl	r5,line		; address of next char */
0x0000f0d9, 0x01620000, /* 01620000 //C 0f0d9   331 mvzl	r6,0		; nuof words found */
0x0000f0da, 0x01720000, /* 01720000 //C 0f0da   332 mvzl	r7,0		; bool in_word */
0x0000f0db, 0x0e050000, /* 0e050000 //C 0f0db   334 ld	r0,r5		; pick a char */
0x0000f0dc, 0x020b0000, /* 020b0000 //C 0f0dc   335 sz	r0		; check end */
0x0000f0dd, 0x11f2f0e7, /* 11f2f0e7 //C 0f0dd   336 jz	tok_delimiter	; found end, pretend delim */
0x0000f0de, 0x0400f0d2, /* 0400f0d2 //C 0f0de   338 call	is_delimiter */
0x0000f0df, 0x31f2f0e7, /* 31f2f0e7 //C 0f0df   339 C1 jmp	tok_delimiter */
0x0000f0e0, 0x027b0000, /* 027b0000 //C 0f0e0   341 sz	r7 */
0x0000f0e1, 0x21f2f0ec, /* 21f2f0ec //C 0f0e1   342 jnz	tok_next	; still inside word */
0x0000f0e2, 0x01720001, /* 01720001 //C 0f0e2   344 mvzl	r7,1		; in_word=true */
0x0000f0e3, 0x09568400, /* 09568400 //C 0f0e3   345 st	r5,r6+,r4	; record word address */
0x0000f0e4, 0x01680005, /* 01680005 //C 0f0e4   346 cmp	r6,MAX_WORDS	; If no more space */
0x0000f0e5, 0x11f2f0f0, /* 11f2f0f0 //C 0f0e5   347 EQ jmp	tok_ret		; then return */
0x0000f0e6, 0x01f2f0ec, /* 01f2f0ec //C 0f0e6   348 jmp	tok_next */
0x0000f0e7, 0x027b0000, /* 027b0000 //C 0f0e7   350 sz	r7 */
0x0000f0e8, 0x11f2f0ec, /* 11f2f0ec //C 0f0e8   351 jz	tok_next	; still between words */
0x0000f0e9, 0x01720000, /* 01720000 //C 0f0e9   353 mvzl	r7,0		; in_word=false */
0x0000f0ea, 0x01120000, /* 01120000 //C 0f0ea   354 mvzl	r1,0		; put a 0 at the end of word */
0x0000f0eb, 0x08150100, /* 08150100 //C 0f0eb   355 st	r1,r5,r1 */
0x0000f0ec, 0x020b0000, /* 020b0000 //C 0f0ec   357 sz	r0		; check EOL */
0x0000f0ed, 0x11f2f0f0, /* 11f2f0f0 //C 0f0ed   358 jz	tok_ret		; jump out if char==0 */
0x0000f0ee, 0x01540001, /* 01540001 //C 0f0ee   359 add	r5,1 */
0x0000f0ef, 0x01f2f0db, /* 01f2f0db //C 0f0ef   360 jmp	tok_cycle */
0x0000f0f0, 0x01120000, /* 01120000 //C 0f0f0   362 mvzl	r1,0 */
0x0000f0f1, 0x01680005, /* 01680005 //C 0f0f1   363 cmp	r6,MAX_WORDS */
0x0000f0f2, 0x11f2f0f5, /* 11f2f0f5 //C 0f0f2   364 jz	tok_end */
0x0000f0f3, 0x09168400, /* 09168400 //C 0f0f3   365 st	r1,r6+,r4 */
0x0000f0f4, 0x01f2f0f0, /* 01f2f0f0 //C 0f0f4   366 jmp	tok_ret */
0x0000f0f5, 0x0ffd0000, /* 0ffd0000 //C 0f0f5   368 pop	pc */
0x0000f0f6, 0x0ded0000, /* 0ded0000 //C 0f0f6   377 push	lr */
0x0000f0f7, 0x0d1d0000, /* 0d1d0000 //C 0f0f7   378 push	r1 */
0x0000f0f8, 0x0d2d0000, /* 0d2d0000 //C 0f0f8   379 push	r2 */
0x0000f0f9, 0x0d3d0000, /* 0d3d0000 //C 0f0f9   380 push	r3 */
0x0000f0fa, 0x0dad0000, /* 0dad0000 //C 0f0fa   381 push	r10 */
0x0000f0fb, 0x0700f66a, /* 0700f66a //C 0f0fb   382 ld	r0,words	; R0= 1st word of command */
0x0000f0fc, 0x020b0000, /* 020b0000 //C 0f0fc   383 sz	r0 */
0x0000f0fd, 0x11f2f12d, /* 11f2f12d //C 0f0fd   384 jz	find_cmd_false */
0x0000f0fe, 0x0e100000, /* 0e100000 //C 0f0fe   387 ld	r1,r0		; 1st char of word1 */
0x0000f0ff, 0x0e200001, /* 0e200001 //C 0f0ff   388 ld	r2,r0,1		; 2nd char */
0x0000f100, 0x0e300002, /* 0e300002 //C 0f100   389 ld	r3,r0,2		; 3rd char */
0x0000f101, 0x011fffdf, /* 011fffdf //C 0f101   390 and	r1,0xffdf	; upcase 1st char */
0x0000f102, 0x01180052, /* 01180052 //C 0f102   391 cmp	r1,'R' */
0x0000f103, 0x21f2f119, /* 21f2f119 //C 0f103   392 jnz	find_not_rx */
0x0000f104, 0x0128002f, /* 0128002f //C 0f104   393 cmp	r2,'/'		; '0'-1 */
0x0000f105, 0xa1f2f119, /* a1f2f119 //C 0f105   394 LS jmp	find_not_rx */
0x0000f106, 0x01280039, /* 01280039 //C 0f106   395 cmp	r2,'9' */
0x0000f107, 0x91f2f119, /* 91f2f119 //C 0f107   396 HI jmp	find_not_rx */
0x0000f108, 0x023b0000, /* 023b0000 //C 0f108   397 sz	r3 */
0x0000f109, 0x11f2f114, /* 11f2f114 //C 0f109   398 jz	find_rx_09 */
0x0000f10a, 0x01280031, /* 01280031 //C 0f10a   400 cmp	r2,'1'		; first char must be '1' */
0x0000f10b, 0x21f2f119, /* 21f2f119 //C 0f10b   401 jnz	find_not_rx */
0x0000f10c, 0x0138002f, /* 0138002f //C 0f10c   402 cmp	r3,'/'		; '0'-1 */
0x0000f10d, 0xa1f2f119, /* a1f2f119 //C 0f10d   403 LS jmp	find_not_rx */
0x0000f10e, 0x01380035, /* 01380035 //C 0f10e   404 cmp	r3,'5' */
0x0000f10f, 0x91f2f119, /* 91f2f119 //C 0f10f   405 HI jmp	find_not_rx */
0x0000f110, 0x01360030, /* 01360030 //C 0f110   406 sub	r3,'0' */
0x0000f111, 0x0134000a, /* 0134000a //C 0f111   407 add	r3,10 */
0x0000f112, 0x0630f671, /* 0630f671 //C 0f112   408 st	r3,nuof_reg */
0x0000f113, 0x01f2f116, /* 01f2f116 //C 0f113   409 jmp	find_rx */
0x0000f114, 0x01260030, /* 01260030 //C 0f114   411 sub	r2,'0' */
0x0000f115, 0x0620f671, /* 0620f671 //C 0f115   412 st	r2,nuof_reg */
0x0000f116, 0x0102f363, /* 0102f363 //C 0f116   414 mvzl	r0,cmd_rx */
0x0000f117, 0x020c0000, /* 020c0000 //C 0f117   415 sec */
0x0000f118, 0x01f2f138, /* 01f2f138 //C 0f118   416 jmp	find_cmd_ret */
0x0000f119, 0x01a2f6d0, /* 01a2f6d0 //C 0f119   419 mvzl	r10,commands */
0x0000f11a, 0x0e2a0000, /* 0e2a0000 //C 0f11a   421 ld	r2,r10		; R2= cmd addr */
0x0000f11b, 0x022b0000, /* 022b0000 //C 0f11b   422 sz	r2 */
0x0000f11c, 0x11f2f12d, /* 11f2f12d //C 0f11c   423 jz	find_cmd_false */
0x0000f11d, 0x01a40001, /* 01a40001 //C 0f11d   424 add	r10,1 */
0x0000f11e, 0x00100a00, /* 00100a00 //C 0f11e   425 mov	r1,r10		; R1= cmd string */
0x0000f11f, 0x01a40001, /* 01a40001 //C 0f11f   428 add	r10,1 */
0x0000f120, 0x0e3a0000, /* 0e3a0000 //C 0f120   429 ld	r3,r10 */
0x0000f121, 0x023b0000, /* 023b0000 //C 0f121   430 sz	r3 */
0x0000f122, 0x21f2f11f, /* 21f2f11f //C 0f122   431 jnz	find_cmd_fw */
0x0000f123, 0x01a40001, /* 01a40001 //C 0f123   432 add	r10,1 */
0x0000f124, 0x0400f416, /* 0400f416 //C 0f124   434 call	streq */
0x0000f125, 0x41f2f11a, /* 41f2f11a //C 0f125   435 C0 jmp	find_cmd_cyc */
0x0000f126, 0x00000200, /* 00000200 //C 0f126   437 mov	r0,r2 */
0x0000f127, 0x020c0000, /* 020c0000 //C 0f127   438 sec */
0x0000f128, 0x01f2f138, /* 01f2f138 //C 0f128   439 jmp	find_cmd_ret */
0x0000f129, 0x0000002f, /* 0000002f //C 0f129   440 db	47 */
0x0000f12a, 0x0000002f, /* 0000002f //C 0f12a   440 db	47 */
0x0000f12b, 0x00000043, /* 00000043 //C 0f12b   440 db	67 */
0x0000f12c, 0x00000000, /* 00000000 //C 0f12c   440 db */
0x0000f12d, 0x01040001, /* 01040001 //C 0f12d   442 add	r0,1		; check second word */
0x0000f12e, 0x0e100000, /* 0e100000 //C 0f12e   443 ld	r1,r0		; for //C command */
0x0000f12f, 0x021b0000, /* 021b0000 //C 0f12f   444 sz	r1 */
0x0000f130, 0x11f2f136, /* 11f2f136 //C 0f130   445 jz	find_cmd_very_false */
0x0000f131, 0x0102f129, /* 0102f129 //C 0f131   446 mvzl	r0,c_cmd_name */
0x0000f132, 0x0400f416, /* 0400f416 //C 0f132   447 call	streq */
0x0000f133, 0x21f2f136, /* 21f2f136 //C 0f133   448 jnz	find_cmd_very_false */
0x0000f134, 0x0122f1eb, /* 0122f1eb //C 0f134   449 mvzl	r2,cmd_c */
0x0000f135, 0x01f2f126, /* 01f2f126 //C 0f135   450 jmp	find_cmd_true */
0x0000f136, 0x020d0000, /* 020d0000 //C 0f136   452 clc */
0x0000f137, 0x01020000, /* 01020000 //C 0f137   453 mvzl	r0,0 */
0x0000f138, 0x0fad0000, /* 0fad0000 //C 0f138   455 pop	r10 */
0x0000f139, 0x0f3d0000, /* 0f3d0000 //C 0f139   456 pop	r3 */
0x0000f13a, 0x0f2d0000, /* 0f2d0000 //C 0f13a   457 pop	r2 */
0x0000f13b, 0x0f1d0000, /* 0f1d0000 //C 0f13b   458 pop	r1 */
0x0000f13c, 0x0ffd0000, /* 0ffd0000 //C 0f13c   459 pop	pc */
0x0000f13d, 0x0ded0000, /* 0ded0000 //C 0f13d   464 push	lr */
0x0000f13e, 0x0122f66a, /* 0122f66a //C 0f13e   465 mvzl	r2,words */
0x0000f13f, 0x01020000, /* 01020000 //C 0f13f   466 mvzl	r0,0 */
0x0000f140, 0x0e420001, /* 0e420001 //C 0f140   468 ld	r4,r2,1		; addr */
0x0000f141, 0x0e520002, /* 0e520002 //C 0f141   469 ld	r5,r2,2		; value */
0x0000f142, 0x024b0000, /* 024b0000 //C 0f142   470 sz 	r4 */
0x0000f143, 0x11f2f169, /* 11f2f169 //C 0f143   471 jz	m_ret */
0x0000f144, 0x00000400, /* 00000400 //C 0f144   473 mov	r0,r4 */
0x0000f145, 0x0400f459, /* 0400f459 //C 0f145   474 call	htoi */
0x0000f146, 0x00400100, /* 00400100 //C 0f146   475 mov	r4,r1 */
0x0000f147, 0x31f2f14b, /* 31f2f14b //C 0f147   476 C1 jmp	m_addr_ok */
0x0000f148, 0x0102f16a, /* 0102f16a //C 0f148   477 mvzl	r0,m_err_addr */
0x0000f149, 0x0400f4d6, /* 0400f4d6 //C 0f149   478 call	printsnl */
0x0000f14a, 0x01f2f169, /* 01f2f169 //C 0f14a   479 jmp	m_ret */
0x0000f14b, 0x025b0000, /* 025b0000 //C 0f14b   481 sz	r5 */
0x0000f14c, 0x11f2f15f, /* 11f2f15f //C 0f14c   482 jz	m_read */
0x0000f14d, 0x0132f000, /* 0132f000 //C 0f14d   484 mvzl	r3,the_begin */
0x0000f14e, 0x00380400, /* 00380400 //C 0f14e   485 cmp	r3,r4 */
0x0000f14f, 0x91f2f157, /* 91f2f157 //C 0f14f   486 HI jmp	m_addrv_ok */
0x0000f150, 0x0132f8d6, /* 0132f8d6 //C 0f150   487 mvzl	r3,the_end */
0x0000f151, 0x00380400, /* 00380400 //C 0f151   488 cmp	r3,r4 */
0x0000f152, 0x91f2f154, /* 91f2f154 //C 0f152   489 HI jmp	m_addrv_nok */
0x0000f153, 0x01f2f157, /* 01f2f157 //C 0f153   490 jmp	m_addrv_ok */
0x0000f154, 0x0102f178, /* 0102f178 //C 0f154   498 mvzl	r0,m_err_addrv */
0x0000f155, 0x0400f4d6, /* 0400f4d6 //C 0f155   499 call	printsnl */
0x0000f156, 0x01f2f169, /* 01f2f169 //C 0f156   500 jmp	m_ret */
0x0000f157, 0x00000500, /* 00000500 //C 0f157   502 mov	r0,r5 */
0x0000f158, 0x0400f459, /* 0400f459 //C 0f158   503 call	htoi */
0x0000f159, 0x00500100, /* 00500100 //C 0f159   504 mov	r5,r1 */
0x0000f15a, 0x31f2f15e, /* 31f2f15e //C 0f15a   505 C1 jmp	m_value_ok */
0x0000f15b, 0x0102f18a, /* 0102f18a //C 0f15b   506 mvzl	r0,m_err_value */
0x0000f15c, 0x0400f4d6, /* 0400f4d6 //C 0f15c   507 call	printsnl */
0x0000f15d, 0x01f2f169, /* 01f2f169 //C 0f15d   508 jmp	m_ret */
0x0000f15e, 0x0c540000, /* 0c540000 //C 0f15e   510 st	r5,r4 */
0x0000f15f, 0x00000400, /* 00000400 //C 0f15f   513 mov	r0,r4 */
0x0000f160, 0x01120004, /* 01120004 //C 0f160   514 mvzl	r1,4 */
0x0000f161, 0x0400f4dd, /* 0400f4dd //C 0f161   515 call	print_vhex */
0x0000f162, 0x01020020, /* 01020020 //C 0f162   516 mvzl	r0,0x20 */
0x0000f163, 0x0400f496, /* 0400f496 //C 0f163   517 call	putchar */
0x0000f164, 0x0e040000, /* 0e040000 //C 0f164   518 ld	r0,r4 */
0x0000f165, 0x01120004, /* 01120004 //C 0f165   519 mvzl	r1,4 */
0x0000f166, 0x0400f4dd, /* 0400f4dd //C 0f166   520 call	print_vhex */
0x0000f167, 0x0102000a, /* 0102000a //C 0f167   521 mvzl	r0,LF */
0x0000f168, 0x0400f496, /* 0400f496 //C 0f168   522 call	putchar */
0x0000f169, 0x0ffd0000, /* 0ffd0000 //C 0f169   524 pop	pc */
0x0000f16a, 0x00000041, /* 00000041 //C 0f16a   526 db	65 */
0x0000f16b, 0x00000064, /* 00000064 //C 0f16b   526 db	100 */
0x0000f16c, 0x00000064, /* 00000064 //C 0f16c   526 db	100 */
0x0000f16d, 0x00000072, /* 00000072 //C 0f16d   526 db	114 */
0x0000f16e, 0x00000065, /* 00000065 //C 0f16e   526 db	101 */
0x0000f16f, 0x00000073, /* 00000073 //C 0f16f   526 db	115 */
0x0000f170, 0x00000073, /* 00000073 //C 0f170   526 db	115 */
0x0000f171, 0x00000020, /* 00000020 //C 0f171   526 db	32 */
0x0000f172, 0x00000065, /* 00000065 //C 0f172   526 db	101 */
0x0000f173, 0x00000072, /* 00000072 //C 0f173   526 db	114 */
0x0000f174, 0x00000072, /* 00000072 //C 0f174   526 db	114 */
0x0000f175, 0x0000006f, /* 0000006f //C 0f175   526 db	111 */
0x0000f176, 0x00000072, /* 00000072 //C 0f176   526 db	114 */
0x0000f177, 0x00000000, /* 00000000 //C 0f177   526 db */
0x0000f178, 0x0000004d, /* 0000004d //C 0f178   527 db	77 */
0x0000f179, 0x0000006f, /* 0000006f //C 0f179   527 db	111 */
0x0000f17a, 0x0000006e, /* 0000006e //C 0f17a   527 db	110 */
0x0000f17b, 0x00000069, /* 00000069 //C 0f17b   527 db	105 */
0x0000f17c, 0x00000074, /* 00000074 //C 0f17c   527 db	116 */
0x0000f17d, 0x0000006f, /* 0000006f //C 0f17d   527 db	111 */
0x0000f17e, 0x00000072, /* 00000072 //C 0f17e   527 db	114 */
0x0000f17f, 0x00000027, /* 00000027 //C 0f17f   527 db	39 */
0x0000f180, 0x00000073, /* 00000073 //C 0f180   527 db	115 */
0x0000f181, 0x00000020, /* 00000020 //C 0f181   527 db	32 */
0x0000f182, 0x00000061, /* 00000061 //C 0f182   527 db	97 */
0x0000f183, 0x00000064, /* 00000064 //C 0f183   527 db	100 */
0x0000f184, 0x00000064, /* 00000064 //C 0f184   527 db	100 */
0x0000f185, 0x00000072, /* 00000072 //C 0f185   527 db	114 */
0x0000f186, 0x00000065, /* 00000065 //C 0f186   527 db	101 */
0x0000f187, 0x00000073, /* 00000073 //C 0f187   527 db	115 */
0x0000f188, 0x00000073, /* 00000073 //C 0f188   527 db	115 */
0x0000f189, 0x00000000, /* 00000000 //C 0f189   527 db */
0x0000f18a, 0x00000056, /* 00000056 //C 0f18a   528 db	86 */
0x0000f18b, 0x00000061, /* 00000061 //C 0f18b   528 db	97 */
0x0000f18c, 0x0000006c, /* 0000006c //C 0f18c   528 db	108 */
0x0000f18d, 0x00000075, /* 00000075 //C 0f18d   528 db	117 */
0x0000f18e, 0x00000065, /* 00000065 //C 0f18e   528 db	101 */
0x0000f18f, 0x00000020, /* 00000020 //C 0f18f   528 db	32 */
0x0000f190, 0x00000065, /* 00000065 //C 0f190   528 db	101 */
0x0000f191, 0x00000072, /* 00000072 //C 0f191   528 db	114 */
0x0000f192, 0x00000072, /* 00000072 //C 0f192   528 db	114 */
0x0000f193, 0x0000006f, /* 0000006f //C 0f193   528 db	111 */
0x0000f194, 0x00000072, /* 00000072 //C 0f194   528 db	114 */
0x0000f195, 0x00000000, /* 00000000 //C 0f195   528 db */
0x0000f196, 0x0ded0000, /* 0ded0000 //C 0f196   533 push	lr */
0x0000f197, 0x0122f66a, /* 0122f66a //C 0f197   534 mvzl	r2,words */
0x0000f198, 0x01020000, /* 01020000 //C 0f198   535 mvzl	r0,0 */
0x0000f199, 0x0e420001, /* 0e420001 //C 0f199   537 ld	r4,r2,1		; addr */
0x0000f19a, 0x0e520002, /* 0e520002 //C 0f19a   538 ld	r5,r2,2		; value */
0x0000f19b, 0x024b0000, /* 024b0000 //C 0f19b   539 sz 	r4 */
0x0000f19c, 0x11f2f169, /* 11f2f169 //C 0f19c   540 jz	m_ret */
0x0000f19d, 0x00000400, /* 00000400 //C 0f19d   542 mov	r0,r4 */
0x0000f19e, 0x0400f459, /* 0400f459 //C 0f19e   543 call	htoi */
0x0000f19f, 0x00400100, /* 00400100 //C 0f19f   544 mov	r4,r1 */
0x0000f1a0, 0x31f2f1a4, /* 31f2f1a4 //C 0f1a0   545 C1 jmp	sm_addr_ok */
0x0000f1a1, 0x0102f16a, /* 0102f16a //C 0f1a1   546 mvzl	r0,m_err_addr */
0x0000f1a2, 0x0400f4d6, /* 0400f4d6 //C 0f1a2   547 call	printsnl */
0x0000f1a3, 0x01f2f1b8, /* 01f2f1b8 //C 0f1a3   548 jmp	sm_ret */
0x0000f1a4, 0x025b0000, /* 025b0000 //C 0f1a4   550 sz	r5 */
0x0000f1a5, 0x11f2f1b8, /* 11f2f1b8 //C 0f1a5   551 jz	sm_ret */
0x0000f1a6, 0x0132f000, /* 0132f000 //C 0f1a6   553 mvzl	r3,the_begin */
0x0000f1a7, 0x00380400, /* 00380400 //C 0f1a7   554 cmp	r3,r4 */
0x0000f1a8, 0x91f2f1b0, /* 91f2f1b0 //C 0f1a8   555 HI jmp	sm_addrv_ok */
0x0000f1a9, 0x0132f8d6, /* 0132f8d6 //C 0f1a9   556 mvzl	r3,the_end */
0x0000f1aa, 0x00380400, /* 00380400 //C 0f1aa   557 cmp	r3,r4 */
0x0000f1ab, 0x91f2f1ad, /* 91f2f1ad //C 0f1ab   558 HI jmp	sm_addrv_nok */
0x0000f1ac, 0x01f2f1b0, /* 01f2f1b0 //C 0f1ac   559 jmp	sm_addrv_ok */
0x0000f1ad, 0x0102f178, /* 0102f178 //C 0f1ad   561 mvzl	r0,m_err_addrv */
0x0000f1ae, 0x0400f4d6, /* 0400f4d6 //C 0f1ae   562 call	printsnl */
0x0000f1af, 0x01f2f1b8, /* 01f2f1b8 //C 0f1af   563 jmp	sm_ret */
0x0000f1b0, 0x00000500, /* 00000500 //C 0f1b0   565 mov	r0,r5 */
0x0000f1b1, 0x0400f459, /* 0400f459 //C 0f1b1   566 call	htoi */
0x0000f1b2, 0x00500100, /* 00500100 //C 0f1b2   567 mov	r5,r1 */
0x0000f1b3, 0x31f2f1b7, /* 31f2f1b7 //C 0f1b3   568 C1 jmp	sm_value_ok */
0x0000f1b4, 0x0102f18a, /* 0102f18a //C 0f1b4   569 mvzl	r0,m_err_value */
0x0000f1b5, 0x0400f4d6, /* 0400f4d6 //C 0f1b5   570 call	printsnl */
0x0000f1b6, 0x01f2f1b8, /* 01f2f1b8 //C 0f1b6   571 jmp	sm_ret */
0x0000f1b7, 0x0c540000, /* 0c540000 //C 0f1b7   573 st	r5,r4 */
0x0000f1b8, 0x0ffd0000, /* 0ffd0000 //C 0f1b8   576 pop	pc */
0x0000f1b9, 0x0ded0000, /* 0ded0000 //C 0f1b9   582 push	lr */
0x0000f1ba, 0x0122f66a, /* 0122f66a //C 0f1ba   583 mvzl	r2,words */
0x0000f1bb, 0x0e020001, /* 0e020001 //C 0f1bb   584 ld	r0,r2,1		; start address */
0x0000f1bc, 0x0400f459, /* 0400f459 //C 0f1bc   585 call	htoi */
0x0000f1bd, 0x00300100, /* 00300100 //C 0f1bd   586 mov	r3,r1 */
0x0000f1be, 0x0e020002, /* 0e020002 //C 0f1be   587 ld	r0,r2,2		; end address */
0x0000f1bf, 0x020b0000, /* 020b0000 //C 0f1bf   588 sz	r0 */
0x0000f1c0, 0x21f2f1c4, /* 21f2f1c4 //C 0f1c0   589 jnz	d_end_ok */
0x0000f1c1, 0x00400300, /* 00400300 //C 0f1c1   590 mov	r4,r3 */
0x0000f1c2, 0x01440010, /* 01440010 //C 0f1c2   591 add	r4,0x10 */
0x0000f1c3, 0x01f2f1c6, /* 01f2f1c6 //C 0f1c3   592 jmp	d_chk_end */
0x0000f1c4, 0x0400f459, /* 0400f459 //C 0f1c4   594 call	htoi */
0x0000f1c5, 0x00400100, /* 00400100 //C 0f1c5   595 mov	r4,r1 */
0x0000f1c6, 0x00380400, /* 00380400 //C 0f1c6   597 cmp	r3,r4		; check if start>end */
0x0000f1c7, 0x91f2f1d6, /* 91f2f1d6 //C 0f1c7   598 HI jmp d_bad */
0x0000f1c8, 0x00000300, /* 00000300 //C 0f1c8   606 mov	r0,r3		; print address */
0x0000f1c9, 0x01120004, /* 01120004 //C 0f1c9   607 mvzl	r1,4 */
0x0000f1ca, 0x0400f4dd, /* 0400f4dd //C 0f1ca   608 call	print_vhex */
0x0000f1cb, 0x01020020, /* 01020020 //C 0f1cb   609 mvzl	r0,0x20		; print one space */
0x0000f1cc, 0x0400f496, /* 0400f496 //C 0f1cc   610 call	putchar */
0x0000f1cd, 0x0e030000, /* 0e030000 //C 0f1cd   611 ld	r0,r3		; load data */
0x0000f1ce, 0x01120004, /* 01120004 //C 0f1ce   612 mvzl	r1,4		; print data */
0x0000f1cf, 0x0400f4dd, /* 0400f4dd //C 0f1cf   613 call	print_vhex */
0x0000f1d0, 0x0102000a, /* 0102000a //C 0f1d0   614 mvzl	r0,LF		; print new line */
0x0000f1d1, 0x0400f496, /* 0400f496 //C 0f1d1   615 call	putchar */
0x0000f1d2, 0x00380400, /* 00380400 //C 0f1d2   616 cmp	r3,r4 */
0x0000f1d3, 0x11f2f1d8, /* 11f2f1d8 //C 0f1d3   617 jz	d_ret */
0x0000f1d4, 0x01340001, /* 01340001 //C 0f1d4   618 add	r3,1 */
0x0000f1d5, 0x01f2f1c8, /* 01f2f1c8 //C 0f1d5   619 jmp	d_cyc */
0x0000f1d6, 0x0102f1d9, /* 0102f1d9 //C 0f1d6   621 mvzl	r0,d_err_bad */
0x0000f1d7, 0x0400f4d6, /* 0400f4d6 //C 0f1d7   622 call	printsnl */
0x0000f1d8, 0x0ffd0000, /* 0ffd0000 //C 0f1d8   624 pop	pc */
0x0000f1d9, 0x00000057, /* 00000057 //C 0f1d9   626 db	87 */
0x0000f1da, 0x00000072, /* 00000072 //C 0f1da   626 db	114 */
0x0000f1db, 0x0000006f, /* 0000006f //C 0f1db   626 db	111 */
0x0000f1dc, 0x0000006e, /* 0000006e //C 0f1dc   626 db	110 */
0x0000f1dd, 0x00000067, /* 00000067 //C 0f1dd   626 db	103 */
0x0000f1de, 0x00000020, /* 00000020 //C 0f1de   626 db	32 */
0x0000f1df, 0x00000065, /* 00000065 //C 0f1df   626 db	101 */
0x0000f1e0, 0x0000006e, /* 0000006e //C 0f1e0   626 db	110 */
0x0000f1e1, 0x00000064, /* 00000064 //C 0f1e1   626 db	100 */
0x0000f1e2, 0x00000020, /* 00000020 //C 0f1e2   626 db	32 */
0x0000f1e3, 0x00000061, /* 00000061 //C 0f1e3   626 db	97 */
0x0000f1e4, 0x00000064, /* 00000064 //C 0f1e4   626 db	100 */
0x0000f1e5, 0x00000064, /* 00000064 //C 0f1e5   626 db	100 */
0x0000f1e6, 0x00000072, /* 00000072 //C 0f1e6   626 db	114 */
0x0000f1e7, 0x00000065, /* 00000065 //C 0f1e7   626 db	101 */
0x0000f1e8, 0x00000073, /* 00000073 //C 0f1e8   626 db	115 */
0x0000f1e9, 0x00000073, /* 00000073 //C 0f1e9   626 db	115 */
0x0000f1ea, 0x00000000, /* 00000000 //C 0f1ea   626 db */
0x0000f1eb, 0x00f00e00, /* 00f00e00 //C 0f1eb   631 ret */
0x0000f1ec, 0x0ded0000, /* 0ded0000 //C 0f1ec   636 push	lr */
0x0000f1ed, 0x01a20000, /* 01a20000 //C 0f1ed   637 mvzl	r10,0		; state (nr of words) */
0x0000f1ee, 0x01820000, /* 01820000 //C 0f1ee   638 mvzl	r8,0		; value */
0x0000f1ef, 0x0162003f, /* 0162003f //C 0f1ef   639 mvzl	r6,'?'		; Record type */
0x0000f1f0, 0x01c20000, /* 01c20000 //C 0f1f0   640 mvzl	r12,0		; Checksum */
0x0000f1f1, 0x0400f487, /* 0400f487 //C 0f1f1   642 call	check_uart */
0x0000f1f2, 0x41f2f1f1, /* 41f2f1f1 //C 0f1f2   643 C0 jmp	l_cyc */
0x0000f1f3, 0x0400f48f, /* 0400f48f //C 0f1f3   644 call	read */
0x0000f1f4, 0x00b00000, /* 00b00000 //C 0f1f4   646 mov	r11,r0		; Copy of char in R11 */
0x0000f1f5, 0x0108000a, /* 0108000a //C 0f1f5   647 cmp	r0,10		; check EOL chars */
0x0000f1f6, 0x11f2f242, /* 11f2f242 //C 0f1f6   648 jz	l_eol */
0x0000f1f7, 0x0108000d, /* 0108000d //C 0f1f7   649 cmp	r0,13 */
0x0000f1f8, 0x11f2f242, /* 11f2f242 //C 0f1f8   650 jz	l_eol */
0x0000f1f9, 0x01a80000, /* 01a80000 //C 0f1f9   652 cmp	r10,0 */
0x0000f1fa, 0x21f2f20a, /* 21f2f20a //C 0f1fa   653 jnz	l_no0 */
0x0000f1fb, 0x0400f426, /* 0400f426 //C 0f1fb   655 call	hexchar2value */
0x0000f1fc, 0x41f2f204, /* 41f2f204 //C 0f1fc   656 C0 jmp	l_eof_0 */
0x0000f1fd, 0x02880000, /* 02880000 //C 0f1fd   657 shl	r8		; shift val(char) into value */
0x0000f1fe, 0x02880000, /* 02880000 //C 0f1fe   658 shl	r8 */
0x0000f1ff, 0x02880000, /* 02880000 //C 0f1ff   659 shl	r8 */
0x0000f200, 0x02880000, /* 02880000 //C 0f200   660 shl	r8 */
0x0000f201, 0x010b000f, /* 010b000f //C 0f201   661 btst	r0,0xf */
0x0000f202, 0x008d0000, /* 008d0000 //C 0f202   662 or	r8,r0 */
0x0000f203, 0x01f2f1f1, /* 01f2f1f1 //C 0f203   663 jmp	l_cyc */
0x0000f204, 0x01a20001, /* 01a20001 //C 0f204   665 mvzl	r10,1		; state0 -> state1 */
0x0000f205, 0x0162003f, /* 0162003f //C 0f205   666 mvzl	r6,'?'		; No //C yet */
0x0000f206, 0x01b8002f, /* 01b8002f //C 0f206   667 cmp	r11,'/'		; is it start of // */
0x0000f207, 0x11720001, /* 11720001 //C 0f207   668 z1 mvzl	r7,1		; Yes, first / found */
0x0000f208, 0x21720000, /* 21720000 //C 0f208   669 z0 mvzl	r7,0		; No '/' yet */
0x0000f209, 0x01f2f1f1, /* 01f2f1f1 //C 0f209   670 jmp	l_cyc */
0x0000f20a, 0x01a80001, /* 01a80001 //C 0f20a   673 cmp	r10,1 */
0x0000f20b, 0x21f2f225, /* 21f2f225 //C 0f20b   674 jnz	l_no1 */
0x0000f20c, 0x027b0000, /* 027b0000 //C 0f20c   676 sz	r7 */
0x0000f20d, 0x21f2f212, /* 21f2f212 //C 0f20d   677 jnz	l_s1_2nd */
0x0000f20e, 0x0108002f, /* 0108002f //C 0f20e   679 cmp	r0,'/' */
0x0000f20f, 0x21f2f1f1, /* 21f2f1f1 //C 0f20f   680 jnz	l_cyc */
0x0000f210, 0x01720001, /* 01720001 //C 0f210   681 mvzl	r7,1 */
0x0000f211, 0x01f2f1f1, /* 01f2f1f1 //C 0f211   682 jmp	l_cyc */
0x0000f212, 0x0108002f, /* 0108002f //C 0f212   684 cmp	r0,'/' */
0x0000f213, 0x11f2f1f1, /* 11f2f1f1 //C 0f213   685 jz	l_cyc */
0x0000f214, 0x01080043, /* 01080043 //C 0f214   686 cmp	r0,'C' */
0x0000f215, 0x11f2f218, /* 11f2f218 //C 0f215   687 jz	l_s1_C */
0x0000f216, 0x01080049, /* 01080049 //C 0f216   688 cmp	r0,'I' */
0x0000f217, 0x21f2f21d, /* 21f2f21d //C 0f217   689 jnz	l_s1_noC */
0x0000f218, 0x00600000, /* 00600000 //C 0f218   692 mov	r6,r0		; record type is in r0, store in r6 */
0x0000f219, 0x01a20002, /* 01a20002 //C 0f219   694 mvzl	r10,2 */
0x0000f21a, 0x01920000, /* 01920000 //C 0f21a   695 mvzl	r9,0		; address= 0 */
0x0000f21b, 0x01520000, /* 01520000 //C 0f21b   696 mvzl	r5,0		; where we are in word: before */
0x0000f21c, 0x01f2f1f1, /* 01f2f1f1 //C 0f21c   697 jmp	l_cyc */
0x0000f21d, 0x01080045, /* 01080045 //C 0f21d   699 cmp	r0,'E' */
0x0000f21e, 0x21f2f222, /* 21f2f222 //C 0f21e   700 jnz	l_s1_noE */
0x0000f21f, 0x01620045, /* 01620045 //C 0f21f   702 mvzl	r6,'E' */
0x0000f220, 0x01a20003, /* 01a20003 //C 0f220   704 mvzl	r10,3 */
0x0000f221, 0x01f2f1f1, /* 01f2f1f1 //C 0f221   705 jmp	l_cyc */
0x0000f222, 0x0400f496, /* 0400f496 //C 0f222   709 call	putchar		; print record type */
0x0000f223, 0x01a2000f, /* 01a2000f //C 0f223   710 mvzl	r10,0xf		; special state: skip everything */
0x0000f224, 0x01f2f1f1, /* 01f2f1f1 //C 0f224   711 jmp	l_cyc */
0x0000f225, 0x01a80002, /* 01a80002 //C 0f225   714 cmp	r10,2 */
0x0000f226, 0x21f2f23a, /* 21f2f23a //C 0f226   715 jnz	l_no2 */
0x0000f227, 0x01580000, /* 01580000 //C 0f227   717 cmp	r5,0 */
0x0000f228, 0x21f2f233, /* 21f2f233 //C 0f228   718 jnz	l_s2_no0 */
0x0000f229, 0x0400f426, /* 0400f426 //C 0f229   720 call	hexchar2value */
0x0000f22a, 0x41f2f1f1, /* 41f2f1f1 //C 0f22a   721 C0 jmp	l_cyc */
0x0000f22b, 0x01520001, /* 01520001 //C 0f22b   722 mvzl	r5,1 */
0x0000f22c, 0x02980000, /* 02980000 //C 0f22c   724 shl	r9 */
0x0000f22d, 0x02980000, /* 02980000 //C 0f22d   725 shl	r9 */
0x0000f22e, 0x02980000, /* 02980000 //C 0f22e   726 shl	r9 */
0x0000f22f, 0x02980000, /* 02980000 //C 0f22f   727 shl	r9 */
0x0000f230, 0x010b000f, /* 010b000f //C 0f230   728 btst	r0,0xf */
0x0000f231, 0x009d0000, /* 009d0000 //C 0f231   729 or	r9,r0 */
0x0000f232, 0x01f2f1f1, /* 01f2f1f1 //C 0f232   731 jmp	l_cyc */
0x0000f233, 0x01580001, /* 01580001 //C 0f233   733 cmp	r5,1 */
0x0000f234, 0x21f2f239, /* 21f2f239 //C 0f234   734 jnz	l_s2_no1 */
0x0000f235, 0x0400f426, /* 0400f426 //C 0f235   736 call	hexchar2value */
0x0000f236, 0x31f2f22c, /* 31f2f22c //C 0f236   737 C1 jmp	l_s2_got */
0x0000f237, 0x01520002, /* 01520002 //C 0f237   738 mvzl	r5,2 */
0x0000f238, 0x01f2f1f1, /* 01f2f1f1 //C 0f238   739 jmp	l_cyc */
0x0000f239, 0x01f2f1f1, /* 01f2f1f1 //C 0f239   741 jmp	l_cyc */
0x0000f23a, 0x01a80003, /* 01a80003 //C 0f23a   743 cmp	r10,3 */
0x0000f23b, 0x21f2f23d, /* 21f2f23d //C 0f23b   744 jnz	l_no3 */
0x0000f23c, 0x01f2f1f1, /* 01f2f1f1 //C 0f23c   746 jmp	l_cyc		; do nothing, just wait EOL */
0x0000f23d, 0x01a8000f, /* 01a8000f //C 0f23d   749 cmp	r10,0xf */
0x0000f23e, 0x01f2f240, /* 01f2f240 //C 0f23e   750 jmp	l_nof */
0x0000f23f, 0x01f2f1f1, /* 01f2f1f1 //C 0f23f   751 jmp	l_cyc		; just skip */
0x0000f240, 0x01f2f1f1, /* 01f2f1f1 //C 0f240   754 jmp	l_cyc */
0x0000f241, 0x01f2f25a, /* 01f2f25a //C 0f241   755 jmp	l_ret */
0x0000f242, 0x01a80000, /* 01a80000 //C 0f242   759 cmp	r10,0		; in state0 */
0x0000f243, 0x11f2f256, /* 11f2f256 //C 0f243   760 jz	l_back_to_0	; just restart */
0x0000f244, 0x01a80001, /* 01a80001 //C 0f244   761 cmp	r10,1		; in state1 */
0x0000f245, 0x11f2f256, /* 11f2f256 //C 0f245   762 jz	l_back_to_0 	;l_cyc ;l_bad ; garbage */
0x0000f246, 0x01a80002, /* 01a80002 //C 0f246   763 cmp	r10,2		; in state2 */
0x0000f247, 0x11f2f24e, /* 11f2f24e //C 0f247   764 jz	l_proc		; process record */
0x0000f248, 0x01a80003, /* 01a80003 //C 0f248   765 cmp	r10,3		; in state3 */
0x0000f249, 0x11f2f25a, /* 11f2f25a //C 0f249   766 jz	l_ret		; eol in end record: finish */
0x0000f24a, 0x01a8000f, /* 01a8000f //C 0f24a   767 cmp	r10,0xf		; in state skip */
0x0000f24b, 0x11f2f256, /* 11f2f256 //C 0f24b   768 jz	l_back_to_0	; reset state for new line */
0x0000f24c, 0x01f2f1f1, /* 01f2f1f1 //C 0f24c   769 jmp	l_cyc */
0x0000f24d, 0x01f2f25a, /* 01f2f25a //C 0f24d   771 jmp	l_ret */
0x0000f24e, 0x01680043, /* 01680043 //C 0f24e   773 cmp	r6,'C'		; is it a C or I record? */
0x0000f24f, 0x1c890000, /* 1c890000 //C 0f24f   774 z st	r8,r9		; then store */
0x0000f250, 0x10c40800, /* 10c40800 //C 0f250   775 z add	r12,r8		; and add to checksum */
0x0000f251, 0x01680049, /* 01680049 //C 0f251   776 cmp	r6,'I' */
0x0000f252, 0x1c890000, /* 1c890000 //C 0f252   777 z st	r8,r9 */
0x0000f253, 0x10c40800, /* 10c40800 //C 0f253   778 z add	r12,r8 */
0x0000f254, 0x00000600, /* 00000600 //C 0f254   779 mov	r0,r6		; echo record type */
0x0000f255, 0x0400f496, /* 0400f496 //C 0f255   780 call	putchar */
0x0000f256, 0x01a20000, /* 01a20000 //C 0f256   785 mvzl	r10,0 */
0x0000f257, 0x01820000, /* 01820000 //C 0f257   786 mvzl	r8,0 */
0x0000f258, 0x0162003f, /* 0162003f //C 0f258   787 mvzl	r6,'?' */
0x0000f259, 0x01f2f1f1, /* 01f2f1f1 //C 0f259   788 jmp	l_cyc */
0x0000f25a, 0x0102000a, /* 0102000a //C 0f25a   790 mvzl	r0,LF */
0x0000f25b, 0x0400f496, /* 0400f496 //C 0f25b   791 call	putchar */
0x0000f25c, 0x00000c00, /* 00000c00 //C 0f25c   793 mov	r0,r12 */
0x0000f25d, 0x01120004, /* 01120004 //C 0f25d   794 mvzl	r1,4 */
0x0000f25e, 0x0400f4dd, /* 0400f4dd //C 0f25e   795 call	print_vhex */
0x0000f25f, 0x0102000a, /* 0102000a //C 0f25f   796 mvzl	r0,LF */
0x0000f260, 0x0400f496, /* 0400f496 //C 0f260   797 call	putchar */
0x0000f261, 0x0ffd0000, /* 0ffd0000 //C 0f261   798 pop	pc */
0x0000f262, 0x0ded0000, /* 0ded0000 //C 0f262   804 push	lr */
0x0000f263, 0x0122f66a, /* 0122f66a //C 0f263   805 mvzl	r2,words */
0x0000f264, 0x0e020001, /* 0e020001 //C 0f264   806 ld	r0,r2,1		; address */
0x0000f265, 0x020b0000, /* 020b0000 //C 0f265   807 sz	r0 */
0x0000f266, 0x11f2f289, /* 11f2f289 //C 0f266   808 jz	g_no_addr */
0x0000f267, 0x0400f459, /* 0400f459 //C 0f267   809 call	htoi */
0x0000f268, 0x00b00100, /* 00b00100 //C 0f268   810 mov	r11,r1 */
0x0000f269, 0x0102f29c, /* 0102f29c //C 0f269   812 mvzl	r0,d_msg_run */
0x0000f26a, 0x0400f49d, /* 0400f49d //C 0f26a   813 call	prints */
0x0000f26b, 0x00000b00, /* 00000b00 //C 0f26b   814 mov	r0,r11 */
0x0000f26c, 0x01120004, /* 01120004 //C 0f26c   815 mvzl	r1,4 */
0x0000f26d, 0x0400f4dd, /* 0400f4dd //C 0f26d   816 call	print_vhex */
0x0000f26e, 0x0102000a, /* 0102000a //C 0f26e   817 mvzl	r0,LF */
0x0000f26f, 0x0400f496, /* 0400f496 //C 0f26f   818 call	putchar */
0x0000f270, 0x06b0f681, /* 06b0f681 //C 0f270   819 st	r11,reg15 */
0x0000f271, 0x0122ff43, /* 0122ff43 //C 0f271   821 mvzl	r2,UART_TSTAT */
0x0000f272, 0x0e920000, /* 0e920000 //C 0f272   823 ld	r9,r2 */
0x0000f273, 0x019c0001, /* 019c0001 //C 0f273   824 test	r9,1 */
0x0000f274, 0x11f2f272, /* 11f2f272 //C 0f274   825 jz	g_wait_tc */
0x0000f275, 0x07000000, /* 07000000 //C 0f275   827 ld	r0,0 */
0x0000f276, 0x0600f670, /* 0600f670 //C 0f276   828 st	r0,called */
0x0000f277, 0x0700f682, /* 0700f682 //C 0f277   830 ld	r0,regf */
0x0000f278, 0x020f0000, /* 020f0000 //C 0f278   831 setf	r0 */
0x0000f279, 0x0700f672, /* 0700f672 //C 0f279   832 ld	r0,reg0 */
0x0000f27a, 0x0710f673, /* 0710f673 //C 0f27a   833 ld	r1,reg1 */
0x0000f27b, 0x0720f674, /* 0720f674 //C 0f27b   834 ld	r2,reg2 */
0x0000f27c, 0x0730f675, /* 0730f675 //C 0f27c   835 ld	r3,reg3 */
0x0000f27d, 0x0740f676, /* 0740f676 //C 0f27d   836 ld	r4,reg4 */
0x0000f27e, 0x0750f677, /* 0750f677 //C 0f27e   837 ld	r5,reg5 */
0x0000f27f, 0x0760f678, /* 0760f678 //C 0f27f   838 ld	r6,reg6 */
0x0000f280, 0x0770f679, /* 0770f679 //C 0f280   839 ld	r7,reg7 */
0x0000f281, 0x0780f67a, /* 0780f67a //C 0f281   840 ld	r8,reg8 */
0x0000f282, 0x0790f67b, /* 0790f67b //C 0f282   841 ld	r9,reg9 */
0x0000f283, 0x07a0f67c, /* 07a0f67c //C 0f283   842 ld	r10,reg10 */
0x0000f284, 0x07b0f67d, /* 07b0f67d //C 0f284   843 ld	r11,reg11 */
0x0000f285, 0x07c0f67e, /* 07c0f67e //C 0f285   844 ld	r12,reg12 */
0x0000f286, 0x07d0f67f, /* 07d0f67f //C 0f286   845 ld	r13,reg13 */
0x0000f287, 0x07e0f680, /* 07e0f680 //C 0f287   846 ld	r14,reg14 */
0x0000f288, 0x07f0f681, /* 07f0f681 //C 0f288   848 ld	r15,reg15 */
0x0000f289, 0x0700f670, /* 0700f670 //C 0f289   851 ld	r0,called */
0x0000f28a, 0x020b0000, /* 020b0000 //C 0f28a   852 sz	r0 */
0x0000f28b, 0x11f2f28e, /* 11f2f28e //C 0f28b   853 jz	g_err */
0x0000f28c, 0x07b0f681, /* 07b0f681 //C 0f28c   854 ld	r11,reg15 */
0x0000f28d, 0x01f2f269, /* 01f2f269 //C 0f28d   855 jmp	g_go11 */
0x0000f28e, 0x0102f291, /* 0102f291 //C 0f28e   857 mvzl	r0,g_err_addr */
0x0000f28f, 0x0400f4d6, /* 0400f4d6 //C 0f28f   858 call	printsnl */
0x0000f290, 0x0ffd0000, /* 0ffd0000 //C 0f290   860 pop	pc */
0x0000f291, 0x0000004e, /* 0000004e //C 0f291   862 db	78 */
0x0000f292, 0x0000006f, /* 0000006f //C 0f292   862 db	111 */
0x0000f293, 0x00000020, /* 00000020 //C 0f293   862 db	32 */
0x0000f294, 0x00000061, /* 00000061 //C 0f294   862 db	97 */
0x0000f295, 0x00000064, /* 00000064 //C 0f295   862 db	100 */
0x0000f296, 0x00000064, /* 00000064 //C 0f296   862 db	100 */
0x0000f297, 0x00000072, /* 00000072 //C 0f297   862 db	114 */
0x0000f298, 0x00000065, /* 00000065 //C 0f298   862 db	101 */
0x0000f299, 0x00000073, /* 00000073 //C 0f299   862 db	115 */
0x0000f29a, 0x00000073, /* 00000073 //C 0f29a   862 db	115 */
0x0000f29b, 0x00000000, /* 00000000 //C 0f29b   862 db */
0x0000f29c, 0x00000052, /* 00000052 //C 0f29c   863 db	82 */
0x0000f29d, 0x00000075, /* 00000075 //C 0f29d   863 db	117 */
0x0000f29e, 0x0000006e, /* 0000006e //C 0f29e   863 db	110 */
0x0000f29f, 0x00000020, /* 00000020 //C 0f29f   863 db	32 */
0x0000f2a0, 0x00000000, /* 00000000 //C 0f2a0   863 db */
0x0000f2a1, 0x0ded0000, /* 0ded0000 //C 0f2a1   868 push	lr */
0x0000f2a2, 0x0122f725, /* 0122f725 //C 0f2a2   869 mvzl	r2,helps */
0x0000f2a3, 0x01320000, /* 01320000 //C 0f2a3   870 mvzl	r3,0 */
0x0000f2a4, 0x0b038200, /* 0b038200 //C 0f2a4   872 ld	r0,r3+,r2	; pick a char */
0x0000f2a5, 0x020b0000, /* 020b0000 //C 0f2a5   873 sz	r0		; is it eos? */
0x0000f2a6, 0x21f2f2aa, /* 21f2f2aa //C 0f2a6   874 jnz	h_print */
0x0000f2a7, 0x0b038200, /* 0b038200 //C 0f2a7   877 ld	r0,r3+,r2	; get first char of next string */
0x0000f2a8, 0x020b0000, /* 020b0000 //C 0f2a8   878 sz	r0 */
0x0000f2a9, 0x11f2f2ac, /* 11f2f2ac //C 0f2a9   879 jz	h_eof */
0x0000f2aa, 0x0400f496, /* 0400f496 //C 0f2aa   881 call	putchar */
0x0000f2ab, 0x01f2f2a4, /* 01f2f2a4 //C 0f2ab   882 jmp	h_cyc */
0x0000f2ac, 0x0ffd0000, /* 0ffd0000 //C 0f2ac   884 pop	pc */
0x0000f2ad, 0x0ded0000, /* 0ded0000 //C 0f2ad   890 push	lr */
0x0000f2ae, 0x0d1d0000, /* 0d1d0000 //C 0f2ae   891 push	r1 */
0x0000f2af, 0x00100000, /* 00100000 //C 0f2af   892 mov	r1,r0 */
0x0000f2b0, 0x01180010, /* 01180010 //C 0f2b0   893 cmp	r1,16		; go out if nr>16 */
0x0000f2b1, 0x91f2f2e5, /* 91f2f2e5 //C 0f2b1   894 HI jmp	prn_ret */
0x0000f2b2, 0x0118000f, /* 0118000f //C 0f2b2   895 cmp	r1,15		; nr=Flag? */
0x0000f2b3, 0xa1f2f2ba, /* a1f2f2ba //C 0f2b3   896 LS jmp	prn_015 */
0x0000f2b4, 0x01020046, /* 01020046 //C 0f2b4   898 mvzl	r0,'F' */
0x0000f2b5, 0x0400f496, /* 0400f496 //C 0f2b5   899 call	putchar */
0x0000f2b6, 0x01020020, /* 01020020 //C 0f2b6   900 mvzl	r0,32 */
0x0000f2b7, 0x0400f496, /* 0400f496 //C 0f2b7   901 call	putchar */
0x0000f2b8, 0x0400f496, /* 0400f496 //C 0f2b8   902 call	putchar */
0x0000f2b9, 0x01f2f2e5, /* 01f2f2e5 //C 0f2b9   903 jmp	prn_ret */
0x0000f2ba, 0x0118000f, /* 0118000f //C 0f2ba   905 cmp	r1,15 */
0x0000f2bb, 0x21f2f2c3, /* 21f2f2c3 //C 0f2bb   906 jnz	prn_no15 */
0x0000f2bc, 0x0102f2bf, /* 0102f2bf //C 0f2bc   907 mvzl	r0,prn_PC */
0x0000f2bd, 0x0400f49d, /* 0400f49d //C 0f2bd   908 call	prints */
0x0000f2be, 0x01f2f2e5, /* 01f2f2e5 //C 0f2be   909 jmp	prn_ret */
0x0000f2bf, 0x00000050, /* 00000050 //C 0f2bf   910 db	80 */
0x0000f2c0, 0x00000043, /* 00000043 //C 0f2c0   910 db	67 */
0x0000f2c1, 0x00000020, /* 00000020 //C 0f2c1   910 db	32 */
0x0000f2c2, 0x00000000, /* 00000000 //C 0f2c2   910 db */
0x0000f2c3, 0x0118000e, /* 0118000e //C 0f2c3   912 cmp	r1,14 */
0x0000f2c4, 0x21f2f2cc, /* 21f2f2cc //C 0f2c4   913 jnz	prn_no14 */
0x0000f2c5, 0x0102f2c8, /* 0102f2c8 //C 0f2c5   914 mvzl	r0,prn_LR */
0x0000f2c6, 0x0400f49d, /* 0400f49d //C 0f2c6   915 call	prints */
0x0000f2c7, 0x01f2f2e5, /* 01f2f2e5 //C 0f2c7   916 jmp	prn_ret */
0x0000f2c8, 0x0000004c, /* 0000004c //C 0f2c8   917 db	76 */
0x0000f2c9, 0x00000052, /* 00000052 //C 0f2c9   917 db	82 */
0x0000f2ca, 0x00000020, /* 00000020 //C 0f2ca   917 db	32 */
0x0000f2cb, 0x00000000, /* 00000000 //C 0f2cb   917 db */
0x0000f2cc, 0x0118000d, /* 0118000d //C 0f2cc   919 cmp	r1,13 */
0x0000f2cd, 0x21f2f2d5, /* 21f2f2d5 //C 0f2cd   920 jnz	prn_no13 */
0x0000f2ce, 0x0102f2d1, /* 0102f2d1 //C 0f2ce   921 mvzl	r0,prn_SP */
0x0000f2cf, 0x0400f49d, /* 0400f49d //C 0f2cf   922 call	prints */
0x0000f2d0, 0x01f2f2e5, /* 01f2f2e5 //C 0f2d0   923 jmp	prn_ret */
0x0000f2d1, 0x00000053, /* 00000053 //C 0f2d1   924 db	83 */
0x0000f2d2, 0x00000050, /* 00000050 //C 0f2d2   924 db	80 */
0x0000f2d3, 0x00000020, /* 00000020 //C 0f2d3   924 db	32 */
0x0000f2d4, 0x00000000, /* 00000000 //C 0f2d4   924 db */
0x0000f2d5, 0x01020052, /* 01020052 //C 0f2d5   926 mvzl	r0,'R' */
0x0000f2d6, 0x0400f496, /* 0400f496 //C 0f2d6   927 call	putchar */
0x0000f2d7, 0x01180009, /* 01180009 //C 0f2d7   928 cmp	r1,9 */
0x0000f2d8, 0x91f2f2df, /* 91f2f2df //C 0f2d8   929 HI jmp	prn_1015 */
0x0000f2d9, 0x00000100, /* 00000100 //C 0f2d9   931 mov	r0,r1 */
0x0000f2da, 0x01040030, /* 01040030 //C 0f2da   932 add	r0,'0' */
0x0000f2db, 0x0400f496, /* 0400f496 //C 0f2db   933 call	putchar */
0x0000f2dc, 0x01020020, /* 01020020 //C 0f2dc   934 mvzl	r0,32 */
0x0000f2dd, 0x0400f496, /* 0400f496 //C 0f2dd   935 call	putchar */
0x0000f2de, 0x01f2f2e5, /* 01f2f2e5 //C 0f2de   936 jmp	prn_ret */
0x0000f2df, 0x01020031, /* 01020031 //C 0f2df   938 mvzl	r0,'1' */
0x0000f2e0, 0x0400f496, /* 0400f496 //C 0f2e0   939 call	putchar */
0x0000f2e1, 0x00000100, /* 00000100 //C 0f2e1   940 mov	r0,r1 */
0x0000f2e2, 0x0106000a, /* 0106000a //C 0f2e2   941 sub	r0,10 */
0x0000f2e3, 0x01040030, /* 01040030 //C 0f2e3   942 add	r0,'0' */
0x0000f2e4, 0x0400f496, /* 0400f496 //C 0f2e4   943 call	putchar */
0x0000f2e5, 0x0f1d0000, /* 0f1d0000 //C 0f2e5   945 pop	r1 */
0x0000f2e6, 0x0ffd0000, /* 0ffd0000 //C 0f2e6   946 pop	pc */
0x0000f2e7, 0x0ded0000, /* 0ded0000 //C 0f2e7   951 push	lr */
0x0000f2e8, 0x0d1d0000, /* 0d1d0000 //C 0f2e8   952 push	r1 */
0x0000f2e9, 0x01080010, /* 01080010 //C 0f2e9   953 cmp	r0,16 */
0x0000f2ea, 0x91f2f2ef, /* 91f2f2ef //C 0f2ea   954 HI jmp	prv_ret */
0x0000f2eb, 0x0112f672, /* 0112f672 //C 0f2eb   955 mvzl	r1,reg0 */
0x0000f2ec, 0x0a010000, /* 0a010000 //C 0f2ec   956 ld	r0,r1,r0 */
0x0000f2ed, 0x01120004, /* 01120004 //C 0f2ed   957 mvzl	r1,4 */
0x0000f2ee, 0x0400f4dd, /* 0400f4dd //C 0f2ee   958 call	print_vhex */
0x0000f2ef, 0x0f1d0000, /* 0f1d0000 //C 0f2ef   960 pop	r1 */
0x0000f2f0, 0x0ffd0000, /* 0ffd0000 //C 0f2f0   961 pop	pc */
0x0000f2f1, 0x0ded0000, /* 0ded0000 //C 0f2f1   968 push	lr */
0x0000f2f2, 0x0dad0000, /* 0dad0000 //C 0f2f2   969 push	r10 */
0x0000f2f3, 0x0d0d0000, /* 0d0d0000 //C 0f2f3   970 push	r0 */
0x0000f2f4, 0x00000a00, /* 00000a00 //C 0f2f4   971 mov	r0,r10 */
0x0000f2f5, 0x0400f2ad, /* 0400f2ad //C 0f2f5   972 call	print_reg_name */
0x0000f2f6, 0x01020020, /* 01020020 //C 0f2f6   973 mvzl	r0,32 */
0x0000f2f7, 0x0400f496, /* 0400f496 //C 0f2f7   974 call	putchar */
0x0000f2f8, 0x00000a00, /* 00000a00 //C 0f2f8   975 mov	r0,r10 */
0x0000f2f9, 0x0400f2e7, /* 0400f2e7 //C 0f2f9   976 call	print_reg_value */
0x0000f2fa, 0x0102000a, /* 0102000a //C 0f2fa   977 mvzl	r0,LF */
0x0000f2fb, 0x0400f496, /* 0400f496 //C 0f2fb   978 call	putchar */
0x0000f2fc, 0x0f0d0000, /* 0f0d0000 //C 0f2fc   979 pop	r0 */
0x0000f2fd, 0x0fad0000, /* 0fad0000 //C 0f2fd   980 pop	r10 */
0x0000f2fe, 0x0ffd0000, /* 0ffd0000 //C 0f2fe   981 pop	pc */
0x0000f2ff, 0x0ded0000, /* 0ded0000 //C 0f2ff   989 push	lr */
0x0000f300, 0x0400f496, /* 0400f496 //C 0f300   990 call	putchar */
0x0000f301, 0x001c0200, /* 001c0200 //C 0f301   991 test	r1,r2 */
0x0000f302, 0x21020031, /* 21020031 //C 0f302   992 NZ mvzl r0,'1' */
0x0000f303, 0x11020030, /* 11020030 //C 0f303   993 Z  mvzl r0,'0' */
0x0000f304, 0x0400f496, /* 0400f496 //C 0f304   994 call	putchar */
0x0000f305, 0x00000300, /* 00000300 //C 0f305   995 mov	r0,r3 */
0x0000f306, 0x0400f496, /* 0400f496 //C 0f306   996 call	putchar */
0x0000f307, 0x0ffd0000, /* 0ffd0000 //C 0f307   997 pop	pc */
0x0000f308, 0x0ded0000, /* 0ded0000 //C 0f308  1002 push	lr */
0x0000f309, 0x0700f670, /* 0700f670 //C 0f309  1003 ld	r0,called */
0x0000f30a, 0x020b0000, /* 020b0000 //C 0f30a  1004 sz	r0 */
0x0000f30b, 0x11f2f314, /* 11f2f314 //C 0f30b  1005 jz	r_not_called */
0x0000f30c, 0x0102f331, /* 0102f331 //C 0f30c  1007 mvzl	r0,msg_r_called */
0x0000f30d, 0x0400f49d, /* 0400f49d //C 0f30d  1008 call	prints */
0x0000f30e, 0x0700f681, /* 0700f681 //C 0f30e  1009 ld	r0,reg15 */
0x0000f30f, 0x01120004, /* 01120004 //C 0f30f  1010 mvzl	r1,4 */
0x0000f310, 0x0400f4dd, /* 0400f4dd //C 0f310  1011 call	print_vhex */
0x0000f311, 0x0102000a, /* 0102000a //C 0f311  1012 mvzl	r0,LF */
0x0000f312, 0x0400f496, /* 0400f496 //C 0f312  1013 call	putchar */
0x0000f313, 0x01f2f316, /* 01f2f316 //C 0f313  1014 jmp	r_start */
0x0000f314, 0x0102f347, /* 0102f347 //C 0f314  1016 mvzl	r0,msg_r_notcalled */
0x0000f315, 0x0400f49d, /* 0400f49d //C 0f315  1017 call	prints */
0x0000f316, 0x01a20000, /* 01a20000 //C 0f316  1019 mvzl	r10,0 */
0x0000f317, 0x0400f2f1, /* 0400f2f1 //C 0f317  1021 call	print_reg_name_value */
0x0000f318, 0x01a40001, /* 01a40001 //C 0f318  1022 add	r10,1 */
0x0000f319, 0x01a80011, /* 01a80011 //C 0f319  1023 cmp	r10,17 */
0x0000f31a, 0x21f2f317, /* 21f2f317 //C 0f31a  1024 jnz	r_cyc */
0x0000f31b, 0x0710f682, /* 0710f682 //C 0f31b  1026 ld	r1,regf */
0x0000f31c, 0x01320020, /* 01320020 //C 0f31c  1027 mvzl	r3,32 */
0x0000f31d, 0x01020055, /* 01020055 //C 0f31d  1028 mvzl	r0,'U' */
0x0000f31e, 0x01220020, /* 01220020 //C 0f31e  1029 mvzl	r2,0x20 */
0x0000f31f, 0x0400f2ff, /* 0400f2ff //C 0f31f  1030 call	print_flag */
0x0000f320, 0x01020050, /* 01020050 //C 0f320  1031 mvzl	r0,'P' */
0x0000f321, 0x01220010, /* 01220010 //C 0f321  1032 mvzl	r2,0x10 */
0x0000f322, 0x0400f2ff, /* 0400f2ff //C 0f322  1033 call	print_flag */
0x0000f323, 0x0102004f, /* 0102004f //C 0f323  1034 mvzl	r0,'O' */
0x0000f324, 0x01220008, /* 01220008 //C 0f324  1035 mvzl	r2,8 */
0x0000f325, 0x0400f2ff, /* 0400f2ff //C 0f325  1036 call	print_flag */
0x0000f326, 0x0102005a, /* 0102005a //C 0f326  1037 mvzl	r0,'Z' */
0x0000f327, 0x01220004, /* 01220004 //C 0f327  1038 mvzl	r2,4 */
0x0000f328, 0x0400f2ff, /* 0400f2ff //C 0f328  1039 call	print_flag */
0x0000f329, 0x01020043, /* 01020043 //C 0f329  1040 mvzl	r0,'C' */
0x0000f32a, 0x01220002, /* 01220002 //C 0f32a  1041 mvzl	r2,2 */
0x0000f32b, 0x0400f2ff, /* 0400f2ff //C 0f32b  1042 call	print_flag */
0x0000f32c, 0x01020053, /* 01020053 //C 0f32c  1043 mvzl	r0,'S' */
0x0000f32d, 0x01220001, /* 01220001 //C 0f32d  1044 mvzl	r2,1 */
0x0000f32e, 0x0132000a, /* 0132000a //C 0f32e  1045 mvzl	r3,LF */
0x0000f32f, 0x0400f2ff, /* 0400f2ff //C 0f32f  1046 call	print_flag */
0x0000f330, 0x0ffd0000, /* 0ffd0000 //C 0f330  1047 pop	pc */
0x0000f331, 0x0000004d, /* 0000004d //C 0f331  1049 db	77 */
0x0000f332, 0x0000006f, /* 0000006f //C 0f332  1049 db	111 */
0x0000f333, 0x0000006e, /* 0000006e //C 0f333  1049 db	110 */
0x0000f334, 0x00000069, /* 00000069 //C 0f334  1049 db	105 */
0x0000f335, 0x00000074, /* 00000074 //C 0f335  1049 db	116 */
0x0000f336, 0x0000006f, /* 0000006f //C 0f336  1049 db	111 */
0x0000f337, 0x00000072, /* 00000072 //C 0f337  1049 db	114 */
0x0000f338, 0x00000020, /* 00000020 //C 0f338  1049 db	32 */
0x0000f339, 0x00000063, /* 00000063 //C 0f339  1049 db	99 */
0x0000f33a, 0x00000061, /* 00000061 //C 0f33a  1049 db	97 */
0x0000f33b, 0x0000006c, /* 0000006c //C 0f33b  1049 db	108 */
0x0000f33c, 0x0000006c, /* 0000006c //C 0f33c  1049 db	108 */
0x0000f33d, 0x00000065, /* 00000065 //C 0f33d  1049 db	101 */
0x0000f33e, 0x00000064, /* 00000064 //C 0f33e  1049 db	100 */
0x0000f33f, 0x00000020, /* 00000020 //C 0f33f  1049 db	32 */
0x0000f340, 0x00000066, /* 00000066 //C 0f340  1049 db	102 */
0x0000f341, 0x00000072, /* 00000072 //C 0f341  1049 db	114 */
0x0000f342, 0x0000006f, /* 0000006f //C 0f342  1049 db	111 */
0x0000f343, 0x0000006d, /* 0000006d //C 0f343  1049 db	109 */
0x0000f344, 0x0000003a, /* 0000003a //C 0f344  1049 db	58 */
0x0000f345, 0x00000020, /* 00000020 //C 0f345  1049 db	32 */
0x0000f346, 0x00000000, /* 00000000 //C 0f346  1049 db */
0x0000f347, 0x0000004d, /* 0000004d //C 0f347  1050 db	77 */
0x0000f348, 0x0000006f, /* 0000006f //C 0f348  1050 db	111 */
0x0000f349, 0x0000006e, /* 0000006e //C 0f349  1050 db	110 */
0x0000f34a, 0x00000069, /* 00000069 //C 0f34a  1050 db	105 */
0x0000f34b, 0x00000074, /* 00000074 //C 0f34b  1050 db	116 */
0x0000f34c, 0x0000006f, /* 0000006f //C 0f34c  1050 db	111 */
0x0000f34d, 0x00000072, /* 00000072 //C 0f34d  1050 db	114 */
0x0000f34e, 0x00000020, /* 00000020 //C 0f34e  1050 db	32 */
0x0000f34f, 0x0000006e, /* 0000006e //C 0f34f  1050 db	110 */
0x0000f350, 0x0000006f, /* 0000006f //C 0f350  1050 db	111 */
0x0000f351, 0x00000074, /* 00000074 //C 0f351  1050 db	116 */
0x0000f352, 0x00000020, /* 00000020 //C 0f352  1050 db	32 */
0x0000f353, 0x00000063, /* 00000063 //C 0f353  1050 db	99 */
0x0000f354, 0x00000061, /* 00000061 //C 0f354  1050 db	97 */
0x0000f355, 0x0000006c, /* 0000006c //C 0f355  1050 db	108 */
0x0000f356, 0x0000006c, /* 0000006c //C 0f356  1050 db	108 */
0x0000f357, 0x00000065, /* 00000065 //C 0f357  1050 db	101 */
0x0000f358, 0x00000064, /* 00000064 //C 0f358  1050 db	100 */
0x0000f359, 0x00000020, /* 00000020 //C 0f359  1050 db	32 */
0x0000f35a, 0x00000062, /* 00000062 //C 0f35a  1050 db	98 */
0x0000f35b, 0x00000079, /* 00000079 //C 0f35b  1050 db	121 */
0x0000f35c, 0x00000020, /* 00000020 //C 0f35c  1050 db	32 */
0x0000f35d, 0x00000075, /* 00000075 //C 0f35d  1050 db	117 */
0x0000f35e, 0x00000073, /* 00000073 //C 0f35e  1050 db	115 */
0x0000f35f, 0x00000065, /* 00000065 //C 0f35f  1050 db	101 */
0x0000f360, 0x00000072, /* 00000072 //C 0f360  1050 db	114 */
0x0000f361, 0x0000000a, /* 0000000a //C 0f361  1050 db	10 */
0x0000f362, 0x00000000, /* 00000000 //C 0f362  1050 db */
0x0000f363, 0x0ded0000, /* 0ded0000 //C 0f363  1055 push	lr */
0x0000f364, 0x07a0f671, /* 07a0f671 //C 0f364  1056 ld	r10,nuof_reg	; Reg num is in R10 */
0x0000f365, 0x01a80010, /* 01a80010 //C 0f365  1057 cmp	r10,16 */
0x0000f366, 0xa1f2f37b, /* a1f2f37b //C 0f366  1058 LS jmp	rx_nr_ok */
0x0000f367, 0x0102f36a, /* 0102f36a //C 0f367  1059 mvzl	r0,rx_err_nr */
0x0000f368, 0x0400f4d6, /* 0400f4d6 //C 0f368  1060 call	printsnl */
0x0000f369, 0x01f2f397, /* 01f2f397 //C 0f369  1061 jmp	rx_ret */
0x0000f36a, 0x0000004e, /* 0000004e //C 0f36a  1063 db	78 */
0x0000f36b, 0x0000006f, /* 0000006f //C 0f36b  1063 db	111 */
0x0000f36c, 0x00000020, /* 00000020 //C 0f36c  1063 db	32 */
0x0000f36d, 0x00000073, /* 00000073 //C 0f36d  1063 db	115 */
0x0000f36e, 0x00000075, /* 00000075 //C 0f36e  1063 db	117 */
0x0000f36f, 0x00000063, /* 00000063 //C 0f36f  1063 db	99 */
0x0000f370, 0x00000068, /* 00000068 //C 0f370  1063 db	104 */
0x0000f371, 0x00000020, /* 00000020 //C 0f371  1063 db	32 */
0x0000f372, 0x00000072, /* 00000072 //C 0f372  1063 db	114 */
0x0000f373, 0x00000065, /* 00000065 //C 0f373  1063 db	101 */
0x0000f374, 0x00000067, /* 00000067 //C 0f374  1063 db	103 */
0x0000f375, 0x00000069, /* 00000069 //C 0f375  1063 db	105 */
0x0000f376, 0x00000073, /* 00000073 //C 0f376  1063 db	115 */
0x0000f377, 0x00000074, /* 00000074 //C 0f377  1063 db	116 */
0x0000f378, 0x00000065, /* 00000065 //C 0f378  1063 db	101 */
0x0000f379, 0x00000072, /* 00000072 //C 0f379  1063 db	114 */
0x0000f37a, 0x00000000, /* 00000000 //C 0f37a  1063 db */
0x0000f37b, 0x0122f66a, /* 0122f66a //C 0f37b  1065 mvzl	r2,words */
0x0000f37c, 0x0e420001, /* 0e420001 //C 0f37c  1066 ld	r4,r2,1		; get aof first parameter */
0x0000f37d, 0x024b0000, /* 024b0000 //C 0f37d  1067 sz	r4		; is it NULL? */
0x0000f37e, 0x11f2f396, /* 11f2f396 //C 0f37e  1068 jz	rx_print */
0x0000f37f, 0x00000400, /* 00000400 //C 0f37f  1069 mov	r0,r4 */
0x0000f380, 0x0400f459, /* 0400f459 //C 0f380  1070 call	htoi */
0x0000f381, 0x00500100, /* 00500100 //C 0f381  1071 mov	r5,r1		; Value is in R5 */
0x0000f382, 0x31f2f392, /* 31f2f392 //C 0f382  1072 C1 jmp	rx_val_ok */
0x0000f383, 0x0102f386, /* 0102f386 //C 0f383  1073 mvzl	r0,rx_err_val */
0x0000f384, 0x0400f4d6, /* 0400f4d6 //C 0f384  1074 call	printsnl */
0x0000f385, 0x01f2f397, /* 01f2f397 //C 0f385  1075 jmp	rx_ret */
0x0000f386, 0x00000056, /* 00000056 //C 0f386  1077 db	86 */
0x0000f387, 0x00000061, /* 00000061 //C 0f387  1077 db	97 */
0x0000f388, 0x0000006c, /* 0000006c //C 0f388  1077 db	108 */
0x0000f389, 0x00000075, /* 00000075 //C 0f389  1077 db	117 */
0x0000f38a, 0x00000065, /* 00000065 //C 0f38a  1077 db	101 */
0x0000f38b, 0x00000020, /* 00000020 //C 0f38b  1077 db	32 */
0x0000f38c, 0x00000065, /* 00000065 //C 0f38c  1077 db	101 */
0x0000f38d, 0x00000072, /* 00000072 //C 0f38d  1077 db	114 */
0x0000f38e, 0x00000072, /* 00000072 //C 0f38e  1077 db	114 */
0x0000f38f, 0x0000006f, /* 0000006f //C 0f38f  1077 db	111 */
0x0000f390, 0x00000072, /* 00000072 //C 0f390  1077 db	114 */
0x0000f391, 0x00000000, /* 00000000 //C 0f391  1077 db */
0x0000f392, 0x01a80010, /* 01a80010 //C 0f392  1079 cmp	r10,16		; Flag reg? */
0x0000f393, 0x115f003f, /* 115f003f //C 0f393  1080 EQ and	r5,0x3f */
0x0000f394, 0x0102f672, /* 0102f672 //C 0f394  1081 mvzl	r0,reg0 */
0x0000f395, 0x08500a00, /* 08500a00 //C 0f395  1082 st	r5,r0,r10 */
0x0000f396, 0x0400f2f1, /* 0400f2f1 //C 0f396  1084 call	print_reg_name_value */
0x0000f397, 0x0ffd0000, /* 0ffd0000 //C 0f397  1086 pop	pc */
0x0000f398, 0x0102000d, /* 0102000d //C 0f398  1092 mvzl	r0,13 */
0x0000f399, 0x0600f671, /* 0600f671 //C 0f399  1093 st	r0,nuof_reg */
0x0000f39a, 0x01f2f363, /* 01f2f363 //C 0f39a  1094 jmp	cmd_rx */
0x0000f39b, 0x0102000e, /* 0102000e //C 0f39b  1099 mvzl	r0,14 */
0x0000f39c, 0x0600f671, /* 0600f671 //C 0f39c  1100 st	r0,nuof_reg */
0x0000f39d, 0x01f2f363, /* 01f2f363 //C 0f39d  1101 jmp	cmd_rx */
0x0000f39e, 0x0102000f, /* 0102000f //C 0f39e  1106 mvzl	r0,15 */
0x0000f39f, 0x0600f671, /* 0600f671 //C 0f39f  1107 st	r0,nuof_reg */
0x0000f3a0, 0x01f2f363, /* 01f2f363 //C 0f3a0  1108 jmp	cmd_rx */
0x0000f3a1, 0x01020010, /* 01020010 //C 0f3a1  1113 mvzl	r0,16 */
0x0000f3a2, 0x0600f671, /* 0600f671 //C 0f3a2  1114 st	r0,nuof_reg */
0x0000f3a3, 0x01f2f363, /* 01f2f363 //C 0f3a3  1115 jmp	cmd_rx */
0x0000f3a4, 0x0d3d0000, /* 0d3d0000 //C 0f3a4  1128 push	r3 */
0x0000f3a5, 0x0d4d0000, /* 0d4d0000 //C 0f3a5  1129 push	r4 */
0x0000f3a6, 0x01220000, /* 01220000 //C 0f3a6  1131 mvzl	r2,0		; byte index re-start */
0x0000f3a7, 0x0e310000, /* 0e310000 //C 0f3a7  1132 ld	r3,r1		; get next word */
0x0000f3a8, 0x023b0000, /* 023b0000 //C 0f3a8  1133 sz	r3		; check for eof */
0x0000f3a9, 0x11f2f3b6, /* 11f2f3b6 //C 0f3a9  1134 jz	strchr_no	; eof string found */
0x0000f3aa, 0x06414302, /* 06414302 //C 0f3aa  1136 getbz	r4,r3,r2	; pick a byte */
0x0000f3ab, 0x024b0000, /* 024b0000 //C 0f3ab  1137 sz	r4		; is it zero? */
0x0000f3ac, 0x11f2f3b2, /* 11f2f3b2 //C 0f3ac  1138 jz	strchr_word	; if yes, pick next word */
0x0000f3ad, 0x00480000, /* 00480000 //C 0f3ad  1139 cmp	r4,r0		; compare */
0x0000f3ae, 0x11f2f3b4, /* 11f2f3b4 //C 0f3ae  1140 jz	strchr_yes	; found it */
0x0000f3af, 0x01240001, /* 01240001 //C 0f3af  1142 inc	r2		; advance byte index */
0x0000f3b0, 0x01280004, /* 01280004 //C 0f3b0  1143 cmp	r2,4		; check byte overflow */
0x0000f3b1, 0x21f2f3aa, /* 21f2f3aa //C 0f3b1  1144 jnz	strchr_go	; no, overflow, go on */
0x0000f3b2, 0x011a0001, /* 011a0001 //C 0f3b2  1146 plus	r1,1		; go to next char */
0x0000f3b3, 0x01f2f3a6, /* 01f2f3a6 //C 0f3b3  1147 jmp	strchr_cyc */
0x0000f3b4, 0x020c0000, /* 020c0000 //C 0f3b4  1149 sec */
0x0000f3b5, 0x01f2f3b8, /* 01f2f3b8 //C 0f3b5  1150 jmp	strchr_ret */
0x0000f3b6, 0x01120000, /* 01120000 //C 0f3b6  1152 mvzl	r1,0 */
0x0000f3b7, 0x020d0000, /* 020d0000 //C 0f3b7  1153 clc */
0x0000f3b8, 0x0f4d0000, /* 0f4d0000 //C 0f3b8  1155 pop	r4 */
0x0000f3b9, 0x0f3d0000, /* 0f3d0000 //C 0f3b9  1156 pop	r3 */
0x0000f3ba, 0x00f00e00, /* 00f00e00 //C 0f3ba  1159 ret */
0x0000f3bb, 0x0ded0000, /* 0ded0000 //C 0f3bb  1170 push	lr		; Save used registers */
0x0000f3bc, 0x0d0d0000, /* 0d0d0000 //C 0f3bc  1171 push	r0		; and input parameters */
0x0000f3bd, 0x0d2d0000, /* 0d2d0000 //C 0f3bd  1173 push	r2 */
0x0000f3be, 0x0d4d0000, /* 0d4d0000 //C 0f3be  1174 push	r4 */
0x0000f3bf, 0x0d5d0000, /* 0d5d0000 //C 0f3bf  1175 push	r5 */
0x0000f3c0, 0x0d6d0000, /* 0d6d0000 //C 0f3c0  1176 push	r6 */
0x0000f3c1, 0x0d7d0000, /* 0d7d0000 //C 0f3c1  1177 push	r7		; byte idx in string 1 */
0x0000f3c2, 0x0d8d0000, /* 0d8d0000 //C 0f3c2  1178 push	r8		; byte idx in string 2 */
0x0000f3c3, 0x01720000, /* 01720000 //C 0f3c3  1179 mvzl	r7,0 */
0x0000f3c4, 0x01820000, /* 01820000 //C 0f3c4  1180 mvzl	r8,0 */
0x0000f3c5, 0x0e200000, /* 0e200000 //C 0f3c5  1182 ld	r2,r0		; Got one char from first str */
0x0000f3c6, 0x022b0000, /* 022b0000 //C 0f3c6  1183 sz	r2		; is it eos? */
0x0000f3c7, 0x11f2f3d6, /* 11f2f3d6 //C 0f3c7  1184 jz	streq_pick2	; if yes, go on */
0x0000f3c8, 0x06214207, /* 06214207 //C 0f3c8  1185 getbz	r2,r2,r7	; pick one byte */
0x0000f3c9, 0x022b0000, /* 022b0000 //C 0f3c9  1186 sz	r2		; is it 0? */
0x0000f3ca, 0x21f2f3d6, /* 21f2f3d6 //C 0f3ca  1187 jnz	streq_pick2	; if not, go on */
0x0000f3cb, 0x01740001, /* 01740001 //C 0f3cb  1188 inc	r7		; step to next byte */
0x0000f3cc, 0x01780004, /* 01780004 //C 0f3cc  1189 cmp	r7,4		; word is overflowed? */
0x0000f3cd, 0x11f2f3d2, /* 11f2f3d2 //C 0f3cd  1190 jz	streq_p1ov */
0x0000f3ce, 0x0e200000, /* 0e200000 //C 0f3ce  1192 ld	r2,r0		; pick orig word, and */
0x0000f3cf, 0x06214207, /* 06214207 //C 0f3cf  1193 getbz	r2,r2,r7	; check next byte */
0x0000f3d0, 0x022b0000, /* 022b0000 //C 0f3d0  1194 sz	r2		; is it 0? */
0x0000f3d1, 0x21f2f3d6, /* 21f2f3d6 //C 0f3d1  1195 jnz	streq_pick2	; if not, go on */
0x0000f3d2, 0x01040001, /* 01040001 //C 0f3d2  1197 inc	r0		; if yes, move pointer */
0x0000f3d3, 0x01720000, /* 01720000 //C 0f3d3  1198 mvzl	r7,0		; and reset byte counter */
0x0000f3d4, 0x0e200000, /* 0e200000 //C 0f3d4  1199 ld	r2,r0		; get first byte of next word */
0x0000f3d5, 0x06214207, /* 06214207 //C 0f3d5  1200 getbz	r2,r2,r7 */
0x0000f3d6, 0x0e610000, /* 0e610000 //C 0f3d6  1203 ld	r6,r1		; pick from second string */
0x0000f3d7, 0x026b0000, /* 026b0000 //C 0f3d7  1204 sz	r6		; is it eos? */
0x0000f3d8, 0x11f2f3e7, /* 11f2f3e7 //C 0f3d8  1205 jz	streq_prep	; if yes, go to compare */
0x0000f3d9, 0x06614608, /* 06614608 //C 0f3d9  1206 getbz	r6,r6,r8	; pick a byte */
0x0000f3da, 0x026b0000, /* 026b0000 //C 0f3da  1207 sz	r6		; is it 0? */
0x0000f3db, 0x21f2f3e7, /* 21f2f3e7 //C 0f3db  1208 jnz	streq_prep	; if not, go to compare */
0x0000f3dc, 0x01840001, /* 01840001 //C 0f3dc  1209 inc	r8		; step to next byte */
0x0000f3dd, 0x01880004, /* 01880004 //C 0f3dd  1210 cmp	r8,4		; is word overflowed? */
0x0000f3de, 0x11f2f3e3, /* 11f2f3e3 //C 0f3de  1211 jz	streq_p2ov */
0x0000f3df, 0x0e610000, /* 0e610000 //C 0f3df  1213 ld	r6,r1		; pick orig word, and */
0x0000f3e0, 0x06614608, /* 06614608 //C 0f3e0  1214 getbz	r6,r6,r8	; check next byte */
0x0000f3e1, 0x026b0000, /* 026b0000 //C 0f3e1  1215 sz	r6		; is it 0? */
0x0000f3e2, 0x21f2f3e7, /* 21f2f3e7 //C 0f3e2  1216 jnz	streq_prep	; if not, go on */
0x0000f3e3, 0x01140001, /* 01140001 //C 0f3e3  1218 inc	r1		; if yes, move pointer */
0x0000f3e4, 0x01820000, /* 01820000 //C 0f3e4  1219 mvzl	r8,0		; and reset byte counter */
0x0000f3e5, 0x0e610000, /* 0e610000 //C 0f3e5  1220 ld	r6,r1		; get next word */
0x0000f3e6, 0x06614608, /* 06614608 //C 0f3e6  1221 getbz	r6,r6,r8	; and pick first byte */
0x0000f3e7, 0x023b0000, /* 023b0000 //C 0f3e7  1224 sz	r3		; Prepare for comparing */
0x0000f3e8, 0x112d0020, /* 112d0020 //C 0f3e8  1225 Z1 or	r2,0x20		; if insensitive case */
0x0000f3e9, 0x023b0000, /* 023b0000 //C 0f3e9  1226 sz	r3 */
0x0000f3ea, 0x116d0020, /* 116d0020 //C 0f3ea  1227 Z1 or	r6,0x20 */
0x0000f3eb, 0x00280600, /* 00280600 //C 0f3eb  1228 cmp	r2,r6		; compare them */
0x0000f3ec, 0x21f2f406, /* 21f2f406 //C 0f3ec  1229 jnz	streq_no	; if differs: strings are not equal */
0x0000f3ed, 0x0e200000, /* 0e200000 //C 0f3ed  1231 ld	r2,r0		; Pick original (non-prepared) */
0x0000f3ee, 0x0e610000, /* 0e610000 //C 0f3ee  1232 ld	r6,r1		; chars to check EOS */
0x0000f3ef, 0x06214207, /* 06214207 //C 0f3ef  1233 getbz	r2,r2,r7 */
0x0000f3f0, 0x06614608, /* 06614608 //C 0f3f0  1234 getbz	r6,r6,r8 */
0x0000f3f1, 0x022b0000, /* 022b0000 //C 0f3f1  1235 sz	r2		; convert them to boolean */
0x0000f3f2, 0x21220001, /* 21220001 //C 0f3f2  1236 Z0 mvzl	r2,1		; values in R2,R6 */
0x0000f3f3, 0x11220000, /* 11220000 //C 0f3f3  1237 Z1 mvzl	r2,0		; and copy in R4,R5 */
0x0000f3f4, 0x00400200, /* 00400200 //C 0f3f4  1238 mov	r4,r2 */
0x0000f3f5, 0x026b0000, /* 026b0000 //C 0f3f5  1239 sz	r6 */
0x0000f3f6, 0x21620001, /* 21620001 //C 0f3f6  1240 Z0 mvzl	r6,1 */
0x0000f3f7, 0x11620000, /* 11620000 //C 0f3f7  1241 Z1 mvzl r6,0 */
0x0000f3f8, 0x00500600, /* 00500600 //C 0f3f8  1242 mov	r5,r6 */
0x0000f3f9, 0x004d0500, /* 004d0500 //C 0f3f9  1243 or	r4,r5		; if both are EOS: equal */
0x0000f3fa, 0x11f2f408, /* 11f2f408 //C 0f3fa  1244 jz	streq_yes */
0x0000f3fb, 0x002f0600, /* 002f0600 //C 0f3fb  1245 and 	r2,r6		; just one is EOS: not equal */
0x0000f3fc, 0x11f2f406, /* 11f2f406 //C 0f3fc  1246 jz	streq_no */
0x0000f3fd, 0x01740001, /* 01740001 //C 0f3fd  1249 inc	r7		; step byte count */
0x0000f3fe, 0x01780004, /* 01780004 //C 0f3fe  1250 cmp	r7,4		; if word overflows */
0x0000f3ff, 0x110a0001, /* 110a0001 //C 0f3ff  1251 Z plus	r0,1		; then step the pointer */
0x0000f400, 0x11720000, /* 11720000 //C 0f400  1252 Z mvzl	r7,0		; and reset the byte counter */
0x0000f401, 0x01840001, /* 01840001 //C 0f401  1254 inc	r8 */
0x0000f402, 0x01880004, /* 01880004 //C 0f402  1255 cmp	r8,4 */
0x0000f403, 0x111a0001, /* 111a0001 //C 0f403  1256 Z plus	r1,1 */
0x0000f404, 0x11820000, /* 11820000 //C 0f404  1257 Z mvzl	r8,0 */
0x0000f405, 0x01f2f3c5, /* 01f2f3c5 //C 0f405  1258 jmp	streq_cyc */
0x0000f406, 0x020d0000, /* 020d0000 //C 0f406  1261 clc			; False result */
0x0000f407, 0x01f2f409, /* 01f2f409 //C 0f407  1262 jmp	streq_ret */
0x0000f408, 0x020c0000, /* 020c0000 //C 0f408  1265 sec			; True result */
0x0000f409, 0x0e610000, /* 0e610000 //C 0f409  1268 ld	r6,r1		; forward R1 to EOS */
0x0000f40a, 0x026b0000, /* 026b0000 //C 0f40a  1269 sz	r6 */
0x0000f40b, 0x11f2f40e, /* 11f2f40e //C 0f40b  1270 jz	streq_ret_ret */
0x0000f40c, 0x01140001, /* 01140001 //C 0f40c  1271 inc	r1 */
0x0000f40d, 0x01f2f409, /* 01f2f409 //C 0f40d  1272 jmp	streq_ret */
0x0000f40e, 0x0f8d0000, /* 0f8d0000 //C 0f40e  1274 pop	r8 */
0x0000f40f, 0x0f7d0000, /* 0f7d0000 //C 0f40f  1275 pop	r7 */
0x0000f410, 0x0f6d0000, /* 0f6d0000 //C 0f410  1276 pop	r6 */
0x0000f411, 0x0f5d0000, /* 0f5d0000 //C 0f411  1277 pop	r5 */
0x0000f412, 0x0f4d0000, /* 0f4d0000 //C 0f412  1278 pop	r4 */
0x0000f413, 0x0f2d0000, /* 0f2d0000 //C 0f413  1279 pop	r2 */
0x0000f414, 0x0f0d0000, /* 0f0d0000 //C 0f414  1281 pop	r0 */
0x0000f415, 0x0ffd0000, /* 0ffd0000 //C 0f415  1282 pop	pc */
0x0000f416, 0x0ded0000, /* 0ded0000 //C 0f416  1290 push	lr */
0x0000f417, 0x0d1d0000, /* 0d1d0000 //C 0f417  1291 push	r1 */
0x0000f418, 0x0d3d0000, /* 0d3d0000 //C 0f418  1292 push	r3 */
0x0000f419, 0x01320001, /* 01320001 //C 0f419  1293 mvzl	r3,1 */
0x0000f41a, 0x0400f3bb, /* 0400f3bb //C 0f41a  1294 call	str_cmp_eq */
0x0000f41b, 0x0f3d0000, /* 0f3d0000 //C 0f41b  1295 pop	r3 */
0x0000f41c, 0x0f1d0000, /* 0f1d0000 //C 0f41c  1296 pop	r1 */
0x0000f41d, 0x0ffd0000, /* 0ffd0000 //C 0f41d  1297 pop	pc */
0x0000f41e, 0x0ded0000, /* 0ded0000 //C 0f41e  1305 push	lr */
0x0000f41f, 0x0d1d0000, /* 0d1d0000 //C 0f41f  1306 push	r1 */
0x0000f420, 0x0d3d0000, /* 0d3d0000 //C 0f420  1307 push	r3 */
0x0000f421, 0x01320000, /* 01320000 //C 0f421  1308 mvzl	r3,0 */
0x0000f422, 0x0400f3bb, /* 0400f3bb //C 0f422  1309 call	str_cmp_eq */
0x0000f423, 0x0f3d0000, /* 0f3d0000 //C 0f423  1310 pop	r3 */
0x0000f424, 0x0f1d0000, /* 0f1d0000 //C 0f424  1311 pop	r1 */
0x0000f425, 0x0ffd0000, /* 0ffd0000 //C 0f425  1312 pop	pc */
0x0000f426, 0x0108002f, /* 0108002f //C 0f426  1321 cmp	r0,'/' */
0x0000f427, 0xa1f2f43c, /* a1f2f43c //C 0f427  1322 LS jmp	hc2v_nok */
0x0000f428, 0x01080039, /* 01080039 //C 0f428  1323 cmp	r0,'9' */
0x0000f429, 0xa1f2f439, /* a1f2f439 //C 0f429  1324 LS jmp	hc2v_0_9 */
0x0000f42a, 0x01080040, /* 01080040 //C 0f42a  1325 cmp	r0,'@' */
0x0000f42b, 0xa1f2f43c, /* a1f2f43c //C 0f42b  1326 LS jmp	hc2v_nok */
0x0000f42c, 0x01080046, /* 01080046 //C 0f42c  1327 cmp	r0,'F' */
0x0000f42d, 0xa1f2f436, /* a1f2f436 //C 0f42d  1328 LS jmp	hc2v_A_F */
0x0000f42e, 0x01080060, /* 01080060 //C 0f42e  1329 cmp	r0,'`' */
0x0000f42f, 0xa1f2f43c, /* a1f2f43c //C 0f42f  1330 LS jmp	hc2v_nok */
0x0000f430, 0x01080066, /* 01080066 //C 0f430  1331 cmp	r0,'f' */
0x0000f431, 0xa1f2f433, /* a1f2f433 //C 0f431  1332 LS jmp	hc2v_a_f */
0x0000f432, 0x01f2f43c, /* 01f2f43c //C 0f432  1333 jmp	hc2v_nok */
0x0000f433, 0x0104000a, /* 0104000a //C 0f433  1335 add	r0,10 */
0x0000f434, 0x01060061, /* 01060061 //C 0f434  1336 sub	r0,'a' */
0x0000f435, 0x01f2f43a, /* 01f2f43a //C 0f435  1337 jmp	hc2v_ok */
0x0000f436, 0x0104000a, /* 0104000a //C 0f436  1339 add	r0,10 */
0x0000f437, 0x01060041, /* 01060041 //C 0f437  1340 sub	r0,'A' */
0x0000f438, 0x01f2f43a, /* 01f2f43a //C 0f438  1341 jmp	hc2v_ok */
0x0000f439, 0x01060030, /* 01060030 //C 0f439  1343 sub	r0,'0' */
0x0000f43a, 0x020c0000, /* 020c0000 //C 0f43a  1345 sec */
0x0000f43b, 0x00f00e00, /* 00f00e00 //C 0f43b  1346 ret */
0x0000f43c, 0x020d0000, /* 020d0000 //C 0f43c  1348 clc */
0x0000f43d, 0x00f00e00, /* 00f00e00 //C 0f43d  1349 ret */
0x0000f43e, 0x0d1d0000, /* 0d1d0000 //C 0f43e  1353 push	r1 */
0x0000f43f, 0x010f000f, /* 010f000f //C 0f43f  1354 and	r0,0xf */
0x0000f440, 0x0112f444, /* 0112f444 //C 0f440  1355 mvzl	r1,v2hc_table */
0x0000f441, 0x0a010000, /* 0a010000 //C 0f441  1356 ld	r0,r1,r0 */
0x0000f442, 0x0f1d0000, /* 0f1d0000 //C 0f442  1357 pop	r1 */
0x0000f443, 0x00f00e00, /* 00f00e00 //C 0f443  1358 ret */
0x0000f444, 0x00000030, /* 00000030 //C 0f444  1359 db	48 */
0x0000f445, 0x00000031, /* 00000031 //C 0f445  1359 db	49 */
0x0000f446, 0x00000032, /* 00000032 //C 0f446  1359 db	50 */
0x0000f447, 0x00000033, /* 00000033 //C 0f447  1359 db	51 */
0x0000f448, 0x00000034, /* 00000034 //C 0f448  1359 db	52 */
0x0000f449, 0x00000035, /* 00000035 //C 0f449  1359 db	53 */
0x0000f44a, 0x00000036, /* 00000036 //C 0f44a  1359 db	54 */
0x0000f44b, 0x00000037, /* 00000037 //C 0f44b  1359 db	55 */
0x0000f44c, 0x00000038, /* 00000038 //C 0f44c  1359 db	56 */
0x0000f44d, 0x00000039, /* 00000039 //C 0f44d  1359 db	57 */
0x0000f44e, 0x00000041, /* 00000041 //C 0f44e  1359 db	65 */
0x0000f44f, 0x00000042, /* 00000042 //C 0f44f  1359 db	66 */
0x0000f450, 0x00000043, /* 00000043 //C 0f450  1359 db	67 */
0x0000f451, 0x00000044, /* 00000044 //C 0f451  1359 db	68 */
0x0000f452, 0x00000045, /* 00000045 //C 0f452  1359 db	69 */
0x0000f453, 0x00000046, /* 00000046 //C 0f453  1359 db	70 */
0x0000f454, 0x00000000, /* 00000000 //C 0f454  1359 db */
0x0000f455, 0x0ded0000, /* 0ded0000 //C 0f455  1362 push	lr */
0x0000f456, 0x0400f43e, /* 0400f43e //C 0f456  1363 call	value2Hexchar */
0x0000f457, 0x010d0020, /* 010d0020 //C 0f457  1364 or	r0,0x20 */
0x0000f458, 0x0ffd0000, /* 0ffd0000 //C 0f458  1365 pop	pc */
0x0000f459, 0x0ded0000, /* 0ded0000 //C 0f459  1374 push	lr */
0x0000f45a, 0x0d2d0000, /* 0d2d0000 //C 0f45a  1375 push	r2 */
0x0000f45b, 0x0d3d0000, /* 0d3d0000 //C 0f45b  1376 push	r3 */
0x0000f45c, 0x0d4d0000, /* 0d4d0000 //C 0f45c  1377 push	r4 */
0x0000f45d, 0x0d5d0000, /* 0d5d0000 //C 0f45d  1378 push	r5 */
0x0000f45e, 0x01120000, /* 01120000 //C 0f45e  1379 mvzl	r1,0		; return value */
0x0000f45f, 0x01320000, /* 01320000 //C 0f45f  1380 mvzl	r3,0		; index */
0x0000f460, 0x01520000, /* 01520000 //C 0f460  1382 mvzl	r5,0 */
0x0000f461, 0x0b438000, /* 0b438000 //C 0f461  1383 ld	r4,r3+,r0	; pick a char */
0x0000f462, 0x024b0000, /* 024b0000 //C 0f462  1384 sz	r4		; check eof string */
0x0000f463, 0x11f2f47f, /* 11f2f47f //C 0f463  1385 jz	htoi_eos */
0x0000f464, 0x06214405, /* 06214405 //C 0f464  1387 getbz	r2,r4,r5 */
0x0000f465, 0x022b0000, /* 022b0000 //C 0f465  1388 sz	r2 */
0x0000f466, 0x11f2f460, /* 11f2f460 //C 0f466  1389 jz	htoi_cyc */
0x0000f467, 0x0128002e, /* 0128002e //C 0f467  1390 cmp	r2,'.'		; skip separators */
0x0000f468, 0x11f2f479, /* 11f2f479 //C 0f468  1391 jz	htoi_next */
0x0000f469, 0x0128005f, /* 0128005f //C 0f469  1392 cmp	r2,'_' */
0x0000f46a, 0x11f2f479, /* 11f2f479 //C 0f46a  1393 jz	htoi_next */
0x0000f46b, 0x0128002d, /* 0128002d //C 0f46b  1394 cmp	r2,'-' */
0x0000f46c, 0x11f2f479, /* 11f2f479 //C 0f46c  1395 jz	htoi_next */
0x0000f46d, 0x0d0d0000, /* 0d0d0000 //C 0f46d  1396 push	r0 */
0x0000f46e, 0x00000200, /* 00000200 //C 0f46e  1397 mov	r0,r2 */
0x0000f46f, 0x0400f426, /* 0400f426 //C 0f46f  1398 call	hexchar2value */
0x0000f470, 0x00200000, /* 00200000 //C 0f470  1399 mov	r2,r0 */
0x0000f471, 0x0f0d0000, /* 0f0d0000 //C 0f471  1400 pop	r0 */
0x0000f472, 0x41f2f47d, /* 41f2f47d //C 0f472  1401 C0 jmp	htoi_nok */
0x0000f473, 0x02180000, /* 02180000 //C 0f473  1402 shl	r1 */
0x0000f474, 0x02180000, /* 02180000 //C 0f474  1403 shl	r1 */
0x0000f475, 0x02180000, /* 02180000 //C 0f475  1404 shl	r1 */
0x0000f476, 0x02180000, /* 02180000 //C 0f476  1405 shl	r1 */
0x0000f477, 0x012f000f, /* 012f000f //C 0f477  1406 and	r2,0xf */
0x0000f478, 0x001d0200, /* 001d0200 //C 0f478  1407 or	r1,r2 */
0x0000f479, 0x01540001, /* 01540001 //C 0f479  1409 inc	r5 */
0x0000f47a, 0x01580004, /* 01580004 //C 0f47a  1410 cmp	r5,4 */
0x0000f47b, 0x11f2f460, /* 11f2f460 //C 0f47b  1411 jz	htoi_cyc */
0x0000f47c, 0x01f2f464, /* 01f2f464 //C 0f47c  1412 jmp	htoi_byte */
0x0000f47d, 0x020d0000, /* 020d0000 //C 0f47d  1414 clc */
0x0000f47e, 0x01f2f482, /* 01f2f482 //C 0f47e  1415 jmp	htoi_ret */
0x0000f47f, 0x01380001, /* 01380001 //C 0f47f  1417 cmp	r3,1 */
0x0000f480, 0x920d0000, /* 920d0000 //C 0f480  1418 HI clc */
0x0000f481, 0xa20c0000, /* a20c0000 //C 0f481  1419 LS sec */
0x0000f482, 0x0f5d0000, /* 0f5d0000 //C 0f482  1421 pop	r5 */
0x0000f483, 0x0f4d0000, /* 0f4d0000 //C 0f483  1422 pop	r4 */
0x0000f484, 0x0f3d0000, /* 0f3d0000 //C 0f484  1423 pop	r3 */
0x0000f485, 0x0f2d0000, /* 0f2d0000 //C 0f485  1424 pop	r2 */
0x0000f486, 0x0ffd0000, /* 0ffd0000 //C 0f486  1425 pop	pc */
0x0000f487, 0x0d0d0000, /* 0d0d0000 //C 0f487  1437 push	r0 */
0x0000f488, 0x0700ff42, /* 0700ff42 //C 0f488  1438 ld	r0,UART_RSTAT */
0x0000f489, 0x010c0001, /* 010c0001 //C 0f489  1440 test	r0,1		; check if queue is not empty */
0x0000f48a, 0x020d0000, /* 020d0000 //C 0f48a  1441 clc */
0x0000f48b, 0x220c0000, /* 220c0000 //C 0f48b  1442 Z0 sec */
0x0000f48c, 0x0f0d0000, /* 0f0d0000 //C 0f48c  1443 pop	r0 */
0x0000f48d, 0x30f00e00, /* 30f00e00 //C 0f48d  1444 C1 ret */
0x0000f48e, 0x00f00e00, /* 00f00e00 //C 0f48e  1445 ret */
0x0000f48f, 0x0700ff40, /* 0700ff40 //C 0f48f  1490 ld	r0,UART_DR */
0x0000f490, 0x0600ff46, /* 0600ff46 //C 0f490  1491 st	r0,UART_IRA */
0x0000f491, 0x00f00e00, /* 00f00e00 //C 0f491  1492 ret */
0x0000f492, 0x0400f487, /* 0400f487 //C 0f492  1524 call	check_uart */
0x0000f493, 0x41f2f492, /* 41f2f492 //C 0f493  1525 C0 jmp	getchar */
0x0000f494, 0x0400f48f, /* 0400f48f //C 0f494  1526 call	read */
0x0000f495, 0x00f00e00, /* 00f00e00 //C 0f495  1527 ret */
0x0000f496, 0x0d9d0000, /* 0d9d0000 //C 0f496  1535 push	r9 */
0x0000f497, 0x0790ff43, /* 0790ff43 //C 0f497  1542 ld	r9,UART_TSTAT */
0x0000f498, 0x019c0001, /* 019c0001 //C 0f498  1543 test	r9,1 */
0x0000f499, 0x11f2f497, /* 11f2f497 //C 0f499  1544 jz	wait_tc */
0x0000f49a, 0x0600ff40, /* 0600ff40 //C 0f49a  1545 st	r0,UART_DR */
0x0000f49b, 0x0f9d0000, /* 0f9d0000 //C 0f49b  1547 pop	r9 */
0x0000f49c, 0x00f00e00, /* 00f00e00 //C 0f49c  1548 ret */
0x0000f49d, 0x0ded0000, /* 0ded0000 //C 0f49d  1556 push	lr */
0x0000f49e, 0x0d0d0000, /* 0d0d0000 //C 0f49e  1557 push	r0 */
0x0000f49f, 0x0d3d0000, /* 0d3d0000 //C 0f49f  1558 push	r3 */
0x0000f4a0, 0x0d4d0000, /* 0d4d0000 //C 0f4a0  1559 push	r4 */
0x0000f4a1, 0x0d2d0000, /* 0d2d0000 //C 0f4a1  1560 push	r2 */
0x0000f4a2, 0x0d1d0000, /* 0d1d0000 //C 0f4a2  1561 push	r1 */
0x0000f4a3, 0x0d5d0000, /* 0d5d0000 //C 0f4a3  1562 push	r5 */
0x0000f4a4, 0x01420000, /* 01420000 //C 0f4a4  1564 mvzl	r4,0 */
0x0000f4a5, 0x020b0000, /* 020b0000 //C 0f4a5  1565 sz	r0 */
0x0000f4a6, 0x1102f6bd, /* 1102f6bd //C 0f4a6  1566 Z1 mvzl	r0,null_str */
0x0000f4a7, 0x00200000, /* 00200000 //C 0f4a7  1567 mov	r2,r0 */
0x0000f4a8, 0x0b148200, /* 0b148200 //C 0f4a8  1569 ld	r1,r4+,r2 */
0x0000f4a9, 0x021b0000, /* 021b0000 //C 0f4a9  1570 sz	r1 */
0x0000f4aa, 0x01520000, /* 01520000 //C 0f4aa  1571 mvzl	r5,0 */
0x0000f4ab, 0x11f2f4b4, /* 11f2f4b4 //C 0f4ab  1572 jz	prints_done */
0x0000f4ac, 0x06014105, /* 06014105 //C 0f4ac  1574 getbz	r0,r1,r5 */
0x0000f4ad, 0x020b0000, /* 020b0000 //C 0f4ad  1575 sz	r0 */
0x0000f4ae, 0x2400f496, /* 2400f496 //C 0f4ae  1576 NZ call	putchar */
0x0000f4af, 0x11f2f4a8, /* 11f2f4a8 //C 0f4af  1577 jz 	prints_go */
0x0000f4b0, 0x01540001, /* 01540001 //C 0f4b0  1578 inc	r5 */
0x0000f4b1, 0x01580004, /* 01580004 //C 0f4b1  1579 cmp	r5,4 */
0x0000f4b2, 0x21f2f4ac, /* 21f2f4ac //C 0f4b2  1580 jnz	prints_byte */
0x0000f4b3, 0x01f2f4a8, /* 01f2f4a8 //C 0f4b3  1581 jmp	prints_go */
0x0000f4b4, 0x0f5d0000, /* 0f5d0000 //C 0f4b4  1584 pop	r5 */
0x0000f4b5, 0x0f1d0000, /* 0f1d0000 //C 0f4b5  1585 pop	r1 */
0x0000f4b6, 0x0f2d0000, /* 0f2d0000 //C 0f4b6  1586 pop	r2 */
0x0000f4b7, 0x0f4d0000, /* 0f4d0000 //C 0f4b7  1587 pop	r4 */
0x0000f4b8, 0x0f3d0000, /* 0f3d0000 //C 0f4b8  1588 pop	r3 */
0x0000f4b9, 0x0f0d0000, /* 0f0d0000 //C 0f4b9  1589 pop	r0 */
0x0000f4ba, 0x0ffd0000, /* 0ffd0000 //C 0f4ba  1590 pop	pc */
0x0000f4bb, 0x00000000, /* 00000000 //C 0f4bb  1598 dd	0 */
0x0000f4bc, 0x0ded0000, /* 0ded0000 //C 0f4bc  1601 push	lr */
0x0000f4bd, 0x0d0d0000, /* 0d0d0000 //C 0f4bd  1602 push	r0 */
0x0000f4be, 0x0d1d0000, /* 0d1d0000 //C 0f4be  1603 push	r1 */
0x0000f4bf, 0x0d2d0000, /* 0d2d0000 //C 0f4bf  1604 push	r2 */
0x0000f4c0, 0x0d3d0000, /* 0d3d0000 //C 0f4c0  1605 push	r3 */
0x0000f4c1, 0x00100e00, /* 00100e00 //C 0f4c1  1606 mov	r1,lr */
0x0000f4c2, 0x0e210000, /* 0e210000 //C 0f4c2  1608 ld	r2,r1 */
0x0000f4c3, 0x01140001, /* 01140001 //C 0f4c3  1609 inc	r1 */
0x0000f4c4, 0x022b0000, /* 022b0000 //C 0f4c4  1610 sz	r2 */
0x0000f4c5, 0x11f2f4cf, /* 11f2f4cf //C 0f4c5  1611 jz	pes_done */
0x0000f4c6, 0x01320000, /* 01320000 //C 0f4c6  1612 mvzl	r3,0 */
0x0000f4c7, 0x06014203, /* 06014203 //C 0f4c7  1614 getbz	r0,r2,r3 */
0x0000f4c8, 0x020b0000, /* 020b0000 //C 0f4c8  1615 sz	r0 */
0x0000f4c9, 0x2400f496, /* 2400f496 //C 0f4c9  1616 NZ call	putchar */
0x0000f4ca, 0x11f2f4c2, /* 11f2f4c2 //C 0f4ca  1617 jz	pes_next */
0x0000f4cb, 0x01340001, /* 01340001 //C 0f4cb  1618 inc	r3 */
0x0000f4cc, 0x01380004, /* 01380004 //C 0f4cc  1619 cmp	r3,4 */
0x0000f4cd, 0x21f2f4c7, /* 21f2f4c7 //C 0f4cd  1620 jnz	pes_byte */
0x0000f4ce, 0x01f2f4c2, /* 01f2f4c2 //C 0f4ce  1621 jmp	pes_next */
0x0000f4cf, 0x0610f4bb, /* 0610f4bb //C 0f4cf  1623 st	r1,pes_ret_to */
0x0000f4d0, 0x0f3d0000, /* 0f3d0000 //C 0f4d0  1624 pop	r3 */
0x0000f4d1, 0x0f2d0000, /* 0f2d0000 //C 0f4d1  1625 pop	r2 */
0x0000f4d2, 0x0f1d0000, /* 0f1d0000 //C 0f4d2  1626 pop	r1 */
0x0000f4d3, 0x0f0d0000, /* 0f0d0000 //C 0f4d3  1627 pop	r0 */
0x0000f4d4, 0x0fed0000, /* 0fed0000 //C 0f4d4  1628 pop	lr */
0x0000f4d5, 0x07f0f4bb, /* 07f0f4bb //C 0f4d5  1629 ld	pc,pes_ret_to */
0x0000f4d6, 0x0ded0000, /* 0ded0000 //C 0f4d6  1637 push	lr */
0x0000f4d7, 0x0400f49d, /* 0400f49d //C 0f4d7  1638 call	prints */
0x0000f4d8, 0x0d0d0000, /* 0d0d0000 //C 0f4d8  1639 push	r0 */
0x0000f4d9, 0x0102000a, /* 0102000a //C 0f4d9  1640 mvzl	r0,LF */
0x0000f4da, 0x0400f496, /* 0400f496 //C 0f4da  1641 call	putchar */
0x0000f4db, 0x0f0d0000, /* 0f0d0000 //C 0f4db  1642 pop	r0 */
0x0000f4dc, 0x0ffd0000, /* 0ffd0000 //C 0f4dc  1643 pop	pc */
0x0000f4dd, 0x0ded0000, /* 0ded0000 //C 0f4dd  1651 push	lr */
0x0000f4de, 0x0d0d0000, /* 0d0d0000 //C 0f4de  1652 push	r0 */
0x0000f4df, 0x0d1d0000, /* 0d1d0000 //C 0f4df  1653 push	r1 */
0x0000f4e0, 0x0d2d0000, /* 0d2d0000 //C 0f4e0  1654 push	r2 */
0x0000f4e1, 0x0d3d0000, /* 0d3d0000 //C 0f4e1  1655 push	r3 */
0x0000f4e2, 0x0d4d0000, /* 0d4d0000 //C 0f4e2  1656 push	r4 */
0x0000f4e3, 0x00300000, /* 00300000 //C 0f4e3  1657 mov	r3,r0 */
0x0000f4e4, 0x01220000, /* 01220000 //C 0f4e4  1658 mvzl	r2,0 */
0x0000f4e5, 0x01420001, /* 01420001 //C 0f4e5  1659 mvzl	r4,1 */
0x0000f4e6, 0x01020000, /* 01020000 //C 0f4e6  1662 mvzl	r0,0 */
0x0000f4e7, 0x02380000, /* 02380000 //C 0f4e7  1663 shl	r3 */
0x0000f4e8, 0x02070000, /* 02070000 //C 0f4e8  1664 rol	r0 */
0x0000f4e9, 0x02380000, /* 02380000 //C 0f4e9  1665 shl	r3 */
0x0000f4ea, 0x02070000, /* 02070000 //C 0f4ea  1666 rol	r0 */
0x0000f4eb, 0x02380000, /* 02380000 //C 0f4eb  1667 shl	r3 */
0x0000f4ec, 0x02070000, /* 02070000 //C 0f4ec  1668 rol	r0 */
0x0000f4ed, 0x02380000, /* 02380000 //C 0f4ed  1669 shl	r3 */
0x0000f4ee, 0x02070000, /* 02070000 //C 0f4ee  1670 rol	r0 */
0x0000f4ef, 0x0400f43e, /* 0400f43e //C 0f4ef  1671 call	value2Hexchar */
0x0000f4f0, 0x0400f496, /* 0400f496 //C 0f4f0  1672 call	putchar */
0x0000f4f1, 0x01240001, /* 01240001 //C 0f4f1  1673 add	r2,1 */
0x0000f4f2, 0x01280008, /* 01280008 //C 0f4f2  1674 cmp	r2,8 */
0x0000f4f3, 0x11f2f4fd, /* 11f2f4fd //C 0f4f3  1675 jz	print_vhex_ret */
0x0000f4f4, 0x021b0000, /* 021b0000 //C 0f4f4  1676 sz	r1 */
0x0000f4f5, 0x11f2f4fb, /* 11f2f4fb //C 0f4f5  1677 jz	print_vhex_nosep */
0x0000f4f6, 0x00480100, /* 00480100 //C 0f4f6  1678 cmp	r4,r1 */
0x0000f4f7, 0x21f2f4fb, /* 21f2f4fb //C 0f4f7  1679 jnz	print_vhex_nosep */
0x0000f4f8, 0x0102005f, /* 0102005f //C 0f4f8  1680 mvzl	r0,'_' */
0x0000f4f9, 0x0400f496, /* 0400f496 //C 0f4f9  1681 call	putchar */
0x0000f4fa, 0x01420000, /* 01420000 //C 0f4fa  1682 mvzl	r4,0 */
0x0000f4fb, 0x01440001, /* 01440001 //C 0f4fb  1684 add	r4,1 */
0x0000f4fc, 0x01f2f4e6, /* 01f2f4e6 //C 0f4fc  1685 jmp	print_vhex_cyc */
0x0000f4fd, 0x0f4d0000, /* 0f4d0000 //C 0f4fd  1687 pop	r4 */
0x0000f4fe, 0x0f3d0000, /* 0f3d0000 //C 0f4fe  1688 pop	r3 */
0x0000f4ff, 0x0f2d0000, /* 0f2d0000 //C 0f4ff  1689 pop	r2 */
0x0000f500, 0x0f1d0000, /* 0f1d0000 //C 0f500  1690 pop	r1 */
0x0000f501, 0x0f0d0000, /* 0f0d0000 //C 0f501  1691 pop	r0 */
0x0000f502, 0x0ffd0000, /* 0ffd0000 //C 0f502  1692 pop	pc */
0x0000f503, 0x0ded0000, /* 0ded0000 //C 0f503  1700 push	lr */
0x0000f504, 0x0d4d0000, /* 0d4d0000 //C 0f504  1701 push	r4 */
0x0000f505, 0x021b0000, /* 021b0000 //C 0f505  1703 sz	r1 */
0x0000f506, 0x21f2f50a, /* 21f2f50a //C 0f506  1704 NZ jmp	div_dok */
0x0000f507, 0x00200000, /* 00200000 //C 0f507  1705 mov	r2,r0		; div by zero */
0x0000f508, 0x01320000, /* 01320000 //C 0f508  1706 mvzl	r3,0 */
0x0000f509, 0x01f2f51b, /* 01f2f51b //C 0f509  1707 jmp	div_ret */
0x0000f50a, 0x01220000, /* 01220000 //C 0f50a  1709 mvzl	r2,0		; Q= 0 */
0x0000f50b, 0x01320000, /* 01320000 //C 0f50b  1710 mvzl	r3,0		; R= 0 */
0x0000f50c, 0x01418000, /* 01418000 //C 0f50c  1711 mvh	r4,0x80000000	; m= 1<<31 */
0x0000f50d, 0x01400000, /* 01400000 //C 0f50d  1712 mvl	r4,0x80000000 */
0x0000f50e, 0x024b0000, /* 024b0000 //C 0f50e  1714 sz	r4 */
0x0000f50f, 0x11f2f51b, /* 11f2f51b //C 0f50f  1715 Z jmp	div_ret */
0x0000f510, 0x02380000, /* 02380000 //C 0f510  1716 shl	r3		; r<<= 1 */
0x0000f511, 0x000c0400, /* 000c0400 //C 0f511  1717 test	r0,r4		; if (n&m) */
0x0000f512, 0x213d0001, /* 213d0001 //C 0f512  1718 NZ or	r3,1		; r|= 1 */
0x0000f513, 0x00380100, /* 00380100 //C 0f513  1719 cmp	r3,r1		; if (r>=d) */
0x0000f514, 0x41f2f519, /* 41f2f519 //C 0f514  1720 LO jmp	div_cyc_next */
0x0000f515, 0x00360100, /* 00360100 //C 0f515  1721 sub	r3,r1		;r-= d */
0x0000f516, 0x002d0400, /* 002d0400 //C 0f516  1722 or	r2,r4		;q|= m */
0x0000f517, 0x01f2f519, /* 01f2f519 //C 0f517  1723 jmp	div_cyc_next */
0x0000f518, 0x01f2f50e, /* 01f2f50e //C 0f518  1724 jmp	div_cyc */
0x0000f519, 0x02490000, /* 02490000 //C 0f519  1726 shr	r4		; m>>= 1 */
0x0000f51a, 0x01f2f50e, /* 01f2f50e //C 0f51a  1727 jmp	div_cyc */
0x0000f51b, 0x0f4d0000, /* 0f4d0000 //C 0f51b  1729 pop	r4 */
0x0000f51c, 0x0ffd0000, /* 0ffd0000 //C 0f51c  1730 pop	pc */
0x0000f51d, 0x0ded0000, /* 0ded0000 //C 0f51d  1738 push	lr */
0x0000f51e, 0x0d0d0000, /* 0d0d0000 //C 0f51e  1739 push	r0 */
0x0000f51f, 0x0d1d0000, /* 0d1d0000 //C 0f51f  1740 push	r1 */
0x0000f520, 0x0d2d0000, /* 0d2d0000 //C 0f520  1741 push	r2 */
0x0000f521, 0x0d3d0000, /* 0d3d0000 //C 0f521  1742 push	r3 */
0x0000f522, 0x0dad0000, /* 0dad0000 //C 0f522  1743 push	r10 */
0x0000f523, 0x0dbd0000, /* 0dbd0000 //C 0f523  1744 push	r11 */
0x0000f524, 0x0dcd0000, /* 0dcd0000 //C 0f524  1745 push	r12 */
0x0000f525, 0x01c2f545, /* 01c2f545 //C 0f525  1747 mvzl	r12,itoa_buffer	; pointer to output buffer */
0x0000f526, 0x01b2f550, /* 01b2f550 //C 0f526  1748 mvzl	r11,itoa_divs	; pointer to dividers */
0x0000f527, 0x01a20000, /* 01a20000 //C 0f527  1749 mvzl	r10,0		; bool: first non-zero char found */
0x0000f528, 0x0e1b0000, /* 0e1b0000 //C 0f528  1751 ld	r1,r11		; get next divider */
0x0000f529, 0x021b0000, /* 021b0000 //C 0f529  1752 sz	r1		; if 0, then */
0x0000f52a, 0x11f2f53d, /* 11f2f53d //C 0f52a  1753 jz	itoa_ret	; finish */
0x0000f52b, 0x01180001, /* 01180001 //C 0f52b  1754 cmp	r1,1		; last divider? */
0x0000f52c, 0x11a20001, /* 11a20001 //C 0f52c  1755 EQ mvzl	r10,1		; always print last char */
0x0000f52d, 0x0400f503, /* 0400f503 //C 0f52d  1756 call	div		; R2,R3= R0/R1 */
0x0000f52e, 0x022b0000, /* 022b0000 //C 0f52e  1757 sz	r2		; is the result zero? */
0x0000f52f, 0x11f2f53a, /* 11f2f53a //C 0f52f  1758 jz	itoa_f0 */
0x0000f530, 0x01a20001, /* 01a20001 //C 0f530  1760 mvzl	r10,1		; non-zero: start to print */
0x0000f531, 0x00000200, /* 00000200 //C 0f531  1762 mov	r0,r2		; convert result to ASCII char */
0x0000f532, 0x0400f455, /* 0400f455 //C 0f532  1763 call	value2hexchar */
0x0000f533, 0x0c0c0000, /* 0c0c0000 //C 0f533  1764 st	r0,r12		; and store it in buffer */
0x0000f534, 0x01c40001, /* 01c40001 //C 0f534  1765 inc	r12		; inc buf ptr */
0x0000f535, 0x01020000, /* 01020000 //C 0f535  1766 mvzl	r0,0		; put 0 after last char */
0x0000f536, 0x0c0c0000, /* 0c0c0000 //C 0f536  1767 st	r0,r12 */
0x0000f537, 0x00000300, /* 00000300 //C 0f537  1769 mov	r0,r3		; continue with the reminder */
0x0000f538, 0x01b40001, /* 01b40001 //C 0f538  1770 inc	r11		; and next divider */
0x0000f539, 0x01f2f528, /* 01f2f528 //C 0f539  1771 jmp	itoa_cyc */
0x0000f53a, 0x02ab0000, /* 02ab0000 //C 0f53a  1773 sz	r10		; just zeros so far? */
0x0000f53b, 0x21f2f531, /* 21f2f531 //C 0f53b  1774 jnz	itoa_store	; no, print */
0x0000f53c, 0x01f2f537, /* 01f2f537 //C 0f53c  1775 jmp	itoa_next */
0x0000f53d, 0x0fcd0000, /* 0fcd0000 //C 0f53d  1777 pop	r12 */
0x0000f53e, 0x0fbd0000, /* 0fbd0000 //C 0f53e  1778 pop	r11 */
0x0000f53f, 0x0fad0000, /* 0fad0000 //C 0f53f  1779 pop	r10 */
0x0000f540, 0x0f3d0000, /* 0f3d0000 //C 0f540  1780 pop	r3 */
0x0000f541, 0x0f2d0000, /* 0f2d0000 //C 0f541  1781 pop	r2 */
0x0000f542, 0x0f1d0000, /* 0f1d0000 //C 0f542  1782 pop	r1 */
0x0000f543, 0x0f0d0000, /* 0f0d0000 //C 0f543  1783 pop	r0 */
0x0000f544, 0x0ffd0000, /* 0ffd0000 //C 0f544  1784 pop	pc */
0x0000f545, 0x00000000, /* 00000000 //C 0f545  1786 itoa_buffer:	ds	11 */
0x0000f550, 0x3b9aca00, /* 3b9aca00 //C 0f550  1788 dd	1000000000 */
0x0000f551, 0x05f5e100, /* 05f5e100 //C 0f551  1789 dd	100000000 */
0x0000f552, 0x00989680, /* 00989680 //C 0f552  1790 dd	10000000 */
0x0000f553, 0x000f4240, /* 000f4240 //C 0f553  1791 dd	1000000 */
0x0000f554, 0x000186a0, /* 000186a0 //C 0f554  1792 dd	100000 */
0x0000f555, 0x00002710, /* 00002710 //C 0f555  1793 dd	10000 */
0x0000f556, 0x000003e8, /* 000003e8 //C 0f556  1794 dd	1000 */
0x0000f557, 0x00000064, /* 00000064 //C 0f557  1795 dd	100 */
0x0000f558, 0x0000000a, /* 0000000a //C 0f558  1796 dd	10 */
0x0000f559, 0x00000001, /* 00000001 //C 0f559  1797 dd	1 */
0x0000f55a, 0x00000000, /* 00000000 //C 0f55a  1798 dd	0 */
0x0000f55b, 0x0ded0000, /* 0ded0000 //C 0f55b  1805 push	lr */
0x0000f55c, 0x0d1d0000, /* 0d1d0000 //C 0f55c  1806 push	r1 */
0x0000f55d, 0x0d2d0000, /* 0d2d0000 //C 0f55d  1807 push	r2 */
0x0000f55e, 0x0108e0ff, /* 0108e0ff //C 0f55e  1808 cmp	r0,99999999 */
0x0000f55f, 0x91f2f56e, /* 91f2f56e //C 0f55f  1809 UGT jmp	itobcd_bad */
0x0000f560, 0x0400f51d, /* 0400f51d //C 0f560  1810 call	itoa */
0x0000f561, 0x01020000, /* 01020000 //C 0f561  1811 mvzl	r0,0 */
0x0000f562, 0x0112f545, /* 0112f545 //C 0f562  1812 mvzl	r1,itoa_buffer */
0x0000f563, 0x0e210000, /* 0e210000 //C 0f563  1814 ld	r2,r1 */
0x0000f564, 0x022b0000, /* 022b0000 //C 0f564  1815 sz	r2 */
0x0000f565, 0x11f2f56f, /* 11f2f56f //C 0f565  1816 jz	itobcd_ret */
0x0000f566, 0x01260030, /* 01260030 //C 0f566  1817 sub	r2,'0' */
0x0000f567, 0x02080000, /* 02080000 //C 0f567  1818 shl	r0 */
0x0000f568, 0x02080000, /* 02080000 //C 0f568  1819 shl	r0 */
0x0000f569, 0x02080000, /* 02080000 //C 0f569  1820 shl	r0 */
0x0000f56a, 0x02080000, /* 02080000 //C 0f56a  1821 shl	r0 */
0x0000f56b, 0x000d0200, /* 000d0200 //C 0f56b  1822 or	r0,r2 */
0x0000f56c, 0x01140001, /* 01140001 //C 0f56c  1823 inc	r1 */
0x0000f56d, 0x01f2f563, /* 01f2f563 //C 0f56d  1824 jmp	itobcd_cyc */
0x0000f56e, 0x01020000, /* 01020000 //C 0f56e  1826 mvzl	r0,0 */
0x0000f56f, 0x0f2d0000, /* 0f2d0000 //C 0f56f  1828 pop	r2 */
0x0000f570, 0x0f1d0000, /* 0f1d0000 //C 0f570  1829 pop	r1 */
0x0000f571, 0x0ffd0000, /* 0ffd0000 //C 0f571  1830 pop	pc */
0x0000f572, 0x0ded0000, /* 0ded0000 //C 0f572  1837 push	lr */
0x0000f573, 0x0400f51d, /* 0400f51d //C 0f573  1838 call	itoa */
0x0000f574, 0x0102f545, /* 0102f545 //C 0f574  1839 mvzl	r0,itoa_buffer */
0x0000f575, 0x0400f49d, /* 0400f49d //C 0f575  1840 call	prints */
0x0000f576, 0x0ffd0000, /* 0ffd0000 //C 0f576  1841 pop	pc */
0x0000f577, 0x0ded0000, /* 0ded0000 //C 0f577  1850 push	lr */
0x0000f578, 0x0d0d0000, /* 0d0d0000 //C 0f578  1851 push	r0 */
0x0000f579, 0x0d1d0000, /* 0d1d0000 //C 0f579  1852 push	r1 */
0x0000f57a, 0x0d3d0000, /* 0d3d0000 //C 0f57a  1853 push	r3 */
0x0000f57b, 0x0610f673, /* 0610f673 //C 0f57b  1855 st	r1,reg1 */
0x0000f57c, 0x0620f674, /* 0620f674 //C 0f57c  1856 st	r2,reg2 */
0x0000f57d, 0x0630f675, /* 0630f675 //C 0f57d  1857 st	r3,reg3 */
0x0000f57e, 0x0640f676, /* 0640f676 //C 0f57e  1858 st	r4,reg4 */
0x0000f57f, 0x0650f677, /* 0650f677 //C 0f57f  1859 st	r5,reg5 */
0x0000f580, 0x0660f678, /* 0660f678 //C 0f580  1860 st	r6,reg6 */
0x0000f581, 0x0670f679, /* 0670f679 //C 0f581  1861 st	r7,reg7 */
0x0000f582, 0x0680f67a, /* 0680f67a //C 0f582  1862 st	r8,reg8 */
0x0000f583, 0x0690f67b, /* 0690f67b //C 0f583  1863 st	r9,reg9 */
0x0000f584, 0x06a0f67c, /* 06a0f67c //C 0f584  1864 st	r10,reg10 */
0x0000f585, 0x06b0f67d, /* 06b0f67d //C 0f585  1865 st	r11,reg11 */
0x0000f586, 0x06c0f67e, /* 06c0f67e //C 0f586  1866 st	r12,reg12 */
0x0000f587, 0x00200000, /* 00200000 //C 0f587  1868 mov	r2,r0		; pointer to format string */
0x0000f588, 0x0112f673, /* 0112f673 //C 0f588  1869 mvzl	r1,reg1		; pointer to params */
0x0000f589, 0x01320000, /* 01320000 //C 0f589  1870 mvzl	r3,0		; byte idx in packed str */
0x0000f58a, 0x0e020000, /* 0e020000 //C 0f58a  1872 ld	r0,r2		; get next char */
0x0000f58b, 0x020b0000, /* 020b0000 //C 0f58b  1873 sz	r0		; is it EOS? */
0x0000f58c, 0x11f2f5f1, /* 11f2f5f1 //C 0f58c  1874 jz	printf_ret */
0x0000f58d, 0x06014003, /* 06014003 //C 0f58d  1875 getbz	r0,r0,r3	; pick next byte */
0x0000f58e, 0x020b0000, /* 020b0000 //C 0f58e  1876 sz	r0		; is it null? */
0x0000f58f, 0x11f2f5ee, /* 11f2f5ee //C 0f58f  1877 jz	printf_nextword	; no more non-nulls */
0x0000f590, 0x0108005c, /* 0108005c //C 0f590  1879 cmp	r0,'\\' */
0x0000f591, 0x21f2f5bc, /* 21f2f5bc //C 0f591  1880 jnz	printf_notescape */
0x0000f592, 0x01340001, /* 01340001 //C 0f592  1882 inc	r3 */
0x0000f593, 0x01380004, /* 01380004 //C 0f593  1883 cmp	r3,4 */
0x0000f594, 0x21f2f59a, /* 21f2f59a //C 0f594  1884 jnz	printf_l1 */
0x0000f595, 0x01320000, /* 01320000 //C 0f595  1886 mvzl	r3,0 */
0x0000f596, 0x01240001, /* 01240001 //C 0f596  1887 inc	r2 */
0x0000f597, 0x0e020000, /* 0e020000 //C 0f597  1888 ld	r0,r2 */
0x0000f598, 0x020b0000, /* 020b0000 //C 0f598  1889 sz	r0 */
0x0000f599, 0x11f2f5f1, /* 11f2f5f1 //C 0f599  1890 jz	printf_ret */
0x0000f59a, 0x0e020000, /* 0e020000 //C 0f59a  1892 ld	r0,r2 */
0x0000f59b, 0x06014003, /* 06014003 //C 0f59b  1893 getbz	r0,r0,r3 */
0x0000f59c, 0x020b0000, /* 020b0000 //C 0f59c  1894 sz	r0 */
0x0000f59d, 0x11f2f595, /* 11f2f595 //C 0f59d  1895 jz	printf_l2 */
0x0000f59e, 0x01080061, /* 01080061 //C 0f59e  1897 cmp	r0,'a' */
0x0000f59f, 0x11020007, /* 11020007 //C 0f59f  1898 Z mvzl	r0,7 */
0x0000f5a0, 0x11f2f5ea, /* 11f2f5ea //C 0f5a0  1899 Z jmp	printf_print */
0x0000f5a1, 0x01080062, /* 01080062 //C 0f5a1  1900 cmp	r0,'b' */
0x0000f5a2, 0x11020008, /* 11020008 //C 0f5a2  1901 Z mvzl	r0,8 */
0x0000f5a3, 0x11f2f5ea, /* 11f2f5ea //C 0f5a3  1902 Z jmp	printf_print */
0x0000f5a4, 0x01080065, /* 01080065 //C 0f5a4  1903 cmp	r0,'e' */
0x0000f5a5, 0x1102001b, /* 1102001b //C 0f5a5  1904 Z mvzl	r0,0x1b */
0x0000f5a6, 0x11f2f5ea, /* 11f2f5ea //C 0f5a6  1905 Z jmp	printf_print */
0x0000f5a7, 0x01080066, /* 01080066 //C 0f5a7  1906 cmp	r0,'f' */
0x0000f5a8, 0x1102000c, /* 1102000c //C 0f5a8  1907 Z mvzl	r0,0xc */
0x0000f5a9, 0x11f2f5ea, /* 11f2f5ea //C 0f5a9  1908 Z jmp	printf_print */
0x0000f5aa, 0x0108006e, /* 0108006e //C 0f5aa  1909 cmp	r0,'n' */
0x0000f5ab, 0x1102000a, /* 1102000a //C 0f5ab  1910 Z mvzl	r0,0xa */
0x0000f5ac, 0x11f2f5ea, /* 11f2f5ea //C 0f5ac  1911 Z jmp	printf_print */
0x0000f5ad, 0x01080072, /* 01080072 //C 0f5ad  1912 cmp	r0,'r' */
0x0000f5ae, 0x1102000d, /* 1102000d //C 0f5ae  1913 Z mvzl	r0,0xd */
0x0000f5af, 0x11f2f5ea, /* 11f2f5ea //C 0f5af  1914 Z jmp	printf_print */
0x0000f5b0, 0x01080074, /* 01080074 //C 0f5b0  1915 cmp	r0,'t' */
0x0000f5b1, 0x11020009, /* 11020009 //C 0f5b1  1916 Z mvzl	r0,9 */
0x0000f5b2, 0x11f2f5ea, /* 11f2f5ea //C 0f5b2  1917 Z jmp	printf_print */
0x0000f5b3, 0x01080076, /* 01080076 //C 0f5b3  1918 cmp	r0,'v' */
0x0000f5b4, 0x1102000b, /* 1102000b //C 0f5b4  1919 Z mvzl	r0,0xb */
0x0000f5b5, 0x11f2f5ea, /* 11f2f5ea //C 0f5b5  1920 Z jmp	printf_print */
0x0000f5b6, 0x0108005c, /* 0108005c //C 0f5b6  1921 cmp	r0,0x5c */
0x0000f5b7, 0x11f2f5ea, /* 11f2f5ea //C 0f5b7  1922 Z jmp	printf_print */
0x0000f5b8, 0x01080030, /* 01080030 //C 0f5b8  1923 cmp	r0,'0' */
0x0000f5b9, 0x11020000, /* 11020000 //C 0f5b9  1924 Z mvzl	r0,0 */
0x0000f5ba, 0x11f2f5ea, /* 11f2f5ea //C 0f5ba  1925 Z jmp	printf_print */
0x0000f5bb, 0x01f2f5ea, /* 01f2f5ea //C 0f5bb  1927 jmp	printf_print */
0x0000f5bc, 0x01080025, /* 01080025 //C 0f5bc  1930 cmp	r0,'%'		; is it a format char? */
0x0000f5bd, 0x21f2f5ea, /* 21f2f5ea //C 0f5bd  1931 jnz	printf_print */
0x0000f5be, 0x01340001, /* 01340001 //C 0f5be  1933 inc	r3 */
0x0000f5bf, 0x01380004, /* 01380004 //C 0f5bf  1934 cmp	r3,4 */
0x0000f5c0, 0x21f2f5c6, /* 21f2f5c6 //C 0f5c0  1935 jnz	printf_l3 */
0x0000f5c1, 0x01320000, /* 01320000 //C 0f5c1  1937 mvzl	r3,0 */
0x0000f5c2, 0x01240001, /* 01240001 //C 0f5c2  1938 inc	r2		; go to format char */
0x0000f5c3, 0x0e020000, /* 0e020000 //C 0f5c3  1939 ld	r0,r2 */
0x0000f5c4, 0x022b0000, /* 022b0000 //C 0f5c4  1940 sz	r2		; is it EOS? */
0x0000f5c5, 0x11f2f5f1, /* 11f2f5f1 //C 0f5c5  1941 jz	printf_ret */
0x0000f5c6, 0x0e020000, /* 0e020000 //C 0f5c6  1943 ld	r0,r2 */
0x0000f5c7, 0x06014003, /* 06014003 //C 0f5c7  1944 getbz	r0,r0,r3 */
0x0000f5c8, 0x020b0000, /* 020b0000 //C 0f5c8  1945 sz	r0 */
0x0000f5c9, 0x11f2f5c1, /* 11f2f5c1 //C 0f5c9  1946 jz	printf_l4 */
0x0000f5ca, 0x01080025, /* 01080025 //C 0f5ca  1948 cmp	r0,'%'		; % is used to print % */
0x0000f5cb, 0x11f2f5ea, /* 11f2f5ea //C 0f5cb  1949 jz	printf_print */
0x0000f5cc, 0x01080075, /* 01080075 //C 0f5cc  1951 cmp	r0,'u'		; u,d print in decimal */
0x0000f5cd, 0x11f2f5d0, /* 11f2f5d0 //C 0f5cd  1952 jz	printf_d */
0x0000f5ce, 0x01080064, /* 01080064 //C 0f5ce  1953 cmp	r0,'d' */
0x0000f5cf, 0x21f2f5d4, /* 21f2f5d4 //C 0f5cf  1954 jnz	printf_notd */
0x0000f5d0, 0x0e010000, /* 0e010000 //C 0f5d0  1956 ld	r0,r1 */
0x0000f5d1, 0x01140001, /* 01140001 //C 0f5d1  1957 inc	r1 */
0x0000f5d2, 0x0400f572, /* 0400f572 //C 0f5d2  1958 call	printd */
0x0000f5d3, 0x01f2f5eb, /* 01f2f5eb //C 0f5d3  1959 jmp	printf_next */
0x0000f5d4, 0x01080078, /* 01080078 //C 0f5d4  1962 cmp	r0,'x' */
0x0000f5d5, 0x21f2f5dd, /* 21f2f5dd //C 0f5d5  1963 jnz	printf_notx */
0x0000f5d6, 0x0e010000, /* 0e010000 //C 0f5d6  1965 ld	r0,r1 */
0x0000f5d7, 0x01140001, /* 01140001 //C 0f5d7  1966 inc	r1 */
0x0000f5d8, 0x0d1d0000, /* 0d1d0000 //C 0f5d8  1967 push	r1 */
0x0000f5d9, 0x01120000, /* 01120000 //C 0f5d9  1968 mvzl	r1,0 */
0x0000f5da, 0x0400f4dd, /* 0400f4dd //C 0f5da  1969 call	print_vhex */
0x0000f5db, 0x0f1d0000, /* 0f1d0000 //C 0f5db  1970 pop	r1 */
0x0000f5dc, 0x01f2f5eb, /* 01f2f5eb //C 0f5dc  1971 jmp	printf_next */
0x0000f5dd, 0x01080073, /* 01080073 //C 0f5dd  1974 cmp	r0,'s' */
0x0000f5de, 0x21f2f5e3, /* 21f2f5e3 //C 0f5de  1975 jnz	printf_nots */
0x0000f5df, 0x0e010000, /* 0e010000 //C 0f5df  1977 ld	r0,r1 */
0x0000f5e0, 0x01140001, /* 01140001 //C 0f5e0  1978 inc	r1 */
0x0000f5e1, 0x0400f49d, /* 0400f49d //C 0f5e1  1979 call	prints */
0x0000f5e2, 0x01f2f5eb, /* 01f2f5eb //C 0f5e2  1980 jmp	printf_next */
0x0000f5e3, 0x01080063, /* 01080063 //C 0f5e3  1983 cmp	r0,'c' */
0x0000f5e4, 0x21f2f5e9, /* 21f2f5e9 //C 0f5e4  1984 jnz	printf_notc */
0x0000f5e5, 0x0e010000, /* 0e010000 //C 0f5e5  1985 ld	r0,r1 */
0x0000f5e6, 0x01140001, /* 01140001 //C 0f5e6  1986 inc	r1 */
0x0000f5e7, 0x0400f496, /* 0400f496 //C 0f5e7  1987 call	putchar */
0x0000f5e8, 0x01f2f5eb, /* 01f2f5eb //C 0f5e8  1988 jmp	printf_next */
0x0000f5e9, 0x01f2f5eb, /* 01f2f5eb //C 0f5e9  1991 jmp	printf_next */
0x0000f5ea, 0x0400f496, /* 0400f496 //C 0f5ea  1993 call	putchar		; print actual char */
0x0000f5eb, 0x01340001, /* 01340001 //C 0f5eb  1995 inc	r3		; next byte in word */
0x0000f5ec, 0x01380004, /* 01380004 //C 0f5ec  1996 cmp	r3,4		; all 4 processed? */
0x0000f5ed, 0x21f2f58a, /* 21f2f58a //C 0f5ed  1997 jnz	printf_cyc */
0x0000f5ee, 0x01240001, /* 01240001 //C 0f5ee  1999 inc	r2		; inc string ptr */
0x0000f5ef, 0x01320000, /* 01320000 //C 0f5ef  2000 mvzl	r3,0		; restart byte idx */
0x0000f5f0, 0x01f2f58a, /* 01f2f58a //C 0f5f0  2001 jmp	printf_cyc */
0x0000f5f1, 0x0f3d0000, /* 0f3d0000 //C 0f5f1  2004 pop	r3 */
0x0000f5f2, 0x0f1d0000, /* 0f1d0000 //C 0f5f2  2005 pop	r1 */
0x0000f5f3, 0x0f0d0000, /* 0f0d0000 //C 0f5f3  2006 pop	r0 */
0x0000f5f4, 0x0ffd0000, /* 0ffd0000 //C 0f5f4  2007 pop	pc */
0x0000f5f5, 0x0d0d0000, /* 0d0d0000 //C 0f5f5  2012 push	r0 */
0x0000f5f6, 0x0d2d0000, /* 0d2d0000 //C 0f5f6  2013 push	r2 */
0x0000f5f7, 0x00000e00, /* 00000e00 //C 0f5f7  2014 mov	r0,LR */
0x0000f5f8, 0x0400f577, /* 0400f577 //C 0f5f8  2015 call	printf */
0x0000f5f9, 0x01240001, /* 01240001 //C 0f5f9  2016 inc	r2 */
0x0000f5fa, 0x0620f674, /* 0620f674 //C 0f5fa  2017 st	r2,reg2 */
0x0000f5fb, 0x0f2d0000, /* 0f2d0000 //C 0f5fb  2018 pop	r2 */
0x0000f5fc, 0x0f0d0000, /* 0f0d0000 //C 0f5fc  2019 pop	r0 */
0x0000f5fd, 0x07f0f674, /* 07f0f674 //C 0f5fd  2020 ld	PC,reg2 */
0x0000f5fe, 0x0d1d0000, /* 0d1d0000 //C 0f5fe  2024 push	r1 */
0x0000f5ff, 0x01020001, /* 01020001 //C 0f5ff  2025 mvzl	r0,version_sub */
0x0000f600, 0x01120001, /* 01120001 //C 0f600  2026 mvzl	r1,version_main */
0x0000f601, 0x07018101, /* 07018101 //C 0f601  2027 putb	r0,r1,1 */
0x0000f602, 0x0f1d0000, /* 0f1d0000 //C 0f602  2028 pop	r1 */
0x0000f603, 0x00f00e00, /* 00f00e00 //C 0f603  2029 ret */
0x0000f604, 0x00000000, /* 00000000 //C 0f604  2034 line:		ds	100		; line buffer */
0x0000f668, 0x00000000, /* 00000000 //C 0f668  2035 line_ptr:	ds	1		; line pointer (index) */
0x0000f669, 0x00000000, /* 00000000 //C 0f669  2036 at_eol:		ds	1		; bool, true if EOL arrived */
0x0000f66a, 0x00000000, /* 00000000 //C 0f66a  2037 words:		ds	5		; Tokens of line */
0x0000f66f, 0x00000000, /* 00000000 //C 0f66f  2038 echo:		ds	1		; bool, do echo or not */
0x0000f670, 0x00000000, /* 00000000 //C 0f670  2039 dd	0 */
0x0000f671, 0x00000000, /* 00000000 //C 0f671  2040 dd	0 */
0x0000f672, 0x00000000, /* 00000000 //C 0f672  2042 dd	0 */
0x0000f673, 0x00000000, /* 00000000 //C 0f673  2043 dd	0 */
0x0000f674, 0x00000000, /* 00000000 //C 0f674  2044 dd	0 */
0x0000f675, 0x00000000, /* 00000000 //C 0f675  2045 dd	0 */
0x0000f676, 0x00000000, /* 00000000 //C 0f676  2046 dd	0 */
0x0000f677, 0x00000000, /* 00000000 //C 0f677  2047 dd	0 */
0x0000f678, 0x00000000, /* 00000000 //C 0f678  2048 dd	0 */
0x0000f679, 0x00000000, /* 00000000 //C 0f679  2049 dd	0 */
0x0000f67a, 0x00000000, /* 00000000 //C 0f67a  2050 dd	0 */
0x0000f67b, 0x00000000, /* 00000000 //C 0f67b  2051 dd	0 */
0x0000f67c, 0x00000000, /* 00000000 //C 0f67c  2052 dd	0 */
0x0000f67d, 0x00000000, /* 00000000 //C 0f67d  2053 dd	0 */
0x0000f67e, 0x00000000, /* 00000000 //C 0f67e  2054 dd	0 */
0x0000f67f, 0x00000000, /* 00000000 //C 0f67f  2055 dd	0 */
0x0000f680, 0x00000000, /* 00000000 //C 0f680  2056 dd	0 */
0x0000f681, 0x00000000, /* 00000000 //C 0f681  2057 dd	0 */
0x0000f682, 0x00000000, /* 00000000 //C 0f682  2058 dd	0 */
0x0000f683, 0x00000050, /* 00000050 //C 0f683  2060 db	80 */
0x0000f684, 0x0000004d, /* 0000004d //C 0f684  2060 db	77 */
0x0000f685, 0x0000006f, /* 0000006f //C 0f685  2060 db	111 */
0x0000f686, 0x0000006e, /* 0000006e //C 0f686  2060 db	110 */
0x0000f687, 0x00000069, /* 00000069 //C 0f687  2060 db	105 */
0x0000f688, 0x00000074, /* 00000074 //C 0f688  2060 db	116 */
0x0000f689, 0x0000006f, /* 0000006f //C 0f689  2060 db	111 */
0x0000f68a, 0x00000072, /* 00000072 //C 0f68a  2060 db	114 */
0x0000f68b, 0x00000020, /* 00000020 //C 0f68b  2060 db	32 */
0x0000f68c, 0x00000076, /* 00000076 //C 0f68c  2060 db	118 */
0x0000f68d, 0x00000025, /* 00000025 //C 0f68d  2060 db	37 */
0x0000f68e, 0x00000064, /* 00000064 //C 0f68e  2060 db	100 */
0x0000f68f, 0x0000002e, /* 0000002e //C 0f68f  2060 db	46 */
0x0000f690, 0x00000025, /* 00000025 //C 0f690  2060 db	37 */
0x0000f691, 0x00000064, /* 00000064 //C 0f691  2060 db	100 */
0x0000f692, 0x00000020, /* 00000020 //C 0f692  2060 db	32 */
0x0000f693, 0x00000028, /* 00000028 //C 0f693  2060 db	40 */
0x0000f694, 0x00000063, /* 00000063 //C 0f694  2060 db	99 */
0x0000f695, 0x00000070, /* 00000070 //C 0f695  2060 db	112 */
0x0000f696, 0x00000075, /* 00000075 //C 0f696  2060 db	117 */
0x0000f697, 0x00000020, /* 00000020 //C 0f697  2060 db	32 */
0x0000f698, 0x00000076, /* 00000076 //C 0f698  2060 db	118 */
0x0000f699, 0x00000025, /* 00000025 //C 0f699  2060 db	37 */
0x0000f69a, 0x00000064, /* 00000064 //C 0f69a  2060 db	100 */
0x0000f69b, 0x0000002e, /* 0000002e //C 0f69b  2060 db	46 */
0x0000f69c, 0x00000025, /* 00000025 //C 0f69c  2060 db	37 */
0x0000f69d, 0x00000064, /* 00000064 //C 0f69d  2060 db	100 */
0x0000f69e, 0x0000002e, /* 0000002e //C 0f69e  2060 db	46 */
0x0000f69f, 0x00000025, /* 00000025 //C 0f69f  2060 db	37 */
0x0000f6a0, 0x00000064, /* 00000064 //C 0f6a0  2060 db	100 */
0x0000f6a1, 0x00000029, /* 00000029 //C 0f6a1  2060 db	41 */
0x0000f6a2, 0x0000000a, /* 0000000a //C 0f6a2  2060 db	10 */
0x0000f6a3, 0x00000000, /* 00000000 //C 0f6a3  2060 db */
0x0000f6a4, 0x00000053, /* 00000053 //C 0f6a4  2061 db	83 */
0x0000f6a5, 0x00000074, /* 00000074 //C 0f6a5  2061 db	116 */
0x0000f6a6, 0x0000006f, /* 0000006f //C 0f6a6  2061 db	111 */
0x0000f6a7, 0x00000070, /* 00000070 //C 0f6a7  2061 db	112 */
0x0000f6a8, 0x00000020, /* 00000020 //C 0f6a8  2061 db	32 */
0x0000f6a9, 0x00000061, /* 00000061 //C 0f6a9  2061 db	97 */
0x0000f6aa, 0x00000074, /* 00000074 //C 0f6aa  2061 db	116 */
0x0000f6ab, 0x0000003a, /* 0000003a //C 0f6ab  2061 db	58 */
0x0000f6ac, 0x00000020, /* 00000020 //C 0f6ac  2061 db	32 */
0x0000f6ad, 0x00000000, /* 00000000 //C 0f6ad  2061 db */
0x0000f6ae, 0x00000008, /* 00000008 //C 0f6ae  2062 db	8 */
0x0000f6af, 0x00000020, /* 00000020 //C 0f6af  2062 db	32 */
0x0000f6b0, 0x00000008, /* 00000008 //C 0f6b0  2062 db	8 */
0x0000f6b1, 0x00000000, /* 00000000 //C 0f6b1  2062 db	0 */
0x0000f6b2, 0x0000003a, /* 0000003a //C 0f6b2  2063 db	58 */
0x0000f6b3, 0x00000000, /* 00000000 //C 0f6b3  2063 db */
0x0000f6b4, 0x00000020, /* 00000020 //C 0f6b4  2064 db	32 */
0x0000f6b5, 0x0000003b, /* 0000003b //C 0f6b5  2064 db	59 */
0x0000f6b6, 0x00000009, /* 00000009 //C 0f6b6  2064 db	9 */
0x0000f6b7, 0x0000000b, /* 0000000b //C 0f6b7  2064 db	11 */
0x0000f6b8, 0x0000002c, /* 0000002c //C 0f6b8  2064 db	44 */
0x0000f6b9, 0x0000003d, /* 0000003d //C 0f6b9  2064 db	61 */
0x0000f6ba, 0x0000005b, /* 0000005b //C 0f6ba  2064 db	91 */
0x0000f6bb, 0x0000005d, /* 0000005d //C 0f6bb  2064 db	93 */
0x0000f6bc, 0x00000000, /* 00000000 //C 0f6bc  2064 db */
0x0000f6bd, 0x00000028, /* 00000028 //C 0f6bd  2065 db	40 */
0x0000f6be, 0x0000006e, /* 0000006e //C 0f6be  2065 db	110 */
0x0000f6bf, 0x00000075, /* 00000075 //C 0f6bf  2065 db	117 */
0x0000f6c0, 0x0000006c, /* 0000006c //C 0f6c0  2065 db	108 */
0x0000f6c1, 0x0000006c, /* 0000006c //C 0f6c1  2065 db	108 */
0x0000f6c2, 0x00000029, /* 00000029 //C 0f6c2  2065 db	41 */
0x0000f6c3, 0x00000000, /* 00000000 //C 0f6c3  2065 db */
0x0000f6c4, 0x00000000, /* 00000000 //C 0f6c4  2066 db	0 */
0x0000f6c5, 0x00000000, /* 00000000 //C 0f6c5  2067 db	0 */
0x0000f6c6, 0x00000000, /* 00000000 //C 0f6c6  2068 sc_buffer:	ds	10 */
0x0000f6d0, 0x0000f196, /* 0000f196 //C 0f6d0  2072 dd	cmd_sm */
0x0000f6d1, 0x00000073, /* 00000073 //C 0f6d1  2073 db	115 */
0x0000f6d2, 0x0000006d, /* 0000006d //C 0f6d2  2073 db	109 */
0x0000f6d3, 0x00000000, /* 00000000 //C 0f6d3  2073 db */
0x0000f6d4, 0x0000f13d, /* 0000f13d //C 0f6d4  2074 dd	cmd_m */
0x0000f6d5, 0x0000006d, /* 0000006d //C 0f6d5  2075 db	109 */
0x0000f6d6, 0x00000000, /* 00000000 //C 0f6d6  2075 db */
0x0000f6d7, 0x0000f13d, /* 0000f13d //C 0f6d7  2076 dd	cmd_m */
0x0000f6d8, 0x0000006d, /* 0000006d //C 0f6d8  2077 db	109 */
0x0000f6d9, 0x00000065, /* 00000065 //C 0f6d9  2077 db	101 */
0x0000f6da, 0x0000006d, /* 0000006d //C 0f6da  2077 db	109 */
0x0000f6db, 0x00000000, /* 00000000 //C 0f6db  2077 db */
0x0000f6dc, 0x0000f1b9, /* 0000f1b9 //C 0f6dc  2078 dd	cmd_d */
0x0000f6dd, 0x00000064, /* 00000064 //C 0f6dd  2079 db	100 */
0x0000f6de, 0x00000000, /* 00000000 //C 0f6de  2079 db */
0x0000f6df, 0x0000f1b9, /* 0000f1b9 //C 0f6df  2080 dd	cmd_d */
0x0000f6e0, 0x00000064, /* 00000064 //C 0f6e0  2081 db	100 */
0x0000f6e1, 0x00000075, /* 00000075 //C 0f6e1  2081 db	117 */
0x0000f6e2, 0x0000006d, /* 0000006d //C 0f6e2  2081 db	109 */
0x0000f6e3, 0x00000070, /* 00000070 //C 0f6e3  2081 db	112 */
0x0000f6e4, 0x00000000, /* 00000000 //C 0f6e4  2081 db */
0x0000f6e5, 0x0000f1ec, /* 0000f1ec //C 0f6e5  2082 dd	cmd_l */
0x0000f6e6, 0x0000006c, /* 0000006c //C 0f6e6  2083 db	108 */
0x0000f6e7, 0x00000000, /* 00000000 //C 0f6e7  2083 db */
0x0000f6e8, 0x0000f1ec, /* 0000f1ec //C 0f6e8  2084 dd	cmd_l */
0x0000f6e9, 0x0000006c, /* 0000006c //C 0f6e9  2085 db	108 */
0x0000f6ea, 0x0000006f, /* 0000006f //C 0f6ea  2085 db	111 */
0x0000f6eb, 0x00000061, /* 00000061 //C 0f6eb  2085 db	97 */
0x0000f6ec, 0x00000064, /* 00000064 //C 0f6ec  2085 db	100 */
0x0000f6ed, 0x00000000, /* 00000000 //C 0f6ed  2085 db */
0x0000f6ee, 0x0000f262, /* 0000f262 //C 0f6ee  2086 dd	cmd_g */
0x0000f6ef, 0x00000067, /* 00000067 //C 0f6ef  2087 db	103 */
0x0000f6f0, 0x00000000, /* 00000000 //C 0f6f0  2087 db */
0x0000f6f1, 0x0000f262, /* 0000f262 //C 0f6f1  2088 dd	cmd_g */
0x0000f6f2, 0x00000067, /* 00000067 //C 0f6f2  2089 db	103 */
0x0000f6f3, 0x0000006f, /* 0000006f //C 0f6f3  2089 db	111 */
0x0000f6f4, 0x00000000, /* 00000000 //C 0f6f4  2089 db */
0x0000f6f5, 0x0000f262, /* 0000f262 //C 0f6f5  2090 dd	cmd_g */
0x0000f6f6, 0x00000072, /* 00000072 //C 0f6f6  2091 db	114 */
0x0000f6f7, 0x00000075, /* 00000075 //C 0f6f7  2091 db	117 */
0x0000f6f8, 0x0000006e, /* 0000006e //C 0f6f8  2091 db	110 */
0x0000f6f9, 0x00000000, /* 00000000 //C 0f6f9  2091 db */
0x0000f6fa, 0x0000f2a1, /* 0000f2a1 //C 0f6fa  2092 dd	cmd_h */
0x0000f6fb, 0x0000003f, /* 0000003f //C 0f6fb  2093 db	63 */
0x0000f6fc, 0x00000000, /* 00000000 //C 0f6fc  2093 db */
0x0000f6fd, 0x0000f2a1, /* 0000f2a1 //C 0f6fd  2094 dd	cmd_h */
0x0000f6fe, 0x00000068, /* 00000068 //C 0f6fe  2095 db	104 */
0x0000f6ff, 0x00000000, /* 00000000 //C 0f6ff  2095 db */
0x0000f700, 0x0000f2a1, /* 0000f2a1 //C 0f700  2096 dd	cmd_h */
0x0000f701, 0x00000068, /* 00000068 //C 0f701  2097 db	104 */
0x0000f702, 0x00000065, /* 00000065 //C 0f702  2097 db	101 */
0x0000f703, 0x0000006c, /* 0000006c //C 0f703  2097 db	108 */
0x0000f704, 0x00000070, /* 00000070 //C 0f704  2097 db	112 */
0x0000f705, 0x00000000, /* 00000000 //C 0f705  2097 db */
0x0000f706, 0x0000f308, /* 0000f308 //C 0f706  2098 dd	cmd_r */
0x0000f707, 0x00000072, /* 00000072 //C 0f707  2099 db	114 */
0x0000f708, 0x00000000, /* 00000000 //C 0f708  2099 db */
0x0000f709, 0x0000f308, /* 0000f308 //C 0f709  2100 dd	cmd_r */
0x0000f70a, 0x00000072, /* 00000072 //C 0f70a  2101 db	114 */
0x0000f70b, 0x00000065, /* 00000065 //C 0f70b  2101 db	101 */
0x0000f70c, 0x00000067, /* 00000067 //C 0f70c  2101 db	103 */
0x0000f70d, 0x00000000, /* 00000000 //C 0f70d  2101 db */
0x0000f70e, 0x0000f308, /* 0000f308 //C 0f70e  2102 dd	cmd_r */
0x0000f70f, 0x00000072, /* 00000072 //C 0f70f  2103 db	114 */
0x0000f710, 0x00000065, /* 00000065 //C 0f710  2103 db	101 */
0x0000f711, 0x00000067, /* 00000067 //C 0f711  2103 db	103 */
0x0000f712, 0x00000073, /* 00000073 //C 0f712  2103 db	115 */
0x0000f713, 0x00000000, /* 00000000 //C 0f713  2103 db */
0x0000f714, 0x0000f398, /* 0000f398 //C 0f714  2104 dd	cmd_sp */
0x0000f715, 0x00000073, /* 00000073 //C 0f715  2105 db	115 */
0x0000f716, 0x00000070, /* 00000070 //C 0f716  2105 db	112 */
0x0000f717, 0x00000000, /* 00000000 //C 0f717  2105 db */
0x0000f718, 0x0000f39b, /* 0000f39b //C 0f718  2106 dd	cmd_lr */
0x0000f719, 0x0000006c, /* 0000006c //C 0f719  2107 db	108 */
0x0000f71a, 0x00000072, /* 00000072 //C 0f71a  2107 db	114 */
0x0000f71b, 0x00000000, /* 00000000 //C 0f71b  2107 db */
0x0000f71c, 0x0000f39e, /* 0000f39e //C 0f71c  2108 dd	cmd_pc */
0x0000f71d, 0x00000070, /* 00000070 //C 0f71d  2109 db	112 */
0x0000f71e, 0x00000063, /* 00000063 //C 0f71e  2109 db	99 */
0x0000f71f, 0x00000000, /* 00000000 //C 0f71f  2109 db */
0x0000f720, 0x0000f3a1, /* 0000f3a1 //C 0f720  2110 dd	cmd_f */
0x0000f721, 0x00000066, /* 00000066 //C 0f721  2111 db	102 */
0x0000f722, 0x00000000, /* 00000000 //C 0f722  2111 db */
0x0000f723, 0x00000000, /* 00000000 //C 0f723  2112 dd	0 */
0x0000f724, 0x00000000, /* 00000000 //C 0f724  2113 dd	0 */
0x0000f725, 0x0000006d, /* 0000006d //C 0f725  2115 db	109 */
0x0000f726, 0x0000005b, /* 0000005b //C 0f726  2115 db	91 */
0x0000f727, 0x00000065, /* 00000065 //C 0f727  2115 db	101 */
0x0000f728, 0x0000006d, /* 0000006d //C 0f728  2115 db	109 */
0x0000f729, 0x0000005d, /* 0000005d //C 0f729  2115 db	93 */
0x0000f72a, 0x00000020, /* 00000020 //C 0f72a  2115 db	32 */
0x0000f72b, 0x00000061, /* 00000061 //C 0f72b  2115 db	97 */
0x0000f72c, 0x00000064, /* 00000064 //C 0f72c  2115 db	100 */
0x0000f72d, 0x00000064, /* 00000064 //C 0f72d  2115 db	100 */
0x0000f72e, 0x00000072, /* 00000072 //C 0f72e  2115 db	114 */
0x0000f72f, 0x00000020, /* 00000020 //C 0f72f  2115 db	32 */
0x0000f730, 0x0000005b, /* 0000005b //C 0f730  2115 db	91 */
0x0000f731, 0x00000076, /* 00000076 //C 0f731  2115 db	118 */
0x0000f732, 0x00000061, /* 00000061 //C 0f732  2115 db	97 */
0x0000f733, 0x0000006c, /* 0000006c //C 0f733  2115 db	108 */
0x0000f734, 0x0000005d, /* 0000005d //C 0f734  2115 db	93 */
0x0000f735, 0x00000020, /* 00000020 //C 0f735  2115 db	32 */
0x0000f736, 0x00000020, /* 00000020 //C 0f736  2115 db	32 */
0x0000f737, 0x00000047, /* 00000047 //C 0f737  2115 db	71 */
0x0000f738, 0x00000065, /* 00000065 //C 0f738  2115 db	101 */
0x0000f739, 0x00000074, /* 00000074 //C 0f739  2115 db	116 */
0x0000f73a, 0x0000002f, /* 0000002f //C 0f73a  2115 db	47 */
0x0000f73b, 0x00000073, /* 00000073 //C 0f73b  2115 db	115 */
0x0000f73c, 0x00000065, /* 00000065 //C 0f73c  2115 db	101 */
0x0000f73d, 0x00000074, /* 00000074 //C 0f73d  2115 db	116 */
0x0000f73e, 0x00000020, /* 00000020 //C 0f73e  2115 db	32 */
0x0000f73f, 0x0000006d, /* 0000006d //C 0f73f  2115 db	109 */
0x0000f740, 0x00000065, /* 00000065 //C 0f740  2115 db	101 */
0x0000f741, 0x0000006d, /* 0000006d //C 0f741  2115 db	109 */
0x0000f742, 0x0000006f, /* 0000006f //C 0f742  2115 db	111 */
0x0000f743, 0x00000072, /* 00000072 //C 0f743  2115 db	114 */
0x0000f744, 0x00000079, /* 00000079 //C 0f744  2115 db	121 */
0x0000f745, 0x0000000a, /* 0000000a //C 0f745  2115 db	10 */
0x0000f746, 0x00000000, /* 00000000 //C 0f746  2115 db */
0x0000f747, 0x00000064, /* 00000064 //C 0f747  2116 db	100 */
0x0000f748, 0x0000005b, /* 0000005b //C 0f748  2116 db	91 */
0x0000f749, 0x00000075, /* 00000075 //C 0f749  2116 db	117 */
0x0000f74a, 0x0000006d, /* 0000006d //C 0f74a  2116 db	109 */
0x0000f74b, 0x00000070, /* 00000070 //C 0f74b  2116 db	112 */
0x0000f74c, 0x0000005d, /* 0000005d //C 0f74c  2116 db	93 */
0x0000f74d, 0x00000020, /* 00000020 //C 0f74d  2116 db	32 */
0x0000f74e, 0x00000073, /* 00000073 //C 0f74e  2116 db	115 */
0x0000f74f, 0x00000074, /* 00000074 //C 0f74f  2116 db	116 */
0x0000f750, 0x00000061, /* 00000061 //C 0f750  2116 db	97 */
0x0000f751, 0x00000072, /* 00000072 //C 0f751  2116 db	114 */
0x0000f752, 0x00000074, /* 00000074 //C 0f752  2116 db	116 */
0x0000f753, 0x00000020, /* 00000020 //C 0f753  2116 db	32 */
0x0000f754, 0x00000065, /* 00000065 //C 0f754  2116 db	101 */
0x0000f755, 0x0000006e, /* 0000006e //C 0f755  2116 db	110 */
0x0000f756, 0x00000064, /* 00000064 //C 0f756  2116 db	100 */
0x0000f757, 0x00000020, /* 00000020 //C 0f757  2116 db	32 */
0x0000f758, 0x00000020, /* 00000020 //C 0f758  2116 db	32 */
0x0000f759, 0x00000044, /* 00000044 //C 0f759  2116 db	68 */
0x0000f75a, 0x00000075, /* 00000075 //C 0f75a  2116 db	117 */
0x0000f75b, 0x0000006d, /* 0000006d //C 0f75b  2116 db	109 */
0x0000f75c, 0x00000070, /* 00000070 //C 0f75c  2116 db	112 */
0x0000f75d, 0x00000020, /* 00000020 //C 0f75d  2116 db	32 */
0x0000f75e, 0x0000006d, /* 0000006d //C 0f75e  2116 db	109 */
0x0000f75f, 0x00000065, /* 00000065 //C 0f75f  2116 db	101 */
0x0000f760, 0x0000006d, /* 0000006d //C 0f760  2116 db	109 */
0x0000f761, 0x0000006f, /* 0000006f //C 0f761  2116 db	111 */
0x0000f762, 0x00000072, /* 00000072 //C 0f762  2116 db	114 */
0x0000f763, 0x00000079, /* 00000079 //C 0f763  2116 db	121 */
0x0000f764, 0x00000020, /* 00000020 //C 0f764  2116 db	32 */
0x0000f765, 0x00000063, /* 00000063 //C 0f765  2116 db	99 */
0x0000f766, 0x0000006f, /* 0000006f //C 0f766  2116 db	111 */
0x0000f767, 0x0000006e, /* 0000006e //C 0f767  2116 db	110 */
0x0000f768, 0x00000074, /* 00000074 //C 0f768  2116 db	116 */
0x0000f769, 0x00000065, /* 00000065 //C 0f769  2116 db	101 */
0x0000f76a, 0x0000006e, /* 0000006e //C 0f76a  2116 db	110 */
0x0000f76b, 0x00000074, /* 00000074 //C 0f76b  2116 db	116 */
0x0000f76c, 0x0000000a, /* 0000000a //C 0f76c  2116 db	10 */
0x0000f76d, 0x00000000, /* 00000000 //C 0f76d  2116 db */
0x0000f76e, 0x0000006c, /* 0000006c //C 0f76e  2117 db	108 */
0x0000f76f, 0x0000005b, /* 0000005b //C 0f76f  2117 db	91 */
0x0000f770, 0x0000006f, /* 0000006f //C 0f770  2117 db	111 */
0x0000f771, 0x00000061, /* 00000061 //C 0f771  2117 db	97 */
0x0000f772, 0x00000064, /* 00000064 //C 0f772  2117 db	100 */
0x0000f773, 0x0000005d, /* 0000005d //C 0f773  2117 db	93 */
0x0000f774, 0x00000020, /* 00000020 //C 0f774  2117 db	32 */
0x0000f775, 0x00000020, /* 00000020 //C 0f775  2117 db	32 */
0x0000f776, 0x00000020, /* 00000020 //C 0f776  2117 db	32 */
0x0000f777, 0x00000020, /* 00000020 //C 0f777  2117 db	32 */
0x0000f778, 0x00000020, /* 00000020 //C 0f778  2117 db	32 */
0x0000f779, 0x00000020, /* 00000020 //C 0f779  2117 db	32 */
0x0000f77a, 0x00000020, /* 00000020 //C 0f77a  2117 db	32 */
0x0000f77b, 0x00000020, /* 00000020 //C 0f77b  2117 db	32 */
0x0000f77c, 0x00000020, /* 00000020 //C 0f77c  2117 db	32 */
0x0000f77d, 0x00000020, /* 00000020 //C 0f77d  2117 db	32 */
0x0000f77e, 0x00000020, /* 00000020 //C 0f77e  2117 db	32 */
0x0000f77f, 0x00000020, /* 00000020 //C 0f77f  2117 db	32 */
0x0000f780, 0x0000004c, /* 0000004c //C 0f780  2117 db	76 */
0x0000f781, 0x0000006f, /* 0000006f //C 0f781  2117 db	111 */
0x0000f782, 0x00000061, /* 00000061 //C 0f782  2117 db	97 */
0x0000f783, 0x00000064, /* 00000064 //C 0f783  2117 db	100 */
0x0000f784, 0x00000020, /* 00000020 //C 0f784  2117 db	32 */
0x0000f785, 0x00000068, /* 00000068 //C 0f785  2117 db	104 */
0x0000f786, 0x00000065, /* 00000065 //C 0f786  2117 db	101 */
0x0000f787, 0x00000078, /* 00000078 //C 0f787  2117 db	120 */
0x0000f788, 0x00000020, /* 00000020 //C 0f788  2117 db	32 */
0x0000f789, 0x00000066, /* 00000066 //C 0f789  2117 db	102 */
0x0000f78a, 0x00000069, /* 00000069 //C 0f78a  2117 db	105 */
0x0000f78b, 0x0000006c, /* 0000006c //C 0f78b  2117 db	108 */
0x0000f78c, 0x00000065, /* 00000065 //C 0f78c  2117 db	101 */
0x0000f78d, 0x00000020, /* 00000020 //C 0f78d  2117 db	32 */
0x0000f78e, 0x00000074, /* 00000074 //C 0f78e  2117 db	116 */
0x0000f78f, 0x0000006f, /* 0000006f //C 0f78f  2117 db	111 */
0x0000f790, 0x00000020, /* 00000020 //C 0f790  2117 db	32 */
0x0000f791, 0x0000006d, /* 0000006d //C 0f791  2117 db	109 */
0x0000f792, 0x00000065, /* 00000065 //C 0f792  2117 db	101 */
0x0000f793, 0x0000006d, /* 0000006d //C 0f793  2117 db	109 */
0x0000f794, 0x0000006f, /* 0000006f //C 0f794  2117 db	111 */
0x0000f795, 0x00000072, /* 00000072 //C 0f795  2117 db	114 */
0x0000f796, 0x00000079, /* 00000079 //C 0f796  2117 db	121 */
0x0000f797, 0x0000000a, /* 0000000a //C 0f797  2117 db	10 */
0x0000f798, 0x00000000, /* 00000000 //C 0f798  2117 db */
0x0000f799, 0x00000067, /* 00000067 //C 0f799  2118 db	103 */
0x0000f79a, 0x0000005b, /* 0000005b //C 0f79a  2118 db	91 */
0x0000f79b, 0x0000006f, /* 0000006f //C 0f79b  2118 db	111 */
0x0000f79c, 0x0000005d, /* 0000005d //C 0f79c  2118 db	93 */
0x0000f79d, 0x0000007c, /* 0000007c //C 0f79d  2118 db	124 */
0x0000f79e, 0x00000072, /* 00000072 //C 0f79e  2118 db	114 */
0x0000f79f, 0x00000075, /* 00000075 //C 0f79f  2118 db	117 */
0x0000f7a0, 0x0000006e, /* 0000006e //C 0f7a0  2118 db	110 */
0x0000f7a1, 0x00000020, /* 00000020 //C 0f7a1  2118 db	32 */
0x0000f7a2, 0x0000005b, /* 0000005b //C 0f7a2  2118 db	91 */
0x0000f7a3, 0x00000061, /* 00000061 //C 0f7a3  2118 db	97 */
0x0000f7a4, 0x00000064, /* 00000064 //C 0f7a4  2118 db	100 */
0x0000f7a5, 0x00000064, /* 00000064 //C 0f7a5  2118 db	100 */
0x0000f7a6, 0x00000072, /* 00000072 //C 0f7a6  2118 db	114 */
0x0000f7a7, 0x0000005d, /* 0000005d //C 0f7a7  2118 db	93 */
0x0000f7a8, 0x00000020, /* 00000020 //C 0f7a8  2118 db	32 */
0x0000f7a9, 0x00000020, /* 00000020 //C 0f7a9  2118 db	32 */
0x0000f7aa, 0x00000020, /* 00000020 //C 0f7aa  2118 db	32 */
0x0000f7ab, 0x00000052, /* 00000052 //C 0f7ab  2118 db	82 */
0x0000f7ac, 0x00000075, /* 00000075 //C 0f7ac  2118 db	117 */
0x0000f7ad, 0x0000006e, /* 0000006e //C 0f7ad  2118 db	110 */
0x0000f7ae, 0x00000020, /* 00000020 //C 0f7ae  2118 db	32 */
0x0000f7af, 0x00000066, /* 00000066 //C 0f7af  2118 db	102 */
0x0000f7b0, 0x00000072, /* 00000072 //C 0f7b0  2118 db	114 */
0x0000f7b1, 0x0000006f, /* 0000006f //C 0f7b1  2118 db	111 */
0x0000f7b2, 0x0000006d, /* 0000006d //C 0f7b2  2118 db	109 */
0x0000f7b3, 0x00000020, /* 00000020 //C 0f7b3  2118 db	32 */
0x0000f7b4, 0x00000061, /* 00000061 //C 0f7b4  2118 db	97 */
0x0000f7b5, 0x00000064, /* 00000064 //C 0f7b5  2118 db	100 */
0x0000f7b6, 0x00000064, /* 00000064 //C 0f7b6  2118 db	100 */
0x0000f7b7, 0x00000072, /* 00000072 //C 0f7b7  2118 db	114 */
0x0000f7b8, 0x00000065, /* 00000065 //C 0f7b8  2118 db	101 */
0x0000f7b9, 0x00000073, /* 00000073 //C 0f7b9  2118 db	115 */
0x0000f7ba, 0x00000073, /* 00000073 //C 0f7ba  2118 db	115 */
0x0000f7bb, 0x0000000a, /* 0000000a //C 0f7bb  2118 db	10 */
0x0000f7bc, 0x00000000, /* 00000000 //C 0f7bc  2118 db */
0x0000f7bd, 0x00000072, /* 00000072 //C 0f7bd  2119 db	114 */
0x0000f7be, 0x0000005b, /* 0000005b //C 0f7be  2119 db	91 */
0x0000f7bf, 0x00000065, /* 00000065 //C 0f7bf  2119 db	101 */
0x0000f7c0, 0x00000067, /* 00000067 //C 0f7c0  2119 db	103 */
0x0000f7c1, 0x0000005b, /* 0000005b //C 0f7c1  2119 db	91 */
0x0000f7c2, 0x00000073, /* 00000073 //C 0f7c2  2119 db	115 */
0x0000f7c3, 0x0000005d, /* 0000005d //C 0f7c3  2119 db	93 */
0x0000f7c4, 0x0000005d, /* 0000005d //C 0f7c4  2119 db	93 */
0x0000f7c5, 0x00000020, /* 00000020 //C 0f7c5  2119 db	32 */
0x0000f7c6, 0x00000020, /* 00000020 //C 0f7c6  2119 db	32 */
0x0000f7c7, 0x00000020, /* 00000020 //C 0f7c7  2119 db	32 */
0x0000f7c8, 0x00000020, /* 00000020 //C 0f7c8  2119 db	32 */
0x0000f7c9, 0x00000020, /* 00000020 //C 0f7c9  2119 db	32 */
0x0000f7ca, 0x00000020, /* 00000020 //C 0f7ca  2119 db	32 */
0x0000f7cb, 0x00000020, /* 00000020 //C 0f7cb  2119 db	32 */
0x0000f7cc, 0x00000020, /* 00000020 //C 0f7cc  2119 db	32 */
0x0000f7cd, 0x00000020, /* 00000020 //C 0f7cd  2119 db	32 */
0x0000f7ce, 0x00000020, /* 00000020 //C 0f7ce  2119 db	32 */
0x0000f7cf, 0x00000050, /* 00000050 //C 0f7cf  2119 db	80 */
0x0000f7d0, 0x00000072, /* 00000072 //C 0f7d0  2119 db	114 */
0x0000f7d1, 0x00000069, /* 00000069 //C 0f7d1  2119 db	105 */
0x0000f7d2, 0x0000006e, /* 0000006e //C 0f7d2  2119 db	110 */
0x0000f7d3, 0x00000074, /* 00000074 //C 0f7d3  2119 db	116 */
0x0000f7d4, 0x00000020, /* 00000020 //C 0f7d4  2119 db	32 */
0x0000f7d5, 0x00000072, /* 00000072 //C 0f7d5  2119 db	114 */
0x0000f7d6, 0x00000065, /* 00000065 //C 0f7d6  2119 db	101 */
0x0000f7d7, 0x00000067, /* 00000067 //C 0f7d7  2119 db	103 */
0x0000f7d8, 0x00000069, /* 00000069 //C 0f7d8  2119 db	105 */
0x0000f7d9, 0x00000073, /* 00000073 //C 0f7d9  2119 db	115 */
0x0000f7da, 0x00000074, /* 00000074 //C 0f7da  2119 db	116 */
0x0000f7db, 0x00000065, /* 00000065 //C 0f7db  2119 db	101 */
0x0000f7dc, 0x00000072, /* 00000072 //C 0f7dc  2119 db	114 */
0x0000f7dd, 0x00000073, /* 00000073 //C 0f7dd  2119 db	115 */
0x0000f7de, 0x0000000a, /* 0000000a //C 0f7de  2119 db	10 */
0x0000f7df, 0x00000000, /* 00000000 //C 0f7df  2119 db */
0x0000f7e0, 0x00000072, /* 00000072 //C 0f7e0  2120 db	114 */
0x0000f7e1, 0x00000058, /* 00000058 //C 0f7e1  2120 db	88 */
0x0000f7e2, 0x00000020, /* 00000020 //C 0f7e2  2120 db	32 */
0x0000f7e3, 0x0000005b, /* 0000005b //C 0f7e3  2120 db	91 */
0x0000f7e4, 0x00000076, /* 00000076 //C 0f7e4  2120 db	118 */
0x0000f7e5, 0x00000061, /* 00000061 //C 0f7e5  2120 db	97 */
0x0000f7e6, 0x0000006c, /* 0000006c //C 0f7e6  2120 db	108 */
0x0000f7e7, 0x0000005d, /* 0000005d //C 0f7e7  2120 db	93 */
0x0000f7e8, 0x00000020, /* 00000020 //C 0f7e8  2120 db	32 */
0x0000f7e9, 0x00000020, /* 00000020 //C 0f7e9  2120 db	32 */
0x0000f7ea, 0x00000020, /* 00000020 //C 0f7ea  2120 db	32 */
0x0000f7eb, 0x00000020, /* 00000020 //C 0f7eb  2120 db	32 */
0x0000f7ec, 0x00000020, /* 00000020 //C 0f7ec  2120 db	32 */
0x0000f7ed, 0x00000020, /* 00000020 //C 0f7ed  2120 db	32 */
0x0000f7ee, 0x00000020, /* 00000020 //C 0f7ee  2120 db	32 */
0x0000f7ef, 0x00000020, /* 00000020 //C 0f7ef  2120 db	32 */
0x0000f7f0, 0x00000020, /* 00000020 //C 0f7f0  2120 db	32 */
0x0000f7f1, 0x00000020, /* 00000020 //C 0f7f1  2120 db	32 */
0x0000f7f2, 0x00000047, /* 00000047 //C 0f7f2  2120 db	71 */
0x0000f7f3, 0x00000065, /* 00000065 //C 0f7f3  2120 db	101 */
0x0000f7f4, 0x00000074, /* 00000074 //C 0f7f4  2120 db	116 */
0x0000f7f5, 0x0000002f, /* 0000002f //C 0f7f5  2120 db	47 */
0x0000f7f6, 0x00000073, /* 00000073 //C 0f7f6  2120 db	115 */
0x0000f7f7, 0x00000065, /* 00000065 //C 0f7f7  2120 db	101 */
0x0000f7f8, 0x00000074, /* 00000074 //C 0f7f8  2120 db	116 */
0x0000f7f9, 0x00000020, /* 00000020 //C 0f7f9  2120 db	32 */
0x0000f7fa, 0x00000052, /* 00000052 //C 0f7fa  2120 db	82 */
0x0000f7fb, 0x00000058, /* 00000058 //C 0f7fb  2120 db	88 */
0x0000f7fc, 0x0000000a, /* 0000000a //C 0f7fc  2120 db	10 */
0x0000f7fd, 0x00000000, /* 00000000 //C 0f7fd  2120 db */
0x0000f7fe, 0x00000073, /* 00000073 //C 0f7fe  2121 db	115 */
0x0000f7ff, 0x00000070, /* 00000070 //C 0f7ff  2121 db	112 */
0x0000f800, 0x00000020, /* 00000020 //C 0f800  2121 db	32 */
0x0000f801, 0x0000005b, /* 0000005b //C 0f801  2121 db	91 */
0x0000f802, 0x00000076, /* 00000076 //C 0f802  2121 db	118 */
0x0000f803, 0x00000061, /* 00000061 //C 0f803  2121 db	97 */
0x0000f804, 0x0000006c, /* 0000006c //C 0f804  2121 db	108 */
0x0000f805, 0x0000005d, /* 0000005d //C 0f805  2121 db	93 */
0x0000f806, 0x00000020, /* 00000020 //C 0f806  2121 db	32 */
0x0000f807, 0x00000020, /* 00000020 //C 0f807  2121 db	32 */
0x0000f808, 0x00000020, /* 00000020 //C 0f808  2121 db	32 */
0x0000f809, 0x00000020, /* 00000020 //C 0f809  2121 db	32 */
0x0000f80a, 0x00000020, /* 00000020 //C 0f80a  2121 db	32 */
0x0000f80b, 0x00000020, /* 00000020 //C 0f80b  2121 db	32 */
0x0000f80c, 0x00000020, /* 00000020 //C 0f80c  2121 db	32 */
0x0000f80d, 0x00000020, /* 00000020 //C 0f80d  2121 db	32 */
0x0000f80e, 0x00000020, /* 00000020 //C 0f80e  2121 db	32 */
0x0000f80f, 0x00000020, /* 00000020 //C 0f80f  2121 db	32 */
0x0000f810, 0x00000047, /* 00000047 //C 0f810  2121 db	71 */
0x0000f811, 0x00000065, /* 00000065 //C 0f811  2121 db	101 */
0x0000f812, 0x00000074, /* 00000074 //C 0f812  2121 db	116 */
0x0000f813, 0x0000002f, /* 0000002f //C 0f813  2121 db	47 */
0x0000f814, 0x00000073, /* 00000073 //C 0f814  2121 db	115 */
0x0000f815, 0x00000065, /* 00000065 //C 0f815  2121 db	101 */
0x0000f816, 0x00000074, /* 00000074 //C 0f816  2121 db	116 */
0x0000f817, 0x00000020, /* 00000020 //C 0f817  2121 db	32 */
0x0000f818, 0x00000052, /* 00000052 //C 0f818  2121 db	82 */
0x0000f819, 0x00000031, /* 00000031 //C 0f819  2121 db	49 */
0x0000f81a, 0x00000033, /* 00000033 //C 0f81a  2121 db	51 */
0x0000f81b, 0x0000000a, /* 0000000a //C 0f81b  2121 db	10 */
0x0000f81c, 0x00000000, /* 00000000 //C 0f81c  2121 db */
0x0000f81d, 0x0000006c, /* 0000006c //C 0f81d  2122 db	108 */
0x0000f81e, 0x00000072, /* 00000072 //C 0f81e  2122 db	114 */
0x0000f81f, 0x00000020, /* 00000020 //C 0f81f  2122 db	32 */
0x0000f820, 0x0000005b, /* 0000005b //C 0f820  2122 db	91 */
0x0000f821, 0x00000076, /* 00000076 //C 0f821  2122 db	118 */
0x0000f822, 0x00000061, /* 00000061 //C 0f822  2122 db	97 */
0x0000f823, 0x0000006c, /* 0000006c //C 0f823  2122 db	108 */
0x0000f824, 0x0000005d, /* 0000005d //C 0f824  2122 db	93 */
0x0000f825, 0x00000020, /* 00000020 //C 0f825  2122 db	32 */
0x0000f826, 0x00000020, /* 00000020 //C 0f826  2122 db	32 */
0x0000f827, 0x00000020, /* 00000020 //C 0f827  2122 db	32 */
0x0000f828, 0x00000020, /* 00000020 //C 0f828  2122 db	32 */
0x0000f829, 0x00000020, /* 00000020 //C 0f829  2122 db	32 */
0x0000f82a, 0x00000020, /* 00000020 //C 0f82a  2122 db	32 */
0x0000f82b, 0x00000020, /* 00000020 //C 0f82b  2122 db	32 */
0x0000f82c, 0x00000020, /* 00000020 //C 0f82c  2122 db	32 */
0x0000f82d, 0x00000020, /* 00000020 //C 0f82d  2122 db	32 */
0x0000f82e, 0x00000020, /* 00000020 //C 0f82e  2122 db	32 */
0x0000f82f, 0x00000047, /* 00000047 //C 0f82f  2122 db	71 */
0x0000f830, 0x00000065, /* 00000065 //C 0f830  2122 db	101 */
0x0000f831, 0x00000074, /* 00000074 //C 0f831  2122 db	116 */
0x0000f832, 0x0000002f, /* 0000002f //C 0f832  2122 db	47 */
0x0000f833, 0x00000073, /* 00000073 //C 0f833  2122 db	115 */
0x0000f834, 0x00000065, /* 00000065 //C 0f834  2122 db	101 */
0x0000f835, 0x00000074, /* 00000074 //C 0f835  2122 db	116 */
0x0000f836, 0x00000020, /* 00000020 //C 0f836  2122 db	32 */
0x0000f837, 0x00000052, /* 00000052 //C 0f837  2122 db	82 */
0x0000f838, 0x00000031, /* 00000031 //C 0f838  2122 db	49 */
0x0000f839, 0x00000034, /* 00000034 //C 0f839  2122 db	52 */
0x0000f83a, 0x0000000a, /* 0000000a //C 0f83a  2122 db	10 */
0x0000f83b, 0x00000000, /* 00000000 //C 0f83b  2122 db */
0x0000f83c, 0x00000070, /* 00000070 //C 0f83c  2123 db	112 */
0x0000f83d, 0x00000063, /* 00000063 //C 0f83d  2123 db	99 */
0x0000f83e, 0x00000020, /* 00000020 //C 0f83e  2123 db	32 */
0x0000f83f, 0x0000005b, /* 0000005b //C 0f83f  2123 db	91 */
0x0000f840, 0x00000076, /* 00000076 //C 0f840  2123 db	118 */
0x0000f841, 0x00000061, /* 00000061 //C 0f841  2123 db	97 */
0x0000f842, 0x0000006c, /* 0000006c //C 0f842  2123 db	108 */
0x0000f843, 0x0000005d, /* 0000005d //C 0f843  2123 db	93 */
0x0000f844, 0x00000020, /* 00000020 //C 0f844  2123 db	32 */
0x0000f845, 0x00000020, /* 00000020 //C 0f845  2123 db	32 */
0x0000f846, 0x00000020, /* 00000020 //C 0f846  2123 db	32 */
0x0000f847, 0x00000020, /* 00000020 //C 0f847  2123 db	32 */
0x0000f848, 0x00000020, /* 00000020 //C 0f848  2123 db	32 */
0x0000f849, 0x00000020, /* 00000020 //C 0f849  2123 db	32 */
0x0000f84a, 0x00000020, /* 00000020 //C 0f84a  2123 db	32 */
0x0000f84b, 0x00000020, /* 00000020 //C 0f84b  2123 db	32 */
0x0000f84c, 0x00000020, /* 00000020 //C 0f84c  2123 db	32 */
0x0000f84d, 0x00000020, /* 00000020 //C 0f84d  2123 db	32 */
0x0000f84e, 0x00000047, /* 00000047 //C 0f84e  2123 db	71 */
0x0000f84f, 0x00000065, /* 00000065 //C 0f84f  2123 db	101 */
0x0000f850, 0x00000074, /* 00000074 //C 0f850  2123 db	116 */
0x0000f851, 0x0000002f, /* 0000002f //C 0f851  2123 db	47 */
0x0000f852, 0x00000073, /* 00000073 //C 0f852  2123 db	115 */
0x0000f853, 0x00000065, /* 00000065 //C 0f853  2123 db	101 */
0x0000f854, 0x00000074, /* 00000074 //C 0f854  2123 db	116 */
0x0000f855, 0x00000020, /* 00000020 //C 0f855  2123 db	32 */
0x0000f856, 0x00000052, /* 00000052 //C 0f856  2123 db	82 */
0x0000f857, 0x00000031, /* 00000031 //C 0f857  2123 db	49 */
0x0000f858, 0x00000035, /* 00000035 //C 0f858  2123 db	53 */
0x0000f859, 0x0000000a, /* 0000000a //C 0f859  2123 db	10 */
0x0000f85a, 0x00000000, /* 00000000 //C 0f85a  2123 db */
0x0000f85b, 0x00000066, /* 00000066 //C 0f85b  2124 db	102 */
0x0000f85c, 0x00000020, /* 00000020 //C 0f85c  2124 db	32 */
0x0000f85d, 0x0000005b, /* 0000005b //C 0f85d  2124 db	91 */
0x0000f85e, 0x00000076, /* 00000076 //C 0f85e  2124 db	118 */
0x0000f85f, 0x00000061, /* 00000061 //C 0f85f  2124 db	97 */
0x0000f860, 0x0000006c, /* 0000006c //C 0f860  2124 db	108 */
0x0000f861, 0x0000005d, /* 0000005d //C 0f861  2124 db	93 */
0x0000f862, 0x00000020, /* 00000020 //C 0f862  2124 db	32 */
0x0000f863, 0x00000020, /* 00000020 //C 0f863  2124 db	32 */
0x0000f864, 0x00000020, /* 00000020 //C 0f864  2124 db	32 */
0x0000f865, 0x00000020, /* 00000020 //C 0f865  2124 db	32 */
0x0000f866, 0x00000020, /* 00000020 //C 0f866  2124 db	32 */
0x0000f867, 0x00000020, /* 00000020 //C 0f867  2124 db	32 */
0x0000f868, 0x00000020, /* 00000020 //C 0f868  2124 db	32 */
0x0000f869, 0x00000020, /* 00000020 //C 0f869  2124 db	32 */
0x0000f86a, 0x00000020, /* 00000020 //C 0f86a  2124 db	32 */
0x0000f86b, 0x00000020, /* 00000020 //C 0f86b  2124 db	32 */
0x0000f86c, 0x00000020, /* 00000020 //C 0f86c  2124 db	32 */
0x0000f86d, 0x00000047, /* 00000047 //C 0f86d  2124 db	71 */
0x0000f86e, 0x00000065, /* 00000065 //C 0f86e  2124 db	101 */
0x0000f86f, 0x00000074, /* 00000074 //C 0f86f  2124 db	116 */
0x0000f870, 0x0000002f, /* 0000002f //C 0f870  2124 db	47 */
0x0000f871, 0x00000073, /* 00000073 //C 0f871  2124 db	115 */
0x0000f872, 0x00000065, /* 00000065 //C 0f872  2124 db	101 */
0x0000f873, 0x00000074, /* 00000074 //C 0f873  2124 db	116 */
0x0000f874, 0x00000020, /* 00000020 //C 0f874  2124 db	32 */
0x0000f875, 0x00000066, /* 00000066 //C 0f875  2124 db	102 */
0x0000f876, 0x0000006c, /* 0000006c //C 0f876  2124 db	108 */
0x0000f877, 0x00000061, /* 00000061 //C 0f877  2124 db	97 */
0x0000f878, 0x00000067, /* 00000067 //C 0f878  2124 db	103 */
0x0000f879, 0x00000073, /* 00000073 //C 0f879  2124 db	115 */
0x0000f87a, 0x0000000a, /* 0000000a //C 0f87a  2124 db	10 */
0x0000f87b, 0x00000000, /* 00000000 //C 0f87b  2124 db */
0x0000f87c, 0x00000068, /* 00000068 //C 0f87c  2125 db	104 */
0x0000f87d, 0x0000005b, /* 0000005b //C 0f87d  2125 db	91 */
0x0000f87e, 0x00000065, /* 00000065 //C 0f87e  2125 db	101 */
0x0000f87f, 0x0000006c, /* 0000006c //C 0f87f  2125 db	108 */
0x0000f880, 0x00000070, /* 00000070 //C 0f880  2125 db	112 */
0x0000f881, 0x0000005d, /* 0000005d //C 0f881  2125 db	93 */
0x0000f882, 0x0000002c, /* 0000002c //C 0f882  2125 db	44 */
0x0000f883, 0x0000003f, /* 0000003f //C 0f883  2125 db	63 */
0x0000f884, 0x00000020, /* 00000020 //C 0f884  2125 db	32 */
0x0000f885, 0x00000020, /* 00000020 //C 0f885  2125 db	32 */
0x0000f886, 0x00000020, /* 00000020 //C 0f886  2125 db	32 */
0x0000f887, 0x00000020, /* 00000020 //C 0f887  2125 db	32 */
0x0000f888, 0x00000020, /* 00000020 //C 0f888  2125 db	32 */
0x0000f889, 0x00000020, /* 00000020 //C 0f889  2125 db	32 */
0x0000f88a, 0x00000020, /* 00000020 //C 0f88a  2125 db	32 */
0x0000f88b, 0x00000020, /* 00000020 //C 0f88b  2125 db	32 */
0x0000f88c, 0x00000020, /* 00000020 //C 0f88c  2125 db	32 */
0x0000f88d, 0x00000020, /* 00000020 //C 0f88d  2125 db	32 */
0x0000f88e, 0x00000048, /* 00000048 //C 0f88e  2125 db	72 */
0x0000f88f, 0x00000065, /* 00000065 //C 0f88f  2125 db	101 */
0x0000f890, 0x0000006c, /* 0000006c //C 0f890  2125 db	108 */
0x0000f891, 0x00000070, /* 00000070 //C 0f891  2125 db	112 */
0x0000f892, 0x0000000a, /* 0000000a //C 0f892  2125 db	10 */
0x0000f893, 0x00000000, /* 00000000 //C 0f893  2125 db */
0x0000f894, 0x00000000, /* 00000000 //C 0f894  2126 dd	0 */
0x0000f895, 0x00000000, /* 00000000 //C 0f895  2132 ds	0x40 */
0x0000f8d5, 0x00000000, /* 00000000 //C 0f8d5  2134 ds	1 */
0x0000f8d6, 0x00000000, /* 00000000 //C 0f8d6     3  */
0xffffffff, 0xffffffff
};
