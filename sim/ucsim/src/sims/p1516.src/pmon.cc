#include "pmon.h"

const char * p12cpu_version= "2.2.12";

t_mem pmon[]= {
0x00000000, 0x01f2f03f, /* 01f2f03f //C 00000    24 jmp	cold_start */
0x0000f000, 0x01f2f028, /* 01f2f028 //C 0f000    29 _f000:	jmp	callin */
0x0000f001, 0x01f2f019, /* 01f2f019 //C 0f001    30 _f001:	jmp	enter_by_uart */
0x0000f002, 0x01f2f52f, /* 01f2f52f //C 0f002    31 _f002:	jmp	getchar */
0x0000f003, 0x01f2f676, /* 01f2f676 //C 0f003    32 _f003:	jmp	version */
0x0000f004, 0x01f2f5f5, /* 01f2f5f5 //C 0f004    33 _f004:	jmp	itobcd */
0x0000f005, 0x01f2f03f, /* 01f2f03f //C 0f005    34 _f005:	jmp	cold_start */
0x0000f006, 0x01f2f441, /* 01f2f441 //C 0f006    35 _f006:	jmp	strchr */
0x0000f007, 0x01f2f4b3, /* 01f2f4b3 //C 0f007    36 _f007:	jmp	streq */
0x0000f008, 0x01f2f524, /* 01f2f524 //C 0f008    37 _f008:	jmp	check_uart */
0x0000f009, 0x01f2f4c3, /* 01f2f4c3 //C 0f009    38 _f009:	jmp	hexchar2value */
0x0000f00a, 0x01f2f4f2, /* 01f2f4f2 //C 0f00a    39 _f00a:	jmp	value2hexchar */
0x0000f00b, 0x01f2f4f6, /* 01f2f4f6 //C 0f00b    40 _f00b:	jmp	htoi */
0x0000f00c, 0x01f2f4bb, /* 01f2f4bb //C 0f00c    41 _f00c:	jmp	strieq */
0x0000f00d, 0x01f2f52c, /* 01f2f52c //C 0f00d    42 _f00d:	jmp	read */
0x0000f00e, 0x01f2f533, /* 01f2f533 //C 0f00e    43 _f00e:	jmp	putchar */
0x0000f00f, 0x01f2f53a, /* 01f2f53a //C 0f00f    44 _f00f:	jmp	prints */
0x0000f010, 0x01f2f570, /* 01f2f570 //C 0f010    45 _f010:	jmp	printsnl */
0x0000f011, 0x01f2f577, /* 01f2f577 //C 0f011    46 _f011:	jmp	print_vhex */
0x0000f012, 0x01f2f558, /* 01f2f558 //C 0f012    47 _f012:	jmp	pes */
0x0000f013, 0x01f2f60c, /* 01f2f60c //C 0f013    48 _f013:	jmp	printd */
0x0000f014, 0x01f2f611, /* 01f2f611 //C 0f014    49 _f014:	jmp	printf */
0x0000f015, 0x01f2f66d, /* 01f2f66d //C 0f015    50 _f015:	jmp	pesf */
0x0000f016, 0x01f2f67c, /* 01f2f67c //C 0f016    51 _f016:	jmp	ascii2seg */
0x0000f017, 0x01f2f3f3, /* 01f2f3f3 //C 0f017    52 _f017:	jmp	strunpack */
0x0000f018, 0x01f2f412, /* 01f2f412 //C 0f018    53 _f018:	jmp	strpack */
0x0000f019, 0x0d0d0000, /* 0d0d0000 //C 0f019    57 push	r0 */
0x0000f01a, 0x020e0000, /* 020e0000 //C 0f01a    58 getf	r0 */
0x0000f01b, 0x0d0d0000, /* 0d0d0000 //C 0f01b    59 push	r0 */
0x0000f01c, 0x0700ff42, /* 0700ff42 //C 0f01c    60 ld	r0,UART_RSTAT */
0x0000f01d, 0x010c0001, /* 010c0001 //C 0f01d    61 test	r0,1 */
0x0000f01e, 0x21f2f023, /* 21f2f023 //C 0f01e    62 jnz	ebu_callin */
0x0000f01f, 0x0f0d0000, /* 0f0d0000 //C 0f01f    64 pop	r0 */
0x0000f020, 0x020f0000, /* 020f0000 //C 0f020    65 setf	r0 */
0x0000f021, 0x0f0d0000, /* 0f0d0000 //C 0f021    66 pop	r0 */
0x0000f022, 0x00f00e00, /* 00f00e00 //C 0f022    67 ret */
0x0000f023, 0x0700ff40, /* 0700ff40 //C 0f023    69 ld	r0,UART_DR */
0x0000f024, 0x0f0d0000, /* 0f0d0000 //C 0f024    70 pop	r0 */
0x0000f025, 0x020f0000, /* 020f0000 //C 0f025    71 setf	r0 */
0x0000f026, 0x0f0d0000, /* 0f0d0000 //C 0f026    72 pop	r0 */
0x0000f027, 0x01f2f028, /* 01f2f028 //C 0f027    73 jmp	callin */
0x0000f028, 0x0600f6f7, /* 0600f6f7 //C 0f028    76 st	r0,reg0 */
0x0000f029, 0x0610f6f8, /* 0610f6f8 //C 0f029    77 st	r1,reg1 */
0x0000f02a, 0x0620f6f9, /* 0620f6f9 //C 0f02a    78 st	r2,reg2 */
0x0000f02b, 0x0630f6fa, /* 0630f6fa //C 0f02b    79 st	r3,reg3 */
0x0000f02c, 0x0640f6fb, /* 0640f6fb //C 0f02c    80 st	r4,reg4 */
0x0000f02d, 0x0650f6fc, /* 0650f6fc //C 0f02d    81 st	r5,reg5 */
0x0000f02e, 0x0660f6fd, /* 0660f6fd //C 0f02e    82 st	r6,reg6 */
0x0000f02f, 0x0670f6fe, /* 0670f6fe //C 0f02f    83 st	r7,reg7 */
0x0000f030, 0x0680f6ff, /* 0680f6ff //C 0f030    84 st	r8,reg8 */
0x0000f031, 0x0690f700, /* 0690f700 //C 0f031    85 st	r9,reg9 */
0x0000f032, 0x06a0f701, /* 06a0f701 //C 0f032    86 st	r10,reg10 */
0x0000f033, 0x06b0f702, /* 06b0f702 //C 0f033    87 st	r11,reg11 */
0x0000f034, 0x06c0f703, /* 06c0f703 //C 0f034    88 st	r12,reg12 */
0x0000f035, 0x06d0f704, /* 06d0f704 //C 0f035    89 st	r13,reg13 */
0x0000f036, 0x06e0f705, /* 06e0f705 //C 0f036    90 st	r14,reg14 */
0x0000f037, 0x06e0f706, /* 06e0f706 //C 0f037    91 st	r14,reg15 */
0x0000f038, 0x020e0000, /* 020e0000 //C 0f038    92 getf	r0 */
0x0000f039, 0x0600f707, /* 0600f707 //C 0f039    93 st	r0,regf */
0x0000f03a, 0x01020001, /* 01020001 //C 0f03a    94 mvzl	r0,1 */
0x0000f03b, 0x0600f6f5, /* 0600f6f5 //C 0f03b    95 st	r0,called */
0x0000f03c, 0x01f2f049, /* 01f2f049 //C 0f03c    96 jmp	common_start */
0x0000f03d, 0x01f2f049, /* 01f2f049 //C 0f03d    98 jmp	common_start */
0x0000f03e, 0x01f2f03f, /* 01f2f03f //C 0f03e   100 jmp	cold_start */
0x0000f03f, 0x01020000, /* 01020000 //C 0f03f   102 mvzl	r0,0 */
0x0000f040, 0x0600f6f5, /* 0600f6f5 //C 0f040   103 st	r0,called */
0x0000f041, 0x0102f03e, /* 0102f03e //C 0f041   104 mvzl	r0,def_zero	; restore jmp to monitor at zero */
0x0000f042, 0x0e000000, /* 0e000000 //C 0f042   105 ld	r0,r0 */
0x0000f043, 0x06000000, /* 06000000 //C 0f043   106 st	r0,0 */
0x0000f044, 0x01020000, /* 01020000 //C 0f044   107 mvzl	r0,0		; def values of some regs */
0x0000f045, 0x0600f707, /* 0600f707 //C 0f045   108 st	r0,regf		; FALGS= 0 */
0x0000f046, 0x0102f7ff, /* 0102f7ff //C 0f046   109 mvzl	r0,0xf7ff	; R13= 0xf7ff */
0x0000f047, 0x0600f704, /* 0600f704 //C 0f047   110 st	r0,reg13 */
0x0000f048, 0x01f2f049, /* 01f2f049 //C 0f048   111 jmp	common_start */
0x0000f049, 0x01d2f9b0, /* 01d2f9b0 //C 0f049   114 mvzl	sp,stack_end */
0x0000f04a, 0x01020001, /* 01020001 //C 0f04a   115 mvzl	r0,1 */
0x0000f04b, 0x0600f6f4, /* 0600f6f4 //C 0f04b   116 st	r0,echo */
0x0000f04c, 0x01020000, /* 01020000 //C 0f04c   117 mvzl	r0,0 */
0x0000f04d, 0x020f0000, /* 020f0000 //C 0f04d   118 setf	r0 */
0x0000f04e, 0x0710ff41, /* 0710ff41 //C 0f04e   123 ld	r1,UART_CTRL	; check if transmit is enabled */
0x0000f04f, 0x011c0002, /* 011c0002 //C 0f04f   124 test	r1,2 */
0x0000f050, 0x11f2f054, /* 11f2f054 //C 0f050   125 jz	tr_is_off */
0x0000f051, 0x0710ff43, /* 0710ff43 //C 0f051   127 ld	r1,UART_TSTAT	; if transmit is ongoing */
0x0000f052, 0x011c0001, /* 011c0001 //C 0f052   128 test	r1,1		; wait it to finish */
0x0000f053, 0x11f2f051, /* 11f2f051 //C 0f053   129 jz	wait_uart_tr */
0x0000f054, 0x01120003, /* 01120003 //C 0f054   131 mvzl	r1,3		; turn on rx and tx */
0x0000f055, 0x0610ff41, /* 0610ff41 //C 0f055   132 st	r1,UART_CTRL */
0x0000f056, 0xf400f558, /* f400f558 //C 0f056   135 ces	pes */
0x0000f057, 0x0000001b, /* 0000001b //C 0f057   136 db	27 */
0x0000f058, 0x0000005b, /* 0000005b //C 0f058   136 db	91 */
0x0000f059, 0x0000003f, /* 0000003f //C 0f059   136 db	63 */
0x0000f05a, 0x00000032, /* 00000032 //C 0f05a   136 db	50 */
0x0000f05b, 0x00000035, /* 00000035 //C 0f05b   136 db	53 */
0x0000f05c, 0x00000068, /* 00000068 //C 0f05c   136 db	104 */
0x0000f05d, 0x00000000, /* 00000000 //C 0f05d   136 db */
0x0000f05e, 0xf400f558, /* f400f558 //C 0f05e   137 ces	pes */
0x0000f05f, 0x0000001b, /* 0000001b //C 0f05f   138 db	27 */
0x0000f060, 0x0000005b, /* 0000005b //C 0f060   138 db	91 */
0x0000f061, 0x00000034, /* 00000034 //C 0f061   138 db	52 */
0x0000f062, 0x00000030, /* 00000030 //C 0f062   138 db	48 */
0x0000f063, 0x0000003b, /* 0000003b //C 0f063   138 db	59 */
0x0000f064, 0x00000033, /* 00000033 //C 0f064   138 db	51 */
0x0000f065, 0x00000037, /* 00000037 //C 0f065   138 db	55 */
0x0000f066, 0x0000006d, /* 0000006d //C 0f066   138 db	109 */
0x0000f067, 0x00000000, /* 00000000 //C 0f067   138 db */
0x0000f068, 0x0102000a, /* 0102000a //C 0f068   139 mvzl	r0,LF */
0x0000f069, 0x0400f533, /* 0400f533 //C 0f069   140 call	putchar */
0x0000f06a, 0x06020100, /* 06020100 //C 0f06a   141 rds	r0,sver */
0x0000f06b, 0x01120001, /* 01120001 //C 0f06b   142 mvzl	r1,version_main */
0x0000f06c, 0x01220004, /* 01220004 //C 0f06c   143 mvzl	r2,version_sub */
0x0000f06d, 0x0631c002, /* 0631c002 //C 0f06d   144 getbz	r3,r0,2 */
0x0000f06e, 0x0641c001, /* 0641c001 //C 0f06e   145 getbz	r4,r0,1 */
0x0000f06f, 0x0651c000, /* 0651c000 //C 0f06f   146 getbz	r5,r0,0 */
0x0000f070, 0x0102f708, /* 0102f708 //C 0f070   147 mvzl	r0,msg_start */
0x0000f071, 0x0400f611, /* 0400f611 //C 0f071   148 call	printf */
0x0000f072, 0x0700f6f5, /* 0700f6f5 //C 0f072   150 ld	r0,called */
0x0000f073, 0x020b0000, /* 020b0000 //C 0f073   151 sz	r0 */
0x0000f074, 0x11f2f07e, /* 11f2f07e //C 0f074   152 jz	no_called_from */
0x0000f075, 0x0102000a, /* 0102000a //C 0f075   153 mvzl	r0,LF */
0x0000f076, 0x0400f533, /* 0400f533 //C 0f076   154 call	putchar */
0x0000f077, 0x0102f729, /* 0102f729 //C 0f077   155 mvzl	r0,msg_stopat */
0x0000f078, 0x0400f53a, /* 0400f53a //C 0f078   156 call	prints */
0x0000f079, 0x0700f705, /* 0700f705 //C 0f079   157 ld	r0,reg14 */
0x0000f07a, 0x01120004, /* 01120004 //C 0f07a   158 mvzl	r1,4 */
0x0000f07b, 0x0400f577, /* 0400f577 //C 0f07b   159 call	print_vhex */
0x0000f07c, 0x0102000a, /* 0102000a //C 0f07c   160 mvzl	r0,LF */
0x0000f07d, 0x0400f533, /* 0400f533 //C 0f07d   161 call	putchar */
0x0000f07e, 0x0400f080, /* 0400f080 //C 0f07e   165 call	setup_line */
0x0000f07f, 0x01f2f089, /* 01f2f089 //C 0f07f   169 jmp	main */
0x0000f080, 0x0ded0000, /* 0ded0000 //C 0f080   175 push	lr */
0x0000f081, 0x01120000, /* 01120000 //C 0f081   176 mvzl	r1,0		; lptr= 0 */
0x0000f082, 0x0610f6ed, /* 0610f6ed //C 0f082   177 st	r1,line_ptr */
0x0000f083, 0x0610f689, /* 0610f689 //C 0f083   178 st	r1,line		; line[0]= 0 */
0x0000f084, 0x01120000, /* 01120000 //C 0f084   179 mvzl	r1,0		; at_eol= 0 */
0x0000f085, 0x0610f6ee, /* 0610f6ee //C 0f085   180 st	r1,at_eol */
0x0000f086, 0x0102f737, /* 0102f737 //C 0f086   182 mvzl	r0,prompt */
0x0000f087, 0x0400f53a, /* 0400f53a //C 0f087   183 call	prints */
0x0000f088, 0x0ffd0000, /* 0ffd0000 //C 0f088   184 pop	pc */
0x0000f089, 0x0400f524, /* 0400f524 //C 0f089   191 call	check_uart */
0x0000f08a, 0x41f2f090, /* 41f2f090 //C 0f08a   192 C0 jmp	no_input */
0x0000f08b, 0x0400f52c, /* 0400f52c //C 0f08b   194 call	read */
0x0000f08c, 0x0400f091, /* 0400f091 //C 0f08c   195 call	proc_input */
0x0000f08d, 0x41f2f090, /* 41f2f090 //C 0f08d   196 C0 jmp	no_line */
0x0000f08e, 0x0400f0c2, /* 0400f0c2 //C 0f08e   199 call	proc_line */
0x0000f08f, 0x0400f080, /* 0400f080 //C 0f08f   200 call	setup_line */
0x0000f090, 0x01f2f089, /* 01f2f089 //C 0f090   203 jmp	main */
0x0000f091, 0x0ded0000, /* 0ded0000 //C 0f091   209 push	lr */
0x0000f092, 0x0108000a, /* 0108000a //C 0f092   210 cmp	r0,LF */
0x0000f093, 0x11f2f0bc, /* 11f2f0bc //C 0f093   211 EQ jmp	got_eol */
0x0000f094, 0x0108000d, /* 0108000d //C 0f094   212 cmp	r0,CR */
0x0000f095, 0x11f2f0bc, /* 11f2f0bc //C 0f095   213 EQ jmp	got_eol */
0x0000f096, 0x01080008, /* 01080008 //C 0f096   214 cmp	r0,8 */
0x0000f097, 0x11f2f09b, /* 11f2f09b //C 0f097   215 jz	got_BS */
0x0000f098, 0x0108007f, /* 0108007f //C 0f098   216 cmp	r0,127 */
0x0000f099, 0x11f2f09b, /* 11f2f09b //C 0f099   217 jz	got_DEL */
0x0000f09a, 0x01f2f0a7, /* 01f2f0a7 //C 0f09a   218 jmp	got_char */
0x0000f09b, 0x0700f6ed, /* 0700f6ed //C 0f09b   221 ld	r0,line_ptr */
0x0000f09c, 0x020b0000, /* 020b0000 //C 0f09c   222 sz	r0 */
0x0000f09d, 0x11f2f0a5, /* 11f2f0a5 //C 0f09d   223 jz	got_done */
0x0000f09e, 0x01060001, /* 01060001 //C 0f09e   224 sub	r0,1 */
0x0000f09f, 0x0600f6ed, /* 0600f6ed //C 0f09f   225 st	r0,line_ptr */
0x0000f0a0, 0x0112f689, /* 0112f689 //C 0f0a0   226 mvzl	r1,line */
0x0000f0a1, 0x01220000, /* 01220000 //C 0f0a1   227 mvzl	r2,0 */
0x0000f0a2, 0x08210000, /* 08210000 //C 0f0a2   228 st	r2,r1,r0 */
0x0000f0a3, 0x0102f733, /* 0102f733 //C 0f0a3   229 mvzl	r0,msg_BS */
0x0000f0a4, 0x0400f53a, /* 0400f53a //C 0f0a4   230 call	prints */
0x0000f0a5, 0x020d0000, /* 020d0000 //C 0f0a5   232 clc */
0x0000f0a6, 0x01f2f0c1, /* 01f2f0c1 //C 0f0a6   233 jmp	proc_input_ret */
0x0000f0a7, 0x0108001f, /* 0108001f //C 0f0a7   235 cmp	r0,31		; refuse control chars */
0x0000f0a8, 0xa1f2f0c1, /* a1f2f0c1 //C 0f0a8   236 LS jmp	proc_input_ret */
0x0000f0a9, 0x0108007e, /* 0108007e //C 0f0a9   237 cmp	r0,126		; refuse graph chars */
0x0000f0aa, 0x91f2f0c1, /* 91f2f0c1 //C 0f0aa   238 HI jmp	proc_input_ret */
0x0000f0ab, 0x01220000, /* 01220000 //C 0f0ab   239 mvzl	r2,0		; at_aol= 0 */
0x0000f0ac, 0x0620f6ee, /* 0620f6ee //C 0f0ac   240 st	r2,at_eol */
0x0000f0ad, 0x0112f6ed, /* 0112f6ed //C 0f0ad   241 mvzl	r1,line_ptr	; line[line_ptr]= char */
0x0000f0ae, 0x0e310000, /* 0e310000 //C 0f0ae   242 ld	r3,r1 */
0x0000f0af, 0x0122f689, /* 0122f689 //C 0f0af   243 mvzl	r2,line */
0x0000f0b0, 0x08020300, /* 08020300 //C 0f0b0   244 st	r0,r2,r3 */
0x0000f0b1, 0x013a0001, /* 013a0001 //C 0f0b1   246 plus	r3,1		; line_ptr++ */
0x0000f0b2, 0x0c310000, /* 0c310000 //C 0f0b2   247 st	r3,r1 */
0x0000f0b3, 0x01420000, /* 01420000 //C 0f0b3   248 mvzl	r4,0 */
0x0000f0b4, 0x09438200, /* 09438200 //C 0f0b4   249 st	r4,r3+,r2	; line[line_ptr]= 0 */
0x0000f0b5, 0x08430200, /* 08430200 //C 0f0b5   250 st	r4,r3,r2	; double 0 at end, needed by tokenizer */
0x0000f0b6, 0x0142f6f4, /* 0142f6f4 //C 0f0b6   251 mvzl	r4,echo		; check if echo is turned on */
0x0000f0b7, 0x0e440000, /* 0e440000 //C 0f0b7   252 ld	r4,r4 */
0x0000f0b8, 0x024b0000, /* 024b0000 //C 0f0b8   253 sz	r4 */
0x0000f0b9, 0x2400f533, /* 2400f533 //C 0f0b9   254 NZ call	putchar		; echo */
0x0000f0ba, 0x020d0000, /* 020d0000 //C 0f0ba   255 clc */
0x0000f0bb, 0x01f2f0c1, /* 01f2f0c1 //C 0f0bb   256 jmp	proc_input_ret */
0x0000f0bc, 0x0112f6ee, /* 0112f6ee //C 0f0bc   258 mvzl	r1,at_eol */
0x0000f0bd, 0x0e110000, /* 0e110000 //C 0f0bd   259 ld	r1,r1 */
0x0000f0be, 0x021b0000, /* 021b0000 //C 0f0be   260 sz	r1		; Z=0 at eol -> skip, not ready */
0x0000f0bf, 0x220d0000, /* 220d0000 //C 0f0bf   261 Z0 clc */
0x0000f0c0, 0x120c0000, /* 120c0000 //C 0f0c0   262 Z1 sec */
0x0000f0c1, 0x0ffd0000, /* 0ffd0000 //C 0f0c1   264 pop	pc */
0x0000f0c2, 0x0ded0000, /* 0ded0000 //C 0f0c2   273 push	lr */
0x0000f0c3, 0x0102000a, /* 0102000a //C 0f0c3   275 mvzl	r0,LF */
0x0000f0c4, 0x0400f533, /* 0400f533 //C 0f0c4   276 call	putchar */
0x0000f0c5, 0x0700f689, /* 0700f689 //C 0f0c5   278 ld	r0,line */
0x0000f0c6, 0x020b0000, /* 020b0000 //C 0f0c6   279 sz	r0 */
0x0000f0c7, 0x11f2f0cf, /* 11f2f0cf //C 0f0c7   280 jz	proc_line_ret */
0x0000f0c8, 0x0400f0eb, /* 0400f0eb //C 0f0c8   288 call	tokenize */
0x0000f0c9, 0x0400f10b, /* 0400f10b //C 0f0c9   299 call	find_cmd */
0x0000f0ca, 0x41f2f0cd, /* 41f2f0cd //C 0f0ca   300 C0 jmp	cmd_not_found */
0x0000f0cb, 0x05000000, /* 05000000 //C 0f0cb   303 call	r0,0 */
0x0000f0cc, 0x01f2f0cf, /* 01f2f0cf //C 0f0cc   305 jmp	proc_line_ret */
0x0000f0cd, 0x0102f0d7, /* 0102f0d7 //C 0f0cd   307 mvzl	r0,snotfound */
0x0000f0ce, 0x0400f570, /* 0400f570 //C 0f0ce   308 call	printsnl */
0x0000f0cf, 0x01120001, /* 01120001 //C 0f0cf   311 mvzl	r1,1		; at_eol= 1 */
0x0000f0d0, 0x0610f6ee, /* 0610f6ee //C 0f0d0   312 st	r1,at_eol */
0x0000f0d1, 0x0ffd0000, /* 0ffd0000 //C 0f0d1   313 pop	pc */
0x0000f0d2, 0x00000047, /* 00000047 //C 0f0d2   315 db	71 */
0x0000f0d3, 0x0000006f, /* 0000006f //C 0f0d3   315 db	111 */
0x0000f0d4, 0x00000074, /* 00000074 //C 0f0d4   315 db	116 */
0x0000f0d5, 0x0000003a, /* 0000003a //C 0f0d5   315 db	58 */
0x0000f0d6, 0x00000000, /* 00000000 //C 0f0d6   315 db */
0x0000f0d7, 0x00000055, /* 00000055 //C 0f0d7   316 db	85 */
0x0000f0d8, 0x0000006e, /* 0000006e //C 0f0d8   316 db	110 */
0x0000f0d9, 0x0000006b, /* 0000006b //C 0f0d9   316 db	107 */
0x0000f0da, 0x0000006e, /* 0000006e //C 0f0da   316 db	110 */
0x0000f0db, 0x0000006f, /* 0000006f //C 0f0db   316 db	111 */
0x0000f0dc, 0x00000077, /* 00000077 //C 0f0dc   316 db	119 */
0x0000f0dd, 0x0000006e, /* 0000006e //C 0f0dd   316 db	110 */
0x0000f0de, 0x00000020, /* 00000020 //C 0f0de   316 db	32 */
0x0000f0df, 0x00000063, /* 00000063 //C 0f0df   316 db	99 */
0x0000f0e0, 0x0000006f, /* 0000006f //C 0f0e0   316 db	111 */
0x0000f0e1, 0x0000006d, /* 0000006d //C 0f0e1   316 db	109 */
0x0000f0e2, 0x0000006d, /* 0000006d //C 0f0e2   316 db	109 */
0x0000f0e3, 0x00000061, /* 00000061 //C 0f0e3   316 db	97 */
0x0000f0e4, 0x0000006e, /* 0000006e //C 0f0e4   316 db	110 */
0x0000f0e5, 0x00000064, /* 00000064 //C 0f0e5   316 db	100 */
0x0000f0e6, 0x00000000, /* 00000000 //C 0f0e6   316 db */
0x0000f0e7, 0x0ded0000, /* 0ded0000 //C 0f0e7   324 push	lr */
0x0000f0e8, 0x0112f739, /* 0112f739 //C 0f0e8   325 mvzl	r1,delimiters */
0x0000f0e9, 0x0400f441, /* 0400f441 //C 0f0e9   326 call	strchr */
0x0000f0ea, 0x0ffd0000, /* 0ffd0000 //C 0f0ea   327 pop	pc */
0x0000f0eb, 0x0ded0000, /* 0ded0000 //C 0f0eb   336 push	lr */
0x0000f0ec, 0x0142f6ef, /* 0142f6ef //C 0f0ec   337 mvzl	r4,words	; array of result */
0x0000f0ed, 0x0152f689, /* 0152f689 //C 0f0ed   338 mvzl	r5,line		; address of next char */
0x0000f0ee, 0x01620000, /* 01620000 //C 0f0ee   339 mvzl	r6,0		; nuof words found */
0x0000f0ef, 0x01720000, /* 01720000 //C 0f0ef   340 mvzl	r7,0		; bool in_word */
0x0000f0f0, 0x0e050000, /* 0e050000 //C 0f0f0   342 ld	r0,r5		; pick a char */
0x0000f0f1, 0x020b0000, /* 020b0000 //C 0f0f1   343 sz	r0		; check end */
0x0000f0f2, 0x11f2f0fc, /* 11f2f0fc //C 0f0f2   344 jz	tok_delimiter	; found end, pretend delim */
0x0000f0f3, 0x0400f0e7, /* 0400f0e7 //C 0f0f3   346 call	is_delimiter */
0x0000f0f4, 0x31f2f0fc, /* 31f2f0fc //C 0f0f4   347 C1 jmp	tok_delimiter */
0x0000f0f5, 0x027b0000, /* 027b0000 //C 0f0f5   349 sz	r7 */
0x0000f0f6, 0x21f2f101, /* 21f2f101 //C 0f0f6   350 jnz	tok_next	; still inside word */
0x0000f0f7, 0x01720001, /* 01720001 //C 0f0f7   352 mvzl	r7,1		; in_word=true */
0x0000f0f8, 0x09568400, /* 09568400 //C 0f0f8   353 st	r5,r6+,r4	; record word address */
0x0000f0f9, 0x01680005, /* 01680005 //C 0f0f9   354 cmp	r6,MAX_WORDS	; If no more space */
0x0000f0fa, 0x11f2f105, /* 11f2f105 //C 0f0fa   355 EQ jmp	tok_ret		; then return */
0x0000f0fb, 0x01f2f101, /* 01f2f101 //C 0f0fb   356 jmp	tok_next */
0x0000f0fc, 0x027b0000, /* 027b0000 //C 0f0fc   358 sz	r7 */
0x0000f0fd, 0x11f2f101, /* 11f2f101 //C 0f0fd   359 jz	tok_next	; still between words */
0x0000f0fe, 0x01720000, /* 01720000 //C 0f0fe   361 mvzl	r7,0		; in_word=false */
0x0000f0ff, 0x01120000, /* 01120000 //C 0f0ff   362 mvzl	r1,0		; put a 0 at the end of word */
0x0000f100, 0x08150100, /* 08150100 //C 0f100   363 st	r1,r5,r1 */
0x0000f101, 0x020b0000, /* 020b0000 //C 0f101   365 sz	r0		; check EOL */
0x0000f102, 0x11f2f105, /* 11f2f105 //C 0f102   366 jz	tok_ret		; jump out if char==0 */
0x0000f103, 0x01540001, /* 01540001 //C 0f103   367 add	r5,1 */
0x0000f104, 0x01f2f0f0, /* 01f2f0f0 //C 0f104   368 jmp	tok_cycle */
0x0000f105, 0x01120000, /* 01120000 //C 0f105   370 mvzl	r1,0 */
0x0000f106, 0x01680005, /* 01680005 //C 0f106   371 cmp	r6,MAX_WORDS */
0x0000f107, 0x11f2f10a, /* 11f2f10a //C 0f107   372 jz	tok_end */
0x0000f108, 0x09168400, /* 09168400 //C 0f108   373 st	r1,r6+,r4 */
0x0000f109, 0x01f2f105, /* 01f2f105 //C 0f109   374 jmp	tok_ret */
0x0000f10a, 0x0ffd0000, /* 0ffd0000 //C 0f10a   376 pop	pc */
0x0000f10b, 0x0ded0000, /* 0ded0000 //C 0f10b   385 push	lr */
0x0000f10c, 0x0d1d0000, /* 0d1d0000 //C 0f10c   386 push	r1 */
0x0000f10d, 0x0d2d0000, /* 0d2d0000 //C 0f10d   387 push	r2 */
0x0000f10e, 0x0d3d0000, /* 0d3d0000 //C 0f10e   388 push	r3 */
0x0000f10f, 0x0dad0000, /* 0dad0000 //C 0f10f   389 push	r10 */
0x0000f110, 0x0700f6ef, /* 0700f6ef //C 0f110   390 ld	r0,words	; R0= 1st word of command */
0x0000f111, 0x020b0000, /* 020b0000 //C 0f111   391 sz	r0 */
0x0000f112, 0x11f2f142, /* 11f2f142 //C 0f112   392 jz	find_cmd_false */
0x0000f113, 0x0e100000, /* 0e100000 //C 0f113   395 ld	r1,r0		; 1st char of word1 */
0x0000f114, 0x0e200001, /* 0e200001 //C 0f114   396 ld	r2,r0,1		; 2nd char */
0x0000f115, 0x0e300002, /* 0e300002 //C 0f115   397 ld	r3,r0,2		; 3rd char */
0x0000f116, 0x011fffdf, /* 011fffdf //C 0f116   398 and	r1,0xffdf	; upcase 1st char */
0x0000f117, 0x01180052, /* 01180052 //C 0f117   399 cmp	r1,'R' */
0x0000f118, 0x21f2f12e, /* 21f2f12e //C 0f118   400 jnz	find_not_rx */
0x0000f119, 0x0128002f, /* 0128002f //C 0f119   401 cmp	r2,'/'		; '0'-1 */
0x0000f11a, 0xa1f2f12e, /* a1f2f12e //C 0f11a   402 LS jmp	find_not_rx */
0x0000f11b, 0x01280039, /* 01280039 //C 0f11b   403 cmp	r2,'9' */
0x0000f11c, 0x91f2f12e, /* 91f2f12e //C 0f11c   404 HI jmp	find_not_rx */
0x0000f11d, 0x023b0000, /* 023b0000 //C 0f11d   405 sz	r3 */
0x0000f11e, 0x11f2f129, /* 11f2f129 //C 0f11e   406 jz	find_rx_09 */
0x0000f11f, 0x01280031, /* 01280031 //C 0f11f   408 cmp	r2,'1'		; first char must be '1' */
0x0000f120, 0x21f2f12e, /* 21f2f12e //C 0f120   409 jnz	find_not_rx */
0x0000f121, 0x0138002f, /* 0138002f //C 0f121   410 cmp	r3,'/'		; '0'-1 */
0x0000f122, 0xa1f2f12e, /* a1f2f12e //C 0f122   411 LS jmp	find_not_rx */
0x0000f123, 0x01380035, /* 01380035 //C 0f123   412 cmp	r3,'5' */
0x0000f124, 0x91f2f12e, /* 91f2f12e //C 0f124   413 HI jmp	find_not_rx */
0x0000f125, 0x01360030, /* 01360030 //C 0f125   414 sub	r3,'0' */
0x0000f126, 0x0134000a, /* 0134000a //C 0f126   415 add	r3,10 */
0x0000f127, 0x0630f6f6, /* 0630f6f6 //C 0f127   416 st	r3,nuof_reg */
0x0000f128, 0x01f2f12b, /* 01f2f12b //C 0f128   417 jmp	find_rx */
0x0000f129, 0x01260030, /* 01260030 //C 0f129   419 sub	r2,'0' */
0x0000f12a, 0x0620f6f6, /* 0620f6f6 //C 0f12a   420 st	r2,nuof_reg */
0x0000f12b, 0x0102f378, /* 0102f378 //C 0f12b   422 mvzl	r0,cmd_rx */
0x0000f12c, 0x020c0000, /* 020c0000 //C 0f12c   423 sec */
0x0000f12d, 0x01f2f14d, /* 01f2f14d //C 0f12d   424 jmp	find_cmd_ret */
0x0000f12e, 0x01a2f755, /* 01a2f755 //C 0f12e   427 mvzl	r10,commands */
0x0000f12f, 0x0e2a0000, /* 0e2a0000 //C 0f12f   429 ld	r2,r10		; R2= cmd addr */
0x0000f130, 0x022b0000, /* 022b0000 //C 0f130   430 sz	r2 */
0x0000f131, 0x11f2f142, /* 11f2f142 //C 0f131   431 jz	find_cmd_false */
0x0000f132, 0x01a40001, /* 01a40001 //C 0f132   432 add	r10,1 */
0x0000f133, 0x00100a00, /* 00100a00 //C 0f133   433 mov	r1,r10		; R1= cmd string */
0x0000f134, 0x01a40001, /* 01a40001 //C 0f134   436 add	r10,1 */
0x0000f135, 0x0e3a0000, /* 0e3a0000 //C 0f135   437 ld	r3,r10 */
0x0000f136, 0x023b0000, /* 023b0000 //C 0f136   438 sz	r3 */
0x0000f137, 0x21f2f134, /* 21f2f134 //C 0f137   439 jnz	find_cmd_fw */
0x0000f138, 0x01a40001, /* 01a40001 //C 0f138   440 add	r10,1 */
0x0000f139, 0x0400f4b3, /* 0400f4b3 //C 0f139   442 call	streq */
0x0000f13a, 0x41f2f12f, /* 41f2f12f //C 0f13a   443 C0 jmp	find_cmd_cyc */
0x0000f13b, 0x00000200, /* 00000200 //C 0f13b   445 mov	r0,r2 */
0x0000f13c, 0x020c0000, /* 020c0000 //C 0f13c   446 sec */
0x0000f13d, 0x01f2f14d, /* 01f2f14d //C 0f13d   447 jmp	find_cmd_ret */
0x0000f13e, 0x0000002f, /* 0000002f //C 0f13e   448 db	47 */
0x0000f13f, 0x0000002f, /* 0000002f //C 0f13f   448 db	47 */
0x0000f140, 0x00000043, /* 00000043 //C 0f140   448 db	67 */
0x0000f141, 0x00000000, /* 00000000 //C 0f141   448 db */
0x0000f142, 0x01040001, /* 01040001 //C 0f142   450 add	r0,1		; check second word */
0x0000f143, 0x0e100000, /* 0e100000 //C 0f143   451 ld	r1,r0		; for //C command */
0x0000f144, 0x021b0000, /* 021b0000 //C 0f144   452 sz	r1 */
0x0000f145, 0x11f2f14b, /* 11f2f14b //C 0f145   453 jz	find_cmd_very_false */
0x0000f146, 0x0102f13e, /* 0102f13e //C 0f146   454 mvzl	r0,c_cmd_name */
0x0000f147, 0x0400f4b3, /* 0400f4b3 //C 0f147   455 call	streq */
0x0000f148, 0x21f2f14b, /* 21f2f14b //C 0f148   456 jnz	find_cmd_very_false */
0x0000f149, 0x0122f200, /* 0122f200 //C 0f149   457 mvzl	r2,cmd_c */
0x0000f14a, 0x01f2f13b, /* 01f2f13b //C 0f14a   458 jmp	find_cmd_true */
0x0000f14b, 0x020d0000, /* 020d0000 //C 0f14b   460 clc */
0x0000f14c, 0x01020000, /* 01020000 //C 0f14c   461 mvzl	r0,0 */
0x0000f14d, 0x0fad0000, /* 0fad0000 //C 0f14d   463 pop	r10 */
0x0000f14e, 0x0f3d0000, /* 0f3d0000 //C 0f14e   464 pop	r3 */
0x0000f14f, 0x0f2d0000, /* 0f2d0000 //C 0f14f   465 pop	r2 */
0x0000f150, 0x0f1d0000, /* 0f1d0000 //C 0f150   466 pop	r1 */
0x0000f151, 0x0ffd0000, /* 0ffd0000 //C 0f151   467 pop	pc */
0x0000f152, 0x0ded0000, /* 0ded0000 //C 0f152   472 push	lr */
0x0000f153, 0x0122f6ef, /* 0122f6ef //C 0f153   473 mvzl	r2,words */
0x0000f154, 0x01020000, /* 01020000 //C 0f154   474 mvzl	r0,0 */
0x0000f155, 0x0e420001, /* 0e420001 //C 0f155   476 ld	r4,r2,1		; addr */
0x0000f156, 0x0e520002, /* 0e520002 //C 0f156   477 ld	r5,r2,2		; value */
0x0000f157, 0x024b0000, /* 024b0000 //C 0f157   478 sz 	r4 */
0x0000f158, 0x11f2f17e, /* 11f2f17e //C 0f158   479 jz	m_ret */
0x0000f159, 0x00000400, /* 00000400 //C 0f159   481 mov	r0,r4 */
0x0000f15a, 0x0400f4f6, /* 0400f4f6 //C 0f15a   482 call	htoi */
0x0000f15b, 0x00400100, /* 00400100 //C 0f15b   483 mov	r4,r1 */
0x0000f15c, 0x31f2f160, /* 31f2f160 //C 0f15c   484 C1 jmp	m_addr_ok */
0x0000f15d, 0x0102f17f, /* 0102f17f //C 0f15d   485 mvzl	r0,m_err_addr */
0x0000f15e, 0x0400f570, /* 0400f570 //C 0f15e   486 call	printsnl */
0x0000f15f, 0x01f2f17e, /* 01f2f17e //C 0f15f   487 jmp	m_ret */
0x0000f160, 0x025b0000, /* 025b0000 //C 0f160   489 sz	r5 */
0x0000f161, 0x11f2f174, /* 11f2f174 //C 0f161   490 jz	m_read */
0x0000f162, 0x0132f000, /* 0132f000 //C 0f162   492 mvzl	r3,the_begin */
0x0000f163, 0x00380400, /* 00380400 //C 0f163   493 cmp	r3,r4 */
0x0000f164, 0x91f2f16c, /* 91f2f16c //C 0f164   494 HI jmp	m_addrv_ok */
0x0000f165, 0x0132f9b1, /* 0132f9b1 //C 0f165   495 mvzl	r3,the_end */
0x0000f166, 0x00380400, /* 00380400 //C 0f166   496 cmp	r3,r4 */
0x0000f167, 0x91f2f169, /* 91f2f169 //C 0f167   497 HI jmp	m_addrv_nok */
0x0000f168, 0x01f2f16c, /* 01f2f16c //C 0f168   498 jmp	m_addrv_ok */
0x0000f169, 0x0102f18d, /* 0102f18d //C 0f169   506 mvzl	r0,m_err_addrv */
0x0000f16a, 0x0400f570, /* 0400f570 //C 0f16a   507 call	printsnl */
0x0000f16b, 0x01f2f17e, /* 01f2f17e //C 0f16b   508 jmp	m_ret */
0x0000f16c, 0x00000500, /* 00000500 //C 0f16c   510 mov	r0,r5 */
0x0000f16d, 0x0400f4f6, /* 0400f4f6 //C 0f16d   511 call	htoi */
0x0000f16e, 0x00500100, /* 00500100 //C 0f16e   512 mov	r5,r1 */
0x0000f16f, 0x31f2f173, /* 31f2f173 //C 0f16f   513 C1 jmp	m_value_ok */
0x0000f170, 0x0102f19f, /* 0102f19f //C 0f170   514 mvzl	r0,m_err_value */
0x0000f171, 0x0400f570, /* 0400f570 //C 0f171   515 call	printsnl */
0x0000f172, 0x01f2f17e, /* 01f2f17e //C 0f172   516 jmp	m_ret */
0x0000f173, 0x0c540000, /* 0c540000 //C 0f173   518 st	r5,r4 */
0x0000f174, 0x00000400, /* 00000400 //C 0f174   521 mov	r0,r4 */
0x0000f175, 0x01120004, /* 01120004 //C 0f175   522 mvzl	r1,4 */
0x0000f176, 0x0400f577, /* 0400f577 //C 0f176   523 call	print_vhex */
0x0000f177, 0x01020020, /* 01020020 //C 0f177   524 mvzl	r0,0x20 */
0x0000f178, 0x0400f533, /* 0400f533 //C 0f178   525 call	putchar */
0x0000f179, 0x0e040000, /* 0e040000 //C 0f179   526 ld	r0,r4 */
0x0000f17a, 0x01120004, /* 01120004 //C 0f17a   527 mvzl	r1,4 */
0x0000f17b, 0x0400f577, /* 0400f577 //C 0f17b   528 call	print_vhex */
0x0000f17c, 0x0102000a, /* 0102000a //C 0f17c   529 mvzl	r0,LF */
0x0000f17d, 0x0400f533, /* 0400f533 //C 0f17d   530 call	putchar */
0x0000f17e, 0x0ffd0000, /* 0ffd0000 //C 0f17e   532 pop	pc */
0x0000f17f, 0x00000041, /* 00000041 //C 0f17f   534 db	65 */
0x0000f180, 0x00000064, /* 00000064 //C 0f180   534 db	100 */
0x0000f181, 0x00000064, /* 00000064 //C 0f181   534 db	100 */
0x0000f182, 0x00000072, /* 00000072 //C 0f182   534 db	114 */
0x0000f183, 0x00000065, /* 00000065 //C 0f183   534 db	101 */
0x0000f184, 0x00000073, /* 00000073 //C 0f184   534 db	115 */
0x0000f185, 0x00000073, /* 00000073 //C 0f185   534 db	115 */
0x0000f186, 0x00000020, /* 00000020 //C 0f186   534 db	32 */
0x0000f187, 0x00000065, /* 00000065 //C 0f187   534 db	101 */
0x0000f188, 0x00000072, /* 00000072 //C 0f188   534 db	114 */
0x0000f189, 0x00000072, /* 00000072 //C 0f189   534 db	114 */
0x0000f18a, 0x0000006f, /* 0000006f //C 0f18a   534 db	111 */
0x0000f18b, 0x00000072, /* 00000072 //C 0f18b   534 db	114 */
0x0000f18c, 0x00000000, /* 00000000 //C 0f18c   534 db */
0x0000f18d, 0x0000004d, /* 0000004d //C 0f18d   535 db	77 */
0x0000f18e, 0x0000006f, /* 0000006f //C 0f18e   535 db	111 */
0x0000f18f, 0x0000006e, /* 0000006e //C 0f18f   535 db	110 */
0x0000f190, 0x00000069, /* 00000069 //C 0f190   535 db	105 */
0x0000f191, 0x00000074, /* 00000074 //C 0f191   535 db	116 */
0x0000f192, 0x0000006f, /* 0000006f //C 0f192   535 db	111 */
0x0000f193, 0x00000072, /* 00000072 //C 0f193   535 db	114 */
0x0000f194, 0x00000027, /* 00000027 //C 0f194   535 db	39 */
0x0000f195, 0x00000073, /* 00000073 //C 0f195   535 db	115 */
0x0000f196, 0x00000020, /* 00000020 //C 0f196   535 db	32 */
0x0000f197, 0x00000061, /* 00000061 //C 0f197   535 db	97 */
0x0000f198, 0x00000064, /* 00000064 //C 0f198   535 db	100 */
0x0000f199, 0x00000064, /* 00000064 //C 0f199   535 db	100 */
0x0000f19a, 0x00000072, /* 00000072 //C 0f19a   535 db	114 */
0x0000f19b, 0x00000065, /* 00000065 //C 0f19b   535 db	101 */
0x0000f19c, 0x00000073, /* 00000073 //C 0f19c   535 db	115 */
0x0000f19d, 0x00000073, /* 00000073 //C 0f19d   535 db	115 */
0x0000f19e, 0x00000000, /* 00000000 //C 0f19e   535 db */
0x0000f19f, 0x00000056, /* 00000056 //C 0f19f   536 db	86 */
0x0000f1a0, 0x00000061, /* 00000061 //C 0f1a0   536 db	97 */
0x0000f1a1, 0x0000006c, /* 0000006c //C 0f1a1   536 db	108 */
0x0000f1a2, 0x00000075, /* 00000075 //C 0f1a2   536 db	117 */
0x0000f1a3, 0x00000065, /* 00000065 //C 0f1a3   536 db	101 */
0x0000f1a4, 0x00000020, /* 00000020 //C 0f1a4   536 db	32 */
0x0000f1a5, 0x00000065, /* 00000065 //C 0f1a5   536 db	101 */
0x0000f1a6, 0x00000072, /* 00000072 //C 0f1a6   536 db	114 */
0x0000f1a7, 0x00000072, /* 00000072 //C 0f1a7   536 db	114 */
0x0000f1a8, 0x0000006f, /* 0000006f //C 0f1a8   536 db	111 */
0x0000f1a9, 0x00000072, /* 00000072 //C 0f1a9   536 db	114 */
0x0000f1aa, 0x00000000, /* 00000000 //C 0f1aa   536 db */
0x0000f1ab, 0x0ded0000, /* 0ded0000 //C 0f1ab   541 push	lr */
0x0000f1ac, 0x0122f6ef, /* 0122f6ef //C 0f1ac   542 mvzl	r2,words */
0x0000f1ad, 0x01020000, /* 01020000 //C 0f1ad   543 mvzl	r0,0 */
0x0000f1ae, 0x0e420001, /* 0e420001 //C 0f1ae   545 ld	r4,r2,1		; addr */
0x0000f1af, 0x0e520002, /* 0e520002 //C 0f1af   546 ld	r5,r2,2		; value */
0x0000f1b0, 0x024b0000, /* 024b0000 //C 0f1b0   547 sz 	r4 */
0x0000f1b1, 0x11f2f17e, /* 11f2f17e //C 0f1b1   548 jz	m_ret */
0x0000f1b2, 0x00000400, /* 00000400 //C 0f1b2   550 mov	r0,r4 */
0x0000f1b3, 0x0400f4f6, /* 0400f4f6 //C 0f1b3   551 call	htoi */
0x0000f1b4, 0x00400100, /* 00400100 //C 0f1b4   552 mov	r4,r1 */
0x0000f1b5, 0x31f2f1b9, /* 31f2f1b9 //C 0f1b5   553 C1 jmp	sm_addr_ok */
0x0000f1b6, 0x0102f17f, /* 0102f17f //C 0f1b6   554 mvzl	r0,m_err_addr */
0x0000f1b7, 0x0400f570, /* 0400f570 //C 0f1b7   555 call	printsnl */
0x0000f1b8, 0x01f2f1cd, /* 01f2f1cd //C 0f1b8   556 jmp	sm_ret */
0x0000f1b9, 0x025b0000, /* 025b0000 //C 0f1b9   558 sz	r5 */
0x0000f1ba, 0x11f2f1cd, /* 11f2f1cd //C 0f1ba   559 jz	sm_ret */
0x0000f1bb, 0x0132f000, /* 0132f000 //C 0f1bb   561 mvzl	r3,the_begin */
0x0000f1bc, 0x00380400, /* 00380400 //C 0f1bc   562 cmp	r3,r4 */
0x0000f1bd, 0x91f2f1c5, /* 91f2f1c5 //C 0f1bd   563 HI jmp	sm_addrv_ok */
0x0000f1be, 0x0132f9b1, /* 0132f9b1 //C 0f1be   564 mvzl	r3,the_end */
0x0000f1bf, 0x00380400, /* 00380400 //C 0f1bf   565 cmp	r3,r4 */
0x0000f1c0, 0x91f2f1c2, /* 91f2f1c2 //C 0f1c0   566 HI jmp	sm_addrv_nok */
0x0000f1c1, 0x01f2f1c5, /* 01f2f1c5 //C 0f1c1   567 jmp	sm_addrv_ok */
0x0000f1c2, 0x0102f18d, /* 0102f18d //C 0f1c2   569 mvzl	r0,m_err_addrv */
0x0000f1c3, 0x0400f570, /* 0400f570 //C 0f1c3   570 call	printsnl */
0x0000f1c4, 0x01f2f1cd, /* 01f2f1cd //C 0f1c4   571 jmp	sm_ret */
0x0000f1c5, 0x00000500, /* 00000500 //C 0f1c5   573 mov	r0,r5 */
0x0000f1c6, 0x0400f4f6, /* 0400f4f6 //C 0f1c6   574 call	htoi */
0x0000f1c7, 0x00500100, /* 00500100 //C 0f1c7   575 mov	r5,r1 */
0x0000f1c8, 0x31f2f1cc, /* 31f2f1cc //C 0f1c8   576 C1 jmp	sm_value_ok */
0x0000f1c9, 0x0102f19f, /* 0102f19f //C 0f1c9   577 mvzl	r0,m_err_value */
0x0000f1ca, 0x0400f570, /* 0400f570 //C 0f1ca   578 call	printsnl */
0x0000f1cb, 0x01f2f1cd, /* 01f2f1cd //C 0f1cb   579 jmp	sm_ret */
0x0000f1cc, 0x0c540000, /* 0c540000 //C 0f1cc   581 st	r5,r4 */
0x0000f1cd, 0x0ffd0000, /* 0ffd0000 //C 0f1cd   584 pop	pc */
0x0000f1ce, 0x0ded0000, /* 0ded0000 //C 0f1ce   590 push	lr */
0x0000f1cf, 0x0122f6ef, /* 0122f6ef //C 0f1cf   591 mvzl	r2,words */
0x0000f1d0, 0x0e020001, /* 0e020001 //C 0f1d0   592 ld	r0,r2,1		; start address */
0x0000f1d1, 0x0400f4f6, /* 0400f4f6 //C 0f1d1   593 call	htoi */
0x0000f1d2, 0x00300100, /* 00300100 //C 0f1d2   594 mov	r3,r1 */
0x0000f1d3, 0x0e020002, /* 0e020002 //C 0f1d3   595 ld	r0,r2,2		; end address */
0x0000f1d4, 0x020b0000, /* 020b0000 //C 0f1d4   596 sz	r0 */
0x0000f1d5, 0x21f2f1d9, /* 21f2f1d9 //C 0f1d5   597 jnz	d_end_ok */
0x0000f1d6, 0x00400300, /* 00400300 //C 0f1d6   598 mov	r4,r3 */
0x0000f1d7, 0x01440010, /* 01440010 //C 0f1d7   599 add	r4,0x10 */
0x0000f1d8, 0x01f2f1db, /* 01f2f1db //C 0f1d8   600 jmp	d_chk_end */
0x0000f1d9, 0x0400f4f6, /* 0400f4f6 //C 0f1d9   602 call	htoi */
0x0000f1da, 0x00400100, /* 00400100 //C 0f1da   603 mov	r4,r1 */
0x0000f1db, 0x00380400, /* 00380400 //C 0f1db   605 cmp	r3,r4		; check if start>end */
0x0000f1dc, 0x91f2f1eb, /* 91f2f1eb //C 0f1dc   606 HI jmp d_bad */
0x0000f1dd, 0x00000300, /* 00000300 //C 0f1dd   614 mov	r0,r3		; print address */
0x0000f1de, 0x01120004, /* 01120004 //C 0f1de   615 mvzl	r1,4 */
0x0000f1df, 0x0400f577, /* 0400f577 //C 0f1df   616 call	print_vhex */
0x0000f1e0, 0x01020020, /* 01020020 //C 0f1e0   617 mvzl	r0,0x20		; print one space */
0x0000f1e1, 0x0400f533, /* 0400f533 //C 0f1e1   618 call	putchar */
0x0000f1e2, 0x0e030000, /* 0e030000 //C 0f1e2   619 ld	r0,r3		; load data */
0x0000f1e3, 0x01120004, /* 01120004 //C 0f1e3   620 mvzl	r1,4		; print data */
0x0000f1e4, 0x0400f577, /* 0400f577 //C 0f1e4   621 call	print_vhex */
0x0000f1e5, 0x0102000a, /* 0102000a //C 0f1e5   622 mvzl	r0,LF		; print new line */
0x0000f1e6, 0x0400f533, /* 0400f533 //C 0f1e6   623 call	putchar */
0x0000f1e7, 0x00380400, /* 00380400 //C 0f1e7   624 cmp	r3,r4 */
0x0000f1e8, 0x11f2f1ed, /* 11f2f1ed //C 0f1e8   625 jz	d_ret */
0x0000f1e9, 0x01340001, /* 01340001 //C 0f1e9   626 add	r3,1 */
0x0000f1ea, 0x01f2f1dd, /* 01f2f1dd //C 0f1ea   627 jmp	d_cyc */
0x0000f1eb, 0x0102f1ee, /* 0102f1ee //C 0f1eb   629 mvzl	r0,d_err_bad */
0x0000f1ec, 0x0400f570, /* 0400f570 //C 0f1ec   630 call	printsnl */
0x0000f1ed, 0x0ffd0000, /* 0ffd0000 //C 0f1ed   632 pop	pc */
0x0000f1ee, 0x00000057, /* 00000057 //C 0f1ee   634 db	87 */
0x0000f1ef, 0x00000072, /* 00000072 //C 0f1ef   634 db	114 */
0x0000f1f0, 0x0000006f, /* 0000006f //C 0f1f0   634 db	111 */
0x0000f1f1, 0x0000006e, /* 0000006e //C 0f1f1   634 db	110 */
0x0000f1f2, 0x00000067, /* 00000067 //C 0f1f2   634 db	103 */
0x0000f1f3, 0x00000020, /* 00000020 //C 0f1f3   634 db	32 */
0x0000f1f4, 0x00000065, /* 00000065 //C 0f1f4   634 db	101 */
0x0000f1f5, 0x0000006e, /* 0000006e //C 0f1f5   634 db	110 */
0x0000f1f6, 0x00000064, /* 00000064 //C 0f1f6   634 db	100 */
0x0000f1f7, 0x00000020, /* 00000020 //C 0f1f7   634 db	32 */
0x0000f1f8, 0x00000061, /* 00000061 //C 0f1f8   634 db	97 */
0x0000f1f9, 0x00000064, /* 00000064 //C 0f1f9   634 db	100 */
0x0000f1fa, 0x00000064, /* 00000064 //C 0f1fa   634 db	100 */
0x0000f1fb, 0x00000072, /* 00000072 //C 0f1fb   634 db	114 */
0x0000f1fc, 0x00000065, /* 00000065 //C 0f1fc   634 db	101 */
0x0000f1fd, 0x00000073, /* 00000073 //C 0f1fd   634 db	115 */
0x0000f1fe, 0x00000073, /* 00000073 //C 0f1fe   634 db	115 */
0x0000f1ff, 0x00000000, /* 00000000 //C 0f1ff   634 db */
0x0000f200, 0x00f00e00, /* 00f00e00 //C 0f200   639 ret */
0x0000f201, 0x0ded0000, /* 0ded0000 //C 0f201   644 push	lr */
0x0000f202, 0x01a20000, /* 01a20000 //C 0f202   645 mvzl	r10,0		; state (nr of words) */
0x0000f203, 0x01820000, /* 01820000 //C 0f203   646 mvzl	r8,0		; value */
0x0000f204, 0x0162003f, /* 0162003f //C 0f204   647 mvzl	r6,'?'		; Record type */
0x0000f205, 0x01c20000, /* 01c20000 //C 0f205   648 mvzl	r12,0		; Checksum */
0x0000f206, 0x0400f524, /* 0400f524 //C 0f206   650 call	check_uart */
0x0000f207, 0x41f2f206, /* 41f2f206 //C 0f207   651 C0 jmp	l_cyc */
0x0000f208, 0x0400f52c, /* 0400f52c //C 0f208   652 call	read */
0x0000f209, 0x00b00000, /* 00b00000 //C 0f209   654 mov	r11,r0		; Copy of char in R11 */
0x0000f20a, 0x0108000a, /* 0108000a //C 0f20a   655 cmp	r0,10		; check EOL chars */
0x0000f20b, 0x11f2f257, /* 11f2f257 //C 0f20b   656 jz	l_eol */
0x0000f20c, 0x0108000d, /* 0108000d //C 0f20c   657 cmp	r0,13 */
0x0000f20d, 0x11f2f257, /* 11f2f257 //C 0f20d   658 jz	l_eol */
0x0000f20e, 0x01a80000, /* 01a80000 //C 0f20e   660 cmp	r10,0 */
0x0000f20f, 0x21f2f21f, /* 21f2f21f //C 0f20f   661 jnz	l_no0 */
0x0000f210, 0x0400f4c3, /* 0400f4c3 //C 0f210   663 call	hexchar2value */
0x0000f211, 0x41f2f219, /* 41f2f219 //C 0f211   664 C0 jmp	l_eof_0 */
0x0000f212, 0x02880000, /* 02880000 //C 0f212   665 shl	r8		; shift val(char) into value */
0x0000f213, 0x02880000, /* 02880000 //C 0f213   666 shl	r8 */
0x0000f214, 0x02880000, /* 02880000 //C 0f214   667 shl	r8 */
0x0000f215, 0x02880000, /* 02880000 //C 0f215   668 shl	r8 */
0x0000f216, 0x010b000f, /* 010b000f //C 0f216   669 btst	r0,0xf */
0x0000f217, 0x008d0000, /* 008d0000 //C 0f217   670 or	r8,r0 */
0x0000f218, 0x01f2f206, /* 01f2f206 //C 0f218   671 jmp	l_cyc */
0x0000f219, 0x01a20001, /* 01a20001 //C 0f219   673 mvzl	r10,1		; state0 -> state1 */
0x0000f21a, 0x0162003f, /* 0162003f //C 0f21a   674 mvzl	r6,'?'		; No //C yet */
0x0000f21b, 0x01b8002f, /* 01b8002f //C 0f21b   675 cmp	r11,'/'		; is it start of // */
0x0000f21c, 0x11720001, /* 11720001 //C 0f21c   676 z1 mvzl	r7,1		; Yes, first / found */
0x0000f21d, 0x21720000, /* 21720000 //C 0f21d   677 z0 mvzl	r7,0		; No '/' yet */
0x0000f21e, 0x01f2f206, /* 01f2f206 //C 0f21e   678 jmp	l_cyc */
0x0000f21f, 0x01a80001, /* 01a80001 //C 0f21f   681 cmp	r10,1 */
0x0000f220, 0x21f2f23a, /* 21f2f23a //C 0f220   682 jnz	l_no1 */
0x0000f221, 0x027b0000, /* 027b0000 //C 0f221   684 sz	r7 */
0x0000f222, 0x21f2f227, /* 21f2f227 //C 0f222   685 jnz	l_s1_2nd */
0x0000f223, 0x0108002f, /* 0108002f //C 0f223   687 cmp	r0,'/' */
0x0000f224, 0x21f2f206, /* 21f2f206 //C 0f224   688 jnz	l_cyc */
0x0000f225, 0x01720001, /* 01720001 //C 0f225   689 mvzl	r7,1 */
0x0000f226, 0x01f2f206, /* 01f2f206 //C 0f226   690 jmp	l_cyc */
0x0000f227, 0x0108002f, /* 0108002f //C 0f227   692 cmp	r0,'/' */
0x0000f228, 0x11f2f206, /* 11f2f206 //C 0f228   693 jz	l_cyc */
0x0000f229, 0x01080043, /* 01080043 //C 0f229   694 cmp	r0,'C' */
0x0000f22a, 0x11f2f22d, /* 11f2f22d //C 0f22a   695 jz	l_s1_C */
0x0000f22b, 0x01080049, /* 01080049 //C 0f22b   696 cmp	r0,'I' */
0x0000f22c, 0x21f2f232, /* 21f2f232 //C 0f22c   697 jnz	l_s1_noC */
0x0000f22d, 0x00600000, /* 00600000 //C 0f22d   700 mov	r6,r0		; record type is in r0, store in r6 */
0x0000f22e, 0x01a20002, /* 01a20002 //C 0f22e   702 mvzl	r10,2 */
0x0000f22f, 0x01920000, /* 01920000 //C 0f22f   703 mvzl	r9,0		; address= 0 */
0x0000f230, 0x01520000, /* 01520000 //C 0f230   704 mvzl	r5,0		; where we are in word: before */
0x0000f231, 0x01f2f206, /* 01f2f206 //C 0f231   705 jmp	l_cyc */
0x0000f232, 0x01080045, /* 01080045 //C 0f232   707 cmp	r0,'E' */
0x0000f233, 0x21f2f237, /* 21f2f237 //C 0f233   708 jnz	l_s1_noE */
0x0000f234, 0x01620045, /* 01620045 //C 0f234   710 mvzl	r6,'E' */
0x0000f235, 0x01a20003, /* 01a20003 //C 0f235   712 mvzl	r10,3 */
0x0000f236, 0x01f2f206, /* 01f2f206 //C 0f236   713 jmp	l_cyc */
0x0000f237, 0x0400f533, /* 0400f533 //C 0f237   717 call	putchar		; print record type */
0x0000f238, 0x01a2000f, /* 01a2000f //C 0f238   718 mvzl	r10,0xf		; special state: skip everything */
0x0000f239, 0x01f2f206, /* 01f2f206 //C 0f239   719 jmp	l_cyc */
0x0000f23a, 0x01a80002, /* 01a80002 //C 0f23a   722 cmp	r10,2 */
0x0000f23b, 0x21f2f24f, /* 21f2f24f //C 0f23b   723 jnz	l_no2 */
0x0000f23c, 0x01580000, /* 01580000 //C 0f23c   725 cmp	r5,0 */
0x0000f23d, 0x21f2f248, /* 21f2f248 //C 0f23d   726 jnz	l_s2_no0 */
0x0000f23e, 0x0400f4c3, /* 0400f4c3 //C 0f23e   728 call	hexchar2value */
0x0000f23f, 0x41f2f206, /* 41f2f206 //C 0f23f   729 C0 jmp	l_cyc */
0x0000f240, 0x01520001, /* 01520001 //C 0f240   730 mvzl	r5,1 */
0x0000f241, 0x02980000, /* 02980000 //C 0f241   732 shl	r9 */
0x0000f242, 0x02980000, /* 02980000 //C 0f242   733 shl	r9 */
0x0000f243, 0x02980000, /* 02980000 //C 0f243   734 shl	r9 */
0x0000f244, 0x02980000, /* 02980000 //C 0f244   735 shl	r9 */
0x0000f245, 0x010b000f, /* 010b000f //C 0f245   736 btst	r0,0xf */
0x0000f246, 0x009d0000, /* 009d0000 //C 0f246   737 or	r9,r0 */
0x0000f247, 0x01f2f206, /* 01f2f206 //C 0f247   739 jmp	l_cyc */
0x0000f248, 0x01580001, /* 01580001 //C 0f248   741 cmp	r5,1 */
0x0000f249, 0x21f2f24e, /* 21f2f24e //C 0f249   742 jnz	l_s2_no1 */
0x0000f24a, 0x0400f4c3, /* 0400f4c3 //C 0f24a   744 call	hexchar2value */
0x0000f24b, 0x31f2f241, /* 31f2f241 //C 0f24b   745 C1 jmp	l_s2_got */
0x0000f24c, 0x01520002, /* 01520002 //C 0f24c   746 mvzl	r5,2 */
0x0000f24d, 0x01f2f206, /* 01f2f206 //C 0f24d   747 jmp	l_cyc */
0x0000f24e, 0x01f2f206, /* 01f2f206 //C 0f24e   749 jmp	l_cyc */
0x0000f24f, 0x01a80003, /* 01a80003 //C 0f24f   751 cmp	r10,3 */
0x0000f250, 0x21f2f252, /* 21f2f252 //C 0f250   752 jnz	l_no3 */
0x0000f251, 0x01f2f206, /* 01f2f206 //C 0f251   754 jmp	l_cyc		; do nothing, just wait EOL */
0x0000f252, 0x01a8000f, /* 01a8000f //C 0f252   757 cmp	r10,0xf */
0x0000f253, 0x01f2f255, /* 01f2f255 //C 0f253   758 jmp	l_nof */
0x0000f254, 0x01f2f206, /* 01f2f206 //C 0f254   759 jmp	l_cyc		; just skip */
0x0000f255, 0x01f2f206, /* 01f2f206 //C 0f255   762 jmp	l_cyc */
0x0000f256, 0x01f2f26f, /* 01f2f26f //C 0f256   763 jmp	l_ret */
0x0000f257, 0x01a80000, /* 01a80000 //C 0f257   767 cmp	r10,0		; in state0 */
0x0000f258, 0x11f2f26b, /* 11f2f26b //C 0f258   768 jz	l_back_to_0	; just restart */
0x0000f259, 0x01a80001, /* 01a80001 //C 0f259   769 cmp	r10,1		; in state1 */
0x0000f25a, 0x11f2f26b, /* 11f2f26b //C 0f25a   770 jz	l_back_to_0 	;l_cyc ;l_bad ; garbage */
0x0000f25b, 0x01a80002, /* 01a80002 //C 0f25b   771 cmp	r10,2		; in state2 */
0x0000f25c, 0x11f2f263, /* 11f2f263 //C 0f25c   772 jz	l_proc		; process record */
0x0000f25d, 0x01a80003, /* 01a80003 //C 0f25d   773 cmp	r10,3		; in state3 */
0x0000f25e, 0x11f2f26f, /* 11f2f26f //C 0f25e   774 jz	l_ret		; eol in end record: finish */
0x0000f25f, 0x01a8000f, /* 01a8000f //C 0f25f   775 cmp	r10,0xf		; in state skip */
0x0000f260, 0x11f2f26b, /* 11f2f26b //C 0f260   776 jz	l_back_to_0	; reset state for new line */
0x0000f261, 0x01f2f206, /* 01f2f206 //C 0f261   777 jmp	l_cyc */
0x0000f262, 0x01f2f26f, /* 01f2f26f //C 0f262   779 jmp	l_ret */
0x0000f263, 0x01680043, /* 01680043 //C 0f263   781 cmp	r6,'C'		; is it a C or I record? */
0x0000f264, 0x1c890000, /* 1c890000 //C 0f264   782 z st	r8,r9		; then store */
0x0000f265, 0x10c40800, /* 10c40800 //C 0f265   783 z add	r12,r8		; and add to checksum */
0x0000f266, 0x01680049, /* 01680049 //C 0f266   784 cmp	r6,'I' */
0x0000f267, 0x1c890000, /* 1c890000 //C 0f267   785 z st	r8,r9 */
0x0000f268, 0x10c40800, /* 10c40800 //C 0f268   786 z add	r12,r8 */
0x0000f269, 0x00000600, /* 00000600 //C 0f269   787 mov	r0,r6		; echo record type */
0x0000f26a, 0x0400f533, /* 0400f533 //C 0f26a   788 call	putchar */
0x0000f26b, 0x01a20000, /* 01a20000 //C 0f26b   793 mvzl	r10,0 */
0x0000f26c, 0x01820000, /* 01820000 //C 0f26c   794 mvzl	r8,0 */
0x0000f26d, 0x0162003f, /* 0162003f //C 0f26d   795 mvzl	r6,'?' */
0x0000f26e, 0x01f2f206, /* 01f2f206 //C 0f26e   796 jmp	l_cyc */
0x0000f26f, 0x0102000a, /* 0102000a //C 0f26f   798 mvzl	r0,LF */
0x0000f270, 0x0400f533, /* 0400f533 //C 0f270   799 call	putchar */
0x0000f271, 0x00000c00, /* 00000c00 //C 0f271   801 mov	r0,r12 */
0x0000f272, 0x01120004, /* 01120004 //C 0f272   802 mvzl	r1,4 */
0x0000f273, 0x0400f577, /* 0400f577 //C 0f273   803 call	print_vhex */
0x0000f274, 0x0102000a, /* 0102000a //C 0f274   804 mvzl	r0,LF */
0x0000f275, 0x0400f533, /* 0400f533 //C 0f275   805 call	putchar */
0x0000f276, 0x0ffd0000, /* 0ffd0000 //C 0f276   806 pop	pc */
0x0000f277, 0x0ded0000, /* 0ded0000 //C 0f277   812 push	lr */
0x0000f278, 0x0122f6ef, /* 0122f6ef //C 0f278   813 mvzl	r2,words */
0x0000f279, 0x0e020001, /* 0e020001 //C 0f279   814 ld	r0,r2,1		; address */
0x0000f27a, 0x020b0000, /* 020b0000 //C 0f27a   815 sz	r0 */
0x0000f27b, 0x11f2f29e, /* 11f2f29e //C 0f27b   816 jz	g_no_addr */
0x0000f27c, 0x0400f4f6, /* 0400f4f6 //C 0f27c   817 call	htoi */
0x0000f27d, 0x00b00100, /* 00b00100 //C 0f27d   818 mov	r11,r1 */
0x0000f27e, 0x0102f2b1, /* 0102f2b1 //C 0f27e   820 mvzl	r0,d_msg_run */
0x0000f27f, 0x0400f53a, /* 0400f53a //C 0f27f   821 call	prints */
0x0000f280, 0x00000b00, /* 00000b00 //C 0f280   822 mov	r0,r11 */
0x0000f281, 0x01120004, /* 01120004 //C 0f281   823 mvzl	r1,4 */
0x0000f282, 0x0400f577, /* 0400f577 //C 0f282   824 call	print_vhex */
0x0000f283, 0x0102000a, /* 0102000a //C 0f283   825 mvzl	r0,LF */
0x0000f284, 0x0400f533, /* 0400f533 //C 0f284   826 call	putchar */
0x0000f285, 0x06b0f706, /* 06b0f706 //C 0f285   827 st	r11,reg15 */
0x0000f286, 0x0122ff43, /* 0122ff43 //C 0f286   829 mvzl	r2,UART_TSTAT */
0x0000f287, 0x0e920000, /* 0e920000 //C 0f287   831 ld	r9,r2 */
0x0000f288, 0x019c0001, /* 019c0001 //C 0f288   832 test	r9,1 */
0x0000f289, 0x11f2f287, /* 11f2f287 //C 0f289   833 jz	g_wait_tc */
0x0000f28a, 0x07000000, /* 07000000 //C 0f28a   835 ld	r0,0 */
0x0000f28b, 0x0600f6f5, /* 0600f6f5 //C 0f28b   836 st	r0,called */
0x0000f28c, 0x0700f707, /* 0700f707 //C 0f28c   838 ld	r0,regf */
0x0000f28d, 0x020f0000, /* 020f0000 //C 0f28d   839 setf	r0 */
0x0000f28e, 0x0700f6f7, /* 0700f6f7 //C 0f28e   840 ld	r0,reg0 */
0x0000f28f, 0x0710f6f8, /* 0710f6f8 //C 0f28f   841 ld	r1,reg1 */
0x0000f290, 0x0720f6f9, /* 0720f6f9 //C 0f290   842 ld	r2,reg2 */
0x0000f291, 0x0730f6fa, /* 0730f6fa //C 0f291   843 ld	r3,reg3 */
0x0000f292, 0x0740f6fb, /* 0740f6fb //C 0f292   844 ld	r4,reg4 */
0x0000f293, 0x0750f6fc, /* 0750f6fc //C 0f293   845 ld	r5,reg5 */
0x0000f294, 0x0760f6fd, /* 0760f6fd //C 0f294   846 ld	r6,reg6 */
0x0000f295, 0x0770f6fe, /* 0770f6fe //C 0f295   847 ld	r7,reg7 */
0x0000f296, 0x0780f6ff, /* 0780f6ff //C 0f296   848 ld	r8,reg8 */
0x0000f297, 0x0790f700, /* 0790f700 //C 0f297   849 ld	r9,reg9 */
0x0000f298, 0x07a0f701, /* 07a0f701 //C 0f298   850 ld	r10,reg10 */
0x0000f299, 0x07b0f702, /* 07b0f702 //C 0f299   851 ld	r11,reg11 */
0x0000f29a, 0x07c0f703, /* 07c0f703 //C 0f29a   852 ld	r12,reg12 */
0x0000f29b, 0x07d0f704, /* 07d0f704 //C 0f29b   853 ld	r13,reg13 */
0x0000f29c, 0x07e0f705, /* 07e0f705 //C 0f29c   854 ld	r14,reg14 */
0x0000f29d, 0x07f0f706, /* 07f0f706 //C 0f29d   856 ld	r15,reg15 */
0x0000f29e, 0x0700f6f5, /* 0700f6f5 //C 0f29e   859 ld	r0,called */
0x0000f29f, 0x020b0000, /* 020b0000 //C 0f29f   860 sz	r0 */
0x0000f2a0, 0x11f2f2a3, /* 11f2f2a3 //C 0f2a0   861 jz	g_err */
0x0000f2a1, 0x07b0f706, /* 07b0f706 //C 0f2a1   862 ld	r11,reg15 */
0x0000f2a2, 0x01f2f27e, /* 01f2f27e //C 0f2a2   863 jmp	g_go11 */
0x0000f2a3, 0x0102f2a6, /* 0102f2a6 //C 0f2a3   865 mvzl	r0,g_err_addr */
0x0000f2a4, 0x0400f570, /* 0400f570 //C 0f2a4   866 call	printsnl */
0x0000f2a5, 0x0ffd0000, /* 0ffd0000 //C 0f2a5   868 pop	pc */
0x0000f2a6, 0x0000004e, /* 0000004e //C 0f2a6   870 db	78 */
0x0000f2a7, 0x0000006f, /* 0000006f //C 0f2a7   870 db	111 */
0x0000f2a8, 0x00000020, /* 00000020 //C 0f2a8   870 db	32 */
0x0000f2a9, 0x00000061, /* 00000061 //C 0f2a9   870 db	97 */
0x0000f2aa, 0x00000064, /* 00000064 //C 0f2aa   870 db	100 */
0x0000f2ab, 0x00000064, /* 00000064 //C 0f2ab   870 db	100 */
0x0000f2ac, 0x00000072, /* 00000072 //C 0f2ac   870 db	114 */
0x0000f2ad, 0x00000065, /* 00000065 //C 0f2ad   870 db	101 */
0x0000f2ae, 0x00000073, /* 00000073 //C 0f2ae   870 db	115 */
0x0000f2af, 0x00000073, /* 00000073 //C 0f2af   870 db	115 */
0x0000f2b0, 0x00000000, /* 00000000 //C 0f2b0   870 db */
0x0000f2b1, 0x00000052, /* 00000052 //C 0f2b1   871 db	82 */
0x0000f2b2, 0x00000075, /* 00000075 //C 0f2b2   871 db	117 */
0x0000f2b3, 0x0000006e, /* 0000006e //C 0f2b3   871 db	110 */
0x0000f2b4, 0x00000020, /* 00000020 //C 0f2b4   871 db	32 */
0x0000f2b5, 0x00000000, /* 00000000 //C 0f2b5   871 db */
0x0000f2b6, 0x0ded0000, /* 0ded0000 //C 0f2b6   876 push	lr */
0x0000f2b7, 0x0122f7b2, /* 0122f7b2 //C 0f2b7   877 mvzl	r2,helps */
0x0000f2b8, 0x01320000, /* 01320000 //C 0f2b8   878 mvzl	r3,0 */
0x0000f2b9, 0x0b038200, /* 0b038200 //C 0f2b9   880 ld	r0,r3+,r2	; pick a char */
0x0000f2ba, 0x020b0000, /* 020b0000 //C 0f2ba   881 sz	r0		; is it eos? */
0x0000f2bb, 0x21f2f2bf, /* 21f2f2bf //C 0f2bb   882 jnz	h_print */
0x0000f2bc, 0x0b038200, /* 0b038200 //C 0f2bc   885 ld	r0,r3+,r2	; get first char of next string */
0x0000f2bd, 0x020b0000, /* 020b0000 //C 0f2bd   886 sz	r0 */
0x0000f2be, 0x11f2f2c1, /* 11f2f2c1 //C 0f2be   887 jz	h_eof */
0x0000f2bf, 0x0400f533, /* 0400f533 //C 0f2bf   889 call	putchar */
0x0000f2c0, 0x01f2f2b9, /* 01f2f2b9 //C 0f2c0   890 jmp	h_cyc */
0x0000f2c1, 0x0ffd0000, /* 0ffd0000 //C 0f2c1   892 pop	pc */
0x0000f2c2, 0x0ded0000, /* 0ded0000 //C 0f2c2   898 push	lr */
0x0000f2c3, 0x0d1d0000, /* 0d1d0000 //C 0f2c3   899 push	r1 */
0x0000f2c4, 0x00100000, /* 00100000 //C 0f2c4   900 mov	r1,r0 */
0x0000f2c5, 0x01180010, /* 01180010 //C 0f2c5   901 cmp	r1,16		; go out if nr>16 */
0x0000f2c6, 0x91f2f2fa, /* 91f2f2fa //C 0f2c6   902 HI jmp	prn_ret */
0x0000f2c7, 0x0118000f, /* 0118000f //C 0f2c7   903 cmp	r1,15		; nr=Flag? */
0x0000f2c8, 0xa1f2f2cf, /* a1f2f2cf //C 0f2c8   904 LS jmp	prn_015 */
0x0000f2c9, 0x01020046, /* 01020046 //C 0f2c9   906 mvzl	r0,'F' */
0x0000f2ca, 0x0400f533, /* 0400f533 //C 0f2ca   907 call	putchar */
0x0000f2cb, 0x01020020, /* 01020020 //C 0f2cb   908 mvzl	r0,32 */
0x0000f2cc, 0x0400f533, /* 0400f533 //C 0f2cc   909 call	putchar */
0x0000f2cd, 0x0400f533, /* 0400f533 //C 0f2cd   910 call	putchar */
0x0000f2ce, 0x01f2f2fa, /* 01f2f2fa //C 0f2ce   911 jmp	prn_ret */
0x0000f2cf, 0x0118000f, /* 0118000f //C 0f2cf   913 cmp	r1,15 */
0x0000f2d0, 0x21f2f2d8, /* 21f2f2d8 //C 0f2d0   914 jnz	prn_no15 */
0x0000f2d1, 0x0102f2d4, /* 0102f2d4 //C 0f2d1   915 mvzl	r0,prn_PC */
0x0000f2d2, 0x0400f53a, /* 0400f53a //C 0f2d2   916 call	prints */
0x0000f2d3, 0x01f2f2fa, /* 01f2f2fa //C 0f2d3   917 jmp	prn_ret */
0x0000f2d4, 0x00000050, /* 00000050 //C 0f2d4   918 db	80 */
0x0000f2d5, 0x00000043, /* 00000043 //C 0f2d5   918 db	67 */
0x0000f2d6, 0x00000020, /* 00000020 //C 0f2d6   918 db	32 */
0x0000f2d7, 0x00000000, /* 00000000 //C 0f2d7   918 db */
0x0000f2d8, 0x0118000e, /* 0118000e //C 0f2d8   920 cmp	r1,14 */
0x0000f2d9, 0x21f2f2e1, /* 21f2f2e1 //C 0f2d9   921 jnz	prn_no14 */
0x0000f2da, 0x0102f2dd, /* 0102f2dd //C 0f2da   922 mvzl	r0,prn_LR */
0x0000f2db, 0x0400f53a, /* 0400f53a //C 0f2db   923 call	prints */
0x0000f2dc, 0x01f2f2fa, /* 01f2f2fa //C 0f2dc   924 jmp	prn_ret */
0x0000f2dd, 0x0000004c, /* 0000004c //C 0f2dd   925 db	76 */
0x0000f2de, 0x00000052, /* 00000052 //C 0f2de   925 db	82 */
0x0000f2df, 0x00000020, /* 00000020 //C 0f2df   925 db	32 */
0x0000f2e0, 0x00000000, /* 00000000 //C 0f2e0   925 db */
0x0000f2e1, 0x0118000d, /* 0118000d //C 0f2e1   927 cmp	r1,13 */
0x0000f2e2, 0x21f2f2ea, /* 21f2f2ea //C 0f2e2   928 jnz	prn_no13 */
0x0000f2e3, 0x0102f2e6, /* 0102f2e6 //C 0f2e3   929 mvzl	r0,prn_SP */
0x0000f2e4, 0x0400f53a, /* 0400f53a //C 0f2e4   930 call	prints */
0x0000f2e5, 0x01f2f2fa, /* 01f2f2fa //C 0f2e5   931 jmp	prn_ret */
0x0000f2e6, 0x00000053, /* 00000053 //C 0f2e6   932 db	83 */
0x0000f2e7, 0x00000050, /* 00000050 //C 0f2e7   932 db	80 */
0x0000f2e8, 0x00000020, /* 00000020 //C 0f2e8   932 db	32 */
0x0000f2e9, 0x00000000, /* 00000000 //C 0f2e9   932 db */
0x0000f2ea, 0x01020052, /* 01020052 //C 0f2ea   934 mvzl	r0,'R' */
0x0000f2eb, 0x0400f533, /* 0400f533 //C 0f2eb   935 call	putchar */
0x0000f2ec, 0x01180009, /* 01180009 //C 0f2ec   936 cmp	r1,9 */
0x0000f2ed, 0x91f2f2f4, /* 91f2f2f4 //C 0f2ed   937 HI jmp	prn_1015 */
0x0000f2ee, 0x00000100, /* 00000100 //C 0f2ee   939 mov	r0,r1 */
0x0000f2ef, 0x01040030, /* 01040030 //C 0f2ef   940 add	r0,'0' */
0x0000f2f0, 0x0400f533, /* 0400f533 //C 0f2f0   941 call	putchar */
0x0000f2f1, 0x01020020, /* 01020020 //C 0f2f1   942 mvzl	r0,32 */
0x0000f2f2, 0x0400f533, /* 0400f533 //C 0f2f2   943 call	putchar */
0x0000f2f3, 0x01f2f2fa, /* 01f2f2fa //C 0f2f3   944 jmp	prn_ret */
0x0000f2f4, 0x01020031, /* 01020031 //C 0f2f4   946 mvzl	r0,'1' */
0x0000f2f5, 0x0400f533, /* 0400f533 //C 0f2f5   947 call	putchar */
0x0000f2f6, 0x00000100, /* 00000100 //C 0f2f6   948 mov	r0,r1 */
0x0000f2f7, 0x0106000a, /* 0106000a //C 0f2f7   949 sub	r0,10 */
0x0000f2f8, 0x01040030, /* 01040030 //C 0f2f8   950 add	r0,'0' */
0x0000f2f9, 0x0400f533, /* 0400f533 //C 0f2f9   951 call	putchar */
0x0000f2fa, 0x0f1d0000, /* 0f1d0000 //C 0f2fa   953 pop	r1 */
0x0000f2fb, 0x0ffd0000, /* 0ffd0000 //C 0f2fb   954 pop	pc */
0x0000f2fc, 0x0ded0000, /* 0ded0000 //C 0f2fc   959 push	lr */
0x0000f2fd, 0x0d1d0000, /* 0d1d0000 //C 0f2fd   960 push	r1 */
0x0000f2fe, 0x01080010, /* 01080010 //C 0f2fe   961 cmp	r0,16 */
0x0000f2ff, 0x91f2f304, /* 91f2f304 //C 0f2ff   962 HI jmp	prv_ret */
0x0000f300, 0x0112f6f7, /* 0112f6f7 //C 0f300   963 mvzl	r1,reg0 */
0x0000f301, 0x0a010000, /* 0a010000 //C 0f301   964 ld	r0,r1,r0 */
0x0000f302, 0x01120004, /* 01120004 //C 0f302   965 mvzl	r1,4 */
0x0000f303, 0x0400f577, /* 0400f577 //C 0f303   966 call	print_vhex */
0x0000f304, 0x0f1d0000, /* 0f1d0000 //C 0f304   968 pop	r1 */
0x0000f305, 0x0ffd0000, /* 0ffd0000 //C 0f305   969 pop	pc */
0x0000f306, 0x0ded0000, /* 0ded0000 //C 0f306   976 push	lr */
0x0000f307, 0x0dad0000, /* 0dad0000 //C 0f307   977 push	r10 */
0x0000f308, 0x0d0d0000, /* 0d0d0000 //C 0f308   978 push	r0 */
0x0000f309, 0x00000a00, /* 00000a00 //C 0f309   979 mov	r0,r10 */
0x0000f30a, 0x0400f2c2, /* 0400f2c2 //C 0f30a   980 call	print_reg_name */
0x0000f30b, 0x01020020, /* 01020020 //C 0f30b   981 mvzl	r0,32 */
0x0000f30c, 0x0400f533, /* 0400f533 //C 0f30c   982 call	putchar */
0x0000f30d, 0x00000a00, /* 00000a00 //C 0f30d   983 mov	r0,r10 */
0x0000f30e, 0x0400f2fc, /* 0400f2fc //C 0f30e   984 call	print_reg_value */
0x0000f30f, 0x0102000a, /* 0102000a //C 0f30f   985 mvzl	r0,LF */
0x0000f310, 0x0400f533, /* 0400f533 //C 0f310   986 call	putchar */
0x0000f311, 0x0f0d0000, /* 0f0d0000 //C 0f311   987 pop	r0 */
0x0000f312, 0x0fad0000, /* 0fad0000 //C 0f312   988 pop	r10 */
0x0000f313, 0x0ffd0000, /* 0ffd0000 //C 0f313   989 pop	pc */
0x0000f314, 0x0ded0000, /* 0ded0000 //C 0f314   997 push	lr */
0x0000f315, 0x0400f533, /* 0400f533 //C 0f315   998 call	putchar */
0x0000f316, 0x001c0200, /* 001c0200 //C 0f316   999 test	r1,r2 */
0x0000f317, 0x21020031, /* 21020031 //C 0f317  1000 NZ mvzl r0,'1' */
0x0000f318, 0x11020030, /* 11020030 //C 0f318  1001 Z  mvzl r0,'0' */
0x0000f319, 0x0400f533, /* 0400f533 //C 0f319  1002 call	putchar */
0x0000f31a, 0x00000300, /* 00000300 //C 0f31a  1003 mov	r0,r3 */
0x0000f31b, 0x0400f533, /* 0400f533 //C 0f31b  1004 call	putchar */
0x0000f31c, 0x0ffd0000, /* 0ffd0000 //C 0f31c  1005 pop	pc */
0x0000f31d, 0x0ded0000, /* 0ded0000 //C 0f31d  1010 push	lr */
0x0000f31e, 0x0700f6f5, /* 0700f6f5 //C 0f31e  1011 ld	r0,called */
0x0000f31f, 0x020b0000, /* 020b0000 //C 0f31f  1012 sz	r0 */
0x0000f320, 0x11f2f329, /* 11f2f329 //C 0f320  1013 jz	r_not_called */
0x0000f321, 0x0102f346, /* 0102f346 //C 0f321  1015 mvzl	r0,msg_r_called */
0x0000f322, 0x0400f53a, /* 0400f53a //C 0f322  1016 call	prints */
0x0000f323, 0x0700f706, /* 0700f706 //C 0f323  1017 ld	r0,reg15 */
0x0000f324, 0x01120004, /* 01120004 //C 0f324  1018 mvzl	r1,4 */
0x0000f325, 0x0400f577, /* 0400f577 //C 0f325  1019 call	print_vhex */
0x0000f326, 0x0102000a, /* 0102000a //C 0f326  1020 mvzl	r0,LF */
0x0000f327, 0x0400f533, /* 0400f533 //C 0f327  1021 call	putchar */
0x0000f328, 0x01f2f32b, /* 01f2f32b //C 0f328  1022 jmp	r_start */
0x0000f329, 0x0102f35c, /* 0102f35c //C 0f329  1024 mvzl	r0,msg_r_notcalled */
0x0000f32a, 0x0400f53a, /* 0400f53a //C 0f32a  1025 call	prints */
0x0000f32b, 0x01a20000, /* 01a20000 //C 0f32b  1027 mvzl	r10,0 */
0x0000f32c, 0x0400f306, /* 0400f306 //C 0f32c  1029 call	print_reg_name_value */
0x0000f32d, 0x01a40001, /* 01a40001 //C 0f32d  1030 add	r10,1 */
0x0000f32e, 0x01a80011, /* 01a80011 //C 0f32e  1031 cmp	r10,17 */
0x0000f32f, 0x21f2f32c, /* 21f2f32c //C 0f32f  1032 jnz	r_cyc */
0x0000f330, 0x0710f707, /* 0710f707 //C 0f330  1034 ld	r1,regf */
0x0000f331, 0x01320020, /* 01320020 //C 0f331  1035 mvzl	r3,32 */
0x0000f332, 0x01020055, /* 01020055 //C 0f332  1036 mvzl	r0,'U' */
0x0000f333, 0x01220020, /* 01220020 //C 0f333  1037 mvzl	r2,0x20 */
0x0000f334, 0x0400f314, /* 0400f314 //C 0f334  1038 call	print_flag */
0x0000f335, 0x01020050, /* 01020050 //C 0f335  1039 mvzl	r0,'P' */
0x0000f336, 0x01220010, /* 01220010 //C 0f336  1040 mvzl	r2,0x10 */
0x0000f337, 0x0400f314, /* 0400f314 //C 0f337  1041 call	print_flag */
0x0000f338, 0x0102004f, /* 0102004f //C 0f338  1042 mvzl	r0,'O' */
0x0000f339, 0x01220008, /* 01220008 //C 0f339  1043 mvzl	r2,8 */
0x0000f33a, 0x0400f314, /* 0400f314 //C 0f33a  1044 call	print_flag */
0x0000f33b, 0x0102005a, /* 0102005a //C 0f33b  1045 mvzl	r0,'Z' */
0x0000f33c, 0x01220004, /* 01220004 //C 0f33c  1046 mvzl	r2,4 */
0x0000f33d, 0x0400f314, /* 0400f314 //C 0f33d  1047 call	print_flag */
0x0000f33e, 0x01020043, /* 01020043 //C 0f33e  1048 mvzl	r0,'C' */
0x0000f33f, 0x01220002, /* 01220002 //C 0f33f  1049 mvzl	r2,2 */
0x0000f340, 0x0400f314, /* 0400f314 //C 0f340  1050 call	print_flag */
0x0000f341, 0x01020053, /* 01020053 //C 0f341  1051 mvzl	r0,'S' */
0x0000f342, 0x01220001, /* 01220001 //C 0f342  1052 mvzl	r2,1 */
0x0000f343, 0x0132000a, /* 0132000a //C 0f343  1053 mvzl	r3,LF */
0x0000f344, 0x0400f314, /* 0400f314 //C 0f344  1054 call	print_flag */
0x0000f345, 0x0ffd0000, /* 0ffd0000 //C 0f345  1055 pop	pc */
0x0000f346, 0x0000004d, /* 0000004d //C 0f346  1057 db	77 */
0x0000f347, 0x0000006f, /* 0000006f //C 0f347  1057 db	111 */
0x0000f348, 0x0000006e, /* 0000006e //C 0f348  1057 db	110 */
0x0000f349, 0x00000069, /* 00000069 //C 0f349  1057 db	105 */
0x0000f34a, 0x00000074, /* 00000074 //C 0f34a  1057 db	116 */
0x0000f34b, 0x0000006f, /* 0000006f //C 0f34b  1057 db	111 */
0x0000f34c, 0x00000072, /* 00000072 //C 0f34c  1057 db	114 */
0x0000f34d, 0x00000020, /* 00000020 //C 0f34d  1057 db	32 */
0x0000f34e, 0x00000063, /* 00000063 //C 0f34e  1057 db	99 */
0x0000f34f, 0x00000061, /* 00000061 //C 0f34f  1057 db	97 */
0x0000f350, 0x0000006c, /* 0000006c //C 0f350  1057 db	108 */
0x0000f351, 0x0000006c, /* 0000006c //C 0f351  1057 db	108 */
0x0000f352, 0x00000065, /* 00000065 //C 0f352  1057 db	101 */
0x0000f353, 0x00000064, /* 00000064 //C 0f353  1057 db	100 */
0x0000f354, 0x00000020, /* 00000020 //C 0f354  1057 db	32 */
0x0000f355, 0x00000066, /* 00000066 //C 0f355  1057 db	102 */
0x0000f356, 0x00000072, /* 00000072 //C 0f356  1057 db	114 */
0x0000f357, 0x0000006f, /* 0000006f //C 0f357  1057 db	111 */
0x0000f358, 0x0000006d, /* 0000006d //C 0f358  1057 db	109 */
0x0000f359, 0x0000003a, /* 0000003a //C 0f359  1057 db	58 */
0x0000f35a, 0x00000020, /* 00000020 //C 0f35a  1057 db	32 */
0x0000f35b, 0x00000000, /* 00000000 //C 0f35b  1057 db */
0x0000f35c, 0x0000004d, /* 0000004d //C 0f35c  1058 db	77 */
0x0000f35d, 0x0000006f, /* 0000006f //C 0f35d  1058 db	111 */
0x0000f35e, 0x0000006e, /* 0000006e //C 0f35e  1058 db	110 */
0x0000f35f, 0x00000069, /* 00000069 //C 0f35f  1058 db	105 */
0x0000f360, 0x00000074, /* 00000074 //C 0f360  1058 db	116 */
0x0000f361, 0x0000006f, /* 0000006f //C 0f361  1058 db	111 */
0x0000f362, 0x00000072, /* 00000072 //C 0f362  1058 db	114 */
0x0000f363, 0x00000020, /* 00000020 //C 0f363  1058 db	32 */
0x0000f364, 0x0000006e, /* 0000006e //C 0f364  1058 db	110 */
0x0000f365, 0x0000006f, /* 0000006f //C 0f365  1058 db	111 */
0x0000f366, 0x00000074, /* 00000074 //C 0f366  1058 db	116 */
0x0000f367, 0x00000020, /* 00000020 //C 0f367  1058 db	32 */
0x0000f368, 0x00000063, /* 00000063 //C 0f368  1058 db	99 */
0x0000f369, 0x00000061, /* 00000061 //C 0f369  1058 db	97 */
0x0000f36a, 0x0000006c, /* 0000006c //C 0f36a  1058 db	108 */
0x0000f36b, 0x0000006c, /* 0000006c //C 0f36b  1058 db	108 */
0x0000f36c, 0x00000065, /* 00000065 //C 0f36c  1058 db	101 */
0x0000f36d, 0x00000064, /* 00000064 //C 0f36d  1058 db	100 */
0x0000f36e, 0x00000020, /* 00000020 //C 0f36e  1058 db	32 */
0x0000f36f, 0x00000062, /* 00000062 //C 0f36f  1058 db	98 */
0x0000f370, 0x00000079, /* 00000079 //C 0f370  1058 db	121 */
0x0000f371, 0x00000020, /* 00000020 //C 0f371  1058 db	32 */
0x0000f372, 0x00000075, /* 00000075 //C 0f372  1058 db	117 */
0x0000f373, 0x00000073, /* 00000073 //C 0f373  1058 db	115 */
0x0000f374, 0x00000065, /* 00000065 //C 0f374  1058 db	101 */
0x0000f375, 0x00000072, /* 00000072 //C 0f375  1058 db	114 */
0x0000f376, 0x0000000a, /* 0000000a //C 0f376  1058 db	10 */
0x0000f377, 0x00000000, /* 00000000 //C 0f377  1058 db */
0x0000f378, 0x0ded0000, /* 0ded0000 //C 0f378  1063 push	lr */
0x0000f379, 0x07a0f6f6, /* 07a0f6f6 //C 0f379  1064 ld	r10,nuof_reg	; Reg num is in R10 */
0x0000f37a, 0x01a80010, /* 01a80010 //C 0f37a  1065 cmp	r10,16 */
0x0000f37b, 0xa1f2f390, /* a1f2f390 //C 0f37b  1066 LS jmp	rx_nr_ok */
0x0000f37c, 0x0102f37f, /* 0102f37f //C 0f37c  1067 mvzl	r0,rx_err_nr */
0x0000f37d, 0x0400f570, /* 0400f570 //C 0f37d  1068 call	printsnl */
0x0000f37e, 0x01f2f3ac, /* 01f2f3ac //C 0f37e  1069 jmp	rx_ret */
0x0000f37f, 0x0000004e, /* 0000004e //C 0f37f  1071 db	78 */
0x0000f380, 0x0000006f, /* 0000006f //C 0f380  1071 db	111 */
0x0000f381, 0x00000020, /* 00000020 //C 0f381  1071 db	32 */
0x0000f382, 0x00000073, /* 00000073 //C 0f382  1071 db	115 */
0x0000f383, 0x00000075, /* 00000075 //C 0f383  1071 db	117 */
0x0000f384, 0x00000063, /* 00000063 //C 0f384  1071 db	99 */
0x0000f385, 0x00000068, /* 00000068 //C 0f385  1071 db	104 */
0x0000f386, 0x00000020, /* 00000020 //C 0f386  1071 db	32 */
0x0000f387, 0x00000072, /* 00000072 //C 0f387  1071 db	114 */
0x0000f388, 0x00000065, /* 00000065 //C 0f388  1071 db	101 */
0x0000f389, 0x00000067, /* 00000067 //C 0f389  1071 db	103 */
0x0000f38a, 0x00000069, /* 00000069 //C 0f38a  1071 db	105 */
0x0000f38b, 0x00000073, /* 00000073 //C 0f38b  1071 db	115 */
0x0000f38c, 0x00000074, /* 00000074 //C 0f38c  1071 db	116 */
0x0000f38d, 0x00000065, /* 00000065 //C 0f38d  1071 db	101 */
0x0000f38e, 0x00000072, /* 00000072 //C 0f38e  1071 db	114 */
0x0000f38f, 0x00000000, /* 00000000 //C 0f38f  1071 db */
0x0000f390, 0x0122f6ef, /* 0122f6ef //C 0f390  1073 mvzl	r2,words */
0x0000f391, 0x0e420001, /* 0e420001 //C 0f391  1074 ld	r4,r2,1		; get aof first parameter */
0x0000f392, 0x024b0000, /* 024b0000 //C 0f392  1075 sz	r4		; is it NULL? */
0x0000f393, 0x11f2f3ab, /* 11f2f3ab //C 0f393  1076 jz	rx_print */
0x0000f394, 0x00000400, /* 00000400 //C 0f394  1077 mov	r0,r4 */
0x0000f395, 0x0400f4f6, /* 0400f4f6 //C 0f395  1078 call	htoi */
0x0000f396, 0x00500100, /* 00500100 //C 0f396  1079 mov	r5,r1		; Value is in R5 */
0x0000f397, 0x31f2f3a7, /* 31f2f3a7 //C 0f397  1080 C1 jmp	rx_val_ok */
0x0000f398, 0x0102f39b, /* 0102f39b //C 0f398  1081 mvzl	r0,rx_err_val */
0x0000f399, 0x0400f570, /* 0400f570 //C 0f399  1082 call	printsnl */
0x0000f39a, 0x01f2f3ac, /* 01f2f3ac //C 0f39a  1083 jmp	rx_ret */
0x0000f39b, 0x00000056, /* 00000056 //C 0f39b  1085 db	86 */
0x0000f39c, 0x00000061, /* 00000061 //C 0f39c  1085 db	97 */
0x0000f39d, 0x0000006c, /* 0000006c //C 0f39d  1085 db	108 */
0x0000f39e, 0x00000075, /* 00000075 //C 0f39e  1085 db	117 */
0x0000f39f, 0x00000065, /* 00000065 //C 0f39f  1085 db	101 */
0x0000f3a0, 0x00000020, /* 00000020 //C 0f3a0  1085 db	32 */
0x0000f3a1, 0x00000065, /* 00000065 //C 0f3a1  1085 db	101 */
0x0000f3a2, 0x00000072, /* 00000072 //C 0f3a2  1085 db	114 */
0x0000f3a3, 0x00000072, /* 00000072 //C 0f3a3  1085 db	114 */
0x0000f3a4, 0x0000006f, /* 0000006f //C 0f3a4  1085 db	111 */
0x0000f3a5, 0x00000072, /* 00000072 //C 0f3a5  1085 db	114 */
0x0000f3a6, 0x00000000, /* 00000000 //C 0f3a6  1085 db */
0x0000f3a7, 0x01a80010, /* 01a80010 //C 0f3a7  1087 cmp	r10,16		; Flag reg? */
0x0000f3a8, 0x115f003f, /* 115f003f //C 0f3a8  1088 EQ and	r5,0x3f */
0x0000f3a9, 0x0102f6f7, /* 0102f6f7 //C 0f3a9  1089 mvzl	r0,reg0 */
0x0000f3aa, 0x08500a00, /* 08500a00 //C 0f3aa  1090 st	r5,r0,r10 */
0x0000f3ab, 0x0400f306, /* 0400f306 //C 0f3ab  1092 call	print_reg_name_value */
0x0000f3ac, 0x0ffd0000, /* 0ffd0000 //C 0f3ac  1094 pop	pc */
0x0000f3ad, 0x0102000d, /* 0102000d //C 0f3ad  1100 mvzl	r0,13 */
0x0000f3ae, 0x0600f6f6, /* 0600f6f6 //C 0f3ae  1101 st	r0,nuof_reg */
0x0000f3af, 0x01f2f378, /* 01f2f378 //C 0f3af  1102 jmp	cmd_rx */
0x0000f3b0, 0x0102000e, /* 0102000e //C 0f3b0  1107 mvzl	r0,14 */
0x0000f3b1, 0x0600f6f6, /* 0600f6f6 //C 0f3b1  1108 st	r0,nuof_reg */
0x0000f3b2, 0x01f2f378, /* 01f2f378 //C 0f3b2  1109 jmp	cmd_rx */
0x0000f3b3, 0x0102000f, /* 0102000f //C 0f3b3  1114 mvzl	r0,15 */
0x0000f3b4, 0x0600f6f6, /* 0600f6f6 //C 0f3b4  1115 st	r0,nuof_reg */
0x0000f3b5, 0x01f2f378, /* 01f2f378 //C 0f3b5  1116 jmp	cmd_rx */
0x0000f3b6, 0x01020010, /* 01020010 //C 0f3b6  1121 mvzl	r0,16 */
0x0000f3b7, 0x0600f6f6, /* 0600f6f6 //C 0f3b7  1122 st	r0,nuof_reg */
0x0000f3b8, 0x01f2f378, /* 01f2f378 //C 0f3b8  1123 jmp	cmd_rx */
0x0000f3b9, 0x0ded0000, /* 0ded0000 //C 0f3b9  1128 push	lr */
0x0000f3ba, 0x01120001, /* 01120001 //C 0f3ba  1129 mvzl	r1,version_main */
0x0000f3bb, 0x01220004, /* 01220004 //C 0f3bb  1130 mvzl	r2,version_sub */
0x0000f3bc, 0x06020100, /* 06020100 //C 0f3bc  1131 rds	r0,SVER */
0x0000f3bd, 0x0631c002, /* 0631c002 //C 0f3bd  1132 getbz	r3,r0,2 */
0x0000f3be, 0x0641c001, /* 0641c001 //C 0f3be  1133 getbz	r4,r0,1 */
0x0000f3bf, 0x0651c000, /* 0651c000 //C 0f3bf  1134 getbz	r5,r0,0 */
0x0000f3c0, 0x06620200, /* 06620200 //C 0f3c0  1135 rds	r6,SFEAT1 */
0x0000f3c1, 0x06720300, /* 06720300 //C 0f3c1  1136 rds	r7,SFEAT2 */
0x0000f3c2, 0xf400f66d, /* f400f66d //C 0f3c2  1137 ces	pesf */
0x0000f3c3, 0x00000070, /* 00000070 //C 0f3c3  1138 db	112 */
0x0000f3c4, 0x0000006d, /* 0000006d //C 0f3c4  1138 db	109 */
0x0000f3c5, 0x0000006f, /* 0000006f //C 0f3c5  1138 db	111 */
0x0000f3c6, 0x0000006e, /* 0000006e //C 0f3c6  1138 db	110 */
0x0000f3c7, 0x0000003a, /* 0000003a //C 0f3c7  1138 db	58 */
0x0000f3c8, 0x00000020, /* 00000020 //C 0f3c8  1138 db	32 */
0x0000f3c9, 0x00000025, /* 00000025 //C 0f3c9  1138 db	37 */
0x0000f3ca, 0x00000064, /* 00000064 //C 0f3ca  1138 db	100 */
0x0000f3cb, 0x0000002e, /* 0000002e //C 0f3cb  1138 db	46 */
0x0000f3cc, 0x00000025, /* 00000025 //C 0f3cc  1138 db	37 */
0x0000f3cd, 0x00000064, /* 00000064 //C 0f3cd  1138 db	100 */
0x0000f3ce, 0x00000020, /* 00000020 //C 0f3ce  1138 db	32 */
0x0000f3cf, 0x00000063, /* 00000063 //C 0f3cf  1138 db	99 */
0x0000f3d0, 0x00000070, /* 00000070 //C 0f3d0  1138 db	112 */
0x0000f3d1, 0x00000075, /* 00000075 //C 0f3d1  1138 db	117 */
0x0000f3d2, 0x0000003a, /* 0000003a //C 0f3d2  1138 db	58 */
0x0000f3d3, 0x00000020, /* 00000020 //C 0f3d3  1138 db	32 */
0x0000f3d4, 0x00000025, /* 00000025 //C 0f3d4  1138 db	37 */
0x0000f3d5, 0x00000064, /* 00000064 //C 0f3d5  1138 db	100 */
0x0000f3d6, 0x0000002e, /* 0000002e //C 0f3d6  1138 db	46 */
0x0000f3d7, 0x00000025, /* 00000025 //C 0f3d7  1138 db	37 */
0x0000f3d8, 0x00000064, /* 00000064 //C 0f3d8  1138 db	100 */
0x0000f3d9, 0x0000002e, /* 0000002e //C 0f3d9  1138 db	46 */
0x0000f3da, 0x00000025, /* 00000025 //C 0f3da  1138 db	37 */
0x0000f3db, 0x00000064, /* 00000064 //C 0f3db  1138 db	100 */
0x0000f3dc, 0x00000020, /* 00000020 //C 0f3dc  1138 db	32 */
0x0000f3dd, 0x00000066, /* 00000066 //C 0f3dd  1138 db	102 */
0x0000f3de, 0x00000065, /* 00000065 //C 0f3de  1138 db	101 */
0x0000f3df, 0x00000061, /* 00000061 //C 0f3df  1138 db	97 */
0x0000f3e0, 0x00000074, /* 00000074 //C 0f3e0  1138 db	116 */
0x0000f3e1, 0x00000031, /* 00000031 //C 0f3e1  1138 db	49 */
0x0000f3e2, 0x0000003a, /* 0000003a //C 0f3e2  1138 db	58 */
0x0000f3e3, 0x00000020, /* 00000020 //C 0f3e3  1138 db	32 */
0x0000f3e4, 0x00000025, /* 00000025 //C 0f3e4  1138 db	37 */
0x0000f3e5, 0x00000078, /* 00000078 //C 0f3e5  1138 db	120 */
0x0000f3e6, 0x00000020, /* 00000020 //C 0f3e6  1138 db	32 */
0x0000f3e7, 0x00000066, /* 00000066 //C 0f3e7  1138 db	102 */
0x0000f3e8, 0x00000065, /* 00000065 //C 0f3e8  1138 db	101 */
0x0000f3e9, 0x00000061, /* 00000061 //C 0f3e9  1138 db	97 */
0x0000f3ea, 0x00000074, /* 00000074 //C 0f3ea  1138 db	116 */
0x0000f3eb, 0x00000032, /* 00000032 //C 0f3eb  1138 db	50 */
0x0000f3ec, 0x0000003a, /* 0000003a //C 0f3ec  1138 db	58 */
0x0000f3ed, 0x00000020, /* 00000020 //C 0f3ed  1138 db	32 */
0x0000f3ee, 0x00000025, /* 00000025 //C 0f3ee  1138 db	37 */
0x0000f3ef, 0x00000078, /* 00000078 //C 0f3ef  1138 db	120 */
0x0000f3f0, 0x0000000a, /* 0000000a //C 0f3f0  1138 db	10 */
0x0000f3f1, 0x00000000, /* 00000000 //C 0f3f1  1138 db */
0x0000f3f2, 0x0ffd0000, /* 0ffd0000 //C 0f3f2  1139 pop	pc */
0x0000f3f3, 0x0d0d0000, /* 0d0d0000 //C 0f3f3  1149 push	r0 */
0x0000f3f4, 0x0d1d0000, /* 0d1d0000 //C 0f3f4  1150 push	r1 */
0x0000f3f5, 0x0d2d0000, /* 0d2d0000 //C 0f3f5  1151 push	r2 */
0x0000f3f6, 0x0d3d0000, /* 0d3d0000 //C 0f3f6  1152 push	r3 */
0x0000f3f7, 0x0d4d0000, /* 0d4d0000 //C 0f3f7  1153 push	r4 */
0x0000f3f8, 0x020b0000, /* 020b0000 //C 0f3f8  1155 sz	r0		; check pointers */
0x0000f3f9, 0x11f2f40c, /* 11f2f40c //C 0f3f9  1156 jz	sup_ret */
0x0000f3fa, 0x021b0000, /* 021b0000 //C 0f3fa  1157 sz	r1 */
0x0000f3fb, 0x11f2f40c, /* 11f2f40c //C 0f3fb  1158 jz	sup_ret */
0x0000f3fc, 0x01220000, /* 01220000 //C 0f3fc  1160 mvzl	r2,0		; byte index in input */
0x0000f3fd, 0x0e300000, /* 0e300000 //C 0f3fd  1161 ld	r3,r0		; pick next word */
0x0000f3fe, 0x023b0000, /* 023b0000 //C 0f3fe  1162 sz	r3		; is it EOS? */
0x0000f3ff, 0x11f2f40a, /* 11f2f40a //C 0f3ff  1163 jz	sup_fin */
0x0000f400, 0x06414302, /* 06414302 //C 0f400  1165 getbz	r4,r3,r2	; pick byte */
0x0000f401, 0x024b0000, /* 024b0000 //C 0f401  1166 sz	r4		; check byte */
0x0000f402, 0x11f2f408, /* 11f2f408 //C 0f402  1167 jz	sup_next_word */
0x0000f403, 0x0c410000, /* 0c410000 //C 0f403  1168 st	r4,r1		; produce output */
0x0000f404, 0x01140001, /* 01140001 //C 0f404  1169 inc	r1 */
0x0000f405, 0x01240001, /* 01240001 //C 0f405  1170 inc	r2		; advance byte index */
0x0000f406, 0x01280004, /* 01280004 //C 0f406  1171 cmp	r2,4 */
0x0000f407, 0x21f2f400, /* 21f2f400 //C 0f407  1172 NE jmp	sup_next_char */
0x0000f408, 0x01040001, /* 01040001 //C 0f408  1174 inc	r0 */
0x0000f409, 0x01f2f3fc, /* 01f2f3fc //C 0f409  1175 jmp	sup_cyc */
0x0000f40a, 0x01320000, /* 01320000 //C 0f40a  1177 mvzl	r3,0		; gen EOS in output */
0x0000f40b, 0x0c310000, /* 0c310000 //C 0f40b  1178 st	r3,r1 */
0x0000f40c, 0x0f4d0000, /* 0f4d0000 //C 0f40c  1180 pop	r4 */
0x0000f40d, 0x0f3d0000, /* 0f3d0000 //C 0f40d  1181 pop	r3 */
0x0000f40e, 0x0f2d0000, /* 0f2d0000 //C 0f40e  1182 pop	r2 */
0x0000f40f, 0x0f1d0000, /* 0f1d0000 //C 0f40f  1183 pop	r1 */
0x0000f410, 0x0f0d0000, /* 0f0d0000 //C 0f410  1184 pop	r0 */
0x0000f411, 0x00f00e00, /* 00f00e00 //C 0f411  1185 ret */
0x0000f412, 0x0d0d0000, /* 0d0d0000 //C 0f412  1192 push	r0 */
0x0000f413, 0x0d1d0000, /* 0d1d0000 //C 0f413  1193 push	r1 */
0x0000f414, 0x0d2d0000, /* 0d2d0000 //C 0f414  1194 push	r2 */
0x0000f415, 0x0d3d0000, /* 0d3d0000 //C 0f415  1195 push	r3 */
0x0000f416, 0x0d4d0000, /* 0d4d0000 //C 0f416  1196 push	r4 */
0x0000f417, 0x0d5d0000, /* 0d5d0000 //C 0f417  1197 push	r5 */
0x0000f418, 0x0d6d0000, /* 0d6d0000 //C 0f418  1198 push	r6 */
0x0000f419, 0x020b0000, /* 020b0000 //C 0f419  1200 sz	r0		; check pointers */
0x0000f41a, 0x11f2f439, /* 11f2f439 //C 0f41a  1201 jz	sp_ret */
0x0000f41b, 0x021b0000, /* 021b0000 //C 0f41b  1202 sz	r1 */
0x0000f41c, 0x11f2f439, /* 11f2f439 //C 0f41c  1203 jz	sp_ret */
0x0000f41d, 0x01520000, /* 01520000 //C 0f41d  1204 mvzl	r5,0		; output byte index */
0x0000f41e, 0x01620000, /* 01620000 //C 0f41e  1205 mvzl	r6,0		; output word */
0x0000f41f, 0x01220000, /* 01220000 //C 0f41f  1207 mvzl	r2,0		; byte index in input */
0x0000f420, 0x0e300000, /* 0e300000 //C 0f420  1208 ld	r3,r0		; pick next word */
0x0000f421, 0x023b0000, /* 023b0000 //C 0f421  1209 sz	r3		; is it EOS? */
0x0000f422, 0x11f2f433, /* 11f2f433 //C 0f422  1210 jz	sp_fin */
0x0000f423, 0x06414302, /* 06414302 //C 0f423  1212 getbz	r4,r3,r2	; pick byte */
0x0000f424, 0x024b0000, /* 024b0000 //C 0f424  1213 sz	r4		; check byte */
0x0000f425, 0x11f2f431, /* 11f2f431 //C 0f425  1214 jz	sp_next_word */
0x0000f426, 0x07610405, /* 07610405 //C 0f426  1216 putb	r6,r4,r5	; pack output word */
0x0000f427, 0x01540001, /* 01540001 //C 0f427  1217 inc	r5		; advance output byte index */
0x0000f428, 0x01580004, /* 01580004 //C 0f428  1218 cmp	r5,4		; is it full? */
0x0000f429, 0x21f2f42e, /* 21f2f42e //C 0f429  1219 jnz	sp_not_full */
0x0000f42a, 0x0c610000, /* 0c610000 //C 0f42a  1220 st	r6,r1		; if yes, store word in mem */
0x0000f42b, 0x01520000, /* 01520000 //C 0f42b  1221 mvzl	r5,0		; and restart word */
0x0000f42c, 0x01620000, /* 01620000 //C 0f42c  1222 mvzl	r6,0 */
0x0000f42d, 0x01140001, /* 01140001 //C 0f42d  1223 inc	r1		; and advance outout pointer */
0x0000f42e, 0x01240001, /* 01240001 //C 0f42e  1225 inc	r2		; advance input byte index */
0x0000f42f, 0x01280004, /* 01280004 //C 0f42f  1226 cmp	r2,4 */
0x0000f430, 0x21f2f423, /* 21f2f423 //C 0f430  1227 NE jmp	sp_next_char */
0x0000f431, 0x01040001, /* 01040001 //C 0f431  1229 inc	r0 */
0x0000f432, 0x01f2f41f, /* 01f2f41f //C 0f432  1230 jmp	sp_cyc */
0x0000f433, 0x025b0000, /* 025b0000 //C 0f433  1232 sz	r5 */
0x0000f434, 0x11f2f437, /* 11f2f437 //C 0f434  1233 jz	sp_word_flushed */
0x0000f435, 0x0c610000, /* 0c610000 //C 0f435  1235 st	r6,r1 */
0x0000f436, 0x01140001, /* 01140001 //C 0f436  1236 inc	r1 */
0x0000f437, 0x01320000, /* 01320000 //C 0f437  1238 mvzl	r3,0		; gen EOS in output */
0x0000f438, 0x0c310000, /* 0c310000 //C 0f438  1239 st	r3,r1 */
0x0000f439, 0x0f6d0000, /* 0f6d0000 //C 0f439  1241 pop	r6 */
0x0000f43a, 0x0f5d0000, /* 0f5d0000 //C 0f43a  1242 pop	r5 */
0x0000f43b, 0x0f4d0000, /* 0f4d0000 //C 0f43b  1243 pop	r4 */
0x0000f43c, 0x0f3d0000, /* 0f3d0000 //C 0f43c  1244 pop	r3 */
0x0000f43d, 0x0f2d0000, /* 0f2d0000 //C 0f43d  1245 pop	r2 */
0x0000f43e, 0x0f1d0000, /* 0f1d0000 //C 0f43e  1246 pop	r1 */
0x0000f43f, 0x0f0d0000, /* 0f0d0000 //C 0f43f  1247 pop	r0 */
0x0000f440, 0x00f00e00, /* 00f00e00 //C 0f440  1248 ret */
0x0000f441, 0x0d3d0000, /* 0d3d0000 //C 0f441  1258 push	r3 */
0x0000f442, 0x0d4d0000, /* 0d4d0000 //C 0f442  1259 push	r4 */
0x0000f443, 0x01220000, /* 01220000 //C 0f443  1261 mvzl	r2,0		; byte index re-start */
0x0000f444, 0x0e310000, /* 0e310000 //C 0f444  1262 ld	r3,r1		; get next word */
0x0000f445, 0x023b0000, /* 023b0000 //C 0f445  1263 sz	r3		; check for eof */
0x0000f446, 0x11f2f453, /* 11f2f453 //C 0f446  1264 jz	strchr_no	; eof string found */
0x0000f447, 0x06414302, /* 06414302 //C 0f447  1266 getbz	r4,r3,r2	; pick a byte */
0x0000f448, 0x024b0000, /* 024b0000 //C 0f448  1267 sz	r4		; is it zero? */
0x0000f449, 0x11f2f44f, /* 11f2f44f //C 0f449  1268 jz	strchr_word	; if yes, pick next word */
0x0000f44a, 0x00480000, /* 00480000 //C 0f44a  1269 cmp	r4,r0		; compare */
0x0000f44b, 0x11f2f451, /* 11f2f451 //C 0f44b  1270 jz	strchr_yes	; found it */
0x0000f44c, 0x01240001, /* 01240001 //C 0f44c  1272 inc	r2		; advance byte index */
0x0000f44d, 0x01280004, /* 01280004 //C 0f44d  1273 cmp	r2,4		; check byte overflow */
0x0000f44e, 0x21f2f447, /* 21f2f447 //C 0f44e  1274 jnz	strchr_go	; no, overflow, go on */
0x0000f44f, 0x011a0001, /* 011a0001 //C 0f44f  1276 plus	r1,1		; go to next char */
0x0000f450, 0x01f2f443, /* 01f2f443 //C 0f450  1277 jmp	strchr_cyc */
0x0000f451, 0x020c0000, /* 020c0000 //C 0f451  1279 sec */
0x0000f452, 0x01f2f455, /* 01f2f455 //C 0f452  1280 jmp	strchr_ret */
0x0000f453, 0x01120000, /* 01120000 //C 0f453  1282 mvzl	r1,0 */
0x0000f454, 0x020d0000, /* 020d0000 //C 0f454  1283 clc */
0x0000f455, 0x0f4d0000, /* 0f4d0000 //C 0f455  1285 pop	r4 */
0x0000f456, 0x0f3d0000, /* 0f3d0000 //C 0f456  1286 pop	r3 */
0x0000f457, 0x00f00e00, /* 00f00e00 //C 0f457  1289 ret */
0x0000f458, 0x0ded0000, /* 0ded0000 //C 0f458  1300 push	lr		; Save used registers */
0x0000f459, 0x0d0d0000, /* 0d0d0000 //C 0f459  1301 push	r0		; and input parameters */
0x0000f45a, 0x0d2d0000, /* 0d2d0000 //C 0f45a  1303 push	r2 */
0x0000f45b, 0x0d4d0000, /* 0d4d0000 //C 0f45b  1304 push	r4 */
0x0000f45c, 0x0d5d0000, /* 0d5d0000 //C 0f45c  1305 push	r5 */
0x0000f45d, 0x0d6d0000, /* 0d6d0000 //C 0f45d  1306 push	r6 */
0x0000f45e, 0x0d7d0000, /* 0d7d0000 //C 0f45e  1307 push	r7		; byte idx in string 1 */
0x0000f45f, 0x0d8d0000, /* 0d8d0000 //C 0f45f  1308 push	r8		; byte idx in string 2 */
0x0000f460, 0x01720000, /* 01720000 //C 0f460  1309 mvzl	r7,0 */
0x0000f461, 0x01820000, /* 01820000 //C 0f461  1310 mvzl	r8,0 */
0x0000f462, 0x0e200000, /* 0e200000 //C 0f462  1312 ld	r2,r0		; Got one char from first str */
0x0000f463, 0x022b0000, /* 022b0000 //C 0f463  1313 sz	r2		; is it eos? */
0x0000f464, 0x11f2f473, /* 11f2f473 //C 0f464  1314 jz	streq_pick2	; if yes, go on */
0x0000f465, 0x06214207, /* 06214207 //C 0f465  1315 getbz	r2,r2,r7	; pick one byte */
0x0000f466, 0x022b0000, /* 022b0000 //C 0f466  1316 sz	r2		; is it 0? */
0x0000f467, 0x21f2f473, /* 21f2f473 //C 0f467  1317 jnz	streq_pick2	; if not, go on */
0x0000f468, 0x01740001, /* 01740001 //C 0f468  1318 inc	r7		; step to next byte */
0x0000f469, 0x01780004, /* 01780004 //C 0f469  1319 cmp	r7,4		; word is overflowed? */
0x0000f46a, 0x11f2f46f, /* 11f2f46f //C 0f46a  1320 jz	streq_p1ov */
0x0000f46b, 0x0e200000, /* 0e200000 //C 0f46b  1322 ld	r2,r0		; pick orig word, and */
0x0000f46c, 0x06214207, /* 06214207 //C 0f46c  1323 getbz	r2,r2,r7	; check next byte */
0x0000f46d, 0x022b0000, /* 022b0000 //C 0f46d  1324 sz	r2		; is it 0? */
0x0000f46e, 0x21f2f473, /* 21f2f473 //C 0f46e  1325 jnz	streq_pick2	; if not, go on */
0x0000f46f, 0x01040001, /* 01040001 //C 0f46f  1327 inc	r0		; if yes, move pointer */
0x0000f470, 0x01720000, /* 01720000 //C 0f470  1328 mvzl	r7,0		; and reset byte counter */
0x0000f471, 0x0e200000, /* 0e200000 //C 0f471  1329 ld	r2,r0		; get first byte of next word */
0x0000f472, 0x06214207, /* 06214207 //C 0f472  1330 getbz	r2,r2,r7 */
0x0000f473, 0x0e610000, /* 0e610000 //C 0f473  1333 ld	r6,r1		; pick from second string */
0x0000f474, 0x026b0000, /* 026b0000 //C 0f474  1334 sz	r6		; is it eos? */
0x0000f475, 0x11f2f484, /* 11f2f484 //C 0f475  1335 jz	streq_prep	; if yes, go to compare */
0x0000f476, 0x06614608, /* 06614608 //C 0f476  1336 getbz	r6,r6,r8	; pick a byte */
0x0000f477, 0x026b0000, /* 026b0000 //C 0f477  1337 sz	r6		; is it 0? */
0x0000f478, 0x21f2f484, /* 21f2f484 //C 0f478  1338 jnz	streq_prep	; if not, go to compare */
0x0000f479, 0x01840001, /* 01840001 //C 0f479  1339 inc	r8		; step to next byte */
0x0000f47a, 0x01880004, /* 01880004 //C 0f47a  1340 cmp	r8,4		; is word overflowed? */
0x0000f47b, 0x11f2f480, /* 11f2f480 //C 0f47b  1341 jz	streq_p2ov */
0x0000f47c, 0x0e610000, /* 0e610000 //C 0f47c  1343 ld	r6,r1		; pick orig word, and */
0x0000f47d, 0x06614608, /* 06614608 //C 0f47d  1344 getbz	r6,r6,r8	; check next byte */
0x0000f47e, 0x026b0000, /* 026b0000 //C 0f47e  1345 sz	r6		; is it 0? */
0x0000f47f, 0x21f2f484, /* 21f2f484 //C 0f47f  1346 jnz	streq_prep	; if not, go on */
0x0000f480, 0x01140001, /* 01140001 //C 0f480  1348 inc	r1		; if yes, move pointer */
0x0000f481, 0x01820000, /* 01820000 //C 0f481  1349 mvzl	r8,0		; and reset byte counter */
0x0000f482, 0x0e610000, /* 0e610000 //C 0f482  1350 ld	r6,r1		; get next word */
0x0000f483, 0x06614608, /* 06614608 //C 0f483  1351 getbz	r6,r6,r8	; and pick first byte */
0x0000f484, 0x023b0000, /* 023b0000 //C 0f484  1354 sz	r3		; Prepare for comparing */
0x0000f485, 0x112d0020, /* 112d0020 //C 0f485  1355 Z1 or	r2,0x20		; if insensitive case */
0x0000f486, 0x023b0000, /* 023b0000 //C 0f486  1356 sz	r3 */
0x0000f487, 0x116d0020, /* 116d0020 //C 0f487  1357 Z1 or	r6,0x20 */
0x0000f488, 0x00280600, /* 00280600 //C 0f488  1358 cmp	r2,r6		; compare them */
0x0000f489, 0x21f2f4a3, /* 21f2f4a3 //C 0f489  1359 jnz	streq_no	; if differs: strings are not equal */
0x0000f48a, 0x0e200000, /* 0e200000 //C 0f48a  1361 ld	r2,r0		; Pick original (non-prepared) */
0x0000f48b, 0x0e610000, /* 0e610000 //C 0f48b  1362 ld	r6,r1		; chars to check EOS */
0x0000f48c, 0x06214207, /* 06214207 //C 0f48c  1363 getbz	r2,r2,r7 */
0x0000f48d, 0x06614608, /* 06614608 //C 0f48d  1364 getbz	r6,r6,r8 */
0x0000f48e, 0x022b0000, /* 022b0000 //C 0f48e  1365 sz	r2		; convert them to boolean */
0x0000f48f, 0x21220001, /* 21220001 //C 0f48f  1366 Z0 mvzl	r2,1		; values in R2,R6 */
0x0000f490, 0x11220000, /* 11220000 //C 0f490  1367 Z1 mvzl	r2,0		; and copy in R4,R5 */
0x0000f491, 0x00400200, /* 00400200 //C 0f491  1368 mov	r4,r2 */
0x0000f492, 0x026b0000, /* 026b0000 //C 0f492  1369 sz	r6 */
0x0000f493, 0x21620001, /* 21620001 //C 0f493  1370 Z0 mvzl	r6,1 */
0x0000f494, 0x11620000, /* 11620000 //C 0f494  1371 Z1 mvzl r6,0 */
0x0000f495, 0x00500600, /* 00500600 //C 0f495  1372 mov	r5,r6 */
0x0000f496, 0x004d0500, /* 004d0500 //C 0f496  1373 or	r4,r5		; if both are EOS: equal */
0x0000f497, 0x11f2f4a5, /* 11f2f4a5 //C 0f497  1374 jz	streq_yes */
0x0000f498, 0x002f0600, /* 002f0600 //C 0f498  1375 and 	r2,r6		; just one is EOS: not equal */
0x0000f499, 0x11f2f4a3, /* 11f2f4a3 //C 0f499  1376 jz	streq_no */
0x0000f49a, 0x01740001, /* 01740001 //C 0f49a  1379 inc	r7		; step byte count */
0x0000f49b, 0x01780004, /* 01780004 //C 0f49b  1380 cmp	r7,4		; if word overflows */
0x0000f49c, 0x110a0001, /* 110a0001 //C 0f49c  1381 Z plus	r0,1		; then step the pointer */
0x0000f49d, 0x11720000, /* 11720000 //C 0f49d  1382 Z mvzl	r7,0		; and reset the byte counter */
0x0000f49e, 0x01840001, /* 01840001 //C 0f49e  1384 inc	r8 */
0x0000f49f, 0x01880004, /* 01880004 //C 0f49f  1385 cmp	r8,4 */
0x0000f4a0, 0x111a0001, /* 111a0001 //C 0f4a0  1386 Z plus	r1,1 */
0x0000f4a1, 0x11820000, /* 11820000 //C 0f4a1  1387 Z mvzl	r8,0 */
0x0000f4a2, 0x01f2f462, /* 01f2f462 //C 0f4a2  1388 jmp	streq_cyc */
0x0000f4a3, 0x020d0000, /* 020d0000 //C 0f4a3  1391 clc			; False result */
0x0000f4a4, 0x01f2f4a6, /* 01f2f4a6 //C 0f4a4  1392 jmp	streq_ret */
0x0000f4a5, 0x020c0000, /* 020c0000 //C 0f4a5  1395 sec			; True result */
0x0000f4a6, 0x0e610000, /* 0e610000 //C 0f4a6  1398 ld	r6,r1		; forward R1 to EOS */
0x0000f4a7, 0x026b0000, /* 026b0000 //C 0f4a7  1399 sz	r6 */
0x0000f4a8, 0x11f2f4ab, /* 11f2f4ab //C 0f4a8  1400 jz	streq_ret_ret */
0x0000f4a9, 0x01140001, /* 01140001 //C 0f4a9  1401 inc	r1 */
0x0000f4aa, 0x01f2f4a6, /* 01f2f4a6 //C 0f4aa  1402 jmp	streq_ret */
0x0000f4ab, 0x0f8d0000, /* 0f8d0000 //C 0f4ab  1404 pop	r8 */
0x0000f4ac, 0x0f7d0000, /* 0f7d0000 //C 0f4ac  1405 pop	r7 */
0x0000f4ad, 0x0f6d0000, /* 0f6d0000 //C 0f4ad  1406 pop	r6 */
0x0000f4ae, 0x0f5d0000, /* 0f5d0000 //C 0f4ae  1407 pop	r5 */
0x0000f4af, 0x0f4d0000, /* 0f4d0000 //C 0f4af  1408 pop	r4 */
0x0000f4b0, 0x0f2d0000, /* 0f2d0000 //C 0f4b0  1409 pop	r2 */
0x0000f4b1, 0x0f0d0000, /* 0f0d0000 //C 0f4b1  1411 pop	r0 */
0x0000f4b2, 0x0ffd0000, /* 0ffd0000 //C 0f4b2  1412 pop	pc */
0x0000f4b3, 0x0ded0000, /* 0ded0000 //C 0f4b3  1420 push	lr */
0x0000f4b4, 0x0d1d0000, /* 0d1d0000 //C 0f4b4  1421 push	r1 */
0x0000f4b5, 0x0d3d0000, /* 0d3d0000 //C 0f4b5  1422 push	r3 */
0x0000f4b6, 0x01320001, /* 01320001 //C 0f4b6  1423 mvzl	r3,1 */
0x0000f4b7, 0x0400f458, /* 0400f458 //C 0f4b7  1424 call	str_cmp_eq */
0x0000f4b8, 0x0f3d0000, /* 0f3d0000 //C 0f4b8  1425 pop	r3 */
0x0000f4b9, 0x0f1d0000, /* 0f1d0000 //C 0f4b9  1426 pop	r1 */
0x0000f4ba, 0x0ffd0000, /* 0ffd0000 //C 0f4ba  1427 pop	pc */
0x0000f4bb, 0x0ded0000, /* 0ded0000 //C 0f4bb  1435 push	lr */
0x0000f4bc, 0x0d1d0000, /* 0d1d0000 //C 0f4bc  1436 push	r1 */
0x0000f4bd, 0x0d3d0000, /* 0d3d0000 //C 0f4bd  1437 push	r3 */
0x0000f4be, 0x01320000, /* 01320000 //C 0f4be  1438 mvzl	r3,0 */
0x0000f4bf, 0x0400f458, /* 0400f458 //C 0f4bf  1439 call	str_cmp_eq */
0x0000f4c0, 0x0f3d0000, /* 0f3d0000 //C 0f4c0  1440 pop	r3 */
0x0000f4c1, 0x0f1d0000, /* 0f1d0000 //C 0f4c1  1441 pop	r1 */
0x0000f4c2, 0x0ffd0000, /* 0ffd0000 //C 0f4c2  1442 pop	pc */
0x0000f4c3, 0x0108002f, /* 0108002f //C 0f4c3  1451 cmp	r0,'/' */
0x0000f4c4, 0xa1f2f4d9, /* a1f2f4d9 //C 0f4c4  1452 LS jmp	hc2v_nok */
0x0000f4c5, 0x01080039, /* 01080039 //C 0f4c5  1453 cmp	r0,'9' */
0x0000f4c6, 0xa1f2f4d6, /* a1f2f4d6 //C 0f4c6  1454 LS jmp	hc2v_0_9 */
0x0000f4c7, 0x01080040, /* 01080040 //C 0f4c7  1455 cmp	r0,'@' */
0x0000f4c8, 0xa1f2f4d9, /* a1f2f4d9 //C 0f4c8  1456 LS jmp	hc2v_nok */
0x0000f4c9, 0x01080046, /* 01080046 //C 0f4c9  1457 cmp	r0,'F' */
0x0000f4ca, 0xa1f2f4d3, /* a1f2f4d3 //C 0f4ca  1458 LS jmp	hc2v_A_F */
0x0000f4cb, 0x01080060, /* 01080060 //C 0f4cb  1459 cmp	r0,'`' */
0x0000f4cc, 0xa1f2f4d9, /* a1f2f4d9 //C 0f4cc  1460 LS jmp	hc2v_nok */
0x0000f4cd, 0x01080066, /* 01080066 //C 0f4cd  1461 cmp	r0,'f' */
0x0000f4ce, 0xa1f2f4d0, /* a1f2f4d0 //C 0f4ce  1462 LS jmp	hc2v_a_f */
0x0000f4cf, 0x01f2f4d9, /* 01f2f4d9 //C 0f4cf  1463 jmp	hc2v_nok */
0x0000f4d0, 0x0104000a, /* 0104000a //C 0f4d0  1465 add	r0,10 */
0x0000f4d1, 0x01060061, /* 01060061 //C 0f4d1  1466 sub	r0,'a' */
0x0000f4d2, 0x01f2f4d7, /* 01f2f4d7 //C 0f4d2  1467 jmp	hc2v_ok */
0x0000f4d3, 0x0104000a, /* 0104000a //C 0f4d3  1469 add	r0,10 */
0x0000f4d4, 0x01060041, /* 01060041 //C 0f4d4  1470 sub	r0,'A' */
0x0000f4d5, 0x01f2f4d7, /* 01f2f4d7 //C 0f4d5  1471 jmp	hc2v_ok */
0x0000f4d6, 0x01060030, /* 01060030 //C 0f4d6  1473 sub	r0,'0' */
0x0000f4d7, 0x020c0000, /* 020c0000 //C 0f4d7  1475 sec */
0x0000f4d8, 0x00f00e00, /* 00f00e00 //C 0f4d8  1476 ret */
0x0000f4d9, 0x020d0000, /* 020d0000 //C 0f4d9  1478 clc */
0x0000f4da, 0x00f00e00, /* 00f00e00 //C 0f4da  1479 ret */
0x0000f4db, 0x0d1d0000, /* 0d1d0000 //C 0f4db  1483 push	r1 */
0x0000f4dc, 0x010f000f, /* 010f000f //C 0f4dc  1484 and	r0,0xf */
0x0000f4dd, 0x0112f4e1, /* 0112f4e1 //C 0f4dd  1485 mvzl	r1,v2hc_table */
0x0000f4de, 0x0a010000, /* 0a010000 //C 0f4de  1486 ld	r0,r1,r0 */
0x0000f4df, 0x0f1d0000, /* 0f1d0000 //C 0f4df  1487 pop	r1 */
0x0000f4e0, 0x00f00e00, /* 00f00e00 //C 0f4e0  1488 ret */
0x0000f4e1, 0x00000030, /* 00000030 //C 0f4e1  1489 db	48 */
0x0000f4e2, 0x00000031, /* 00000031 //C 0f4e2  1489 db	49 */
0x0000f4e3, 0x00000032, /* 00000032 //C 0f4e3  1489 db	50 */
0x0000f4e4, 0x00000033, /* 00000033 //C 0f4e4  1489 db	51 */
0x0000f4e5, 0x00000034, /* 00000034 //C 0f4e5  1489 db	52 */
0x0000f4e6, 0x00000035, /* 00000035 //C 0f4e6  1489 db	53 */
0x0000f4e7, 0x00000036, /* 00000036 //C 0f4e7  1489 db	54 */
0x0000f4e8, 0x00000037, /* 00000037 //C 0f4e8  1489 db	55 */
0x0000f4e9, 0x00000038, /* 00000038 //C 0f4e9  1489 db	56 */
0x0000f4ea, 0x00000039, /* 00000039 //C 0f4ea  1489 db	57 */
0x0000f4eb, 0x00000041, /* 00000041 //C 0f4eb  1489 db	65 */
0x0000f4ec, 0x00000042, /* 00000042 //C 0f4ec  1489 db	66 */
0x0000f4ed, 0x00000043, /* 00000043 //C 0f4ed  1489 db	67 */
0x0000f4ee, 0x00000044, /* 00000044 //C 0f4ee  1489 db	68 */
0x0000f4ef, 0x00000045, /* 00000045 //C 0f4ef  1489 db	69 */
0x0000f4f0, 0x00000046, /* 00000046 //C 0f4f0  1489 db	70 */
0x0000f4f1, 0x00000000, /* 00000000 //C 0f4f1  1489 db */
0x0000f4f2, 0x0ded0000, /* 0ded0000 //C 0f4f2  1492 push	lr */
0x0000f4f3, 0x0400f4db, /* 0400f4db //C 0f4f3  1493 call	value2Hexchar */
0x0000f4f4, 0x010d0020, /* 010d0020 //C 0f4f4  1494 or	r0,0x20 */
0x0000f4f5, 0x0ffd0000, /* 0ffd0000 //C 0f4f5  1495 pop	pc */
0x0000f4f6, 0x0ded0000, /* 0ded0000 //C 0f4f6  1504 push	lr */
0x0000f4f7, 0x0d2d0000, /* 0d2d0000 //C 0f4f7  1505 push	r2 */
0x0000f4f8, 0x0d3d0000, /* 0d3d0000 //C 0f4f8  1506 push	r3 */
0x0000f4f9, 0x0d4d0000, /* 0d4d0000 //C 0f4f9  1507 push	r4 */
0x0000f4fa, 0x0d5d0000, /* 0d5d0000 //C 0f4fa  1508 push	r5 */
0x0000f4fb, 0x01120000, /* 01120000 //C 0f4fb  1509 mvzl	r1,0		; return value */
0x0000f4fc, 0x01320000, /* 01320000 //C 0f4fc  1510 mvzl	r3,0		; index */
0x0000f4fd, 0x01520000, /* 01520000 //C 0f4fd  1512 mvzl	r5,0 */
0x0000f4fe, 0x0b438000, /* 0b438000 //C 0f4fe  1513 ld	r4,r3+,r0	; pick a char */
0x0000f4ff, 0x024b0000, /* 024b0000 //C 0f4ff  1514 sz	r4		; check eof string */
0x0000f500, 0x11f2f51c, /* 11f2f51c //C 0f500  1515 jz	htoi_eos */
0x0000f501, 0x06214405, /* 06214405 //C 0f501  1517 getbz	r2,r4,r5 */
0x0000f502, 0x022b0000, /* 022b0000 //C 0f502  1518 sz	r2 */
0x0000f503, 0x11f2f4fd, /* 11f2f4fd //C 0f503  1519 jz	htoi_cyc */
0x0000f504, 0x0128002e, /* 0128002e //C 0f504  1520 cmp	r2,'.'		; skip separators */
0x0000f505, 0x11f2f516, /* 11f2f516 //C 0f505  1521 jz	htoi_next */
0x0000f506, 0x0128005f, /* 0128005f //C 0f506  1522 cmp	r2,'_' */
0x0000f507, 0x11f2f516, /* 11f2f516 //C 0f507  1523 jz	htoi_next */
0x0000f508, 0x0128002d, /* 0128002d //C 0f508  1524 cmp	r2,'-' */
0x0000f509, 0x11f2f516, /* 11f2f516 //C 0f509  1525 jz	htoi_next */
0x0000f50a, 0x0d0d0000, /* 0d0d0000 //C 0f50a  1526 push	r0 */
0x0000f50b, 0x00000200, /* 00000200 //C 0f50b  1527 mov	r0,r2 */
0x0000f50c, 0x0400f4c3, /* 0400f4c3 //C 0f50c  1528 call	hexchar2value */
0x0000f50d, 0x00200000, /* 00200000 //C 0f50d  1529 mov	r2,r0 */
0x0000f50e, 0x0f0d0000, /* 0f0d0000 //C 0f50e  1530 pop	r0 */
0x0000f50f, 0x41f2f51a, /* 41f2f51a //C 0f50f  1531 C0 jmp	htoi_nok */
0x0000f510, 0x02180000, /* 02180000 //C 0f510  1532 shl	r1 */
0x0000f511, 0x02180000, /* 02180000 //C 0f511  1533 shl	r1 */
0x0000f512, 0x02180000, /* 02180000 //C 0f512  1534 shl	r1 */
0x0000f513, 0x02180000, /* 02180000 //C 0f513  1535 shl	r1 */
0x0000f514, 0x012f000f, /* 012f000f //C 0f514  1536 and	r2,0xf */
0x0000f515, 0x001d0200, /* 001d0200 //C 0f515  1537 or	r1,r2 */
0x0000f516, 0x01540001, /* 01540001 //C 0f516  1539 inc	r5 */
0x0000f517, 0x01580004, /* 01580004 //C 0f517  1540 cmp	r5,4 */
0x0000f518, 0x11f2f4fd, /* 11f2f4fd //C 0f518  1541 jz	htoi_cyc */
0x0000f519, 0x01f2f501, /* 01f2f501 //C 0f519  1542 jmp	htoi_byte */
0x0000f51a, 0x020d0000, /* 020d0000 //C 0f51a  1544 clc */
0x0000f51b, 0x01f2f51f, /* 01f2f51f //C 0f51b  1545 jmp	htoi_ret */
0x0000f51c, 0x01380001, /* 01380001 //C 0f51c  1547 cmp	r3,1 */
0x0000f51d, 0x920d0000, /* 920d0000 //C 0f51d  1548 HI clc */
0x0000f51e, 0xa20c0000, /* a20c0000 //C 0f51e  1549 LS sec */
0x0000f51f, 0x0f5d0000, /* 0f5d0000 //C 0f51f  1551 pop	r5 */
0x0000f520, 0x0f4d0000, /* 0f4d0000 //C 0f520  1552 pop	r4 */
0x0000f521, 0x0f3d0000, /* 0f3d0000 //C 0f521  1553 pop	r3 */
0x0000f522, 0x0f2d0000, /* 0f2d0000 //C 0f522  1554 pop	r2 */
0x0000f523, 0x0ffd0000, /* 0ffd0000 //C 0f523  1555 pop	pc */
0x0000f524, 0x0d0d0000, /* 0d0d0000 //C 0f524  1567 push	r0 */
0x0000f525, 0x0700ff42, /* 0700ff42 //C 0f525  1568 ld	r0,UART_RSTAT */
0x0000f526, 0x010c0001, /* 010c0001 //C 0f526  1570 test	r0,1		; check if queue is not empty */
0x0000f527, 0x020d0000, /* 020d0000 //C 0f527  1571 clc */
0x0000f528, 0x220c0000, /* 220c0000 //C 0f528  1572 Z0 sec */
0x0000f529, 0x0f0d0000, /* 0f0d0000 //C 0f529  1573 pop	r0 */
0x0000f52a, 0x30f00e00, /* 30f00e00 //C 0f52a  1574 C1 ret */
0x0000f52b, 0x00f00e00, /* 00f00e00 //C 0f52b  1575 ret */
0x0000f52c, 0x0700ff40, /* 0700ff40 //C 0f52c  1620 ld	r0,UART_DR */
0x0000f52d, 0x0600ff46, /* 0600ff46 //C 0f52d  1621 st	r0,UART_IRA */
0x0000f52e, 0x00f00e00, /* 00f00e00 //C 0f52e  1622 ret */
0x0000f52f, 0x0400f524, /* 0400f524 //C 0f52f  1654 call	check_uart */
0x0000f530, 0x41f2f52f, /* 41f2f52f //C 0f530  1655 C0 jmp	getchar */
0x0000f531, 0x0400f52c, /* 0400f52c //C 0f531  1656 call	read */
0x0000f532, 0x00f00e00, /* 00f00e00 //C 0f532  1657 ret */
0x0000f533, 0x0d9d0000, /* 0d9d0000 //C 0f533  1665 push	r9 */
0x0000f534, 0x0790ff43, /* 0790ff43 //C 0f534  1672 ld	r9,UART_TSTAT */
0x0000f535, 0x019c0001, /* 019c0001 //C 0f535  1673 test	r9,1 */
0x0000f536, 0x11f2f534, /* 11f2f534 //C 0f536  1674 jz	wait_tc */
0x0000f537, 0x0600ff40, /* 0600ff40 //C 0f537  1675 st	r0,UART_DR */
0x0000f538, 0x0f9d0000, /* 0f9d0000 //C 0f538  1677 pop	r9 */
0x0000f539, 0x00f00e00, /* 00f00e00 //C 0f539  1678 ret */
0x0000f53a, 0x0ded0000, /* 0ded0000 //C 0f53a  1686 push	lr */
0x0000f53b, 0x0d0d0000, /* 0d0d0000 //C 0f53b  1687 push	r0 */
0x0000f53c, 0x0d3d0000, /* 0d3d0000 //C 0f53c  1688 push	r3 */
0x0000f53d, 0x0d4d0000, /* 0d4d0000 //C 0f53d  1689 push	r4 */
0x0000f53e, 0x0d2d0000, /* 0d2d0000 //C 0f53e  1690 push	r2 */
0x0000f53f, 0x0d1d0000, /* 0d1d0000 //C 0f53f  1691 push	r1 */
0x0000f540, 0x0d5d0000, /* 0d5d0000 //C 0f540  1692 push	r5 */
0x0000f541, 0x01420000, /* 01420000 //C 0f541  1694 mvzl	r4,0 */
0x0000f542, 0x020b0000, /* 020b0000 //C 0f542  1695 sz	r0 */
0x0000f543, 0x1102f742, /* 1102f742 //C 0f543  1696 Z1 mvzl	r0,null_str */
0x0000f544, 0x00200000, /* 00200000 //C 0f544  1697 mov	r2,r0 */
0x0000f545, 0x0b148200, /* 0b148200 //C 0f545  1699 ld	r1,r4+,r2 */
0x0000f546, 0x021b0000, /* 021b0000 //C 0f546  1700 sz	r1 */
0x0000f547, 0x01520000, /* 01520000 //C 0f547  1701 mvzl	r5,0 */
0x0000f548, 0x11f2f551, /* 11f2f551 //C 0f548  1702 jz	prints_done */
0x0000f549, 0x06014105, /* 06014105 //C 0f549  1704 getbz	r0,r1,r5 */
0x0000f54a, 0x020b0000, /* 020b0000 //C 0f54a  1705 sz	r0 */
0x0000f54b, 0x2400f533, /* 2400f533 //C 0f54b  1706 NZ call	putchar */
0x0000f54c, 0x11f2f545, /* 11f2f545 //C 0f54c  1707 jz 	prints_go */
0x0000f54d, 0x01540001, /* 01540001 //C 0f54d  1708 inc	r5 */
0x0000f54e, 0x01580004, /* 01580004 //C 0f54e  1709 cmp	r5,4 */
0x0000f54f, 0x21f2f549, /* 21f2f549 //C 0f54f  1710 jnz	prints_byte */
0x0000f550, 0x01f2f545, /* 01f2f545 //C 0f550  1711 jmp	prints_go */
0x0000f551, 0x0f5d0000, /* 0f5d0000 //C 0f551  1714 pop	r5 */
0x0000f552, 0x0f1d0000, /* 0f1d0000 //C 0f552  1715 pop	r1 */
0x0000f553, 0x0f2d0000, /* 0f2d0000 //C 0f553  1716 pop	r2 */
0x0000f554, 0x0f4d0000, /* 0f4d0000 //C 0f554  1717 pop	r4 */
0x0000f555, 0x0f3d0000, /* 0f3d0000 //C 0f555  1718 pop	r3 */
0x0000f556, 0x0f0d0000, /* 0f0d0000 //C 0f556  1719 pop	r0 */
0x0000f557, 0x0ffd0000, /* 0ffd0000 //C 0f557  1720 pop	pc */
0x0000f558, 0x0d0d0000, /* 0d0d0000 //C 0f558  1732 push	r0 */
0x0000f559, 0x0d1d0000, /* 0d1d0000 //C 0f559  1733 push	r1 */
0x0000f55a, 0x0d2d0000, /* 0d2d0000 //C 0f55a  1734 push	r2 */
0x0000f55b, 0x0d3d0000, /* 0d3d0000 //C 0f55b  1735 push	r3 */
0x0000f55c, 0x00100e00, /* 00100e00 //C 0f55c  1736 mov	r1,lr */
0x0000f55d, 0x0e210000, /* 0e210000 //C 0f55d  1738 ld	r2,r1 */
0x0000f55e, 0x01140001, /* 01140001 //C 0f55e  1739 inc	r1 */
0x0000f55f, 0x022b0000, /* 022b0000 //C 0f55f  1740 sz	r2 */
0x0000f560, 0x11f2f56a, /* 11f2f56a //C 0f560  1741 jz	pes_done */
0x0000f561, 0x01320000, /* 01320000 //C 0f561  1742 mvzl	r3,0 */
0x0000f562, 0x06014203, /* 06014203 //C 0f562  1744 getbz	r0,r2,r3 */
0x0000f563, 0x020b0000, /* 020b0000 //C 0f563  1745 sz	r0 */
0x0000f564, 0x2400f533, /* 2400f533 //C 0f564  1746 NZ call	putchar */
0x0000f565, 0x11f2f55d, /* 11f2f55d //C 0f565  1747 jz	pes_next */
0x0000f566, 0x01340001, /* 01340001 //C 0f566  1748 inc	r3 */
0x0000f567, 0x01380004, /* 01380004 //C 0f567  1749 cmp	r3,4 */
0x0000f568, 0x21f2f562, /* 21f2f562 //C 0f568  1750 jnz	pes_byte */
0x0000f569, 0x01f2f55d, /* 01f2f55d //C 0f569  1751 jmp	pes_next */
0x0000f56a, 0x00e00100, /* 00e00100 //C 0f56a  1754 mov	LR,r1 */
0x0000f56b, 0x0f3d0000, /* 0f3d0000 //C 0f56b  1755 pop	r3 */
0x0000f56c, 0x0f2d0000, /* 0f2d0000 //C 0f56c  1756 pop	r2 */
0x0000f56d, 0x0f1d0000, /* 0f1d0000 //C 0f56d  1757 pop	r1 */
0x0000f56e, 0x0f0d0000, /* 0f0d0000 //C 0f56e  1758 pop	r0 */
0x0000f56f, 0x00f00e00, /* 00f00e00 //C 0f56f  1761 ret */
0x0000f570, 0x0ded0000, /* 0ded0000 //C 0f570  1769 push	lr */
0x0000f571, 0x0400f53a, /* 0400f53a //C 0f571  1770 call	prints */
0x0000f572, 0x0d0d0000, /* 0d0d0000 //C 0f572  1771 push	r0 */
0x0000f573, 0x0102000a, /* 0102000a //C 0f573  1772 mvzl	r0,LF */
0x0000f574, 0x0400f533, /* 0400f533 //C 0f574  1773 call	putchar */
0x0000f575, 0x0f0d0000, /* 0f0d0000 //C 0f575  1774 pop	r0 */
0x0000f576, 0x0ffd0000, /* 0ffd0000 //C 0f576  1775 pop	pc */
0x0000f577, 0x0ded0000, /* 0ded0000 //C 0f577  1783 push	lr */
0x0000f578, 0x0d0d0000, /* 0d0d0000 //C 0f578  1784 push	r0 */
0x0000f579, 0x0d1d0000, /* 0d1d0000 //C 0f579  1785 push	r1 */
0x0000f57a, 0x0d2d0000, /* 0d2d0000 //C 0f57a  1786 push	r2 */
0x0000f57b, 0x0d3d0000, /* 0d3d0000 //C 0f57b  1787 push	r3 */
0x0000f57c, 0x0d4d0000, /* 0d4d0000 //C 0f57c  1788 push	r4 */
0x0000f57d, 0x00300000, /* 00300000 //C 0f57d  1789 mov	r3,r0 */
0x0000f57e, 0x01220000, /* 01220000 //C 0f57e  1790 mvzl	r2,0 */
0x0000f57f, 0x01420001, /* 01420001 //C 0f57f  1791 mvzl	r4,1 */
0x0000f580, 0x01020000, /* 01020000 //C 0f580  1794 mvzl	r0,0 */
0x0000f581, 0x02380000, /* 02380000 //C 0f581  1795 shl	r3 */
0x0000f582, 0x02070000, /* 02070000 //C 0f582  1796 rol	r0 */
0x0000f583, 0x02380000, /* 02380000 //C 0f583  1797 shl	r3 */
0x0000f584, 0x02070000, /* 02070000 //C 0f584  1798 rol	r0 */
0x0000f585, 0x02380000, /* 02380000 //C 0f585  1799 shl	r3 */
0x0000f586, 0x02070000, /* 02070000 //C 0f586  1800 rol	r0 */
0x0000f587, 0x02380000, /* 02380000 //C 0f587  1801 shl	r3 */
0x0000f588, 0x02070000, /* 02070000 //C 0f588  1802 rol	r0 */
0x0000f589, 0x0400f4db, /* 0400f4db //C 0f589  1803 call	value2Hexchar */
0x0000f58a, 0x0400f533, /* 0400f533 //C 0f58a  1804 call	putchar */
0x0000f58b, 0x01240001, /* 01240001 //C 0f58b  1805 add	r2,1 */
0x0000f58c, 0x01280008, /* 01280008 //C 0f58c  1806 cmp	r2,8 */
0x0000f58d, 0x11f2f597, /* 11f2f597 //C 0f58d  1807 jz	print_vhex_ret */
0x0000f58e, 0x021b0000, /* 021b0000 //C 0f58e  1808 sz	r1 */
0x0000f58f, 0x11f2f595, /* 11f2f595 //C 0f58f  1809 jz	print_vhex_nosep */
0x0000f590, 0x00480100, /* 00480100 //C 0f590  1810 cmp	r4,r1 */
0x0000f591, 0x21f2f595, /* 21f2f595 //C 0f591  1811 jnz	print_vhex_nosep */
0x0000f592, 0x0102005f, /* 0102005f //C 0f592  1812 mvzl	r0,'_' */
0x0000f593, 0x0400f533, /* 0400f533 //C 0f593  1813 call	putchar */
0x0000f594, 0x01420000, /* 01420000 //C 0f594  1814 mvzl	r4,0 */
0x0000f595, 0x01440001, /* 01440001 //C 0f595  1816 add	r4,1 */
0x0000f596, 0x01f2f580, /* 01f2f580 //C 0f596  1817 jmp	print_vhex_cyc */
0x0000f597, 0x0f4d0000, /* 0f4d0000 //C 0f597  1819 pop	r4 */
0x0000f598, 0x0f3d0000, /* 0f3d0000 //C 0f598  1820 pop	r3 */
0x0000f599, 0x0f2d0000, /* 0f2d0000 //C 0f599  1821 pop	r2 */
0x0000f59a, 0x0f1d0000, /* 0f1d0000 //C 0f59a  1822 pop	r1 */
0x0000f59b, 0x0f0d0000, /* 0f0d0000 //C 0f59b  1823 pop	r0 */
0x0000f59c, 0x0ffd0000, /* 0ffd0000 //C 0f59c  1824 pop	pc */
0x0000f59d, 0x0ded0000, /* 0ded0000 //C 0f59d  1832 push	lr */
0x0000f59e, 0x0d4d0000, /* 0d4d0000 //C 0f59e  1833 push	r4 */
0x0000f59f, 0x021b0000, /* 021b0000 //C 0f59f  1835 sz	r1 */
0x0000f5a0, 0x21f2f5a4, /* 21f2f5a4 //C 0f5a0  1836 NZ jmp	div_dok */
0x0000f5a1, 0x00200000, /* 00200000 //C 0f5a1  1837 mov	r2,r0		; div by zero */
0x0000f5a2, 0x01320000, /* 01320000 //C 0f5a2  1838 mvzl	r3,0 */
0x0000f5a3, 0x01f2f5b5, /* 01f2f5b5 //C 0f5a3  1839 jmp	div_ret */
0x0000f5a4, 0x01220000, /* 01220000 //C 0f5a4  1841 mvzl	r2,0		; Q= 0 */
0x0000f5a5, 0x01320000, /* 01320000 //C 0f5a5  1842 mvzl	r3,0		; R= 0 */
0x0000f5a6, 0x01418000, /* 01418000 //C 0f5a6  1843 mvh	r4,0x80000000	; m= 1<<31 */
0x0000f5a7, 0x01400000, /* 01400000 //C 0f5a7  1844 mvl	r4,0x80000000 */
0x0000f5a8, 0x024b0000, /* 024b0000 //C 0f5a8  1846 sz	r4 */
0x0000f5a9, 0x11f2f5b5, /* 11f2f5b5 //C 0f5a9  1847 Z jmp	div_ret */
0x0000f5aa, 0x02380000, /* 02380000 //C 0f5aa  1848 shl	r3		; r<<= 1 */
0x0000f5ab, 0x000c0400, /* 000c0400 //C 0f5ab  1849 test	r0,r4		; if (n&m) */
0x0000f5ac, 0x213d0001, /* 213d0001 //C 0f5ac  1850 NZ or	r3,1		; r|= 1 */
0x0000f5ad, 0x00380100, /* 00380100 //C 0f5ad  1851 cmp	r3,r1		; if (r>=d) */
0x0000f5ae, 0x41f2f5b3, /* 41f2f5b3 //C 0f5ae  1852 LO jmp	div_cyc_next */
0x0000f5af, 0x00360100, /* 00360100 //C 0f5af  1853 sub	r3,r1		;r-= d */
0x0000f5b0, 0x002d0400, /* 002d0400 //C 0f5b0  1854 or	r2,r4		;q|= m */
0x0000f5b1, 0x01f2f5b3, /* 01f2f5b3 //C 0f5b1  1855 jmp	div_cyc_next */
0x0000f5b2, 0x01f2f5a8, /* 01f2f5a8 //C 0f5b2  1856 jmp	div_cyc */
0x0000f5b3, 0x02490000, /* 02490000 //C 0f5b3  1858 shr	r4		; m>>= 1 */
0x0000f5b4, 0x01f2f5a8, /* 01f2f5a8 //C 0f5b4  1859 jmp	div_cyc */
0x0000f5b5, 0x0f4d0000, /* 0f4d0000 //C 0f5b5  1861 pop	r4 */
0x0000f5b6, 0x0ffd0000, /* 0ffd0000 //C 0f5b6  1862 pop	pc */
0x0000f5b7, 0x0ded0000, /* 0ded0000 //C 0f5b7  1870 push	lr */
0x0000f5b8, 0x0d0d0000, /* 0d0d0000 //C 0f5b8  1871 push	r0 */
0x0000f5b9, 0x0d1d0000, /* 0d1d0000 //C 0f5b9  1872 push	r1 */
0x0000f5ba, 0x0d2d0000, /* 0d2d0000 //C 0f5ba  1873 push	r2 */
0x0000f5bb, 0x0d3d0000, /* 0d3d0000 //C 0f5bb  1874 push	r3 */
0x0000f5bc, 0x0dad0000, /* 0dad0000 //C 0f5bc  1875 push	r10 */
0x0000f5bd, 0x0dbd0000, /* 0dbd0000 //C 0f5bd  1876 push	r11 */
0x0000f5be, 0x0dcd0000, /* 0dcd0000 //C 0f5be  1877 push	r12 */
0x0000f5bf, 0x01c2f5df, /* 01c2f5df //C 0f5bf  1879 mvzl	r12,itoa_buffer	; pointer to output buffer */
0x0000f5c0, 0x01b2f5ea, /* 01b2f5ea //C 0f5c0  1880 mvzl	r11,itoa_divs	; pointer to dividers */
0x0000f5c1, 0x01a20000, /* 01a20000 //C 0f5c1  1881 mvzl	r10,0		; bool: first non-zero char found */
0x0000f5c2, 0x0e1b0000, /* 0e1b0000 //C 0f5c2  1883 ld	r1,r11		; get next divider */
0x0000f5c3, 0x021b0000, /* 021b0000 //C 0f5c3  1884 sz	r1		; if 0, then */
0x0000f5c4, 0x11f2f5d7, /* 11f2f5d7 //C 0f5c4  1885 jz	itoa_ret	; finish */
0x0000f5c5, 0x01180001, /* 01180001 //C 0f5c5  1886 cmp	r1,1		; last divider? */
0x0000f5c6, 0x11a20001, /* 11a20001 //C 0f5c6  1887 EQ mvzl	r10,1		; always print last char */
0x0000f5c7, 0x0400f59d, /* 0400f59d //C 0f5c7  1888 call	div		; R2,R3= R0/R1 */
0x0000f5c8, 0x022b0000, /* 022b0000 //C 0f5c8  1889 sz	r2		; is the result zero? */
0x0000f5c9, 0x11f2f5d4, /* 11f2f5d4 //C 0f5c9  1890 jz	itoa_f0 */
0x0000f5ca, 0x01a20001, /* 01a20001 //C 0f5ca  1892 mvzl	r10,1		; non-zero: start to print */
0x0000f5cb, 0x00000200, /* 00000200 //C 0f5cb  1894 mov	r0,r2		; convert result to ASCII char */
0x0000f5cc, 0x0400f4f2, /* 0400f4f2 //C 0f5cc  1895 call	value2hexchar */
0x0000f5cd, 0x0c0c0000, /* 0c0c0000 //C 0f5cd  1896 st	r0,r12		; and store it in buffer */
0x0000f5ce, 0x01c40001, /* 01c40001 //C 0f5ce  1897 inc	r12		; inc buf ptr */
0x0000f5cf, 0x01020000, /* 01020000 //C 0f5cf  1898 mvzl	r0,0		; put 0 after last char */
0x0000f5d0, 0x0c0c0000, /* 0c0c0000 //C 0f5d0  1899 st	r0,r12 */
0x0000f5d1, 0x00000300, /* 00000300 //C 0f5d1  1901 mov	r0,r3		; continue with the reminder */
0x0000f5d2, 0x01b40001, /* 01b40001 //C 0f5d2  1902 inc	r11		; and next divider */
0x0000f5d3, 0x01f2f5c2, /* 01f2f5c2 //C 0f5d3  1903 jmp	itoa_cyc */
0x0000f5d4, 0x02ab0000, /* 02ab0000 //C 0f5d4  1905 sz	r10		; just zeros so far? */
0x0000f5d5, 0x21f2f5cb, /* 21f2f5cb //C 0f5d5  1906 jnz	itoa_store	; no, print */
0x0000f5d6, 0x01f2f5d1, /* 01f2f5d1 //C 0f5d6  1907 jmp	itoa_next */
0x0000f5d7, 0x0fcd0000, /* 0fcd0000 //C 0f5d7  1909 pop	r12 */
0x0000f5d8, 0x0fbd0000, /* 0fbd0000 //C 0f5d8  1910 pop	r11 */
0x0000f5d9, 0x0fad0000, /* 0fad0000 //C 0f5d9  1911 pop	r10 */
0x0000f5da, 0x0f3d0000, /* 0f3d0000 //C 0f5da  1912 pop	r3 */
0x0000f5db, 0x0f2d0000, /* 0f2d0000 //C 0f5db  1913 pop	r2 */
0x0000f5dc, 0x0f1d0000, /* 0f1d0000 //C 0f5dc  1914 pop	r1 */
0x0000f5dd, 0x0f0d0000, /* 0f0d0000 //C 0f5dd  1915 pop	r0 */
0x0000f5de, 0x0ffd0000, /* 0ffd0000 //C 0f5de  1916 pop	pc */
0x0000f5df, 0x00000000, /* 00000000 //C 0f5df  1918 itoa_buffer:	ds	11 */
0x0000f5ea, 0x3b9aca00, /* 3b9aca00 //C 0f5ea  1920 dd	1000000000 */
0x0000f5eb, 0x05f5e100, /* 05f5e100 //C 0f5eb  1921 dd	100000000 */
0x0000f5ec, 0x00989680, /* 00989680 //C 0f5ec  1922 dd	10000000 */
0x0000f5ed, 0x000f4240, /* 000f4240 //C 0f5ed  1923 dd	1000000 */
0x0000f5ee, 0x000186a0, /* 000186a0 //C 0f5ee  1924 dd	100000 */
0x0000f5ef, 0x00002710, /* 00002710 //C 0f5ef  1925 dd	10000 */
0x0000f5f0, 0x000003e8, /* 000003e8 //C 0f5f0  1926 dd	1000 */
0x0000f5f1, 0x00000064, /* 00000064 //C 0f5f1  1927 dd	100 */
0x0000f5f2, 0x0000000a, /* 0000000a //C 0f5f2  1928 dd	10 */
0x0000f5f3, 0x00000001, /* 00000001 //C 0f5f3  1929 dd	1 */
0x0000f5f4, 0x00000000, /* 00000000 //C 0f5f4  1930 dd	0 */
0x0000f5f5, 0x0ded0000, /* 0ded0000 //C 0f5f5  1937 push	lr */
0x0000f5f6, 0x0d1d0000, /* 0d1d0000 //C 0f5f6  1938 push	r1 */
0x0000f5f7, 0x0d2d0000, /* 0d2d0000 //C 0f5f7  1939 push	r2 */
0x0000f5f8, 0x0108e0ff, /* 0108e0ff //C 0f5f8  1940 cmp	r0,99999999 */
0x0000f5f9, 0x91f2f608, /* 91f2f608 //C 0f5f9  1941 UGT jmp	itobcd_bad */
0x0000f5fa, 0x0400f5b7, /* 0400f5b7 //C 0f5fa  1942 call	itoa */
0x0000f5fb, 0x01020000, /* 01020000 //C 0f5fb  1943 mvzl	r0,0 */
0x0000f5fc, 0x0112f5df, /* 0112f5df //C 0f5fc  1944 mvzl	r1,itoa_buffer */
0x0000f5fd, 0x0e210000, /* 0e210000 //C 0f5fd  1946 ld	r2,r1 */
0x0000f5fe, 0x022b0000, /* 022b0000 //C 0f5fe  1947 sz	r2 */
0x0000f5ff, 0x11f2f609, /* 11f2f609 //C 0f5ff  1948 jz	itobcd_ret */
0x0000f600, 0x01260030, /* 01260030 //C 0f600  1949 sub	r2,'0' */
0x0000f601, 0x02080000, /* 02080000 //C 0f601  1950 shl	r0 */
0x0000f602, 0x02080000, /* 02080000 //C 0f602  1951 shl	r0 */
0x0000f603, 0x02080000, /* 02080000 //C 0f603  1952 shl	r0 */
0x0000f604, 0x02080000, /* 02080000 //C 0f604  1953 shl	r0 */
0x0000f605, 0x000d0200, /* 000d0200 //C 0f605  1954 or	r0,r2 */
0x0000f606, 0x01140001, /* 01140001 //C 0f606  1955 inc	r1 */
0x0000f607, 0x01f2f5fd, /* 01f2f5fd //C 0f607  1956 jmp	itobcd_cyc */
0x0000f608, 0x01020000, /* 01020000 //C 0f608  1958 mvzl	r0,0 */
0x0000f609, 0x0f2d0000, /* 0f2d0000 //C 0f609  1960 pop	r2 */
0x0000f60a, 0x0f1d0000, /* 0f1d0000 //C 0f60a  1961 pop	r1 */
0x0000f60b, 0x0ffd0000, /* 0ffd0000 //C 0f60b  1962 pop	pc */
0x0000f60c, 0x0ded0000, /* 0ded0000 //C 0f60c  1969 push	lr */
0x0000f60d, 0x0400f5b7, /* 0400f5b7 //C 0f60d  1970 call	itoa */
0x0000f60e, 0x0102f5df, /* 0102f5df //C 0f60e  1971 mvzl	r0,itoa_buffer */
0x0000f60f, 0x0400f53a, /* 0400f53a //C 0f60f  1972 call	prints */
0x0000f610, 0x0ffd0000, /* 0ffd0000 //C 0f610  1973 pop	pc */
0x0000f611, 0x0ded0000, /* 0ded0000 //C 0f611  1982 push	lr */
0x0000f612, 0x0d0d0000, /* 0d0d0000 //C 0f612  1983 push	r0 */
0x0000f613, 0x0d1d0000, /* 0d1d0000 //C 0f613  1984 push	r1 */
0x0000f614, 0x0d3d0000, /* 0d3d0000 //C 0f614  1985 push	r3 */
0x0000f615, 0x0610f6f8, /* 0610f6f8 //C 0f615  1987 st	r1,reg1 */
0x0000f616, 0x0620f6f9, /* 0620f6f9 //C 0f616  1988 st	r2,reg2 */
0x0000f617, 0x0630f6fa, /* 0630f6fa //C 0f617  1989 st	r3,reg3 */
0x0000f618, 0x0640f6fb, /* 0640f6fb //C 0f618  1990 st	r4,reg4 */
0x0000f619, 0x0650f6fc, /* 0650f6fc //C 0f619  1991 st	r5,reg5 */
0x0000f61a, 0x0660f6fd, /* 0660f6fd //C 0f61a  1992 st	r6,reg6 */
0x0000f61b, 0x0670f6fe, /* 0670f6fe //C 0f61b  1993 st	r7,reg7 */
0x0000f61c, 0x0680f6ff, /* 0680f6ff //C 0f61c  1994 st	r8,reg8 */
0x0000f61d, 0x0690f700, /* 0690f700 //C 0f61d  1995 st	r9,reg9 */
0x0000f61e, 0x06a0f701, /* 06a0f701 //C 0f61e  1996 st	r10,reg10 */
0x0000f61f, 0x06b0f702, /* 06b0f702 //C 0f61f  1997 st	r11,reg11 */
0x0000f620, 0x06c0f703, /* 06c0f703 //C 0f620  1998 st	r12,reg12 */
0x0000f621, 0x00200000, /* 00200000 //C 0f621  2000 mov	r2,r0		; pointer to format string */
0x0000f622, 0x0112f6f8, /* 0112f6f8 //C 0f622  2001 mvzl	r1,reg1		; pointer to params */
0x0000f623, 0x01320000, /* 01320000 //C 0f623  2002 mvzl	r3,0		; byte idx in packed str */
0x0000f624, 0x0e020000, /* 0e020000 //C 0f624  2004 ld	r0,r2		; get next char */
0x0000f625, 0x020b0000, /* 020b0000 //C 0f625  2005 sz	r0		; is it EOS? */
0x0000f626, 0x11f2f660, /* 11f2f660 //C 0f626  2006 jz	printf_ret */
0x0000f627, 0x06014003, /* 06014003 //C 0f627  2007 getbz	r0,r0,r3	; pick next byte */
0x0000f628, 0x020b0000, /* 020b0000 //C 0f628  2008 sz	r0		; is it null? */
0x0000f629, 0x11f2f65d, /* 11f2f65d //C 0f629  2009 jz	printf_nextword	; no more non-nulls */
0x0000f62a, 0x01f2f62b, /* 01f2f62b //C 0f62a  2013 jmp	printf_notescape */
0x0000f62b, 0x01080025, /* 01080025 //C 0f62b  2063 cmp	r0,'%'		; is it a format char? */
0x0000f62c, 0x21f2f659, /* 21f2f659 //C 0f62c  2064 jnz	printf_print */
0x0000f62d, 0x01340001, /* 01340001 //C 0f62d  2066 inc	r3 */
0x0000f62e, 0x01380004, /* 01380004 //C 0f62e  2067 cmp	r3,4 */
0x0000f62f, 0x21f2f635, /* 21f2f635 //C 0f62f  2068 jnz	printf_l3 */
0x0000f630, 0x01320000, /* 01320000 //C 0f630  2070 mvzl	r3,0 */
0x0000f631, 0x01240001, /* 01240001 //C 0f631  2071 inc	r2		; go to format char */
0x0000f632, 0x0e020000, /* 0e020000 //C 0f632  2072 ld	r0,r2 */
0x0000f633, 0x022b0000, /* 022b0000 //C 0f633  2073 sz	r2		; is it EOS? */
0x0000f634, 0x11f2f660, /* 11f2f660 //C 0f634  2074 jz	printf_ret */
0x0000f635, 0x0e020000, /* 0e020000 //C 0f635  2076 ld	r0,r2 */
0x0000f636, 0x06014003, /* 06014003 //C 0f636  2077 getbz	r0,r0,r3 */
0x0000f637, 0x020b0000, /* 020b0000 //C 0f637  2078 sz	r0 */
0x0000f638, 0x11f2f630, /* 11f2f630 //C 0f638  2079 jz	printf_l4 */
0x0000f639, 0x01080025, /* 01080025 //C 0f639  2081 cmp	r0,'%'		; % is used to print % */
0x0000f63a, 0x11f2f659, /* 11f2f659 //C 0f63a  2082 jz	printf_print */
0x0000f63b, 0x01080075, /* 01080075 //C 0f63b  2084 cmp	r0,'u'		; u,d print in decimal */
0x0000f63c, 0x11f2f63f, /* 11f2f63f //C 0f63c  2085 jz	printf_d */
0x0000f63d, 0x01080064, /* 01080064 //C 0f63d  2086 cmp	r0,'d' */
0x0000f63e, 0x21f2f643, /* 21f2f643 //C 0f63e  2087 jnz	printf_notd */
0x0000f63f, 0x0e010000, /* 0e010000 //C 0f63f  2089 ld	r0,r1 */
0x0000f640, 0x01140001, /* 01140001 //C 0f640  2090 inc	r1 */
0x0000f641, 0x0400f60c, /* 0400f60c //C 0f641  2091 call	printd */
0x0000f642, 0x01f2f65a, /* 01f2f65a //C 0f642  2092 jmp	printf_next */
0x0000f643, 0x01080078, /* 01080078 //C 0f643  2095 cmp	r0,'x' */
0x0000f644, 0x21f2f64c, /* 21f2f64c //C 0f644  2096 jnz	printf_notx */
0x0000f645, 0x0e010000, /* 0e010000 //C 0f645  2098 ld	r0,r1 */
0x0000f646, 0x01140001, /* 01140001 //C 0f646  2099 inc	r1 */
0x0000f647, 0x0d1d0000, /* 0d1d0000 //C 0f647  2100 push	r1 */
0x0000f648, 0x01120000, /* 01120000 //C 0f648  2101 mvzl	r1,0 */
0x0000f649, 0x0400f577, /* 0400f577 //C 0f649  2104 call	print_vhex */
0x0000f64a, 0x0f1d0000, /* 0f1d0000 //C 0f64a  2105 pop	r1 */
0x0000f64b, 0x01f2f65a, /* 01f2f65a //C 0f64b  2106 jmp	printf_next */
0x0000f64c, 0x01080073, /* 01080073 //C 0f64c  2109 cmp	r0,'s' */
0x0000f64d, 0x21f2f652, /* 21f2f652 //C 0f64d  2110 jnz	printf_nots */
0x0000f64e, 0x0e010000, /* 0e010000 //C 0f64e  2112 ld	r0,r1 */
0x0000f64f, 0x01140001, /* 01140001 //C 0f64f  2113 inc	r1 */
0x0000f650, 0x0400f53a, /* 0400f53a //C 0f650  2114 call	prints */
0x0000f651, 0x01f2f65a, /* 01f2f65a //C 0f651  2115 jmp	printf_next */
0x0000f652, 0x01080063, /* 01080063 //C 0f652  2118 cmp	r0,'c' */
0x0000f653, 0x21f2f658, /* 21f2f658 //C 0f653  2119 jnz	printf_notc */
0x0000f654, 0x0e010000, /* 0e010000 //C 0f654  2120 ld	r0,r1 */
0x0000f655, 0x01140001, /* 01140001 //C 0f655  2121 inc	r1 */
0x0000f656, 0x0400f533, /* 0400f533 //C 0f656  2122 call	putchar */
0x0000f657, 0x01f2f65a, /* 01f2f65a //C 0f657  2123 jmp	printf_next */
0x0000f658, 0x01f2f65a, /* 01f2f65a //C 0f658  2126 jmp	printf_next */
0x0000f659, 0x0400f533, /* 0400f533 //C 0f659  2128 call	putchar		; print actual char */
0x0000f65a, 0x01340001, /* 01340001 //C 0f65a  2130 inc	r3		; next byte in word */
0x0000f65b, 0x01380004, /* 01380004 //C 0f65b  2131 cmp	r3,4		; all 4 processed? */
0x0000f65c, 0x21f2f624, /* 21f2f624 //C 0f65c  2132 jnz	printf_cyc */
0x0000f65d, 0x01240001, /* 01240001 //C 0f65d  2134 inc	r2		; inc string ptr */
0x0000f65e, 0x01320000, /* 01320000 //C 0f65e  2135 mvzl	r3,0		; restart byte idx */
0x0000f65f, 0x01f2f624, /* 01f2f624 //C 0f65f  2136 jmp	printf_cyc */
0x0000f660, 0x07c0f703, /* 07c0f703 //C 0f660  2139 ld	r12,reg12 */
0x0000f661, 0x07b0f702, /* 07b0f702 //C 0f661  2140 ld	r11,reg11 */
0x0000f662, 0x07a0f701, /* 07a0f701 //C 0f662  2141 ld	r10,reg10 */
0x0000f663, 0x0790f700, /* 0790f700 //C 0f663  2142 ld	r9,reg9 */
0x0000f664, 0x0780f6ff, /* 0780f6ff //C 0f664  2143 ld	r8,reg8 */
0x0000f665, 0x0770f6fe, /* 0770f6fe //C 0f665  2144 ld	r7,reg7 */
0x0000f666, 0x0760f6fd, /* 0760f6fd //C 0f666  2145 ld	r6,reg6 */
0x0000f667, 0x0750f6fc, /* 0750f6fc //C 0f667  2146 ld	r5,reg5 */
0x0000f668, 0x0740f6fb, /* 0740f6fb //C 0f668  2147 ld	r4,reg4 */
0x0000f669, 0x0f3d0000, /* 0f3d0000 //C 0f669  2149 pop	r3 */
0x0000f66a, 0x0f1d0000, /* 0f1d0000 //C 0f66a  2150 pop	r1 */
0x0000f66b, 0x0f0d0000, /* 0f0d0000 //C 0f66b  2151 pop	r0 */
0x0000f66c, 0x0ffd0000, /* 0ffd0000 //C 0f66c  2152 pop	pc */
0x0000f66d, 0x0d0d0000, /* 0d0d0000 //C 0f66d  2157 push	r0 */
0x0000f66e, 0x0d2d0000, /* 0d2d0000 //C 0f66e  2158 push	r2 */
0x0000f66f, 0x00000e00, /* 00000e00 //C 0f66f  2159 mov	r0,LR */
0x0000f670, 0x0400f611, /* 0400f611 //C 0f670  2160 call	printf */
0x0000f671, 0x01240001, /* 01240001 //C 0f671  2161 inc	r2 */
0x0000f672, 0x00e00200, /* 00e00200 //C 0f672  2162 mov	LR,r2 */
0x0000f673, 0x0f2d0000, /* 0f2d0000 //C 0f673  2163 pop	r2 */
0x0000f674, 0x0f0d0000, /* 0f0d0000 //C 0f674  2164 pop	r0 */
0x0000f675, 0x00f00e00, /* 00f00e00 //C 0f675  2165 ret */
0x0000f676, 0x0d1d0000, /* 0d1d0000 //C 0f676  2169 push	r1 */
0x0000f677, 0x01020004, /* 01020004 //C 0f677  2170 mvzl	r0,version_sub */
0x0000f678, 0x01120001, /* 01120001 //C 0f678  2171 mvzl	r1,version_main */
0x0000f679, 0x07018101, /* 07018101 //C 0f679  2172 putb	r0,r1,1 */
0x0000f67a, 0x0f1d0000, /* 0f1d0000 //C 0f67a  2173 pop	r1 */
0x0000f67b, 0x00f00e00, /* 00f00e00 //C 0f67b  2174 ret */
0x0000f67c, 0x01420000, /* 01420000 //C 0f67c  2180 mvzl	r4,0 */
0x0000f67d, 0x0108007f, /* 0108007f //C 0f67d  2181 cmp	r0,0x7f */
0x0000f67e, 0x90f00e00, /* 90f00e00 //C 0f67e  2182 UGT ret */
0x0000f67f, 0x0d0d0000, /* 0d0d0000 //C 0f67f  2184 push	r0 */
0x0000f680, 0x0d1d0000, /* 0d1d0000 //C 0f680  2185 push	r1 */
0x0000f681, 0x02090000, /* 02090000 //C 0f681  2187 shr	r0 */
0x0000f682, 0x02090000, /* 02090000 //C 0f682  2188 shr	r0 */
0x0000f683, 0x0112f950, /* 0112f950 //C 0f683  2189 mvzl	r1,ascii2seg_table */
0x0000f684, 0x0a400100, /* 0a400100 //C 0f684  2190 ld	r4,r0,r1 */
0x0000f685, 0x0f1d0000, /* 0f1d0000 //C 0f685  2191 pop	r1 */
0x0000f686, 0x0f0d0000, /* 0f0d0000 //C 0f686  2192 pop	r0 */
0x0000f687, 0x06414400, /* 06414400 //C 0f687  2193 getbz	r4,r4,r0 */
0x0000f688, 0x00f00e00, /* 00f00e00 //C 0f688  2194 ret */
0x0000f689, 0x00000000, /* 00000000 //C 0f689  2199 line:		ds	100		; line buffer */
0x0000f6ed, 0x00000000, /* 00000000 //C 0f6ed  2200 line_ptr:	ds	1		; line pointer (index) */
0x0000f6ee, 0x00000000, /* 00000000 //C 0f6ee  2201 at_eol:		ds	1		; bool, true if EOL arrived */
0x0000f6ef, 0x00000000, /* 00000000 //C 0f6ef  2202 words:		ds	5		; Tokens of line */
0x0000f6f4, 0x00000000, /* 00000000 //C 0f6f4  2203 echo:		ds	1		; bool, do echo or not */
0x0000f6f5, 0x00000000, /* 00000000 //C 0f6f5  2204 dd	0 */
0x0000f6f6, 0x00000000, /* 00000000 //C 0f6f6  2205 dd	0 */
0x0000f6f7, 0x00000000, /* 00000000 //C 0f6f7  2207 dd	0 */
0x0000f6f8, 0x00000000, /* 00000000 //C 0f6f8  2208 dd	0 */
0x0000f6f9, 0x00000000, /* 00000000 //C 0f6f9  2209 dd	0 */
0x0000f6fa, 0x00000000, /* 00000000 //C 0f6fa  2210 dd	0 */
0x0000f6fb, 0x00000000, /* 00000000 //C 0f6fb  2211 dd	0 */
0x0000f6fc, 0x00000000, /* 00000000 //C 0f6fc  2212 dd	0 */
0x0000f6fd, 0x00000000, /* 00000000 //C 0f6fd  2213 dd	0 */
0x0000f6fe, 0x00000000, /* 00000000 //C 0f6fe  2214 dd	0 */
0x0000f6ff, 0x00000000, /* 00000000 //C 0f6ff  2215 dd	0 */
0x0000f700, 0x00000000, /* 00000000 //C 0f700  2216 dd	0 */
0x0000f701, 0x00000000, /* 00000000 //C 0f701  2217 dd	0 */
0x0000f702, 0x00000000, /* 00000000 //C 0f702  2218 dd	0 */
0x0000f703, 0x00000000, /* 00000000 //C 0f703  2219 dd	0 */
0x0000f704, 0x00000000, /* 00000000 //C 0f704  2220 dd	0 */
0x0000f705, 0x00000000, /* 00000000 //C 0f705  2221 dd	0 */
0x0000f706, 0x00000000, /* 00000000 //C 0f706  2222 dd	0 */
0x0000f707, 0x00000000, /* 00000000 //C 0f707  2223 dd	0 */
0x0000f708, 0x00000050, /* 00000050 //C 0f708  2225 db	80 */
0x0000f709, 0x0000004d, /* 0000004d //C 0f709  2225 db	77 */
0x0000f70a, 0x0000006f, /* 0000006f //C 0f70a  2225 db	111 */
0x0000f70b, 0x0000006e, /* 0000006e //C 0f70b  2225 db	110 */
0x0000f70c, 0x00000069, /* 00000069 //C 0f70c  2225 db	105 */
0x0000f70d, 0x00000074, /* 00000074 //C 0f70d  2225 db	116 */
0x0000f70e, 0x0000006f, /* 0000006f //C 0f70e  2225 db	111 */
0x0000f70f, 0x00000072, /* 00000072 //C 0f70f  2225 db	114 */
0x0000f710, 0x00000020, /* 00000020 //C 0f710  2225 db	32 */
0x0000f711, 0x00000076, /* 00000076 //C 0f711  2225 db	118 */
0x0000f712, 0x00000025, /* 00000025 //C 0f712  2225 db	37 */
0x0000f713, 0x00000064, /* 00000064 //C 0f713  2225 db	100 */
0x0000f714, 0x0000002e, /* 0000002e //C 0f714  2225 db	46 */
0x0000f715, 0x00000025, /* 00000025 //C 0f715  2225 db	37 */
0x0000f716, 0x00000064, /* 00000064 //C 0f716  2225 db	100 */
0x0000f717, 0x00000020, /* 00000020 //C 0f717  2225 db	32 */
0x0000f718, 0x00000028, /* 00000028 //C 0f718  2225 db	40 */
0x0000f719, 0x00000063, /* 00000063 //C 0f719  2225 db	99 */
0x0000f71a, 0x00000070, /* 00000070 //C 0f71a  2225 db	112 */
0x0000f71b, 0x00000075, /* 00000075 //C 0f71b  2225 db	117 */
0x0000f71c, 0x00000020, /* 00000020 //C 0f71c  2225 db	32 */
0x0000f71d, 0x00000076, /* 00000076 //C 0f71d  2225 db	118 */
0x0000f71e, 0x00000025, /* 00000025 //C 0f71e  2225 db	37 */
0x0000f71f, 0x00000064, /* 00000064 //C 0f71f  2225 db	100 */
0x0000f720, 0x0000002e, /* 0000002e //C 0f720  2225 db	46 */
0x0000f721, 0x00000025, /* 00000025 //C 0f721  2225 db	37 */
0x0000f722, 0x00000064, /* 00000064 //C 0f722  2225 db	100 */
0x0000f723, 0x0000002e, /* 0000002e //C 0f723  2225 db	46 */
0x0000f724, 0x00000025, /* 00000025 //C 0f724  2225 db	37 */
0x0000f725, 0x00000064, /* 00000064 //C 0f725  2225 db	100 */
0x0000f726, 0x00000029, /* 00000029 //C 0f726  2225 db	41 */
0x0000f727, 0x0000000a, /* 0000000a //C 0f727  2225 db	10 */
0x0000f728, 0x00000000, /* 00000000 //C 0f728  2225 db */
0x0000f729, 0x00000053, /* 00000053 //C 0f729  2226 db	83 */
0x0000f72a, 0x00000074, /* 00000074 //C 0f72a  2226 db	116 */
0x0000f72b, 0x0000006f, /* 0000006f //C 0f72b  2226 db	111 */
0x0000f72c, 0x00000070, /* 00000070 //C 0f72c  2226 db	112 */
0x0000f72d, 0x00000020, /* 00000020 //C 0f72d  2226 db	32 */
0x0000f72e, 0x00000061, /* 00000061 //C 0f72e  2226 db	97 */
0x0000f72f, 0x00000074, /* 00000074 //C 0f72f  2226 db	116 */
0x0000f730, 0x0000003a, /* 0000003a //C 0f730  2226 db	58 */
0x0000f731, 0x00000020, /* 00000020 //C 0f731  2226 db	32 */
0x0000f732, 0x00000000, /* 00000000 //C 0f732  2226 db */
0x0000f733, 0x00000008, /* 00000008 //C 0f733  2227 db	8 */
0x0000f734, 0x00000020, /* 00000020 //C 0f734  2227 db	32 */
0x0000f735, 0x00000008, /* 00000008 //C 0f735  2227 db	8 */
0x0000f736, 0x00000000, /* 00000000 //C 0f736  2227 db	0 */
0x0000f737, 0x0000003a, /* 0000003a //C 0f737  2228 db	58 */
0x0000f738, 0x00000000, /* 00000000 //C 0f738  2228 db */
0x0000f739, 0x00000020, /* 00000020 //C 0f739  2229 db	32 */
0x0000f73a, 0x0000003b, /* 0000003b //C 0f73a  2229 db	59 */
0x0000f73b, 0x00000009, /* 00000009 //C 0f73b  2229 db	9 */
0x0000f73c, 0x0000000b, /* 0000000b //C 0f73c  2229 db	11 */
0x0000f73d, 0x0000002c, /* 0000002c //C 0f73d  2229 db	44 */
0x0000f73e, 0x0000003d, /* 0000003d //C 0f73e  2229 db	61 */
0x0000f73f, 0x0000005b, /* 0000005b //C 0f73f  2229 db	91 */
0x0000f740, 0x0000005d, /* 0000005d //C 0f740  2229 db	93 */
0x0000f741, 0x00000000, /* 00000000 //C 0f741  2229 db */
0x0000f742, 0x00000028, /* 00000028 //C 0f742  2230 db	40 */
0x0000f743, 0x0000006e, /* 0000006e //C 0f743  2230 db	110 */
0x0000f744, 0x00000075, /* 00000075 //C 0f744  2230 db	117 */
0x0000f745, 0x0000006c, /* 0000006c //C 0f745  2230 db	108 */
0x0000f746, 0x0000006c, /* 0000006c //C 0f746  2230 db	108 */
0x0000f747, 0x00000029, /* 00000029 //C 0f747  2230 db	41 */
0x0000f748, 0x00000000, /* 00000000 //C 0f748  2230 db */
0x0000f749, 0x00000000, /* 00000000 //C 0f749  2231 db	0 */
0x0000f74a, 0x00000000, /* 00000000 //C 0f74a  2232 db	0 */
0x0000f74b, 0x00000000, /* 00000000 //C 0f74b  2233 sc_buffer:	ds	10 */
0x0000f755, 0x0000f1ab, /* 0000f1ab //C 0f755  2237 dd	cmd_sm */
0x0000f756, 0x00000073, /* 00000073 //C 0f756  2238 db	115 */
0x0000f757, 0x0000006d, /* 0000006d //C 0f757  2238 db	109 */
0x0000f758, 0x00000000, /* 00000000 //C 0f758  2238 db */
0x0000f759, 0x0000f152, /* 0000f152 //C 0f759  2239 dd	cmd_m */
0x0000f75a, 0x0000006d, /* 0000006d //C 0f75a  2240 db	109 */
0x0000f75b, 0x00000000, /* 00000000 //C 0f75b  2240 db */
0x0000f75c, 0x0000f152, /* 0000f152 //C 0f75c  2241 dd	cmd_m */
0x0000f75d, 0x0000006d, /* 0000006d //C 0f75d  2242 db	109 */
0x0000f75e, 0x00000065, /* 00000065 //C 0f75e  2242 db	101 */
0x0000f75f, 0x0000006d, /* 0000006d //C 0f75f  2242 db	109 */
0x0000f760, 0x00000000, /* 00000000 //C 0f760  2242 db */
0x0000f761, 0x0000f1ce, /* 0000f1ce //C 0f761  2243 dd	cmd_d */
0x0000f762, 0x00000064, /* 00000064 //C 0f762  2244 db	100 */
0x0000f763, 0x00000000, /* 00000000 //C 0f763  2244 db */
0x0000f764, 0x0000f1ce, /* 0000f1ce //C 0f764  2245 dd	cmd_d */
0x0000f765, 0x00000064, /* 00000064 //C 0f765  2246 db	100 */
0x0000f766, 0x00000075, /* 00000075 //C 0f766  2246 db	117 */
0x0000f767, 0x0000006d, /* 0000006d //C 0f767  2246 db	109 */
0x0000f768, 0x00000070, /* 00000070 //C 0f768  2246 db	112 */
0x0000f769, 0x00000000, /* 00000000 //C 0f769  2246 db */
0x0000f76a, 0x0000f201, /* 0000f201 //C 0f76a  2247 dd	cmd_l */
0x0000f76b, 0x0000006c, /* 0000006c //C 0f76b  2248 db	108 */
0x0000f76c, 0x00000000, /* 00000000 //C 0f76c  2248 db */
0x0000f76d, 0x0000f201, /* 0000f201 //C 0f76d  2249 dd	cmd_l */
0x0000f76e, 0x0000006c, /* 0000006c //C 0f76e  2250 db	108 */
0x0000f76f, 0x0000006f, /* 0000006f //C 0f76f  2250 db	111 */
0x0000f770, 0x00000061, /* 00000061 //C 0f770  2250 db	97 */
0x0000f771, 0x00000064, /* 00000064 //C 0f771  2250 db	100 */
0x0000f772, 0x00000000, /* 00000000 //C 0f772  2250 db */
0x0000f773, 0x0000f277, /* 0000f277 //C 0f773  2251 dd	cmd_g */
0x0000f774, 0x00000067, /* 00000067 //C 0f774  2252 db	103 */
0x0000f775, 0x00000000, /* 00000000 //C 0f775  2252 db */
0x0000f776, 0x0000f277, /* 0000f277 //C 0f776  2253 dd	cmd_g */
0x0000f777, 0x00000067, /* 00000067 //C 0f777  2254 db	103 */
0x0000f778, 0x0000006f, /* 0000006f //C 0f778  2254 db	111 */
0x0000f779, 0x00000000, /* 00000000 //C 0f779  2254 db */
0x0000f77a, 0x0000f277, /* 0000f277 //C 0f77a  2255 dd	cmd_g */
0x0000f77b, 0x00000072, /* 00000072 //C 0f77b  2256 db	114 */
0x0000f77c, 0x00000075, /* 00000075 //C 0f77c  2256 db	117 */
0x0000f77d, 0x0000006e, /* 0000006e //C 0f77d  2256 db	110 */
0x0000f77e, 0x00000000, /* 00000000 //C 0f77e  2256 db */
0x0000f77f, 0x0000f2b6, /* 0000f2b6 //C 0f77f  2257 dd	cmd_h */
0x0000f780, 0x0000003f, /* 0000003f //C 0f780  2258 db	63 */
0x0000f781, 0x00000000, /* 00000000 //C 0f781  2258 db */
0x0000f782, 0x0000f2b6, /* 0000f2b6 //C 0f782  2259 dd	cmd_h */
0x0000f783, 0x00000068, /* 00000068 //C 0f783  2260 db	104 */
0x0000f784, 0x00000000, /* 00000000 //C 0f784  2260 db */
0x0000f785, 0x0000f2b6, /* 0000f2b6 //C 0f785  2261 dd	cmd_h */
0x0000f786, 0x00000068, /* 00000068 //C 0f786  2262 db	104 */
0x0000f787, 0x00000065, /* 00000065 //C 0f787  2262 db	101 */
0x0000f788, 0x0000006c, /* 0000006c //C 0f788  2262 db	108 */
0x0000f789, 0x00000070, /* 00000070 //C 0f789  2262 db	112 */
0x0000f78a, 0x00000000, /* 00000000 //C 0f78a  2262 db */
0x0000f78b, 0x0000f31d, /* 0000f31d //C 0f78b  2263 dd	cmd_r */
0x0000f78c, 0x00000072, /* 00000072 //C 0f78c  2264 db	114 */
0x0000f78d, 0x00000000, /* 00000000 //C 0f78d  2264 db */
0x0000f78e, 0x0000f31d, /* 0000f31d //C 0f78e  2265 dd	cmd_r */
0x0000f78f, 0x00000072, /* 00000072 //C 0f78f  2266 db	114 */
0x0000f790, 0x00000065, /* 00000065 //C 0f790  2266 db	101 */
0x0000f791, 0x00000067, /* 00000067 //C 0f791  2266 db	103 */
0x0000f792, 0x00000000, /* 00000000 //C 0f792  2266 db */
0x0000f793, 0x0000f31d, /* 0000f31d //C 0f793  2267 dd	cmd_r */
0x0000f794, 0x00000072, /* 00000072 //C 0f794  2268 db	114 */
0x0000f795, 0x00000065, /* 00000065 //C 0f795  2268 db	101 */
0x0000f796, 0x00000067, /* 00000067 //C 0f796  2268 db	103 */
0x0000f797, 0x00000073, /* 00000073 //C 0f797  2268 db	115 */
0x0000f798, 0x00000000, /* 00000000 //C 0f798  2268 db */
0x0000f799, 0x0000f3ad, /* 0000f3ad //C 0f799  2269 dd	cmd_sp */
0x0000f79a, 0x00000073, /* 00000073 //C 0f79a  2270 db	115 */
0x0000f79b, 0x00000070, /* 00000070 //C 0f79b  2270 db	112 */
0x0000f79c, 0x00000000, /* 00000000 //C 0f79c  2270 db */
0x0000f79d, 0x0000f3b0, /* 0000f3b0 //C 0f79d  2271 dd	cmd_lr */
0x0000f79e, 0x0000006c, /* 0000006c //C 0f79e  2272 db	108 */
0x0000f79f, 0x00000072, /* 00000072 //C 0f79f  2272 db	114 */
0x0000f7a0, 0x00000000, /* 00000000 //C 0f7a0  2272 db */
0x0000f7a1, 0x0000f3b3, /* 0000f3b3 //C 0f7a1  2273 dd	cmd_pc */
0x0000f7a2, 0x00000070, /* 00000070 //C 0f7a2  2274 db	112 */
0x0000f7a3, 0x00000063, /* 00000063 //C 0f7a3  2274 db	99 */
0x0000f7a4, 0x00000000, /* 00000000 //C 0f7a4  2274 db */
0x0000f7a5, 0x0000f3b6, /* 0000f3b6 //C 0f7a5  2275 dd	cmd_f */
0x0000f7a6, 0x00000066, /* 00000066 //C 0f7a6  2276 db	102 */
0x0000f7a7, 0x00000000, /* 00000000 //C 0f7a7  2276 db */
0x0000f7a8, 0x0000f3b9, /* 0000f3b9 //C 0f7a8  2277 dd	cmd_v */
0x0000f7a9, 0x00000076, /* 00000076 //C 0f7a9  2278 db	118 */
0x0000f7aa, 0x00000065, /* 00000065 //C 0f7aa  2278 db	101 */
0x0000f7ab, 0x00000072, /* 00000072 //C 0f7ab  2278 db	114 */
0x0000f7ac, 0x00000000, /* 00000000 //C 0f7ac  2278 db */
0x0000f7ad, 0x0000f3b9, /* 0000f3b9 //C 0f7ad  2279 dd	cmd_v */
0x0000f7ae, 0x00000076, /* 00000076 //C 0f7ae  2280 db	118 */
0x0000f7af, 0x00000000, /* 00000000 //C 0f7af  2280 db */
0x0000f7b0, 0x00000000, /* 00000000 //C 0f7b0  2281 dd	0 */
0x0000f7b1, 0x00000000, /* 00000000 //C 0f7b1  2282 dd	0 */
0x0000f7b2, 0x0000006d, /* 0000006d //C 0f7b2  2284 db	109 */
0x0000f7b3, 0x0000005b, /* 0000005b //C 0f7b3  2284 db	91 */
0x0000f7b4, 0x00000065, /* 00000065 //C 0f7b4  2284 db	101 */
0x0000f7b5, 0x0000006d, /* 0000006d //C 0f7b5  2284 db	109 */
0x0000f7b6, 0x0000005d, /* 0000005d //C 0f7b6  2284 db	93 */
0x0000f7b7, 0x00000020, /* 00000020 //C 0f7b7  2284 db	32 */
0x0000f7b8, 0x00000061, /* 00000061 //C 0f7b8  2284 db	97 */
0x0000f7b9, 0x00000064, /* 00000064 //C 0f7b9  2284 db	100 */
0x0000f7ba, 0x00000064, /* 00000064 //C 0f7ba  2284 db	100 */
0x0000f7bb, 0x00000072, /* 00000072 //C 0f7bb  2284 db	114 */
0x0000f7bc, 0x00000020, /* 00000020 //C 0f7bc  2284 db	32 */
0x0000f7bd, 0x0000005b, /* 0000005b //C 0f7bd  2284 db	91 */
0x0000f7be, 0x00000076, /* 00000076 //C 0f7be  2284 db	118 */
0x0000f7bf, 0x00000061, /* 00000061 //C 0f7bf  2284 db	97 */
0x0000f7c0, 0x0000006c, /* 0000006c //C 0f7c0  2284 db	108 */
0x0000f7c1, 0x0000005d, /* 0000005d //C 0f7c1  2284 db	93 */
0x0000f7c2, 0x00000020, /* 00000020 //C 0f7c2  2284 db	32 */
0x0000f7c3, 0x00000020, /* 00000020 //C 0f7c3  2284 db	32 */
0x0000f7c4, 0x00000047, /* 00000047 //C 0f7c4  2284 db	71 */
0x0000f7c5, 0x00000065, /* 00000065 //C 0f7c5  2284 db	101 */
0x0000f7c6, 0x00000074, /* 00000074 //C 0f7c6  2284 db	116 */
0x0000f7c7, 0x0000002f, /* 0000002f //C 0f7c7  2284 db	47 */
0x0000f7c8, 0x00000073, /* 00000073 //C 0f7c8  2284 db	115 */
0x0000f7c9, 0x00000065, /* 00000065 //C 0f7c9  2284 db	101 */
0x0000f7ca, 0x00000074, /* 00000074 //C 0f7ca  2284 db	116 */
0x0000f7cb, 0x00000020, /* 00000020 //C 0f7cb  2284 db	32 */
0x0000f7cc, 0x0000006d, /* 0000006d //C 0f7cc  2284 db	109 */
0x0000f7cd, 0x00000065, /* 00000065 //C 0f7cd  2284 db	101 */
0x0000f7ce, 0x0000006d, /* 0000006d //C 0f7ce  2284 db	109 */
0x0000f7cf, 0x0000006f, /* 0000006f //C 0f7cf  2284 db	111 */
0x0000f7d0, 0x00000072, /* 00000072 //C 0f7d0  2284 db	114 */
0x0000f7d1, 0x00000079, /* 00000079 //C 0f7d1  2284 db	121 */
0x0000f7d2, 0x0000000a, /* 0000000a //C 0f7d2  2284 db	10 */
0x0000f7d3, 0x00000000, /* 00000000 //C 0f7d3  2284 db */
0x0000f7d4, 0x00000064, /* 00000064 //C 0f7d4  2285 db	100 */
0x0000f7d5, 0x0000005b, /* 0000005b //C 0f7d5  2285 db	91 */
0x0000f7d6, 0x00000075, /* 00000075 //C 0f7d6  2285 db	117 */
0x0000f7d7, 0x0000006d, /* 0000006d //C 0f7d7  2285 db	109 */
0x0000f7d8, 0x00000070, /* 00000070 //C 0f7d8  2285 db	112 */
0x0000f7d9, 0x0000005d, /* 0000005d //C 0f7d9  2285 db	93 */
0x0000f7da, 0x00000020, /* 00000020 //C 0f7da  2285 db	32 */
0x0000f7db, 0x00000073, /* 00000073 //C 0f7db  2285 db	115 */
0x0000f7dc, 0x00000074, /* 00000074 //C 0f7dc  2285 db	116 */
0x0000f7dd, 0x00000061, /* 00000061 //C 0f7dd  2285 db	97 */
0x0000f7de, 0x00000072, /* 00000072 //C 0f7de  2285 db	114 */
0x0000f7df, 0x00000074, /* 00000074 //C 0f7df  2285 db	116 */
0x0000f7e0, 0x00000020, /* 00000020 //C 0f7e0  2285 db	32 */
0x0000f7e1, 0x00000065, /* 00000065 //C 0f7e1  2285 db	101 */
0x0000f7e2, 0x0000006e, /* 0000006e //C 0f7e2  2285 db	110 */
0x0000f7e3, 0x00000064, /* 00000064 //C 0f7e3  2285 db	100 */
0x0000f7e4, 0x00000020, /* 00000020 //C 0f7e4  2285 db	32 */
0x0000f7e5, 0x00000020, /* 00000020 //C 0f7e5  2285 db	32 */
0x0000f7e6, 0x00000044, /* 00000044 //C 0f7e6  2285 db	68 */
0x0000f7e7, 0x00000075, /* 00000075 //C 0f7e7  2285 db	117 */
0x0000f7e8, 0x0000006d, /* 0000006d //C 0f7e8  2285 db	109 */
0x0000f7e9, 0x00000070, /* 00000070 //C 0f7e9  2285 db	112 */
0x0000f7ea, 0x00000020, /* 00000020 //C 0f7ea  2285 db	32 */
0x0000f7eb, 0x0000006d, /* 0000006d //C 0f7eb  2285 db	109 */
0x0000f7ec, 0x00000065, /* 00000065 //C 0f7ec  2285 db	101 */
0x0000f7ed, 0x0000006d, /* 0000006d //C 0f7ed  2285 db	109 */
0x0000f7ee, 0x0000006f, /* 0000006f //C 0f7ee  2285 db	111 */
0x0000f7ef, 0x00000072, /* 00000072 //C 0f7ef  2285 db	114 */
0x0000f7f0, 0x00000079, /* 00000079 //C 0f7f0  2285 db	121 */
0x0000f7f1, 0x00000020, /* 00000020 //C 0f7f1  2285 db	32 */
0x0000f7f2, 0x00000063, /* 00000063 //C 0f7f2  2285 db	99 */
0x0000f7f3, 0x0000006f, /* 0000006f //C 0f7f3  2285 db	111 */
0x0000f7f4, 0x0000006e, /* 0000006e //C 0f7f4  2285 db	110 */
0x0000f7f5, 0x00000074, /* 00000074 //C 0f7f5  2285 db	116 */
0x0000f7f6, 0x00000065, /* 00000065 //C 0f7f6  2285 db	101 */
0x0000f7f7, 0x0000006e, /* 0000006e //C 0f7f7  2285 db	110 */
0x0000f7f8, 0x00000074, /* 00000074 //C 0f7f8  2285 db	116 */
0x0000f7f9, 0x0000000a, /* 0000000a //C 0f7f9  2285 db	10 */
0x0000f7fa, 0x00000000, /* 00000000 //C 0f7fa  2285 db */
0x0000f7fb, 0x0000006c, /* 0000006c //C 0f7fb  2286 db	108 */
0x0000f7fc, 0x0000005b, /* 0000005b //C 0f7fc  2286 db	91 */
0x0000f7fd, 0x0000006f, /* 0000006f //C 0f7fd  2286 db	111 */
0x0000f7fe, 0x00000061, /* 00000061 //C 0f7fe  2286 db	97 */
0x0000f7ff, 0x00000064, /* 00000064 //C 0f7ff  2286 db	100 */
0x0000f800, 0x0000005d, /* 0000005d //C 0f800  2286 db	93 */
0x0000f801, 0x00000020, /* 00000020 //C 0f801  2286 db	32 */
0x0000f802, 0x00000020, /* 00000020 //C 0f802  2286 db	32 */
0x0000f803, 0x00000020, /* 00000020 //C 0f803  2286 db	32 */
0x0000f804, 0x00000020, /* 00000020 //C 0f804  2286 db	32 */
0x0000f805, 0x00000020, /* 00000020 //C 0f805  2286 db	32 */
0x0000f806, 0x00000020, /* 00000020 //C 0f806  2286 db	32 */
0x0000f807, 0x00000020, /* 00000020 //C 0f807  2286 db	32 */
0x0000f808, 0x00000020, /* 00000020 //C 0f808  2286 db	32 */
0x0000f809, 0x00000020, /* 00000020 //C 0f809  2286 db	32 */
0x0000f80a, 0x00000020, /* 00000020 //C 0f80a  2286 db	32 */
0x0000f80b, 0x00000020, /* 00000020 //C 0f80b  2286 db	32 */
0x0000f80c, 0x00000020, /* 00000020 //C 0f80c  2286 db	32 */
0x0000f80d, 0x0000004c, /* 0000004c //C 0f80d  2286 db	76 */
0x0000f80e, 0x0000006f, /* 0000006f //C 0f80e  2286 db	111 */
0x0000f80f, 0x00000061, /* 00000061 //C 0f80f  2286 db	97 */
0x0000f810, 0x00000064, /* 00000064 //C 0f810  2286 db	100 */
0x0000f811, 0x00000020, /* 00000020 //C 0f811  2286 db	32 */
0x0000f812, 0x00000068, /* 00000068 //C 0f812  2286 db	104 */
0x0000f813, 0x00000065, /* 00000065 //C 0f813  2286 db	101 */
0x0000f814, 0x00000078, /* 00000078 //C 0f814  2286 db	120 */
0x0000f815, 0x00000020, /* 00000020 //C 0f815  2286 db	32 */
0x0000f816, 0x00000066, /* 00000066 //C 0f816  2286 db	102 */
0x0000f817, 0x00000069, /* 00000069 //C 0f817  2286 db	105 */
0x0000f818, 0x0000006c, /* 0000006c //C 0f818  2286 db	108 */
0x0000f819, 0x00000065, /* 00000065 //C 0f819  2286 db	101 */
0x0000f81a, 0x00000020, /* 00000020 //C 0f81a  2286 db	32 */
0x0000f81b, 0x00000074, /* 00000074 //C 0f81b  2286 db	116 */
0x0000f81c, 0x0000006f, /* 0000006f //C 0f81c  2286 db	111 */
0x0000f81d, 0x00000020, /* 00000020 //C 0f81d  2286 db	32 */
0x0000f81e, 0x0000006d, /* 0000006d //C 0f81e  2286 db	109 */
0x0000f81f, 0x00000065, /* 00000065 //C 0f81f  2286 db	101 */
0x0000f820, 0x0000006d, /* 0000006d //C 0f820  2286 db	109 */
0x0000f821, 0x0000006f, /* 0000006f //C 0f821  2286 db	111 */
0x0000f822, 0x00000072, /* 00000072 //C 0f822  2286 db	114 */
0x0000f823, 0x00000079, /* 00000079 //C 0f823  2286 db	121 */
0x0000f824, 0x0000000a, /* 0000000a //C 0f824  2286 db	10 */
0x0000f825, 0x00000000, /* 00000000 //C 0f825  2286 db */
0x0000f826, 0x00000067, /* 00000067 //C 0f826  2287 db	103 */
0x0000f827, 0x0000005b, /* 0000005b //C 0f827  2287 db	91 */
0x0000f828, 0x0000006f, /* 0000006f //C 0f828  2287 db	111 */
0x0000f829, 0x0000005d, /* 0000005d //C 0f829  2287 db	93 */
0x0000f82a, 0x0000007c, /* 0000007c //C 0f82a  2287 db	124 */
0x0000f82b, 0x00000072, /* 00000072 //C 0f82b  2287 db	114 */
0x0000f82c, 0x00000075, /* 00000075 //C 0f82c  2287 db	117 */
0x0000f82d, 0x0000006e, /* 0000006e //C 0f82d  2287 db	110 */
0x0000f82e, 0x00000020, /* 00000020 //C 0f82e  2287 db	32 */
0x0000f82f, 0x0000005b, /* 0000005b //C 0f82f  2287 db	91 */
0x0000f830, 0x00000061, /* 00000061 //C 0f830  2287 db	97 */
0x0000f831, 0x00000064, /* 00000064 //C 0f831  2287 db	100 */
0x0000f832, 0x00000064, /* 00000064 //C 0f832  2287 db	100 */
0x0000f833, 0x00000072, /* 00000072 //C 0f833  2287 db	114 */
0x0000f834, 0x0000005d, /* 0000005d //C 0f834  2287 db	93 */
0x0000f835, 0x00000020, /* 00000020 //C 0f835  2287 db	32 */
0x0000f836, 0x00000020, /* 00000020 //C 0f836  2287 db	32 */
0x0000f837, 0x00000020, /* 00000020 //C 0f837  2287 db	32 */
0x0000f838, 0x00000052, /* 00000052 //C 0f838  2287 db	82 */
0x0000f839, 0x00000075, /* 00000075 //C 0f839  2287 db	117 */
0x0000f83a, 0x0000006e, /* 0000006e //C 0f83a  2287 db	110 */
0x0000f83b, 0x00000020, /* 00000020 //C 0f83b  2287 db	32 */
0x0000f83c, 0x00000066, /* 00000066 //C 0f83c  2287 db	102 */
0x0000f83d, 0x00000072, /* 00000072 //C 0f83d  2287 db	114 */
0x0000f83e, 0x0000006f, /* 0000006f //C 0f83e  2287 db	111 */
0x0000f83f, 0x0000006d, /* 0000006d //C 0f83f  2287 db	109 */
0x0000f840, 0x00000020, /* 00000020 //C 0f840  2287 db	32 */
0x0000f841, 0x00000061, /* 00000061 //C 0f841  2287 db	97 */
0x0000f842, 0x00000064, /* 00000064 //C 0f842  2287 db	100 */
0x0000f843, 0x00000064, /* 00000064 //C 0f843  2287 db	100 */
0x0000f844, 0x00000072, /* 00000072 //C 0f844  2287 db	114 */
0x0000f845, 0x00000065, /* 00000065 //C 0f845  2287 db	101 */
0x0000f846, 0x00000073, /* 00000073 //C 0f846  2287 db	115 */
0x0000f847, 0x00000073, /* 00000073 //C 0f847  2287 db	115 */
0x0000f848, 0x0000000a, /* 0000000a //C 0f848  2287 db	10 */
0x0000f849, 0x00000000, /* 00000000 //C 0f849  2287 db */
0x0000f84a, 0x00000072, /* 00000072 //C 0f84a  2288 db	114 */
0x0000f84b, 0x0000005b, /* 0000005b //C 0f84b  2288 db	91 */
0x0000f84c, 0x00000065, /* 00000065 //C 0f84c  2288 db	101 */
0x0000f84d, 0x00000067, /* 00000067 //C 0f84d  2288 db	103 */
0x0000f84e, 0x0000005b, /* 0000005b //C 0f84e  2288 db	91 */
0x0000f84f, 0x00000073, /* 00000073 //C 0f84f  2288 db	115 */
0x0000f850, 0x0000005d, /* 0000005d //C 0f850  2288 db	93 */
0x0000f851, 0x0000005d, /* 0000005d //C 0f851  2288 db	93 */
0x0000f852, 0x00000020, /* 00000020 //C 0f852  2288 db	32 */
0x0000f853, 0x00000020, /* 00000020 //C 0f853  2288 db	32 */
0x0000f854, 0x00000020, /* 00000020 //C 0f854  2288 db	32 */
0x0000f855, 0x00000020, /* 00000020 //C 0f855  2288 db	32 */
0x0000f856, 0x00000020, /* 00000020 //C 0f856  2288 db	32 */
0x0000f857, 0x00000020, /* 00000020 //C 0f857  2288 db	32 */
0x0000f858, 0x00000020, /* 00000020 //C 0f858  2288 db	32 */
0x0000f859, 0x00000020, /* 00000020 //C 0f859  2288 db	32 */
0x0000f85a, 0x00000020, /* 00000020 //C 0f85a  2288 db	32 */
0x0000f85b, 0x00000020, /* 00000020 //C 0f85b  2288 db	32 */
0x0000f85c, 0x00000050, /* 00000050 //C 0f85c  2288 db	80 */
0x0000f85d, 0x00000072, /* 00000072 //C 0f85d  2288 db	114 */
0x0000f85e, 0x00000069, /* 00000069 //C 0f85e  2288 db	105 */
0x0000f85f, 0x0000006e, /* 0000006e //C 0f85f  2288 db	110 */
0x0000f860, 0x00000074, /* 00000074 //C 0f860  2288 db	116 */
0x0000f861, 0x00000020, /* 00000020 //C 0f861  2288 db	32 */
0x0000f862, 0x00000072, /* 00000072 //C 0f862  2288 db	114 */
0x0000f863, 0x00000065, /* 00000065 //C 0f863  2288 db	101 */
0x0000f864, 0x00000067, /* 00000067 //C 0f864  2288 db	103 */
0x0000f865, 0x00000069, /* 00000069 //C 0f865  2288 db	105 */
0x0000f866, 0x00000073, /* 00000073 //C 0f866  2288 db	115 */
0x0000f867, 0x00000074, /* 00000074 //C 0f867  2288 db	116 */
0x0000f868, 0x00000065, /* 00000065 //C 0f868  2288 db	101 */
0x0000f869, 0x00000072, /* 00000072 //C 0f869  2288 db	114 */
0x0000f86a, 0x00000073, /* 00000073 //C 0f86a  2288 db	115 */
0x0000f86b, 0x0000000a, /* 0000000a //C 0f86b  2288 db	10 */
0x0000f86c, 0x00000000, /* 00000000 //C 0f86c  2288 db */
0x0000f86d, 0x00000072, /* 00000072 //C 0f86d  2289 db	114 */
0x0000f86e, 0x00000058, /* 00000058 //C 0f86e  2289 db	88 */
0x0000f86f, 0x00000020, /* 00000020 //C 0f86f  2289 db	32 */
0x0000f870, 0x0000005b, /* 0000005b //C 0f870  2289 db	91 */
0x0000f871, 0x00000076, /* 00000076 //C 0f871  2289 db	118 */
0x0000f872, 0x00000061, /* 00000061 //C 0f872  2289 db	97 */
0x0000f873, 0x0000006c, /* 0000006c //C 0f873  2289 db	108 */
0x0000f874, 0x0000005d, /* 0000005d //C 0f874  2289 db	93 */
0x0000f875, 0x00000020, /* 00000020 //C 0f875  2289 db	32 */
0x0000f876, 0x00000020, /* 00000020 //C 0f876  2289 db	32 */
0x0000f877, 0x00000020, /* 00000020 //C 0f877  2289 db	32 */
0x0000f878, 0x00000020, /* 00000020 //C 0f878  2289 db	32 */
0x0000f879, 0x00000020, /* 00000020 //C 0f879  2289 db	32 */
0x0000f87a, 0x00000020, /* 00000020 //C 0f87a  2289 db	32 */
0x0000f87b, 0x00000020, /* 00000020 //C 0f87b  2289 db	32 */
0x0000f87c, 0x00000020, /* 00000020 //C 0f87c  2289 db	32 */
0x0000f87d, 0x00000020, /* 00000020 //C 0f87d  2289 db	32 */
0x0000f87e, 0x00000020, /* 00000020 //C 0f87e  2289 db	32 */
0x0000f87f, 0x00000047, /* 00000047 //C 0f87f  2289 db	71 */
0x0000f880, 0x00000065, /* 00000065 //C 0f880  2289 db	101 */
0x0000f881, 0x00000074, /* 00000074 //C 0f881  2289 db	116 */
0x0000f882, 0x0000002f, /* 0000002f //C 0f882  2289 db	47 */
0x0000f883, 0x00000073, /* 00000073 //C 0f883  2289 db	115 */
0x0000f884, 0x00000065, /* 00000065 //C 0f884  2289 db	101 */
0x0000f885, 0x00000074, /* 00000074 //C 0f885  2289 db	116 */
0x0000f886, 0x00000020, /* 00000020 //C 0f886  2289 db	32 */
0x0000f887, 0x00000052, /* 00000052 //C 0f887  2289 db	82 */
0x0000f888, 0x00000058, /* 00000058 //C 0f888  2289 db	88 */
0x0000f889, 0x0000000a, /* 0000000a //C 0f889  2289 db	10 */
0x0000f88a, 0x00000000, /* 00000000 //C 0f88a  2289 db */
0x0000f88b, 0x00000073, /* 00000073 //C 0f88b  2290 db	115 */
0x0000f88c, 0x00000070, /* 00000070 //C 0f88c  2290 db	112 */
0x0000f88d, 0x00000020, /* 00000020 //C 0f88d  2290 db	32 */
0x0000f88e, 0x0000005b, /* 0000005b //C 0f88e  2290 db	91 */
0x0000f88f, 0x00000076, /* 00000076 //C 0f88f  2290 db	118 */
0x0000f890, 0x00000061, /* 00000061 //C 0f890  2290 db	97 */
0x0000f891, 0x0000006c, /* 0000006c //C 0f891  2290 db	108 */
0x0000f892, 0x0000005d, /* 0000005d //C 0f892  2290 db	93 */
0x0000f893, 0x00000020, /* 00000020 //C 0f893  2290 db	32 */
0x0000f894, 0x00000020, /* 00000020 //C 0f894  2290 db	32 */
0x0000f895, 0x00000020, /* 00000020 //C 0f895  2290 db	32 */
0x0000f896, 0x00000020, /* 00000020 //C 0f896  2290 db	32 */
0x0000f897, 0x00000020, /* 00000020 //C 0f897  2290 db	32 */
0x0000f898, 0x00000020, /* 00000020 //C 0f898  2290 db	32 */
0x0000f899, 0x00000020, /* 00000020 //C 0f899  2290 db	32 */
0x0000f89a, 0x00000020, /* 00000020 //C 0f89a  2290 db	32 */
0x0000f89b, 0x00000020, /* 00000020 //C 0f89b  2290 db	32 */
0x0000f89c, 0x00000020, /* 00000020 //C 0f89c  2290 db	32 */
0x0000f89d, 0x00000047, /* 00000047 //C 0f89d  2290 db	71 */
0x0000f89e, 0x00000065, /* 00000065 //C 0f89e  2290 db	101 */
0x0000f89f, 0x00000074, /* 00000074 //C 0f89f  2290 db	116 */
0x0000f8a0, 0x0000002f, /* 0000002f //C 0f8a0  2290 db	47 */
0x0000f8a1, 0x00000073, /* 00000073 //C 0f8a1  2290 db	115 */
0x0000f8a2, 0x00000065, /* 00000065 //C 0f8a2  2290 db	101 */
0x0000f8a3, 0x00000074, /* 00000074 //C 0f8a3  2290 db	116 */
0x0000f8a4, 0x00000020, /* 00000020 //C 0f8a4  2290 db	32 */
0x0000f8a5, 0x00000052, /* 00000052 //C 0f8a5  2290 db	82 */
0x0000f8a6, 0x00000031, /* 00000031 //C 0f8a6  2290 db	49 */
0x0000f8a7, 0x00000033, /* 00000033 //C 0f8a7  2290 db	51 */
0x0000f8a8, 0x0000000a, /* 0000000a //C 0f8a8  2290 db	10 */
0x0000f8a9, 0x00000000, /* 00000000 //C 0f8a9  2290 db */
0x0000f8aa, 0x0000006c, /* 0000006c //C 0f8aa  2291 db	108 */
0x0000f8ab, 0x00000072, /* 00000072 //C 0f8ab  2291 db	114 */
0x0000f8ac, 0x00000020, /* 00000020 //C 0f8ac  2291 db	32 */
0x0000f8ad, 0x0000005b, /* 0000005b //C 0f8ad  2291 db	91 */
0x0000f8ae, 0x00000076, /* 00000076 //C 0f8ae  2291 db	118 */
0x0000f8af, 0x00000061, /* 00000061 //C 0f8af  2291 db	97 */
0x0000f8b0, 0x0000006c, /* 0000006c //C 0f8b0  2291 db	108 */
0x0000f8b1, 0x0000005d, /* 0000005d //C 0f8b1  2291 db	93 */
0x0000f8b2, 0x00000020, /* 00000020 //C 0f8b2  2291 db	32 */
0x0000f8b3, 0x00000020, /* 00000020 //C 0f8b3  2291 db	32 */
0x0000f8b4, 0x00000020, /* 00000020 //C 0f8b4  2291 db	32 */
0x0000f8b5, 0x00000020, /* 00000020 //C 0f8b5  2291 db	32 */
0x0000f8b6, 0x00000020, /* 00000020 //C 0f8b6  2291 db	32 */
0x0000f8b7, 0x00000020, /* 00000020 //C 0f8b7  2291 db	32 */
0x0000f8b8, 0x00000020, /* 00000020 //C 0f8b8  2291 db	32 */
0x0000f8b9, 0x00000020, /* 00000020 //C 0f8b9  2291 db	32 */
0x0000f8ba, 0x00000020, /* 00000020 //C 0f8ba  2291 db	32 */
0x0000f8bb, 0x00000020, /* 00000020 //C 0f8bb  2291 db	32 */
0x0000f8bc, 0x00000047, /* 00000047 //C 0f8bc  2291 db	71 */
0x0000f8bd, 0x00000065, /* 00000065 //C 0f8bd  2291 db	101 */
0x0000f8be, 0x00000074, /* 00000074 //C 0f8be  2291 db	116 */
0x0000f8bf, 0x0000002f, /* 0000002f //C 0f8bf  2291 db	47 */
0x0000f8c0, 0x00000073, /* 00000073 //C 0f8c0  2291 db	115 */
0x0000f8c1, 0x00000065, /* 00000065 //C 0f8c1  2291 db	101 */
0x0000f8c2, 0x00000074, /* 00000074 //C 0f8c2  2291 db	116 */
0x0000f8c3, 0x00000020, /* 00000020 //C 0f8c3  2291 db	32 */
0x0000f8c4, 0x00000052, /* 00000052 //C 0f8c4  2291 db	82 */
0x0000f8c5, 0x00000031, /* 00000031 //C 0f8c5  2291 db	49 */
0x0000f8c6, 0x00000034, /* 00000034 //C 0f8c6  2291 db	52 */
0x0000f8c7, 0x0000000a, /* 0000000a //C 0f8c7  2291 db	10 */
0x0000f8c8, 0x00000000, /* 00000000 //C 0f8c8  2291 db */
0x0000f8c9, 0x00000070, /* 00000070 //C 0f8c9  2292 db	112 */
0x0000f8ca, 0x00000063, /* 00000063 //C 0f8ca  2292 db	99 */
0x0000f8cb, 0x00000020, /* 00000020 //C 0f8cb  2292 db	32 */
0x0000f8cc, 0x0000005b, /* 0000005b //C 0f8cc  2292 db	91 */
0x0000f8cd, 0x00000076, /* 00000076 //C 0f8cd  2292 db	118 */
0x0000f8ce, 0x00000061, /* 00000061 //C 0f8ce  2292 db	97 */
0x0000f8cf, 0x0000006c, /* 0000006c //C 0f8cf  2292 db	108 */
0x0000f8d0, 0x0000005d, /* 0000005d //C 0f8d0  2292 db	93 */
0x0000f8d1, 0x00000020, /* 00000020 //C 0f8d1  2292 db	32 */
0x0000f8d2, 0x00000020, /* 00000020 //C 0f8d2  2292 db	32 */
0x0000f8d3, 0x00000020, /* 00000020 //C 0f8d3  2292 db	32 */
0x0000f8d4, 0x00000020, /* 00000020 //C 0f8d4  2292 db	32 */
0x0000f8d5, 0x00000020, /* 00000020 //C 0f8d5  2292 db	32 */
0x0000f8d6, 0x00000020, /* 00000020 //C 0f8d6  2292 db	32 */
0x0000f8d7, 0x00000020, /* 00000020 //C 0f8d7  2292 db	32 */
0x0000f8d8, 0x00000020, /* 00000020 //C 0f8d8  2292 db	32 */
0x0000f8d9, 0x00000020, /* 00000020 //C 0f8d9  2292 db	32 */
0x0000f8da, 0x00000020, /* 00000020 //C 0f8da  2292 db	32 */
0x0000f8db, 0x00000047, /* 00000047 //C 0f8db  2292 db	71 */
0x0000f8dc, 0x00000065, /* 00000065 //C 0f8dc  2292 db	101 */
0x0000f8dd, 0x00000074, /* 00000074 //C 0f8dd  2292 db	116 */
0x0000f8de, 0x0000002f, /* 0000002f //C 0f8de  2292 db	47 */
0x0000f8df, 0x00000073, /* 00000073 //C 0f8df  2292 db	115 */
0x0000f8e0, 0x00000065, /* 00000065 //C 0f8e0  2292 db	101 */
0x0000f8e1, 0x00000074, /* 00000074 //C 0f8e1  2292 db	116 */
0x0000f8e2, 0x00000020, /* 00000020 //C 0f8e2  2292 db	32 */
0x0000f8e3, 0x00000052, /* 00000052 //C 0f8e3  2292 db	82 */
0x0000f8e4, 0x00000031, /* 00000031 //C 0f8e4  2292 db	49 */
0x0000f8e5, 0x00000035, /* 00000035 //C 0f8e5  2292 db	53 */
0x0000f8e6, 0x0000000a, /* 0000000a //C 0f8e6  2292 db	10 */
0x0000f8e7, 0x00000000, /* 00000000 //C 0f8e7  2292 db */
0x0000f8e8, 0x00000066, /* 00000066 //C 0f8e8  2293 db	102 */
0x0000f8e9, 0x00000020, /* 00000020 //C 0f8e9  2293 db	32 */
0x0000f8ea, 0x0000005b, /* 0000005b //C 0f8ea  2293 db	91 */
0x0000f8eb, 0x00000076, /* 00000076 //C 0f8eb  2293 db	118 */
0x0000f8ec, 0x00000061, /* 00000061 //C 0f8ec  2293 db	97 */
0x0000f8ed, 0x0000006c, /* 0000006c //C 0f8ed  2293 db	108 */
0x0000f8ee, 0x0000005d, /* 0000005d //C 0f8ee  2293 db	93 */
0x0000f8ef, 0x00000020, /* 00000020 //C 0f8ef  2293 db	32 */
0x0000f8f0, 0x00000020, /* 00000020 //C 0f8f0  2293 db	32 */
0x0000f8f1, 0x00000020, /* 00000020 //C 0f8f1  2293 db	32 */
0x0000f8f2, 0x00000020, /* 00000020 //C 0f8f2  2293 db	32 */
0x0000f8f3, 0x00000020, /* 00000020 //C 0f8f3  2293 db	32 */
0x0000f8f4, 0x00000020, /* 00000020 //C 0f8f4  2293 db	32 */
0x0000f8f5, 0x00000020, /* 00000020 //C 0f8f5  2293 db	32 */
0x0000f8f6, 0x00000020, /* 00000020 //C 0f8f6  2293 db	32 */
0x0000f8f7, 0x00000020, /* 00000020 //C 0f8f7  2293 db	32 */
0x0000f8f8, 0x00000020, /* 00000020 //C 0f8f8  2293 db	32 */
0x0000f8f9, 0x00000020, /* 00000020 //C 0f8f9  2293 db	32 */
0x0000f8fa, 0x00000047, /* 00000047 //C 0f8fa  2293 db	71 */
0x0000f8fb, 0x00000065, /* 00000065 //C 0f8fb  2293 db	101 */
0x0000f8fc, 0x00000074, /* 00000074 //C 0f8fc  2293 db	116 */
0x0000f8fd, 0x0000002f, /* 0000002f //C 0f8fd  2293 db	47 */
0x0000f8fe, 0x00000073, /* 00000073 //C 0f8fe  2293 db	115 */
0x0000f8ff, 0x00000065, /* 00000065 //C 0f8ff  2293 db	101 */
0x0000f900, 0x00000074, /* 00000074 //C 0f900  2293 db	116 */
0x0000f901, 0x00000020, /* 00000020 //C 0f901  2293 db	32 */
0x0000f902, 0x00000066, /* 00000066 //C 0f902  2293 db	102 */
0x0000f903, 0x0000006c, /* 0000006c //C 0f903  2293 db	108 */
0x0000f904, 0x00000061, /* 00000061 //C 0f904  2293 db	97 */
0x0000f905, 0x00000067, /* 00000067 //C 0f905  2293 db	103 */
0x0000f906, 0x00000073, /* 00000073 //C 0f906  2293 db	115 */
0x0000f907, 0x0000000a, /* 0000000a //C 0f907  2293 db	10 */
0x0000f908, 0x00000000, /* 00000000 //C 0f908  2293 db */
0x0000f909, 0x00000076, /* 00000076 //C 0f909  2294 db	118 */
0x0000f90a, 0x0000005b, /* 0000005b //C 0f90a  2294 db	91 */
0x0000f90b, 0x00000065, /* 00000065 //C 0f90b  2294 db	101 */
0x0000f90c, 0x00000072, /* 00000072 //C 0f90c  2294 db	114 */
0x0000f90d, 0x0000005d, /* 0000005d //C 0f90d  2294 db	93 */
0x0000f90e, 0x00000020, /* 00000020 //C 0f90e  2294 db	32 */
0x0000f90f, 0x00000020, /* 00000020 //C 0f90f  2294 db	32 */
0x0000f910, 0x00000020, /* 00000020 //C 0f910  2294 db	32 */
0x0000f911, 0x00000020, /* 00000020 //C 0f911  2294 db	32 */
0x0000f912, 0x00000020, /* 00000020 //C 0f912  2294 db	32 */
0x0000f913, 0x00000020, /* 00000020 //C 0f913  2294 db	32 */
0x0000f914, 0x00000020, /* 00000020 //C 0f914  2294 db	32 */
0x0000f915, 0x00000020, /* 00000020 //C 0f915  2294 db	32 */
0x0000f916, 0x00000020, /* 00000020 //C 0f916  2294 db	32 */
0x0000f917, 0x00000020, /* 00000020 //C 0f917  2294 db	32 */
0x0000f918, 0x00000020, /* 00000020 //C 0f918  2294 db	32 */
0x0000f919, 0x00000020, /* 00000020 //C 0f919  2294 db	32 */
0x0000f91a, 0x00000020, /* 00000020 //C 0f91a  2294 db	32 */
0x0000f91b, 0x00000050, /* 00000050 //C 0f91b  2294 db	80 */
0x0000f91c, 0x00000072, /* 00000072 //C 0f91c  2294 db	114 */
0x0000f91d, 0x00000069, /* 00000069 //C 0f91d  2294 db	105 */
0x0000f91e, 0x0000006e, /* 0000006e //C 0f91e  2294 db	110 */
0x0000f91f, 0x00000074, /* 00000074 //C 0f91f  2294 db	116 */
0x0000f920, 0x00000020, /* 00000020 //C 0f920  2294 db	32 */
0x0000f921, 0x00000070, /* 00000070 //C 0f921  2294 db	112 */
0x0000f922, 0x0000006d, /* 0000006d //C 0f922  2294 db	109 */
0x0000f923, 0x0000006f, /* 0000006f //C 0f923  2294 db	111 */
0x0000f924, 0x0000006e, /* 0000006e //C 0f924  2294 db	110 */
0x0000f925, 0x00000020, /* 00000020 //C 0f925  2294 db	32 */
0x0000f926, 0x00000061, /* 00000061 //C 0f926  2294 db	97 */
0x0000f927, 0x0000006e, /* 0000006e //C 0f927  2294 db	110 */
0x0000f928, 0x00000064, /* 00000064 //C 0f928  2294 db	100 */
0x0000f929, 0x00000020, /* 00000020 //C 0f929  2294 db	32 */
0x0000f92a, 0x00000063, /* 00000063 //C 0f92a  2294 db	99 */
0x0000f92b, 0x00000070, /* 00000070 //C 0f92b  2294 db	112 */
0x0000f92c, 0x00000075, /* 00000075 //C 0f92c  2294 db	117 */
0x0000f92d, 0x00000020, /* 00000020 //C 0f92d  2294 db	32 */
0x0000f92e, 0x00000076, /* 00000076 //C 0f92e  2294 db	118 */
0x0000f92f, 0x00000065, /* 00000065 //C 0f92f  2294 db	101 */
0x0000f930, 0x00000072, /* 00000072 //C 0f930  2294 db	114 */
0x0000f931, 0x00000073, /* 00000073 //C 0f931  2294 db	115 */
0x0000f932, 0x00000069, /* 00000069 //C 0f932  2294 db	105 */
0x0000f933, 0x0000006f, /* 0000006f //C 0f933  2294 db	111 */
0x0000f934, 0x0000006e, /* 0000006e //C 0f934  2294 db	110 */
0x0000f935, 0x0000000a, /* 0000000a //C 0f935  2294 db	10 */
0x0000f936, 0x00000000, /* 00000000 //C 0f936  2294 db */
0x0000f937, 0x00000068, /* 00000068 //C 0f937  2295 db	104 */
0x0000f938, 0x0000005b, /* 0000005b //C 0f938  2295 db	91 */
0x0000f939, 0x00000065, /* 00000065 //C 0f939  2295 db	101 */
0x0000f93a, 0x0000006c, /* 0000006c //C 0f93a  2295 db	108 */
0x0000f93b, 0x00000070, /* 00000070 //C 0f93b  2295 db	112 */
0x0000f93c, 0x0000005d, /* 0000005d //C 0f93c  2295 db	93 */
0x0000f93d, 0x0000002c, /* 0000002c //C 0f93d  2295 db	44 */
0x0000f93e, 0x0000003f, /* 0000003f //C 0f93e  2295 db	63 */
0x0000f93f, 0x00000020, /* 00000020 //C 0f93f  2295 db	32 */
0x0000f940, 0x00000020, /* 00000020 //C 0f940  2295 db	32 */
0x0000f941, 0x00000020, /* 00000020 //C 0f941  2295 db	32 */
0x0000f942, 0x00000020, /* 00000020 //C 0f942  2295 db	32 */
0x0000f943, 0x00000020, /* 00000020 //C 0f943  2295 db	32 */
0x0000f944, 0x00000020, /* 00000020 //C 0f944  2295 db	32 */
0x0000f945, 0x00000020, /* 00000020 //C 0f945  2295 db	32 */
0x0000f946, 0x00000020, /* 00000020 //C 0f946  2295 db	32 */
0x0000f947, 0x00000020, /* 00000020 //C 0f947  2295 db	32 */
0x0000f948, 0x00000020, /* 00000020 //C 0f948  2295 db	32 */
0x0000f949, 0x00000048, /* 00000048 //C 0f949  2295 db	72 */
0x0000f94a, 0x00000065, /* 00000065 //C 0f94a  2295 db	101 */
0x0000f94b, 0x0000006c, /* 0000006c //C 0f94b  2295 db	108 */
0x0000f94c, 0x00000070, /* 00000070 //C 0f94c  2295 db	112 */
0x0000f94d, 0x0000000a, /* 0000000a //C 0f94d  2295 db	10 */
0x0000f94e, 0x00000000, /* 00000000 //C 0f94e  2295 db */
0x0000f94f, 0x00000000, /* 00000000 //C 0f94f  2296 dd	0 */
0x0000f950, 0x00000000, /* 00000000 //C 0f950  2300 dd	0 */
0x0000f951, 0x00000000, /* 00000000 //C 0f951  2301 dd	0 */
0x0000f952, 0x00000000, /* 00000000 //C 0f952  2302 dd	0 */
0x0000f953, 0x00000000, /* 00000000 //C 0f953  2303 dd	0 */
0x0000f954, 0x00000000, /* 00000000 //C 0f954  2304 dd	0 */
0x0000f955, 0x00000000, /* 00000000 //C 0f955  2305 dd	0 */
0x0000f956, 0x00000000, /* 00000000 //C 0f956  2306 dd	0 */
0x0000f957, 0x00000000, /* 00000000 //C 0f957  2307 dd	0 */
0x0000f958, 0x00220000, /* 00220000 //C 0f958  2308 dd	0x00220000 */
0x0000f959, 0x02000000, /* 02000000 //C 0f959  2309 dd	0x02000000 */
0x0000f95a, 0x00000000, /* 00000000 //C 0f95a  2310 dd	0x00000000 */
0x0000f95b, 0x00004000, /* 00004000 //C 0f95b  2311 dd	0x00004000 */
0x0000f95c, 0x4f5b063f, /* 4f5b063f //C 0f95c  2312 dd	0x4f5b063f */
0x0000f95d, 0x277d6d66, /* 277d6d66 //C 0f95d  2313 dd	0x277d6d66 */
0x0000f95e, 0x00006fff, /* 00006fff //C 0f95e  2314 dd	0x00006fff */
0x0000f95f, 0x00004800, /* 00004800 //C 0f95f  2315 dd	0x00004800 */
0x0000f960, 0x397c7700, /* 397c7700 //C 0f960  2316 dd	0x397c7700 */
0x0000f961, 0x3d71795e, /* 3d71795e //C 0f961  2317 dd	0x3d71795e */
0x0000f962, 0x001e3076, /* 001e3076 //C 0f962  2318 dd	0x001e3076 */
0x0000f963, 0x3f543738, /* 3f543738 //C 0f963  2319 dd	0x3f543738 */
0x0000f964, 0x6d500073, /* 6d500073 //C 0f964  2320 dd	0x6d500073 */
0x0000f965, 0x00003e78, /* 00003e78 //C 0f965  2321 dd	0x00003e78 */
0x0000f966, 0x39006e00, /* 39006e00 //C 0f966  2322 dd	0x39006e00 */
0x0000f967, 0x08000f00, /* 08000f00 //C 0f967  2323 dd	0x08000f00 */
0x0000f968, 0x587c7700, /* 587c7700 //C 0f968  2324 dd	0x587c7700 */
0x0000f969, 0x3d71795e, /* 3d71795e //C 0f969  2325 dd	0x3d71795e */
0x0000f96a, 0x001e0474, /* 001e0474 //C 0f96a  2326 dd	0x001e0474 */
0x0000f96b, 0x5c540038, /* 5c540038 //C 0f96b  2327 dd	0x5c540038 */
0x0000f96c, 0x6d500073, /* 6d500073 //C 0f96c  2328 dd	0x6d500073 */
0x0000f96d, 0x00001c78, /* 00001c78 //C 0f96d  2329 dd	0x00001c78 */
0x0000f96e, 0x00000000, /* 00000000 //C 0f96e  2330 dd	0x00000000 */
0x0000f96f, 0x00000000, /* 00000000 //C 0f96f  2331 dd	0x00000000 */
0x0000f970, 0x00000000, /* 00000000 //C 0f970  2336 ds	0x40 */
0x0000f9b0, 0x00000000, /* 00000000 //C 0f9b0  2338 dd	0 */
0x0000f9b1, 0x00000000, /* 00000000 //C 0f9b1     3  */
0xffffffff, 0xffffffff
};
