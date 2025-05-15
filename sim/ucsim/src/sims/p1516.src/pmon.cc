#include "pmon.h"

const char * p12cpu_version= "2.2.15";

t_mem pmon[]= {
0x00000000, 0x01f2f040, /* 01f2f040 //C 00000    21 jmp	cold_start */
0x0000f000, 0x01f2f029, /* 01f2f029 //C 0f000    26 _f000:	jmp	callin */
0x0000f001, 0x01f2f01a, /* 01f2f01a //C 0f001    27 _f001:	jmp	enter_by_uart */
0x0000f002, 0x01f2f681, /* 01f2f681 //C 0f002    28 _f002:	jmp	getchar */
0x0000f003, 0x01f2f7fe, /* 01f2f7fe //C 0f003    29 _f003:	jmp	version */
0x0000f004, 0x01f2f403, /* 01f2f403 //C 0f004    30 _f004:	jmp	mon_itobcd */
0x0000f005, 0x01f2f040, /* 01f2f040 //C 0f005    31 _f005:	jmp	cold_start */
0x0000f006, 0x01f2f475, /* 01f2f475 //C 0f006    32 _f006:	jmp	strchr */
0x0000f007, 0x01f2f4e7, /* 01f2f4e7 //C 0f007    33 _f007:	jmp	streq */
0x0000f008, 0x01f2f676, /* 01f2f676 //C 0f008    34 _f008:	jmp	check_uart */
0x0000f009, 0x01f2f510, /* 01f2f510 //C 0f009    35 _f009:	jmp	hexchar2value */
0x0000f00a, 0x01f2f53f, /* 01f2f53f //C 0f00a    36 _f00a:	jmp	value2hexchar */
0x0000f00b, 0x01f2f543, /* 01f2f543 //C 0f00b    37 _f00b:	jmp	htoi */
0x0000f00c, 0x01f2f4ef, /* 01f2f4ef //C 0f00c    38 _f00c:	jmp	strieq */
0x0000f00d, 0x01f2f67e, /* 01f2f67e //C 0f00d    39 _f00d:	jmp	read */
0x0000f00e, 0x01f2f685, /* 01f2f685 //C 0f00e    40 _f00e:	jmp	putchar */
0x0000f00f, 0x01f2f68c, /* 01f2f68c //C 0f00f    41 _f00f:	jmp	prints */
0x0000f010, 0x01f2f6c2, /* 01f2f6c2 //C 0f010    42 _f010:	jmp	printsnl */
0x0000f011, 0x01f2f6c9, /* 01f2f6c9 //C 0f011    43 _f011:	jmp	print_vhex */
0x0000f012, 0x01f2f6aa, /* 01f2f6aa //C 0f012    44 _f012:	jmp	pes */
0x0000f013, 0x01f2f6ef, /* 01f2f6ef //C 0f013    45 _f013:	jmp	printd */
0x0000f014, 0x01f2f702, /* 01f2f702 //C 0f014    46 _f014:	jmp	printf */
0x0000f015, 0x01f2f7f5, /* 01f2f7f5 //C 0f015    47 _f015:	jmp	pesf */
0x0000f016, 0x01f2f41a, /* 01f2f41a //C 0f016    48 _f016:	jmp	ascii2seg */
0x0000f017, 0x01f2f427, /* 01f2f427 //C 0f017    49 _f017:	jmp	strunpack */
0x0000f018, 0x01f2f446, /* 01f2f446 //C 0f018    50 _f018:	jmp	strpack */
0x0000f019, 0x01f2f4f7, /* 01f2f4f7 //C 0f019    51 _f019:	jmp	strlen */
0x0000f01a, 0x0d0d0000, /* 0d0d0000 //C 0f01a    55 push	r0 */
0x0000f01b, 0x020e0000, /* 020e0000 //C 0f01b    56 getf	r0 */
0x0000f01c, 0x0d0d0000, /* 0d0d0000 //C 0f01c    57 push	r0 */
0x0000f01d, 0x0700ff42, /* 0700ff42 //C 0f01d    58 ld	r0,UART_RSTAT */
0x0000f01e, 0x010c0001, /* 010c0001 //C 0f01e    59 test	r0,1 */
0x0000f01f, 0x21f2f024, /* 21f2f024 //C 0f01f    60 jnz	ebu_callin */
0x0000f020, 0x0f0d0000, /* 0f0d0000 //C 0f020    62 pop	r0 */
0x0000f021, 0x020f0000, /* 020f0000 //C 0f021    63 setf	r0 */
0x0000f022, 0x0f0d0000, /* 0f0d0000 //C 0f022    64 pop	r0 */
0x0000f023, 0x00f00e00, /* 00f00e00 //C 0f023    65 ret */
0x0000f024, 0x0700ff40, /* 0700ff40 //C 0f024    67 ld	r0,UART_DR */
0x0000f025, 0x0f0d0000, /* 0f0d0000 //C 0f025    68 pop	r0 */
0x0000f026, 0x020f0000, /* 020f0000 //C 0f026    69 setf	r0 */
0x0000f027, 0x0f0d0000, /* 0f0d0000 //C 0f027    70 pop	r0 */
0x0000f028, 0x01f2f029, /* 01f2f029 //C 0f028    71 jmp	callin */
0x0000f029, 0x0600f86e, /* 0600f86e //C 0f029    74 st	r0,reg0 */
0x0000f02a, 0x0610f86f, /* 0610f86f //C 0f02a    75 st	r1,reg1 */
0x0000f02b, 0x0620f870, /* 0620f870 //C 0f02b    76 st	r2,reg2 */
0x0000f02c, 0x0630f871, /* 0630f871 //C 0f02c    77 st	r3,reg3 */
0x0000f02d, 0x0640f872, /* 0640f872 //C 0f02d    78 st	r4,reg4 */
0x0000f02e, 0x0650f873, /* 0650f873 //C 0f02e    79 st	r5,reg5 */
0x0000f02f, 0x0660f874, /* 0660f874 //C 0f02f    80 st	r6,reg6 */
0x0000f030, 0x0670f875, /* 0670f875 //C 0f030    81 st	r7,reg7 */
0x0000f031, 0x0680f876, /* 0680f876 //C 0f031    82 st	r8,reg8 */
0x0000f032, 0x0690f877, /* 0690f877 //C 0f032    83 st	r9,reg9 */
0x0000f033, 0x06a0f878, /* 06a0f878 //C 0f033    84 st	r10,reg10 */
0x0000f034, 0x06b0f879, /* 06b0f879 //C 0f034    85 st	r11,reg11 */
0x0000f035, 0x06c0f87a, /* 06c0f87a //C 0f035    86 st	r12,reg12 */
0x0000f036, 0x06d0f87b, /* 06d0f87b //C 0f036    87 st	r13,reg13 */
0x0000f037, 0x06e0f87c, /* 06e0f87c //C 0f037    88 st	r14,reg14 */
0x0000f038, 0x06e0f87d, /* 06e0f87d //C 0f038    89 st	r14,reg15 */
0x0000f039, 0x020e0000, /* 020e0000 //C 0f039    90 getf	r0 */
0x0000f03a, 0x0600f87e, /* 0600f87e //C 0f03a    91 st	r0,regf */
0x0000f03b, 0x01020001, /* 01020001 //C 0f03b    92 mvzl	r0,1 */
0x0000f03c, 0x0600f86c, /* 0600f86c //C 0f03c    93 st	r0,called */
0x0000f03d, 0x01f2f04a, /* 01f2f04a //C 0f03d    94 jmp	common_start */
0x0000f03e, 0x01f2f04a, /* 01f2f04a //C 0f03e    96 jmp	common_start */
0x0000f03f, 0x01f2f040, /* 01f2f040 //C 0f03f    98 jmp	cold_start */
0x0000f040, 0x01020000, /* 01020000 //C 0f040   100 mvzl	r0,0 */
0x0000f041, 0x0600f86c, /* 0600f86c //C 0f041   101 st	r0,called */
0x0000f042, 0x0102f03f, /* 0102f03f //C 0f042   102 mvzl	r0,def_zero	; restore jmp to monitor at zero */
0x0000f043, 0x0e000000, /* 0e000000 //C 0f043   103 ld	r0,r0 */
0x0000f044, 0x06000000, /* 06000000 //C 0f044   104 st	r0,0 */
0x0000f045, 0x01020000, /* 01020000 //C 0f045   105 mvzl	r0,0		; def values of some regs */
0x0000f046, 0x0600f87e, /* 0600f87e //C 0f046   106 st	r0,regf		; FALGS= 0 */
0x0000f047, 0x0102f7ff, /* 0102f7ff //C 0f047   107 mvzl	r0,0xf7ff	; R13= 0xf7ff */
0x0000f048, 0x0600f87b, /* 0600f87b //C 0f048   108 st	r0,reg13 */
0x0000f049, 0x01f2f04a, /* 01f2f04a //C 0f049   109 jmp	common_start */
0x0000f04a, 0x01d2fb1f, /* 01d2fb1f //C 0f04a   112 mvzl	sp,stack_end */
0x0000f04b, 0x01020001, /* 01020001 //C 0f04b   113 mvzl	r0,1 */
0x0000f04c, 0x0600f86b, /* 0600f86b //C 0f04c   114 st	r0,echo */
0x0000f04d, 0x01020000, /* 01020000 //C 0f04d   115 mvzl	r0,0 */
0x0000f04e, 0x020f0000, /* 020f0000 //C 0f04e   116 setf	r0 */
0x0000f04f, 0x0710ff41, /* 0710ff41 //C 0f04f   121 ld	r1,UART_CTRL	; check if transmit is enabled */
0x0000f050, 0x011c0002, /* 011c0002 //C 0f050   122 test	r1,2 */
0x0000f051, 0x11f2f055, /* 11f2f055 //C 0f051   123 jz	tr_is_off */
0x0000f052, 0x0710ff43, /* 0710ff43 //C 0f052   125 ld	r1,UART_TSTAT	; if transmit is ongoing */
0x0000f053, 0x011c0001, /* 011c0001 //C 0f053   126 test	r1,1		; wait it to finish */
0x0000f054, 0x11f2f052, /* 11f2f052 //C 0f054   127 jz	wait_uart_tr */
0x0000f055, 0x01120003, /* 01120003 //C 0f055   129 mvzl	r1,3		; turn on rx and tx */
0x0000f056, 0x0610ff41, /* 0610ff41 //C 0f056   130 st	r1,UART_CTRL */
0x0000f057, 0xf400f6aa, /* f400f6aa //C 0f057   133 ces	pes */
0x0000f058, 0x0000001b, /* 0000001b //C 0f058   134 db	27 */
0x0000f059, 0x0000005b, /* 0000005b //C 0f059   134 db	91 */
0x0000f05a, 0x0000003f, /* 0000003f //C 0f05a   134 db	63 */
0x0000f05b, 0x00000032, /* 00000032 //C 0f05b   134 db	50 */
0x0000f05c, 0x00000035, /* 00000035 //C 0f05c   134 db	53 */
0x0000f05d, 0x00000068, /* 00000068 //C 0f05d   134 db	104 */
0x0000f05e, 0x00000000, /* 00000000 //C 0f05e   134 db */
0x0000f05f, 0xf400f6aa, /* f400f6aa //C 0f05f   135 ces	pes */
0x0000f060, 0x0000001b, /* 0000001b //C 0f060   136 db	27 */
0x0000f061, 0x0000005b, /* 0000005b //C 0f061   136 db	91 */
0x0000f062, 0x00000034, /* 00000034 //C 0f062   136 db	52 */
0x0000f063, 0x00000030, /* 00000030 //C 0f063   136 db	48 */
0x0000f064, 0x0000003b, /* 0000003b //C 0f064   136 db	59 */
0x0000f065, 0x00000033, /* 00000033 //C 0f065   136 db	51 */
0x0000f066, 0x00000037, /* 00000037 //C 0f066   136 db	55 */
0x0000f067, 0x0000006d, /* 0000006d //C 0f067   136 db	109 */
0x0000f068, 0x00000000, /* 00000000 //C 0f068   136 db */
0x0000f069, 0x0102000a, /* 0102000a //C 0f069   137 mvzl	r0,LF */
0x0000f06a, 0x0400f685, /* 0400f685 //C 0f06a   138 call	putchar */
0x0000f06b, 0x0102005f, /* 0102005f //C 0f06b   140 mvzl	r0,'_' */
0x0000f06c, 0x0600ffff, /* 0600ffff //C 0f06c   141 st	r0,SIMIF */
0x0000f06d, 0x0700ffff, /* 0700ffff //C 0f06d   142 ld	r0,SIMIF */
0x0000f06e, 0x01080076, /* 01080076 //C 0f06e   143 cmp	r0,'v' */
0x0000f06f, 0x11f2f076, /* 11f2f076 //C 0f06f   144 jz	skip_welcome */
0x0000f070, 0x06020100, /* 06020100 //C 0f070   146 rds	r0,sver */
0x0000f071, 0x0611c002, /* 0611c002 //C 0f071   147 getbz	r1,r0,2 */
0x0000f072, 0x0621c001, /* 0621c001 //C 0f072   148 getbz	r2,r0,1 */
0x0000f073, 0x0631c000, /* 0631c000 //C 0f073   149 getbz	r3,r0,0 */
0x0000f074, 0x0102f87f, /* 0102f87f //C 0f074   150 mvzl	r0,msg_start */
0x0000f075, 0x0400f702, /* 0400f702 //C 0f075   151 call	printf */
0x0000f076, 0x0700f86c, /* 0700f86c //C 0f076   154 ld	r0,called */
0x0000f077, 0x020b0000, /* 020b0000 //C 0f077   155 sz	r0 */
0x0000f078, 0x11f2f082, /* 11f2f082 //C 0f078   156 jz	no_called_from */
0x0000f079, 0x0102000a, /* 0102000a //C 0f079   157 mvzl	r0,LF */
0x0000f07a, 0x0400f685, /* 0400f685 //C 0f07a   158 call	putchar */
0x0000f07b, 0x0102f898, /* 0102f898 //C 0f07b   159 mvzl	r0,msg_stopat */
0x0000f07c, 0x0400f68c, /* 0400f68c //C 0f07c   160 call	prints */
0x0000f07d, 0x0700f87c, /* 0700f87c //C 0f07d   161 ld	r0,reg14 */
0x0000f07e, 0x01120004, /* 01120004 //C 0f07e   162 mvzl	r1,4 */
0x0000f07f, 0x0400f6c9, /* 0400f6c9 //C 0f07f   163 call	print_vhex */
0x0000f080, 0x0102000a, /* 0102000a //C 0f080   164 mvzl	r0,LF */
0x0000f081, 0x0400f685, /* 0400f685 //C 0f081   165 call	putchar */
0x0000f082, 0x0400f084, /* 0400f084 //C 0f082   169 call	setup_line */
0x0000f083, 0x01f2f08d, /* 01f2f08d //C 0f083   173 jmp	main */
0x0000f084, 0x0ded0000, /* 0ded0000 //C 0f084   179 push	lr */
0x0000f085, 0x01120000, /* 01120000 //C 0f085   180 mvzl	r1,0		; lptr= 0 */
0x0000f086, 0x0610f864, /* 0610f864 //C 0f086   181 st	r1,line_ptr */
0x0000f087, 0x0610f800, /* 0610f800 //C 0f087   182 st	r1,line		; line[0]= 0 */
0x0000f088, 0x01120000, /* 01120000 //C 0f088   183 mvzl	r1,0		; at_eol= 0 */
0x0000f089, 0x0610f865, /* 0610f865 //C 0f089   184 st	r1,at_eol */
0x0000f08a, 0x0102f8a6, /* 0102f8a6 //C 0f08a   186 mvzl	r0,prompt */
0x0000f08b, 0x0400f68c, /* 0400f68c //C 0f08b   187 call	prints */
0x0000f08c, 0x0ffd0000, /* 0ffd0000 //C 0f08c   188 pop	pc */
0x0000f08d, 0x0400f676, /* 0400f676 //C 0f08d   195 call	check_uart */
0x0000f08e, 0x41f2f094, /* 41f2f094 //C 0f08e   196 C0 jmp	no_input */
0x0000f08f, 0x0400f67e, /* 0400f67e //C 0f08f   198 call	read */
0x0000f090, 0x0400f095, /* 0400f095 //C 0f090   199 call	proc_input */
0x0000f091, 0x41f2f094, /* 41f2f094 //C 0f091   200 C0 jmp	no_line */
0x0000f092, 0x0400f0c6, /* 0400f0c6 //C 0f092   203 call	proc_line */
0x0000f093, 0x0400f084, /* 0400f084 //C 0f093   204 call	setup_line */
0x0000f094, 0x01f2f08d, /* 01f2f08d //C 0f094   207 jmp	main */
0x0000f095, 0x0ded0000, /* 0ded0000 //C 0f095   213 push	lr */
0x0000f096, 0x0108000a, /* 0108000a //C 0f096   214 cmp	r0,LF */
0x0000f097, 0x11f2f0c0, /* 11f2f0c0 //C 0f097   215 EQ jmp	got_eol */
0x0000f098, 0x0108000d, /* 0108000d //C 0f098   216 cmp	r0,CR */
0x0000f099, 0x11f2f0c0, /* 11f2f0c0 //C 0f099   217 EQ jmp	got_eol */
0x0000f09a, 0x01080008, /* 01080008 //C 0f09a   218 cmp	r0,8 */
0x0000f09b, 0x11f2f09f, /* 11f2f09f //C 0f09b   219 jz	got_BS */
0x0000f09c, 0x0108007f, /* 0108007f //C 0f09c   220 cmp	r0,127 */
0x0000f09d, 0x11f2f09f, /* 11f2f09f //C 0f09d   221 jz	got_DEL */
0x0000f09e, 0x01f2f0ab, /* 01f2f0ab //C 0f09e   222 jmp	got_char */
0x0000f09f, 0x0700f864, /* 0700f864 //C 0f09f   225 ld	r0,line_ptr */
0x0000f0a0, 0x020b0000, /* 020b0000 //C 0f0a0   226 sz	r0 */
0x0000f0a1, 0x11f2f0a9, /* 11f2f0a9 //C 0f0a1   227 jz	got_done */
0x0000f0a2, 0x01060001, /* 01060001 //C 0f0a2   228 sub	r0,1 */
0x0000f0a3, 0x0600f864, /* 0600f864 //C 0f0a3   229 st	r0,line_ptr */
0x0000f0a4, 0x0112f800, /* 0112f800 //C 0f0a4   230 mvzl	r1,line */
0x0000f0a5, 0x01220000, /* 01220000 //C 0f0a5   231 mvzl	r2,0 */
0x0000f0a6, 0x08210000, /* 08210000 //C 0f0a6   232 st	r2,r1,r0 */
0x0000f0a7, 0x0102f8a2, /* 0102f8a2 //C 0f0a7   233 mvzl	r0,msg_BS */
0x0000f0a8, 0x0400f68c, /* 0400f68c //C 0f0a8   234 call	prints */
0x0000f0a9, 0x020d0000, /* 020d0000 //C 0f0a9   236 clc */
0x0000f0aa, 0x01f2f0c5, /* 01f2f0c5 //C 0f0aa   237 jmp	proc_input_ret */
0x0000f0ab, 0x0108001f, /* 0108001f //C 0f0ab   239 cmp	r0,31		; refuse control chars */
0x0000f0ac, 0xa1f2f0c5, /* a1f2f0c5 //C 0f0ac   240 LS jmp	proc_input_ret */
0x0000f0ad, 0x0108007e, /* 0108007e //C 0f0ad   241 cmp	r0,126		; refuse graph chars */
0x0000f0ae, 0x91f2f0c5, /* 91f2f0c5 //C 0f0ae   242 HI jmp	proc_input_ret */
0x0000f0af, 0x01220000, /* 01220000 //C 0f0af   243 mvzl	r2,0		; at_aol= 0 */
0x0000f0b0, 0x0620f865, /* 0620f865 //C 0f0b0   244 st	r2,at_eol */
0x0000f0b1, 0x0112f864, /* 0112f864 //C 0f0b1   245 mvzl	r1,line_ptr	; line[line_ptr]= char */
0x0000f0b2, 0x0e310000, /* 0e310000 //C 0f0b2   246 ld	r3,r1 */
0x0000f0b3, 0x0122f800, /* 0122f800 //C 0f0b3   247 mvzl	r2,line */
0x0000f0b4, 0x08020300, /* 08020300 //C 0f0b4   248 st	r0,r2,r3 */
0x0000f0b5, 0x013a0001, /* 013a0001 //C 0f0b5   250 plus	r3,1		; line_ptr++ */
0x0000f0b6, 0x0c310000, /* 0c310000 //C 0f0b6   251 st	r3,r1 */
0x0000f0b7, 0x01420000, /* 01420000 //C 0f0b7   252 mvzl	r4,0 */
0x0000f0b8, 0x09438200, /* 09438200 //C 0f0b8   253 st	r4,r3+,r2	; line[line_ptr]= 0 */
0x0000f0b9, 0x08430200, /* 08430200 //C 0f0b9   254 st	r4,r3,r2	; double 0 at end, needed by tokenizer */
0x0000f0ba, 0x0142f86b, /* 0142f86b //C 0f0ba   255 mvzl	r4,echo		; check if echo is turned on */
0x0000f0bb, 0x0e440000, /* 0e440000 //C 0f0bb   256 ld	r4,r4 */
0x0000f0bc, 0x024b0000, /* 024b0000 //C 0f0bc   257 sz	r4 */
0x0000f0bd, 0x2400f685, /* 2400f685 //C 0f0bd   258 NZ call	putchar		; echo */
0x0000f0be, 0x020d0000, /* 020d0000 //C 0f0be   259 clc */
0x0000f0bf, 0x01f2f0c5, /* 01f2f0c5 //C 0f0bf   260 jmp	proc_input_ret */
0x0000f0c0, 0x0112f865, /* 0112f865 //C 0f0c0   262 mvzl	r1,at_eol */
0x0000f0c1, 0x0e110000, /* 0e110000 //C 0f0c1   263 ld	r1,r1 */
0x0000f0c2, 0x021b0000, /* 021b0000 //C 0f0c2   264 sz	r1		; Z=0 at eol -> skip, not ready */
0x0000f0c3, 0x220d0000, /* 220d0000 //C 0f0c3   265 Z0 clc */
0x0000f0c4, 0x120c0000, /* 120c0000 //C 0f0c4   266 Z1 sec */
0x0000f0c5, 0x0ffd0000, /* 0ffd0000 //C 0f0c5   268 pop	pc */
0x0000f0c6, 0x0ded0000, /* 0ded0000 //C 0f0c6   277 push	lr */
0x0000f0c7, 0x0102000a, /* 0102000a //C 0f0c7   279 mvzl	r0,LF */
0x0000f0c8, 0x0400f685, /* 0400f685 //C 0f0c8   280 call	putchar */
0x0000f0c9, 0x0700f800, /* 0700f800 //C 0f0c9   282 ld	r0,line */
0x0000f0ca, 0x020b0000, /* 020b0000 //C 0f0ca   283 sz	r0 */
0x0000f0cb, 0x11f2f0d3, /* 11f2f0d3 //C 0f0cb   284 jz	proc_line_ret */
0x0000f0cc, 0x0400f0ef, /* 0400f0ef //C 0f0cc   292 call	tokenize */
0x0000f0cd, 0x0400f10f, /* 0400f10f //C 0f0cd   303 call	find_cmd */
0x0000f0ce, 0x41f2f0d1, /* 41f2f0d1 //C 0f0ce   304 C0 jmp	cmd_not_found */
0x0000f0cf, 0x05000000, /* 05000000 //C 0f0cf   307 call	r0,0 */
0x0000f0d0, 0x01f2f0d3, /* 01f2f0d3 //C 0f0d0   309 jmp	proc_line_ret */
0x0000f0d1, 0x0102f0db, /* 0102f0db //C 0f0d1   311 mvzl	r0,snotfound */
0x0000f0d2, 0x0400f6c2, /* 0400f6c2 //C 0f0d2   312 call	printsnl */
0x0000f0d3, 0x01120001, /* 01120001 //C 0f0d3   315 mvzl	r1,1		; at_eol= 1 */
0x0000f0d4, 0x0610f865, /* 0610f865 //C 0f0d4   316 st	r1,at_eol */
0x0000f0d5, 0x0ffd0000, /* 0ffd0000 //C 0f0d5   317 pop	pc */
0x0000f0d6, 0x00000047, /* 00000047 //C 0f0d6   319 db	71 */
0x0000f0d7, 0x0000006f, /* 0000006f //C 0f0d7   319 db	111 */
0x0000f0d8, 0x00000074, /* 00000074 //C 0f0d8   319 db	116 */
0x0000f0d9, 0x0000003a, /* 0000003a //C 0f0d9   319 db	58 */
0x0000f0da, 0x00000000, /* 00000000 //C 0f0da   319 db */
0x0000f0db, 0x00000055, /* 00000055 //C 0f0db   320 db	85 */
0x0000f0dc, 0x0000006e, /* 0000006e //C 0f0dc   320 db	110 */
0x0000f0dd, 0x0000006b, /* 0000006b //C 0f0dd   320 db	107 */
0x0000f0de, 0x0000006e, /* 0000006e //C 0f0de   320 db	110 */
0x0000f0df, 0x0000006f, /* 0000006f //C 0f0df   320 db	111 */
0x0000f0e0, 0x00000077, /* 00000077 //C 0f0e0   320 db	119 */
0x0000f0e1, 0x0000006e, /* 0000006e //C 0f0e1   320 db	110 */
0x0000f0e2, 0x00000020, /* 00000020 //C 0f0e2   320 db	32 */
0x0000f0e3, 0x00000063, /* 00000063 //C 0f0e3   320 db	99 */
0x0000f0e4, 0x0000006f, /* 0000006f //C 0f0e4   320 db	111 */
0x0000f0e5, 0x0000006d, /* 0000006d //C 0f0e5   320 db	109 */
0x0000f0e6, 0x0000006d, /* 0000006d //C 0f0e6   320 db	109 */
0x0000f0e7, 0x00000061, /* 00000061 //C 0f0e7   320 db	97 */
0x0000f0e8, 0x0000006e, /* 0000006e //C 0f0e8   320 db	110 */
0x0000f0e9, 0x00000064, /* 00000064 //C 0f0e9   320 db	100 */
0x0000f0ea, 0x00000000, /* 00000000 //C 0f0ea   320 db */
0x0000f0eb, 0x0ded0000, /* 0ded0000 //C 0f0eb   328 push	lr */
0x0000f0ec, 0x0112f8a8, /* 0112f8a8 //C 0f0ec   329 mvzl	r1,delimiters */
0x0000f0ed, 0x0400f475, /* 0400f475 //C 0f0ed   330 call	strchr */
0x0000f0ee, 0x0ffd0000, /* 0ffd0000 //C 0f0ee   331 pop	pc */
0x0000f0ef, 0x0ded0000, /* 0ded0000 //C 0f0ef   340 push	lr */
0x0000f0f0, 0x0142f866, /* 0142f866 //C 0f0f0   341 mvzl	r4,words	; array of result */
0x0000f0f1, 0x0152f800, /* 0152f800 //C 0f0f1   342 mvzl	r5,line		; address of next char */
0x0000f0f2, 0x01620000, /* 01620000 //C 0f0f2   343 mvzl	r6,0		; nuof words found */
0x0000f0f3, 0x01720000, /* 01720000 //C 0f0f3   344 mvzl	r7,0		; bool in_word */
0x0000f0f4, 0x0e050000, /* 0e050000 //C 0f0f4   346 ld	r0,r5		; pick a char */
0x0000f0f5, 0x020b0000, /* 020b0000 //C 0f0f5   347 sz	r0		; check end */
0x0000f0f6, 0x11f2f100, /* 11f2f100 //C 0f0f6   348 jz	tok_delimiter	; found end, pretend delim */
0x0000f0f7, 0x0400f0eb, /* 0400f0eb //C 0f0f7   350 call	is_delimiter */
0x0000f0f8, 0x31f2f100, /* 31f2f100 //C 0f0f8   351 C1 jmp	tok_delimiter */
0x0000f0f9, 0x027b0000, /* 027b0000 //C 0f0f9   353 sz	r7 */
0x0000f0fa, 0x21f2f105, /* 21f2f105 //C 0f0fa   354 jnz	tok_next	; still inside word */
0x0000f0fb, 0x01720001, /* 01720001 //C 0f0fb   356 mvzl	r7,1		; in_word=true */
0x0000f0fc, 0x09568400, /* 09568400 //C 0f0fc   357 st	r5,r6+,r4	; record word address */
0x0000f0fd, 0x01680005, /* 01680005 //C 0f0fd   358 cmp	r6,MAX_WORDS	; If no more space */
0x0000f0fe, 0x11f2f109, /* 11f2f109 //C 0f0fe   359 EQ jmp	tok_ret		; then return */
0x0000f0ff, 0x01f2f105, /* 01f2f105 //C 0f0ff   360 jmp	tok_next */
0x0000f100, 0x027b0000, /* 027b0000 //C 0f100   362 sz	r7 */
0x0000f101, 0x11f2f105, /* 11f2f105 //C 0f101   363 jz	tok_next	; still between words */
0x0000f102, 0x01720000, /* 01720000 //C 0f102   365 mvzl	r7,0		; in_word=false */
0x0000f103, 0x01120000, /* 01120000 //C 0f103   366 mvzl	r1,0		; put a 0 at the end of word */
0x0000f104, 0x08150100, /* 08150100 //C 0f104   367 st	r1,r5,r1 */
0x0000f105, 0x020b0000, /* 020b0000 //C 0f105   369 sz	r0		; check EOL */
0x0000f106, 0x11f2f109, /* 11f2f109 //C 0f106   370 jz	tok_ret		; jump out if char==0 */
0x0000f107, 0x01540001, /* 01540001 //C 0f107   371 add	r5,1 */
0x0000f108, 0x01f2f0f4, /* 01f2f0f4 //C 0f108   372 jmp	tok_cycle */
0x0000f109, 0x01120000, /* 01120000 //C 0f109   374 mvzl	r1,0 */
0x0000f10a, 0x01680005, /* 01680005 //C 0f10a   375 cmp	r6,MAX_WORDS */
0x0000f10b, 0x11f2f10e, /* 11f2f10e //C 0f10b   376 jz	tok_end */
0x0000f10c, 0x09168400, /* 09168400 //C 0f10c   377 st	r1,r6+,r4 */
0x0000f10d, 0x01f2f109, /* 01f2f109 //C 0f10d   378 jmp	tok_ret */
0x0000f10e, 0x0ffd0000, /* 0ffd0000 //C 0f10e   380 pop	pc */
0x0000f10f, 0x0ded0000, /* 0ded0000 //C 0f10f   389 push	lr */
0x0000f110, 0x0d1d0000, /* 0d1d0000 //C 0f110   390 push	r1 */
0x0000f111, 0x0d2d0000, /* 0d2d0000 //C 0f111   391 push	r2 */
0x0000f112, 0x0d3d0000, /* 0d3d0000 //C 0f112   392 push	r3 */
0x0000f113, 0x0dad0000, /* 0dad0000 //C 0f113   393 push	r10 */
0x0000f114, 0x0700f866, /* 0700f866 //C 0f114   394 ld	r0,words	; R0= 1st word of command */
0x0000f115, 0x020b0000, /* 020b0000 //C 0f115   395 sz	r0 */
0x0000f116, 0x11f2f146, /* 11f2f146 //C 0f116   396 jz	find_cmd_false */
0x0000f117, 0x0e100000, /* 0e100000 //C 0f117   399 ld	r1,r0		; 1st char of word1 */
0x0000f118, 0x0e200001, /* 0e200001 //C 0f118   400 ld	r2,r0,1		; 2nd char */
0x0000f119, 0x0e300002, /* 0e300002 //C 0f119   401 ld	r3,r0,2		; 3rd char */
0x0000f11a, 0x011fffdf, /* 011fffdf //C 0f11a   402 and	r1,0xffdf	; upcase 1st char */
0x0000f11b, 0x01180052, /* 01180052 //C 0f11b   403 cmp	r1,'R' */
0x0000f11c, 0x21f2f132, /* 21f2f132 //C 0f11c   404 jnz	find_not_rx */
0x0000f11d, 0x0128002f, /* 0128002f //C 0f11d   405 cmp	r2,'/'		; '0'-1 */
0x0000f11e, 0xa1f2f132, /* a1f2f132 //C 0f11e   406 LS jmp	find_not_rx */
0x0000f11f, 0x01280039, /* 01280039 //C 0f11f   407 cmp	r2,'9' */
0x0000f120, 0x91f2f132, /* 91f2f132 //C 0f120   408 HI jmp	find_not_rx */
0x0000f121, 0x023b0000, /* 023b0000 //C 0f121   409 sz	r3 */
0x0000f122, 0x11f2f12d, /* 11f2f12d //C 0f122   410 jz	find_rx_09 */
0x0000f123, 0x01280031, /* 01280031 //C 0f123   412 cmp	r2,'1'		; first char must be '1' */
0x0000f124, 0x21f2f132, /* 21f2f132 //C 0f124   413 jnz	find_not_rx */
0x0000f125, 0x0138002f, /* 0138002f //C 0f125   414 cmp	r3,'/'		; '0'-1 */
0x0000f126, 0xa1f2f132, /* a1f2f132 //C 0f126   415 LS jmp	find_not_rx */
0x0000f127, 0x01380035, /* 01380035 //C 0f127   416 cmp	r3,'5' */
0x0000f128, 0x91f2f132, /* 91f2f132 //C 0f128   417 HI jmp	find_not_rx */
0x0000f129, 0x01360030, /* 01360030 //C 0f129   418 sub	r3,'0' */
0x0000f12a, 0x0134000a, /* 0134000a //C 0f12a   419 add	r3,10 */
0x0000f12b, 0x0630f86d, /* 0630f86d //C 0f12b   420 st	r3,nuof_reg */
0x0000f12c, 0x01f2f12f, /* 01f2f12f //C 0f12c   421 jmp	find_rx */
0x0000f12d, 0x01260030, /* 01260030 //C 0f12d   423 sub	r2,'0' */
0x0000f12e, 0x0620f86d, /* 0620f86d //C 0f12e   424 st	r2,nuof_reg */
0x0000f12f, 0x0102f37c, /* 0102f37c //C 0f12f   426 mvzl	r0,cmd_rx */
0x0000f130, 0x020c0000, /* 020c0000 //C 0f130   427 sec */
0x0000f131, 0x01f2f151, /* 01f2f151 //C 0f131   428 jmp	find_cmd_ret */
0x0000f132, 0x01a2f8c4, /* 01a2f8c4 //C 0f132   431 mvzl	r10,commands */
0x0000f133, 0x0e2a0000, /* 0e2a0000 //C 0f133   433 ld	r2,r10		; R2= cmd addr */
0x0000f134, 0x022b0000, /* 022b0000 //C 0f134   434 sz	r2 */
0x0000f135, 0x11f2f146, /* 11f2f146 //C 0f135   435 jz	find_cmd_false */
0x0000f136, 0x01a40001, /* 01a40001 //C 0f136   436 add	r10,1 */
0x0000f137, 0x00100a00, /* 00100a00 //C 0f137   437 mov	r1,r10		; R1= cmd string */
0x0000f138, 0x01a40001, /* 01a40001 //C 0f138   440 add	r10,1 */
0x0000f139, 0x0e3a0000, /* 0e3a0000 //C 0f139   441 ld	r3,r10 */
0x0000f13a, 0x023b0000, /* 023b0000 //C 0f13a   442 sz	r3 */
0x0000f13b, 0x21f2f138, /* 21f2f138 //C 0f13b   443 jnz	find_cmd_fw */
0x0000f13c, 0x01a40001, /* 01a40001 //C 0f13c   444 add	r10,1 */
0x0000f13d, 0x0400f4e7, /* 0400f4e7 //C 0f13d   446 call	streq */
0x0000f13e, 0x41f2f133, /* 41f2f133 //C 0f13e   447 C0 jmp	find_cmd_cyc */
0x0000f13f, 0x00000200, /* 00000200 //C 0f13f   449 mov	r0,r2 */
0x0000f140, 0x020c0000, /* 020c0000 //C 0f140   450 sec */
0x0000f141, 0x01f2f151, /* 01f2f151 //C 0f141   451 jmp	find_cmd_ret */
0x0000f142, 0x0000002f, /* 0000002f //C 0f142   452 db	47 */
0x0000f143, 0x0000002f, /* 0000002f //C 0f143   452 db	47 */
0x0000f144, 0x00000043, /* 00000043 //C 0f144   452 db	67 */
0x0000f145, 0x00000000, /* 00000000 //C 0f145   452 db */
0x0000f146, 0x01040001, /* 01040001 //C 0f146   454 add	r0,1		; check second word */
0x0000f147, 0x0e100000, /* 0e100000 //C 0f147   455 ld	r1,r0		; for //C command */
0x0000f148, 0x021b0000, /* 021b0000 //C 0f148   456 sz	r1 */
0x0000f149, 0x11f2f14f, /* 11f2f14f //C 0f149   457 jz	find_cmd_very_false */
0x0000f14a, 0x0102f142, /* 0102f142 //C 0f14a   458 mvzl	r0,c_cmd_name */
0x0000f14b, 0x0400f4e7, /* 0400f4e7 //C 0f14b   459 call	streq */
0x0000f14c, 0x21f2f14f, /* 21f2f14f //C 0f14c   460 jnz	find_cmd_very_false */
0x0000f14d, 0x0122f204, /* 0122f204 //C 0f14d   461 mvzl	r2,cmd_c */
0x0000f14e, 0x01f2f13f, /* 01f2f13f //C 0f14e   462 jmp	find_cmd_true */
0x0000f14f, 0x020d0000, /* 020d0000 //C 0f14f   464 clc */
0x0000f150, 0x01020000, /* 01020000 //C 0f150   465 mvzl	r0,0 */
0x0000f151, 0x0fad0000, /* 0fad0000 //C 0f151   467 pop	r10 */
0x0000f152, 0x0f3d0000, /* 0f3d0000 //C 0f152   468 pop	r3 */
0x0000f153, 0x0f2d0000, /* 0f2d0000 //C 0f153   469 pop	r2 */
0x0000f154, 0x0f1d0000, /* 0f1d0000 //C 0f154   470 pop	r1 */
0x0000f155, 0x0ffd0000, /* 0ffd0000 //C 0f155   471 pop	pc */
0x0000f156, 0x0ded0000, /* 0ded0000 //C 0f156   476 push	lr */
0x0000f157, 0x0122f866, /* 0122f866 //C 0f157   477 mvzl	r2,words */
0x0000f158, 0x01020000, /* 01020000 //C 0f158   478 mvzl	r0,0 */
0x0000f159, 0x0e420001, /* 0e420001 //C 0f159   480 ld	r4,r2,1		; addr */
0x0000f15a, 0x0e520002, /* 0e520002 //C 0f15a   481 ld	r5,r2,2		; value */
0x0000f15b, 0x024b0000, /* 024b0000 //C 0f15b   482 sz 	r4 */
0x0000f15c, 0x11f2f182, /* 11f2f182 //C 0f15c   483 jz	m_ret */
0x0000f15d, 0x00000400, /* 00000400 //C 0f15d   485 mov	r0,r4 */
0x0000f15e, 0x0400f543, /* 0400f543 //C 0f15e   486 call	htoi */
0x0000f15f, 0x00400100, /* 00400100 //C 0f15f   487 mov	r4,r1 */
0x0000f160, 0x31f2f164, /* 31f2f164 //C 0f160   488 C1 jmp	m_addr_ok */
0x0000f161, 0x0102f183, /* 0102f183 //C 0f161   489 mvzl	r0,m_err_addr */
0x0000f162, 0x0400f6c2, /* 0400f6c2 //C 0f162   490 call	printsnl */
0x0000f163, 0x01f2f182, /* 01f2f182 //C 0f163   491 jmp	m_ret */
0x0000f164, 0x025b0000, /* 025b0000 //C 0f164   493 sz	r5 */
0x0000f165, 0x11f2f178, /* 11f2f178 //C 0f165   494 jz	m_read */
0x0000f166, 0x0132f000, /* 0132f000 //C 0f166   496 mvzl	r3,the_begin */
0x0000f167, 0x00380400, /* 00380400 //C 0f167   497 cmp	r3,r4 */
0x0000f168, 0x91f2f170, /* 91f2f170 //C 0f168   498 HI jmp	m_addrv_ok */
0x0000f169, 0x0132fb20, /* 0132fb20 //C 0f169   499 mvzl	r3,the_end */
0x0000f16a, 0x00380400, /* 00380400 //C 0f16a   500 cmp	r3,r4 */
0x0000f16b, 0x91f2f16d, /* 91f2f16d //C 0f16b   501 HI jmp	m_addrv_nok */
0x0000f16c, 0x01f2f170, /* 01f2f170 //C 0f16c   502 jmp	m_addrv_ok */
0x0000f16d, 0x0102f191, /* 0102f191 //C 0f16d   510 mvzl	r0,m_err_addrv */
0x0000f16e, 0x0400f6c2, /* 0400f6c2 //C 0f16e   511 call	printsnl */
0x0000f16f, 0x01f2f182, /* 01f2f182 //C 0f16f   512 jmp	m_ret */
0x0000f170, 0x00000500, /* 00000500 //C 0f170   514 mov	r0,r5 */
0x0000f171, 0x0400f543, /* 0400f543 //C 0f171   515 call	htoi */
0x0000f172, 0x00500100, /* 00500100 //C 0f172   516 mov	r5,r1 */
0x0000f173, 0x31f2f177, /* 31f2f177 //C 0f173   517 C1 jmp	m_value_ok */
0x0000f174, 0x0102f1a3, /* 0102f1a3 //C 0f174   518 mvzl	r0,m_err_value */
0x0000f175, 0x0400f6c2, /* 0400f6c2 //C 0f175   519 call	printsnl */
0x0000f176, 0x01f2f182, /* 01f2f182 //C 0f176   520 jmp	m_ret */
0x0000f177, 0x0c540000, /* 0c540000 //C 0f177   522 st	r5,r4 */
0x0000f178, 0x00000400, /* 00000400 //C 0f178   525 mov	r0,r4 */
0x0000f179, 0x01120004, /* 01120004 //C 0f179   526 mvzl	r1,4 */
0x0000f17a, 0x0400f6c9, /* 0400f6c9 //C 0f17a   527 call	print_vhex */
0x0000f17b, 0x01020020, /* 01020020 //C 0f17b   528 mvzl	r0,0x20 */
0x0000f17c, 0x0400f685, /* 0400f685 //C 0f17c   529 call	putchar */
0x0000f17d, 0x0e040000, /* 0e040000 //C 0f17d   530 ld	r0,r4 */
0x0000f17e, 0x01120004, /* 01120004 //C 0f17e   531 mvzl	r1,4 */
0x0000f17f, 0x0400f6c9, /* 0400f6c9 //C 0f17f   532 call	print_vhex */
0x0000f180, 0x0102000a, /* 0102000a //C 0f180   533 mvzl	r0,LF */
0x0000f181, 0x0400f685, /* 0400f685 //C 0f181   534 call	putchar */
0x0000f182, 0x0ffd0000, /* 0ffd0000 //C 0f182   536 pop	pc */
0x0000f183, 0x00000041, /* 00000041 //C 0f183   538 db	65 */
0x0000f184, 0x00000064, /* 00000064 //C 0f184   538 db	100 */
0x0000f185, 0x00000064, /* 00000064 //C 0f185   538 db	100 */
0x0000f186, 0x00000072, /* 00000072 //C 0f186   538 db	114 */
0x0000f187, 0x00000065, /* 00000065 //C 0f187   538 db	101 */
0x0000f188, 0x00000073, /* 00000073 //C 0f188   538 db	115 */
0x0000f189, 0x00000073, /* 00000073 //C 0f189   538 db	115 */
0x0000f18a, 0x00000020, /* 00000020 //C 0f18a   538 db	32 */
0x0000f18b, 0x00000065, /* 00000065 //C 0f18b   538 db	101 */
0x0000f18c, 0x00000072, /* 00000072 //C 0f18c   538 db	114 */
0x0000f18d, 0x00000072, /* 00000072 //C 0f18d   538 db	114 */
0x0000f18e, 0x0000006f, /* 0000006f //C 0f18e   538 db	111 */
0x0000f18f, 0x00000072, /* 00000072 //C 0f18f   538 db	114 */
0x0000f190, 0x00000000, /* 00000000 //C 0f190   538 db */
0x0000f191, 0x0000004d, /* 0000004d //C 0f191   539 db	77 */
0x0000f192, 0x0000006f, /* 0000006f //C 0f192   539 db	111 */
0x0000f193, 0x0000006e, /* 0000006e //C 0f193   539 db	110 */
0x0000f194, 0x00000069, /* 00000069 //C 0f194   539 db	105 */
0x0000f195, 0x00000074, /* 00000074 //C 0f195   539 db	116 */
0x0000f196, 0x0000006f, /* 0000006f //C 0f196   539 db	111 */
0x0000f197, 0x00000072, /* 00000072 //C 0f197   539 db	114 */
0x0000f198, 0x00000027, /* 00000027 //C 0f198   539 db	39 */
0x0000f199, 0x00000073, /* 00000073 //C 0f199   539 db	115 */
0x0000f19a, 0x00000020, /* 00000020 //C 0f19a   539 db	32 */
0x0000f19b, 0x00000061, /* 00000061 //C 0f19b   539 db	97 */
0x0000f19c, 0x00000064, /* 00000064 //C 0f19c   539 db	100 */
0x0000f19d, 0x00000064, /* 00000064 //C 0f19d   539 db	100 */
0x0000f19e, 0x00000072, /* 00000072 //C 0f19e   539 db	114 */
0x0000f19f, 0x00000065, /* 00000065 //C 0f19f   539 db	101 */
0x0000f1a0, 0x00000073, /* 00000073 //C 0f1a0   539 db	115 */
0x0000f1a1, 0x00000073, /* 00000073 //C 0f1a1   539 db	115 */
0x0000f1a2, 0x00000000, /* 00000000 //C 0f1a2   539 db */
0x0000f1a3, 0x00000056, /* 00000056 //C 0f1a3   540 db	86 */
0x0000f1a4, 0x00000061, /* 00000061 //C 0f1a4   540 db	97 */
0x0000f1a5, 0x0000006c, /* 0000006c //C 0f1a5   540 db	108 */
0x0000f1a6, 0x00000075, /* 00000075 //C 0f1a6   540 db	117 */
0x0000f1a7, 0x00000065, /* 00000065 //C 0f1a7   540 db	101 */
0x0000f1a8, 0x00000020, /* 00000020 //C 0f1a8   540 db	32 */
0x0000f1a9, 0x00000065, /* 00000065 //C 0f1a9   540 db	101 */
0x0000f1aa, 0x00000072, /* 00000072 //C 0f1aa   540 db	114 */
0x0000f1ab, 0x00000072, /* 00000072 //C 0f1ab   540 db	114 */
0x0000f1ac, 0x0000006f, /* 0000006f //C 0f1ac   540 db	111 */
0x0000f1ad, 0x00000072, /* 00000072 //C 0f1ad   540 db	114 */
0x0000f1ae, 0x00000000, /* 00000000 //C 0f1ae   540 db */
0x0000f1af, 0x0ded0000, /* 0ded0000 //C 0f1af   545 push	lr */
0x0000f1b0, 0x0122f866, /* 0122f866 //C 0f1b0   546 mvzl	r2,words */
0x0000f1b1, 0x01020000, /* 01020000 //C 0f1b1   547 mvzl	r0,0 */
0x0000f1b2, 0x0e420001, /* 0e420001 //C 0f1b2   549 ld	r4,r2,1		; addr */
0x0000f1b3, 0x0e520002, /* 0e520002 //C 0f1b3   550 ld	r5,r2,2		; value */
0x0000f1b4, 0x024b0000, /* 024b0000 //C 0f1b4   551 sz 	r4 */
0x0000f1b5, 0x11f2f182, /* 11f2f182 //C 0f1b5   552 jz	m_ret */
0x0000f1b6, 0x00000400, /* 00000400 //C 0f1b6   554 mov	r0,r4 */
0x0000f1b7, 0x0400f543, /* 0400f543 //C 0f1b7   555 call	htoi */
0x0000f1b8, 0x00400100, /* 00400100 //C 0f1b8   556 mov	r4,r1 */
0x0000f1b9, 0x31f2f1bd, /* 31f2f1bd //C 0f1b9   557 C1 jmp	sm_addr_ok */
0x0000f1ba, 0x0102f183, /* 0102f183 //C 0f1ba   558 mvzl	r0,m_err_addr */
0x0000f1bb, 0x0400f6c2, /* 0400f6c2 //C 0f1bb   559 call	printsnl */
0x0000f1bc, 0x01f2f1d1, /* 01f2f1d1 //C 0f1bc   560 jmp	sm_ret */
0x0000f1bd, 0x025b0000, /* 025b0000 //C 0f1bd   562 sz	r5 */
0x0000f1be, 0x11f2f1d1, /* 11f2f1d1 //C 0f1be   563 jz	sm_ret */
0x0000f1bf, 0x0132f000, /* 0132f000 //C 0f1bf   565 mvzl	r3,the_begin */
0x0000f1c0, 0x00380400, /* 00380400 //C 0f1c0   566 cmp	r3,r4 */
0x0000f1c1, 0x91f2f1c9, /* 91f2f1c9 //C 0f1c1   567 HI jmp	sm_addrv_ok */
0x0000f1c2, 0x0132fb20, /* 0132fb20 //C 0f1c2   568 mvzl	r3,the_end */
0x0000f1c3, 0x00380400, /* 00380400 //C 0f1c3   569 cmp	r3,r4 */
0x0000f1c4, 0x91f2f1c6, /* 91f2f1c6 //C 0f1c4   570 HI jmp	sm_addrv_nok */
0x0000f1c5, 0x01f2f1c9, /* 01f2f1c9 //C 0f1c5   571 jmp	sm_addrv_ok */
0x0000f1c6, 0x0102f191, /* 0102f191 //C 0f1c6   573 mvzl	r0,m_err_addrv */
0x0000f1c7, 0x0400f6c2, /* 0400f6c2 //C 0f1c7   574 call	printsnl */
0x0000f1c8, 0x01f2f1d1, /* 01f2f1d1 //C 0f1c8   575 jmp	sm_ret */
0x0000f1c9, 0x00000500, /* 00000500 //C 0f1c9   577 mov	r0,r5 */
0x0000f1ca, 0x0400f543, /* 0400f543 //C 0f1ca   578 call	htoi */
0x0000f1cb, 0x00500100, /* 00500100 //C 0f1cb   579 mov	r5,r1 */
0x0000f1cc, 0x31f2f1d0, /* 31f2f1d0 //C 0f1cc   580 C1 jmp	sm_value_ok */
0x0000f1cd, 0x0102f1a3, /* 0102f1a3 //C 0f1cd   581 mvzl	r0,m_err_value */
0x0000f1ce, 0x0400f6c2, /* 0400f6c2 //C 0f1ce   582 call	printsnl */
0x0000f1cf, 0x01f2f1d1, /* 01f2f1d1 //C 0f1cf   583 jmp	sm_ret */
0x0000f1d0, 0x0c540000, /* 0c540000 //C 0f1d0   585 st	r5,r4 */
0x0000f1d1, 0x0ffd0000, /* 0ffd0000 //C 0f1d1   588 pop	pc */
0x0000f1d2, 0x0ded0000, /* 0ded0000 //C 0f1d2   594 push	lr */
0x0000f1d3, 0x0122f866, /* 0122f866 //C 0f1d3   595 mvzl	r2,words */
0x0000f1d4, 0x0e020001, /* 0e020001 //C 0f1d4   596 ld	r0,r2,1		; start address */
0x0000f1d5, 0x0400f543, /* 0400f543 //C 0f1d5   597 call	htoi */
0x0000f1d6, 0x00300100, /* 00300100 //C 0f1d6   598 mov	r3,r1 */
0x0000f1d7, 0x0e020002, /* 0e020002 //C 0f1d7   599 ld	r0,r2,2		; end address */
0x0000f1d8, 0x020b0000, /* 020b0000 //C 0f1d8   600 sz	r0 */
0x0000f1d9, 0x21f2f1dd, /* 21f2f1dd //C 0f1d9   601 jnz	d_end_ok */
0x0000f1da, 0x00400300, /* 00400300 //C 0f1da   602 mov	r4,r3 */
0x0000f1db, 0x01440010, /* 01440010 //C 0f1db   603 add	r4,0x10 */
0x0000f1dc, 0x01f2f1df, /* 01f2f1df //C 0f1dc   604 jmp	d_chk_end */
0x0000f1dd, 0x0400f543, /* 0400f543 //C 0f1dd   606 call	htoi */
0x0000f1de, 0x00400100, /* 00400100 //C 0f1de   607 mov	r4,r1 */
0x0000f1df, 0x00380400, /* 00380400 //C 0f1df   609 cmp	r3,r4		; check if start>end */
0x0000f1e0, 0x91f2f1ef, /* 91f2f1ef //C 0f1e0   610 HI jmp d_bad */
0x0000f1e1, 0x00000300, /* 00000300 //C 0f1e1   618 mov	r0,r3		; print address */
0x0000f1e2, 0x01120004, /* 01120004 //C 0f1e2   619 mvzl	r1,4 */
0x0000f1e3, 0x0400f6c9, /* 0400f6c9 //C 0f1e3   620 call	print_vhex */
0x0000f1e4, 0x01020020, /* 01020020 //C 0f1e4   621 mvzl	r0,0x20		; print one space */
0x0000f1e5, 0x0400f685, /* 0400f685 //C 0f1e5   622 call	putchar */
0x0000f1e6, 0x0e030000, /* 0e030000 //C 0f1e6   623 ld	r0,r3		; load data */
0x0000f1e7, 0x01120004, /* 01120004 //C 0f1e7   624 mvzl	r1,4		; print data */
0x0000f1e8, 0x0400f6c9, /* 0400f6c9 //C 0f1e8   625 call	print_vhex */
0x0000f1e9, 0x0102000a, /* 0102000a //C 0f1e9   626 mvzl	r0,LF		; print new line */
0x0000f1ea, 0x0400f685, /* 0400f685 //C 0f1ea   627 call	putchar */
0x0000f1eb, 0x00380400, /* 00380400 //C 0f1eb   628 cmp	r3,r4 */
0x0000f1ec, 0x11f2f1f1, /* 11f2f1f1 //C 0f1ec   629 jz	d_ret */
0x0000f1ed, 0x01340001, /* 01340001 //C 0f1ed   630 add	r3,1 */
0x0000f1ee, 0x01f2f1e1, /* 01f2f1e1 //C 0f1ee   631 jmp	d_cyc */
0x0000f1ef, 0x0102f1f2, /* 0102f1f2 //C 0f1ef   633 mvzl	r0,d_err_bad */
0x0000f1f0, 0x0400f6c2, /* 0400f6c2 //C 0f1f0   634 call	printsnl */
0x0000f1f1, 0x0ffd0000, /* 0ffd0000 //C 0f1f1   636 pop	pc */
0x0000f1f2, 0x00000057, /* 00000057 //C 0f1f2   638 db	87 */
0x0000f1f3, 0x00000072, /* 00000072 //C 0f1f3   638 db	114 */
0x0000f1f4, 0x0000006f, /* 0000006f //C 0f1f4   638 db	111 */
0x0000f1f5, 0x0000006e, /* 0000006e //C 0f1f5   638 db	110 */
0x0000f1f6, 0x00000067, /* 00000067 //C 0f1f6   638 db	103 */
0x0000f1f7, 0x00000020, /* 00000020 //C 0f1f7   638 db	32 */
0x0000f1f8, 0x00000065, /* 00000065 //C 0f1f8   638 db	101 */
0x0000f1f9, 0x0000006e, /* 0000006e //C 0f1f9   638 db	110 */
0x0000f1fa, 0x00000064, /* 00000064 //C 0f1fa   638 db	100 */
0x0000f1fb, 0x00000020, /* 00000020 //C 0f1fb   638 db	32 */
0x0000f1fc, 0x00000061, /* 00000061 //C 0f1fc   638 db	97 */
0x0000f1fd, 0x00000064, /* 00000064 //C 0f1fd   638 db	100 */
0x0000f1fe, 0x00000064, /* 00000064 //C 0f1fe   638 db	100 */
0x0000f1ff, 0x00000072, /* 00000072 //C 0f1ff   638 db	114 */
0x0000f200, 0x00000065, /* 00000065 //C 0f200   638 db	101 */
0x0000f201, 0x00000073, /* 00000073 //C 0f201   638 db	115 */
0x0000f202, 0x00000073, /* 00000073 //C 0f202   638 db	115 */
0x0000f203, 0x00000000, /* 00000000 //C 0f203   638 db */
0x0000f204, 0x00f00e00, /* 00f00e00 //C 0f204   643 ret */
0x0000f205, 0x0ded0000, /* 0ded0000 //C 0f205   648 push	lr */
0x0000f206, 0x01a20000, /* 01a20000 //C 0f206   649 mvzl	r10,0		; state (nr of words) */
0x0000f207, 0x01820000, /* 01820000 //C 0f207   650 mvzl	r8,0		; value */
0x0000f208, 0x0162003f, /* 0162003f //C 0f208   651 mvzl	r6,'?'		; Record type */
0x0000f209, 0x01c20000, /* 01c20000 //C 0f209   652 mvzl	r12,0		; Checksum */
0x0000f20a, 0x0400f676, /* 0400f676 //C 0f20a   654 call	check_uart */
0x0000f20b, 0x41f2f20a, /* 41f2f20a //C 0f20b   655 C0 jmp	l_cyc */
0x0000f20c, 0x0400f67e, /* 0400f67e //C 0f20c   656 call	read */
0x0000f20d, 0x00b00000, /* 00b00000 //C 0f20d   658 mov	r11,r0		; Copy of char in R11 */
0x0000f20e, 0x0108000a, /* 0108000a //C 0f20e   659 cmp	r0,10		; check EOL chars */
0x0000f20f, 0x11f2f25b, /* 11f2f25b //C 0f20f   660 jz	l_eol */
0x0000f210, 0x0108000d, /* 0108000d //C 0f210   661 cmp	r0,13 */
0x0000f211, 0x11f2f25b, /* 11f2f25b //C 0f211   662 jz	l_eol */
0x0000f212, 0x01a80000, /* 01a80000 //C 0f212   664 cmp	r10,0 */
0x0000f213, 0x21f2f223, /* 21f2f223 //C 0f213   665 jnz	l_no0 */
0x0000f214, 0x0400f510, /* 0400f510 //C 0f214   667 call	hexchar2value */
0x0000f215, 0x41f2f21d, /* 41f2f21d //C 0f215   668 C0 jmp	l_eof_0 */
0x0000f216, 0x02880000, /* 02880000 //C 0f216   669 shl	r8		; shift val(char) into value */
0x0000f217, 0x02880000, /* 02880000 //C 0f217   670 shl	r8 */
0x0000f218, 0x02880000, /* 02880000 //C 0f218   671 shl	r8 */
0x0000f219, 0x02880000, /* 02880000 //C 0f219   672 shl	r8 */
0x0000f21a, 0x010b000f, /* 010b000f //C 0f21a   673 btst	r0,0xf */
0x0000f21b, 0x008d0000, /* 008d0000 //C 0f21b   674 or	r8,r0 */
0x0000f21c, 0x01f2f20a, /* 01f2f20a //C 0f21c   675 jmp	l_cyc */
0x0000f21d, 0x01a20001, /* 01a20001 //C 0f21d   677 mvzl	r10,1		; state0 -> state1 */
0x0000f21e, 0x0162003f, /* 0162003f //C 0f21e   678 mvzl	r6,'?'		; No //C yet */
0x0000f21f, 0x01b8002f, /* 01b8002f //C 0f21f   679 cmp	r11,'/'		; is it start of // */
0x0000f220, 0x11720001, /* 11720001 //C 0f220   680 z1 mvzl	r7,1		; Yes, first / found */
0x0000f221, 0x21720000, /* 21720000 //C 0f221   681 z0 mvzl	r7,0		; No '/' yet */
0x0000f222, 0x01f2f20a, /* 01f2f20a //C 0f222   682 jmp	l_cyc */
0x0000f223, 0x01a80001, /* 01a80001 //C 0f223   685 cmp	r10,1 */
0x0000f224, 0x21f2f23e, /* 21f2f23e //C 0f224   686 jnz	l_no1 */
0x0000f225, 0x027b0000, /* 027b0000 //C 0f225   688 sz	r7 */
0x0000f226, 0x21f2f22b, /* 21f2f22b //C 0f226   689 jnz	l_s1_2nd */
0x0000f227, 0x0108002f, /* 0108002f //C 0f227   691 cmp	r0,'/' */
0x0000f228, 0x21f2f20a, /* 21f2f20a //C 0f228   692 jnz	l_cyc */
0x0000f229, 0x01720001, /* 01720001 //C 0f229   693 mvzl	r7,1 */
0x0000f22a, 0x01f2f20a, /* 01f2f20a //C 0f22a   694 jmp	l_cyc */
0x0000f22b, 0x0108002f, /* 0108002f //C 0f22b   696 cmp	r0,'/' */
0x0000f22c, 0x11f2f20a, /* 11f2f20a //C 0f22c   697 jz	l_cyc */
0x0000f22d, 0x01080043, /* 01080043 //C 0f22d   698 cmp	r0,'C' */
0x0000f22e, 0x11f2f231, /* 11f2f231 //C 0f22e   699 jz	l_s1_C */
0x0000f22f, 0x01080049, /* 01080049 //C 0f22f   700 cmp	r0,'I' */
0x0000f230, 0x21f2f236, /* 21f2f236 //C 0f230   701 jnz	l_s1_noC */
0x0000f231, 0x00600000, /* 00600000 //C 0f231   704 mov	r6,r0		; record type is in r0, store in r6 */
0x0000f232, 0x01a20002, /* 01a20002 //C 0f232   706 mvzl	r10,2 */
0x0000f233, 0x01920000, /* 01920000 //C 0f233   707 mvzl	r9,0		; address= 0 */
0x0000f234, 0x01520000, /* 01520000 //C 0f234   708 mvzl	r5,0		; where we are in word: before */
0x0000f235, 0x01f2f20a, /* 01f2f20a //C 0f235   709 jmp	l_cyc */
0x0000f236, 0x01080045, /* 01080045 //C 0f236   711 cmp	r0,'E' */
0x0000f237, 0x21f2f23b, /* 21f2f23b //C 0f237   712 jnz	l_s1_noE */
0x0000f238, 0x01620045, /* 01620045 //C 0f238   714 mvzl	r6,'E' */
0x0000f239, 0x01a20003, /* 01a20003 //C 0f239   716 mvzl	r10,3 */
0x0000f23a, 0x01f2f20a, /* 01f2f20a //C 0f23a   717 jmp	l_cyc */
0x0000f23b, 0x0400f685, /* 0400f685 //C 0f23b   721 call	putchar		; print record type */
0x0000f23c, 0x01a2000f, /* 01a2000f //C 0f23c   722 mvzl	r10,0xf		; special state: skip everything */
0x0000f23d, 0x01f2f20a, /* 01f2f20a //C 0f23d   723 jmp	l_cyc */
0x0000f23e, 0x01a80002, /* 01a80002 //C 0f23e   726 cmp	r10,2 */
0x0000f23f, 0x21f2f253, /* 21f2f253 //C 0f23f   727 jnz	l_no2 */
0x0000f240, 0x01580000, /* 01580000 //C 0f240   729 cmp	r5,0 */
0x0000f241, 0x21f2f24c, /* 21f2f24c //C 0f241   730 jnz	l_s2_no0 */
0x0000f242, 0x0400f510, /* 0400f510 //C 0f242   732 call	hexchar2value */
0x0000f243, 0x41f2f20a, /* 41f2f20a //C 0f243   733 C0 jmp	l_cyc */
0x0000f244, 0x01520001, /* 01520001 //C 0f244   734 mvzl	r5,1 */
0x0000f245, 0x02980000, /* 02980000 //C 0f245   736 shl	r9 */
0x0000f246, 0x02980000, /* 02980000 //C 0f246   737 shl	r9 */
0x0000f247, 0x02980000, /* 02980000 //C 0f247   738 shl	r9 */
0x0000f248, 0x02980000, /* 02980000 //C 0f248   739 shl	r9 */
0x0000f249, 0x010b000f, /* 010b000f //C 0f249   740 btst	r0,0xf */
0x0000f24a, 0x009d0000, /* 009d0000 //C 0f24a   741 or	r9,r0 */
0x0000f24b, 0x01f2f20a, /* 01f2f20a //C 0f24b   743 jmp	l_cyc */
0x0000f24c, 0x01580001, /* 01580001 //C 0f24c   745 cmp	r5,1 */
0x0000f24d, 0x21f2f252, /* 21f2f252 //C 0f24d   746 jnz	l_s2_no1 */
0x0000f24e, 0x0400f510, /* 0400f510 //C 0f24e   748 call	hexchar2value */
0x0000f24f, 0x31f2f245, /* 31f2f245 //C 0f24f   749 C1 jmp	l_s2_got */
0x0000f250, 0x01520002, /* 01520002 //C 0f250   750 mvzl	r5,2 */
0x0000f251, 0x01f2f20a, /* 01f2f20a //C 0f251   751 jmp	l_cyc */
0x0000f252, 0x01f2f20a, /* 01f2f20a //C 0f252   753 jmp	l_cyc */
0x0000f253, 0x01a80003, /* 01a80003 //C 0f253   755 cmp	r10,3 */
0x0000f254, 0x21f2f256, /* 21f2f256 //C 0f254   756 jnz	l_no3 */
0x0000f255, 0x01f2f20a, /* 01f2f20a //C 0f255   758 jmp	l_cyc		; do nothing, just wait EOL */
0x0000f256, 0x01a8000f, /* 01a8000f //C 0f256   761 cmp	r10,0xf */
0x0000f257, 0x01f2f259, /* 01f2f259 //C 0f257   762 jmp	l_nof */
0x0000f258, 0x01f2f20a, /* 01f2f20a //C 0f258   763 jmp	l_cyc		; just skip */
0x0000f259, 0x01f2f20a, /* 01f2f20a //C 0f259   766 jmp	l_cyc */
0x0000f25a, 0x01f2f273, /* 01f2f273 //C 0f25a   767 jmp	l_ret */
0x0000f25b, 0x01a80000, /* 01a80000 //C 0f25b   771 cmp	r10,0		; in state0 */
0x0000f25c, 0x11f2f26f, /* 11f2f26f //C 0f25c   772 jz	l_back_to_0	; just restart */
0x0000f25d, 0x01a80001, /* 01a80001 //C 0f25d   773 cmp	r10,1		; in state1 */
0x0000f25e, 0x11f2f26f, /* 11f2f26f //C 0f25e   774 jz	l_back_to_0 	;l_cyc ;l_bad ; garbage */
0x0000f25f, 0x01a80002, /* 01a80002 //C 0f25f   775 cmp	r10,2		; in state2 */
0x0000f260, 0x11f2f267, /* 11f2f267 //C 0f260   776 jz	l_proc		; process record */
0x0000f261, 0x01a80003, /* 01a80003 //C 0f261   777 cmp	r10,3		; in state3 */
0x0000f262, 0x11f2f273, /* 11f2f273 //C 0f262   778 jz	l_ret		; eol in end record: finish */
0x0000f263, 0x01a8000f, /* 01a8000f //C 0f263   779 cmp	r10,0xf		; in state skip */
0x0000f264, 0x11f2f26f, /* 11f2f26f //C 0f264   780 jz	l_back_to_0	; reset state for new line */
0x0000f265, 0x01f2f20a, /* 01f2f20a //C 0f265   781 jmp	l_cyc */
0x0000f266, 0x01f2f273, /* 01f2f273 //C 0f266   783 jmp	l_ret */
0x0000f267, 0x01680043, /* 01680043 //C 0f267   785 cmp	r6,'C'		; is it a C or I record? */
0x0000f268, 0x1c890000, /* 1c890000 //C 0f268   786 z st	r8,r9		; then store */
0x0000f269, 0x10c40800, /* 10c40800 //C 0f269   787 z add	r12,r8		; and add to checksum */
0x0000f26a, 0x01680049, /* 01680049 //C 0f26a   788 cmp	r6,'I' */
0x0000f26b, 0x1c890000, /* 1c890000 //C 0f26b   789 z st	r8,r9 */
0x0000f26c, 0x10c40800, /* 10c40800 //C 0f26c   790 z add	r12,r8 */
0x0000f26d, 0x00000600, /* 00000600 //C 0f26d   791 mov	r0,r6		; echo record type */
0x0000f26e, 0x0400f685, /* 0400f685 //C 0f26e   792 call	putchar */
0x0000f26f, 0x01a20000, /* 01a20000 //C 0f26f   797 mvzl	r10,0 */
0x0000f270, 0x01820000, /* 01820000 //C 0f270   798 mvzl	r8,0 */
0x0000f271, 0x0162003f, /* 0162003f //C 0f271   799 mvzl	r6,'?' */
0x0000f272, 0x01f2f20a, /* 01f2f20a //C 0f272   800 jmp	l_cyc */
0x0000f273, 0x0102000a, /* 0102000a //C 0f273   802 mvzl	r0,LF */
0x0000f274, 0x0400f685, /* 0400f685 //C 0f274   803 call	putchar */
0x0000f275, 0x00000c00, /* 00000c00 //C 0f275   805 mov	r0,r12 */
0x0000f276, 0x01120004, /* 01120004 //C 0f276   806 mvzl	r1,4 */
0x0000f277, 0x0400f6c9, /* 0400f6c9 //C 0f277   807 call	print_vhex */
0x0000f278, 0x0102000a, /* 0102000a //C 0f278   808 mvzl	r0,LF */
0x0000f279, 0x0400f685, /* 0400f685 //C 0f279   809 call	putchar */
0x0000f27a, 0x0ffd0000, /* 0ffd0000 //C 0f27a   810 pop	pc */
0x0000f27b, 0x0ded0000, /* 0ded0000 //C 0f27b   816 push	lr */
0x0000f27c, 0x0122f866, /* 0122f866 //C 0f27c   817 mvzl	r2,words */
0x0000f27d, 0x0e020001, /* 0e020001 //C 0f27d   818 ld	r0,r2,1		; address */
0x0000f27e, 0x020b0000, /* 020b0000 //C 0f27e   819 sz	r0 */
0x0000f27f, 0x11f2f2a2, /* 11f2f2a2 //C 0f27f   820 jz	g_no_addr */
0x0000f280, 0x0400f543, /* 0400f543 //C 0f280   821 call	htoi */
0x0000f281, 0x00b00100, /* 00b00100 //C 0f281   822 mov	r11,r1 */
0x0000f282, 0x0102f2b5, /* 0102f2b5 //C 0f282   824 mvzl	r0,d_msg_run */
0x0000f283, 0x0400f68c, /* 0400f68c //C 0f283   825 call	prints */
0x0000f284, 0x00000b00, /* 00000b00 //C 0f284   826 mov	r0,r11 */
0x0000f285, 0x01120004, /* 01120004 //C 0f285   827 mvzl	r1,4 */
0x0000f286, 0x0400f6c9, /* 0400f6c9 //C 0f286   828 call	print_vhex */
0x0000f287, 0x0102000a, /* 0102000a //C 0f287   829 mvzl	r0,LF */
0x0000f288, 0x0400f685, /* 0400f685 //C 0f288   830 call	putchar */
0x0000f289, 0x06b0f87d, /* 06b0f87d //C 0f289   831 st	r11,reg15 */
0x0000f28a, 0x0122ff43, /* 0122ff43 //C 0f28a   833 mvzl	r2,UART_TSTAT */
0x0000f28b, 0x0e920000, /* 0e920000 //C 0f28b   835 ld	r9,r2 */
0x0000f28c, 0x019c0001, /* 019c0001 //C 0f28c   836 test	r9,1 */
0x0000f28d, 0x11f2f28b, /* 11f2f28b //C 0f28d   837 jz	g_wait_tc */
0x0000f28e, 0x07000000, /* 07000000 //C 0f28e   839 ld	r0,0 */
0x0000f28f, 0x0600f86c, /* 0600f86c //C 0f28f   840 st	r0,called */
0x0000f290, 0x0700f87e, /* 0700f87e //C 0f290   842 ld	r0,regf */
0x0000f291, 0x020f0000, /* 020f0000 //C 0f291   843 setf	r0 */
0x0000f292, 0x0700f86e, /* 0700f86e //C 0f292   844 ld	r0,reg0 */
0x0000f293, 0x0710f86f, /* 0710f86f //C 0f293   845 ld	r1,reg1 */
0x0000f294, 0x0720f870, /* 0720f870 //C 0f294   846 ld	r2,reg2 */
0x0000f295, 0x0730f871, /* 0730f871 //C 0f295   847 ld	r3,reg3 */
0x0000f296, 0x0740f872, /* 0740f872 //C 0f296   848 ld	r4,reg4 */
0x0000f297, 0x0750f873, /* 0750f873 //C 0f297   849 ld	r5,reg5 */
0x0000f298, 0x0760f874, /* 0760f874 //C 0f298   850 ld	r6,reg6 */
0x0000f299, 0x0770f875, /* 0770f875 //C 0f299   851 ld	r7,reg7 */
0x0000f29a, 0x0780f876, /* 0780f876 //C 0f29a   852 ld	r8,reg8 */
0x0000f29b, 0x0790f877, /* 0790f877 //C 0f29b   853 ld	r9,reg9 */
0x0000f29c, 0x07a0f878, /* 07a0f878 //C 0f29c   854 ld	r10,reg10 */
0x0000f29d, 0x07b0f879, /* 07b0f879 //C 0f29d   855 ld	r11,reg11 */
0x0000f29e, 0x07c0f87a, /* 07c0f87a //C 0f29e   856 ld	r12,reg12 */
0x0000f29f, 0x07d0f87b, /* 07d0f87b //C 0f29f   857 ld	r13,reg13 */
0x0000f2a0, 0x07e0f87c, /* 07e0f87c //C 0f2a0   858 ld	r14,reg14 */
0x0000f2a1, 0x07f0f87d, /* 07f0f87d //C 0f2a1   860 ld	r15,reg15 */
0x0000f2a2, 0x0700f86c, /* 0700f86c //C 0f2a2   863 ld	r0,called */
0x0000f2a3, 0x020b0000, /* 020b0000 //C 0f2a3   864 sz	r0 */
0x0000f2a4, 0x11f2f2a7, /* 11f2f2a7 //C 0f2a4   865 jz	g_err */
0x0000f2a5, 0x07b0f87d, /* 07b0f87d //C 0f2a5   866 ld	r11,reg15 */
0x0000f2a6, 0x01f2f282, /* 01f2f282 //C 0f2a6   867 jmp	g_go11 */
0x0000f2a7, 0x0102f2aa, /* 0102f2aa //C 0f2a7   869 mvzl	r0,g_err_addr */
0x0000f2a8, 0x0400f6c2, /* 0400f6c2 //C 0f2a8   870 call	printsnl */
0x0000f2a9, 0x0ffd0000, /* 0ffd0000 //C 0f2a9   872 pop	pc */
0x0000f2aa, 0x0000004e, /* 0000004e //C 0f2aa   874 db	78 */
0x0000f2ab, 0x0000006f, /* 0000006f //C 0f2ab   874 db	111 */
0x0000f2ac, 0x00000020, /* 00000020 //C 0f2ac   874 db	32 */
0x0000f2ad, 0x00000061, /* 00000061 //C 0f2ad   874 db	97 */
0x0000f2ae, 0x00000064, /* 00000064 //C 0f2ae   874 db	100 */
0x0000f2af, 0x00000064, /* 00000064 //C 0f2af   874 db	100 */
0x0000f2b0, 0x00000072, /* 00000072 //C 0f2b0   874 db	114 */
0x0000f2b1, 0x00000065, /* 00000065 //C 0f2b1   874 db	101 */
0x0000f2b2, 0x00000073, /* 00000073 //C 0f2b2   874 db	115 */
0x0000f2b3, 0x00000073, /* 00000073 //C 0f2b3   874 db	115 */
0x0000f2b4, 0x00000000, /* 00000000 //C 0f2b4   874 db */
0x0000f2b5, 0x00000052, /* 00000052 //C 0f2b5   875 db	82 */
0x0000f2b6, 0x00000075, /* 00000075 //C 0f2b6   875 db	117 */
0x0000f2b7, 0x0000006e, /* 0000006e //C 0f2b7   875 db	110 */
0x0000f2b8, 0x00000020, /* 00000020 //C 0f2b8   875 db	32 */
0x0000f2b9, 0x00000000, /* 00000000 //C 0f2b9   875 db */
0x0000f2ba, 0x0ded0000, /* 0ded0000 //C 0f2ba   880 push	lr */
0x0000f2bb, 0x0122f921, /* 0122f921 //C 0f2bb   881 mvzl	r2,helps */
0x0000f2bc, 0x01320000, /* 01320000 //C 0f2bc   882 mvzl	r3,0 */
0x0000f2bd, 0x0b038200, /* 0b038200 //C 0f2bd   884 ld	r0,r3+,r2	; pick a char */
0x0000f2be, 0x020b0000, /* 020b0000 //C 0f2be   885 sz	r0		; is it eos? */
0x0000f2bf, 0x21f2f2c3, /* 21f2f2c3 //C 0f2bf   886 jnz	h_print */
0x0000f2c0, 0x0b038200, /* 0b038200 //C 0f2c0   889 ld	r0,r3+,r2	; get first char of next string */
0x0000f2c1, 0x020b0000, /* 020b0000 //C 0f2c1   890 sz	r0 */
0x0000f2c2, 0x11f2f2c5, /* 11f2f2c5 //C 0f2c2   891 jz	h_eof */
0x0000f2c3, 0x0400f685, /* 0400f685 //C 0f2c3   893 call	putchar */
0x0000f2c4, 0x01f2f2bd, /* 01f2f2bd //C 0f2c4   894 jmp	h_cyc */
0x0000f2c5, 0x0ffd0000, /* 0ffd0000 //C 0f2c5   896 pop	pc */
0x0000f2c6, 0x0ded0000, /* 0ded0000 //C 0f2c6   902 push	lr */
0x0000f2c7, 0x0d1d0000, /* 0d1d0000 //C 0f2c7   903 push	r1 */
0x0000f2c8, 0x00100000, /* 00100000 //C 0f2c8   904 mov	r1,r0 */
0x0000f2c9, 0x01180010, /* 01180010 //C 0f2c9   905 cmp	r1,16		; go out if nr>16 */
0x0000f2ca, 0x91f2f2fe, /* 91f2f2fe //C 0f2ca   906 HI jmp	prn_ret */
0x0000f2cb, 0x0118000f, /* 0118000f //C 0f2cb   907 cmp	r1,15		; nr=Flag? */
0x0000f2cc, 0xa1f2f2d3, /* a1f2f2d3 //C 0f2cc   908 LS jmp	prn_015 */
0x0000f2cd, 0x01020046, /* 01020046 //C 0f2cd   910 mvzl	r0,'F' */
0x0000f2ce, 0x0400f685, /* 0400f685 //C 0f2ce   911 call	putchar */
0x0000f2cf, 0x01020020, /* 01020020 //C 0f2cf   912 mvzl	r0,32 */
0x0000f2d0, 0x0400f685, /* 0400f685 //C 0f2d0   913 call	putchar */
0x0000f2d1, 0x0400f685, /* 0400f685 //C 0f2d1   914 call	putchar */
0x0000f2d2, 0x01f2f2fe, /* 01f2f2fe //C 0f2d2   915 jmp	prn_ret */
0x0000f2d3, 0x0118000f, /* 0118000f //C 0f2d3   917 cmp	r1,15 */
0x0000f2d4, 0x21f2f2dc, /* 21f2f2dc //C 0f2d4   918 jnz	prn_no15 */
0x0000f2d5, 0x0102f2d8, /* 0102f2d8 //C 0f2d5   919 mvzl	r0,prn_PC */
0x0000f2d6, 0x0400f68c, /* 0400f68c //C 0f2d6   920 call	prints */
0x0000f2d7, 0x01f2f2fe, /* 01f2f2fe //C 0f2d7   921 jmp	prn_ret */
0x0000f2d8, 0x00000050, /* 00000050 //C 0f2d8   922 db	80 */
0x0000f2d9, 0x00000043, /* 00000043 //C 0f2d9   922 db	67 */
0x0000f2da, 0x00000020, /* 00000020 //C 0f2da   922 db	32 */
0x0000f2db, 0x00000000, /* 00000000 //C 0f2db   922 db */
0x0000f2dc, 0x0118000e, /* 0118000e //C 0f2dc   924 cmp	r1,14 */
0x0000f2dd, 0x21f2f2e5, /* 21f2f2e5 //C 0f2dd   925 jnz	prn_no14 */
0x0000f2de, 0x0102f2e1, /* 0102f2e1 //C 0f2de   926 mvzl	r0,prn_LR */
0x0000f2df, 0x0400f68c, /* 0400f68c //C 0f2df   927 call	prints */
0x0000f2e0, 0x01f2f2fe, /* 01f2f2fe //C 0f2e0   928 jmp	prn_ret */
0x0000f2e1, 0x0000004c, /* 0000004c //C 0f2e1   929 db	76 */
0x0000f2e2, 0x00000052, /* 00000052 //C 0f2e2   929 db	82 */
0x0000f2e3, 0x00000020, /* 00000020 //C 0f2e3   929 db	32 */
0x0000f2e4, 0x00000000, /* 00000000 //C 0f2e4   929 db */
0x0000f2e5, 0x0118000d, /* 0118000d //C 0f2e5   931 cmp	r1,13 */
0x0000f2e6, 0x21f2f2ee, /* 21f2f2ee //C 0f2e6   932 jnz	prn_no13 */
0x0000f2e7, 0x0102f2ea, /* 0102f2ea //C 0f2e7   933 mvzl	r0,prn_SP */
0x0000f2e8, 0x0400f68c, /* 0400f68c //C 0f2e8   934 call	prints */
0x0000f2e9, 0x01f2f2fe, /* 01f2f2fe //C 0f2e9   935 jmp	prn_ret */
0x0000f2ea, 0x00000053, /* 00000053 //C 0f2ea   936 db	83 */
0x0000f2eb, 0x00000050, /* 00000050 //C 0f2eb   936 db	80 */
0x0000f2ec, 0x00000020, /* 00000020 //C 0f2ec   936 db	32 */
0x0000f2ed, 0x00000000, /* 00000000 //C 0f2ed   936 db */
0x0000f2ee, 0x01020052, /* 01020052 //C 0f2ee   938 mvzl	r0,'R' */
0x0000f2ef, 0x0400f685, /* 0400f685 //C 0f2ef   939 call	putchar */
0x0000f2f0, 0x01180009, /* 01180009 //C 0f2f0   940 cmp	r1,9 */
0x0000f2f1, 0x91f2f2f8, /* 91f2f2f8 //C 0f2f1   941 HI jmp	prn_1015 */
0x0000f2f2, 0x00000100, /* 00000100 //C 0f2f2   943 mov	r0,r1 */
0x0000f2f3, 0x01040030, /* 01040030 //C 0f2f3   944 add	r0,'0' */
0x0000f2f4, 0x0400f685, /* 0400f685 //C 0f2f4   945 call	putchar */
0x0000f2f5, 0x01020020, /* 01020020 //C 0f2f5   946 mvzl	r0,32 */
0x0000f2f6, 0x0400f685, /* 0400f685 //C 0f2f6   947 call	putchar */
0x0000f2f7, 0x01f2f2fe, /* 01f2f2fe //C 0f2f7   948 jmp	prn_ret */
0x0000f2f8, 0x01020031, /* 01020031 //C 0f2f8   950 mvzl	r0,'1' */
0x0000f2f9, 0x0400f685, /* 0400f685 //C 0f2f9   951 call	putchar */
0x0000f2fa, 0x00000100, /* 00000100 //C 0f2fa   952 mov	r0,r1 */
0x0000f2fb, 0x0106000a, /* 0106000a //C 0f2fb   953 sub	r0,10 */
0x0000f2fc, 0x01040030, /* 01040030 //C 0f2fc   954 add	r0,'0' */
0x0000f2fd, 0x0400f685, /* 0400f685 //C 0f2fd   955 call	putchar */
0x0000f2fe, 0x0f1d0000, /* 0f1d0000 //C 0f2fe   957 pop	r1 */
0x0000f2ff, 0x0ffd0000, /* 0ffd0000 //C 0f2ff   958 pop	pc */
0x0000f300, 0x0ded0000, /* 0ded0000 //C 0f300   963 push	lr */
0x0000f301, 0x0d1d0000, /* 0d1d0000 //C 0f301   964 push	r1 */
0x0000f302, 0x01080010, /* 01080010 //C 0f302   965 cmp	r0,16 */
0x0000f303, 0x91f2f308, /* 91f2f308 //C 0f303   966 HI jmp	prv_ret */
0x0000f304, 0x0112f86e, /* 0112f86e //C 0f304   967 mvzl	r1,reg0 */
0x0000f305, 0x0a010000, /* 0a010000 //C 0f305   968 ld	r0,r1,r0 */
0x0000f306, 0x01120004, /* 01120004 //C 0f306   969 mvzl	r1,4 */
0x0000f307, 0x0400f6c9, /* 0400f6c9 //C 0f307   970 call	print_vhex */
0x0000f308, 0x0f1d0000, /* 0f1d0000 //C 0f308   972 pop	r1 */
0x0000f309, 0x0ffd0000, /* 0ffd0000 //C 0f309   973 pop	pc */
0x0000f30a, 0x0ded0000, /* 0ded0000 //C 0f30a   980 push	lr */
0x0000f30b, 0x0dad0000, /* 0dad0000 //C 0f30b   981 push	r10 */
0x0000f30c, 0x0d0d0000, /* 0d0d0000 //C 0f30c   982 push	r0 */
0x0000f30d, 0x00000a00, /* 00000a00 //C 0f30d   983 mov	r0,r10 */
0x0000f30e, 0x0400f2c6, /* 0400f2c6 //C 0f30e   984 call	print_reg_name */
0x0000f30f, 0x01020020, /* 01020020 //C 0f30f   985 mvzl	r0,32 */
0x0000f310, 0x0400f685, /* 0400f685 //C 0f310   986 call	putchar */
0x0000f311, 0x00000a00, /* 00000a00 //C 0f311   987 mov	r0,r10 */
0x0000f312, 0x0400f300, /* 0400f300 //C 0f312   988 call	print_reg_value */
0x0000f313, 0x0102000a, /* 0102000a //C 0f313   989 mvzl	r0,LF */
0x0000f314, 0x0400f685, /* 0400f685 //C 0f314   990 call	putchar */
0x0000f315, 0x0f0d0000, /* 0f0d0000 //C 0f315   991 pop	r0 */
0x0000f316, 0x0fad0000, /* 0fad0000 //C 0f316   992 pop	r10 */
0x0000f317, 0x0ffd0000, /* 0ffd0000 //C 0f317   993 pop	pc */
0x0000f318, 0x0ded0000, /* 0ded0000 //C 0f318  1001 push	lr */
0x0000f319, 0x0400f685, /* 0400f685 //C 0f319  1002 call	putchar */
0x0000f31a, 0x001c0200, /* 001c0200 //C 0f31a  1003 test	r1,r2 */
0x0000f31b, 0x21020031, /* 21020031 //C 0f31b  1004 NZ mvzl r0,'1' */
0x0000f31c, 0x11020030, /* 11020030 //C 0f31c  1005 Z  mvzl r0,'0' */
0x0000f31d, 0x0400f685, /* 0400f685 //C 0f31d  1006 call	putchar */
0x0000f31e, 0x00000300, /* 00000300 //C 0f31e  1007 mov	r0,r3 */
0x0000f31f, 0x0400f685, /* 0400f685 //C 0f31f  1008 call	putchar */
0x0000f320, 0x0ffd0000, /* 0ffd0000 //C 0f320  1009 pop	pc */
0x0000f321, 0x0ded0000, /* 0ded0000 //C 0f321  1014 push	lr */
0x0000f322, 0x0700f86c, /* 0700f86c //C 0f322  1015 ld	r0,called */
0x0000f323, 0x020b0000, /* 020b0000 //C 0f323  1016 sz	r0 */
0x0000f324, 0x11f2f32d, /* 11f2f32d //C 0f324  1017 jz	r_not_called */
0x0000f325, 0x0102f34a, /* 0102f34a //C 0f325  1019 mvzl	r0,msg_r_called */
0x0000f326, 0x0400f68c, /* 0400f68c //C 0f326  1020 call	prints */
0x0000f327, 0x0700f87d, /* 0700f87d //C 0f327  1021 ld	r0,reg15 */
0x0000f328, 0x01120004, /* 01120004 //C 0f328  1022 mvzl	r1,4 */
0x0000f329, 0x0400f6c9, /* 0400f6c9 //C 0f329  1023 call	print_vhex */
0x0000f32a, 0x0102000a, /* 0102000a //C 0f32a  1024 mvzl	r0,LF */
0x0000f32b, 0x0400f685, /* 0400f685 //C 0f32b  1025 call	putchar */
0x0000f32c, 0x01f2f32f, /* 01f2f32f //C 0f32c  1026 jmp	r_start */
0x0000f32d, 0x0102f360, /* 0102f360 //C 0f32d  1028 mvzl	r0,msg_r_notcalled */
0x0000f32e, 0x0400f68c, /* 0400f68c //C 0f32e  1029 call	prints */
0x0000f32f, 0x01a20000, /* 01a20000 //C 0f32f  1031 mvzl	r10,0 */
0x0000f330, 0x0400f30a, /* 0400f30a //C 0f330  1033 call	print_reg_name_value */
0x0000f331, 0x01a40001, /* 01a40001 //C 0f331  1034 add	r10,1 */
0x0000f332, 0x01a80011, /* 01a80011 //C 0f332  1035 cmp	r10,17 */
0x0000f333, 0x21f2f330, /* 21f2f330 //C 0f333  1036 jnz	r_cyc */
0x0000f334, 0x0710f87e, /* 0710f87e //C 0f334  1038 ld	r1,regf */
0x0000f335, 0x01320020, /* 01320020 //C 0f335  1039 mvzl	r3,32 */
0x0000f336, 0x01020055, /* 01020055 //C 0f336  1040 mvzl	r0,'U' */
0x0000f337, 0x01220020, /* 01220020 //C 0f337  1041 mvzl	r2,0x20 */
0x0000f338, 0x0400f318, /* 0400f318 //C 0f338  1042 call	print_flag */
0x0000f339, 0x01020050, /* 01020050 //C 0f339  1043 mvzl	r0,'P' */
0x0000f33a, 0x01220010, /* 01220010 //C 0f33a  1044 mvzl	r2,0x10 */
0x0000f33b, 0x0400f318, /* 0400f318 //C 0f33b  1045 call	print_flag */
0x0000f33c, 0x0102004f, /* 0102004f //C 0f33c  1046 mvzl	r0,'O' */
0x0000f33d, 0x01220008, /* 01220008 //C 0f33d  1047 mvzl	r2,8 */
0x0000f33e, 0x0400f318, /* 0400f318 //C 0f33e  1048 call	print_flag */
0x0000f33f, 0x0102005a, /* 0102005a //C 0f33f  1049 mvzl	r0,'Z' */
0x0000f340, 0x01220004, /* 01220004 //C 0f340  1050 mvzl	r2,4 */
0x0000f341, 0x0400f318, /* 0400f318 //C 0f341  1051 call	print_flag */
0x0000f342, 0x01020043, /* 01020043 //C 0f342  1052 mvzl	r0,'C' */
0x0000f343, 0x01220002, /* 01220002 //C 0f343  1053 mvzl	r2,2 */
0x0000f344, 0x0400f318, /* 0400f318 //C 0f344  1054 call	print_flag */
0x0000f345, 0x01020053, /* 01020053 //C 0f345  1055 mvzl	r0,'S' */
0x0000f346, 0x01220001, /* 01220001 //C 0f346  1056 mvzl	r2,1 */
0x0000f347, 0x0132000a, /* 0132000a //C 0f347  1057 mvzl	r3,LF */
0x0000f348, 0x0400f318, /* 0400f318 //C 0f348  1058 call	print_flag */
0x0000f349, 0x0ffd0000, /* 0ffd0000 //C 0f349  1059 pop	pc */
0x0000f34a, 0x0000004d, /* 0000004d //C 0f34a  1061 db	77 */
0x0000f34b, 0x0000006f, /* 0000006f //C 0f34b  1061 db	111 */
0x0000f34c, 0x0000006e, /* 0000006e //C 0f34c  1061 db	110 */
0x0000f34d, 0x00000069, /* 00000069 //C 0f34d  1061 db	105 */
0x0000f34e, 0x00000074, /* 00000074 //C 0f34e  1061 db	116 */
0x0000f34f, 0x0000006f, /* 0000006f //C 0f34f  1061 db	111 */
0x0000f350, 0x00000072, /* 00000072 //C 0f350  1061 db	114 */
0x0000f351, 0x00000020, /* 00000020 //C 0f351  1061 db	32 */
0x0000f352, 0x00000063, /* 00000063 //C 0f352  1061 db	99 */
0x0000f353, 0x00000061, /* 00000061 //C 0f353  1061 db	97 */
0x0000f354, 0x0000006c, /* 0000006c //C 0f354  1061 db	108 */
0x0000f355, 0x0000006c, /* 0000006c //C 0f355  1061 db	108 */
0x0000f356, 0x00000065, /* 00000065 //C 0f356  1061 db	101 */
0x0000f357, 0x00000064, /* 00000064 //C 0f357  1061 db	100 */
0x0000f358, 0x00000020, /* 00000020 //C 0f358  1061 db	32 */
0x0000f359, 0x00000066, /* 00000066 //C 0f359  1061 db	102 */
0x0000f35a, 0x00000072, /* 00000072 //C 0f35a  1061 db	114 */
0x0000f35b, 0x0000006f, /* 0000006f //C 0f35b  1061 db	111 */
0x0000f35c, 0x0000006d, /* 0000006d //C 0f35c  1061 db	109 */
0x0000f35d, 0x0000003a, /* 0000003a //C 0f35d  1061 db	58 */
0x0000f35e, 0x00000020, /* 00000020 //C 0f35e  1061 db	32 */
0x0000f35f, 0x00000000, /* 00000000 //C 0f35f  1061 db */
0x0000f360, 0x0000004d, /* 0000004d //C 0f360  1062 db	77 */
0x0000f361, 0x0000006f, /* 0000006f //C 0f361  1062 db	111 */
0x0000f362, 0x0000006e, /* 0000006e //C 0f362  1062 db	110 */
0x0000f363, 0x00000069, /* 00000069 //C 0f363  1062 db	105 */
0x0000f364, 0x00000074, /* 00000074 //C 0f364  1062 db	116 */
0x0000f365, 0x0000006f, /* 0000006f //C 0f365  1062 db	111 */
0x0000f366, 0x00000072, /* 00000072 //C 0f366  1062 db	114 */
0x0000f367, 0x00000020, /* 00000020 //C 0f367  1062 db	32 */
0x0000f368, 0x0000006e, /* 0000006e //C 0f368  1062 db	110 */
0x0000f369, 0x0000006f, /* 0000006f //C 0f369  1062 db	111 */
0x0000f36a, 0x00000074, /* 00000074 //C 0f36a  1062 db	116 */
0x0000f36b, 0x00000020, /* 00000020 //C 0f36b  1062 db	32 */
0x0000f36c, 0x00000063, /* 00000063 //C 0f36c  1062 db	99 */
0x0000f36d, 0x00000061, /* 00000061 //C 0f36d  1062 db	97 */
0x0000f36e, 0x0000006c, /* 0000006c //C 0f36e  1062 db	108 */
0x0000f36f, 0x0000006c, /* 0000006c //C 0f36f  1062 db	108 */
0x0000f370, 0x00000065, /* 00000065 //C 0f370  1062 db	101 */
0x0000f371, 0x00000064, /* 00000064 //C 0f371  1062 db	100 */
0x0000f372, 0x00000020, /* 00000020 //C 0f372  1062 db	32 */
0x0000f373, 0x00000062, /* 00000062 //C 0f373  1062 db	98 */
0x0000f374, 0x00000079, /* 00000079 //C 0f374  1062 db	121 */
0x0000f375, 0x00000020, /* 00000020 //C 0f375  1062 db	32 */
0x0000f376, 0x00000075, /* 00000075 //C 0f376  1062 db	117 */
0x0000f377, 0x00000073, /* 00000073 //C 0f377  1062 db	115 */
0x0000f378, 0x00000065, /* 00000065 //C 0f378  1062 db	101 */
0x0000f379, 0x00000072, /* 00000072 //C 0f379  1062 db	114 */
0x0000f37a, 0x0000000a, /* 0000000a //C 0f37a  1062 db	10 */
0x0000f37b, 0x00000000, /* 00000000 //C 0f37b  1062 db */
0x0000f37c, 0x0ded0000, /* 0ded0000 //C 0f37c  1067 push	lr */
0x0000f37d, 0x07a0f86d, /* 07a0f86d //C 0f37d  1068 ld	r10,nuof_reg	; Reg num is in R10 */
0x0000f37e, 0x01a80010, /* 01a80010 //C 0f37e  1069 cmp	r10,16 */
0x0000f37f, 0xa1f2f394, /* a1f2f394 //C 0f37f  1070 LS jmp	rx_nr_ok */
0x0000f380, 0x0102f383, /* 0102f383 //C 0f380  1071 mvzl	r0,rx_err_nr */
0x0000f381, 0x0400f6c2, /* 0400f6c2 //C 0f381  1072 call	printsnl */
0x0000f382, 0x01f2f3b0, /* 01f2f3b0 //C 0f382  1073 jmp	rx_ret */
0x0000f383, 0x0000004e, /* 0000004e //C 0f383  1075 db	78 */
0x0000f384, 0x0000006f, /* 0000006f //C 0f384  1075 db	111 */
0x0000f385, 0x00000020, /* 00000020 //C 0f385  1075 db	32 */
0x0000f386, 0x00000073, /* 00000073 //C 0f386  1075 db	115 */
0x0000f387, 0x00000075, /* 00000075 //C 0f387  1075 db	117 */
0x0000f388, 0x00000063, /* 00000063 //C 0f388  1075 db	99 */
0x0000f389, 0x00000068, /* 00000068 //C 0f389  1075 db	104 */
0x0000f38a, 0x00000020, /* 00000020 //C 0f38a  1075 db	32 */
0x0000f38b, 0x00000072, /* 00000072 //C 0f38b  1075 db	114 */
0x0000f38c, 0x00000065, /* 00000065 //C 0f38c  1075 db	101 */
0x0000f38d, 0x00000067, /* 00000067 //C 0f38d  1075 db	103 */
0x0000f38e, 0x00000069, /* 00000069 //C 0f38e  1075 db	105 */
0x0000f38f, 0x00000073, /* 00000073 //C 0f38f  1075 db	115 */
0x0000f390, 0x00000074, /* 00000074 //C 0f390  1075 db	116 */
0x0000f391, 0x00000065, /* 00000065 //C 0f391  1075 db	101 */
0x0000f392, 0x00000072, /* 00000072 //C 0f392  1075 db	114 */
0x0000f393, 0x00000000, /* 00000000 //C 0f393  1075 db */
0x0000f394, 0x0122f866, /* 0122f866 //C 0f394  1077 mvzl	r2,words */
0x0000f395, 0x0e420001, /* 0e420001 //C 0f395  1078 ld	r4,r2,1		; get aof first parameter */
0x0000f396, 0x024b0000, /* 024b0000 //C 0f396  1079 sz	r4		; is it NULL? */
0x0000f397, 0x11f2f3af, /* 11f2f3af //C 0f397  1080 jz	rx_print */
0x0000f398, 0x00000400, /* 00000400 //C 0f398  1081 mov	r0,r4 */
0x0000f399, 0x0400f543, /* 0400f543 //C 0f399  1082 call	htoi */
0x0000f39a, 0x00500100, /* 00500100 //C 0f39a  1083 mov	r5,r1		; Value is in R5 */
0x0000f39b, 0x31f2f3ab, /* 31f2f3ab //C 0f39b  1084 C1 jmp	rx_val_ok */
0x0000f39c, 0x0102f39f, /* 0102f39f //C 0f39c  1085 mvzl	r0,rx_err_val */
0x0000f39d, 0x0400f6c2, /* 0400f6c2 //C 0f39d  1086 call	printsnl */
0x0000f39e, 0x01f2f3b0, /* 01f2f3b0 //C 0f39e  1087 jmp	rx_ret */
0x0000f39f, 0x00000056, /* 00000056 //C 0f39f  1089 db	86 */
0x0000f3a0, 0x00000061, /* 00000061 //C 0f3a0  1089 db	97 */
0x0000f3a1, 0x0000006c, /* 0000006c //C 0f3a1  1089 db	108 */
0x0000f3a2, 0x00000075, /* 00000075 //C 0f3a2  1089 db	117 */
0x0000f3a3, 0x00000065, /* 00000065 //C 0f3a3  1089 db	101 */
0x0000f3a4, 0x00000020, /* 00000020 //C 0f3a4  1089 db	32 */
0x0000f3a5, 0x00000065, /* 00000065 //C 0f3a5  1089 db	101 */
0x0000f3a6, 0x00000072, /* 00000072 //C 0f3a6  1089 db	114 */
0x0000f3a7, 0x00000072, /* 00000072 //C 0f3a7  1089 db	114 */
0x0000f3a8, 0x0000006f, /* 0000006f //C 0f3a8  1089 db	111 */
0x0000f3a9, 0x00000072, /* 00000072 //C 0f3a9  1089 db	114 */
0x0000f3aa, 0x00000000, /* 00000000 //C 0f3aa  1089 db */
0x0000f3ab, 0x01a80010, /* 01a80010 //C 0f3ab  1091 cmp	r10,16		; Flag reg? */
0x0000f3ac, 0x115f003f, /* 115f003f //C 0f3ac  1092 EQ and	r5,0x3f */
0x0000f3ad, 0x0102f86e, /* 0102f86e //C 0f3ad  1093 mvzl	r0,reg0 */
0x0000f3ae, 0x08500a00, /* 08500a00 //C 0f3ae  1094 st	r5,r0,r10 */
0x0000f3af, 0x0400f30a, /* 0400f30a //C 0f3af  1096 call	print_reg_name_value */
0x0000f3b0, 0x0ffd0000, /* 0ffd0000 //C 0f3b0  1098 pop	pc */
0x0000f3b1, 0x0102000d, /* 0102000d //C 0f3b1  1104 mvzl	r0,13 */
0x0000f3b2, 0x0600f86d, /* 0600f86d //C 0f3b2  1105 st	r0,nuof_reg */
0x0000f3b3, 0x01f2f37c, /* 01f2f37c //C 0f3b3  1106 jmp	cmd_rx */
0x0000f3b4, 0x0102000e, /* 0102000e //C 0f3b4  1111 mvzl	r0,14 */
0x0000f3b5, 0x0600f86d, /* 0600f86d //C 0f3b5  1112 st	r0,nuof_reg */
0x0000f3b6, 0x01f2f37c, /* 01f2f37c //C 0f3b6  1113 jmp	cmd_rx */
0x0000f3b7, 0x0102000f, /* 0102000f //C 0f3b7  1118 mvzl	r0,15 */
0x0000f3b8, 0x0600f86d, /* 0600f86d //C 0f3b8  1119 st	r0,nuof_reg */
0x0000f3b9, 0x01f2f37c, /* 01f2f37c //C 0f3b9  1120 jmp	cmd_rx */
0x0000f3ba, 0x01020010, /* 01020010 //C 0f3ba  1125 mvzl	r0,16 */
0x0000f3bb, 0x0600f86d, /* 0600f86d //C 0f3bb  1126 st	r0,nuof_reg */
0x0000f3bc, 0x01f2f37c, /* 01f2f37c //C 0f3bc  1127 jmp	cmd_rx */
0x0000f3bd, 0x0ded0000, /* 0ded0000 //C 0f3bd  1132 push	lr */
0x0000f3be, 0x06020100, /* 06020100 //C 0f3be  1133 rds	r0,SVER */
0x0000f3bf, 0x0611c002, /* 0611c002 //C 0f3bf  1134 getbz	r1,r0,2 */
0x0000f3c0, 0x0621c001, /* 0621c001 //C 0f3c0  1135 getbz	r2,r0,1 */
0x0000f3c1, 0x0631c000, /* 0631c000 //C 0f3c1  1136 getbz	r3,r0,0 */
0x0000f3c2, 0x06420200, /* 06420200 //C 0f3c2  1137 rds	r4,SFEAT1 */
0x0000f3c3, 0x06520300, /* 06520300 //C 0f3c3  1138 rds	r5,SFEAT2 */
0x0000f3c4, 0xf400f7f5, /* f400f7f5 //C 0f3c4  1139 ces	pesf */
0x0000f3c5, 0x00000063, /* 00000063 //C 0f3c5  1140 db	99 */
0x0000f3c6, 0x00000070, /* 00000070 //C 0f3c6  1140 db	112 */
0x0000f3c7, 0x00000075, /* 00000075 //C 0f3c7  1140 db	117 */
0x0000f3c8, 0x0000003a, /* 0000003a //C 0f3c8  1140 db	58 */
0x0000f3c9, 0x00000020, /* 00000020 //C 0f3c9  1140 db	32 */
0x0000f3ca, 0x00000025, /* 00000025 //C 0f3ca  1140 db	37 */
0x0000f3cb, 0x00000064, /* 00000064 //C 0f3cb  1140 db	100 */
0x0000f3cc, 0x0000002e, /* 0000002e //C 0f3cc  1140 db	46 */
0x0000f3cd, 0x00000025, /* 00000025 //C 0f3cd  1140 db	37 */
0x0000f3ce, 0x00000064, /* 00000064 //C 0f3ce  1140 db	100 */
0x0000f3cf, 0x0000002e, /* 0000002e //C 0f3cf  1140 db	46 */
0x0000f3d0, 0x00000025, /* 00000025 //C 0f3d0  1140 db	37 */
0x0000f3d1, 0x00000064, /* 00000064 //C 0f3d1  1140 db	100 */
0x0000f3d2, 0x00000020, /* 00000020 //C 0f3d2  1140 db	32 */
0x0000f3d3, 0x00000066, /* 00000066 //C 0f3d3  1140 db	102 */
0x0000f3d4, 0x00000065, /* 00000065 //C 0f3d4  1140 db	101 */
0x0000f3d5, 0x00000061, /* 00000061 //C 0f3d5  1140 db	97 */
0x0000f3d6, 0x00000074, /* 00000074 //C 0f3d6  1140 db	116 */
0x0000f3d7, 0x00000031, /* 00000031 //C 0f3d7  1140 db	49 */
0x0000f3d8, 0x0000003a, /* 0000003a //C 0f3d8  1140 db	58 */
0x0000f3d9, 0x00000020, /* 00000020 //C 0f3d9  1140 db	32 */
0x0000f3da, 0x00000025, /* 00000025 //C 0f3da  1140 db	37 */
0x0000f3db, 0x00000078, /* 00000078 //C 0f3db  1140 db	120 */
0x0000f3dc, 0x00000020, /* 00000020 //C 0f3dc  1140 db	32 */
0x0000f3dd, 0x00000066, /* 00000066 //C 0f3dd  1140 db	102 */
0x0000f3de, 0x00000065, /* 00000065 //C 0f3de  1140 db	101 */
0x0000f3df, 0x00000061, /* 00000061 //C 0f3df  1140 db	97 */
0x0000f3e0, 0x00000074, /* 00000074 //C 0f3e0  1140 db	116 */
0x0000f3e1, 0x00000032, /* 00000032 //C 0f3e1  1140 db	50 */
0x0000f3e2, 0x0000003a, /* 0000003a //C 0f3e2  1140 db	58 */
0x0000f3e3, 0x00000020, /* 00000020 //C 0f3e3  1140 db	32 */
0x0000f3e4, 0x00000025, /* 00000025 //C 0f3e4  1140 db	37 */
0x0000f3e5, 0x00000078, /* 00000078 //C 0f3e5  1140 db	120 */
0x0000f3e6, 0x0000000a, /* 0000000a //C 0f3e6  1140 db	10 */
0x0000f3e7, 0x00000000, /* 00000000 //C 0f3e7  1140 db */
0x0000f3e8, 0x0ffd0000, /* 0ffd0000 //C 0f3e8  1141 pop	pc */
0x0000f3e9, 0x0ded0000, /* 0ded0000 //C 0f3e9  1151 push	lr */
0x0000f3ea, 0x0d4d0000, /* 0d4d0000 //C 0f3ea  1152 push	r4 */
0x0000f3eb, 0x021b0000, /* 021b0000 //C 0f3eb  1154 sz	r1 */
0x0000f3ec, 0x21f2f3f0, /* 21f2f3f0 //C 0f3ec  1155 NZ jmp	div_dok */
0x0000f3ed, 0x00200000, /* 00200000 //C 0f3ed  1156 mov	r2,r0		; div by zero */
0x0000f3ee, 0x01320000, /* 01320000 //C 0f3ee  1157 mvzl	r3,0 */
0x0000f3ef, 0x01f2f401, /* 01f2f401 //C 0f3ef  1158 jmp	div_ret */
0x0000f3f0, 0x01220000, /* 01220000 //C 0f3f0  1160 mvzl	r2,0		; Q= 0 */
0x0000f3f1, 0x01320000, /* 01320000 //C 0f3f1  1161 mvzl	r3,0		; R= 0 */
0x0000f3f2, 0x01418000, /* 01418000 //C 0f3f2  1162 mvh	r4,0x80000000	; m= 1<<31 */
0x0000f3f3, 0x01400000, /* 01400000 //C 0f3f3  1163 mvl	r4,0x80000000 */
0x0000f3f4, 0x024b0000, /* 024b0000 //C 0f3f4  1165 sz	r4 */
0x0000f3f5, 0x11f2f401, /* 11f2f401 //C 0f3f5  1166 Z jmp	div_ret */
0x0000f3f6, 0x02380000, /* 02380000 //C 0f3f6  1167 shl	r3		; r<<= 1 */
0x0000f3f7, 0x000c0400, /* 000c0400 //C 0f3f7  1168 test	r0,r4		; if (n&m) */
0x0000f3f8, 0x213d0001, /* 213d0001 //C 0f3f8  1169 NZ or	r3,1		; r|= 1 */
0x0000f3f9, 0x00380100, /* 00380100 //C 0f3f9  1170 cmp	r3,r1		; if (r>=d) */
0x0000f3fa, 0x41f2f3ff, /* 41f2f3ff //C 0f3fa  1171 LO jmp	div_cyc_next */
0x0000f3fb, 0x00360100, /* 00360100 //C 0f3fb  1172 sub	r3,r1		;r-= d */
0x0000f3fc, 0x002d0400, /* 002d0400 //C 0f3fc  1173 or	r2,r4		;q|= m */
0x0000f3fd, 0x01f2f3ff, /* 01f2f3ff //C 0f3fd  1174 jmp	div_cyc_next */
0x0000f3fe, 0x01f2f3f4, /* 01f2f3f4 //C 0f3fe  1175 jmp	div_cyc */
0x0000f3ff, 0x02490000, /* 02490000 //C 0f3ff  1177 shr	r4		; m>>= 1 */
0x0000f400, 0x01f2f3f4, /* 01f2f3f4 //C 0f400  1178 jmp	div_cyc */
0x0000f401, 0x0f4d0000, /* 0f4d0000 //C 0f401  1180 pop	r4 */
0x0000f402, 0x0ffd0000, /* 0ffd0000 //C 0f402  1181 pop	pc */
0x0000f403, 0x0ded0000, /* 0ded0000 //C 0f403  1188 push	lr */
0x0000f404, 0x0d1d0000, /* 0d1d0000 //C 0f404  1189 push	r1 */
0x0000f405, 0x0d2d0000, /* 0d2d0000 //C 0f405  1190 push	r2 */
0x0000f406, 0x0108e0ff, /* 0108e0ff //C 0f406  1191 cmp	r0,99999999 */
0x0000f407, 0x91f2f416, /* 91f2f416 //C 0f407  1192 UGT jmp	itobcd_bad */
0x0000f408, 0x0400f58f, /* 0400f58f //C 0f408  1193 call	utoa */
0x0000f409, 0x01020000, /* 01020000 //C 0f409  1194 mvzl	r0,0 */
0x0000f40a, 0x0112f5c7, /* 0112f5c7 //C 0f40a  1195 mvzl	r1,itoa_buffer */
0x0000f40b, 0x0e210000, /* 0e210000 //C 0f40b  1197 ld	r2,r1 */
0x0000f40c, 0x022b0000, /* 022b0000 //C 0f40c  1198 sz	r2 */
0x0000f40d, 0x11f2f417, /* 11f2f417 //C 0f40d  1199 jz	itobcd_ret */
0x0000f40e, 0x01260030, /* 01260030 //C 0f40e  1200 sub	r2,'0' */
0x0000f40f, 0x02080000, /* 02080000 //C 0f40f  1201 shl	r0 */
0x0000f410, 0x02080000, /* 02080000 //C 0f410  1202 shl	r0 */
0x0000f411, 0x02080000, /* 02080000 //C 0f411  1203 shl	r0 */
0x0000f412, 0x02080000, /* 02080000 //C 0f412  1204 shl	r0 */
0x0000f413, 0x000d0200, /* 000d0200 //C 0f413  1205 or	r0,r2 */
0x0000f414, 0x01140001, /* 01140001 //C 0f414  1206 inc	r1 */
0x0000f415, 0x01f2f40b, /* 01f2f40b //C 0f415  1207 jmp	itobcd_cyc */
0x0000f416, 0x01020000, /* 01020000 //C 0f416  1209 mvzl	r0,0 */
0x0000f417, 0x0f2d0000, /* 0f2d0000 //C 0f417  1211 pop	r2 */
0x0000f418, 0x0f1d0000, /* 0f1d0000 //C 0f418  1212 pop	r1 */
0x0000f419, 0x0ffd0000, /* 0ffd0000 //C 0f419  1213 pop	pc */
0x0000f41a, 0x01420000, /* 01420000 //C 0f41a  1219 mvzl	r4,0 */
0x0000f41b, 0x0108007f, /* 0108007f //C 0f41b  1220 cmp	r0,0x7f */
0x0000f41c, 0x90f00e00, /* 90f00e00 //C 0f41c  1221 UGT ret */
0x0000f41d, 0x0d0d0000, /* 0d0d0000 //C 0f41d  1223 push	r0 */
0x0000f41e, 0x0d1d0000, /* 0d1d0000 //C 0f41e  1224 push	r1 */
0x0000f41f, 0x02090000, /* 02090000 //C 0f41f  1226 shr	r0 */
0x0000f420, 0x02090000, /* 02090000 //C 0f420  1227 shr	r0 */
0x0000f421, 0x0112fabf, /* 0112fabf //C 0f421  1228 mvzl	r1,ascii2seg_table */
0x0000f422, 0x0a400100, /* 0a400100 //C 0f422  1229 ld	r4,r0,r1 */
0x0000f423, 0x0f1d0000, /* 0f1d0000 //C 0f423  1230 pop	r1 */
0x0000f424, 0x0f0d0000, /* 0f0d0000 //C 0f424  1231 pop	r0 */
0x0000f425, 0x06414400, /* 06414400 //C 0f425  1232 getbz	r4,r4,r0 */
0x0000f426, 0x00f00e00, /* 00f00e00 //C 0f426  1233 ret */
0x0000f427, 0x0d0d0000, /* 0d0d0000 //C 0f427  1243 push	r0 */
0x0000f428, 0x0d1d0000, /* 0d1d0000 //C 0f428  1244 push	r1 */
0x0000f429, 0x0d2d0000, /* 0d2d0000 //C 0f429  1245 push	r2 */
0x0000f42a, 0x0d3d0000, /* 0d3d0000 //C 0f42a  1246 push	r3 */
0x0000f42b, 0x0d4d0000, /* 0d4d0000 //C 0f42b  1247 push	r4 */
0x0000f42c, 0x020b0000, /* 020b0000 //C 0f42c  1249 sz	r0		; check pointers */
0x0000f42d, 0x11f2f440, /* 11f2f440 //C 0f42d  1250 jz	sup_ret */
0x0000f42e, 0x021b0000, /* 021b0000 //C 0f42e  1251 sz	r1 */
0x0000f42f, 0x11f2f440, /* 11f2f440 //C 0f42f  1252 jz	sup_ret */
0x0000f430, 0x01220000, /* 01220000 //C 0f430  1254 mvzl	r2,0		; byte index in input */
0x0000f431, 0x0e300000, /* 0e300000 //C 0f431  1255 ld	r3,r0		; pick next word */
0x0000f432, 0x023b0000, /* 023b0000 //C 0f432  1256 sz	r3		; is it EOS? */
0x0000f433, 0x11f2f43e, /* 11f2f43e //C 0f433  1257 jz	sup_fin */
0x0000f434, 0x06414302, /* 06414302 //C 0f434  1259 getbz	r4,r3,r2	; pick byte */
0x0000f435, 0x024b0000, /* 024b0000 //C 0f435  1260 sz	r4		; check byte */
0x0000f436, 0x11f2f43c, /* 11f2f43c //C 0f436  1261 jz	sup_next_word */
0x0000f437, 0x0c410000, /* 0c410000 //C 0f437  1262 st	r4,r1		; produce output */
0x0000f438, 0x01140001, /* 01140001 //C 0f438  1263 inc	r1 */
0x0000f439, 0x01240001, /* 01240001 //C 0f439  1264 inc	r2		; advance byte index */
0x0000f43a, 0x01280004, /* 01280004 //C 0f43a  1265 cmp	r2,4 */
0x0000f43b, 0x21f2f434, /* 21f2f434 //C 0f43b  1266 NE jmp	sup_next_char */
0x0000f43c, 0x01040001, /* 01040001 //C 0f43c  1268 inc	r0 */
0x0000f43d, 0x01f2f430, /* 01f2f430 //C 0f43d  1269 jmp	sup_cyc */
0x0000f43e, 0x01320000, /* 01320000 //C 0f43e  1271 mvzl	r3,0		; gen EOS in output */
0x0000f43f, 0x0c310000, /* 0c310000 //C 0f43f  1272 st	r3,r1 */
0x0000f440, 0x0f4d0000, /* 0f4d0000 //C 0f440  1274 pop	r4 */
0x0000f441, 0x0f3d0000, /* 0f3d0000 //C 0f441  1275 pop	r3 */
0x0000f442, 0x0f2d0000, /* 0f2d0000 //C 0f442  1276 pop	r2 */
0x0000f443, 0x0f1d0000, /* 0f1d0000 //C 0f443  1277 pop	r1 */
0x0000f444, 0x0f0d0000, /* 0f0d0000 //C 0f444  1278 pop	r0 */
0x0000f445, 0x00f00e00, /* 00f00e00 //C 0f445  1279 ret */
0x0000f446, 0x0d0d0000, /* 0d0d0000 //C 0f446  1286 push	r0 */
0x0000f447, 0x0d1d0000, /* 0d1d0000 //C 0f447  1287 push	r1 */
0x0000f448, 0x0d2d0000, /* 0d2d0000 //C 0f448  1288 push	r2 */
0x0000f449, 0x0d3d0000, /* 0d3d0000 //C 0f449  1289 push	r3 */
0x0000f44a, 0x0d4d0000, /* 0d4d0000 //C 0f44a  1290 push	r4 */
0x0000f44b, 0x0d5d0000, /* 0d5d0000 //C 0f44b  1291 push	r5 */
0x0000f44c, 0x0d6d0000, /* 0d6d0000 //C 0f44c  1292 push	r6 */
0x0000f44d, 0x020b0000, /* 020b0000 //C 0f44d  1294 sz	r0		; check pointers */
0x0000f44e, 0x11f2f46d, /* 11f2f46d //C 0f44e  1295 jz	sp_ret */
0x0000f44f, 0x021b0000, /* 021b0000 //C 0f44f  1296 sz	r1 */
0x0000f450, 0x11f2f46d, /* 11f2f46d //C 0f450  1297 jz	sp_ret */
0x0000f451, 0x01520000, /* 01520000 //C 0f451  1298 mvzl	r5,0		; output byte index */
0x0000f452, 0x01620000, /* 01620000 //C 0f452  1299 mvzl	r6,0		; output word */
0x0000f453, 0x01220000, /* 01220000 //C 0f453  1301 mvzl	r2,0		; byte index in input */
0x0000f454, 0x0e300000, /* 0e300000 //C 0f454  1302 ld	r3,r0		; pick next word */
0x0000f455, 0x023b0000, /* 023b0000 //C 0f455  1303 sz	r3		; is it EOS? */
0x0000f456, 0x11f2f467, /* 11f2f467 //C 0f456  1304 jz	sp_fin */
0x0000f457, 0x06414302, /* 06414302 //C 0f457  1306 getbz	r4,r3,r2	; pick byte */
0x0000f458, 0x024b0000, /* 024b0000 //C 0f458  1307 sz	r4		; check byte */
0x0000f459, 0x11f2f465, /* 11f2f465 //C 0f459  1308 jz	sp_next_word */
0x0000f45a, 0x07610405, /* 07610405 //C 0f45a  1310 putb	r6,r4,r5	; pack output word */
0x0000f45b, 0x01540001, /* 01540001 //C 0f45b  1311 inc	r5		; advance output byte index */
0x0000f45c, 0x01580004, /* 01580004 //C 0f45c  1312 cmp	r5,4		; is it full? */
0x0000f45d, 0x21f2f462, /* 21f2f462 //C 0f45d  1313 jnz	sp_not_full */
0x0000f45e, 0x0c610000, /* 0c610000 //C 0f45e  1314 st	r6,r1		; if yes, store word in mem */
0x0000f45f, 0x01520000, /* 01520000 //C 0f45f  1315 mvzl	r5,0		; and restart word */
0x0000f460, 0x01620000, /* 01620000 //C 0f460  1316 mvzl	r6,0 */
0x0000f461, 0x01140001, /* 01140001 //C 0f461  1317 inc	r1		; and advance outout pointer */
0x0000f462, 0x01240001, /* 01240001 //C 0f462  1319 inc	r2		; advance input byte index */
0x0000f463, 0x01280004, /* 01280004 //C 0f463  1320 cmp	r2,4 */
0x0000f464, 0x21f2f457, /* 21f2f457 //C 0f464  1321 NE jmp	sp_next_char */
0x0000f465, 0x01040001, /* 01040001 //C 0f465  1323 inc	r0 */
0x0000f466, 0x01f2f453, /* 01f2f453 //C 0f466  1324 jmp	sp_cyc */
0x0000f467, 0x025b0000, /* 025b0000 //C 0f467  1326 sz	r5 */
0x0000f468, 0x11f2f46b, /* 11f2f46b //C 0f468  1327 jz	sp_word_flushed */
0x0000f469, 0x0c610000, /* 0c610000 //C 0f469  1329 st	r6,r1 */
0x0000f46a, 0x01140001, /* 01140001 //C 0f46a  1330 inc	r1 */
0x0000f46b, 0x01320000, /* 01320000 //C 0f46b  1332 mvzl	r3,0		; gen EOS in output */
0x0000f46c, 0x0c310000, /* 0c310000 //C 0f46c  1333 st	r3,r1 */
0x0000f46d, 0x0f6d0000, /* 0f6d0000 //C 0f46d  1335 pop	r6 */
0x0000f46e, 0x0f5d0000, /* 0f5d0000 //C 0f46e  1336 pop	r5 */
0x0000f46f, 0x0f4d0000, /* 0f4d0000 //C 0f46f  1337 pop	r4 */
0x0000f470, 0x0f3d0000, /* 0f3d0000 //C 0f470  1338 pop	r3 */
0x0000f471, 0x0f2d0000, /* 0f2d0000 //C 0f471  1339 pop	r2 */
0x0000f472, 0x0f1d0000, /* 0f1d0000 //C 0f472  1340 pop	r1 */
0x0000f473, 0x0f0d0000, /* 0f0d0000 //C 0f473  1341 pop	r0 */
0x0000f474, 0x00f00e00, /* 00f00e00 //C 0f474  1342 ret */
0x0000f475, 0x0d3d0000, /* 0d3d0000 //C 0f475  1352 push	r3 */
0x0000f476, 0x0d4d0000, /* 0d4d0000 //C 0f476  1353 push	r4 */
0x0000f477, 0x01220000, /* 01220000 //C 0f477  1355 mvzl	r2,0		; byte index re-start */
0x0000f478, 0x0e310000, /* 0e310000 //C 0f478  1356 ld	r3,r1		; get next word */
0x0000f479, 0x023b0000, /* 023b0000 //C 0f479  1357 sz	r3		; check for eof */
0x0000f47a, 0x11f2f487, /* 11f2f487 //C 0f47a  1358 jz	strchr_no	; eof string found */
0x0000f47b, 0x06414302, /* 06414302 //C 0f47b  1360 getbz	r4,r3,r2	; pick a byte */
0x0000f47c, 0x024b0000, /* 024b0000 //C 0f47c  1361 sz	r4		; is it zero? */
0x0000f47d, 0x11f2f483, /* 11f2f483 //C 0f47d  1362 jz	strchr_word	; if yes, pick next word */
0x0000f47e, 0x00480000, /* 00480000 //C 0f47e  1363 cmp	r4,r0		; compare */
0x0000f47f, 0x11f2f485, /* 11f2f485 //C 0f47f  1364 jz	strchr_yes	; found it */
0x0000f480, 0x01240001, /* 01240001 //C 0f480  1366 inc	r2		; advance byte index */
0x0000f481, 0x01280004, /* 01280004 //C 0f481  1367 cmp	r2,4		; check byte overflow */
0x0000f482, 0x21f2f47b, /* 21f2f47b //C 0f482  1368 jnz	strchr_go	; no, overflow, go on */
0x0000f483, 0x011a0001, /* 011a0001 //C 0f483  1370 plus	r1,1		; go to next char */
0x0000f484, 0x01f2f477, /* 01f2f477 //C 0f484  1371 jmp	strchr_cyc */
0x0000f485, 0x020c0000, /* 020c0000 //C 0f485  1373 sec */
0x0000f486, 0x01f2f489, /* 01f2f489 //C 0f486  1374 jmp	strchr_ret */
0x0000f487, 0x01120000, /* 01120000 //C 0f487  1376 mvzl	r1,0 */
0x0000f488, 0x020d0000, /* 020d0000 //C 0f488  1377 clc */
0x0000f489, 0x0f4d0000, /* 0f4d0000 //C 0f489  1379 pop	r4 */
0x0000f48a, 0x0f3d0000, /* 0f3d0000 //C 0f48a  1380 pop	r3 */
0x0000f48b, 0x00f00e00, /* 00f00e00 //C 0f48b  1383 ret */
0x0000f48c, 0x0ded0000, /* 0ded0000 //C 0f48c  1394 push	lr		; Save used registers */
0x0000f48d, 0x0d0d0000, /* 0d0d0000 //C 0f48d  1395 push	r0		; and input parameters */
0x0000f48e, 0x0d2d0000, /* 0d2d0000 //C 0f48e  1397 push	r2 */
0x0000f48f, 0x0d4d0000, /* 0d4d0000 //C 0f48f  1398 push	r4 */
0x0000f490, 0x0d5d0000, /* 0d5d0000 //C 0f490  1399 push	r5 */
0x0000f491, 0x0d6d0000, /* 0d6d0000 //C 0f491  1400 push	r6 */
0x0000f492, 0x0d7d0000, /* 0d7d0000 //C 0f492  1401 push	r7		; byte idx in string 1 */
0x0000f493, 0x0d8d0000, /* 0d8d0000 //C 0f493  1402 push	r8		; byte idx in string 2 */
0x0000f494, 0x01720000, /* 01720000 //C 0f494  1403 mvzl	r7,0 */
0x0000f495, 0x01820000, /* 01820000 //C 0f495  1404 mvzl	r8,0 */
0x0000f496, 0x0e200000, /* 0e200000 //C 0f496  1406 ld	r2,r0		; Got one char from first str */
0x0000f497, 0x022b0000, /* 022b0000 //C 0f497  1407 sz	r2		; is it eos? */
0x0000f498, 0x11f2f4a7, /* 11f2f4a7 //C 0f498  1408 jz	streq_pick2	; if yes, go on */
0x0000f499, 0x06214207, /* 06214207 //C 0f499  1409 getbz	r2,r2,r7	; pick one byte */
0x0000f49a, 0x022b0000, /* 022b0000 //C 0f49a  1410 sz	r2		; is it 0? */
0x0000f49b, 0x21f2f4a7, /* 21f2f4a7 //C 0f49b  1411 jnz	streq_pick2	; if not, go on */
0x0000f49c, 0x01740001, /* 01740001 //C 0f49c  1412 inc	r7		; step to next byte */
0x0000f49d, 0x01780004, /* 01780004 //C 0f49d  1413 cmp	r7,4		; word is overflowed? */
0x0000f49e, 0x11f2f4a3, /* 11f2f4a3 //C 0f49e  1414 jz	streq_p1ov */
0x0000f49f, 0x0e200000, /* 0e200000 //C 0f49f  1416 ld	r2,r0		; pick orig word, and */
0x0000f4a0, 0x06214207, /* 06214207 //C 0f4a0  1417 getbz	r2,r2,r7	; check next byte */
0x0000f4a1, 0x022b0000, /* 022b0000 //C 0f4a1  1418 sz	r2		; is it 0? */
0x0000f4a2, 0x21f2f4a7, /* 21f2f4a7 //C 0f4a2  1419 jnz	streq_pick2	; if not, go on */
0x0000f4a3, 0x01040001, /* 01040001 //C 0f4a3  1421 inc	r0		; if yes, move pointer */
0x0000f4a4, 0x01720000, /* 01720000 //C 0f4a4  1422 mvzl	r7,0		; and reset byte counter */
0x0000f4a5, 0x0e200000, /* 0e200000 //C 0f4a5  1423 ld	r2,r0		; get first byte of next word */
0x0000f4a6, 0x06214207, /* 06214207 //C 0f4a6  1424 getbz	r2,r2,r7 */
0x0000f4a7, 0x0e610000, /* 0e610000 //C 0f4a7  1427 ld	r6,r1		; pick from second string */
0x0000f4a8, 0x026b0000, /* 026b0000 //C 0f4a8  1428 sz	r6		; is it eos? */
0x0000f4a9, 0x11f2f4b8, /* 11f2f4b8 //C 0f4a9  1429 jz	streq_prep	; if yes, go to compare */
0x0000f4aa, 0x06614608, /* 06614608 //C 0f4aa  1430 getbz	r6,r6,r8	; pick a byte */
0x0000f4ab, 0x026b0000, /* 026b0000 //C 0f4ab  1431 sz	r6		; is it 0? */
0x0000f4ac, 0x21f2f4b8, /* 21f2f4b8 //C 0f4ac  1432 jnz	streq_prep	; if not, go to compare */
0x0000f4ad, 0x01840001, /* 01840001 //C 0f4ad  1433 inc	r8		; step to next byte */
0x0000f4ae, 0x01880004, /* 01880004 //C 0f4ae  1434 cmp	r8,4		; is word overflowed? */
0x0000f4af, 0x11f2f4b4, /* 11f2f4b4 //C 0f4af  1435 jz	streq_p2ov */
0x0000f4b0, 0x0e610000, /* 0e610000 //C 0f4b0  1437 ld	r6,r1		; pick orig word, and */
0x0000f4b1, 0x06614608, /* 06614608 //C 0f4b1  1438 getbz	r6,r6,r8	; check next byte */
0x0000f4b2, 0x026b0000, /* 026b0000 //C 0f4b2  1439 sz	r6		; is it 0? */
0x0000f4b3, 0x21f2f4b8, /* 21f2f4b8 //C 0f4b3  1440 jnz	streq_prep	; if not, go on */
0x0000f4b4, 0x01140001, /* 01140001 //C 0f4b4  1442 inc	r1		; if yes, move pointer */
0x0000f4b5, 0x01820000, /* 01820000 //C 0f4b5  1443 mvzl	r8,0		; and reset byte counter */
0x0000f4b6, 0x0e610000, /* 0e610000 //C 0f4b6  1444 ld	r6,r1		; get next word */
0x0000f4b7, 0x06614608, /* 06614608 //C 0f4b7  1445 getbz	r6,r6,r8	; and pick first byte */
0x0000f4b8, 0x023b0000, /* 023b0000 //C 0f4b8  1448 sz	r3		; Prepare for comparing */
0x0000f4b9, 0x112d0020, /* 112d0020 //C 0f4b9  1449 Z1 or	r2,0x20		; if insensitive case */
0x0000f4ba, 0x023b0000, /* 023b0000 //C 0f4ba  1450 sz	r3 */
0x0000f4bb, 0x116d0020, /* 116d0020 //C 0f4bb  1451 Z1 or	r6,0x20 */
0x0000f4bc, 0x00280600, /* 00280600 //C 0f4bc  1452 cmp	r2,r6		; compare them */
0x0000f4bd, 0x21f2f4d7, /* 21f2f4d7 //C 0f4bd  1453 jnz	streq_no	; if differs: strings are not equal */
0x0000f4be, 0x0e200000, /* 0e200000 //C 0f4be  1455 ld	r2,r0		; Pick original (non-prepared) */
0x0000f4bf, 0x0e610000, /* 0e610000 //C 0f4bf  1456 ld	r6,r1		; chars to check EOS */
0x0000f4c0, 0x06214207, /* 06214207 //C 0f4c0  1457 getbz	r2,r2,r7 */
0x0000f4c1, 0x06614608, /* 06614608 //C 0f4c1  1458 getbz	r6,r6,r8 */
0x0000f4c2, 0x022b0000, /* 022b0000 //C 0f4c2  1459 sz	r2		; convert them to boolean */
0x0000f4c3, 0x21220001, /* 21220001 //C 0f4c3  1460 Z0 mvzl	r2,1		; values in R2,R6 */
0x0000f4c4, 0x11220000, /* 11220000 //C 0f4c4  1461 Z1 mvzl	r2,0		; and copy in R4,R5 */
0x0000f4c5, 0x00400200, /* 00400200 //C 0f4c5  1462 mov	r4,r2 */
0x0000f4c6, 0x026b0000, /* 026b0000 //C 0f4c6  1463 sz	r6 */
0x0000f4c7, 0x21620001, /* 21620001 //C 0f4c7  1464 Z0 mvzl	r6,1 */
0x0000f4c8, 0x11620000, /* 11620000 //C 0f4c8  1465 Z1 mvzl r6,0 */
0x0000f4c9, 0x00500600, /* 00500600 //C 0f4c9  1466 mov	r5,r6 */
0x0000f4ca, 0x004d0500, /* 004d0500 //C 0f4ca  1467 or	r4,r5		; if both are EOS: equal */
0x0000f4cb, 0x11f2f4d9, /* 11f2f4d9 //C 0f4cb  1468 jz	streq_yes */
0x0000f4cc, 0x002f0600, /* 002f0600 //C 0f4cc  1469 and 	r2,r6		; just one is EOS: not equal */
0x0000f4cd, 0x11f2f4d7, /* 11f2f4d7 //C 0f4cd  1470 jz	streq_no */
0x0000f4ce, 0x01740001, /* 01740001 //C 0f4ce  1473 inc	r7		; step byte count */
0x0000f4cf, 0x01780004, /* 01780004 //C 0f4cf  1474 cmp	r7,4		; if word overflows */
0x0000f4d0, 0x110a0001, /* 110a0001 //C 0f4d0  1475 Z plus	r0,1		; then step the pointer */
0x0000f4d1, 0x11720000, /* 11720000 //C 0f4d1  1476 Z mvzl	r7,0		; and reset the byte counter */
0x0000f4d2, 0x01840001, /* 01840001 //C 0f4d2  1478 inc	r8 */
0x0000f4d3, 0x01880004, /* 01880004 //C 0f4d3  1479 cmp	r8,4 */
0x0000f4d4, 0x111a0001, /* 111a0001 //C 0f4d4  1480 Z plus	r1,1 */
0x0000f4d5, 0x11820000, /* 11820000 //C 0f4d5  1481 Z mvzl	r8,0 */
0x0000f4d6, 0x01f2f496, /* 01f2f496 //C 0f4d6  1482 jmp	streq_cyc */
0x0000f4d7, 0x020d0000, /* 020d0000 //C 0f4d7  1485 clc			; False result */
0x0000f4d8, 0x01f2f4da, /* 01f2f4da //C 0f4d8  1486 jmp	streq_ret */
0x0000f4d9, 0x020c0000, /* 020c0000 //C 0f4d9  1489 sec			; True result */
0x0000f4da, 0x0e610000, /* 0e610000 //C 0f4da  1492 ld	r6,r1		; forward R1 to EOS */
0x0000f4db, 0x026b0000, /* 026b0000 //C 0f4db  1493 sz	r6 */
0x0000f4dc, 0x11f2f4df, /* 11f2f4df //C 0f4dc  1494 jz	streq_ret_ret */
0x0000f4dd, 0x01140001, /* 01140001 //C 0f4dd  1495 inc	r1 */
0x0000f4de, 0x01f2f4da, /* 01f2f4da //C 0f4de  1496 jmp	streq_ret */
0x0000f4df, 0x0f8d0000, /* 0f8d0000 //C 0f4df  1498 pop	r8 */
0x0000f4e0, 0x0f7d0000, /* 0f7d0000 //C 0f4e0  1499 pop	r7 */
0x0000f4e1, 0x0f6d0000, /* 0f6d0000 //C 0f4e1  1500 pop	r6 */
0x0000f4e2, 0x0f5d0000, /* 0f5d0000 //C 0f4e2  1501 pop	r5 */
0x0000f4e3, 0x0f4d0000, /* 0f4d0000 //C 0f4e3  1502 pop	r4 */
0x0000f4e4, 0x0f2d0000, /* 0f2d0000 //C 0f4e4  1503 pop	r2 */
0x0000f4e5, 0x0f0d0000, /* 0f0d0000 //C 0f4e5  1505 pop	r0 */
0x0000f4e6, 0x0ffd0000, /* 0ffd0000 //C 0f4e6  1506 pop	pc */
0x0000f4e7, 0x0ded0000, /* 0ded0000 //C 0f4e7  1514 push	lr */
0x0000f4e8, 0x0d1d0000, /* 0d1d0000 //C 0f4e8  1515 push	r1 */
0x0000f4e9, 0x0d3d0000, /* 0d3d0000 //C 0f4e9  1516 push	r3 */
0x0000f4ea, 0x01320001, /* 01320001 //C 0f4ea  1517 mvzl	r3,1 */
0x0000f4eb, 0x0400f48c, /* 0400f48c //C 0f4eb  1518 call	str_cmp_eq */
0x0000f4ec, 0x0f3d0000, /* 0f3d0000 //C 0f4ec  1519 pop	r3 */
0x0000f4ed, 0x0f1d0000, /* 0f1d0000 //C 0f4ed  1520 pop	r1 */
0x0000f4ee, 0x0ffd0000, /* 0ffd0000 //C 0f4ee  1521 pop	pc */
0x0000f4ef, 0x0ded0000, /* 0ded0000 //C 0f4ef  1529 push	lr */
0x0000f4f0, 0x0d1d0000, /* 0d1d0000 //C 0f4f0  1530 push	r1 */
0x0000f4f1, 0x0d3d0000, /* 0d3d0000 //C 0f4f1  1531 push	r3 */
0x0000f4f2, 0x01320000, /* 01320000 //C 0f4f2  1532 mvzl	r3,0 */
0x0000f4f3, 0x0400f48c, /* 0400f48c //C 0f4f3  1533 call	str_cmp_eq */
0x0000f4f4, 0x0f3d0000, /* 0f3d0000 //C 0f4f4  1534 pop	r3 */
0x0000f4f5, 0x0f1d0000, /* 0f1d0000 //C 0f4f5  1535 pop	r1 */
0x0000f4f6, 0x0ffd0000, /* 0ffd0000 //C 0f4f6  1536 pop	pc */
0x0000f4f7, 0x0d0d0000, /* 0d0d0000 //C 0f4f7  1543 push	r0 */
0x0000f4f8, 0x0d1d0000, /* 0d1d0000 //C 0f4f8  1544 push	r1 */
0x0000f4f9, 0x0d2d0000, /* 0d2d0000 //C 0f4f9  1545 push	r2 */
0x0000f4fa, 0x0d3d0000, /* 0d3d0000 //C 0f4fa  1546 push	r3 */
0x0000f4fb, 0x00100000, /* 00100000 //C 0f4fb  1547 mov	r1,r0 */
0x0000f4fc, 0x01220000, /* 01220000 //C 0f4fc  1548 mvzl	r2,0 */
0x0000f4fd, 0x01420000, /* 01420000 //C 0f4fd  1549 mvzl	r4,0 */
0x0000f4fe, 0x020b0000, /* 020b0000 //C 0f4fe  1550 sz	r0		; check NULL pointer */
0x0000f4ff, 0x11f2f50b, /* 11f2f50b //C 0f4ff  1551 jz	p2_end */
0x0000f500, 0x0e310000, /* 0e310000 //C 0f500  1553 ld	r3,r1 */
0x0000f501, 0x023b0000, /* 023b0000 //C 0f501  1554 sz	r3 */
0x0000f502, 0x11f2f50b, /* 11f2f50b //C 0f502  1555 jz	p2_end */
0x0000f503, 0x06014302, /* 06014302 //C 0f503  1557 getbz	r0,r3,r2 */
0x0000f504, 0x020b0000, /* 020b0000 //C 0f504  1558 sz	r0 */
0x0000f505, 0x21440001, /* 21440001 //C 0f505  1559 NZ inc	r4 */
0x0000f506, 0x01240001, /* 01240001 //C 0f506  1560 inc	r2 */
0x0000f507, 0x012c0003, /* 012c0003 //C 0f507  1561 test	r2,3 */
0x0000f508, 0x111a0001, /* 111a0001 //C 0f508  1562 Z plus	r1,1 */
0x0000f509, 0x11f2f500, /* 11f2f500 //C 0f509  1563 Z jmp	p2_next */
0x0000f50a, 0x01f2f503, /* 01f2f503 //C 0f50a  1564 jmp	p2_cyc */
0x0000f50b, 0x0f3d0000, /* 0f3d0000 //C 0f50b  1566 pop	r3 */
0x0000f50c, 0x0f2d0000, /* 0f2d0000 //C 0f50c  1567 pop	r2 */
0x0000f50d, 0x0f1d0000, /* 0f1d0000 //C 0f50d  1568 pop	r1 */
0x0000f50e, 0x0f0d0000, /* 0f0d0000 //C 0f50e  1569 pop	r0 */
0x0000f50f, 0x00f00e00, /* 00f00e00 //C 0f50f  1570 ret */
0x0000f510, 0x0108002f, /* 0108002f //C 0f510  1578 cmp	r0,'/' */
0x0000f511, 0xa1f2f526, /* a1f2f526 //C 0f511  1579 LS jmp	hc2v_nok */
0x0000f512, 0x01080039, /* 01080039 //C 0f512  1580 cmp	r0,'9' */
0x0000f513, 0xa1f2f523, /* a1f2f523 //C 0f513  1581 LS jmp	hc2v_0_9 */
0x0000f514, 0x01080040, /* 01080040 //C 0f514  1582 cmp	r0,'@' */
0x0000f515, 0xa1f2f526, /* a1f2f526 //C 0f515  1583 LS jmp	hc2v_nok */
0x0000f516, 0x01080046, /* 01080046 //C 0f516  1584 cmp	r0,'F' */
0x0000f517, 0xa1f2f520, /* a1f2f520 //C 0f517  1585 LS jmp	hc2v_A_F */
0x0000f518, 0x01080060, /* 01080060 //C 0f518  1586 cmp	r0,'`' */
0x0000f519, 0xa1f2f526, /* a1f2f526 //C 0f519  1587 LS jmp	hc2v_nok */
0x0000f51a, 0x01080066, /* 01080066 //C 0f51a  1588 cmp	r0,'f' */
0x0000f51b, 0xa1f2f51d, /* a1f2f51d //C 0f51b  1589 LS jmp	hc2v_a_f */
0x0000f51c, 0x01f2f526, /* 01f2f526 //C 0f51c  1590 jmp	hc2v_nok */
0x0000f51d, 0x0104000a, /* 0104000a //C 0f51d  1592 add	r0,10 */
0x0000f51e, 0x01060061, /* 01060061 //C 0f51e  1593 sub	r0,'a' */
0x0000f51f, 0x01f2f524, /* 01f2f524 //C 0f51f  1594 jmp	hc2v_ok */
0x0000f520, 0x0104000a, /* 0104000a //C 0f520  1596 add	r0,10 */
0x0000f521, 0x01060041, /* 01060041 //C 0f521  1597 sub	r0,'A' */
0x0000f522, 0x01f2f524, /* 01f2f524 //C 0f522  1598 jmp	hc2v_ok */
0x0000f523, 0x01060030, /* 01060030 //C 0f523  1600 sub	r0,'0' */
0x0000f524, 0x020c0000, /* 020c0000 //C 0f524  1602 sec */
0x0000f525, 0x00f00e00, /* 00f00e00 //C 0f525  1603 ret */
0x0000f526, 0x020d0000, /* 020d0000 //C 0f526  1605 clc */
0x0000f527, 0x00f00e00, /* 00f00e00 //C 0f527  1606 ret */
0x0000f528, 0x0d1d0000, /* 0d1d0000 //C 0f528  1613 push	r1 */
0x0000f529, 0x010f000f, /* 010f000f //C 0f529  1614 and	r0,0xf */
0x0000f52a, 0x0112f52e, /* 0112f52e //C 0f52a  1615 mvzl	r1,v2hc_table */
0x0000f52b, 0x0a010000, /* 0a010000 //C 0f52b  1616 ld	r0,r1,r0 */
0x0000f52c, 0x0f1d0000, /* 0f1d0000 //C 0f52c  1617 pop	r1 */
0x0000f52d, 0x00f00e00, /* 00f00e00 //C 0f52d  1618 ret */
0x0000f52e, 0x00000030, /* 00000030 //C 0f52e  1619 db	48 */
0x0000f52f, 0x00000031, /* 00000031 //C 0f52f  1619 db	49 */
0x0000f530, 0x00000032, /* 00000032 //C 0f530  1619 db	50 */
0x0000f531, 0x00000033, /* 00000033 //C 0f531  1619 db	51 */
0x0000f532, 0x00000034, /* 00000034 //C 0f532  1619 db	52 */
0x0000f533, 0x00000035, /* 00000035 //C 0f533  1619 db	53 */
0x0000f534, 0x00000036, /* 00000036 //C 0f534  1619 db	54 */
0x0000f535, 0x00000037, /* 00000037 //C 0f535  1619 db	55 */
0x0000f536, 0x00000038, /* 00000038 //C 0f536  1619 db	56 */
0x0000f537, 0x00000039, /* 00000039 //C 0f537  1619 db	57 */
0x0000f538, 0x00000041, /* 00000041 //C 0f538  1619 db	65 */
0x0000f539, 0x00000042, /* 00000042 //C 0f539  1619 db	66 */
0x0000f53a, 0x00000043, /* 00000043 //C 0f53a  1619 db	67 */
0x0000f53b, 0x00000044, /* 00000044 //C 0f53b  1619 db	68 */
0x0000f53c, 0x00000045, /* 00000045 //C 0f53c  1619 db	69 */
0x0000f53d, 0x00000046, /* 00000046 //C 0f53d  1619 db	70 */
0x0000f53e, 0x00000000, /* 00000000 //C 0f53e  1619 db */
0x0000f53f, 0x0ded0000, /* 0ded0000 //C 0f53f  1626 push	lr */
0x0000f540, 0x0400f528, /* 0400f528 //C 0f540  1627 call	value2Hexchar */
0x0000f541, 0x010d0020, /* 010d0020 //C 0f541  1628 or	r0,0x20 */
0x0000f542, 0x0ffd0000, /* 0ffd0000 //C 0f542  1629 pop	pc */
0x0000f543, 0x0ded0000, /* 0ded0000 //C 0f543  1637 push	lr */
0x0000f544, 0x0d2d0000, /* 0d2d0000 //C 0f544  1638 push	r2 */
0x0000f545, 0x0d3d0000, /* 0d3d0000 //C 0f545  1639 push	r3 */
0x0000f546, 0x0d4d0000, /* 0d4d0000 //C 0f546  1640 push	r4 */
0x0000f547, 0x0d5d0000, /* 0d5d0000 //C 0f547  1641 push	r5 */
0x0000f548, 0x01120000, /* 01120000 //C 0f548  1642 mvzl	r1,0		; return value */
0x0000f549, 0x01320000, /* 01320000 //C 0f549  1643 mvzl	r3,0		; index */
0x0000f54a, 0x01520000, /* 01520000 //C 0f54a  1645 mvzl	r5,0 */
0x0000f54b, 0x0b438000, /* 0b438000 //C 0f54b  1646 ld	r4,r3+,r0	; pick a char */
0x0000f54c, 0x024b0000, /* 024b0000 //C 0f54c  1647 sz	r4		; check eof string */
0x0000f54d, 0x11f2f569, /* 11f2f569 //C 0f54d  1648 jz	htoi_eos */
0x0000f54e, 0x06214405, /* 06214405 //C 0f54e  1650 getbz	r2,r4,r5 */
0x0000f54f, 0x022b0000, /* 022b0000 //C 0f54f  1651 sz	r2 */
0x0000f550, 0x11f2f54a, /* 11f2f54a //C 0f550  1652 jz	htoi_cyc */
0x0000f551, 0x0128002e, /* 0128002e //C 0f551  1653 cmp	r2,'.'		; skip separators */
0x0000f552, 0x11f2f563, /* 11f2f563 //C 0f552  1654 jz	htoi_next */
0x0000f553, 0x0128005f, /* 0128005f //C 0f553  1655 cmp	r2,'_' */
0x0000f554, 0x11f2f563, /* 11f2f563 //C 0f554  1656 jz	htoi_next */
0x0000f555, 0x0128002d, /* 0128002d //C 0f555  1657 cmp	r2,'-' */
0x0000f556, 0x11f2f563, /* 11f2f563 //C 0f556  1658 jz	htoi_next */
0x0000f557, 0x0d0d0000, /* 0d0d0000 //C 0f557  1659 push	r0 */
0x0000f558, 0x00000200, /* 00000200 //C 0f558  1660 mov	r0,r2 */
0x0000f559, 0x0400f510, /* 0400f510 //C 0f559  1661 call	hexchar2value */
0x0000f55a, 0x00200000, /* 00200000 //C 0f55a  1662 mov	r2,r0 */
0x0000f55b, 0x0f0d0000, /* 0f0d0000 //C 0f55b  1663 pop	r0 */
0x0000f55c, 0x41f2f567, /* 41f2f567 //C 0f55c  1664 C0 jmp	htoi_nok */
0x0000f55d, 0x02180000, /* 02180000 //C 0f55d  1665 shl	r1 */
0x0000f55e, 0x02180000, /* 02180000 //C 0f55e  1666 shl	r1 */
0x0000f55f, 0x02180000, /* 02180000 //C 0f55f  1667 shl	r1 */
0x0000f560, 0x02180000, /* 02180000 //C 0f560  1668 shl	r1 */
0x0000f561, 0x012f000f, /* 012f000f //C 0f561  1669 and	r2,0xf */
0x0000f562, 0x001d0200, /* 001d0200 //C 0f562  1670 or	r1,r2 */
0x0000f563, 0x01540001, /* 01540001 //C 0f563  1672 inc	r5 */
0x0000f564, 0x01580004, /* 01580004 //C 0f564  1673 cmp	r5,4 */
0x0000f565, 0x11f2f54a, /* 11f2f54a //C 0f565  1674 jz	htoi_cyc */
0x0000f566, 0x01f2f54e, /* 01f2f54e //C 0f566  1675 jmp	htoi_byte */
0x0000f567, 0x020d0000, /* 020d0000 //C 0f567  1677 clc */
0x0000f568, 0x01f2f56c, /* 01f2f56c //C 0f568  1678 jmp	htoi_ret */
0x0000f569, 0x01380001, /* 01380001 //C 0f569  1680 cmp	r3,1 */
0x0000f56a, 0x920d0000, /* 920d0000 //C 0f56a  1681 HI clc */
0x0000f56b, 0xa20c0000, /* a20c0000 //C 0f56b  1682 LS sec */
0x0000f56c, 0x0f5d0000, /* 0f5d0000 //C 0f56c  1684 pop	r5 */
0x0000f56d, 0x0f4d0000, /* 0f4d0000 //C 0f56d  1685 pop	r4 */
0x0000f56e, 0x0f3d0000, /* 0f3d0000 //C 0f56e  1686 pop	r3 */
0x0000f56f, 0x0f2d0000, /* 0f2d0000 //C 0f56f  1687 pop	r2 */
0x0000f570, 0x0ffd0000, /* 0ffd0000 //C 0f570  1688 pop	pc */
0x0000f571, 0x0ded0000, /* 0ded0000 //C 0f571  1695 push	lr */
0x0000f572, 0x0d0d0000, /* 0d0d0000 //C 0f572  1696 push	r0 */
0x0000f573, 0x0d1d0000, /* 0d1d0000 //C 0f573  1697 push	r1 */
0x0000f574, 0x020b0000, /* 020b0000 //C 0f574  1699 sz	r0 */
0x0000f575, 0x61f2f57d, /* 61f2f57d //C 0f575  1700 S0 jmp	itoa_pos */
0x0000f576, 0x0112002d, /* 0112002d //C 0f576  1702 mvzl	r1,'-' */
0x0000f577, 0x0610f5c7, /* 0610f5c7 //C 0f577  1703 st	r1,itoa_buffer */
0x0000f578, 0x0112f5c7, /* 0112f5c7 //C 0f578  1704 mvzl	r1,itoa_buffer */
0x0000f579, 0x01140001, /* 01140001 //C 0f579  1705 inc	r1 */
0x0000f57a, 0x02050000, /* 02050000 //C 0f57a  1706 neg	r0 */
0x0000f57b, 0x0400f59e, /* 0400f59e //C 0f57b  1707 call	bin2asc */
0x0000f57c, 0x01f2f57f, /* 01f2f57f //C 0f57c  1708 jmp	itoa_ret */
0x0000f57d, 0x0112f5c7, /* 0112f5c7 //C 0f57d  1710 mvzl	r1,itoa_buffer */
0x0000f57e, 0x0400f59e, /* 0400f59e //C 0f57e  1712 call	bin2asc */
0x0000f57f, 0x0f1d0000, /* 0f1d0000 //C 0f57f  1715 pop	r1 */
0x0000f580, 0x0f0d0000, /* 0f0d0000 //C 0f580  1716 pop	r0 */
0x0000f581, 0x0ffd0000, /* 0ffd0000 //C 0f581  1717 pop	pc */
0x0000f582, 0x0ded0000, /* 0ded0000 //C 0f582  1724 push	lr */
0x0000f583, 0x0d1d0000, /* 0d1d0000 //C 0f583  1725 push	r1 */
0x0000f584, 0x020b0000, /* 020b0000 //C 0f584  1726 sz	r0 */
0x0000f585, 0x61f2f588, /* 61f2f588 //C 0f585  1727 S0 jmp	itosa_pos */
0x0000f586, 0x0400f571, /* 0400f571 //C 0f586  1729 call	itoa */
0x0000f587, 0x01f2f58d, /* 01f2f58d //C 0f587  1730 jmp	itosa_ret */
0x0000f588, 0x0112002b, /* 0112002b //C 0f588  1732 mvzl	r1,'+' */
0x0000f589, 0x0610f5c7, /* 0610f5c7 //C 0f589  1733 st	r1,itoa_buffer */
0x0000f58a, 0x0112f5c7, /* 0112f5c7 //C 0f58a  1734 mvzl	r1,itoa_buffer */
0x0000f58b, 0x01140001, /* 01140001 //C 0f58b  1735 inc	r1 */
0x0000f58c, 0x0400f59e, /* 0400f59e //C 0f58c  1736 call	bin2asc */
0x0000f58d, 0x0f1d0000, /* 0f1d0000 //C 0f58d  1738 pop	r1 */
0x0000f58e, 0x0ffd0000, /* 0ffd0000 //C 0f58e  1739 pop	pc */
0x0000f58f, 0x0ded0000, /* 0ded0000 //C 0f58f  1746 push	lr */
0x0000f590, 0x0d1d0000, /* 0d1d0000 //C 0f590  1747 push	r1 */
0x0000f591, 0x0112f5c7, /* 0112f5c7 //C 0f591  1748 mvzl	r1,itoa_buffer */
0x0000f592, 0x0400f59e, /* 0400f59e //C 0f592  1749 call	bin2asc */
0x0000f593, 0x0f1d0000, /* 0f1d0000 //C 0f593  1750 pop	r1 */
0x0000f594, 0x0ffd0000, /* 0ffd0000 //C 0f594  1751 pop	pc */
0x0000f595, 0x0ded0000, /* 0ded0000 //C 0f595  1758 push	lr */
0x0000f596, 0x0d1d0000, /* 0d1d0000 //C 0f596  1759 push	r1 */
0x0000f597, 0x0112002b, /* 0112002b //C 0f597  1760 mvzl	r1,'+' */
0x0000f598, 0x0610f5c7, /* 0610f5c7 //C 0f598  1761 st	r1,itoa_buffer */
0x0000f599, 0x0112f5c7, /* 0112f5c7 //C 0f599  1762 mvzl	r1,itoa_buffer */
0x0000f59a, 0x01140001, /* 01140001 //C 0f59a  1763 inc	r1 */
0x0000f59b, 0x0400f59e, /* 0400f59e //C 0f59b  1764 call	bin2asc */
0x0000f59c, 0x0f1d0000, /* 0f1d0000 //C 0f59c  1765 pop	r1 */
0x0000f59d, 0x0ffd0000, /* 0ffd0000 //C 0f59d  1766 pop	pc */
0x0000f59e, 0x0ded0000, /* 0ded0000 //C 0f59e  1773 push	lr */
0x0000f59f, 0x0d0d0000, /* 0d0d0000 //C 0f59f  1774 push	r0 */
0x0000f5a0, 0x0d1d0000, /* 0d1d0000 //C 0f5a0  1775 push	r1 */
0x0000f5a1, 0x0d2d0000, /* 0d2d0000 //C 0f5a1  1776 push	r2 */
0x0000f5a2, 0x0d3d0000, /* 0d3d0000 //C 0f5a2  1777 push	r3 */
0x0000f5a3, 0x0dad0000, /* 0dad0000 //C 0f5a3  1778 push	r10 */
0x0000f5a4, 0x0dbd0000, /* 0dbd0000 //C 0f5a4  1779 push	r11 */
0x0000f5a5, 0x0dcd0000, /* 0dcd0000 //C 0f5a5  1780 push	r12 */
0x0000f5a6, 0x00c00100, /* 00c00100 //C 0f5a6  1782 mov	r12,r1		; pointer to output buffer */
0x0000f5a7, 0x01b2f5e8, /* 01b2f5e8 //C 0f5a7  1783 mvzl	r11,itoa_divs	; pointer to dividers */
0x0000f5a8, 0x01a20000, /* 01a20000 //C 0f5a8  1784 mvzl	r10,0		; bool: first non-zero char found */
0x0000f5a9, 0x0cac0000, /* 0cac0000 //C 0f5a9  1785 st	r10,r12		; start to produce an empty string */
0x0000f5aa, 0x0e1b0000, /* 0e1b0000 //C 0f5aa  1787 ld	r1,r11		; get next divider */
0x0000f5ab, 0x021b0000, /* 021b0000 //C 0f5ab  1788 sz	r1		; if 0, then */
0x0000f5ac, 0x11f2f5bf, /* 11f2f5bf //C 0f5ac  1789 jz	b2a_ret		; finish */
0x0000f5ad, 0x01180001, /* 01180001 //C 0f5ad  1790 cmp	r1,1		; last divider? */
0x0000f5ae, 0x11a20001, /* 11a20001 //C 0f5ae  1791 EQ mvzl	r10,1		; always print last char */
0x0000f5af, 0x0400f3e9, /* 0400f3e9 //C 0f5af  1792 call	div		; R2,R3= R0/R1 */
0x0000f5b0, 0x022b0000, /* 022b0000 //C 0f5b0  1793 sz	r2		; is the result zero? */
0x0000f5b1, 0x11f2f5bc, /* 11f2f5bc //C 0f5b1  1794 jz	b2a_f0 */
0x0000f5b2, 0x01a20001, /* 01a20001 //C 0f5b2  1796 mvzl	r10,1		; non-zero: start to print */
0x0000f5b3, 0x00000200, /* 00000200 //C 0f5b3  1798 mov	r0,r2		; convert result to ASCII char */
0x0000f5b4, 0x0400f53f, /* 0400f53f //C 0f5b4  1799 call	value2hexchar */
0x0000f5b5, 0x0c0c0000, /* 0c0c0000 //C 0f5b5  1800 st	r0,r12		; and store it in buffer */
0x0000f5b6, 0x01c40001, /* 01c40001 //C 0f5b6  1801 inc	r12		; inc buf ptr */
0x0000f5b7, 0x01020000, /* 01020000 //C 0f5b7  1802 mvzl	r0,0		; put 0 after last char */
0x0000f5b8, 0x0c0c0000, /* 0c0c0000 //C 0f5b8  1803 st	r0,r12 */
0x0000f5b9, 0x00000300, /* 00000300 //C 0f5b9  1805 mov	r0,r3		; continue with the reminder */
0x0000f5ba, 0x01b40001, /* 01b40001 //C 0f5ba  1806 inc	r11		; and next divider */
0x0000f5bb, 0x01f2f5aa, /* 01f2f5aa //C 0f5bb  1807 jmp	b2a_cyc */
0x0000f5bc, 0x02ab0000, /* 02ab0000 //C 0f5bc  1809 sz	r10		; just zeros so far? */
0x0000f5bd, 0x21f2f5b3, /* 21f2f5b3 //C 0f5bd  1810 jnz	b2a_store	; no, print */
0x0000f5be, 0x01f2f5b9, /* 01f2f5b9 //C 0f5be  1811 jmp	b2a_next */
0x0000f5bf, 0x0fcd0000, /* 0fcd0000 //C 0f5bf  1813 pop	r12 */
0x0000f5c0, 0x0fbd0000, /* 0fbd0000 //C 0f5c0  1814 pop	r11 */
0x0000f5c1, 0x0fad0000, /* 0fad0000 //C 0f5c1  1815 pop	r10 */
0x0000f5c2, 0x0f3d0000, /* 0f3d0000 //C 0f5c2  1816 pop	r3 */
0x0000f5c3, 0x0f2d0000, /* 0f2d0000 //C 0f5c3  1817 pop	r2 */
0x0000f5c4, 0x0f1d0000, /* 0f1d0000 //C 0f5c4  1818 pop	r1 */
0x0000f5c5, 0x0f0d0000, /* 0f0d0000 //C 0f5c5  1819 pop	r0 */
0x0000f5c6, 0x0ffd0000, /* 0ffd0000 //C 0f5c6  1820 pop	pc */
0x0000f5c7, 0x00000000, /* 00000000 //C 0f5c7  1822 itoa_buffer:	ds	33 */
0x0000f5e8, 0x3b9aca00, /* 3b9aca00 //C 0f5e8  1824 dd	1000000000 */
0x0000f5e9, 0x05f5e100, /* 05f5e100 //C 0f5e9  1825 dd	100000000 */
0x0000f5ea, 0x00989680, /* 00989680 //C 0f5ea  1826 dd	10000000 */
0x0000f5eb, 0x000f4240, /* 000f4240 //C 0f5eb  1827 dd	1000000 */
0x0000f5ec, 0x000186a0, /* 000186a0 //C 0f5ec  1828 dd	100000 */
0x0000f5ed, 0x00002710, /* 00002710 //C 0f5ed  1829 dd	10000 */
0x0000f5ee, 0x000003e8, /* 000003e8 //C 0f5ee  1830 dd	1000 */
0x0000f5ef, 0x00000064, /* 00000064 //C 0f5ef  1831 dd	100 */
0x0000f5f0, 0x0000000a, /* 0000000a //C 0f5f0  1832 dd	10 */
0x0000f5f1, 0x00000001, /* 00000001 //C 0f5f1  1833 dd	1 */
0x0000f5f2, 0x00000000, /* 00000000 //C 0f5f2  1834 dd	0 */
0x0000f5f3, 0x0d0d0000, /* 0d0d0000 //C 0f5f3  1844 push	r0 */
0x0000f5f4, 0x0d1d0000, /* 0d1d0000 //C 0f5f4  1845 push	r1 */
0x0000f5f5, 0x0d2d0000, /* 0d2d0000 //C 0f5f5  1846 push	r2 */
0x0000f5f6, 0x0d3d0000, /* 0d3d0000 //C 0f5f6  1847 push	r3 */
0x0000f5f7, 0x0d4d0000, /* 0d4d0000 //C 0f5f7  1848 push	r4 */
0x0000f5f8, 0x0d5d0000, /* 0d5d0000 //C 0f5f8  1849 push	r5 */
0x0000f5f9, 0x0112f5c7, /* 0112f5c7 //C 0f5f9  1850 mvzl	r1,itoa_buffer	; output ptr */
0x0000f5fa, 0x01220000, /* 01220000 //C 0f5fa  1851 mvzl	r2,0		; is printing started */
0x0000f5fb, 0x01320007, /* 01320007 //C 0f5fb  1852 mvzl	r3,7		; cycle variable */
0x0000f5fc, 0x0152f52e, /* 0152f52e //C 0f5fc  1853 mvzl	r5,v2hc_table */
0x0000f5fd, 0x01420000, /* 01420000 //C 0f5fd  1855 mvzl	r4,0		; pick next char to R4 */
0x0000f5fe, 0x02080000, /* 02080000 //C 0f5fe  1856 shl	r0 */
0x0000f5ff, 0x02470000, /* 02470000 //C 0f5ff  1857 rol	r4 */
0x0000f600, 0x02080000, /* 02080000 //C 0f600  1858 shl	r0 */
0x0000f601, 0x02470000, /* 02470000 //C 0f601  1859 rol	r4 */
0x0000f602, 0x02080000, /* 02080000 //C 0f602  1860 shl	r0 */
0x0000f603, 0x02470000, /* 02470000 //C 0f603  1861 rol	r4 */
0x0000f604, 0x02080000, /* 02080000 //C 0f604  1862 shl	r0 */
0x0000f605, 0x02470000, /* 02470000 //C 0f605  1863 rol	r4 */
0x0000f606, 0x023b0000, /* 023b0000 //C 0f606  1864 sz	r3		; last char is always printed */
0x0000f607, 0x11f2f60d, /* 11f2f60d //C 0f607  1865 jz	utoh_print */
0x0000f608, 0x022b0000, /* 022b0000 //C 0f608  1866 sz	r2		; printing? */
0x0000f609, 0x21f2f60d, /* 21f2f60d //C 0f609  1867 true	utoh_print */
0x0000f60a, 0x024b0000, /* 024b0000 //C 0f60a  1868 sz	r4		; should start print? */
0x0000f60b, 0x11f2f613, /* 11f2f613 //C 0f60b  1869 jz	utoh_next */
0x0000f60c, 0x01220001, /* 01220001 //C 0f60c  1870 mvzl	r2,1 */
0x0000f60d, 0x0a450400, /* 0a450400 //C 0f60d  1872 ld	r4,r5,r4	; uppercase hex char */
0x0000f60e, 0x014d0020, /* 014d0020 //C 0f60e  1873 or	r4,0x20		; convert to lowercase */
0x0000f60f, 0x0c410000, /* 0c410000 //C 0f60f  1874 st	r4,r1 */
0x0000f610, 0x01140001, /* 01140001 //C 0f610  1875 inc	r1 */
0x0000f611, 0x01420000, /* 01420000 //C 0f611  1876 mvzl	r4,0 */
0x0000f612, 0x0c410000, /* 0c410000 //C 0f612  1877 st	r4,r1 */
0x0000f613, 0x023b0000, /* 023b0000 //C 0f613  1879 sz	r3 */
0x0000f614, 0x11f2f617, /* 11f2f617 //C 0f614  1880 jz	utoh_ret */
0x0000f615, 0x0134ffff, /* 0134ffff //C 0f615  1881 dec	r3 */
0x0000f616, 0x01f2f5fd, /* 01f2f5fd //C 0f616  1882 jmp	utoh_cyc */
0x0000f617, 0x0f5d0000, /* 0f5d0000 //C 0f617  1884 pop	r5 */
0x0000f618, 0x0f4d0000, /* 0f4d0000 //C 0f618  1885 pop	r4 */
0x0000f619, 0x0f3d0000, /* 0f3d0000 //C 0f619  1886 pop	r3 */
0x0000f61a, 0x0f2d0000, /* 0f2d0000 //C 0f61a  1887 pop	r2 */
0x0000f61b, 0x0f1d0000, /* 0f1d0000 //C 0f61b  1888 pop	r1 */
0x0000f61c, 0x0f0d0000, /* 0f0d0000 //C 0f61c  1889 pop	r0 */
0x0000f61d, 0x00f00e00, /* 00f00e00 //C 0f61d  1890 ret */
0x0000f61e, 0x0d0d0000, /* 0d0d0000 //C 0f61e  1894 push	r0 */
0x0000f61f, 0x0d1d0000, /* 0d1d0000 //C 0f61f  1895 push	r1 */
0x0000f620, 0x0d2d0000, /* 0d2d0000 //C 0f620  1896 push	r2 */
0x0000f621, 0x0d3d0000, /* 0d3d0000 //C 0f621  1897 push	r3 */
0x0000f622, 0x0d4d0000, /* 0d4d0000 //C 0f622  1898 push	r4 */
0x0000f623, 0x0112f5c7, /* 0112f5c7 //C 0f623  1899 mvzl	r1,itoa_buffer	; output ptr */
0x0000f624, 0x01320007, /* 01320007 //C 0f624  1900 mvzl	r3,7		; cycle variable */
0x0000f625, 0x0122f52e, /* 0122f52e //C 0f625  1901 mvzl	r2,v2hc_table */
0x0000f626, 0x01420000, /* 01420000 //C 0f626  1903 mvzl	r4,0		; pick next char to R4 */
0x0000f627, 0x02080000, /* 02080000 //C 0f627  1904 shl	r0 */
0x0000f628, 0x02470000, /* 02470000 //C 0f628  1905 rol	r4 */
0x0000f629, 0x02080000, /* 02080000 //C 0f629  1906 shl	r0 */
0x0000f62a, 0x02470000, /* 02470000 //C 0f62a  1907 rol	r4 */
0x0000f62b, 0x02080000, /* 02080000 //C 0f62b  1908 shl	r0 */
0x0000f62c, 0x02470000, /* 02470000 //C 0f62c  1909 rol	r4 */
0x0000f62d, 0x02080000, /* 02080000 //C 0f62d  1910 shl	r0 */
0x0000f62e, 0x02470000, /* 02470000 //C 0f62e  1911 rol	r4 */
0x0000f62f, 0x0a420400, /* 0a420400 //C 0f62f  1912 ld	r4,r2,r4	; uppercase hex char */
0x0000f630, 0x014d0020, /* 014d0020 //C 0f630  1913 or	r4,0x20		; convert to lowercase */
0x0000f631, 0x0c410000, /* 0c410000 //C 0f631  1914 st	r4,r1 */
0x0000f632, 0x01140001, /* 01140001 //C 0f632  1915 inc	r1 */
0x0000f633, 0x01420000, /* 01420000 //C 0f633  1916 mvzl	r4,0 */
0x0000f634, 0x0c410000, /* 0c410000 //C 0f634  1917 st	r4,r1 */
0x0000f635, 0x023b0000, /* 023b0000 //C 0f635  1919 sz	r3 */
0x0000f636, 0x11f2f639, /* 11f2f639 //C 0f636  1920 jz	utoh32_ret */
0x0000f637, 0x0134ffff, /* 0134ffff //C 0f637  1921 dec	r3 */
0x0000f638, 0x01f2f626, /* 01f2f626 //C 0f638  1922 jmp	utoh32_cyc */
0x0000f639, 0x0f4d0000, /* 0f4d0000 //C 0f639  1924 pop	r4 */
0x0000f63a, 0x0f3d0000, /* 0f3d0000 //C 0f63a  1925 pop	r3 */
0x0000f63b, 0x0f2d0000, /* 0f2d0000 //C 0f63b  1926 pop	r2 */
0x0000f63c, 0x0f1d0000, /* 0f1d0000 //C 0f63c  1927 pop	r1 */
0x0000f63d, 0x0f0d0000, /* 0f0d0000 //C 0f63d  1928 pop	r0 */
0x0000f63e, 0x00f00e00, /* 00f00e00 //C 0f63e  1929 ret */
0x0000f63f, 0x0d0d0000, /* 0d0d0000 //C 0f63f  1933 push	r0 */
0x0000f640, 0x0d1d0000, /* 0d1d0000 //C 0f640  1934 push	r1 */
0x0000f641, 0x0d2d0000, /* 0d2d0000 //C 0f641  1935 push	r2 */
0x0000f642, 0x0d3d0000, /* 0d3d0000 //C 0f642  1936 push	r3 */
0x0000f643, 0x0d4d0000, /* 0d4d0000 //C 0f643  1937 push	r4 */
0x0000f644, 0x0112f5c7, /* 0112f5c7 //C 0f644  1938 mvzl	r1,itoa_buffer	; output ptr */
0x0000f645, 0x01320007, /* 01320007 //C 0f645  1939 mvzl	r3,7		; cycle variable */
0x0000f646, 0x0122f52e, /* 0122f52e //C 0f646  1940 mvzl	r2,v2hc_table */
0x0000f647, 0x01380003, /* 01380003 //C 0f647  1942 cmp	r3,3 */
0x0000f648, 0x1142005f, /* 1142005f //C 0f648  1943 Z mvzl	r4,'_' */
0x0000f649, 0x1c410000, /* 1c410000 //C 0f649  1944 Z st	r4,r1 */
0x0000f64a, 0x11140001, /* 11140001 //C 0f64a  1945 Z inc	r1 */
0x0000f64b, 0x01420000, /* 01420000 //C 0f64b  1946 mvzl	r4,0		; pick next char to R4 */
0x0000f64c, 0x02080000, /* 02080000 //C 0f64c  1947 shl	r0 */
0x0000f64d, 0x02470000, /* 02470000 //C 0f64d  1948 rol	r4 */
0x0000f64e, 0x02080000, /* 02080000 //C 0f64e  1949 shl	r0 */
0x0000f64f, 0x02470000, /* 02470000 //C 0f64f  1950 rol	r4 */
0x0000f650, 0x02080000, /* 02080000 //C 0f650  1951 shl	r0 */
0x0000f651, 0x02470000, /* 02470000 //C 0f651  1952 rol	r4 */
0x0000f652, 0x02080000, /* 02080000 //C 0f652  1953 shl	r0 */
0x0000f653, 0x02470000, /* 02470000 //C 0f653  1954 rol	r4 */
0x0000f654, 0x0a420400, /* 0a420400 //C 0f654  1955 ld	r4,r2,r4	; uppercase hex char */
0x0000f655, 0x0c410000, /* 0c410000 //C 0f655  1957 st	r4,r1 */
0x0000f656, 0x01140001, /* 01140001 //C 0f656  1958 inc	r1 */
0x0000f657, 0x01420000, /* 01420000 //C 0f657  1959 mvzl	r4,0 */
0x0000f658, 0x0c410000, /* 0c410000 //C 0f658  1960 st	r4,r1 */
0x0000f659, 0x023b0000, /* 023b0000 //C 0f659  1962 sz	r3 */
0x0000f65a, 0x11f2f65d, /* 11f2f65d //C 0f65a  1963 jz	ptoa_ret */
0x0000f65b, 0x0134ffff, /* 0134ffff //C 0f65b  1964 dec	r3 */
0x0000f65c, 0x01f2f647, /* 01f2f647 //C 0f65c  1965 jmp	ptoa_cyc */
0x0000f65d, 0x0f4d0000, /* 0f4d0000 //C 0f65d  1967 pop	r4 */
0x0000f65e, 0x0f3d0000, /* 0f3d0000 //C 0f65e  1968 pop	r3 */
0x0000f65f, 0x0f2d0000, /* 0f2d0000 //C 0f65f  1969 pop	r2 */
0x0000f660, 0x0f1d0000, /* 0f1d0000 //C 0f660  1970 pop	r1 */
0x0000f661, 0x0f0d0000, /* 0f0d0000 //C 0f661  1971 pop	r0 */
0x0000f662, 0x00f00e00, /* 00f00e00 //C 0f662  1972 ret */
0x0000f663, 0x0d0d0000, /* 0d0d0000 //C 0f663  1979 push	r0 */
0x0000f664, 0x0d1d0000, /* 0d1d0000 //C 0f664  1980 push	r1 */
0x0000f665, 0x0d2d0000, /* 0d2d0000 //C 0f665  1981 push	r2 */
0x0000f666, 0x0d3d0000, /* 0d3d0000 //C 0f666  1982 push	r3 */
0x0000f667, 0x0112f5c7, /* 0112f5c7 //C 0f667  1983 mvzl	r1,itoa_buffer */
0x0000f668, 0x01220000, /* 01220000 //C 0f668  1984 mvzl	r2,0 */
0x0000f669, 0x02080000, /* 02080000 //C 0f669  1986 shl	r0 */
0x0000f66a, 0x31320031, /* 31320031 //C 0f66a  1987 C mvzl	r3,'1' */
0x0000f66b, 0x41320030, /* 41320030 //C 0f66b  1988 NC mvzl	r3,'0' */
0x0000f66c, 0x09328100, /* 09328100 //C 0f66c  1989 st	r3,r2+,r1 */
0x0000f66d, 0x01280020, /* 01280020 //C 0f66d  1990 cmp	r2,32 */
0x0000f66e, 0x21f2f669, /* 21f2f669 //C 0f66e  1991 jnz	btoa_cyc */
0x0000f66f, 0x01320000, /* 01320000 //C 0f66f  1992 mvzl	r3,0 */
0x0000f670, 0x08320100, /* 08320100 //C 0f670  1993 st	r3,r2,r1 */
0x0000f671, 0x0f3d0000, /* 0f3d0000 //C 0f671  1994 pop	r3 */
0x0000f672, 0x0f2d0000, /* 0f2d0000 //C 0f672  1995 pop	r2 */
0x0000f673, 0x0f1d0000, /* 0f1d0000 //C 0f673  1996 pop	r1 */
0x0000f674, 0x0f0d0000, /* 0f0d0000 //C 0f674  1997 pop	r0 */
0x0000f675, 0x00f00e00, /* 00f00e00 //C 0f675  1998 ret */
0x0000f676, 0x0d0d0000, /* 0d0d0000 //C 0f676  2009 push	r0 */
0x0000f677, 0x0700ff42, /* 0700ff42 //C 0f677  2010 ld	r0,UART_RSTAT */
0x0000f678, 0x010c0001, /* 010c0001 //C 0f678  2012 test	r0,1		; check if queue is not empty */
0x0000f679, 0x020d0000, /* 020d0000 //C 0f679  2013 clc */
0x0000f67a, 0x220c0000, /* 220c0000 //C 0f67a  2014 Z0 sec */
0x0000f67b, 0x0f0d0000, /* 0f0d0000 //C 0f67b  2015 pop	r0 */
0x0000f67c, 0x30f00e00, /* 30f00e00 //C 0f67c  2016 C1 ret */
0x0000f67d, 0x00f00e00, /* 00f00e00 //C 0f67d  2017 ret */
0x0000f67e, 0x0700ff40, /* 0700ff40 //C 0f67e  2023 ld	r0,UART_DR */
0x0000f67f, 0x0600ff46, /* 0600ff46 //C 0f67f  2024 st	r0,UART_IRA */
0x0000f680, 0x00f00e00, /* 00f00e00 //C 0f680  2025 ret */
0x0000f681, 0x0400f676, /* 0400f676 //C 0f681  2034 call	check_uart */
0x0000f682, 0x41f2f681, /* 41f2f681 //C 0f682  2035 C0 jmp	getchar */
0x0000f683, 0x0400f67e, /* 0400f67e //C 0f683  2036 call	read */
0x0000f684, 0x00f00e00, /* 00f00e00 //C 0f684  2037 ret */
0x0000f685, 0x0d9d0000, /* 0d9d0000 //C 0f685  2045 push	r9 */
0x0000f686, 0x0790ff43, /* 0790ff43 //C 0f686  2052 ld	r9,UART_TSTAT */
0x0000f687, 0x019c0001, /* 019c0001 //C 0f687  2053 test	r9,1 */
0x0000f688, 0x11f2f686, /* 11f2f686 //C 0f688  2054 jz	wait_tc */
0x0000f689, 0x0600ff40, /* 0600ff40 //C 0f689  2055 st	r0,UART_DR */
0x0000f68a, 0x0f9d0000, /* 0f9d0000 //C 0f68a  2057 pop	r9 */
0x0000f68b, 0x00f00e00, /* 00f00e00 //C 0f68b  2058 ret */
0x0000f68c, 0x0ded0000, /* 0ded0000 //C 0f68c  2066 push	lr */
0x0000f68d, 0x0d0d0000, /* 0d0d0000 //C 0f68d  2067 push	r0 */
0x0000f68e, 0x0d3d0000, /* 0d3d0000 //C 0f68e  2068 push	r3 */
0x0000f68f, 0x0d4d0000, /* 0d4d0000 //C 0f68f  2069 push	r4 */
0x0000f690, 0x0d2d0000, /* 0d2d0000 //C 0f690  2070 push	r2 */
0x0000f691, 0x0d1d0000, /* 0d1d0000 //C 0f691  2071 push	r1 */
0x0000f692, 0x0d5d0000, /* 0d5d0000 //C 0f692  2072 push	r5 */
0x0000f693, 0x01420000, /* 01420000 //C 0f693  2074 mvzl	r4,0 */
0x0000f694, 0x020b0000, /* 020b0000 //C 0f694  2075 sz	r0 */
0x0000f695, 0x1102f8b1, /* 1102f8b1 //C 0f695  2076 Z1 mvzl	r0,null_str */
0x0000f696, 0x00200000, /* 00200000 //C 0f696  2077 mov	r2,r0 */
0x0000f697, 0x0b148200, /* 0b148200 //C 0f697  2079 ld	r1,r4+,r2 */
0x0000f698, 0x021b0000, /* 021b0000 //C 0f698  2080 sz	r1 */
0x0000f699, 0x01520000, /* 01520000 //C 0f699  2081 mvzl	r5,0 */
0x0000f69a, 0x11f2f6a3, /* 11f2f6a3 //C 0f69a  2082 jz	prints_done */
0x0000f69b, 0x06014105, /* 06014105 //C 0f69b  2084 getbz	r0,r1,r5 */
0x0000f69c, 0x020b0000, /* 020b0000 //C 0f69c  2085 sz	r0 */
0x0000f69d, 0x2400f685, /* 2400f685 //C 0f69d  2086 NZ call	putchar */
0x0000f69e, 0x11f2f697, /* 11f2f697 //C 0f69e  2087 jz 	prints_go */
0x0000f69f, 0x01540001, /* 01540001 //C 0f69f  2088 inc	r5 */
0x0000f6a0, 0x01580004, /* 01580004 //C 0f6a0  2089 cmp	r5,4 */
0x0000f6a1, 0x21f2f69b, /* 21f2f69b //C 0f6a1  2090 jnz	prints_byte */
0x0000f6a2, 0x01f2f697, /* 01f2f697 //C 0f6a2  2091 jmp	prints_go */
0x0000f6a3, 0x0f5d0000, /* 0f5d0000 //C 0f6a3  2094 pop	r5 */
0x0000f6a4, 0x0f1d0000, /* 0f1d0000 //C 0f6a4  2095 pop	r1 */
0x0000f6a5, 0x0f2d0000, /* 0f2d0000 //C 0f6a5  2096 pop	r2 */
0x0000f6a6, 0x0f4d0000, /* 0f4d0000 //C 0f6a6  2097 pop	r4 */
0x0000f6a7, 0x0f3d0000, /* 0f3d0000 //C 0f6a7  2098 pop	r3 */
0x0000f6a8, 0x0f0d0000, /* 0f0d0000 //C 0f6a8  2099 pop	r0 */
0x0000f6a9, 0x0ffd0000, /* 0ffd0000 //C 0f6a9  2100 pop	pc */
0x0000f6aa, 0x0d0d0000, /* 0d0d0000 //C 0f6aa  2110 push	r0 */
0x0000f6ab, 0x0d1d0000, /* 0d1d0000 //C 0f6ab  2111 push	r1 */
0x0000f6ac, 0x0d2d0000, /* 0d2d0000 //C 0f6ac  2112 push	r2 */
0x0000f6ad, 0x0d3d0000, /* 0d3d0000 //C 0f6ad  2113 push	r3 */
0x0000f6ae, 0x00100e00, /* 00100e00 //C 0f6ae  2114 mov	r1,lr */
0x0000f6af, 0x0e210000, /* 0e210000 //C 0f6af  2116 ld	r2,r1 */
0x0000f6b0, 0x01140001, /* 01140001 //C 0f6b0  2117 inc	r1 */
0x0000f6b1, 0x022b0000, /* 022b0000 //C 0f6b1  2118 sz	r2 */
0x0000f6b2, 0x11f2f6bc, /* 11f2f6bc //C 0f6b2  2119 jz	pes_done */
0x0000f6b3, 0x01320000, /* 01320000 //C 0f6b3  2120 mvzl	r3,0 */
0x0000f6b4, 0x06014203, /* 06014203 //C 0f6b4  2122 getbz	r0,r2,r3 */
0x0000f6b5, 0x020b0000, /* 020b0000 //C 0f6b5  2123 sz	r0 */
0x0000f6b6, 0x2400f685, /* 2400f685 //C 0f6b6  2124 NZ call	putchar */
0x0000f6b7, 0x11f2f6af, /* 11f2f6af //C 0f6b7  2125 jz	pes_next */
0x0000f6b8, 0x01340001, /* 01340001 //C 0f6b8  2126 inc	r3 */
0x0000f6b9, 0x01380004, /* 01380004 //C 0f6b9  2127 cmp	r3,4 */
0x0000f6ba, 0x21f2f6b4, /* 21f2f6b4 //C 0f6ba  2128 jnz	pes_byte */
0x0000f6bb, 0x01f2f6af, /* 01f2f6af //C 0f6bb  2129 jmp	pes_next */
0x0000f6bc, 0x00e00100, /* 00e00100 //C 0f6bc  2131 mov	LR,r1 */
0x0000f6bd, 0x0f3d0000, /* 0f3d0000 //C 0f6bd  2132 pop	r3 */
0x0000f6be, 0x0f2d0000, /* 0f2d0000 //C 0f6be  2133 pop	r2 */
0x0000f6bf, 0x0f1d0000, /* 0f1d0000 //C 0f6bf  2134 pop	r1 */
0x0000f6c0, 0x0f0d0000, /* 0f0d0000 //C 0f6c0  2135 pop	r0 */
0x0000f6c1, 0x00f00e00, /* 00f00e00 //C 0f6c1  2136 ret */
0x0000f6c2, 0x0ded0000, /* 0ded0000 //C 0f6c2  2144 push	lr */
0x0000f6c3, 0x0400f68c, /* 0400f68c //C 0f6c3  2145 call	prints */
0x0000f6c4, 0x0d0d0000, /* 0d0d0000 //C 0f6c4  2146 push	r0 */
0x0000f6c5, 0x0102000a, /* 0102000a //C 0f6c5  2147 mvzl	r0,LF */
0x0000f6c6, 0x0400f685, /* 0400f685 //C 0f6c6  2148 call	putchar */
0x0000f6c7, 0x0f0d0000, /* 0f0d0000 //C 0f6c7  2149 pop	r0 */
0x0000f6c8, 0x0ffd0000, /* 0ffd0000 //C 0f6c8  2150 pop	pc */
0x0000f6c9, 0x0ded0000, /* 0ded0000 //C 0f6c9  2157 push	lr */
0x0000f6ca, 0x0d0d0000, /* 0d0d0000 //C 0f6ca  2158 push	r0 */
0x0000f6cb, 0x0d1d0000, /* 0d1d0000 //C 0f6cb  2159 push	r1 */
0x0000f6cc, 0x0d2d0000, /* 0d2d0000 //C 0f6cc  2160 push	r2 */
0x0000f6cd, 0x0d3d0000, /* 0d3d0000 //C 0f6cd  2161 push	r3 */
0x0000f6ce, 0x0d4d0000, /* 0d4d0000 //C 0f6ce  2162 push	r4 */
0x0000f6cf, 0x00300000, /* 00300000 //C 0f6cf  2163 mov	r3,r0 */
0x0000f6d0, 0x01220000, /* 01220000 //C 0f6d0  2164 mvzl	r2,0 */
0x0000f6d1, 0x01420001, /* 01420001 //C 0f6d1  2165 mvzl	r4,1 */
0x0000f6d2, 0x01020000, /* 01020000 //C 0f6d2  2168 mvzl	r0,0 */
0x0000f6d3, 0x02380000, /* 02380000 //C 0f6d3  2169 shl	r3 */
0x0000f6d4, 0x02070000, /* 02070000 //C 0f6d4  2170 rol	r0 */
0x0000f6d5, 0x02380000, /* 02380000 //C 0f6d5  2171 shl	r3 */
0x0000f6d6, 0x02070000, /* 02070000 //C 0f6d6  2172 rol	r0 */
0x0000f6d7, 0x02380000, /* 02380000 //C 0f6d7  2173 shl	r3 */
0x0000f6d8, 0x02070000, /* 02070000 //C 0f6d8  2174 rol	r0 */
0x0000f6d9, 0x02380000, /* 02380000 //C 0f6d9  2175 shl	r3 */
0x0000f6da, 0x02070000, /* 02070000 //C 0f6da  2176 rol	r0 */
0x0000f6db, 0x0400f528, /* 0400f528 //C 0f6db  2177 call	value2Hexchar */
0x0000f6dc, 0x0400f685, /* 0400f685 //C 0f6dc  2178 call	putchar */
0x0000f6dd, 0x01240001, /* 01240001 //C 0f6dd  2179 add	r2,1 */
0x0000f6de, 0x01280008, /* 01280008 //C 0f6de  2180 cmp	r2,8 */
0x0000f6df, 0x11f2f6e9, /* 11f2f6e9 //C 0f6df  2181 jz	print_vhex_ret */
0x0000f6e0, 0x021b0000, /* 021b0000 //C 0f6e0  2182 sz	r1 */
0x0000f6e1, 0x11f2f6e7, /* 11f2f6e7 //C 0f6e1  2183 jz	print_vhex_nosep */
0x0000f6e2, 0x00480100, /* 00480100 //C 0f6e2  2184 cmp	r4,r1 */
0x0000f6e3, 0x21f2f6e7, /* 21f2f6e7 //C 0f6e3  2185 jnz	print_vhex_nosep */
0x0000f6e4, 0x0102005f, /* 0102005f //C 0f6e4  2186 mvzl	r0,'_' */
0x0000f6e5, 0x0400f685, /* 0400f685 //C 0f6e5  2187 call	putchar */
0x0000f6e6, 0x01420000, /* 01420000 //C 0f6e6  2188 mvzl	r4,0 */
0x0000f6e7, 0x01440001, /* 01440001 //C 0f6e7  2190 add	r4,1 */
0x0000f6e8, 0x01f2f6d2, /* 01f2f6d2 //C 0f6e8  2191 jmp	print_vhex_cyc */
0x0000f6e9, 0x0f4d0000, /* 0f4d0000 //C 0f6e9  2193 pop	r4 */
0x0000f6ea, 0x0f3d0000, /* 0f3d0000 //C 0f6ea  2194 pop	r3 */
0x0000f6eb, 0x0f2d0000, /* 0f2d0000 //C 0f6eb  2195 pop	r2 */
0x0000f6ec, 0x0f1d0000, /* 0f1d0000 //C 0f6ec  2196 pop	r1 */
0x0000f6ed, 0x0f0d0000, /* 0f0d0000 //C 0f6ed  2197 pop	r0 */
0x0000f6ee, 0x0ffd0000, /* 0ffd0000 //C 0f6ee  2198 pop	pc */
0x0000f6ef, 0x0ded0000, /* 0ded0000 //C 0f6ef  2205 push	lr */
0x0000f6f0, 0x0400f571, /* 0400f571 //C 0f6f0  2206 call	itoa */
0x0000f6f1, 0x0102f5c7, /* 0102f5c7 //C 0f6f1  2207 mvzl	r0,itoa_buffer */
0x0000f6f2, 0x0400f68c, /* 0400f68c //C 0f6f2  2208 call	prints */
0x0000f6f3, 0x0ffd0000, /* 0ffd0000 //C 0f6f3  2209 pop	pc */
0x0000f6f4, 0x0ded0000, /* 0ded0000 //C 0f6f4  2216 push	lr */
0x0000f6f5, 0x0d0d0000, /* 0d0d0000 //C 0f6f5  2217 push	r0 */
0x0000f6f6, 0x0d1d0000, /* 0d1d0000 //C 0f6f6  2218 push	r1 */
0x0000f6f7, 0x0112f5c7, /* 0112f5c7 //C 0f6f7  2219 mvzl	r1,itoa_buffer */
0x0000f6f8, 0x0400f59e, /* 0400f59e //C 0f6f8  2220 call	bin2asc */
0x0000f6f9, 0x0102f5c7, /* 0102f5c7 //C 0f6f9  2221 mvzl	r0,itoa_buffer */
0x0000f6fa, 0x0400f68c, /* 0400f68c //C 0f6fa  2222 call	prints */
0x0000f6fb, 0x0f1d0000, /* 0f1d0000 //C 0f6fb  2223 pop	r1 */
0x0000f6fc, 0x0f0d0000, /* 0f0d0000 //C 0f6fc  2224 pop	r0 */
0x0000f6fd, 0x0ffd0000, /* 0ffd0000 //C 0f6fd  2225 pop	pc */
0x0000f6fe, 0x00000000, /* 00000000 //C 0f6fe  2228 printf_left_align:	.ds	1 */
0x0000f6ff, 0x00000000, /* 00000000 //C 0f6ff  2229 printf_show_sign:	.ds	1 */
0x0000f700, 0x00000000, /* 00000000 //C 0f700  2230 printf_fill_zero:	.ds	1 */
0x0000f701, 0x00000000, /* 00000000 //C 0f701  2231 printf_min_len:		.ds	1 */
0x0000f702, 0x020b0000, /* 020b0000 //C 0f702  2238 sz	r0		; check str pointer */
0x0000f703, 0x10f00e00, /* 10f00e00 //C 0f703  2239 Z ret */
0x0000f704, 0x0ded0000, /* 0ded0000 //C 0f704  2240 push	lr */
0x0000f705, 0x0d0d0000, /* 0d0d0000 //C 0f705  2241 push	r0 */
0x0000f706, 0x0d1d0000, /* 0d1d0000 //C 0f706  2242 push	r1 */
0x0000f707, 0x0d3d0000, /* 0d3d0000 //C 0f707  2243 push	r3 */
0x0000f708, 0x0d4d0000, /* 0d4d0000 //C 0f708  2244 push	r4 */
0x0000f709, 0x0d5d0000, /* 0d5d0000 //C 0f709  2245 push	r5 */
0x0000f70a, 0x0610f86f, /* 0610f86f //C 0f70a  2247 st	r1,reg1 */
0x0000f70b, 0x0620f870, /* 0620f870 //C 0f70b  2248 st	r2,reg2 */
0x0000f70c, 0x0630f871, /* 0630f871 //C 0f70c  2249 st	r3,reg3 */
0x0000f70d, 0x0640f872, /* 0640f872 //C 0f70d  2250 st	r4,reg4 */
0x0000f70e, 0x0650f873, /* 0650f873 //C 0f70e  2251 st	r5,reg5 */
0x0000f70f, 0x0660f874, /* 0660f874 //C 0f70f  2252 st	r6,reg6 */
0x0000f710, 0x0670f875, /* 0670f875 //C 0f710  2253 st	r7,reg7 */
0x0000f711, 0x0680f876, /* 0680f876 //C 0f711  2254 st	r8,reg8 */
0x0000f712, 0x0690f877, /* 0690f877 //C 0f712  2255 st	r9,reg9 */
0x0000f713, 0x06a0f878, /* 06a0f878 //C 0f713  2256 st	r10,reg10 */
0x0000f714, 0x06b0f879, /* 06b0f879 //C 0f714  2257 st	r11,reg11 */
0x0000f715, 0x06c0f87a, /* 06c0f87a //C 0f715  2258 st	r12,reg12 */
0x0000f716, 0x00200000, /* 00200000 //C 0f716  2260 mov	r2,r0		; pointer to format string */
0x0000f717, 0x0112f86f, /* 0112f86f //C 0f717  2261 mvzl	r1,reg1		; pointer to params */
0x0000f718, 0x01320000, /* 01320000 //C 0f718  2262 mvzl	r3,0		; byte idx in packed str */
0x0000f719, 0x01420000, /* 01420000 //C 0f719  2263 mvzl	r4,0		; bool: len reading mode */
0x0000f71a, 0x0e020000, /* 0e020000 //C 0f71a  2266 ld	r0,r2		; get next char */
0x0000f71b, 0x020b0000, /* 020b0000 //C 0f71b  2267 sz	r0		; is it EOS? */
0x0000f71c, 0x11f2f7b2, /* 11f2f7b2 //C 0f71c  2268 jz	printf_ret */
0x0000f71d, 0x06014003, /* 06014003 //C 0f71d  2269 getbz	r0,r0,r3	; pick next byte */
0x0000f71e, 0x020b0000, /* 020b0000 //C 0f71e  2270 sz	r0		; is it null? */
0x0000f71f, 0x11f2f7af, /* 11f2f7af //C 0f71f  2271 jz	printf_nextword	; no more non-nulls */
0x0000f720, 0x01080025, /* 01080025 //C 0f720  2273 cmp	r0,'%'		; is it a format char? */
0x0000f721, 0x21f2f7ab, /* 21f2f7ab //C 0f721  2274 jnz	printf_print */
0x0000f722, 0x01520000, /* 01520000 //C 0f722  2277 mvzl	r5,0 */
0x0000f723, 0x0650f6fe, /* 0650f6fe //C 0f723  2278 st	r5,printf_left_align */
0x0000f724, 0x0650f6ff, /* 0650f6ff //C 0f724  2279 st	r5,printf_show_sign */
0x0000f725, 0x0650f700, /* 0650f700 //C 0f725  2280 st	r5,printf_fill_zero */
0x0000f726, 0x0650f701, /* 0650f701 //C 0f726  2281 st	r5,printf_min_len */
0x0000f727, 0x01340001, /* 01340001 //C 0f727  2283 inc	r3 */
0x0000f728, 0x01380004, /* 01380004 //C 0f728  2284 cmp	r3,4 */
0x0000f729, 0x21f2f72c, /* 21f2f72c //C 0f729  2285 jnz	printf_l3 */
0x0000f72a, 0x01320000, /* 01320000 //C 0f72a  2287 mvzl	r3,0 */
0x0000f72b, 0x01240001, /* 01240001 //C 0f72b  2288 inc	r2		; go to format char */
0x0000f72c, 0x0e020000, /* 0e020000 //C 0f72c  2290 ld	r0,r2 */
0x0000f72d, 0x020b0000, /* 020b0000 //C 0f72d  2291 sz	r0		; is it EOS? */
0x0000f72e, 0x11f2f7b2, /* 11f2f7b2 //C 0f72e  2292 jz	printf_ret */
0x0000f72f, 0x06014003, /* 06014003 //C 0f72f  2293 getbz	r0,r0,r3 */
0x0000f730, 0x020b0000, /* 020b0000 //C 0f730  2294 sz	r0 */
0x0000f731, 0x11f2f72a, /* 11f2f72a //C 0f731  2295 jz	printf_l4 */
0x0000f732, 0x024b0000, /* 024b0000 //C 0f732  2298 sz	r4		; len reading? */
0x0000f733, 0x11f2f73f, /* 11f2f73f //C 0f733  2299 jz	printf_nolen_read */
0x0000f734, 0x01080030, /* 01080030 //C 0f734  2301 cmp	r0,'0'		; when non-digit found: */
0x0000f735, 0x41f2f73e, /* 41f2f73e //C 0f735  2302 ULT jmp	printf_len_read_exit */
0x0000f736, 0x01080039, /* 01080039 //C 0f736  2303 cmp	r0,'9' */
0x0000f737, 0x91f2f73e, /* 91f2f73e //C 0f737  2304 UGT jmp	printf_len_read_exit */
0x0000f738, 0x01060030, /* 01060030 //C 0f738  2306 sub	r0,'0'		; convert to binary */
0x0000f739, 0x0750f701, /* 0750f701 //C 0f739  2307 ld	r5,printf_min_len ; and mix to parameter */
0x0000f73a, 0x0159000a, /* 0159000a //C 0f73a  2308 mul	r5,10 */
0x0000f73b, 0x00540000, /* 00540000 //C 0f73b  2309 add	r5,r0 */
0x0000f73c, 0x0650f701, /* 0650f701 //C 0f73c  2310 st	r5,printf_min_len */
0x0000f73d, 0x01f2f727, /* 01f2f727 //C 0f73d  2311 jmp	printf_fmt_next */
0x0000f73e, 0x01420000, /* 01420000 //C 0f73e  2313 mvzl	r4,0 */
0x0000f73f, 0x0108002b, /* 0108002b //C 0f73f  2316 cmp	r0,'+' */
0x0000f740, 0x21f2f744, /* 21f2f744 //C 0f740  2317 jnz	printf_n1 */
0x0000f741, 0x01020001, /* 01020001 //C 0f741  2318 mvzl	r0,1 */
0x0000f742, 0x0600f6ff, /* 0600f6ff //C 0f742  2319 st	r0,printf_show_sign */
0x0000f743, 0x01f2f727, /* 01f2f727 //C 0f743  2320 jmp	printf_fmt_next */
0x0000f744, 0x0108002d, /* 0108002d //C 0f744  2322 cmp	r0,'-' */
0x0000f745, 0x21f2f749, /* 21f2f749 //C 0f745  2323 jnz	printf_n2 */
0x0000f746, 0x01020001, /* 01020001 //C 0f746  2324 mvzl	r0,1 */
0x0000f747, 0x0600f6fe, /* 0600f6fe //C 0f747  2325 st	r0,printf_left_align */
0x0000f748, 0x01f2f727, /* 01f2f727 //C 0f748  2326 jmp	printf_fmt_next */
0x0000f749, 0x01080030, /* 01080030 //C 0f749  2328 cmp	r0,'0' */
0x0000f74a, 0x21f2f74e, /* 21f2f74e //C 0f74a  2329 jnz	printf_n3 */
0x0000f74b, 0x01020001, /* 01020001 //C 0f74b  2330 mvzl	r0,1 */
0x0000f74c, 0x0600f700, /* 0600f700 //C 0f74c  2331 st	r0,printf_fill_zero */
0x0000f74d, 0x01f2f727, /* 01f2f727 //C 0f74d  2332 jmp	printf_fmt_next */
0x0000f74e, 0x01080031, /* 01080031 //C 0f74e  2334 cmp	r0,'1' */
0x0000f74f, 0x41f2f754, /* 41f2f754 //C 0f74f  2335 ULT jmp	printf_n4 */
0x0000f750, 0x01080039, /* 01080039 //C 0f750  2336 cmp	r0,'9' */
0x0000f751, 0x91f2f754, /* 91f2f754 //C 0f751  2337 UGT jmp	printf_n4 */
0x0000f752, 0x01420001, /* 01420001 //C 0f752  2338 mvzl	r4,1 */
0x0000f753, 0x01f2f738, /* 01f2f738 //C 0f753  2339 jmp	printf_add2len */
0x0000f754, 0x01080025, /* 01080025 //C 0f754  2341 cmp	r0,'%'		; % is used to print % */
0x0000f755, 0x11f2f7ab, /* 11f2f7ab //C 0f755  2342 jz	printf_print */
0x0000f756, 0x01080075, /* 01080075 //C 0f756  2344 cmp	r0,'u'		; u print unsigned in decimal */
0x0000f757, 0x21f2f75e, /* 21f2f75e //C 0f757  2345 jnz	printf_notu */
0x0000f758, 0x0e010000, /* 0e010000 //C 0f758  2349 ld	r0,r1 */
0x0000f759, 0x01140001, /* 01140001 //C 0f759  2350 inc	r1 */
0x0000f75a, 0x0400f58f, /* 0400f58f //C 0f75a  2351 call	utoa */
0x0000f75b, 0x0102f5c7, /* 0102f5c7 //C 0f75b  2352 mvzl	r0,itoa_buffer */
0x0000f75c, 0x0400f7c1, /* 0400f7c1 //C 0f75c  2353 call	printf_pr */
0x0000f75d, 0x01f2f7ac, /* 01f2f7ac //C 0f75d  2354 jmp	printf_next */
0x0000f75e, 0x01080064, /* 01080064 //C 0f75e  2357 cmp	r0,'d'		;d print signed in decimal */
0x0000f75f, 0x21f2f770, /* 21f2f770 //C 0f75f  2358 jnz	printf_notd */
0x0000f760, 0x0e010000, /* 0e010000 //C 0f760  2362 ld	r0,r1 */
0x0000f761, 0x01140001, /* 01140001 //C 0f761  2363 inc	r1 */
0x0000f762, 0x0750f6ff, /* 0750f6ff //C 0f762  2364 ld	r5,printf_show_sign */
0x0000f763, 0x025b0000, /* 025b0000 //C 0f763  2365 sz	r5 */
0x0000f764, 0x21f2f76a, /* 21f2f76a //C 0f764  2366 jnz	printf_d_sign */
0x0000f765, 0x020b0000, /* 020b0000 //C 0f765  2368 sz	r0 */
0x0000f766, 0x51520000, /* 51520000 //C 0f766  2369 S1 mvzl	r5,0 */
0x0000f767, 0x5650f700, /* 5650f700 //C 0f767  2370 S1 st	r5,printf_fill_zero */
0x0000f768, 0x0400f571, /* 0400f571 //C 0f768  2371 call	itoa */
0x0000f769, 0x01f2f76d, /* 01f2f76d //C 0f769  2372 jmp	printf_d_pr */
0x0000f76a, 0x01520000, /* 01520000 //C 0f76a  2374 mvzl	r5,0 */
0x0000f76b, 0x0650f700, /* 0650f700 //C 0f76b  2375 st	r5,printf_fill_zero */
0x0000f76c, 0x0400f582, /* 0400f582 //C 0f76c  2376 call	itosa */
0x0000f76d, 0x0102f5c7, /* 0102f5c7 //C 0f76d  2378 mvzl	r0,itoa_buffer */
0x0000f76e, 0x0400f7c1, /* 0400f7c1 //C 0f76e  2379 call	printf_pr */
0x0000f76f, 0x01f2f7ac, /* 01f2f7ac //C 0f76f  2380 jmp	printf_next */
0x0000f770, 0x01080078, /* 01080078 //C 0f770  2383 cmp	r0,'x' */
0x0000f771, 0x21f2f784, /* 21f2f784 //C 0f771  2384 jnz	printf_notx */
0x0000f772, 0x0e010000, /* 0e010000 //C 0f772  2388 ld	r0,r1 */
0x0000f773, 0x01140001, /* 01140001 //C 0f773  2389 inc	r1 */
0x0000f774, 0x01520000, /* 01520000 //C 0f774  2390 mvzl	r5,0 */
0x0000f775, 0x0650f6ff, /* 0650f6ff //C 0f775  2391 st	r5,printf_show_sign */
0x0000f776, 0x0750f701, /* 0750f701 //C 0f776  2392 ld	r5,printf_min_len */
0x0000f777, 0x025b0000, /* 025b0000 //C 0f777  2393 sz	r5 */
0x0000f778, 0x21f2f77c, /* 21f2f77c //C 0f778  2394 jnz	printf_x_len */
0x0000f779, 0x0400f5f3, /* 0400f5f3 //C 0f779  2396 call	utoh */
0x0000f77a, 0x0102f5c7, /* 0102f5c7 //C 0f77a  2397 mvzl	r0,itoa_buffer */
0x0000f77b, 0x01f2f782, /* 01f2f782 //C 0f77b  2398 jmp	printf_x_do */
0x0000f77c, 0x01580008, /* 01580008 //C 0f77c  2400 cmp	r5,8 */
0x0000f77d, 0x91f2f779, /* 91f2f779 //C 0f77d  2401 UGT jmp	printf_x_def */
0x0000f77e, 0x0400f61e, /* 0400f61e //C 0f77e  2403 call	utoh32 */
0x0000f77f, 0x0102f5c7, /* 0102f5c7 //C 0f77f  2404 mvzl	r0,itoa_buffer */
0x0000f780, 0x01040008, /* 01040008 //C 0f780  2405 add	r0,8 */
0x0000f781, 0x00060500, /* 00060500 //C 0f781  2406 sub	r0,r5 */
0x0000f782, 0x0400f7c1, /* 0400f7c1 //C 0f782  2408 call	printf_pr */
0x0000f783, 0x01f2f7ac, /* 01f2f7ac //C 0f783  2409 jmp	printf_next */
0x0000f784, 0x01080073, /* 01080073 //C 0f784  2412 cmp	r0,'s' */
0x0000f785, 0x21f2f78c, /* 21f2f78c //C 0f785  2413 jnz	printf_nots */
0x0000f786, 0x0e010000, /* 0e010000 //C 0f786  2417 ld	r0,r1 */
0x0000f787, 0x01140001, /* 01140001 //C 0f787  2418 inc	r1 */
0x0000f788, 0x01520000, /* 01520000 //C 0f788  2419 mvzl	r5,0 */
0x0000f789, 0x0650f700, /* 0650f700 //C 0f789  2420 st	r5,printf_fill_zero */
0x0000f78a, 0x0400f7c1, /* 0400f7c1 //C 0f78a  2421 call	printf_pr */
0x0000f78b, 0x01f2f7ac, /* 01f2f7ac //C 0f78b  2422 jmp	printf_next */
0x0000f78c, 0x01080063, /* 01080063 //C 0f78c  2425 cmp	r0,'c' */
0x0000f78d, 0x21f2f792, /* 21f2f792 //C 0f78d  2426 jnz	printf_notc */
0x0000f78e, 0x0e010000, /* 0e010000 //C 0f78e  2430 ld	r0,r1 */
0x0000f78f, 0x01140001, /* 01140001 //C 0f78f  2431 inc	r1 */
0x0000f790, 0x0400f685, /* 0400f685 //C 0f790  2432 call	putchar */
0x0000f791, 0x01f2f7ac, /* 01f2f7ac //C 0f791  2433 jmp	printf_next */
0x0000f792, 0x01080062, /* 01080062 //C 0f792  2436 cmp	r0,'b' */
0x0000f793, 0x21f2f7a2, /* 21f2f7a2 //C 0f793  2437 jnz	printf_notb */
0x0000f794, 0x0e010000, /* 0e010000 //C 0f794  2441 ld	r0,r1 */
0x0000f795, 0x01140001, /* 01140001 //C 0f795  2442 inc	r1 */
0x0000f796, 0x01520000, /* 01520000 //C 0f796  2443 mvzl	r5,0 */
0x0000f797, 0x0650f6ff, /* 0650f6ff //C 0f797  2444 st	r5,printf_show_sign */
0x0000f798, 0x0400f663, /* 0400f663 //C 0f798  2445 call	btoa */
0x0000f799, 0x0102f5c7, /* 0102f5c7 //C 0f799  2446 mvzl	r0,itoa_buffer */
0x0000f79a, 0x0750f701, /* 0750f701 //C 0f79a  2447 ld	r5,printf_min_len */
0x0000f79b, 0x025b0000, /* 025b0000 //C 0f79b  2448 sz	r5 */
0x0000f79c, 0x11f2f7a0, /* 11f2f7a0 //C 0f79c  2449 jz	printf_b_do */
0x0000f79d, 0x01580020, /* 01580020 //C 0f79d  2451 cmp	r5,32 */
0x0000f79e, 0xa10a0020, /* a10a0020 //C 0f79e  2454 ULE plus r0,32 */
0x0000f79f, 0xa0060500, /* a0060500 //C 0f79f  2455 ULE sub	r0,r5 */
0x0000f7a0, 0x0400f7c1, /* 0400f7c1 //C 0f7a0  2457 call	printf_pr */
0x0000f7a1, 0x01f2f7ac, /* 01f2f7ac //C 0f7a1  2458 jmp	printf_next */
0x0000f7a2, 0x01080070, /* 01080070 //C 0f7a2  2461 cmp	r0,'p' */
0x0000f7a3, 0x21f2f7aa, /* 21f2f7aa //C 0f7a3  2462 jnz	printf_notp */
0x0000f7a4, 0x0e010000, /* 0e010000 //C 0f7a4  2466 ld	r0,r1 */
0x0000f7a5, 0x01140001, /* 01140001 //C 0f7a5  2467 inc	r1 */
0x0000f7a6, 0x0400f63f, /* 0400f63f //C 0f7a6  2468 call	ptoa */
0x0000f7a7, 0x0102f5c7, /* 0102f5c7 //C 0f7a7  2469 mvzl	r0,itoa_buffer */
0x0000f7a8, 0x0400f7c1, /* 0400f7c1 //C 0f7a8  2470 call	printf_pr */
0x0000f7a9, 0x01f2f7ac, /* 01f2f7ac //C 0f7a9  2471 jmp	printf_next */
0x0000f7aa, 0x01f2f7ac, /* 01f2f7ac //C 0f7aa  2474 jmp	printf_next */
0x0000f7ab, 0x0400f685, /* 0400f685 //C 0f7ab  2477 call	putchar		; print actual char */
0x0000f7ac, 0x01340001, /* 01340001 //C 0f7ac  2479 inc	r3		; next byte in word */
0x0000f7ad, 0x01380004, /* 01380004 //C 0f7ad  2480 cmp	r3,4		; all 4 processed? */
0x0000f7ae, 0x21f2f71a, /* 21f2f71a //C 0f7ae  2481 jnz	printf_cyc */
0x0000f7af, 0x01240001, /* 01240001 //C 0f7af  2483 inc	r2		; inc string ptr */
0x0000f7b0, 0x01320000, /* 01320000 //C 0f7b0  2484 mvzl	r3,0		; restart byte idx */
0x0000f7b1, 0x01f2f71a, /* 01f2f71a //C 0f7b1  2485 jmp	printf_cyc */
0x0000f7b2, 0x07c0f87a, /* 07c0f87a //C 0f7b2  2488 ld	r12,reg12 */
0x0000f7b3, 0x07b0f879, /* 07b0f879 //C 0f7b3  2489 ld	r11,reg11 */
0x0000f7b4, 0x07a0f878, /* 07a0f878 //C 0f7b4  2490 ld	r10,reg10 */
0x0000f7b5, 0x0790f877, /* 0790f877 //C 0f7b5  2491 ld	r9,reg9 */
0x0000f7b6, 0x0780f876, /* 0780f876 //C 0f7b6  2492 ld	r8,reg8 */
0x0000f7b7, 0x0770f875, /* 0770f875 //C 0f7b7  2493 ld	r7,reg7 */
0x0000f7b8, 0x0760f874, /* 0760f874 //C 0f7b8  2494 ld	r6,reg6 */
0x0000f7b9, 0x0750f873, /* 0750f873 //C 0f7b9  2495 ld	r5,reg5 */
0x0000f7ba, 0x0740f872, /* 0740f872 //C 0f7ba  2496 ld	r4,reg4 */
0x0000f7bb, 0x0f5d0000, /* 0f5d0000 //C 0f7bb  2498 pop	r5 */
0x0000f7bc, 0x0f4d0000, /* 0f4d0000 //C 0f7bc  2499 pop	r4 */
0x0000f7bd, 0x0f3d0000, /* 0f3d0000 //C 0f7bd  2500 pop	r3 */
0x0000f7be, 0x0f1d0000, /* 0f1d0000 //C 0f7be  2501 pop	r1 */
0x0000f7bf, 0x0f0d0000, /* 0f0d0000 //C 0f7bf  2502 pop	r0 */
0x0000f7c0, 0x0ffd0000, /* 0ffd0000 //C 0f7c0  2503 pop	pc */
0x0000f7c1, 0x020b0000, /* 020b0000 //C 0f7c1  2507 sz	r0 */
0x0000f7c2, 0x10f00e00, /* 10f00e00 //C 0f7c2  2508 Z ret */
0x0000f7c3, 0x0ded0000, /* 0ded0000 //C 0f7c3  2509 push	lr */
0x0000f7c4, 0x0d0d0000, /* 0d0d0000 //C 0f7c4  2510 push	r0 */
0x0000f7c5, 0x0d1d0000, /* 0d1d0000 //C 0f7c5  2511 push	r1 */
0x0000f7c6, 0x0d2d0000, /* 0d2d0000 //C 0f7c6  2512 push	r2 */
0x0000f7c7, 0x0d4d0000, /* 0d4d0000 //C 0f7c7  2513 push	r4 */
0x0000f7c8, 0x0710f701, /* 0710f701 //C 0f7c8  2514 ld	r1,printf_min_len */
0x0000f7c9, 0x021b0000, /* 021b0000 //C 0f7c9  2515 sz	r1 */
0x0000f7ca, 0x21f2f7cd, /* 21f2f7cd //C 0f7ca  2516 jnz	pp_use_len */
0x0000f7cb, 0x0400f68c, /* 0400f68c //C 0f7cb  2518 call	prints */
0x0000f7cc, 0x01f2f7db, /* 01f2f7db //C 0f7cc  2519 jmp	pp_ret */
0x0000f7cd, 0x0400f4f7, /* 0400f4f7 //C 0f7cd  2521 call	strlen		; R4= str length */
0x0000f7ce, 0x00160400, /* 00160400 //C 0f7ce  2522 sub	r1,r4		; R1= fill length */
0x0000f7cf, 0x0720f6fe, /* 0720f6fe //C 0f7cf  2523 ld	r2,printf_left_align */
0x0000f7d0, 0x022b0000, /* 022b0000 //C 0f7d0  2524 sz	r2 */
0x0000f7d1, 0x21f2f7d8, /* 21f2f7d8 //C 0f7d1  2525 jnz	pp_left */
0x0000f7d2, 0x0d0d0000, /* 0d0d0000 //C 0f7d2  2527 push	r0 */
0x0000f7d3, 0x00000100, /* 00000100 //C 0f7d3  2528 mov	r0,r1 */
0x0000f7d4, 0x0400f7e0, /* 0400f7e0 //C 0f7d4  2529 call	printf_fill */
0x0000f7d5, 0x0f0d0000, /* 0f0d0000 //C 0f7d5  2530 pop	r0 */
0x0000f7d6, 0x0400f68c, /* 0400f68c //C 0f7d6  2531 call	prints */
0x0000f7d7, 0x01f2f7db, /* 01f2f7db //C 0f7d7  2532 jmp	pp_ret */
0x0000f7d8, 0x0400f68c, /* 0400f68c //C 0f7d8  2534 call	prints */
0x0000f7d9, 0x00000100, /* 00000100 //C 0f7d9  2535 mov	r0,r1 */
0x0000f7da, 0x0400f7e0, /* 0400f7e0 //C 0f7da  2536 call	printf_fill */
0x0000f7db, 0x0f4d0000, /* 0f4d0000 //C 0f7db  2538 pop	r4 */
0x0000f7dc, 0x0f2d0000, /* 0f2d0000 //C 0f7dc  2539 pop	r2 */
0x0000f7dd, 0x0f1d0000, /* 0f1d0000 //C 0f7dd  2540 pop	r1 */
0x0000f7de, 0x0f0d0000, /* 0f0d0000 //C 0f7de  2541 pop	r0 */
0x0000f7df, 0x0ffd0000, /* 0ffd0000 //C 0f7df  2542 pop	pc */
0x0000f7e0, 0x020b0000, /* 020b0000 //C 0f7e0  2546 sz	r0 */
0x0000f7e1, 0x50f00e00, /* 50f00e00 //C 0f7e1  2547 S1 ret */
0x0000f7e2, 0x10f00e00, /* 10f00e00 //C 0f7e2  2548 Z ret */
0x0000f7e3, 0x0ded0000, /* 0ded0000 //C 0f7e3  2549 push	lr */
0x0000f7e4, 0x0d0d0000, /* 0d0d0000 //C 0f7e4  2550 push	r0 */
0x0000f7e5, 0x0d1d0000, /* 0d1d0000 //C 0f7e5  2551 push	r1 */
0x0000f7e6, 0x00100000, /* 00100000 //C 0f7e6  2552 mov	r1,r0 */
0x0000f7e7, 0x0700f6fe, /* 0700f6fe //C 0f7e7  2553 ld	r0,printf_left_align */
0x0000f7e8, 0x020b0000, /* 020b0000 //C 0f7e8  2554 sz	r0 */
0x0000f7e9, 0x21020020, /* 21020020 //C 0f7e9  2555 NZ mvzl	r0,32 */
0x0000f7ea, 0x21f2f7ef, /* 21f2f7ef //C 0f7ea  2556 jnz	pf_cyc */
0x0000f7eb, 0x0700f700, /* 0700f700 //C 0f7eb  2557 ld	r0,printf_fill_zero */
0x0000f7ec, 0x020b0000, /* 020b0000 //C 0f7ec  2558 sz	r0 */
0x0000f7ed, 0x21020030, /* 21020030 //C 0f7ed  2559 NZ mvzl r0,'0' */
0x0000f7ee, 0x11020020, /* 11020020 //C 0f7ee  2560 Z mvzl	r0,32 */
0x0000f7ef, 0x0400f685, /* 0400f685 //C 0f7ef  2561 pf_cyc:	call	putchar */
0x0000f7f0, 0x0114ffff, /* 0114ffff //C 0f7f0  2562 dec	r1 */
0x0000f7f1, 0x21f2f7ef, /* 21f2f7ef //C 0f7f1  2563 jnz	pf_cyc */
0x0000f7f2, 0x0f1d0000, /* 0f1d0000 //C 0f7f2  2564 pop	r1 */
0x0000f7f3, 0x0f0d0000, /* 0f0d0000 //C 0f7f3  2565 pop	r0 */
0x0000f7f4, 0x0ffd0000, /* 0ffd0000 //C 0f7f4  2566 pop	pc */
0x0000f7f5, 0x0d0d0000, /* 0d0d0000 //C 0f7f5  2569 push	r0 */
0x0000f7f6, 0x0d2d0000, /* 0d2d0000 //C 0f7f6  2570 push	r2 */
0x0000f7f7, 0x00000e00, /* 00000e00 //C 0f7f7  2571 mov	r0,LR */
0x0000f7f8, 0x0400f702, /* 0400f702 //C 0f7f8  2572 call	printf */
0x0000f7f9, 0x01240001, /* 01240001 //C 0f7f9  2573 inc	r2 */
0x0000f7fa, 0x00e00200, /* 00e00200 //C 0f7fa  2574 mov	LR,r2 */
0x0000f7fb, 0x0f2d0000, /* 0f2d0000 //C 0f7fb  2575 pop	r2 */
0x0000f7fc, 0x0f0d0000, /* 0f0d0000 //C 0f7fc  2576 pop	r0 */
0x0000f7fd, 0x00f00e00, /* 00f00e00 //C 0f7fd  2577 ret */
0x0000f7fe, 0x06020100, /* 06020100 //C 0f7fe  2585 rds	r0,sver */
0x0000f7ff, 0x00f00e00, /* 00f00e00 //C 0f7ff  2587 ret */
0x0000f800, 0x00000000, /* 00000000 //C 0f800  2592 line:		ds	100		; line buffer */
0x0000f864, 0x00000000, /* 00000000 //C 0f864  2593 line_ptr:	ds	1		; line pointer (index) */
0x0000f865, 0x00000000, /* 00000000 //C 0f865  2594 at_eol:		ds	1		; bool, true if EOL arrived */
0x0000f866, 0x00000000, /* 00000000 //C 0f866  2595 words:		ds	5		; Tokens of line */
0x0000f86b, 0x00000000, /* 00000000 //C 0f86b  2596 echo:		ds	1		; bool, do echo or not */
0x0000f86c, 0x00000000, /* 00000000 //C 0f86c  2597 dd	0 */
0x0000f86d, 0x00000000, /* 00000000 //C 0f86d  2598 dd	0 */
0x0000f86e, 0x00000000, /* 00000000 //C 0f86e  2600 dd	0 */
0x0000f86f, 0x00000000, /* 00000000 //C 0f86f  2601 dd	0 */
0x0000f870, 0x00000000, /* 00000000 //C 0f870  2602 dd	0 */
0x0000f871, 0x00000000, /* 00000000 //C 0f871  2603 dd	0 */
0x0000f872, 0x00000000, /* 00000000 //C 0f872  2604 dd	0 */
0x0000f873, 0x00000000, /* 00000000 //C 0f873  2605 dd	0 */
0x0000f874, 0x00000000, /* 00000000 //C 0f874  2606 dd	0 */
0x0000f875, 0x00000000, /* 00000000 //C 0f875  2607 dd	0 */
0x0000f876, 0x00000000, /* 00000000 //C 0f876  2608 dd	0 */
0x0000f877, 0x00000000, /* 00000000 //C 0f877  2609 dd	0 */
0x0000f878, 0x00000000, /* 00000000 //C 0f878  2610 dd	0 */
0x0000f879, 0x00000000, /* 00000000 //C 0f879  2611 dd	0 */
0x0000f87a, 0x00000000, /* 00000000 //C 0f87a  2612 dd	0 */
0x0000f87b, 0x00000000, /* 00000000 //C 0f87b  2613 dd	0 */
0x0000f87c, 0x00000000, /* 00000000 //C 0f87c  2614 dd	0 */
0x0000f87d, 0x00000000, /* 00000000 //C 0f87d  2615 dd	0 */
0x0000f87e, 0x00000000, /* 00000000 //C 0f87e  2616 dd	0 */
0x0000f87f, 0x00000050, /* 00000050 //C 0f87f  2618 db	80 */
0x0000f880, 0x0000004d, /* 0000004d //C 0f880  2618 db	77 */
0x0000f881, 0x0000006f, /* 0000006f //C 0f881  2618 db	111 */
0x0000f882, 0x0000006e, /* 0000006e //C 0f882  2618 db	110 */
0x0000f883, 0x00000069, /* 00000069 //C 0f883  2618 db	105 */
0x0000f884, 0x00000074, /* 00000074 //C 0f884  2618 db	116 */
0x0000f885, 0x0000006f, /* 0000006f //C 0f885  2618 db	111 */
0x0000f886, 0x00000072, /* 00000072 //C 0f886  2618 db	114 */
0x0000f887, 0x0000002c, /* 0000002c //C 0f887  2618 db	44 */
0x0000f888, 0x00000020, /* 00000020 //C 0f888  2618 db	32 */
0x0000f889, 0x00000063, /* 00000063 //C 0f889  2618 db	99 */
0x0000f88a, 0x00000070, /* 00000070 //C 0f88a  2618 db	112 */
0x0000f88b, 0x00000075, /* 00000075 //C 0f88b  2618 db	117 */
0x0000f88c, 0x00000020, /* 00000020 //C 0f88c  2618 db	32 */
0x0000f88d, 0x00000076, /* 00000076 //C 0f88d  2618 db	118 */
0x0000f88e, 0x00000025, /* 00000025 //C 0f88e  2618 db	37 */
0x0000f88f, 0x00000064, /* 00000064 //C 0f88f  2618 db	100 */
0x0000f890, 0x0000002e, /* 0000002e //C 0f890  2618 db	46 */
0x0000f891, 0x00000025, /* 00000025 //C 0f891  2618 db	37 */
0x0000f892, 0x00000064, /* 00000064 //C 0f892  2618 db	100 */
0x0000f893, 0x0000002e, /* 0000002e //C 0f893  2618 db	46 */
0x0000f894, 0x00000025, /* 00000025 //C 0f894  2618 db	37 */
0x0000f895, 0x00000064, /* 00000064 //C 0f895  2618 db	100 */
0x0000f896, 0x0000000a, /* 0000000a //C 0f896  2618 db	10 */
0x0000f897, 0x00000000, /* 00000000 //C 0f897  2618 db */
0x0000f898, 0x00000053, /* 00000053 //C 0f898  2619 db	83 */
0x0000f899, 0x00000074, /* 00000074 //C 0f899  2619 db	116 */
0x0000f89a, 0x0000006f, /* 0000006f //C 0f89a  2619 db	111 */
0x0000f89b, 0x00000070, /* 00000070 //C 0f89b  2619 db	112 */
0x0000f89c, 0x00000020, /* 00000020 //C 0f89c  2619 db	32 */
0x0000f89d, 0x00000061, /* 00000061 //C 0f89d  2619 db	97 */
0x0000f89e, 0x00000074, /* 00000074 //C 0f89e  2619 db	116 */
0x0000f89f, 0x0000003a, /* 0000003a //C 0f89f  2619 db	58 */
0x0000f8a0, 0x00000020, /* 00000020 //C 0f8a0  2619 db	32 */
0x0000f8a1, 0x00000000, /* 00000000 //C 0f8a1  2619 db */
0x0000f8a2, 0x00000008, /* 00000008 //C 0f8a2  2620 db	8 */
0x0000f8a3, 0x00000020, /* 00000020 //C 0f8a3  2620 db	32 */
0x0000f8a4, 0x00000008, /* 00000008 //C 0f8a4  2620 db	8 */
0x0000f8a5, 0x00000000, /* 00000000 //C 0f8a5  2620 db	0 */
0x0000f8a6, 0x0000003a, /* 0000003a //C 0f8a6  2621 db	58 */
0x0000f8a7, 0x00000000, /* 00000000 //C 0f8a7  2621 db */
0x0000f8a8, 0x00000020, /* 00000020 //C 0f8a8  2622 db	32 */
0x0000f8a9, 0x0000003b, /* 0000003b //C 0f8a9  2622 db	59 */
0x0000f8aa, 0x00000009, /* 00000009 //C 0f8aa  2622 db	9 */
0x0000f8ab, 0x0000000b, /* 0000000b //C 0f8ab  2622 db	11 */
0x0000f8ac, 0x0000002c, /* 0000002c //C 0f8ac  2622 db	44 */
0x0000f8ad, 0x0000003d, /* 0000003d //C 0f8ad  2622 db	61 */
0x0000f8ae, 0x0000005b, /* 0000005b //C 0f8ae  2622 db	91 */
0x0000f8af, 0x0000005d, /* 0000005d //C 0f8af  2622 db	93 */
0x0000f8b0, 0x00000000, /* 00000000 //C 0f8b0  2622 db */
0x0000f8b1, 0x00000028, /* 00000028 //C 0f8b1  2623 db	40 */
0x0000f8b2, 0x0000006e, /* 0000006e //C 0f8b2  2623 db	110 */
0x0000f8b3, 0x00000075, /* 00000075 //C 0f8b3  2623 db	117 */
0x0000f8b4, 0x0000006c, /* 0000006c //C 0f8b4  2623 db	108 */
0x0000f8b5, 0x0000006c, /* 0000006c //C 0f8b5  2623 db	108 */
0x0000f8b6, 0x00000029, /* 00000029 //C 0f8b6  2623 db	41 */
0x0000f8b7, 0x00000000, /* 00000000 //C 0f8b7  2623 db */
0x0000f8b8, 0x00000000, /* 00000000 //C 0f8b8  2624 db	0 */
0x0000f8b9, 0x00000000, /* 00000000 //C 0f8b9  2625 db	0 */
0x0000f8ba, 0x00000000, /* 00000000 //C 0f8ba  2626 sc_buffer:	ds	10 */
0x0000f8c4, 0x0000f1af, /* 0000f1af //C 0f8c4  2630 dd	cmd_sm */
0x0000f8c5, 0x00000073, /* 00000073 //C 0f8c5  2631 db	115 */
0x0000f8c6, 0x0000006d, /* 0000006d //C 0f8c6  2631 db	109 */
0x0000f8c7, 0x00000000, /* 00000000 //C 0f8c7  2631 db */
0x0000f8c8, 0x0000f156, /* 0000f156 //C 0f8c8  2632 dd	cmd_m */
0x0000f8c9, 0x0000006d, /* 0000006d //C 0f8c9  2633 db	109 */
0x0000f8ca, 0x00000000, /* 00000000 //C 0f8ca  2633 db */
0x0000f8cb, 0x0000f156, /* 0000f156 //C 0f8cb  2634 dd	cmd_m */
0x0000f8cc, 0x0000006d, /* 0000006d //C 0f8cc  2635 db	109 */
0x0000f8cd, 0x00000065, /* 00000065 //C 0f8cd  2635 db	101 */
0x0000f8ce, 0x0000006d, /* 0000006d //C 0f8ce  2635 db	109 */
0x0000f8cf, 0x00000000, /* 00000000 //C 0f8cf  2635 db */
0x0000f8d0, 0x0000f1d2, /* 0000f1d2 //C 0f8d0  2636 dd	cmd_d */
0x0000f8d1, 0x00000064, /* 00000064 //C 0f8d1  2637 db	100 */
0x0000f8d2, 0x00000000, /* 00000000 //C 0f8d2  2637 db */
0x0000f8d3, 0x0000f1d2, /* 0000f1d2 //C 0f8d3  2638 dd	cmd_d */
0x0000f8d4, 0x00000064, /* 00000064 //C 0f8d4  2639 db	100 */
0x0000f8d5, 0x00000075, /* 00000075 //C 0f8d5  2639 db	117 */
0x0000f8d6, 0x0000006d, /* 0000006d //C 0f8d6  2639 db	109 */
0x0000f8d7, 0x00000070, /* 00000070 //C 0f8d7  2639 db	112 */
0x0000f8d8, 0x00000000, /* 00000000 //C 0f8d8  2639 db */
0x0000f8d9, 0x0000f205, /* 0000f205 //C 0f8d9  2640 dd	cmd_l */
0x0000f8da, 0x0000006c, /* 0000006c //C 0f8da  2641 db	108 */
0x0000f8db, 0x00000000, /* 00000000 //C 0f8db  2641 db */
0x0000f8dc, 0x0000f205, /* 0000f205 //C 0f8dc  2642 dd	cmd_l */
0x0000f8dd, 0x0000006c, /* 0000006c //C 0f8dd  2643 db	108 */
0x0000f8de, 0x0000006f, /* 0000006f //C 0f8de  2643 db	111 */
0x0000f8df, 0x00000061, /* 00000061 //C 0f8df  2643 db	97 */
0x0000f8e0, 0x00000064, /* 00000064 //C 0f8e0  2643 db	100 */
0x0000f8e1, 0x00000000, /* 00000000 //C 0f8e1  2643 db */
0x0000f8e2, 0x0000f27b, /* 0000f27b //C 0f8e2  2644 dd	cmd_g */
0x0000f8e3, 0x00000067, /* 00000067 //C 0f8e3  2645 db	103 */
0x0000f8e4, 0x00000000, /* 00000000 //C 0f8e4  2645 db */
0x0000f8e5, 0x0000f27b, /* 0000f27b //C 0f8e5  2646 dd	cmd_g */
0x0000f8e6, 0x00000067, /* 00000067 //C 0f8e6  2647 db	103 */
0x0000f8e7, 0x0000006f, /* 0000006f //C 0f8e7  2647 db	111 */
0x0000f8e8, 0x00000000, /* 00000000 //C 0f8e8  2647 db */
0x0000f8e9, 0x0000f27b, /* 0000f27b //C 0f8e9  2648 dd	cmd_g */
0x0000f8ea, 0x00000072, /* 00000072 //C 0f8ea  2649 db	114 */
0x0000f8eb, 0x00000075, /* 00000075 //C 0f8eb  2649 db	117 */
0x0000f8ec, 0x0000006e, /* 0000006e //C 0f8ec  2649 db	110 */
0x0000f8ed, 0x00000000, /* 00000000 //C 0f8ed  2649 db */
0x0000f8ee, 0x0000f2ba, /* 0000f2ba //C 0f8ee  2650 dd	cmd_h */
0x0000f8ef, 0x0000003f, /* 0000003f //C 0f8ef  2651 db	63 */
0x0000f8f0, 0x00000000, /* 00000000 //C 0f8f0  2651 db */
0x0000f8f1, 0x0000f2ba, /* 0000f2ba //C 0f8f1  2652 dd	cmd_h */
0x0000f8f2, 0x00000068, /* 00000068 //C 0f8f2  2653 db	104 */
0x0000f8f3, 0x00000000, /* 00000000 //C 0f8f3  2653 db */
0x0000f8f4, 0x0000f2ba, /* 0000f2ba //C 0f8f4  2654 dd	cmd_h */
0x0000f8f5, 0x00000068, /* 00000068 //C 0f8f5  2655 db	104 */
0x0000f8f6, 0x00000065, /* 00000065 //C 0f8f6  2655 db	101 */
0x0000f8f7, 0x0000006c, /* 0000006c //C 0f8f7  2655 db	108 */
0x0000f8f8, 0x00000070, /* 00000070 //C 0f8f8  2655 db	112 */
0x0000f8f9, 0x00000000, /* 00000000 //C 0f8f9  2655 db */
0x0000f8fa, 0x0000f321, /* 0000f321 //C 0f8fa  2656 dd	cmd_r */
0x0000f8fb, 0x00000072, /* 00000072 //C 0f8fb  2657 db	114 */
0x0000f8fc, 0x00000000, /* 00000000 //C 0f8fc  2657 db */
0x0000f8fd, 0x0000f321, /* 0000f321 //C 0f8fd  2658 dd	cmd_r */
0x0000f8fe, 0x00000072, /* 00000072 //C 0f8fe  2659 db	114 */
0x0000f8ff, 0x00000065, /* 00000065 //C 0f8ff  2659 db	101 */
0x0000f900, 0x00000067, /* 00000067 //C 0f900  2659 db	103 */
0x0000f901, 0x00000000, /* 00000000 //C 0f901  2659 db */
0x0000f902, 0x0000f321, /* 0000f321 //C 0f902  2660 dd	cmd_r */
0x0000f903, 0x00000072, /* 00000072 //C 0f903  2661 db	114 */
0x0000f904, 0x00000065, /* 00000065 //C 0f904  2661 db	101 */
0x0000f905, 0x00000067, /* 00000067 //C 0f905  2661 db	103 */
0x0000f906, 0x00000073, /* 00000073 //C 0f906  2661 db	115 */
0x0000f907, 0x00000000, /* 00000000 //C 0f907  2661 db */
0x0000f908, 0x0000f3b1, /* 0000f3b1 //C 0f908  2662 dd	cmd_sp */
0x0000f909, 0x00000073, /* 00000073 //C 0f909  2663 db	115 */
0x0000f90a, 0x00000070, /* 00000070 //C 0f90a  2663 db	112 */
0x0000f90b, 0x00000000, /* 00000000 //C 0f90b  2663 db */
0x0000f90c, 0x0000f3b4, /* 0000f3b4 //C 0f90c  2664 dd	cmd_lr */
0x0000f90d, 0x0000006c, /* 0000006c //C 0f90d  2665 db	108 */
0x0000f90e, 0x00000072, /* 00000072 //C 0f90e  2665 db	114 */
0x0000f90f, 0x00000000, /* 00000000 //C 0f90f  2665 db */
0x0000f910, 0x0000f3b7, /* 0000f3b7 //C 0f910  2666 dd	cmd_pc */
0x0000f911, 0x00000070, /* 00000070 //C 0f911  2667 db	112 */
0x0000f912, 0x00000063, /* 00000063 //C 0f912  2667 db	99 */
0x0000f913, 0x00000000, /* 00000000 //C 0f913  2667 db */
0x0000f914, 0x0000f3ba, /* 0000f3ba //C 0f914  2668 dd	cmd_f */
0x0000f915, 0x00000066, /* 00000066 //C 0f915  2669 db	102 */
0x0000f916, 0x00000000, /* 00000000 //C 0f916  2669 db */
0x0000f917, 0x0000f3bd, /* 0000f3bd //C 0f917  2670 dd	cmd_v */
0x0000f918, 0x00000076, /* 00000076 //C 0f918  2671 db	118 */
0x0000f919, 0x00000065, /* 00000065 //C 0f919  2671 db	101 */
0x0000f91a, 0x00000072, /* 00000072 //C 0f91a  2671 db	114 */
0x0000f91b, 0x00000000, /* 00000000 //C 0f91b  2671 db */
0x0000f91c, 0x0000f3bd, /* 0000f3bd //C 0f91c  2672 dd	cmd_v */
0x0000f91d, 0x00000076, /* 00000076 //C 0f91d  2673 db	118 */
0x0000f91e, 0x00000000, /* 00000000 //C 0f91e  2673 db */
0x0000f91f, 0x00000000, /* 00000000 //C 0f91f  2674 dd	0 */
0x0000f920, 0x00000000, /* 00000000 //C 0f920  2675 dd	0 */
0x0000f921, 0x0000006d, /* 0000006d //C 0f921  2677 db	109 */
0x0000f922, 0x0000005b, /* 0000005b //C 0f922  2677 db	91 */
0x0000f923, 0x00000065, /* 00000065 //C 0f923  2677 db	101 */
0x0000f924, 0x0000006d, /* 0000006d //C 0f924  2677 db	109 */
0x0000f925, 0x0000005d, /* 0000005d //C 0f925  2677 db	93 */
0x0000f926, 0x00000020, /* 00000020 //C 0f926  2677 db	32 */
0x0000f927, 0x00000061, /* 00000061 //C 0f927  2677 db	97 */
0x0000f928, 0x00000064, /* 00000064 //C 0f928  2677 db	100 */
0x0000f929, 0x00000064, /* 00000064 //C 0f929  2677 db	100 */
0x0000f92a, 0x00000072, /* 00000072 //C 0f92a  2677 db	114 */
0x0000f92b, 0x00000020, /* 00000020 //C 0f92b  2677 db	32 */
0x0000f92c, 0x0000005b, /* 0000005b //C 0f92c  2677 db	91 */
0x0000f92d, 0x00000076, /* 00000076 //C 0f92d  2677 db	118 */
0x0000f92e, 0x00000061, /* 00000061 //C 0f92e  2677 db	97 */
0x0000f92f, 0x0000006c, /* 0000006c //C 0f92f  2677 db	108 */
0x0000f930, 0x0000005d, /* 0000005d //C 0f930  2677 db	93 */
0x0000f931, 0x00000020, /* 00000020 //C 0f931  2677 db	32 */
0x0000f932, 0x00000020, /* 00000020 //C 0f932  2677 db	32 */
0x0000f933, 0x00000047, /* 00000047 //C 0f933  2677 db	71 */
0x0000f934, 0x00000065, /* 00000065 //C 0f934  2677 db	101 */
0x0000f935, 0x00000074, /* 00000074 //C 0f935  2677 db	116 */
0x0000f936, 0x0000002f, /* 0000002f //C 0f936  2677 db	47 */
0x0000f937, 0x00000073, /* 00000073 //C 0f937  2677 db	115 */
0x0000f938, 0x00000065, /* 00000065 //C 0f938  2677 db	101 */
0x0000f939, 0x00000074, /* 00000074 //C 0f939  2677 db	116 */
0x0000f93a, 0x00000020, /* 00000020 //C 0f93a  2677 db	32 */
0x0000f93b, 0x0000006d, /* 0000006d //C 0f93b  2677 db	109 */
0x0000f93c, 0x00000065, /* 00000065 //C 0f93c  2677 db	101 */
0x0000f93d, 0x0000006d, /* 0000006d //C 0f93d  2677 db	109 */
0x0000f93e, 0x0000006f, /* 0000006f //C 0f93e  2677 db	111 */
0x0000f93f, 0x00000072, /* 00000072 //C 0f93f  2677 db	114 */
0x0000f940, 0x00000079, /* 00000079 //C 0f940  2677 db	121 */
0x0000f941, 0x0000000a, /* 0000000a //C 0f941  2677 db	10 */
0x0000f942, 0x00000000, /* 00000000 //C 0f942  2677 db */
0x0000f943, 0x00000064, /* 00000064 //C 0f943  2678 db	100 */
0x0000f944, 0x0000005b, /* 0000005b //C 0f944  2678 db	91 */
0x0000f945, 0x00000075, /* 00000075 //C 0f945  2678 db	117 */
0x0000f946, 0x0000006d, /* 0000006d //C 0f946  2678 db	109 */
0x0000f947, 0x00000070, /* 00000070 //C 0f947  2678 db	112 */
0x0000f948, 0x0000005d, /* 0000005d //C 0f948  2678 db	93 */
0x0000f949, 0x00000020, /* 00000020 //C 0f949  2678 db	32 */
0x0000f94a, 0x00000073, /* 00000073 //C 0f94a  2678 db	115 */
0x0000f94b, 0x00000074, /* 00000074 //C 0f94b  2678 db	116 */
0x0000f94c, 0x00000061, /* 00000061 //C 0f94c  2678 db	97 */
0x0000f94d, 0x00000072, /* 00000072 //C 0f94d  2678 db	114 */
0x0000f94e, 0x00000074, /* 00000074 //C 0f94e  2678 db	116 */
0x0000f94f, 0x00000020, /* 00000020 //C 0f94f  2678 db	32 */
0x0000f950, 0x00000065, /* 00000065 //C 0f950  2678 db	101 */
0x0000f951, 0x0000006e, /* 0000006e //C 0f951  2678 db	110 */
0x0000f952, 0x00000064, /* 00000064 //C 0f952  2678 db	100 */
0x0000f953, 0x00000020, /* 00000020 //C 0f953  2678 db	32 */
0x0000f954, 0x00000020, /* 00000020 //C 0f954  2678 db	32 */
0x0000f955, 0x00000044, /* 00000044 //C 0f955  2678 db	68 */
0x0000f956, 0x00000075, /* 00000075 //C 0f956  2678 db	117 */
0x0000f957, 0x0000006d, /* 0000006d //C 0f957  2678 db	109 */
0x0000f958, 0x00000070, /* 00000070 //C 0f958  2678 db	112 */
0x0000f959, 0x00000020, /* 00000020 //C 0f959  2678 db	32 */
0x0000f95a, 0x0000006d, /* 0000006d //C 0f95a  2678 db	109 */
0x0000f95b, 0x00000065, /* 00000065 //C 0f95b  2678 db	101 */
0x0000f95c, 0x0000006d, /* 0000006d //C 0f95c  2678 db	109 */
0x0000f95d, 0x0000006f, /* 0000006f //C 0f95d  2678 db	111 */
0x0000f95e, 0x00000072, /* 00000072 //C 0f95e  2678 db	114 */
0x0000f95f, 0x00000079, /* 00000079 //C 0f95f  2678 db	121 */
0x0000f960, 0x00000020, /* 00000020 //C 0f960  2678 db	32 */
0x0000f961, 0x00000063, /* 00000063 //C 0f961  2678 db	99 */
0x0000f962, 0x0000006f, /* 0000006f //C 0f962  2678 db	111 */
0x0000f963, 0x0000006e, /* 0000006e //C 0f963  2678 db	110 */
0x0000f964, 0x00000074, /* 00000074 //C 0f964  2678 db	116 */
0x0000f965, 0x00000065, /* 00000065 //C 0f965  2678 db	101 */
0x0000f966, 0x0000006e, /* 0000006e //C 0f966  2678 db	110 */
0x0000f967, 0x00000074, /* 00000074 //C 0f967  2678 db	116 */
0x0000f968, 0x0000000a, /* 0000000a //C 0f968  2678 db	10 */
0x0000f969, 0x00000000, /* 00000000 //C 0f969  2678 db */
0x0000f96a, 0x0000006c, /* 0000006c //C 0f96a  2679 db	108 */
0x0000f96b, 0x0000005b, /* 0000005b //C 0f96b  2679 db	91 */
0x0000f96c, 0x0000006f, /* 0000006f //C 0f96c  2679 db	111 */
0x0000f96d, 0x00000061, /* 00000061 //C 0f96d  2679 db	97 */
0x0000f96e, 0x00000064, /* 00000064 //C 0f96e  2679 db	100 */
0x0000f96f, 0x0000005d, /* 0000005d //C 0f96f  2679 db	93 */
0x0000f970, 0x00000020, /* 00000020 //C 0f970  2679 db	32 */
0x0000f971, 0x00000020, /* 00000020 //C 0f971  2679 db	32 */
0x0000f972, 0x00000020, /* 00000020 //C 0f972  2679 db	32 */
0x0000f973, 0x00000020, /* 00000020 //C 0f973  2679 db	32 */
0x0000f974, 0x00000020, /* 00000020 //C 0f974  2679 db	32 */
0x0000f975, 0x00000020, /* 00000020 //C 0f975  2679 db	32 */
0x0000f976, 0x00000020, /* 00000020 //C 0f976  2679 db	32 */
0x0000f977, 0x00000020, /* 00000020 //C 0f977  2679 db	32 */
0x0000f978, 0x00000020, /* 00000020 //C 0f978  2679 db	32 */
0x0000f979, 0x00000020, /* 00000020 //C 0f979  2679 db	32 */
0x0000f97a, 0x00000020, /* 00000020 //C 0f97a  2679 db	32 */
0x0000f97b, 0x00000020, /* 00000020 //C 0f97b  2679 db	32 */
0x0000f97c, 0x0000004c, /* 0000004c //C 0f97c  2679 db	76 */
0x0000f97d, 0x0000006f, /* 0000006f //C 0f97d  2679 db	111 */
0x0000f97e, 0x00000061, /* 00000061 //C 0f97e  2679 db	97 */
0x0000f97f, 0x00000064, /* 00000064 //C 0f97f  2679 db	100 */
0x0000f980, 0x00000020, /* 00000020 //C 0f980  2679 db	32 */
0x0000f981, 0x00000068, /* 00000068 //C 0f981  2679 db	104 */
0x0000f982, 0x00000065, /* 00000065 //C 0f982  2679 db	101 */
0x0000f983, 0x00000078, /* 00000078 //C 0f983  2679 db	120 */
0x0000f984, 0x00000020, /* 00000020 //C 0f984  2679 db	32 */
0x0000f985, 0x00000066, /* 00000066 //C 0f985  2679 db	102 */
0x0000f986, 0x00000069, /* 00000069 //C 0f986  2679 db	105 */
0x0000f987, 0x0000006c, /* 0000006c //C 0f987  2679 db	108 */
0x0000f988, 0x00000065, /* 00000065 //C 0f988  2679 db	101 */
0x0000f989, 0x00000020, /* 00000020 //C 0f989  2679 db	32 */
0x0000f98a, 0x00000074, /* 00000074 //C 0f98a  2679 db	116 */
0x0000f98b, 0x0000006f, /* 0000006f //C 0f98b  2679 db	111 */
0x0000f98c, 0x00000020, /* 00000020 //C 0f98c  2679 db	32 */
0x0000f98d, 0x0000006d, /* 0000006d //C 0f98d  2679 db	109 */
0x0000f98e, 0x00000065, /* 00000065 //C 0f98e  2679 db	101 */
0x0000f98f, 0x0000006d, /* 0000006d //C 0f98f  2679 db	109 */
0x0000f990, 0x0000006f, /* 0000006f //C 0f990  2679 db	111 */
0x0000f991, 0x00000072, /* 00000072 //C 0f991  2679 db	114 */
0x0000f992, 0x00000079, /* 00000079 //C 0f992  2679 db	121 */
0x0000f993, 0x0000000a, /* 0000000a //C 0f993  2679 db	10 */
0x0000f994, 0x00000000, /* 00000000 //C 0f994  2679 db */
0x0000f995, 0x00000067, /* 00000067 //C 0f995  2680 db	103 */
0x0000f996, 0x0000005b, /* 0000005b //C 0f996  2680 db	91 */
0x0000f997, 0x0000006f, /* 0000006f //C 0f997  2680 db	111 */
0x0000f998, 0x0000005d, /* 0000005d //C 0f998  2680 db	93 */
0x0000f999, 0x0000007c, /* 0000007c //C 0f999  2680 db	124 */
0x0000f99a, 0x00000072, /* 00000072 //C 0f99a  2680 db	114 */
0x0000f99b, 0x00000075, /* 00000075 //C 0f99b  2680 db	117 */
0x0000f99c, 0x0000006e, /* 0000006e //C 0f99c  2680 db	110 */
0x0000f99d, 0x00000020, /* 00000020 //C 0f99d  2680 db	32 */
0x0000f99e, 0x0000005b, /* 0000005b //C 0f99e  2680 db	91 */
0x0000f99f, 0x00000061, /* 00000061 //C 0f99f  2680 db	97 */
0x0000f9a0, 0x00000064, /* 00000064 //C 0f9a0  2680 db	100 */
0x0000f9a1, 0x00000064, /* 00000064 //C 0f9a1  2680 db	100 */
0x0000f9a2, 0x00000072, /* 00000072 //C 0f9a2  2680 db	114 */
0x0000f9a3, 0x0000005d, /* 0000005d //C 0f9a3  2680 db	93 */
0x0000f9a4, 0x00000020, /* 00000020 //C 0f9a4  2680 db	32 */
0x0000f9a5, 0x00000020, /* 00000020 //C 0f9a5  2680 db	32 */
0x0000f9a6, 0x00000020, /* 00000020 //C 0f9a6  2680 db	32 */
0x0000f9a7, 0x00000052, /* 00000052 //C 0f9a7  2680 db	82 */
0x0000f9a8, 0x00000075, /* 00000075 //C 0f9a8  2680 db	117 */
0x0000f9a9, 0x0000006e, /* 0000006e //C 0f9a9  2680 db	110 */
0x0000f9aa, 0x00000020, /* 00000020 //C 0f9aa  2680 db	32 */
0x0000f9ab, 0x00000066, /* 00000066 //C 0f9ab  2680 db	102 */
0x0000f9ac, 0x00000072, /* 00000072 //C 0f9ac  2680 db	114 */
0x0000f9ad, 0x0000006f, /* 0000006f //C 0f9ad  2680 db	111 */
0x0000f9ae, 0x0000006d, /* 0000006d //C 0f9ae  2680 db	109 */
0x0000f9af, 0x00000020, /* 00000020 //C 0f9af  2680 db	32 */
0x0000f9b0, 0x00000061, /* 00000061 //C 0f9b0  2680 db	97 */
0x0000f9b1, 0x00000064, /* 00000064 //C 0f9b1  2680 db	100 */
0x0000f9b2, 0x00000064, /* 00000064 //C 0f9b2  2680 db	100 */
0x0000f9b3, 0x00000072, /* 00000072 //C 0f9b3  2680 db	114 */
0x0000f9b4, 0x00000065, /* 00000065 //C 0f9b4  2680 db	101 */
0x0000f9b5, 0x00000073, /* 00000073 //C 0f9b5  2680 db	115 */
0x0000f9b6, 0x00000073, /* 00000073 //C 0f9b6  2680 db	115 */
0x0000f9b7, 0x0000000a, /* 0000000a //C 0f9b7  2680 db	10 */
0x0000f9b8, 0x00000000, /* 00000000 //C 0f9b8  2680 db */
0x0000f9b9, 0x00000072, /* 00000072 //C 0f9b9  2681 db	114 */
0x0000f9ba, 0x0000005b, /* 0000005b //C 0f9ba  2681 db	91 */
0x0000f9bb, 0x00000065, /* 00000065 //C 0f9bb  2681 db	101 */
0x0000f9bc, 0x00000067, /* 00000067 //C 0f9bc  2681 db	103 */
0x0000f9bd, 0x0000005b, /* 0000005b //C 0f9bd  2681 db	91 */
0x0000f9be, 0x00000073, /* 00000073 //C 0f9be  2681 db	115 */
0x0000f9bf, 0x0000005d, /* 0000005d //C 0f9bf  2681 db	93 */
0x0000f9c0, 0x0000005d, /* 0000005d //C 0f9c0  2681 db	93 */
0x0000f9c1, 0x00000020, /* 00000020 //C 0f9c1  2681 db	32 */
0x0000f9c2, 0x00000020, /* 00000020 //C 0f9c2  2681 db	32 */
0x0000f9c3, 0x00000020, /* 00000020 //C 0f9c3  2681 db	32 */
0x0000f9c4, 0x00000020, /* 00000020 //C 0f9c4  2681 db	32 */
0x0000f9c5, 0x00000020, /* 00000020 //C 0f9c5  2681 db	32 */
0x0000f9c6, 0x00000020, /* 00000020 //C 0f9c6  2681 db	32 */
0x0000f9c7, 0x00000020, /* 00000020 //C 0f9c7  2681 db	32 */
0x0000f9c8, 0x00000020, /* 00000020 //C 0f9c8  2681 db	32 */
0x0000f9c9, 0x00000020, /* 00000020 //C 0f9c9  2681 db	32 */
0x0000f9ca, 0x00000020, /* 00000020 //C 0f9ca  2681 db	32 */
0x0000f9cb, 0x00000050, /* 00000050 //C 0f9cb  2681 db	80 */
0x0000f9cc, 0x00000072, /* 00000072 //C 0f9cc  2681 db	114 */
0x0000f9cd, 0x00000069, /* 00000069 //C 0f9cd  2681 db	105 */
0x0000f9ce, 0x0000006e, /* 0000006e //C 0f9ce  2681 db	110 */
0x0000f9cf, 0x00000074, /* 00000074 //C 0f9cf  2681 db	116 */
0x0000f9d0, 0x00000020, /* 00000020 //C 0f9d0  2681 db	32 */
0x0000f9d1, 0x00000072, /* 00000072 //C 0f9d1  2681 db	114 */
0x0000f9d2, 0x00000065, /* 00000065 //C 0f9d2  2681 db	101 */
0x0000f9d3, 0x00000067, /* 00000067 //C 0f9d3  2681 db	103 */
0x0000f9d4, 0x00000069, /* 00000069 //C 0f9d4  2681 db	105 */
0x0000f9d5, 0x00000073, /* 00000073 //C 0f9d5  2681 db	115 */
0x0000f9d6, 0x00000074, /* 00000074 //C 0f9d6  2681 db	116 */
0x0000f9d7, 0x00000065, /* 00000065 //C 0f9d7  2681 db	101 */
0x0000f9d8, 0x00000072, /* 00000072 //C 0f9d8  2681 db	114 */
0x0000f9d9, 0x00000073, /* 00000073 //C 0f9d9  2681 db	115 */
0x0000f9da, 0x0000000a, /* 0000000a //C 0f9da  2681 db	10 */
0x0000f9db, 0x00000000, /* 00000000 //C 0f9db  2681 db */
0x0000f9dc, 0x00000072, /* 00000072 //C 0f9dc  2682 db	114 */
0x0000f9dd, 0x00000058, /* 00000058 //C 0f9dd  2682 db	88 */
0x0000f9de, 0x00000020, /* 00000020 //C 0f9de  2682 db	32 */
0x0000f9df, 0x0000005b, /* 0000005b //C 0f9df  2682 db	91 */
0x0000f9e0, 0x00000076, /* 00000076 //C 0f9e0  2682 db	118 */
0x0000f9e1, 0x00000061, /* 00000061 //C 0f9e1  2682 db	97 */
0x0000f9e2, 0x0000006c, /* 0000006c //C 0f9e2  2682 db	108 */
0x0000f9e3, 0x0000005d, /* 0000005d //C 0f9e3  2682 db	93 */
0x0000f9e4, 0x00000020, /* 00000020 //C 0f9e4  2682 db	32 */
0x0000f9e5, 0x00000020, /* 00000020 //C 0f9e5  2682 db	32 */
0x0000f9e6, 0x00000020, /* 00000020 //C 0f9e6  2682 db	32 */
0x0000f9e7, 0x00000020, /* 00000020 //C 0f9e7  2682 db	32 */
0x0000f9e8, 0x00000020, /* 00000020 //C 0f9e8  2682 db	32 */
0x0000f9e9, 0x00000020, /* 00000020 //C 0f9e9  2682 db	32 */
0x0000f9ea, 0x00000020, /* 00000020 //C 0f9ea  2682 db	32 */
0x0000f9eb, 0x00000020, /* 00000020 //C 0f9eb  2682 db	32 */
0x0000f9ec, 0x00000020, /* 00000020 //C 0f9ec  2682 db	32 */
0x0000f9ed, 0x00000020, /* 00000020 //C 0f9ed  2682 db	32 */
0x0000f9ee, 0x00000047, /* 00000047 //C 0f9ee  2682 db	71 */
0x0000f9ef, 0x00000065, /* 00000065 //C 0f9ef  2682 db	101 */
0x0000f9f0, 0x00000074, /* 00000074 //C 0f9f0  2682 db	116 */
0x0000f9f1, 0x0000002f, /* 0000002f //C 0f9f1  2682 db	47 */
0x0000f9f2, 0x00000073, /* 00000073 //C 0f9f2  2682 db	115 */
0x0000f9f3, 0x00000065, /* 00000065 //C 0f9f3  2682 db	101 */
0x0000f9f4, 0x00000074, /* 00000074 //C 0f9f4  2682 db	116 */
0x0000f9f5, 0x00000020, /* 00000020 //C 0f9f5  2682 db	32 */
0x0000f9f6, 0x00000052, /* 00000052 //C 0f9f6  2682 db	82 */
0x0000f9f7, 0x00000058, /* 00000058 //C 0f9f7  2682 db	88 */
0x0000f9f8, 0x0000000a, /* 0000000a //C 0f9f8  2682 db	10 */
0x0000f9f9, 0x00000000, /* 00000000 //C 0f9f9  2682 db */
0x0000f9fa, 0x00000073, /* 00000073 //C 0f9fa  2683 db	115 */
0x0000f9fb, 0x00000070, /* 00000070 //C 0f9fb  2683 db	112 */
0x0000f9fc, 0x00000020, /* 00000020 //C 0f9fc  2683 db	32 */
0x0000f9fd, 0x0000005b, /* 0000005b //C 0f9fd  2683 db	91 */
0x0000f9fe, 0x00000076, /* 00000076 //C 0f9fe  2683 db	118 */
0x0000f9ff, 0x00000061, /* 00000061 //C 0f9ff  2683 db	97 */
0x0000fa00, 0x0000006c, /* 0000006c //C 0fa00  2683 db	108 */
0x0000fa01, 0x0000005d, /* 0000005d //C 0fa01  2683 db	93 */
0x0000fa02, 0x00000020, /* 00000020 //C 0fa02  2683 db	32 */
0x0000fa03, 0x00000020, /* 00000020 //C 0fa03  2683 db	32 */
0x0000fa04, 0x00000020, /* 00000020 //C 0fa04  2683 db	32 */
0x0000fa05, 0x00000020, /* 00000020 //C 0fa05  2683 db	32 */
0x0000fa06, 0x00000020, /* 00000020 //C 0fa06  2683 db	32 */
0x0000fa07, 0x00000020, /* 00000020 //C 0fa07  2683 db	32 */
0x0000fa08, 0x00000020, /* 00000020 //C 0fa08  2683 db	32 */
0x0000fa09, 0x00000020, /* 00000020 //C 0fa09  2683 db	32 */
0x0000fa0a, 0x00000020, /* 00000020 //C 0fa0a  2683 db	32 */
0x0000fa0b, 0x00000020, /* 00000020 //C 0fa0b  2683 db	32 */
0x0000fa0c, 0x00000047, /* 00000047 //C 0fa0c  2683 db	71 */
0x0000fa0d, 0x00000065, /* 00000065 //C 0fa0d  2683 db	101 */
0x0000fa0e, 0x00000074, /* 00000074 //C 0fa0e  2683 db	116 */
0x0000fa0f, 0x0000002f, /* 0000002f //C 0fa0f  2683 db	47 */
0x0000fa10, 0x00000073, /* 00000073 //C 0fa10  2683 db	115 */
0x0000fa11, 0x00000065, /* 00000065 //C 0fa11  2683 db	101 */
0x0000fa12, 0x00000074, /* 00000074 //C 0fa12  2683 db	116 */
0x0000fa13, 0x00000020, /* 00000020 //C 0fa13  2683 db	32 */
0x0000fa14, 0x00000052, /* 00000052 //C 0fa14  2683 db	82 */
0x0000fa15, 0x00000031, /* 00000031 //C 0fa15  2683 db	49 */
0x0000fa16, 0x00000033, /* 00000033 //C 0fa16  2683 db	51 */
0x0000fa17, 0x0000000a, /* 0000000a //C 0fa17  2683 db	10 */
0x0000fa18, 0x00000000, /* 00000000 //C 0fa18  2683 db */
0x0000fa19, 0x0000006c, /* 0000006c //C 0fa19  2684 db	108 */
0x0000fa1a, 0x00000072, /* 00000072 //C 0fa1a  2684 db	114 */
0x0000fa1b, 0x00000020, /* 00000020 //C 0fa1b  2684 db	32 */
0x0000fa1c, 0x0000005b, /* 0000005b //C 0fa1c  2684 db	91 */
0x0000fa1d, 0x00000076, /* 00000076 //C 0fa1d  2684 db	118 */
0x0000fa1e, 0x00000061, /* 00000061 //C 0fa1e  2684 db	97 */
0x0000fa1f, 0x0000006c, /* 0000006c //C 0fa1f  2684 db	108 */
0x0000fa20, 0x0000005d, /* 0000005d //C 0fa20  2684 db	93 */
0x0000fa21, 0x00000020, /* 00000020 //C 0fa21  2684 db	32 */
0x0000fa22, 0x00000020, /* 00000020 //C 0fa22  2684 db	32 */
0x0000fa23, 0x00000020, /* 00000020 //C 0fa23  2684 db	32 */
0x0000fa24, 0x00000020, /* 00000020 //C 0fa24  2684 db	32 */
0x0000fa25, 0x00000020, /* 00000020 //C 0fa25  2684 db	32 */
0x0000fa26, 0x00000020, /* 00000020 //C 0fa26  2684 db	32 */
0x0000fa27, 0x00000020, /* 00000020 //C 0fa27  2684 db	32 */
0x0000fa28, 0x00000020, /* 00000020 //C 0fa28  2684 db	32 */
0x0000fa29, 0x00000020, /* 00000020 //C 0fa29  2684 db	32 */
0x0000fa2a, 0x00000020, /* 00000020 //C 0fa2a  2684 db	32 */
0x0000fa2b, 0x00000047, /* 00000047 //C 0fa2b  2684 db	71 */
0x0000fa2c, 0x00000065, /* 00000065 //C 0fa2c  2684 db	101 */
0x0000fa2d, 0x00000074, /* 00000074 //C 0fa2d  2684 db	116 */
0x0000fa2e, 0x0000002f, /* 0000002f //C 0fa2e  2684 db	47 */
0x0000fa2f, 0x00000073, /* 00000073 //C 0fa2f  2684 db	115 */
0x0000fa30, 0x00000065, /* 00000065 //C 0fa30  2684 db	101 */
0x0000fa31, 0x00000074, /* 00000074 //C 0fa31  2684 db	116 */
0x0000fa32, 0x00000020, /* 00000020 //C 0fa32  2684 db	32 */
0x0000fa33, 0x00000052, /* 00000052 //C 0fa33  2684 db	82 */
0x0000fa34, 0x00000031, /* 00000031 //C 0fa34  2684 db	49 */
0x0000fa35, 0x00000034, /* 00000034 //C 0fa35  2684 db	52 */
0x0000fa36, 0x0000000a, /* 0000000a //C 0fa36  2684 db	10 */
0x0000fa37, 0x00000000, /* 00000000 //C 0fa37  2684 db */
0x0000fa38, 0x00000070, /* 00000070 //C 0fa38  2685 db	112 */
0x0000fa39, 0x00000063, /* 00000063 //C 0fa39  2685 db	99 */
0x0000fa3a, 0x00000020, /* 00000020 //C 0fa3a  2685 db	32 */
0x0000fa3b, 0x0000005b, /* 0000005b //C 0fa3b  2685 db	91 */
0x0000fa3c, 0x00000076, /* 00000076 //C 0fa3c  2685 db	118 */
0x0000fa3d, 0x00000061, /* 00000061 //C 0fa3d  2685 db	97 */
0x0000fa3e, 0x0000006c, /* 0000006c //C 0fa3e  2685 db	108 */
0x0000fa3f, 0x0000005d, /* 0000005d //C 0fa3f  2685 db	93 */
0x0000fa40, 0x00000020, /* 00000020 //C 0fa40  2685 db	32 */
0x0000fa41, 0x00000020, /* 00000020 //C 0fa41  2685 db	32 */
0x0000fa42, 0x00000020, /* 00000020 //C 0fa42  2685 db	32 */
0x0000fa43, 0x00000020, /* 00000020 //C 0fa43  2685 db	32 */
0x0000fa44, 0x00000020, /* 00000020 //C 0fa44  2685 db	32 */
0x0000fa45, 0x00000020, /* 00000020 //C 0fa45  2685 db	32 */
0x0000fa46, 0x00000020, /* 00000020 //C 0fa46  2685 db	32 */
0x0000fa47, 0x00000020, /* 00000020 //C 0fa47  2685 db	32 */
0x0000fa48, 0x00000020, /* 00000020 //C 0fa48  2685 db	32 */
0x0000fa49, 0x00000020, /* 00000020 //C 0fa49  2685 db	32 */
0x0000fa4a, 0x00000047, /* 00000047 //C 0fa4a  2685 db	71 */
0x0000fa4b, 0x00000065, /* 00000065 //C 0fa4b  2685 db	101 */
0x0000fa4c, 0x00000074, /* 00000074 //C 0fa4c  2685 db	116 */
0x0000fa4d, 0x0000002f, /* 0000002f //C 0fa4d  2685 db	47 */
0x0000fa4e, 0x00000073, /* 00000073 //C 0fa4e  2685 db	115 */
0x0000fa4f, 0x00000065, /* 00000065 //C 0fa4f  2685 db	101 */
0x0000fa50, 0x00000074, /* 00000074 //C 0fa50  2685 db	116 */
0x0000fa51, 0x00000020, /* 00000020 //C 0fa51  2685 db	32 */
0x0000fa52, 0x00000052, /* 00000052 //C 0fa52  2685 db	82 */
0x0000fa53, 0x00000031, /* 00000031 //C 0fa53  2685 db	49 */
0x0000fa54, 0x00000035, /* 00000035 //C 0fa54  2685 db	53 */
0x0000fa55, 0x0000000a, /* 0000000a //C 0fa55  2685 db	10 */
0x0000fa56, 0x00000000, /* 00000000 //C 0fa56  2685 db */
0x0000fa57, 0x00000066, /* 00000066 //C 0fa57  2686 db	102 */
0x0000fa58, 0x00000020, /* 00000020 //C 0fa58  2686 db	32 */
0x0000fa59, 0x0000005b, /* 0000005b //C 0fa59  2686 db	91 */
0x0000fa5a, 0x00000076, /* 00000076 //C 0fa5a  2686 db	118 */
0x0000fa5b, 0x00000061, /* 00000061 //C 0fa5b  2686 db	97 */
0x0000fa5c, 0x0000006c, /* 0000006c //C 0fa5c  2686 db	108 */
0x0000fa5d, 0x0000005d, /* 0000005d //C 0fa5d  2686 db	93 */
0x0000fa5e, 0x00000020, /* 00000020 //C 0fa5e  2686 db	32 */
0x0000fa5f, 0x00000020, /* 00000020 //C 0fa5f  2686 db	32 */
0x0000fa60, 0x00000020, /* 00000020 //C 0fa60  2686 db	32 */
0x0000fa61, 0x00000020, /* 00000020 //C 0fa61  2686 db	32 */
0x0000fa62, 0x00000020, /* 00000020 //C 0fa62  2686 db	32 */
0x0000fa63, 0x00000020, /* 00000020 //C 0fa63  2686 db	32 */
0x0000fa64, 0x00000020, /* 00000020 //C 0fa64  2686 db	32 */
0x0000fa65, 0x00000020, /* 00000020 //C 0fa65  2686 db	32 */
0x0000fa66, 0x00000020, /* 00000020 //C 0fa66  2686 db	32 */
0x0000fa67, 0x00000020, /* 00000020 //C 0fa67  2686 db	32 */
0x0000fa68, 0x00000020, /* 00000020 //C 0fa68  2686 db	32 */
0x0000fa69, 0x00000047, /* 00000047 //C 0fa69  2686 db	71 */
0x0000fa6a, 0x00000065, /* 00000065 //C 0fa6a  2686 db	101 */
0x0000fa6b, 0x00000074, /* 00000074 //C 0fa6b  2686 db	116 */
0x0000fa6c, 0x0000002f, /* 0000002f //C 0fa6c  2686 db	47 */
0x0000fa6d, 0x00000073, /* 00000073 //C 0fa6d  2686 db	115 */
0x0000fa6e, 0x00000065, /* 00000065 //C 0fa6e  2686 db	101 */
0x0000fa6f, 0x00000074, /* 00000074 //C 0fa6f  2686 db	116 */
0x0000fa70, 0x00000020, /* 00000020 //C 0fa70  2686 db	32 */
0x0000fa71, 0x00000066, /* 00000066 //C 0fa71  2686 db	102 */
0x0000fa72, 0x0000006c, /* 0000006c //C 0fa72  2686 db	108 */
0x0000fa73, 0x00000061, /* 00000061 //C 0fa73  2686 db	97 */
0x0000fa74, 0x00000067, /* 00000067 //C 0fa74  2686 db	103 */
0x0000fa75, 0x00000073, /* 00000073 //C 0fa75  2686 db	115 */
0x0000fa76, 0x0000000a, /* 0000000a //C 0fa76  2686 db	10 */
0x0000fa77, 0x00000000, /* 00000000 //C 0fa77  2686 db */
0x0000fa78, 0x00000076, /* 00000076 //C 0fa78  2687 db	118 */
0x0000fa79, 0x0000005b, /* 0000005b //C 0fa79  2687 db	91 */
0x0000fa7a, 0x00000065, /* 00000065 //C 0fa7a  2687 db	101 */
0x0000fa7b, 0x00000072, /* 00000072 //C 0fa7b  2687 db	114 */
0x0000fa7c, 0x0000005d, /* 0000005d //C 0fa7c  2687 db	93 */
0x0000fa7d, 0x00000020, /* 00000020 //C 0fa7d  2687 db	32 */
0x0000fa7e, 0x00000020, /* 00000020 //C 0fa7e  2687 db	32 */
0x0000fa7f, 0x00000020, /* 00000020 //C 0fa7f  2687 db	32 */
0x0000fa80, 0x00000020, /* 00000020 //C 0fa80  2687 db	32 */
0x0000fa81, 0x00000020, /* 00000020 //C 0fa81  2687 db	32 */
0x0000fa82, 0x00000020, /* 00000020 //C 0fa82  2687 db	32 */
0x0000fa83, 0x00000020, /* 00000020 //C 0fa83  2687 db	32 */
0x0000fa84, 0x00000020, /* 00000020 //C 0fa84  2687 db	32 */
0x0000fa85, 0x00000020, /* 00000020 //C 0fa85  2687 db	32 */
0x0000fa86, 0x00000020, /* 00000020 //C 0fa86  2687 db	32 */
0x0000fa87, 0x00000020, /* 00000020 //C 0fa87  2687 db	32 */
0x0000fa88, 0x00000020, /* 00000020 //C 0fa88  2687 db	32 */
0x0000fa89, 0x00000020, /* 00000020 //C 0fa89  2687 db	32 */
0x0000fa8a, 0x00000050, /* 00000050 //C 0fa8a  2687 db	80 */
0x0000fa8b, 0x00000072, /* 00000072 //C 0fa8b  2687 db	114 */
0x0000fa8c, 0x00000069, /* 00000069 //C 0fa8c  2687 db	105 */
0x0000fa8d, 0x0000006e, /* 0000006e //C 0fa8d  2687 db	110 */
0x0000fa8e, 0x00000074, /* 00000074 //C 0fa8e  2687 db	116 */
0x0000fa8f, 0x00000020, /* 00000020 //C 0fa8f  2687 db	32 */
0x0000fa90, 0x00000070, /* 00000070 //C 0fa90  2687 db	112 */
0x0000fa91, 0x0000006d, /* 0000006d //C 0fa91  2687 db	109 */
0x0000fa92, 0x0000006f, /* 0000006f //C 0fa92  2687 db	111 */
0x0000fa93, 0x0000006e, /* 0000006e //C 0fa93  2687 db	110 */
0x0000fa94, 0x00000020, /* 00000020 //C 0fa94  2687 db	32 */
0x0000fa95, 0x00000061, /* 00000061 //C 0fa95  2687 db	97 */
0x0000fa96, 0x0000006e, /* 0000006e //C 0fa96  2687 db	110 */
0x0000fa97, 0x00000064, /* 00000064 //C 0fa97  2687 db	100 */
0x0000fa98, 0x00000020, /* 00000020 //C 0fa98  2687 db	32 */
0x0000fa99, 0x00000063, /* 00000063 //C 0fa99  2687 db	99 */
0x0000fa9a, 0x00000070, /* 00000070 //C 0fa9a  2687 db	112 */
0x0000fa9b, 0x00000075, /* 00000075 //C 0fa9b  2687 db	117 */
0x0000fa9c, 0x00000020, /* 00000020 //C 0fa9c  2687 db	32 */
0x0000fa9d, 0x00000076, /* 00000076 //C 0fa9d  2687 db	118 */
0x0000fa9e, 0x00000065, /* 00000065 //C 0fa9e  2687 db	101 */
0x0000fa9f, 0x00000072, /* 00000072 //C 0fa9f  2687 db	114 */
0x0000faa0, 0x00000073, /* 00000073 //C 0faa0  2687 db	115 */
0x0000faa1, 0x00000069, /* 00000069 //C 0faa1  2687 db	105 */
0x0000faa2, 0x0000006f, /* 0000006f //C 0faa2  2687 db	111 */
0x0000faa3, 0x0000006e, /* 0000006e //C 0faa3  2687 db	110 */
0x0000faa4, 0x0000000a, /* 0000000a //C 0faa4  2687 db	10 */
0x0000faa5, 0x00000000, /* 00000000 //C 0faa5  2687 db */
0x0000faa6, 0x00000068, /* 00000068 //C 0faa6  2688 db	104 */
0x0000faa7, 0x0000005b, /* 0000005b //C 0faa7  2688 db	91 */
0x0000faa8, 0x00000065, /* 00000065 //C 0faa8  2688 db	101 */
0x0000faa9, 0x0000006c, /* 0000006c //C 0faa9  2688 db	108 */
0x0000faaa, 0x00000070, /* 00000070 //C 0faaa  2688 db	112 */
0x0000faab, 0x0000005d, /* 0000005d //C 0faab  2688 db	93 */
0x0000faac, 0x0000002c, /* 0000002c //C 0faac  2688 db	44 */
0x0000faad, 0x0000003f, /* 0000003f //C 0faad  2688 db	63 */
0x0000faae, 0x00000020, /* 00000020 //C 0faae  2688 db	32 */
0x0000faaf, 0x00000020, /* 00000020 //C 0faaf  2688 db	32 */
0x0000fab0, 0x00000020, /* 00000020 //C 0fab0  2688 db	32 */
0x0000fab1, 0x00000020, /* 00000020 //C 0fab1  2688 db	32 */
0x0000fab2, 0x00000020, /* 00000020 //C 0fab2  2688 db	32 */
0x0000fab3, 0x00000020, /* 00000020 //C 0fab3  2688 db	32 */
0x0000fab4, 0x00000020, /* 00000020 //C 0fab4  2688 db	32 */
0x0000fab5, 0x00000020, /* 00000020 //C 0fab5  2688 db	32 */
0x0000fab6, 0x00000020, /* 00000020 //C 0fab6  2688 db	32 */
0x0000fab7, 0x00000020, /* 00000020 //C 0fab7  2688 db	32 */
0x0000fab8, 0x00000048, /* 00000048 //C 0fab8  2688 db	72 */
0x0000fab9, 0x00000065, /* 00000065 //C 0fab9  2688 db	101 */
0x0000faba, 0x0000006c, /* 0000006c //C 0faba  2688 db	108 */
0x0000fabb, 0x00000070, /* 00000070 //C 0fabb  2688 db	112 */
0x0000fabc, 0x0000000a, /* 0000000a //C 0fabc  2688 db	10 */
0x0000fabd, 0x00000000, /* 00000000 //C 0fabd  2688 db */
0x0000fabe, 0x00000000, /* 00000000 //C 0fabe  2689 dd	0 */
0x0000fabf, 0x00000000, /* 00000000 //C 0fabf  2693 dd	0 */
0x0000fac0, 0x00000000, /* 00000000 //C 0fac0  2694 dd	0 */
0x0000fac1, 0x00000000, /* 00000000 //C 0fac1  2695 dd	0 */
0x0000fac2, 0x00000000, /* 00000000 //C 0fac2  2696 dd	0 */
0x0000fac3, 0x00000000, /* 00000000 //C 0fac3  2697 dd	0 */
0x0000fac4, 0x00000000, /* 00000000 //C 0fac4  2698 dd	0 */
0x0000fac5, 0x00000000, /* 00000000 //C 0fac5  2699 dd	0 */
0x0000fac6, 0x00000000, /* 00000000 //C 0fac6  2700 dd	0 */
0x0000fac7, 0x00220000, /* 00220000 //C 0fac7  2701 dd	0x00220000 */
0x0000fac8, 0x02000000, /* 02000000 //C 0fac8  2702 dd	0x02000000 */
0x0000fac9, 0x00000000, /* 00000000 //C 0fac9  2703 dd	0x00000000 */
0x0000faca, 0x00004000, /* 00004000 //C 0faca  2704 dd	0x00004000 */
0x0000facb, 0x4f5b063f, /* 4f5b063f //C 0facb  2705 dd	0x4f5b063f */
0x0000facc, 0x277d6d66, /* 277d6d66 //C 0facc  2706 dd	0x277d6d66 */
0x0000facd, 0x00006fff, /* 00006fff //C 0facd  2707 dd	0x00006fff */
0x0000face, 0x00004800, /* 00004800 //C 0face  2708 dd	0x00004800 */
0x0000facf, 0x397c7700, /* 397c7700 //C 0facf  2709 dd	0x397c7700 */
0x0000fad0, 0x3d71795e, /* 3d71795e //C 0fad0  2710 dd	0x3d71795e */
0x0000fad1, 0x001e3076, /* 001e3076 //C 0fad1  2711 dd	0x001e3076 */
0x0000fad2, 0x3f543738, /* 3f543738 //C 0fad2  2712 dd	0x3f543738 */
0x0000fad3, 0x6d500073, /* 6d500073 //C 0fad3  2713 dd	0x6d500073 */
0x0000fad4, 0x00003e78, /* 00003e78 //C 0fad4  2714 dd	0x00003e78 */
0x0000fad5, 0x39006e00, /* 39006e00 //C 0fad5  2715 dd	0x39006e00 */
0x0000fad6, 0x08000f00, /* 08000f00 //C 0fad6  2716 dd	0x08000f00 */
0x0000fad7, 0x587c7700, /* 587c7700 //C 0fad7  2717 dd	0x587c7700 */
0x0000fad8, 0x3d71795e, /* 3d71795e //C 0fad8  2718 dd	0x3d71795e */
0x0000fad9, 0x001e0474, /* 001e0474 //C 0fad9  2719 dd	0x001e0474 */
0x0000fada, 0x5c540038, /* 5c540038 //C 0fada  2720 dd	0x5c540038 */
0x0000fadb, 0x6d500073, /* 6d500073 //C 0fadb  2721 dd	0x6d500073 */
0x0000fadc, 0x00001c78, /* 00001c78 //C 0fadc  2722 dd	0x00001c78 */
0x0000fadd, 0x00000000, /* 00000000 //C 0fadd  2723 dd	0x00000000 */
0x0000fade, 0x00000000, /* 00000000 //C 0fade  2724 dd	0x00000000 */
0x0000fadf, 0x00000000, /* 00000000 //C 0fadf  2729 ds	0x40 */
0x0000fb1f, 0x00000000, /* 00000000 //C 0fb1f  2731 dd	0 */
0x0000fb20, 0x00000000, /* 00000000 //C 0fb20     3  */
0xffffffff, 0xffffffff
};
