#include "pmon.h"

t_mem pmon[]= {
0x00000000, 0x01f2f825, /* 01f2f825 //C 00000 jmp	cold_start */
0x0000f800, 0x01f2f80e, /* 01f2f80e //C 0f800 _f800:	jmp	callin */
0x0000f801, 0x01f2f825, /* 01f2f825 //C 0f801 _f801:	jmp	cold_start */
0x0000f802, 0x01f2fb00, /* 01f2fb00 //C 0f802 _f802:	jmp	strchr */
0x0000f803, 0x01f2fb3b, /* 01f2fb3b //C 0f803 _f803:	jmp	streq */
0x0000f804, 0x01f2fb42, /* 01f2fb42 //C 0f804 _f804:	jmp	strieq */
0x0000f805, 0x01f2fb49, /* 01f2fb49 //C 0f805 _f805:	jmp	hexchar2value */
0x0000f806, 0x01f2fb78, /* 01f2fb78 //C 0f806 _f806:	jmp	value2hexchar */
0x0000f807, 0x01f2fb7d, /* 01f2fb7d //C 0f807 _f807:	jmp	htoi */
0x0000f808, 0x01f2fb9f, /* 01f2fb9f //C 0f808 _f808:	jmp	check_uart */
0x0000f809, 0x01f2fba6, /* 01f2fba6 //C 0f809 _f809:	jmp	read */
0x0000f80a, 0x01f2fba8, /* 01f2fba8 //C 0f80a _f80a:	jmp	putchar */
0x0000f80b, 0x01f2fbaf, /* 01f2fbaf //C 0f80b _f80b:	jmp	prints */
0x0000f80c, 0x01f2fbc3, /* 01f2fbc3 //C 0f80c _f80c:	jmp	printsnl */
0x0000f80d, 0x01f2fbcb, /* 01f2fbcb //C 0f80d _f80d:	jmp	print_vhex */
0x0000f80e, 0x0600fc60, /* 0600fc60 //C 0f80e st	r0,reg0 */
0x0000f80f, 0x0610fc61, /* 0610fc61 //C 0f80f st	r1,reg1 */
0x0000f810, 0x0620fc62, /* 0620fc62 //C 0f810 st	r2,reg2 */
0x0000f811, 0x0630fc63, /* 0630fc63 //C 0f811 st	r3,reg3 */
0x0000f812, 0x0640fc64, /* 0640fc64 //C 0f812 st	r4,reg4 */
0x0000f813, 0x0650fc65, /* 0650fc65 //C 0f813 st	r5,reg5 */
0x0000f814, 0x0660fc66, /* 0660fc66 //C 0f814 st	r6,reg6 */
0x0000f815, 0x0670fc67, /* 0670fc67 //C 0f815 st	r7,reg7 */
0x0000f816, 0x0680fc68, /* 0680fc68 //C 0f816 st	r8,reg8 */
0x0000f817, 0x0690fc69, /* 0690fc69 //C 0f817 st	r9,reg9 */
0x0000f818, 0x06a0fc6a, /* 06a0fc6a //C 0f818 st	r10,reg10 */
0x0000f819, 0x06b0fc6b, /* 06b0fc6b //C 0f819 st	r11,reg11 */
0x0000f81a, 0x06c0fc6c, /* 06c0fc6c //C 0f81a st	r12,reg12 */
0x0000f81b, 0x06d0fc6d, /* 06d0fc6d //C 0f81b st	r13,reg13 */
0x0000f81c, 0x06e0fc6e, /* 06e0fc6e //C 0f81c st	r14,reg14 */
0x0000f81d, 0x06e0fc6f, /* 06e0fc6f //C 0f81d st	r14,reg15 */
0x0000f81e, 0x020e0000, /* 020e0000 //C 0f81e getf	r0 */
0x0000f81f, 0x0600fc70, /* 0600fc70 //C 0f81f st	r0,regf */
0x0000f820, 0x01020001, /* 01020001 //C 0f820 mvzl	r0,1 */
0x0000f821, 0x0600fc5e, /* 0600fc5e //C 0f821 st	r0,called */
0x0000f822, 0x01f2f82f, /* 01f2f82f //C 0f822 jmp	common_start */
0x0000f823, 0x01f2f82f, /* 01f2f82f //C 0f823 jmp	common_start */
0x0000f824, 0x01f2f825, /* 01f2f825 //C 0f824 jmp	cold_start */
0x0000f825, 0x01020000, /* 01020000 //C 0f825 mvzl	r0,0 */
0x0000f826, 0x0600fc5e, /* 0600fc5e //C 0f826 st	r0,called */
0x0000f827, 0x0102f824, /* 0102f824 //C 0f827 mvzl	r0,def_zero	; restore jmp to monitor at zero */
0x0000f828, 0x0e000000, /* 0e000000 //C 0f828 ld	r0,r0 */
0x0000f829, 0x06000000, /* 06000000 //C 0f829 st	r0,0 */
0x0000f82a, 0x01020000, /* 01020000 //C 0f82a mvzl	r0,0		; def values of some regs */
0x0000f82b, 0x0600fc70, /* 0600fc70 //C 0f82b st	r0,regf		; FALGS= 0 */
0x0000f82c, 0x0102f7ff, /* 0102f7ff //C 0f82c mvzl	r0,0xf7ff	; R13= 0xf7ff */
0x0000f82d, 0x0600fc6d, /* 0600fc6d //C 0f82d st	r0,reg13 */
0x0000f82e, 0x01f2f82f, /* 01f2f82f //C 0f82e jmp	common_start: */
0x0000f82f, 0x01d2ff52, /* 01d2ff52 //C 0f82f mvzl	sp,stack_end */
0x0000f830, 0x01020001, /* 01020001 //C 0f830 mvzl	r0,1 */
0x0000f831, 0x0600fc5d, /* 0600fc5d //C 0f831 st	r0,echo */
0x0000f832, 0x01020000, /* 01020000 //C 0f832 mvzl	r0,0 */
0x0000f833, 0x020f0000, /* 020f0000 //C 0f833 setf	r0 */
0x0000f834, 0x011200d9, /* 011200d9 //C 0f834 mvzl	r1,217 */
0x0000f835, 0x0610ff44, /* 0610ff44 //C 0f835 st	r1,UART_CPB */
0x0000f836, 0x01120003, /* 01120003 //C 0f836 mvzl	r1,3 */
0x0000f837, 0x0610ff41, /* 0610ff41 //C 0f837 st	r1,UART_CTRL */
0x0000f838, 0x0102fc71, /* 0102fc71 //C 0f838 mvzl	r0,msg_start */
0x0000f839, 0x0400fbc3, /* 0400fbc3 //C 0f839 call	printsnl */
0x0000f83a, 0x0400f83c, /* 0400f83c //C 0f83a call	setup_line */
0x0000f83b, 0x01f2f846, /* 01f2f846 //C 0f83b jmp	main */
0x0000f83c, 0x0ded0000, /* 0ded0000 //C 0f83c push	lr */
0x0000f83d, 0x01120000, /* 01120000 //C 0f83d mvzl	r1,0		; lptr= 0 */
0x0000f83e, 0x0610fc56, /* 0610fc56 //C 0f83e st	r1,line_ptr */
0x0000f83f, 0x0610fbf2, /* 0610fbf2 //C 0f83f st	r1,line		; line[0]= 0 */
0x0000f840, 0x01120000, /* 01120000 //C 0f840 mvzl	r1,0		; at_eol= 0 */
0x0000f841, 0x0610fc57, /* 0610fc57 //C 0f841 st	r1,at_eol */
0x0000f842, 0x0102fc7f, /* 0102fc7f //C 0f842 mvzl	r0,prompt */
0x0000f843, 0x0400fbaf, /* 0400fbaf //C 0f843 call	prints */
0x0000f844, 0x0fed0000, /* 0fed0000 //C 0f844 pop	lr */
0x0000f845, 0x00f00e00, /* 00f00e00 //C 0f845 ret */
0x0000f846, 0x0400fb9f, /* 0400fb9f //C 0f846 call	check_uart */
0x0000f847, 0x41f2f84d, /* 41f2f84d //C 0f847 C0 jmp	no_input */
0x0000f848, 0x0400fba6, /* 0400fba6 //C 0f848 call	read */
0x0000f849, 0x0400f84e, /* 0400f84e //C 0f849 call	proc_input */
0x0000f84a, 0x41f2f84d, /* 41f2f84d //C 0f84a C0 jmp	no_line */
0x0000f84b, 0x0400f86b, /* 0400f86b //C 0f84b call	proc_line */
0x0000f84c, 0x0400f83c, /* 0400f83c //C 0f84c call	setup_line */
0x0000f84d, 0x01f2f846, /* 01f2f846 //C 0f84d jmp	main */
0x0000f84e, 0x0ded0000, /* 0ded0000 //C 0f84e push	lr */
0x0000f84f, 0x0108000a, /* 0108000a //C 0f84f cmp	r0,LF */
0x0000f850, 0x11f2f864, /* 11f2f864 //C 0f850 EQ jmp	got_eol */
0x0000f851, 0x0108000d, /* 0108000d //C 0f851 cmp	r0,CR */
0x0000f852, 0x11f2f864, /* 11f2f864 //C 0f852 EQ jmp	got_eol */
0x0000f853, 0x01220000, /* 01220000 //C 0f853 mvzl	r2,0		; at_aol= 0 */
0x0000f854, 0x0620fc57, /* 0620fc57 //C 0f854 st	r2,at_eol */
0x0000f855, 0x0112fc56, /* 0112fc56 //C 0f855 mvzl	r1,line_ptr	; line[line_ptr]= char */
0x0000f856, 0x0e310000, /* 0e310000 //C 0f856 ld	r3,r1 */
0x0000f857, 0x0122fbf2, /* 0122fbf2 //C 0f857 mvzl	r2,line */
0x0000f858, 0x08020300, /* 08020300 //C 0f858 st	r0,r2,r3 */
0x0000f859, 0x013a0001, /* 013a0001 //C 0f859 plus	r3,1		; line_ptr++ */
0x0000f85a, 0x0c310000, /* 0c310000 //C 0f85a st	r3,r1 */
0x0000f85b, 0x01420000, /* 01420000 //C 0f85b mvzl	r4,0 */
0x0000f85c, 0x09438200, /* 09438200 //C 0f85c st	r4,r3+,r2	; line[line_ptr]= 0 */
0x0000f85d, 0x08430200, /* 08430200 //C 0f85d st	r4,r3,r2	; double 0 at end, needed by tokenizer */
0x0000f85e, 0x0142fc5d, /* 0142fc5d //C 0f85e mvzl	r4,echo		; check if echo is turned on */
0x0000f85f, 0x0e440000, /* 0e440000 //C 0f85f ld	r4,r4 */
0x0000f860, 0x024b0000, /* 024b0000 //C 0f860 sz	r4 */
0x0000f861, 0x2400fba8, /* 2400fba8 //C 0f861 NZ call	putchar		; echo */
0x0000f862, 0x020d0000, /* 020d0000 //C 0f862 clc */
0x0000f863, 0x01f2f869, /* 01f2f869 //C 0f863 jmp	proc_input_ret */
0x0000f864, 0x0112fc57, /* 0112fc57 //C 0f864 mvzl	r1,at_eol */
0x0000f865, 0x0e110000, /* 0e110000 //C 0f865 ld	r1,r1 */
0x0000f866, 0x021b0000, /* 021b0000 //C 0f866 sz	r1		; Z=0 at eol -> skip, not ready */
0x0000f867, 0x220d0000, /* 220d0000 //C 0f867 Z0 clc */
0x0000f868, 0x120c0000, /* 120c0000 //C 0f868 Z1 sec */
0x0000f869, 0x0fed0000, /* 0fed0000 //C 0f869 pop	lr */
0x0000f86a, 0x00f00e00, /* 00f00e00 //C 0f86a ret */
0x0000f86b, 0x0ded0000, /* 0ded0000 //C 0f86b push	lr */
0x0000f86c, 0x0102000a, /* 0102000a //C 0f86c mvzl	r0,LF */
0x0000f86d, 0x0400fba8, /* 0400fba8 //C 0f86d call	putchar */
0x0000f86e, 0x0700fbf2, /* 0700fbf2 //C 0f86e ld	r0,line */
0x0000f86f, 0x020b0000, /* 020b0000 //C 0f86f sz	r0 */
0x0000f870, 0x11f2f878, /* 11f2f878 //C 0f870 jz	proc_line_ret */
0x0000f871, 0x0400f896, /* 0400f896 //C 0f871 call	tokenize */
0x0000f872, 0x0400f8b7, /* 0400f8b7 //C 0f872 call	find_cmd */
0x0000f873, 0x41f2f876, /* 41f2f876 //C 0f873 C0 jmp	cmd_not_found */
0x0000f874, 0x05000000, /* 05000000 //C 0f874 call	r0,0 */
0x0000f875, 0x01f2f878, /* 01f2f878 //C 0f875 jmp	proc_line_ret */
0x0000f876, 0x0102f881, /* 0102f881 //C 0f876 mvzl	r0,snotfound */
0x0000f877, 0x0400fbc3, /* 0400fbc3 //C 0f877 call	printsnl */
0x0000f878, 0x01120001, /* 01120001 //C 0f878 mvzl	r1,1		; at_eol= 1 */
0x0000f879, 0x0610fc57, /* 0610fc57 //C 0f879 st	r1,at_eol */
0x0000f87a, 0x0fed0000, /* 0fed0000 //C 0f87a pop	lr */
0x0000f87b, 0x00f00e00, /* 00f00e00 //C 0f87b ret */
0x0000f87c, 0x00000047, /* 00000047 //C 0f87c db	71 */
0x0000f87d, 0x0000006f, /* 0000006f //C 0f87d db	111 */
0x0000f87e, 0x00000074, /* 00000074 //C 0f87e db	116 */
0x0000f87f, 0x0000003a, /* 0000003a //C 0f87f db	58 */
0x0000f880, 0x00000000, /* 00000000 //C 0f880 db */
0x0000f881, 0x00000055, /* 00000055 //C 0f881 db	85 */
0x0000f882, 0x0000006e, /* 0000006e //C 0f882 db	110 */
0x0000f883, 0x0000006b, /* 0000006b //C 0f883 db	107 */
0x0000f884, 0x0000006e, /* 0000006e //C 0f884 db	110 */
0x0000f885, 0x0000006f, /* 0000006f //C 0f885 db	111 */
0x0000f886, 0x00000077, /* 00000077 //C 0f886 db	119 */
0x0000f887, 0x0000006e, /* 0000006e //C 0f887 db	110 */
0x0000f888, 0x00000020, /* 00000020 //C 0f888 db	32 */
0x0000f889, 0x00000063, /* 00000063 //C 0f889 db	99 */
0x0000f88a, 0x0000006f, /* 0000006f //C 0f88a db	111 */
0x0000f88b, 0x0000006d, /* 0000006d //C 0f88b db	109 */
0x0000f88c, 0x0000006d, /* 0000006d //C 0f88c db	109 */
0x0000f88d, 0x00000061, /* 00000061 //C 0f88d db	97 */
0x0000f88e, 0x0000006e, /* 0000006e //C 0f88e db	110 */
0x0000f88f, 0x00000064, /* 00000064 //C 0f88f db	100 */
0x0000f890, 0x00000000, /* 00000000 //C 0f890 db */
0x0000f891, 0x0ded0000, /* 0ded0000 //C 0f891 push	lr */
0x0000f892, 0x0112fc81, /* 0112fc81 //C 0f892 mvzl	r1,delimiters */
0x0000f893, 0x0400fb00, /* 0400fb00 //C 0f893 call	strchr */
0x0000f894, 0x0fed0000, /* 0fed0000 //C 0f894 pop	lr */
0x0000f895, 0x00f00e00, /* 00f00e00 //C 0f895 ret */
0x0000f896, 0x0ded0000, /* 0ded0000 //C 0f896 push	lr */
0x0000f897, 0x0142fc58, /* 0142fc58 //C 0f897 mvzl	r4,words	; array of result */
0x0000f898, 0x0152fbf2, /* 0152fbf2 //C 0f898 mvzl	r5,line		; address of next char */
0x0000f899, 0x01620000, /* 01620000 //C 0f899 mvzl	r6,0		; nuof words found */
0x0000f89a, 0x01720000, /* 01720000 //C 0f89a mvzl	r7,0		; bool in_word */
0x0000f89b, 0x0e050000, /* 0e050000 //C 0f89b ld	r0,r5		; pick a char */
0x0000f89c, 0x020b0000, /* 020b0000 //C 0f89c sz	r0		; check end */
0x0000f89d, 0x11f2f8a7, /* 11f2f8a7 //C 0f89d jz	tok_delimiter	; found end, pretend delim */
0x0000f89e, 0x0400f891, /* 0400f891 //C 0f89e call	is_delimiter */
0x0000f89f, 0x31f2f8a7, /* 31f2f8a7 //C 0f89f C1 jmp	tok_delimiter */
0x0000f8a0, 0x027b0000, /* 027b0000 //C 0f8a0 sz	r7 */
0x0000f8a1, 0x21f2f8ac, /* 21f2f8ac //C 0f8a1 jnz	tok_next	; still inside word */
0x0000f8a2, 0x01720001, /* 01720001 //C 0f8a2 mvzl	r7,1		; in_word=true */
0x0000f8a3, 0x09568400, /* 09568400 //C 0f8a3 st	r5,r6+,r4	; record word address */
0x0000f8a4, 0x01680005, /* 01680005 //C 0f8a4 cmp	r6,MAX_WORDS	; If no more space */
0x0000f8a5, 0x11f2f8b0, /* 11f2f8b0 //C 0f8a5 EQ jmp	tok_ret		; then return */
0x0000f8a6, 0x01f2f8ac, /* 01f2f8ac //C 0f8a6 jmp	tok_next */
0x0000f8a7, 0x027b0000, /* 027b0000 //C 0f8a7 sz	r7 */
0x0000f8a8, 0x11f2f8ac, /* 11f2f8ac //C 0f8a8 jz	tok_next	; still between words */
0x0000f8a9, 0x01720000, /* 01720000 //C 0f8a9 mvzl	r7,0		; in_word=false */
0x0000f8aa, 0x01120000, /* 01120000 //C 0f8aa mvzl	r1,0		; put a 0 at the end of word */
0x0000f8ab, 0x08150100, /* 08150100 //C 0f8ab st	r1,r5,r1 */
0x0000f8ac, 0x020b0000, /* 020b0000 //C 0f8ac sz	r0		; check EOL */
0x0000f8ad, 0x11f2f8b0, /* 11f2f8b0 //C 0f8ad jz	tok_ret		; jump out if char==0 */
0x0000f8ae, 0x01540001, /* 01540001 //C 0f8ae add	r5,1 */
0x0000f8af, 0x01f2f89b, /* 01f2f89b //C 0f8af jmp	tok_cycle */
0x0000f8b0, 0x01120000, /* 01120000 //C 0f8b0 mvzl	r1,0 */
0x0000f8b1, 0x01680005, /* 01680005 //C 0f8b1 cmp	r6,MAX_WORDS */
0x0000f8b2, 0x11f2f8b5, /* 11f2f8b5 //C 0f8b2 jz	tok_end */
0x0000f8b3, 0x09168400, /* 09168400 //C 0f8b3 st	r1,r6+,r4 */
0x0000f8b4, 0x01f2f8b0, /* 01f2f8b0 //C 0f8b4 jmp	tok_ret */
0x0000f8b5, 0x0fed0000, /* 0fed0000 //C 0f8b5 pop	lr */
0x0000f8b6, 0x00f00e00, /* 00f00e00 //C 0f8b6 ret */
0x0000f8b7, 0x0ded0000, /* 0ded0000 //C 0f8b7 push	lr */
0x0000f8b8, 0x0d1d0000, /* 0d1d0000 //C 0f8b8 push	r1 */
0x0000f8b9, 0x0d2d0000, /* 0d2d0000 //C 0f8b9 push	r2 */
0x0000f8ba, 0x0d3d0000, /* 0d3d0000 //C 0f8ba push	r3 */
0x0000f8bb, 0x0dad0000, /* 0dad0000 //C 0f8bb push	r10 */
0x0000f8bc, 0x0700fc58, /* 0700fc58 //C 0f8bc ld	r0,words	; R0= 1st word of command */
0x0000f8bd, 0x020b0000, /* 020b0000 //C 0f8bd sz	r0 */
0x0000f8be, 0x11f2f8ee, /* 11f2f8ee //C 0f8be jz	find_cmd_false */
0x0000f8bf, 0x0e100000, /* 0e100000 //C 0f8bf ld	r1,r0		; 1st char of word1 */
0x0000f8c0, 0x0e200001, /* 0e200001 //C 0f8c0 ld	r2,r0,1		; 2nd char */
0x0000f8c1, 0x0e300002, /* 0e300002 //C 0f8c1 ld	r3,r0,2		; 3rd char */
0x0000f8c2, 0x011fffdf, /* 011fffdf //C 0f8c2 and	r1,0xffdf	; upcase 1st char */
0x0000f8c3, 0x01180052, /* 01180052 //C 0f8c3 cmp	r1,'R' */
0x0000f8c4, 0x21f2f8da, /* 21f2f8da //C 0f8c4 jnz	find_not_rx */
0x0000f8c5, 0x0128002f, /* 0128002f //C 0f8c5 cmp	r2,'/'		; '0'-1 */
0x0000f8c6, 0xa1f2f8da, /* a1f2f8da //C 0f8c6 LS jmp	find_not_rx */
0x0000f8c7, 0x01280039, /* 01280039 //C 0f8c7 cmp	r2,'9' */
0x0000f8c8, 0x91f2f8da, /* 91f2f8da //C 0f8c8 HI jmp	find_not_rx */
0x0000f8c9, 0x023b0000, /* 023b0000 //C 0f8c9 sz	r3 */
0x0000f8ca, 0x11f2f8d5, /* 11f2f8d5 //C 0f8ca jz	find_rx_09 */
0x0000f8cb, 0x01280031, /* 01280031 //C 0f8cb cmp	r2,'1'		; first char must be '1' */
0x0000f8cc, 0x21f2f8da, /* 21f2f8da //C 0f8cc jnz	find_not_rx */
0x0000f8cd, 0x0138002f, /* 0138002f //C 0f8cd cmp	r3,'/'		; '0'-1 */
0x0000f8ce, 0xa1f2f8da, /* a1f2f8da //C 0f8ce LS jmp	find_not_rx */
0x0000f8cf, 0x01380035, /* 01380035 //C 0f8cf cmp	r3,'5' */
0x0000f8d0, 0x91f2f8da, /* 91f2f8da //C 0f8d0 HI jmp	find_not_rx */
0x0000f8d1, 0x01360030, /* 01360030 //C 0f8d1 sub	r3,'0' */
0x0000f8d2, 0x0134000a, /* 0134000a //C 0f8d2 add	r3,10 */
0x0000f8d3, 0x0630fc5f, /* 0630fc5f //C 0f8d3 st	r3,nuof_reg */
0x0000f8d4, 0x01f2f8d7, /* 01f2f8d7 //C 0f8d4 jmp	find_rx */
0x0000f8d5, 0x01260030, /* 01260030 //C 0f8d5 sub	r2,'0' */
0x0000f8d6, 0x0620fc5f, /* 0620fc5f //C 0f8d6 st	r2,nuof_reg */
0x0000f8d7, 0x0102fabe, /* 0102fabe //C 0f8d7 mvzl	r0,cmd_rx */
0x0000f8d8, 0x020c0000, /* 020c0000 //C 0f8d8 sec */
0x0000f8d9, 0x01f2f8f9, /* 01f2f8f9 //C 0f8d9 jmp	find_cmd_ret */
0x0000f8da, 0x01a2fc91, /* 01a2fc91 //C 0f8da mvzl	r10,commands */
0x0000f8db, 0x0e2a0000, /* 0e2a0000 //C 0f8db ld	r2,r10		; R2= cmd addr */
0x0000f8dc, 0x022b0000, /* 022b0000 //C 0f8dc sz	r2 */
0x0000f8dd, 0x11f2f8ee, /* 11f2f8ee //C 0f8dd jz	find_cmd_false */
0x0000f8de, 0x01a40001, /* 01a40001 //C 0f8de add	r10,1 */
0x0000f8df, 0x00100a00, /* 00100a00 //C 0f8df mov	r1,r10		; R1= cmd string */
0x0000f8e0, 0x01a40001, /* 01a40001 //C 0f8e0 add	r10,1 */
0x0000f8e1, 0x0e3a0000, /* 0e3a0000 //C 0f8e1 ld	r3,r10 */
0x0000f8e2, 0x023b0000, /* 023b0000 //C 0f8e2 sz	r3 */
0x0000f8e3, 0x21f2f8e0, /* 21f2f8e0 //C 0f8e3 jnz	find_cmd_fw */
0x0000f8e4, 0x01a40001, /* 01a40001 //C 0f8e4 add	r10,1 */
0x0000f8e5, 0x0400fb3b, /* 0400fb3b //C 0f8e5 call	streq */
0x0000f8e6, 0x41f2f8db, /* 41f2f8db //C 0f8e6 C0 jmp	find_cmd_cyc */
0x0000f8e7, 0x00000200, /* 00000200 //C 0f8e7 mov	r0,r2 */
0x0000f8e8, 0x020c0000, /* 020c0000 //C 0f8e8 sec */
0x0000f8e9, 0x01f2f8f9, /* 01f2f8f9 //C 0f8e9 jmp	find_cmd_ret */
0x0000f8ea, 0x0000002f, /* 0000002f //C 0f8ea db	47 */
0x0000f8eb, 0x0000002f, /* 0000002f //C 0f8eb db	47 */
0x0000f8ec, 0x00000043, /* 00000043 //C 0f8ec db	67 */
0x0000f8ed, 0x00000000, /* 00000000 //C 0f8ed db */
0x0000f8ee, 0x01040001, /* 01040001 //C 0f8ee add	r0,1		; check second word */
0x0000f8ef, 0x0e100000, /* 0e100000 //C 0f8ef ld	r1,r0		; for //C command */
0x0000f8f0, 0x021b0000, /* 021b0000 //C 0f8f0 sz	r1 */
0x0000f8f1, 0x11f2f8f7, /* 11f2f8f7 //C 0f8f1 jz	find_cmd_very_false */
0x0000f8f2, 0x0102f8ea, /* 0102f8ea //C 0f8f2 mvzl	r0,c_cmd_name */
0x0000f8f3, 0x0400fb3b, /* 0400fb3b //C 0f8f3 call	streq */
0x0000f8f4, 0x21f2f8f7, /* 21f2f8f7 //C 0f8f4 jnz	find_cmd_very_false */
0x0000f8f5, 0x0122f987, /* 0122f987 //C 0f8f5 mvzl	r2,cmd_c */
0x0000f8f6, 0x01f2f8e7, /* 01f2f8e7 //C 0f8f6 jmp	find_cmd_true */
0x0000f8f7, 0x020d0000, /* 020d0000 //C 0f8f7 clc */
0x0000f8f8, 0x01020000, /* 01020000 //C 0f8f8 mvzl	r0,0 */
0x0000f8f9, 0x0fad0000, /* 0fad0000 //C 0f8f9 pop	r10 */
0x0000f8fa, 0x0f3d0000, /* 0f3d0000 //C 0f8fa pop	r3 */
0x0000f8fb, 0x0f2d0000, /* 0f2d0000 //C 0f8fb pop	r2 */
0x0000f8fc, 0x0f1d0000, /* 0f1d0000 //C 0f8fc pop	r1 */
0x0000f8fd, 0x0fed0000, /* 0fed0000 //C 0f8fd pop	lr */
0x0000f8fe, 0x00f00e00, /* 00f00e00 //C 0f8fe ret */
0x0000f8ff, 0x0ded0000, /* 0ded0000 //C 0f8ff push	lr */
0x0000f900, 0x0122fc58, /* 0122fc58 //C 0f900 mvzl	r2,words */
0x0000f901, 0x01020000, /* 01020000 //C 0f901 mvzl	r0,0 */
0x0000f902, 0x0e420001, /* 0e420001 //C 0f902 ld	r4,r2,1		; addr */
0x0000f903, 0x0e520002, /* 0e520002 //C 0f903 ld	r5,r2,2		; value */
0x0000f904, 0x024b0000, /* 024b0000 //C 0f904 sz 	r4 */
0x0000f905, 0x11f2f926, /* 11f2f926 //C 0f905 jz	m_ret */
0x0000f906, 0x00000400, /* 00000400 //C 0f906 mov	r0,r4 */
0x0000f907, 0x0400fb7d, /* 0400fb7d //C 0f907 call	htoi */
0x0000f908, 0x00400100, /* 00400100 //C 0f908 mov	r4,r1 */
0x0000f909, 0x31f2f90d, /* 31f2f90d //C 0f909 C1 jmp	m_addr_ok */
0x0000f90a, 0x0102f928, /* 0102f928 //C 0f90a mvzl	r0,m_err_addr */
0x0000f90b, 0x0400fbc3, /* 0400fbc3 //C 0f90b call	printsnl */
0x0000f90c, 0x01f2f926, /* 01f2f926 //C 0f90c jmp	m_ret */
0x0000f90d, 0x025b0000, /* 025b0000 //C 0f90d sz	r5 */
0x0000f90e, 0x11f2f91c, /* 11f2f91c //C 0f90e jz	m_read */
0x0000f90f, 0x0148ff61, /* 0148ff61 //C 0f90f cmp	r4,the_end */
0x0000f910, 0x91f2f914, /* 91f2f914 //C 0f910 HI jmp	m_addrv_ok */
0x0000f911, 0x0102f936, /* 0102f936 //C 0f911 mvzl	r0,m_err_addrv */
0x0000f912, 0x0400fbc3, /* 0400fbc3 //C 0f912 call	printsnl */
0x0000f913, 0x01f2f926, /* 01f2f926 //C 0f913 jmp	m_ret */
0x0000f914, 0x00000500, /* 00000500 //C 0f914 mov	r0,r5 */
0x0000f915, 0x0400fb7d, /* 0400fb7d //C 0f915 call	htoi */
0x0000f916, 0x00500100, /* 00500100 //C 0f916 mov	r5,r1 */
0x0000f917, 0x31f2f91b, /* 31f2f91b //C 0f917 C1 jmp	m_value_ok */
0x0000f918, 0x0102f948, /* 0102f948 //C 0f918 mvzl	r0,m_err_value */
0x0000f919, 0x0400fbc3, /* 0400fbc3 //C 0f919 call	printsnl */
0x0000f91a, 0x01f2f926, /* 01f2f926 //C 0f91a jmp	m_ret */
0x0000f91b, 0x0c540000, /* 0c540000 //C 0f91b st	r5,r4 */
0x0000f91c, 0x00000400, /* 00000400 //C 0f91c mov	r0,r4 */
0x0000f91d, 0x01120004, /* 01120004 //C 0f91d mvzl	r1,4 */
0x0000f91e, 0x0400fbcb, /* 0400fbcb //C 0f91e call	print_vhex */
0x0000f91f, 0x01020020, /* 01020020 //C 0f91f mvzl	r0,0x20 */
0x0000f920, 0x0400fba8, /* 0400fba8 //C 0f920 call	putchar */
0x0000f921, 0x0e040000, /* 0e040000 //C 0f921 ld	r0,r4 */
0x0000f922, 0x01120004, /* 01120004 //C 0f922 mvzl	r1,4 */
0x0000f923, 0x0400fbcb, /* 0400fbcb //C 0f923 call	print_vhex */
0x0000f924, 0x0102000a, /* 0102000a //C 0f924 mvzl	r0,LF */
0x0000f925, 0x0400fba8, /* 0400fba8 //C 0f925 call	putchar */
0x0000f926, 0x0fed0000, /* 0fed0000 //C 0f926 pop	lr */
0x0000f927, 0x00f00e00, /* 00f00e00 //C 0f927 ret */
0x0000f928, 0x00000041, /* 00000041 //C 0f928 db	65 */
0x0000f929, 0x00000064, /* 00000064 //C 0f929 db	100 */
0x0000f92a, 0x00000064, /* 00000064 //C 0f92a db	100 */
0x0000f92b, 0x00000072, /* 00000072 //C 0f92b db	114 */
0x0000f92c, 0x00000065, /* 00000065 //C 0f92c db	101 */
0x0000f92d, 0x00000073, /* 00000073 //C 0f92d db	115 */
0x0000f92e, 0x00000073, /* 00000073 //C 0f92e db	115 */
0x0000f92f, 0x00000020, /* 00000020 //C 0f92f db	32 */
0x0000f930, 0x00000065, /* 00000065 //C 0f930 db	101 */
0x0000f931, 0x00000072, /* 00000072 //C 0f931 db	114 */
0x0000f932, 0x00000072, /* 00000072 //C 0f932 db	114 */
0x0000f933, 0x0000006f, /* 0000006f //C 0f933 db	111 */
0x0000f934, 0x00000072, /* 00000072 //C 0f934 db	114 */
0x0000f935, 0x00000000, /* 00000000 //C 0f935 db */
0x0000f936, 0x0000004d, /* 0000004d //C 0f936 db	77 */
0x0000f937, 0x0000006f, /* 0000006f //C 0f937 db	111 */
0x0000f938, 0x0000006e, /* 0000006e //C 0f938 db	110 */
0x0000f939, 0x00000069, /* 00000069 //C 0f939 db	105 */
0x0000f93a, 0x00000074, /* 00000074 //C 0f93a db	116 */
0x0000f93b, 0x0000006f, /* 0000006f //C 0f93b db	111 */
0x0000f93c, 0x00000072, /* 00000072 //C 0f93c db	114 */
0x0000f93d, 0x00000027, /* 00000027 //C 0f93d db	39 */
0x0000f93e, 0x00000073, /* 00000073 //C 0f93e db	115 */
0x0000f93f, 0x00000020, /* 00000020 //C 0f93f db	32 */
0x0000f940, 0x00000061, /* 00000061 //C 0f940 db	97 */
0x0000f941, 0x00000064, /* 00000064 //C 0f941 db	100 */
0x0000f942, 0x00000064, /* 00000064 //C 0f942 db	100 */
0x0000f943, 0x00000072, /* 00000072 //C 0f943 db	114 */
0x0000f944, 0x00000065, /* 00000065 //C 0f944 db	101 */
0x0000f945, 0x00000073, /* 00000073 //C 0f945 db	115 */
0x0000f946, 0x00000073, /* 00000073 //C 0f946 db	115 */
0x0000f947, 0x00000000, /* 00000000 //C 0f947 db */
0x0000f948, 0x00000056, /* 00000056 //C 0f948 db	86 */
0x0000f949, 0x00000061, /* 00000061 //C 0f949 db	97 */
0x0000f94a, 0x0000006c, /* 0000006c //C 0f94a db	108 */
0x0000f94b, 0x00000075, /* 00000075 //C 0f94b db	117 */
0x0000f94c, 0x00000065, /* 00000065 //C 0f94c db	101 */
0x0000f94d, 0x00000020, /* 00000020 //C 0f94d db	32 */
0x0000f94e, 0x00000065, /* 00000065 //C 0f94e db	101 */
0x0000f94f, 0x00000072, /* 00000072 //C 0f94f db	114 */
0x0000f950, 0x00000072, /* 00000072 //C 0f950 db	114 */
0x0000f951, 0x0000006f, /* 0000006f //C 0f951 db	111 */
0x0000f952, 0x00000072, /* 00000072 //C 0f952 db	114 */
0x0000f953, 0x00000000, /* 00000000 //C 0f953 db */
0x0000f954, 0x0ded0000, /* 0ded0000 //C 0f954 push	lr */
0x0000f955, 0x0122fc58, /* 0122fc58 //C 0f955 mvzl	r2,words */
0x0000f956, 0x0e020001, /* 0e020001 //C 0f956 ld	r0,r2,1		; start address */
0x0000f957, 0x0400fb7d, /* 0400fb7d //C 0f957 call	htoi */
0x0000f958, 0x00300100, /* 00300100 //C 0f958 mov	r3,r1 */
0x0000f959, 0x0e020002, /* 0e020002 //C 0f959 ld	r0,r2,2		; end address */
0x0000f95a, 0x020b0000, /* 020b0000 //C 0f95a sz	r0 */
0x0000f95b, 0x21f2f95f, /* 21f2f95f //C 0f95b jnz	d_end_ok */
0x0000f95c, 0x00400300, /* 00400300 //C 0f95c mov	r4,r3 */
0x0000f95d, 0x01440010, /* 01440010 //C 0f95d add	r4,0x10 */
0x0000f95e, 0x01f2f961, /* 01f2f961 //C 0f95e jmp	d_chk_end */
0x0000f95f, 0x0400fb7d, /* 0400fb7d //C 0f95f call	htoi */
0x0000f960, 0x00400100, /* 00400100 //C 0f960 mov	r4,r1 */
0x0000f961, 0x00380400, /* 00380400 //C 0f961 cmp	r3,r4		; check if start>end */
0x0000f962, 0x91f2f971, /* 91f2f971 //C 0f962 HI jmp d_bad */
0x0000f963, 0x00000300, /* 00000300 //C 0f963 mov	r0,r3		; print address */
0x0000f964, 0x01120004, /* 01120004 //C 0f964 mvzl	r1,4 */
0x0000f965, 0x0400fbcb, /* 0400fbcb //C 0f965 call	print_vhex */
0x0000f966, 0x01020020, /* 01020020 //C 0f966 mvzl	r0,0x20		; print one space */
0x0000f967, 0x0400fba8, /* 0400fba8 //C 0f967 call	putchar */
0x0000f968, 0x0e030000, /* 0e030000 //C 0f968 ld	r0,r3		; load data */
0x0000f969, 0x01120004, /* 01120004 //C 0f969 mvzl	r1,4		; print data */
0x0000f96a, 0x0400fbcb, /* 0400fbcb //C 0f96a call	print_vhex */
0x0000f96b, 0x0102000a, /* 0102000a //C 0f96b mvzl	r0,LF		; print new line */
0x0000f96c, 0x0400fba8, /* 0400fba8 //C 0f96c call	putchar */
0x0000f96d, 0x00380400, /* 00380400 //C 0f96d cmp	r3,r4 */
0x0000f96e, 0x11f2f973, /* 11f2f973 //C 0f96e jz	d_ret */
0x0000f96f, 0x01340001, /* 01340001 //C 0f96f add	r3,1 */
0x0000f970, 0x01f2f963, /* 01f2f963 //C 0f970 jmp	d_cyc */
0x0000f971, 0x0102f975, /* 0102f975 //C 0f971 mvzl	r0,d_err_bad */
0x0000f972, 0x0400fbc3, /* 0400fbc3 //C 0f972 call	printsnl */
0x0000f973, 0x0fed0000, /* 0fed0000 //C 0f973 pop	lr */
0x0000f974, 0x00f00e00, /* 00f00e00 //C 0f974 ret */
0x0000f975, 0x00000057, /* 00000057 //C 0f975 db	87 */
0x0000f976, 0x00000072, /* 00000072 //C 0f976 db	114 */
0x0000f977, 0x0000006f, /* 0000006f //C 0f977 db	111 */
0x0000f978, 0x0000006e, /* 0000006e //C 0f978 db	110 */
0x0000f979, 0x00000067, /* 00000067 //C 0f979 db	103 */
0x0000f97a, 0x00000020, /* 00000020 //C 0f97a db	32 */
0x0000f97b, 0x00000065, /* 00000065 //C 0f97b db	101 */
0x0000f97c, 0x0000006e, /* 0000006e //C 0f97c db	110 */
0x0000f97d, 0x00000064, /* 00000064 //C 0f97d db	100 */
0x0000f97e, 0x00000020, /* 00000020 //C 0f97e db	32 */
0x0000f97f, 0x00000061, /* 00000061 //C 0f97f db	97 */
0x0000f980, 0x00000064, /* 00000064 //C 0f980 db	100 */
0x0000f981, 0x00000064, /* 00000064 //C 0f981 db	100 */
0x0000f982, 0x00000072, /* 00000072 //C 0f982 db	114 */
0x0000f983, 0x00000065, /* 00000065 //C 0f983 db	101 */
0x0000f984, 0x00000073, /* 00000073 //C 0f984 db	115 */
0x0000f985, 0x00000073, /* 00000073 //C 0f985 db	115 */
0x0000f986, 0x00000000, /* 00000000 //C 0f986 db */
0x0000f987, 0x00f00e00, /* 00f00e00 //C 0f987 ret */
0x0000f988, 0x0ded0000, /* 0ded0000 //C 0f988 push	lr */
0x0000f989, 0x01a20000, /* 01a20000 //C 0f989 mvzl	r10,0		; state (nr of words) */
0x0000f98a, 0x01820000, /* 01820000 //C 0f98a mvzl	r8,0		; value */
0x0000f98b, 0x0162003f, /* 0162003f //C 0f98b mvzl	r6,'?'		; Record type */
0x0000f98c, 0x0400fb9f, /* 0400fb9f //C 0f98c call	check_uart */
0x0000f98d, 0x41f2f98c, /* 41f2f98c //C 0f98d C0 jmp	l_cyc */
0x0000f98e, 0x0400fba6, /* 0400fba6 //C 0f98e call	read */
0x0000f98f, 0x00b00000, /* 00b00000 //C 0f98f mov	r11,r0		; Copy of char in R11 */
0x0000f990, 0x0108000a, /* 0108000a //C 0f990 cmp	r0,10		; check EOL chars */
0x0000f991, 0x11f2f9db, /* 11f2f9db //C 0f991 jz	l_eol */
0x0000f992, 0x0108000d, /* 0108000d //C 0f992 cmp	r0,13 */
0x0000f993, 0x11f2f9db, /* 11f2f9db //C 0f993 jz	l_eol */
0x0000f994, 0x01a80000, /* 01a80000 //C 0f994 cmp	r10,0 */
0x0000f995, 0x21f2f9a5, /* 21f2f9a5 //C 0f995 jnz	l_no0 */
0x0000f996, 0x0400fb49, /* 0400fb49 //C 0f996 call	hexchar2value */
0x0000f997, 0x41f2f99f, /* 41f2f99f //C 0f997 C0 jmp	l_eof_0 */
0x0000f998, 0x02880000, /* 02880000 //C 0f998 shl	r8		; shift val(char) into value */
0x0000f999, 0x02880000, /* 02880000 //C 0f999 shl	r8 */
0x0000f99a, 0x02880000, /* 02880000 //C 0f99a shl	r8 */
0x0000f99b, 0x02880000, /* 02880000 //C 0f99b shl	r8 */
0x0000f99c, 0x010b000f, /* 010b000f //C 0f99c btst	r0,0xf */
0x0000f99d, 0x008d0000, /* 008d0000 //C 0f99d or	r8,r0 */
0x0000f99e, 0x01f2f98c, /* 01f2f98c //C 0f99e jmp	l_cyc */
0x0000f99f, 0x01a20001, /* 01a20001 //C 0f99f mvzl	r10,1		; state0 -> state1 */
0x0000f9a0, 0x0162003f, /* 0162003f //C 0f9a0 mvzl	r6,'?'		; No //C yet */
0x0000f9a1, 0x01b8002f, /* 01b8002f //C 0f9a1 cmp	r11,'/'		; is it start of // */
0x0000f9a2, 0x11720001, /* 11720001 //C 0f9a2 z1 mvzl	r7,1		; Yes, first / found */
0x0000f9a3, 0x21720000, /* 21720000 //C 0f9a3 z0 mvzl	r7,0		; No '/' yet */
0x0000f9a4, 0x01f2f98c, /* 01f2f98c //C 0f9a4 jmp	l_cyc */
0x0000f9a5, 0x01a80001, /* 01a80001 //C 0f9a5 cmp	r10,1 */
0x0000f9a6, 0x21f2f9be, /* 21f2f9be //C 0f9a6 jnz	l_no1 */
0x0000f9a7, 0x027b0000, /* 027b0000 //C 0f9a7 sz	r7 */
0x0000f9a8, 0x21f2f9ad, /* 21f2f9ad //C 0f9a8 jnz	l_s1_2nd */
0x0000f9a9, 0x0108002f, /* 0108002f //C 0f9a9 cmp	r0,'/' */
0x0000f9aa, 0x21f2f98c, /* 21f2f98c //C 0f9aa jnz	l_cyc */
0x0000f9ab, 0x01720001, /* 01720001 //C 0f9ab mvzl	r7,1 */
0x0000f9ac, 0x01f2f98c, /* 01f2f98c //C 0f9ac jmp	l_cyc */
0x0000f9ad, 0x0108002f, /* 0108002f //C 0f9ad cmp	r0,'/' */
0x0000f9ae, 0x11f2f98c, /* 11f2f98c //C 0f9ae jz	l_cyc */
0x0000f9af, 0x01080043, /* 01080043 //C 0f9af cmp	r0,'C' */
0x0000f9b0, 0x21f2f9b6, /* 21f2f9b6 //C 0f9b0 jnz	l_s1_noC */
0x0000f9b1, 0x01620043, /* 01620043 //C 0f9b1 mvzl	r6,'C' */
0x0000f9b2, 0x01a20002, /* 01a20002 //C 0f9b2 mvzl	r10,2 */
0x0000f9b3, 0x01920000, /* 01920000 //C 0f9b3 mvzl	r9,0		; address= 0 */
0x0000f9b4, 0x01520000, /* 01520000 //C 0f9b4 mvzl	r5,0		; where we are in word: before */
0x0000f9b5, 0x01f2f98c, /* 01f2f98c //C 0f9b5 jmp	l_cyc */
0x0000f9b6, 0x01080045, /* 01080045 //C 0f9b6 cmp	r0,'E' */
0x0000f9b7, 0x21f2f9bb, /* 21f2f9bb //C 0f9b7 jnz	l_s1_noE */
0x0000f9b8, 0x01620045, /* 01620045 //C 0f9b8 mvzl	r6,'E' */
0x0000f9b9, 0x01a20003, /* 01a20003 //C 0f9b9 mvzl	r10,3 */
0x0000f9ba, 0x01f2f98c, /* 01f2f98c //C 0f9ba jmp	l_cyc */
0x0000f9bb, 0x0400fba8, /* 0400fba8 //C 0f9bb call	putchar		; print record type */
0x0000f9bc, 0x01a2000f, /* 01a2000f //C 0f9bc mvzl	r10,0xf		; special state: skip everything */
0x0000f9bd, 0x01f2f98c, /* 01f2f98c //C 0f9bd jmp	l_cyc */
0x0000f9be, 0x01a80002, /* 01a80002 //C 0f9be cmp	r10,2 */
0x0000f9bf, 0x21f2f9d3, /* 21f2f9d3 //C 0f9bf jnz	l_no2 */
0x0000f9c0, 0x01580000, /* 01580000 //C 0f9c0 cmp	r5,0 */
0x0000f9c1, 0x21f2f9cc, /* 21f2f9cc //C 0f9c1 jnz	l_s2_no0 */
0x0000f9c2, 0x0400fb49, /* 0400fb49 //C 0f9c2 call	hexchar2value */
0x0000f9c3, 0x41f2f98c, /* 41f2f98c //C 0f9c3 C0 jmp	l_cyc */
0x0000f9c4, 0x01520001, /* 01520001 //C 0f9c4 mvzl	r5,1 */
0x0000f9c5, 0x02980000, /* 02980000 //C 0f9c5 shl	r9 */
0x0000f9c6, 0x02980000, /* 02980000 //C 0f9c6 shl	r9 */
0x0000f9c7, 0x02980000, /* 02980000 //C 0f9c7 shl	r9 */
0x0000f9c8, 0x02980000, /* 02980000 //C 0f9c8 shl	r9 */
0x0000f9c9, 0x010b000f, /* 010b000f //C 0f9c9 btst	r0,0xf */
0x0000f9ca, 0x009d0000, /* 009d0000 //C 0f9ca or	r9,r0 */
0x0000f9cb, 0x01f2f98c, /* 01f2f98c //C 0f9cb jmp	l_cyc */
0x0000f9cc, 0x01580001, /* 01580001 //C 0f9cc cmp	r5,1 */
0x0000f9cd, 0x21f2f9d2, /* 21f2f9d2 //C 0f9cd jnz	l_s2_no1 */
0x0000f9ce, 0x0400fb49, /* 0400fb49 //C 0f9ce call	hexchar2value */
0x0000f9cf, 0x31f2f9c5, /* 31f2f9c5 //C 0f9cf C1 jmp	l_s2_got */
0x0000f9d0, 0x01520002, /* 01520002 //C 0f9d0 mvzl	r5,2 */
0x0000f9d1, 0x01f2f98c, /* 01f2f98c //C 0f9d1 jmp	l_cyc */
0x0000f9d2, 0x01f2f98c, /* 01f2f98c //C 0f9d2 jmp	l_cyc */
0x0000f9d3, 0x01a80003, /* 01a80003 //C 0f9d3 cmp	r10,3 */
0x0000f9d4, 0x21f2f9d6, /* 21f2f9d6 //C 0f9d4 jnz	l_no3 */
0x0000f9d5, 0x01f2f98c, /* 01f2f98c //C 0f9d5 jmp	l_cyc		; do nothing, just wait EOL */
0x0000f9d6, 0x01a8000f, /* 01a8000f //C 0f9d6 cmp	r10,0xf */
0x0000f9d7, 0x01f2f9d9, /* 01f2f9d9 //C 0f9d7 jmp	l_nof */
0x0000f9d8, 0x01f2f98c, /* 01f2f98c //C 0f9d8 jmp	l_cyc		; just skip */
0x0000f9d9, 0x01f2f98c, /* 01f2f98c //C 0f9d9 jmp	l_cyc */
0x0000f9da, 0x01f2f9f1, /* 01f2f9f1 //C 0f9da jmp	l_ret */
0x0000f9db, 0x01a80000, /* 01a80000 //C 0f9db cmp	r10,0		; in state0 */
0x0000f9dc, 0x11f2f9eb, /* 11f2f9eb //C 0f9dc jz	l_back_to_0	; just restart */
0x0000f9dd, 0x01a80001, /* 01a80001 //C 0f9dd cmp	r10,1		; in state1 */
0x0000f9de, 0x11f2f9eb, /* 11f2f9eb //C 0f9de jz	l_back_to_0 	;l_cyc ;l_bad ; garbage */
0x0000f9df, 0x01a80002, /* 01a80002 //C 0f9df cmp	r10,2		; in state2 */
0x0000f9e0, 0x11f2f9e7, /* 11f2f9e7 //C 0f9e0 jz	l_proc		; process record */
0x0000f9e1, 0x01a80003, /* 01a80003 //C 0f9e1 cmp	r10,3		; in state3 */
0x0000f9e2, 0x11f2f9f1, /* 11f2f9f1 //C 0f9e2 jz	l_ret		; eol in end record: finish */
0x0000f9e3, 0x01a8000f, /* 01a8000f //C 0f9e3 cmp	r10,0xf		; in state skip */
0x0000f9e4, 0x11f2f9eb, /* 11f2f9eb //C 0f9e4 jz	l_back_to_0	; reset state for new line */
0x0000f9e5, 0x01f2f98c, /* 01f2f98c //C 0f9e5 jmp	l_cyc */
0x0000f9e6, 0x01f2f9f1, /* 01f2f9f1 //C 0f9e6 jmp	l_ret */
0x0000f9e7, 0x01680043, /* 01680043 //C 0f9e7 cmp	r6,'C'		; is it a C record? */
0x0000f9e8, 0x1c890000, /* 1c890000 //C 0f9e8 z st	r8,r9		; store */
0x0000f9e9, 0x00000600, /* 00000600 //C 0f9e9 mov	r0,r6		; echo record type */
0x0000f9ea, 0x0400fba8, /* 0400fba8 //C 0f9ea call	putchar */
0x0000f9eb, 0x0102002e, /* 0102002e //C 0f9eb mvzl	r0,'.' */
0x0000f9ec, 0x0400fba8, /* 0400fba8 //C 0f9ec call	putchar */
0x0000f9ed, 0x01a20000, /* 01a20000 //C 0f9ed mvzl	r10,0 */
0x0000f9ee, 0x01820000, /* 01820000 //C 0f9ee mvzl	r8,0 */
0x0000f9ef, 0x0162003f, /* 0162003f //C 0f9ef mvzl	r6,'?' */
0x0000f9f0, 0x01f2f98c, /* 01f2f98c //C 0f9f0 jmp	l_cyc */
0x0000f9f1, 0x0102000a, /* 0102000a //C 0f9f1 mvzl	r0,LF */
0x0000f9f2, 0x0400fba8, /* 0400fba8 //C 0f9f2 call	putchar */
0x0000f9f3, 0x0fed0000, /* 0fed0000 //C 0f9f3 pop	lr */
0x0000f9f4, 0x00f00e00, /* 00f00e00 //C 0f9f4 ret */
0x0000f9f5, 0x0ded0000, /* 0ded0000 //C 0f9f5 push	lr */
0x0000f9f6, 0x0122fc58, /* 0122fc58 //C 0f9f6 mvzl	r2,words */
0x0000f9f7, 0x0e020001, /* 0e020001 //C 0f9f7 ld	r0,r2,1		; address */
0x0000f9f8, 0x020b0000, /* 020b0000 //C 0f9f8 sz	r0 */
0x0000f9f9, 0x11f2fa1c, /* 11f2fa1c //C 0f9f9 jz	g_no_addr */
0x0000f9fa, 0x0400fb7d, /* 0400fb7d //C 0f9fa call	htoi */
0x0000f9fb, 0x00b00100, /* 00b00100 //C 0f9fb mov	r11,r1 */
0x0000f9fc, 0x0102fa30, /* 0102fa30 //C 0f9fc mvzl	r0,d_msg_run */
0x0000f9fd, 0x0400fbaf, /* 0400fbaf //C 0f9fd call	prints */
0x0000f9fe, 0x00000b00, /* 00000b00 //C 0f9fe mov	r0,r11 */
0x0000f9ff, 0x01120004, /* 01120004 //C 0f9ff mvzl	r1,4 */
0x0000fa00, 0x0400fbcb, /* 0400fbcb //C 0fa00 call	print_vhex */
0x0000fa01, 0x0102000a, /* 0102000a //C 0fa01 mvzl	r0,LF */
0x0000fa02, 0x0400fba8, /* 0400fba8 //C 0fa02 call	putchar */
0x0000fa03, 0x06b0fc6f, /* 06b0fc6f //C 0fa03 st	r11,reg15 */
0x0000fa04, 0x0122ff43, /* 0122ff43 //C 0fa04 mvzl	r2,UART_TSTAT */
0x0000fa05, 0x0e920000, /* 0e920000 //C 0fa05 ld	r9,r2 */
0x0000fa06, 0x019c0001, /* 019c0001 //C 0fa06 test	r9,1 */
0x0000fa07, 0x11f2fa05, /* 11f2fa05 //C 0fa07 jz	g_wait_tc */
0x0000fa08, 0x07000000, /* 07000000 //C 0fa08 ld	r0,0 */
0x0000fa09, 0x0600fc5e, /* 0600fc5e //C 0fa09 st	r0,called */
0x0000fa0a, 0x0700fc70, /* 0700fc70 //C 0fa0a ld	r0,regf */
0x0000fa0b, 0x020f0000, /* 020f0000 //C 0fa0b setf	r0 */
0x0000fa0c, 0x0700fc60, /* 0700fc60 //C 0fa0c ld	r0,reg0 */
0x0000fa0d, 0x0710fc61, /* 0710fc61 //C 0fa0d ld	r1,reg1 */
0x0000fa0e, 0x0720fc62, /* 0720fc62 //C 0fa0e ld	r2,reg2 */
0x0000fa0f, 0x0730fc63, /* 0730fc63 //C 0fa0f ld	r3,reg3 */
0x0000fa10, 0x0740fc64, /* 0740fc64 //C 0fa10 ld	r4,reg4 */
0x0000fa11, 0x0750fc65, /* 0750fc65 //C 0fa11 ld	r5,reg5 */
0x0000fa12, 0x0760fc66, /* 0760fc66 //C 0fa12 ld	r6,reg6 */
0x0000fa13, 0x0770fc67, /* 0770fc67 //C 0fa13 ld	r7,reg7 */
0x0000fa14, 0x0780fc68, /* 0780fc68 //C 0fa14 ld	r8,reg8 */
0x0000fa15, 0x0790fc69, /* 0790fc69 //C 0fa15 ld	r9,reg9 */
0x0000fa16, 0x07a0fc6a, /* 07a0fc6a //C 0fa16 ld	r10,reg10 */
0x0000fa17, 0x07b0fc6b, /* 07b0fc6b //C 0fa17 ld	r11,reg11 */
0x0000fa18, 0x07c0fc6c, /* 07c0fc6c //C 0fa18 ld	r12,reg12 */
0x0000fa19, 0x07d0fc6d, /* 07d0fc6d //C 0fa19 ld	r13,reg13 */
0x0000fa1a, 0x07e0fc6e, /* 07e0fc6e //C 0fa1a ld	r14,reg14 */
0x0000fa1b, 0x07f0fc6f, /* 07f0fc6f //C 0fa1b ld	r15,reg15 */
0x0000fa1c, 0x0700fc5e, /* 0700fc5e //C 0fa1c ld	r0,called */
0x0000fa1d, 0x020b0000, /* 020b0000 //C 0fa1d sz	r0 */
0x0000fa1e, 0x11f2fa21, /* 11f2fa21 //C 0fa1e jz	g_err */
0x0000fa1f, 0x07b0fc6f, /* 07b0fc6f //C 0fa1f ld	r11,reg15 */
0x0000fa20, 0x01f2f9fc, /* 01f2f9fc //C 0fa20 jmp	g_go11 */
0x0000fa21, 0x0102fa25, /* 0102fa25 //C 0fa21 mvzl	r0,g_err_addr */
0x0000fa22, 0x0400fbc3, /* 0400fbc3 //C 0fa22 call	printsnl */
0x0000fa23, 0x0fed0000, /* 0fed0000 //C 0fa23 pop	lr */
0x0000fa24, 0x00f00e00, /* 00f00e00 //C 0fa24 ret */
0x0000fa25, 0x0000004e, /* 0000004e //C 0fa25 db	78 */
0x0000fa26, 0x0000006f, /* 0000006f //C 0fa26 db	111 */
0x0000fa27, 0x00000020, /* 00000020 //C 0fa27 db	32 */
0x0000fa28, 0x00000061, /* 00000061 //C 0fa28 db	97 */
0x0000fa29, 0x00000064, /* 00000064 //C 0fa29 db	100 */
0x0000fa2a, 0x00000064, /* 00000064 //C 0fa2a db	100 */
0x0000fa2b, 0x00000072, /* 00000072 //C 0fa2b db	114 */
0x0000fa2c, 0x00000065, /* 00000065 //C 0fa2c db	101 */
0x0000fa2d, 0x00000073, /* 00000073 //C 0fa2d db	115 */
0x0000fa2e, 0x00000073, /* 00000073 //C 0fa2e db	115 */
0x0000fa2f, 0x00000000, /* 00000000 //C 0fa2f db */
0x0000fa30, 0x00000052, /* 00000052 //C 0fa30 db	82 */
0x0000fa31, 0x00000075, /* 00000075 //C 0fa31 db	117 */
0x0000fa32, 0x0000006e, /* 0000006e //C 0fa32 db	110 */
0x0000fa33, 0x00000020, /* 00000020 //C 0fa33 db	32 */
0x0000fa34, 0x00000000, /* 00000000 //C 0fa34 db */
0x0000fa35, 0x0ded0000, /* 0ded0000 //C 0fa35 push	lr */
0x0000fa36, 0x0122fce2, /* 0122fce2 //C 0fa36 mvzl	r2,helps */
0x0000fa37, 0x01320000, /* 01320000 //C 0fa37 mvzl	r3,0 */
0x0000fa38, 0x0b038200, /* 0b038200 //C 0fa38 ld	r0,r3+,r2	; pick a char */
0x0000fa39, 0x020b0000, /* 020b0000 //C 0fa39 sz	r0		; is it eos? */
0x0000fa3a, 0x21f2fa3e, /* 21f2fa3e //C 0fa3a jnz	h_print */
0x0000fa3b, 0x0b038200, /* 0b038200 //C 0fa3b ld	r0,r3+,r2	; get first char of next string */
0x0000fa3c, 0x020b0000, /* 020b0000 //C 0fa3c sz	r0 */
0x0000fa3d, 0x11f2fa40, /* 11f2fa40 //C 0fa3d jz	h_eof */
0x0000fa3e, 0x0400fba8, /* 0400fba8 //C 0fa3e call	putchar */
0x0000fa3f, 0x01f2fa38, /* 01f2fa38 //C 0fa3f jmp	h_cyc */
0x0000fa40, 0x0fed0000, /* 0fed0000 //C 0fa40 pop	lr */
0x0000fa41, 0x00f00e00, /* 00f00e00 //C 0fa41 ret */
0x0000fa42, 0x0ded0000, /* 0ded0000 //C 0fa42 push	lr */
0x0000fa43, 0x0d1d0000, /* 0d1d0000 //C 0fa43 push	r1 */
0x0000fa44, 0x00100000, /* 00100000 //C 0fa44 mov	r1,r0 */
0x0000fa45, 0x01180010, /* 01180010 //C 0fa45 cmp	r1,16		; go out if nr>16 */
0x0000fa46, 0x91f2fa7a, /* 91f2fa7a //C 0fa46 HI jmp	prn_ret */
0x0000fa47, 0x0118000f, /* 0118000f //C 0fa47 cmp	r1,15		; nr=Flag? */
0x0000fa48, 0xa1f2fa4f, /* a1f2fa4f //C 0fa48 LS jmp	prn_015 */
0x0000fa49, 0x01020046, /* 01020046 //C 0fa49 mvzl	r0,'F' */
0x0000fa4a, 0x0400fba8, /* 0400fba8 //C 0fa4a call	putchar */
0x0000fa4b, 0x01020020, /* 01020020 //C 0fa4b mvzl	r0,32 */
0x0000fa4c, 0x0400fba8, /* 0400fba8 //C 0fa4c call	putchar */
0x0000fa4d, 0x0400fba8, /* 0400fba8 //C 0fa4d call	putchar */
0x0000fa4e, 0x01f2fa7a, /* 01f2fa7a //C 0fa4e jmp	prn_ret */
0x0000fa4f, 0x0118000f, /* 0118000f //C 0fa4f cmp	r1,15 */
0x0000fa50, 0x21f2fa58, /* 21f2fa58 //C 0fa50 jnz	prn_no15 */
0x0000fa51, 0x0102fa54, /* 0102fa54 //C 0fa51 mvzl	r0,prn_PC */
0x0000fa52, 0x0400fbaf, /* 0400fbaf //C 0fa52 call	prints */
0x0000fa53, 0x01f2fa7a, /* 01f2fa7a //C 0fa53 jmp	prn_ret */
0x0000fa54, 0x00000050, /* 00000050 //C 0fa54 db	80 */
0x0000fa55, 0x00000043, /* 00000043 //C 0fa55 db	67 */
0x0000fa56, 0x00000020, /* 00000020 //C 0fa56 db	32 */
0x0000fa57, 0x00000000, /* 00000000 //C 0fa57 db */
0x0000fa58, 0x0118000e, /* 0118000e //C 0fa58 cmp	r1,14 */
0x0000fa59, 0x21f2fa61, /* 21f2fa61 //C 0fa59 jnz	prn_no14 */
0x0000fa5a, 0x0102fa5d, /* 0102fa5d //C 0fa5a mvzl	r0,prn_LR */
0x0000fa5b, 0x0400fbaf, /* 0400fbaf //C 0fa5b call	prints */
0x0000fa5c, 0x01f2fa7a, /* 01f2fa7a //C 0fa5c jmp	prn_ret */
0x0000fa5d, 0x0000004c, /* 0000004c //C 0fa5d db	76 */
0x0000fa5e, 0x00000052, /* 00000052 //C 0fa5e db	82 */
0x0000fa5f, 0x00000020, /* 00000020 //C 0fa5f db	32 */
0x0000fa60, 0x00000000, /* 00000000 //C 0fa60 db */
0x0000fa61, 0x0118000d, /* 0118000d //C 0fa61 cmp	r1,13 */
0x0000fa62, 0x21f2fa6a, /* 21f2fa6a //C 0fa62 jnz	prn_no13 */
0x0000fa63, 0x0102fa66, /* 0102fa66 //C 0fa63 mvzl	r0,prn_SP */
0x0000fa64, 0x0400fbaf, /* 0400fbaf //C 0fa64 call	prints */
0x0000fa65, 0x01f2fa7a, /* 01f2fa7a //C 0fa65 jmp	prn_ret */
0x0000fa66, 0x00000053, /* 00000053 //C 0fa66 db	83 */
0x0000fa67, 0x00000050, /* 00000050 //C 0fa67 db	80 */
0x0000fa68, 0x00000020, /* 00000020 //C 0fa68 db	32 */
0x0000fa69, 0x00000000, /* 00000000 //C 0fa69 db */
0x0000fa6a, 0x01020052, /* 01020052 //C 0fa6a mvzl	r0,'R' */
0x0000fa6b, 0x0400fba8, /* 0400fba8 //C 0fa6b call	putchar */
0x0000fa6c, 0x01180009, /* 01180009 //C 0fa6c cmp	r1,9 */
0x0000fa6d, 0x91f2fa74, /* 91f2fa74 //C 0fa6d HI jmp	prn_1015 */
0x0000fa6e, 0x00000100, /* 00000100 //C 0fa6e mov	r0,r1 */
0x0000fa6f, 0x01040030, /* 01040030 //C 0fa6f add	r0,'0' */
0x0000fa70, 0x0400fba8, /* 0400fba8 //C 0fa70 call	putchar */
0x0000fa71, 0x01020020, /* 01020020 //C 0fa71 mvzl	r0,32 */
0x0000fa72, 0x0400fba8, /* 0400fba8 //C 0fa72 call	putchar */
0x0000fa73, 0x01f2fa7a, /* 01f2fa7a //C 0fa73 jmp	prn_ret */
0x0000fa74, 0x01020031, /* 01020031 //C 0fa74 mvzl	r0,'1' */
0x0000fa75, 0x0400fba8, /* 0400fba8 //C 0fa75 call	putchar */
0x0000fa76, 0x00000100, /* 00000100 //C 0fa76 mov	r0,r1 */
0x0000fa77, 0x0106000a, /* 0106000a //C 0fa77 sub	r0,10 */
0x0000fa78, 0x01040030, /* 01040030 //C 0fa78 add	r0,'0' */
0x0000fa79, 0x0400fba8, /* 0400fba8 //C 0fa79 call	putchar */
0x0000fa7a, 0x0f1d0000, /* 0f1d0000 //C 0fa7a pop	r1 */
0x0000fa7b, 0x0fed0000, /* 0fed0000 //C 0fa7b pop	lr */
0x0000fa7c, 0x00f00e00, /* 00f00e00 //C 0fa7c ret */
0x0000fa7d, 0x0ded0000, /* 0ded0000 //C 0fa7d push	lr */
0x0000fa7e, 0x0d1d0000, /* 0d1d0000 //C 0fa7e push	r1 */
0x0000fa7f, 0x01080010, /* 01080010 //C 0fa7f cmp	r0,16 */
0x0000fa80, 0x91f2fa85, /* 91f2fa85 //C 0fa80 HI jmp	prv_ret */
0x0000fa81, 0x0112fc60, /* 0112fc60 //C 0fa81 mvzl	r1,reg0 */
0x0000fa82, 0x0a010000, /* 0a010000 //C 0fa82 ld	r0,r1,r0 */
0x0000fa83, 0x01120004, /* 01120004 //C 0fa83 mvzl	r1,4 */
0x0000fa84, 0x0400fbcb, /* 0400fbcb //C 0fa84 call	print_vhex */
0x0000fa85, 0x0f1d0000, /* 0f1d0000 //C 0fa85 pop	r1 */
0x0000fa86, 0x0fed0000, /* 0fed0000 //C 0fa86 pop	lr */
0x0000fa87, 0x00f00e00, /* 00f00e00 //C 0fa87 ret */
0x0000fa88, 0x0ded0000, /* 0ded0000 //C 0fa88 push	lr */
0x0000fa89, 0x0dad0000, /* 0dad0000 //C 0fa89 push	r10 */
0x0000fa8a, 0x0d0d0000, /* 0d0d0000 //C 0fa8a push	r0 */
0x0000fa8b, 0x00000a00, /* 00000a00 //C 0fa8b mov	r0,r10 */
0x0000fa8c, 0x0400fa42, /* 0400fa42 //C 0fa8c call	print_reg_name */
0x0000fa8d, 0x01020020, /* 01020020 //C 0fa8d mvzl	r0,32 */
0x0000fa8e, 0x0400fba8, /* 0400fba8 //C 0fa8e call	putchar */
0x0000fa8f, 0x00000a00, /* 00000a00 //C 0fa8f mov	r0,r10 */
0x0000fa90, 0x0400fa7d, /* 0400fa7d //C 0fa90 call	print_reg_value */
0x0000fa91, 0x0102000a, /* 0102000a //C 0fa91 mvzl	r0,LF */
0x0000fa92, 0x0400fba8, /* 0400fba8 //C 0fa92 call	putchar */
0x0000fa93, 0x0f0d0000, /* 0f0d0000 //C 0fa93 pop	r0 */
0x0000fa94, 0x0fad0000, /* 0fad0000 //C 0fa94 pop	r10 */
0x0000fa95, 0x0fed0000, /* 0fed0000 //C 0fa95 pop	lr */
0x0000fa96, 0x00f00e00, /* 00f00e00 //C 0fa96 ret */
0x0000fa97, 0x0ded0000, /* 0ded0000 //C 0fa97 push	lr */
0x0000fa98, 0x0400fba8, /* 0400fba8 //C 0fa98 call	putchar */
0x0000fa99, 0x001c0200, /* 001c0200 //C 0fa99 test	r1,r2 */
0x0000fa9a, 0x21020031, /* 21020031 //C 0fa9a NZ mvzl r0,'1' */
0x0000fa9b, 0x11020030, /* 11020030 //C 0fa9b Z  mvzl r0,'0' */
0x0000fa9c, 0x0400fba8, /* 0400fba8 //C 0fa9c call	putchar */
0x0000fa9d, 0x00000300, /* 00000300 //C 0fa9d mov	r0,r3 */
0x0000fa9e, 0x0400fba8, /* 0400fba8 //C 0fa9e call	putchar */
0x0000fa9f, 0x0fed0000, /* 0fed0000 //C 0fa9f pop	lr */
0x0000faa0, 0x00f00e00, /* 00f00e00 //C 0faa0 ret */
0x0000faa1, 0x0ded0000, /* 0ded0000 //C 0faa1 push	lr */
0x0000faa2, 0x01a20000, /* 01a20000 //C 0faa2 mvzl	r10,0 */
0x0000faa3, 0x0400fa88, /* 0400fa88 //C 0faa3 call	print_reg_name_value */
0x0000faa4, 0x01a40001, /* 01a40001 //C 0faa4 add	r10,1 */
0x0000faa5, 0x01a80011, /* 01a80011 //C 0faa5 cmp	r10,17 */
0x0000faa6, 0x21f2faa3, /* 21f2faa3 //C 0faa6 jnz	r_cyc */
0x0000faa7, 0x0710fc70, /* 0710fc70 //C 0faa7 ld	r1,regf */
0x0000faa8, 0x01320020, /* 01320020 //C 0faa8 mvzl	r3,32 */
0x0000faa9, 0x01020055, /* 01020055 //C 0faa9 mvzl	r0,'U' */
0x0000faaa, 0x01220020, /* 01220020 //C 0faaa mvzl	r2,0x20 */
0x0000faab, 0x0400fa97, /* 0400fa97 //C 0faab call	print_flag */
0x0000faac, 0x01020050, /* 01020050 //C 0faac mvzl	r0,'P' */
0x0000faad, 0x01220010, /* 01220010 //C 0faad mvzl	r2,0x10 */
0x0000faae, 0x0400fa97, /* 0400fa97 //C 0faae call	print_flag */
0x0000faaf, 0x0102004f, /* 0102004f //C 0faaf mvzl	r0,'O' */
0x0000fab0, 0x01220008, /* 01220008 //C 0fab0 mvzl	r2,8 */
0x0000fab1, 0x0400fa97, /* 0400fa97 //C 0fab1 call	print_flag */
0x0000fab2, 0x0102005a, /* 0102005a //C 0fab2 mvzl	r0,'Z' */
0x0000fab3, 0x01220004, /* 01220004 //C 0fab3 mvzl	r2,4 */
0x0000fab4, 0x0400fa97, /* 0400fa97 //C 0fab4 call	print_flag */
0x0000fab5, 0x01020043, /* 01020043 //C 0fab5 mvzl	r0,'C' */
0x0000fab6, 0x01220002, /* 01220002 //C 0fab6 mvzl	r2,2 */
0x0000fab7, 0x0400fa97, /* 0400fa97 //C 0fab7 call	print_flag */
0x0000fab8, 0x01020053, /* 01020053 //C 0fab8 mvzl	r0,'S' */
0x0000fab9, 0x01220001, /* 01220001 //C 0fab9 mvzl	r2,1 */
0x0000faba, 0x0132000a, /* 0132000a //C 0faba mvzl	r3,LF */
0x0000fabb, 0x0400fa97, /* 0400fa97 //C 0fabb call	print_flag */
0x0000fabc, 0x0fed0000, /* 0fed0000 //C 0fabc pop	lr */
0x0000fabd, 0x00f00e00, /* 00f00e00 //C 0fabd ret */
0x0000fabe, 0x0ded0000, /* 0ded0000 //C 0fabe push	lr */
0x0000fabf, 0x07a0fc5f, /* 07a0fc5f //C 0fabf ld	r10,nuof_reg	; Reg num is in R10 */
0x0000fac0, 0x01a80010, /* 01a80010 //C 0fac0 cmp	r10,16 */
0x0000fac1, 0xa1f2fad6, /* a1f2fad6 //C 0fac1 LS jmp	rx_nr_ok */
0x0000fac2, 0x0102fac5, /* 0102fac5 //C 0fac2 mvzl	r0,rx_err_nr */
0x0000fac3, 0x0400fbc3, /* 0400fbc3 //C 0fac3 call	printsnl */
0x0000fac4, 0x01f2faf2, /* 01f2faf2 //C 0fac4 jmp	rx_ret */
0x0000fac5, 0x0000004e, /* 0000004e //C 0fac5 db	78 */
0x0000fac6, 0x0000006f, /* 0000006f //C 0fac6 db	111 */
0x0000fac7, 0x00000020, /* 00000020 //C 0fac7 db	32 */
0x0000fac8, 0x00000073, /* 00000073 //C 0fac8 db	115 */
0x0000fac9, 0x00000075, /* 00000075 //C 0fac9 db	117 */
0x0000faca, 0x00000063, /* 00000063 //C 0faca db	99 */
0x0000facb, 0x00000068, /* 00000068 //C 0facb db	104 */
0x0000facc, 0x00000020, /* 00000020 //C 0facc db	32 */
0x0000facd, 0x00000072, /* 00000072 //C 0facd db	114 */
0x0000face, 0x00000065, /* 00000065 //C 0face db	101 */
0x0000facf, 0x00000067, /* 00000067 //C 0facf db	103 */
0x0000fad0, 0x00000069, /* 00000069 //C 0fad0 db	105 */
0x0000fad1, 0x00000073, /* 00000073 //C 0fad1 db	115 */
0x0000fad2, 0x00000074, /* 00000074 //C 0fad2 db	116 */
0x0000fad3, 0x00000065, /* 00000065 //C 0fad3 db	101 */
0x0000fad4, 0x00000072, /* 00000072 //C 0fad4 db	114 */
0x0000fad5, 0x00000000, /* 00000000 //C 0fad5 db */
0x0000fad6, 0x0122fc58, /* 0122fc58 //C 0fad6 mvzl	r2,words */
0x0000fad7, 0x0e420001, /* 0e420001 //C 0fad7 ld	r4,r2,1		; get aof first parameter */
0x0000fad8, 0x024b0000, /* 024b0000 //C 0fad8 sz	r4		; is it NULL? */
0x0000fad9, 0x11f2faf1, /* 11f2faf1 //C 0fad9 jz	rx_print */
0x0000fada, 0x00000400, /* 00000400 //C 0fada mov	r0,r4 */
0x0000fadb, 0x0400fb7d, /* 0400fb7d //C 0fadb call	htoi */
0x0000fadc, 0x00500100, /* 00500100 //C 0fadc mov	r5,r1		; Value is in R5 */
0x0000fadd, 0x31f2faed, /* 31f2faed //C 0fadd C1 jmp	rx_val_ok */
0x0000fade, 0x0102fae1, /* 0102fae1 //C 0fade mvzl	r0,rx_err_val */
0x0000fadf, 0x0400fbc3, /* 0400fbc3 //C 0fadf call	printsnl */
0x0000fae0, 0x01f2faf2, /* 01f2faf2 //C 0fae0 jmp	rx_ret */
0x0000fae1, 0x00000056, /* 00000056 //C 0fae1 db	86 */
0x0000fae2, 0x00000061, /* 00000061 //C 0fae2 db	97 */
0x0000fae3, 0x0000006c, /* 0000006c //C 0fae3 db	108 */
0x0000fae4, 0x00000075, /* 00000075 //C 0fae4 db	117 */
0x0000fae5, 0x00000065, /* 00000065 //C 0fae5 db	101 */
0x0000fae6, 0x00000020, /* 00000020 //C 0fae6 db	32 */
0x0000fae7, 0x00000065, /* 00000065 //C 0fae7 db	101 */
0x0000fae8, 0x00000072, /* 00000072 //C 0fae8 db	114 */
0x0000fae9, 0x00000072, /* 00000072 //C 0fae9 db	114 */
0x0000faea, 0x0000006f, /* 0000006f //C 0faea db	111 */
0x0000faeb, 0x00000072, /* 00000072 //C 0faeb db	114 */
0x0000faec, 0x00000000, /* 00000000 //C 0faec db */
0x0000faed, 0x01a80010, /* 01a80010 //C 0faed cmp	r10,16		; Flag reg? */
0x0000faee, 0x115f003f, /* 115f003f //C 0faee EQ and	r5,0x3f */
0x0000faef, 0x0102fc60, /* 0102fc60 //C 0faef mvzl	r0,reg0 */
0x0000faf0, 0x08500a00, /* 08500a00 //C 0faf0 st	r5,r0,r10 */
0x0000faf1, 0x0400fa88, /* 0400fa88 //C 0faf1 call	print_reg_name_value */
0x0000faf2, 0x0fed0000, /* 0fed0000 //C 0faf2 pop	lr */
0x0000faf3, 0x00f00e00, /* 00f00e00 //C 0faf3 ret */
0x0000faf4, 0x0102000d, /* 0102000d //C 0faf4 mvzl	r0,13 */
0x0000faf5, 0x0600fc5f, /* 0600fc5f //C 0faf5 st	r0,nuof_reg */
0x0000faf6, 0x01f2fabe, /* 01f2fabe //C 0faf6 jmp	cmd_rx */
0x0000faf7, 0x0102000e, /* 0102000e //C 0faf7 mvzl	r0,14 */
0x0000faf8, 0x0600fc5f, /* 0600fc5f //C 0faf8 st	r0,nuof_reg */
0x0000faf9, 0x01f2fabe, /* 01f2fabe //C 0faf9 jmp	cmd_rx */
0x0000fafa, 0x0102000f, /* 0102000f //C 0fafa mvzl	r0,15 */
0x0000fafb, 0x0600fc5f, /* 0600fc5f //C 0fafb st	r0,nuof_reg */
0x0000fafc, 0x01f2fabe, /* 01f2fabe //C 0fafc jmp	cmd_rx */
0x0000fafd, 0x01020010, /* 01020010 //C 0fafd mvzl	r0,16 */
0x0000fafe, 0x0600fc5f, /* 0600fc5f //C 0fafe st	r0,nuof_reg */
0x0000faff, 0x01f2fabe, /* 01f2fabe //C 0faff jmp	cmd_rx */
0x0000fb00, 0x0d1d0000, /* 0d1d0000 //C 0fb00 push	r1 */
0x0000fb01, 0x0d2d0000, /* 0d2d0000 //C 0fb01 push	r2 */
0x0000fb02, 0x0e210000, /* 0e210000 //C 0fb02 ld	r2,r1 */
0x0000fb03, 0x022b0000, /* 022b0000 //C 0fb03 sz	r2 */
0x0000fb04, 0x11f2fb0b, /* 11f2fb0b //C 0fb04 jz	strchr_no	; eof string found */
0x0000fb05, 0x00280000, /* 00280000 //C 0fb05 cmp	r2,r0		; compare */
0x0000fb06, 0x11f2fb09, /* 11f2fb09 //C 0fb06 jz	strchr_yes */
0x0000fb07, 0x011a0001, /* 011a0001 //C 0fb07 plus	r1,1		; go to next char */
0x0000fb08, 0x01f2fb02, /* 01f2fb02 //C 0fb08 jmp	strchr_cyc */
0x0000fb09, 0x020c0000, /* 020c0000 //C 0fb09 sec */
0x0000fb0a, 0x01f2fb0d, /* 01f2fb0d //C 0fb0a jmp	strchr_ret */
0x0000fb0b, 0x01120000, /* 01120000 //C 0fb0b mvzl	r1,0 */
0x0000fb0c, 0x020d0000, /* 020d0000 //C 0fb0c clc */
0x0000fb0d, 0x0f2d0000, /* 0f2d0000 //C 0fb0d pop	r2 */
0x0000fb0e, 0x0f1d0000, /* 0f1d0000 //C 0fb0e pop	r1 */
0x0000fb0f, 0x00f00e00, /* 00f00e00 //C 0fb0f ret */
0x0000fb10, 0x0ded0000, /* 0ded0000 //C 0fb10 push	lr		; Save used registers */
0x0000fb11, 0x0d0d0000, /* 0d0d0000 //C 0fb11 push	r0		; and input parameters */
0x0000fb12, 0x0d1d0000, /* 0d1d0000 //C 0fb12 push	r1 */
0x0000fb13, 0x0d2d0000, /* 0d2d0000 //C 0fb13 push	r2 */
0x0000fb14, 0x0d4d0000, /* 0d4d0000 //C 0fb14 push	r4 */
0x0000fb15, 0x0d5d0000, /* 0d5d0000 //C 0fb15 push	r5 */
0x0000fb16, 0x0d6d0000, /* 0d6d0000 //C 0fb16 push	r6 */
0x0000fb17, 0x0e200000, /* 0e200000 //C 0fb17 ld	r2,r0		; Got one-one char */
0x0000fb18, 0x0e610000, /* 0e610000 //C 0fb18 ld	r6,r1		; from two strings */
0x0000fb19, 0x023b0000, /* 023b0000 //C 0fb19 sz	r3		; Prepare for comparing */
0x0000fb1a, 0x112d0020, /* 112d0020 //C 0fb1a Z1 or	r2,0x20		; if insensitive case */
0x0000fb1b, 0x023b0000, /* 023b0000 //C 0fb1b sz	r3 */
0x0000fb1c, 0x116d0020, /* 116d0020 //C 0fb1c Z1 or	r6,0x20 */
0x0000fb1d, 0x00280600, /* 00280600 //C 0fb1d cmp	r2,r6		; compare them */
0x0000fb1e, 0x21f2fb30, /* 21f2fb30 //C 0fb1e jnz	streq_no	; if differs: strings are not equal */
0x0000fb1f, 0x0e200000, /* 0e200000 //C 0fb1f ld	r2,r0		; Pick original (non-prepared) */
0x0000fb20, 0x0e610000, /* 0e610000 //C 0fb20 ld	r6,r1		; chars to check EOS */
0x0000fb21, 0x022b0000, /* 022b0000 //C 0fb21 sz	r2		; convert them to boolean */
0x0000fb22, 0x21220001, /* 21220001 //C 0fb22 Z0 mvzl	r2,1		; values in R2,R6 */
0x0000fb23, 0x11220000, /* 11220000 //C 0fb23 Z1 mvzl	r2,0		; and copy in R4,R5 */
0x0000fb24, 0x00400200, /* 00400200 //C 0fb24 mov	r4,r2 */
0x0000fb25, 0x026b0000, /* 026b0000 //C 0fb25 sz	r6 */
0x0000fb26, 0x21620001, /* 21620001 //C 0fb26 Z0 mvzl	r6,1 */
0x0000fb27, 0x11620000, /* 11620000 //C 0fb27 Z1 mvzl r6,0 */
0x0000fb28, 0x00500600, /* 00500600 //C 0fb28 mov	r5,r6 */
0x0000fb29, 0x004d0500, /* 004d0500 //C 0fb29 or	r4,r5		; if both are EOS: equal */
0x0000fb2a, 0x11f2fb32, /* 11f2fb32 //C 0fb2a jz	streq_yes */
0x0000fb2b, 0x002f0600, /* 002f0600 //C 0fb2b and 	r2,r6		; just one is EOS: not equal */
0x0000fb2c, 0x11f2fb30, /* 11f2fb30 //C 0fb2c jz	streq_no */
0x0000fb2d, 0x010a0001, /* 010a0001 //C 0fb2d plus	r0,1		; non are EOS: go to check next char */
0x0000fb2e, 0x011a0001, /* 011a0001 //C 0fb2e plus	r1,1 */
0x0000fb2f, 0x01f2fb17, /* 01f2fb17 //C 0fb2f jmp	streq_cyc */
0x0000fb30, 0x020d0000, /* 020d0000 //C 0fb30 clc			; False result */
0x0000fb31, 0x01f2fb33, /* 01f2fb33 //C 0fb31 jmp	streq_ret */
0x0000fb32, 0x020c0000, /* 020c0000 //C 0fb32 sec			; True result */
0x0000fb33, 0x0f6d0000, /* 0f6d0000 //C 0fb33 pop	r6 */
0x0000fb34, 0x0f5d0000, /* 0f5d0000 //C 0fb34 pop	r5 */
0x0000fb35, 0x0f4d0000, /* 0f4d0000 //C 0fb35 pop	r4 */
0x0000fb36, 0x0f2d0000, /* 0f2d0000 //C 0fb36 pop	r2 */
0x0000fb37, 0x0f1d0000, /* 0f1d0000 //C 0fb37 pop	r1 */
0x0000fb38, 0x0f0d0000, /* 0f0d0000 //C 0fb38 pop	r0 */
0x0000fb39, 0x0fed0000, /* 0fed0000 //C 0fb39 pop	lr */
0x0000fb3a, 0x00f00e00, /* 00f00e00 //C 0fb3a ret */
0x0000fb3b, 0x0ded0000, /* 0ded0000 //C 0fb3b push	lr */
0x0000fb3c, 0x0d3d0000, /* 0d3d0000 //C 0fb3c push	r3 */
0x0000fb3d, 0x01320001, /* 01320001 //C 0fb3d mvzl	r3,1 */
0x0000fb3e, 0x0400fb10, /* 0400fb10 //C 0fb3e call	str_cmp_eq */
0x0000fb3f, 0x0f3d0000, /* 0f3d0000 //C 0fb3f pop	r3 */
0x0000fb40, 0x0fed0000, /* 0fed0000 //C 0fb40 pop	lr */
0x0000fb41, 0x00f00e00, /* 00f00e00 //C 0fb41 ret */
0x0000fb42, 0x0ded0000, /* 0ded0000 //C 0fb42 push	lr */
0x0000fb43, 0x0d3d0000, /* 0d3d0000 //C 0fb43 push	r3 */
0x0000fb44, 0x01320000, /* 01320000 //C 0fb44 mvzl	r3,0 */
0x0000fb45, 0x0400fb10, /* 0400fb10 //C 0fb45 call	str_cmp_eq */
0x0000fb46, 0x0f3d0000, /* 0f3d0000 //C 0fb46 pop	r3 */
0x0000fb47, 0x0fed0000, /* 0fed0000 //C 0fb47 pop	lr */
0x0000fb48, 0x00f00e00, /* 00f00e00 //C 0fb48 ret */
0x0000fb49, 0x0108002f, /* 0108002f //C 0fb49 cmp	r0,'/' */
0x0000fb4a, 0xa1f2fb5f, /* a1f2fb5f //C 0fb4a LS jmp	hc2v_nok */
0x0000fb4b, 0x01080039, /* 01080039 //C 0fb4b cmp	r0,'9' */
0x0000fb4c, 0xa1f2fb5c, /* a1f2fb5c //C 0fb4c LS jmp	hc2v_0_9 */
0x0000fb4d, 0x01080040, /* 01080040 //C 0fb4d cmp	r0,'@' */
0x0000fb4e, 0xa1f2fb5f, /* a1f2fb5f //C 0fb4e LS jmp	hc2v_nok */
0x0000fb4f, 0x01080046, /* 01080046 //C 0fb4f cmp	r0,'F' */
0x0000fb50, 0xa1f2fb59, /* a1f2fb59 //C 0fb50 LS jmp	hc2v_A_F */
0x0000fb51, 0x01080060, /* 01080060 //C 0fb51 cmp	r0,'`' */
0x0000fb52, 0xa1f2fb5f, /* a1f2fb5f //C 0fb52 LS jmp	hc2v_nok */
0x0000fb53, 0x01080066, /* 01080066 //C 0fb53 cmp	r0,'f' */
0x0000fb54, 0xa1f2fb56, /* a1f2fb56 //C 0fb54 LS jmp	hc2v_a_f */
0x0000fb55, 0x01f2fb5f, /* 01f2fb5f //C 0fb55 jmp	hc2v_nok */
0x0000fb56, 0x0104000a, /* 0104000a //C 0fb56 add	r0,10 */
0x0000fb57, 0x01060061, /* 01060061 //C 0fb57 sub	r0,'a' */
0x0000fb58, 0x01f2fb5d, /* 01f2fb5d //C 0fb58 jmp	hc2v_ok */
0x0000fb59, 0x0104000a, /* 0104000a //C 0fb59 add	r0,10 */
0x0000fb5a, 0x01060041, /* 01060041 //C 0fb5a sub	r0,'A' */
0x0000fb5b, 0x01f2fb5d, /* 01f2fb5d //C 0fb5b jmp	hc2v_ok */
0x0000fb5c, 0x01060030, /* 01060030 //C 0fb5c sub	r0,'0' */
0x0000fb5d, 0x020c0000, /* 020c0000 //C 0fb5d sec */
0x0000fb5e, 0x00f00e00, /* 00f00e00 //C 0fb5e ret */
0x0000fb5f, 0x020d0000, /* 020d0000 //C 0fb5f clc */
0x0000fb60, 0x00f00e00, /* 00f00e00 //C 0fb60 ret */
0x0000fb61, 0x0d1d0000, /* 0d1d0000 //C 0fb61 push	r1 */
0x0000fb62, 0x010f000f, /* 010f000f //C 0fb62 and	r0,0xf */
0x0000fb63, 0x0112fb67, /* 0112fb67 //C 0fb63 mvzl	r1,v2hc_table */
0x0000fb64, 0x0a010000, /* 0a010000 //C 0fb64 ld	r0,r1,r0 */
0x0000fb65, 0x0f1d0000, /* 0f1d0000 //C 0fb65 pop	r1 */
0x0000fb66, 0x00f00e00, /* 00f00e00 //C 0fb66 ret */
0x0000fb67, 0x00000030, /* 00000030 //C 0fb67 db	48 */
0x0000fb68, 0x00000031, /* 00000031 //C 0fb68 db	49 */
0x0000fb69, 0x00000032, /* 00000032 //C 0fb69 db	50 */
0x0000fb6a, 0x00000033, /* 00000033 //C 0fb6a db	51 */
0x0000fb6b, 0x00000034, /* 00000034 //C 0fb6b db	52 */
0x0000fb6c, 0x00000035, /* 00000035 //C 0fb6c db	53 */
0x0000fb6d, 0x00000036, /* 00000036 //C 0fb6d db	54 */
0x0000fb6e, 0x00000037, /* 00000037 //C 0fb6e db	55 */
0x0000fb6f, 0x00000038, /* 00000038 //C 0fb6f db	56 */
0x0000fb70, 0x00000039, /* 00000039 //C 0fb70 db	57 */
0x0000fb71, 0x00000041, /* 00000041 //C 0fb71 db	65 */
0x0000fb72, 0x00000042, /* 00000042 //C 0fb72 db	66 */
0x0000fb73, 0x00000043, /* 00000043 //C 0fb73 db	67 */
0x0000fb74, 0x00000044, /* 00000044 //C 0fb74 db	68 */
0x0000fb75, 0x00000045, /* 00000045 //C 0fb75 db	69 */
0x0000fb76, 0x00000046, /* 00000046 //C 0fb76 db	70 */
0x0000fb77, 0x00000000, /* 00000000 //C 0fb77 db */
0x0000fb78, 0x0ded0000, /* 0ded0000 //C 0fb78 push	lr */
0x0000fb79, 0x0400fb61, /* 0400fb61 //C 0fb79 call	value2Hexchar */
0x0000fb7a, 0x010d0020, /* 010d0020 //C 0fb7a or	r0,0x20 */
0x0000fb7b, 0x0fed0000, /* 0fed0000 //C 0fb7b pop	lr */
0x0000fb7c, 0x00f00e00, /* 00f00e00 //C 0fb7c ret */
0x0000fb7d, 0x0ded0000, /* 0ded0000 //C 0fb7d push	lr */
0x0000fb7e, 0x0d2d0000, /* 0d2d0000 //C 0fb7e push	r2 */
0x0000fb7f, 0x0d3d0000, /* 0d3d0000 //C 0fb7f push	r3 */
0x0000fb80, 0x01120000, /* 01120000 //C 0fb80 mvzl	r1,0		; return value */
0x0000fb81, 0x01320000, /* 01320000 //C 0fb81 mvzl	r3,0		; index */
0x0000fb82, 0x0b238000, /* 0b238000 //C 0fb82 ld	r2,r3+,r0	; pick a char */
0x0000fb83, 0x022b0000, /* 022b0000 //C 0fb83 sz	r2		; check eof string */
0x0000fb84, 0x11f2fb98, /* 11f2fb98 //C 0fb84 jz	htoi_eos */
0x0000fb85, 0x0128002e, /* 0128002e //C 0fb85 cmp	r2,'.'		; skip separators */
0x0000fb86, 0x11f2fb82, /* 11f2fb82 //C 0fb86 jz	htoi_cyc */
0x0000fb87, 0x0128005f, /* 0128005f //C 0fb87 cmp	r2,'_' */
0x0000fb88, 0x11f2fb82, /* 11f2fb82 //C 0fb88 jz	htoi_cyc */
0x0000fb89, 0x0d0d0000, /* 0d0d0000 //C 0fb89 push	r0 */
0x0000fb8a, 0x00000200, /* 00000200 //C 0fb8a mov	r0,r2 */
0x0000fb8b, 0x0400fb49, /* 0400fb49 //C 0fb8b call	hexchar2value */
0x0000fb8c, 0x00200000, /* 00200000 //C 0fb8c mov	r2,r0 */
0x0000fb8d, 0x0f0d0000, /* 0f0d0000 //C 0fb8d pop	r0 */
0x0000fb8e, 0x41f2fb96, /* 41f2fb96 //C 0fb8e C0 jmp	htoi_nok */
0x0000fb8f, 0x02180000, /* 02180000 //C 0fb8f shl	r1 */
0x0000fb90, 0x02180000, /* 02180000 //C 0fb90 shl	r1 */
0x0000fb91, 0x02180000, /* 02180000 //C 0fb91 shl	r1 */
0x0000fb92, 0x02180000, /* 02180000 //C 0fb92 shl	r1 */
0x0000fb93, 0x012f000f, /* 012f000f //C 0fb93 and	r2,0xf */
0x0000fb94, 0x001d0200, /* 001d0200 //C 0fb94 or	r1,r2 */
0x0000fb95, 0x01f2fb82, /* 01f2fb82 //C 0fb95 jmp	htoi_cyc */
0x0000fb96, 0x020d0000, /* 020d0000 //C 0fb96 clc */
0x0000fb97, 0x01f2fb9b, /* 01f2fb9b //C 0fb97 jmp	htoi_ret */
0x0000fb98, 0x01380001, /* 01380001 //C 0fb98 cmp	r3,1 */
0x0000fb99, 0x920d0000, /* 920d0000 //C 0fb99 HI clc */
0x0000fb9a, 0xa20c0000, /* a20c0000 //C 0fb9a LS sec */
0x0000fb9b, 0x0f3d0000, /* 0f3d0000 //C 0fb9b pop	r3 */
0x0000fb9c, 0x0f2d0000, /* 0f2d0000 //C 0fb9c pop	r2 */
0x0000fb9d, 0x0fed0000, /* 0fed0000 //C 0fb9d pop	lr */
0x0000fb9e, 0x00f00e00, /* 00f00e00 //C 0fb9e ret */
0x0000fb9f, 0x0d0d0000, /* 0d0d0000 //C 0fb9f push	r0 */
0x0000fba0, 0x0700ff42, /* 0700ff42 //C 0fba0 ld	r0,UART_RSTAT */
0x0000fba1, 0x010c0001, /* 010c0001 //C 0fba1 test	r0,1 */
0x0000fba2, 0x020d0000, /* 020d0000 //C 0fba2 clc */
0x0000fba3, 0x220c0000, /* 220c0000 //C 0fba3 Z0 sec */
0x0000fba4, 0x0f0d0000, /* 0f0d0000 //C 0fba4 pop	r0 */
0x0000fba5, 0x00f00e00, /* 00f00e00 //C 0fba5 ret */
0x0000fba6, 0x0700ff40, /* 0700ff40 //C 0fba6 ld	r0,UART_DR */
0x0000fba7, 0x00f00e00, /* 00f00e00 //C 0fba7 ret */
0x0000fba8, 0x0d9d0000, /* 0d9d0000 //C 0fba8 push	r9 */
0x0000fba9, 0x0790ff43, /* 0790ff43 //C 0fba9 ld	r9,UART_TSTAT */
0x0000fbaa, 0x019c0001, /* 019c0001 //C 0fbaa test	r9,1 */
0x0000fbab, 0x11f2fba9, /* 11f2fba9 //C 0fbab jz	wait_tc */
0x0000fbac, 0x0600ff40, /* 0600ff40 //C 0fbac st	r0,UART_DR */
0x0000fbad, 0x0f9d0000, /* 0f9d0000 //C 0fbad pop	r9 */
0x0000fbae, 0x00f00e00, /* 00f00e00 //C 0fbae ret */
0x0000fbaf, 0x0ded0000, /* 0ded0000 //C 0fbaf push	lr */
0x0000fbb0, 0x0d0d0000, /* 0d0d0000 //C 0fbb0 push	r0 */
0x0000fbb1, 0x0d3d0000, /* 0d3d0000 //C 0fbb1 push	r3 */
0x0000fbb2, 0x0d4d0000, /* 0d4d0000 //C 0fbb2 push	r4 */
0x0000fbb3, 0x01420000, /* 01420000 //C 0fbb3 mvzl	r4,0 */
0x0000fbb4, 0x020b0000, /* 020b0000 //C 0fbb4 sz	r0 */
0x0000fbb5, 0x1102fc8a, /* 1102fc8a //C 0fbb5 Z1 mvzl	r0,null_str */
0x0000fbb6, 0x0b348000, /* 0b348000 //C 0fbb6 ld	r3,r4+,r0 */
0x0000fbb7, 0x023b0000, /* 023b0000 //C 0fbb7 sz	r3 */
0x0000fbb8, 0x11f2fbbe, /* 11f2fbbe //C 0fbb8 jz	prints_done */
0x0000fbb9, 0x0d0d0000, /* 0d0d0000 //C 0fbb9 push	r0 */
0x0000fbba, 0x00000300, /* 00000300 //C 0fbba mov	r0,r3 */
0x0000fbbb, 0x0400fba8, /* 0400fba8 //C 0fbbb call	putchar */
0x0000fbbc, 0x0f0d0000, /* 0f0d0000 //C 0fbbc pop	r0 */
0x0000fbbd, 0x01f2fbb6, /* 01f2fbb6 //C 0fbbd jmp	prints_go */
0x0000fbbe, 0x0f4d0000, /* 0f4d0000 //C 0fbbe pop	r4 */
0x0000fbbf, 0x0f3d0000, /* 0f3d0000 //C 0fbbf pop	r3 */
0x0000fbc0, 0x0f0d0000, /* 0f0d0000 //C 0fbc0 pop	r0 */
0x0000fbc1, 0x0fed0000, /* 0fed0000 //C 0fbc1 pop	lr */
0x0000fbc2, 0x00f00e00, /* 00f00e00 //C 0fbc2 ret */
0x0000fbc3, 0x0ded0000, /* 0ded0000 //C 0fbc3 push	lr */
0x0000fbc4, 0x0400fbaf, /* 0400fbaf //C 0fbc4 call	prints */
0x0000fbc5, 0x0d0d0000, /* 0d0d0000 //C 0fbc5 push	r0 */
0x0000fbc6, 0x0102000a, /* 0102000a //C 0fbc6 mvzl	r0,LF */
0x0000fbc7, 0x0400fba8, /* 0400fba8 //C 0fbc7 call	putchar */
0x0000fbc8, 0x0f0d0000, /* 0f0d0000 //C 0fbc8 pop	r0 */
0x0000fbc9, 0x0fed0000, /* 0fed0000 //C 0fbc9 pop	lr */
0x0000fbca, 0x00f00e00, /* 00f00e00 //C 0fbca ret */
0x0000fbcb, 0x0ded0000, /* 0ded0000 //C 0fbcb push	lr */
0x0000fbcc, 0x0d0d0000, /* 0d0d0000 //C 0fbcc push	r0 */
0x0000fbcd, 0x0d1d0000, /* 0d1d0000 //C 0fbcd push	r1 */
0x0000fbce, 0x0d2d0000, /* 0d2d0000 //C 0fbce push	r2 */
0x0000fbcf, 0x0d3d0000, /* 0d3d0000 //C 0fbcf push	r3 */
0x0000fbd0, 0x0d4d0000, /* 0d4d0000 //C 0fbd0 push	r4 */
0x0000fbd1, 0x00300000, /* 00300000 //C 0fbd1 mov	r3,r0 */
0x0000fbd2, 0x01220000, /* 01220000 //C 0fbd2 mvzl	r2,0 */
0x0000fbd3, 0x01420001, /* 01420001 //C 0fbd3 mvzl	r4,1 */
0x0000fbd4, 0x01020000, /* 01020000 //C 0fbd4 mvzl	r0,0 */
0x0000fbd5, 0x02380000, /* 02380000 //C 0fbd5 shl	r3 */
0x0000fbd6, 0x02070000, /* 02070000 //C 0fbd6 rol	r0 */
0x0000fbd7, 0x02380000, /* 02380000 //C 0fbd7 shl	r3 */
0x0000fbd8, 0x02070000, /* 02070000 //C 0fbd8 rol	r0 */
0x0000fbd9, 0x02380000, /* 02380000 //C 0fbd9 shl	r3 */
0x0000fbda, 0x02070000, /* 02070000 //C 0fbda rol	r0 */
0x0000fbdb, 0x02380000, /* 02380000 //C 0fbdb shl	r3 */
0x0000fbdc, 0x02070000, /* 02070000 //C 0fbdc rol	r0 */
0x0000fbdd, 0x0400fb61, /* 0400fb61 //C 0fbdd call	value2Hexchar */
0x0000fbde, 0x0400fba8, /* 0400fba8 //C 0fbde call	putchar */
0x0000fbdf, 0x01240001, /* 01240001 //C 0fbdf add	r2,1 */
0x0000fbe0, 0x01280008, /* 01280008 //C 0fbe0 cmp	r2,8 */
0x0000fbe1, 0x11f2fbeb, /* 11f2fbeb //C 0fbe1 jz	print_vhex_ret */
0x0000fbe2, 0x021b0000, /* 021b0000 //C 0fbe2 sz	r1 */
0x0000fbe3, 0x11f2fbe9, /* 11f2fbe9 //C 0fbe3 jz	print_vhex_nosep */
0x0000fbe4, 0x00480100, /* 00480100 //C 0fbe4 cmp	r4,r1 */
0x0000fbe5, 0x21f2fbe9, /* 21f2fbe9 //C 0fbe5 jnz	print_vhex_nosep */
0x0000fbe6, 0x0102005f, /* 0102005f //C 0fbe6 mvzl	r0,'_' */
0x0000fbe7, 0x0400fba8, /* 0400fba8 //C 0fbe7 call	putchar */
0x0000fbe8, 0x01420000, /* 01420000 //C 0fbe8 mvzl	r4,0 */
0x0000fbe9, 0x01440001, /* 01440001 //C 0fbe9 add	r4,1 */
0x0000fbea, 0x01f2fbd4, /* 01f2fbd4 //C 0fbea jmp	print_vhex_cyc */
0x0000fbeb, 0x0f4d0000, /* 0f4d0000 //C 0fbeb pop	r4 */
0x0000fbec, 0x0f3d0000, /* 0f3d0000 //C 0fbec pop	r3 */
0x0000fbed, 0x0f2d0000, /* 0f2d0000 //C 0fbed pop	r2 */
0x0000fbee, 0x0f1d0000, /* 0f1d0000 //C 0fbee pop	r1 */
0x0000fbef, 0x0f0d0000, /* 0f0d0000 //C 0fbef pop	r0 */
0x0000fbf0, 0x0fed0000, /* 0fed0000 //C 0fbf0 pop	lr */
0x0000fbf1, 0x00f00e00, /* 00f00e00 //C 0fbf1 ret */
0x0000fc5e, 0x00000000, /* 00000000 //C 0fc5e dd	0 */
0x0000fc5f, 0x00000000, /* 00000000 //C 0fc5f dd	0 */
0x0000fc60, 0x00000000, /* 00000000 //C 0fc60 dd	0 */
0x0000fc61, 0x00000000, /* 00000000 //C 0fc61 dd	0 */
0x0000fc62, 0x00000000, /* 00000000 //C 0fc62 dd	0 */
0x0000fc63, 0x00000000, /* 00000000 //C 0fc63 dd	0 */
0x0000fc64, 0x00000000, /* 00000000 //C 0fc64 dd	0 */
0x0000fc65, 0x00000000, /* 00000000 //C 0fc65 dd	0 */
0x0000fc66, 0x00000000, /* 00000000 //C 0fc66 dd	0 */
0x0000fc67, 0x00000000, /* 00000000 //C 0fc67 dd	0 */
0x0000fc68, 0x00000000, /* 00000000 //C 0fc68 dd	0 */
0x0000fc69, 0x00000000, /* 00000000 //C 0fc69 dd	0 */
0x0000fc6a, 0x00000000, /* 00000000 //C 0fc6a dd	0 */
0x0000fc6b, 0x00000000, /* 00000000 //C 0fc6b dd	0 */
0x0000fc6c, 0x00000000, /* 00000000 //C 0fc6c dd	0 */
0x0000fc6d, 0x00000000, /* 00000000 //C 0fc6d dd	0 */
0x0000fc6e, 0x00000000, /* 00000000 //C 0fc6e dd	0 */
0x0000fc6f, 0x00000000, /* 00000000 //C 0fc6f dd	0 */
0x0000fc70, 0x00000000, /* 00000000 //C 0fc70 dd	0 */
0x0000fc71, 0x00000050, /* 00000050 //C 0fc71 db	80 */
0x0000fc72, 0x0000004d, /* 0000004d //C 0fc72 db	77 */
0x0000fc73, 0x0000006f, /* 0000006f //C 0fc73 db	111 */
0x0000fc74, 0x0000006e, /* 0000006e //C 0fc74 db	110 */
0x0000fc75, 0x00000069, /* 00000069 //C 0fc75 db	105 */
0x0000fc76, 0x00000074, /* 00000074 //C 0fc76 db	116 */
0x0000fc77, 0x0000006f, /* 0000006f //C 0fc77 db	111 */
0x0000fc78, 0x00000072, /* 00000072 //C 0fc78 db	114 */
0x0000fc79, 0x00000020, /* 00000020 //C 0fc79 db	32 */
0x0000fc7a, 0x00000076, /* 00000076 //C 0fc7a db	118 */
0x0000fc7b, 0x00000031, /* 00000031 //C 0fc7b db	49 */
0x0000fc7c, 0x0000002e, /* 0000002e //C 0fc7c db	46 */
0x0000fc7d, 0x00000030, /* 00000030 //C 0fc7d db	48 */
0x0000fc7e, 0x00000000, /* 00000000 //C 0fc7e db */
0x0000fc7f, 0x0000003e, /* 0000003e //C 0fc7f db	62 */
0x0000fc80, 0x00000000, /* 00000000 //C 0fc80 db */
0x0000fc81, 0x00000020, /* 00000020 //C 0fc81 db	32 */
0x0000fc82, 0x0000003b, /* 0000003b //C 0fc82 db	59 */
0x0000fc83, 0x00000009, /* 00000009 //C 0fc83 db	9 */
0x0000fc84, 0x0000000b, /* 0000000b //C 0fc84 db	11 */
0x0000fc85, 0x0000002c, /* 0000002c //C 0fc85 db	44 */
0x0000fc86, 0x0000003d, /* 0000003d //C 0fc86 db	61 */
0x0000fc87, 0x0000005b, /* 0000005b //C 0fc87 db	91 */
0x0000fc88, 0x0000005d, /* 0000005d //C 0fc88 db	93 */
0x0000fc89, 0x00000000, /* 00000000 //C 0fc89 db */
0x0000fc8a, 0x00000028, /* 00000028 //C 0fc8a db	40 */
0x0000fc8b, 0x0000006e, /* 0000006e //C 0fc8b db	110 */
0x0000fc8c, 0x00000075, /* 00000075 //C 0fc8c db	117 */
0x0000fc8d, 0x0000006c, /* 0000006c //C 0fc8d db	108 */
0x0000fc8e, 0x0000006c, /* 0000006c //C 0fc8e db	108 */
0x0000fc8f, 0x00000029, /* 00000029 //C 0fc8f db	41 */
0x0000fc90, 0x00000000, /* 00000000 //C 0fc90 db */
0x0000fc91, 0x0000f8ff, /* 0000f8ff //C 0fc91 dd	cmd_m */
0x0000fc92, 0x0000006d, /* 0000006d //C 0fc92 db	109 */
0x0000fc93, 0x00000000, /* 00000000 //C 0fc93 db */
0x0000fc94, 0x0000f8ff, /* 0000f8ff //C 0fc94 dd	cmd_m */
0x0000fc95, 0x0000006d, /* 0000006d //C 0fc95 db	109 */
0x0000fc96, 0x00000065, /* 00000065 //C 0fc96 db	101 */
0x0000fc97, 0x0000006d, /* 0000006d //C 0fc97 db	109 */
0x0000fc98, 0x00000000, /* 00000000 //C 0fc98 db */
0x0000fc99, 0x0000f954, /* 0000f954 //C 0fc99 dd	cmd_d */
0x0000fc9a, 0x00000064, /* 00000064 //C 0fc9a db	100 */
0x0000fc9b, 0x00000000, /* 00000000 //C 0fc9b db */
0x0000fc9c, 0x0000f954, /* 0000f954 //C 0fc9c dd	cmd_d */
0x0000fc9d, 0x00000064, /* 00000064 //C 0fc9d db	100 */
0x0000fc9e, 0x00000075, /* 00000075 //C 0fc9e db	117 */
0x0000fc9f, 0x0000006d, /* 0000006d //C 0fc9f db	109 */
0x0000fca0, 0x00000070, /* 00000070 //C 0fca0 db	112 */
0x0000fca1, 0x00000000, /* 00000000 //C 0fca1 db */
0x0000fca2, 0x0000f988, /* 0000f988 //C 0fca2 dd	cmd_l */
0x0000fca3, 0x0000006c, /* 0000006c //C 0fca3 db	108 */
0x0000fca4, 0x00000000, /* 00000000 //C 0fca4 db */
0x0000fca5, 0x0000f988, /* 0000f988 //C 0fca5 dd	cmd_l */
0x0000fca6, 0x0000006c, /* 0000006c //C 0fca6 db	108 */
0x0000fca7, 0x0000006f, /* 0000006f //C 0fca7 db	111 */
0x0000fca8, 0x00000061, /* 00000061 //C 0fca8 db	97 */
0x0000fca9, 0x00000064, /* 00000064 //C 0fca9 db	100 */
0x0000fcaa, 0x00000000, /* 00000000 //C 0fcaa db */
0x0000fcab, 0x0000f9f5, /* 0000f9f5 //C 0fcab dd	cmd_g */
0x0000fcac, 0x00000067, /* 00000067 //C 0fcac db	103 */
0x0000fcad, 0x00000000, /* 00000000 //C 0fcad db */
0x0000fcae, 0x0000f9f5, /* 0000f9f5 //C 0fcae dd	cmd_g */
0x0000fcaf, 0x00000067, /* 00000067 //C 0fcaf db	103 */
0x0000fcb0, 0x0000006f, /* 0000006f //C 0fcb0 db	111 */
0x0000fcb1, 0x00000000, /* 00000000 //C 0fcb1 db */
0x0000fcb2, 0x0000f9f5, /* 0000f9f5 //C 0fcb2 dd	cmd_g */
0x0000fcb3, 0x00000072, /* 00000072 //C 0fcb3 db	114 */
0x0000fcb4, 0x00000075, /* 00000075 //C 0fcb4 db	117 */
0x0000fcb5, 0x0000006e, /* 0000006e //C 0fcb5 db	110 */
0x0000fcb6, 0x00000000, /* 00000000 //C 0fcb6 db */
0x0000fcb7, 0x0000fa35, /* 0000fa35 //C 0fcb7 dd	cmd_h */
0x0000fcb8, 0x0000003f, /* 0000003f //C 0fcb8 db	63 */
0x0000fcb9, 0x00000000, /* 00000000 //C 0fcb9 db */
0x0000fcba, 0x0000fa35, /* 0000fa35 //C 0fcba dd	cmd_h */
0x0000fcbb, 0x00000068, /* 00000068 //C 0fcbb db	104 */
0x0000fcbc, 0x00000000, /* 00000000 //C 0fcbc db */
0x0000fcbd, 0x0000fa35, /* 0000fa35 //C 0fcbd dd	cmd_h */
0x0000fcbe, 0x00000068, /* 00000068 //C 0fcbe db	104 */
0x0000fcbf, 0x00000065, /* 00000065 //C 0fcbf db	101 */
0x0000fcc0, 0x0000006c, /* 0000006c //C 0fcc0 db	108 */
0x0000fcc1, 0x00000070, /* 00000070 //C 0fcc1 db	112 */
0x0000fcc2, 0x00000000, /* 00000000 //C 0fcc2 db */
0x0000fcc3, 0x0000faa1, /* 0000faa1 //C 0fcc3 dd	cmd_r */
0x0000fcc4, 0x00000072, /* 00000072 //C 0fcc4 db	114 */
0x0000fcc5, 0x00000000, /* 00000000 //C 0fcc5 db */
0x0000fcc6, 0x0000faa1, /* 0000faa1 //C 0fcc6 dd	cmd_r */
0x0000fcc7, 0x00000072, /* 00000072 //C 0fcc7 db	114 */
0x0000fcc8, 0x00000065, /* 00000065 //C 0fcc8 db	101 */
0x0000fcc9, 0x00000067, /* 00000067 //C 0fcc9 db	103 */
0x0000fcca, 0x00000000, /* 00000000 //C 0fcca db */
0x0000fccb, 0x0000faa1, /* 0000faa1 //C 0fccb dd	cmd_r */
0x0000fccc, 0x00000072, /* 00000072 //C 0fccc db	114 */
0x0000fccd, 0x00000065, /* 00000065 //C 0fccd db	101 */
0x0000fcce, 0x00000067, /* 00000067 //C 0fcce db	103 */
0x0000fccf, 0x00000073, /* 00000073 //C 0fccf db	115 */
0x0000fcd0, 0x00000000, /* 00000000 //C 0fcd0 db */
0x0000fcd1, 0x0000faf4, /* 0000faf4 //C 0fcd1 dd	cmd_sp */
0x0000fcd2, 0x00000073, /* 00000073 //C 0fcd2 db	115 */
0x0000fcd3, 0x00000070, /* 00000070 //C 0fcd3 db	112 */
0x0000fcd4, 0x00000000, /* 00000000 //C 0fcd4 db */
0x0000fcd5, 0x0000faf7, /* 0000faf7 //C 0fcd5 dd	cmd_lr */
0x0000fcd6, 0x0000006c, /* 0000006c //C 0fcd6 db	108 */
0x0000fcd7, 0x00000072, /* 00000072 //C 0fcd7 db	114 */
0x0000fcd8, 0x00000000, /* 00000000 //C 0fcd8 db */
0x0000fcd9, 0x0000fafa, /* 0000fafa //C 0fcd9 dd	cmd_pc */
0x0000fcda, 0x00000070, /* 00000070 //C 0fcda db	112 */
0x0000fcdb, 0x00000063, /* 00000063 //C 0fcdb db	99 */
0x0000fcdc, 0x00000000, /* 00000000 //C 0fcdc db */
0x0000fcdd, 0x0000fafd, /* 0000fafd //C 0fcdd dd	cmd_f */
0x0000fcde, 0x00000066, /* 00000066 //C 0fcde db	102 */
0x0000fcdf, 0x00000000, /* 00000000 //C 0fcdf db */
0x0000fce0, 0x00000000, /* 00000000 //C 0fce0 dd	0 */
0x0000fce1, 0x00000000, /* 00000000 //C 0fce1 dd	0 */
0x0000fce2, 0x0000006d, /* 0000006d //C 0fce2 db	109 */
0x0000fce3, 0x0000005b, /* 0000005b //C 0fce3 db	91 */
0x0000fce4, 0x00000065, /* 00000065 //C 0fce4 db	101 */
0x0000fce5, 0x0000006d, /* 0000006d //C 0fce5 db	109 */
0x0000fce6, 0x0000005d, /* 0000005d //C 0fce6 db	93 */
0x0000fce7, 0x00000020, /* 00000020 //C 0fce7 db	32 */
0x0000fce8, 0x00000061, /* 00000061 //C 0fce8 db	97 */
0x0000fce9, 0x00000064, /* 00000064 //C 0fce9 db	100 */
0x0000fcea, 0x00000064, /* 00000064 //C 0fcea db	100 */
0x0000fceb, 0x00000072, /* 00000072 //C 0fceb db	114 */
0x0000fcec, 0x00000020, /* 00000020 //C 0fcec db	32 */
0x0000fced, 0x0000005b, /* 0000005b //C 0fced db	91 */
0x0000fcee, 0x00000076, /* 00000076 //C 0fcee db	118 */
0x0000fcef, 0x00000061, /* 00000061 //C 0fcef db	97 */
0x0000fcf0, 0x0000006c, /* 0000006c //C 0fcf0 db	108 */
0x0000fcf1, 0x0000005d, /* 0000005d //C 0fcf1 db	93 */
0x0000fcf2, 0x00000020, /* 00000020 //C 0fcf2 db	32 */
0x0000fcf3, 0x00000020, /* 00000020 //C 0fcf3 db	32 */
0x0000fcf4, 0x00000047, /* 00000047 //C 0fcf4 db	71 */
0x0000fcf5, 0x00000065, /* 00000065 //C 0fcf5 db	101 */
0x0000fcf6, 0x00000074, /* 00000074 //C 0fcf6 db	116 */
0x0000fcf7, 0x0000002f, /* 0000002f //C 0fcf7 db	47 */
0x0000fcf8, 0x00000073, /* 00000073 //C 0fcf8 db	115 */
0x0000fcf9, 0x00000065, /* 00000065 //C 0fcf9 db	101 */
0x0000fcfa, 0x00000074, /* 00000074 //C 0fcfa db	116 */
0x0000fcfb, 0x00000020, /* 00000020 //C 0fcfb db	32 */
0x0000fcfc, 0x0000006d, /* 0000006d //C 0fcfc db	109 */
0x0000fcfd, 0x00000065, /* 00000065 //C 0fcfd db	101 */
0x0000fcfe, 0x0000006d, /* 0000006d //C 0fcfe db	109 */
0x0000fcff, 0x0000006f, /* 0000006f //C 0fcff db	111 */
0x0000fd00, 0x00000072, /* 00000072 //C 0fd00 db	114 */
0x0000fd01, 0x00000079, /* 00000079 //C 0fd01 db	121 */
0x0000fd02, 0x0000000a, /* 0000000a //C 0fd02 db	10 */
0x0000fd03, 0x00000000, /* 00000000 //C 0fd03 db */
0x0000fd04, 0x00000064, /* 00000064 //C 0fd04 db	100 */
0x0000fd05, 0x0000005b, /* 0000005b //C 0fd05 db	91 */
0x0000fd06, 0x00000075, /* 00000075 //C 0fd06 db	117 */
0x0000fd07, 0x0000006d, /* 0000006d //C 0fd07 db	109 */
0x0000fd08, 0x00000070, /* 00000070 //C 0fd08 db	112 */
0x0000fd09, 0x0000005d, /* 0000005d //C 0fd09 db	93 */
0x0000fd0a, 0x00000020, /* 00000020 //C 0fd0a db	32 */
0x0000fd0b, 0x00000073, /* 00000073 //C 0fd0b db	115 */
0x0000fd0c, 0x00000074, /* 00000074 //C 0fd0c db	116 */
0x0000fd0d, 0x00000061, /* 00000061 //C 0fd0d db	97 */
0x0000fd0e, 0x00000072, /* 00000072 //C 0fd0e db	114 */
0x0000fd0f, 0x00000074, /* 00000074 //C 0fd0f db	116 */
0x0000fd10, 0x00000020, /* 00000020 //C 0fd10 db	32 */
0x0000fd11, 0x00000065, /* 00000065 //C 0fd11 db	101 */
0x0000fd12, 0x0000006e, /* 0000006e //C 0fd12 db	110 */
0x0000fd13, 0x00000064, /* 00000064 //C 0fd13 db	100 */
0x0000fd14, 0x00000020, /* 00000020 //C 0fd14 db	32 */
0x0000fd15, 0x00000020, /* 00000020 //C 0fd15 db	32 */
0x0000fd16, 0x00000044, /* 00000044 //C 0fd16 db	68 */
0x0000fd17, 0x00000075, /* 00000075 //C 0fd17 db	117 */
0x0000fd18, 0x0000006d, /* 0000006d //C 0fd18 db	109 */
0x0000fd19, 0x00000070, /* 00000070 //C 0fd19 db	112 */
0x0000fd1a, 0x00000020, /* 00000020 //C 0fd1a db	32 */
0x0000fd1b, 0x0000006d, /* 0000006d //C 0fd1b db	109 */
0x0000fd1c, 0x00000065, /* 00000065 //C 0fd1c db	101 */
0x0000fd1d, 0x0000006d, /* 0000006d //C 0fd1d db	109 */
0x0000fd1e, 0x0000006f, /* 0000006f //C 0fd1e db	111 */
0x0000fd1f, 0x00000072, /* 00000072 //C 0fd1f db	114 */
0x0000fd20, 0x00000079, /* 00000079 //C 0fd20 db	121 */
0x0000fd21, 0x00000020, /* 00000020 //C 0fd21 db	32 */
0x0000fd22, 0x00000063, /* 00000063 //C 0fd22 db	99 */
0x0000fd23, 0x0000006f, /* 0000006f //C 0fd23 db	111 */
0x0000fd24, 0x0000006e, /* 0000006e //C 0fd24 db	110 */
0x0000fd25, 0x00000074, /* 00000074 //C 0fd25 db	116 */
0x0000fd26, 0x00000065, /* 00000065 //C 0fd26 db	101 */
0x0000fd27, 0x0000006e, /* 0000006e //C 0fd27 db	110 */
0x0000fd28, 0x00000074, /* 00000074 //C 0fd28 db	116 */
0x0000fd29, 0x0000000a, /* 0000000a //C 0fd29 db	10 */
0x0000fd2a, 0x00000000, /* 00000000 //C 0fd2a db */
0x0000fd2b, 0x0000006c, /* 0000006c //C 0fd2b db	108 */
0x0000fd2c, 0x0000005b, /* 0000005b //C 0fd2c db	91 */
0x0000fd2d, 0x0000006f, /* 0000006f //C 0fd2d db	111 */
0x0000fd2e, 0x00000061, /* 00000061 //C 0fd2e db	97 */
0x0000fd2f, 0x00000064, /* 00000064 //C 0fd2f db	100 */
0x0000fd30, 0x0000005d, /* 0000005d //C 0fd30 db	93 */
0x0000fd31, 0x00000020, /* 00000020 //C 0fd31 db	32 */
0x0000fd32, 0x00000020, /* 00000020 //C 0fd32 db	32 */
0x0000fd33, 0x00000020, /* 00000020 //C 0fd33 db	32 */
0x0000fd34, 0x00000020, /* 00000020 //C 0fd34 db	32 */
0x0000fd35, 0x00000020, /* 00000020 //C 0fd35 db	32 */
0x0000fd36, 0x00000020, /* 00000020 //C 0fd36 db	32 */
0x0000fd37, 0x00000020, /* 00000020 //C 0fd37 db	32 */
0x0000fd38, 0x00000020, /* 00000020 //C 0fd38 db	32 */
0x0000fd39, 0x00000020, /* 00000020 //C 0fd39 db	32 */
0x0000fd3a, 0x00000020, /* 00000020 //C 0fd3a db	32 */
0x0000fd3b, 0x00000020, /* 00000020 //C 0fd3b db	32 */
0x0000fd3c, 0x00000020, /* 00000020 //C 0fd3c db	32 */
0x0000fd3d, 0x0000004c, /* 0000004c //C 0fd3d db	76 */
0x0000fd3e, 0x0000006f, /* 0000006f //C 0fd3e db	111 */
0x0000fd3f, 0x00000061, /* 00000061 //C 0fd3f db	97 */
0x0000fd40, 0x00000064, /* 00000064 //C 0fd40 db	100 */
0x0000fd41, 0x00000020, /* 00000020 //C 0fd41 db	32 */
0x0000fd42, 0x00000068, /* 00000068 //C 0fd42 db	104 */
0x0000fd43, 0x00000065, /* 00000065 //C 0fd43 db	101 */
0x0000fd44, 0x00000078, /* 00000078 //C 0fd44 db	120 */
0x0000fd45, 0x00000020, /* 00000020 //C 0fd45 db	32 */
0x0000fd46, 0x00000066, /* 00000066 //C 0fd46 db	102 */
0x0000fd47, 0x00000069, /* 00000069 //C 0fd47 db	105 */
0x0000fd48, 0x0000006c, /* 0000006c //C 0fd48 db	108 */
0x0000fd49, 0x00000065, /* 00000065 //C 0fd49 db	101 */
0x0000fd4a, 0x00000020, /* 00000020 //C 0fd4a db	32 */
0x0000fd4b, 0x00000074, /* 00000074 //C 0fd4b db	116 */
0x0000fd4c, 0x0000006f, /* 0000006f //C 0fd4c db	111 */
0x0000fd4d, 0x00000020, /* 00000020 //C 0fd4d db	32 */
0x0000fd4e, 0x0000006d, /* 0000006d //C 0fd4e db	109 */
0x0000fd4f, 0x00000065, /* 00000065 //C 0fd4f db	101 */
0x0000fd50, 0x0000006d, /* 0000006d //C 0fd50 db	109 */
0x0000fd51, 0x0000006f, /* 0000006f //C 0fd51 db	111 */
0x0000fd52, 0x00000072, /* 00000072 //C 0fd52 db	114 */
0x0000fd53, 0x00000079, /* 00000079 //C 0fd53 db	121 */
0x0000fd54, 0x0000000a, /* 0000000a //C 0fd54 db	10 */
0x0000fd55, 0x00000000, /* 00000000 //C 0fd55 db */
0x0000fd56, 0x00000067, /* 00000067 //C 0fd56 db	103 */
0x0000fd57, 0x0000005b, /* 0000005b //C 0fd57 db	91 */
0x0000fd58, 0x0000006f, /* 0000006f //C 0fd58 db	111 */
0x0000fd59, 0x0000005d, /* 0000005d //C 0fd59 db	93 */
0x0000fd5a, 0x0000007c, /* 0000007c //C 0fd5a db	124 */
0x0000fd5b, 0x00000072, /* 00000072 //C 0fd5b db	114 */
0x0000fd5c, 0x00000075, /* 00000075 //C 0fd5c db	117 */
0x0000fd5d, 0x0000006e, /* 0000006e //C 0fd5d db	110 */
0x0000fd5e, 0x00000020, /* 00000020 //C 0fd5e db	32 */
0x0000fd5f, 0x0000005b, /* 0000005b //C 0fd5f db	91 */
0x0000fd60, 0x00000061, /* 00000061 //C 0fd60 db	97 */
0x0000fd61, 0x00000064, /* 00000064 //C 0fd61 db	100 */
0x0000fd62, 0x00000064, /* 00000064 //C 0fd62 db	100 */
0x0000fd63, 0x00000072, /* 00000072 //C 0fd63 db	114 */
0x0000fd64, 0x0000005d, /* 0000005d //C 0fd64 db	93 */
0x0000fd65, 0x00000020, /* 00000020 //C 0fd65 db	32 */
0x0000fd66, 0x00000020, /* 00000020 //C 0fd66 db	32 */
0x0000fd67, 0x00000020, /* 00000020 //C 0fd67 db	32 */
0x0000fd68, 0x00000052, /* 00000052 //C 0fd68 db	82 */
0x0000fd69, 0x00000075, /* 00000075 //C 0fd69 db	117 */
0x0000fd6a, 0x0000006e, /* 0000006e //C 0fd6a db	110 */
0x0000fd6b, 0x00000020, /* 00000020 //C 0fd6b db	32 */
0x0000fd6c, 0x00000066, /* 00000066 //C 0fd6c db	102 */
0x0000fd6d, 0x00000072, /* 00000072 //C 0fd6d db	114 */
0x0000fd6e, 0x0000006f, /* 0000006f //C 0fd6e db	111 */
0x0000fd6f, 0x0000006d, /* 0000006d //C 0fd6f db	109 */
0x0000fd70, 0x00000020, /* 00000020 //C 0fd70 db	32 */
0x0000fd71, 0x00000061, /* 00000061 //C 0fd71 db	97 */
0x0000fd72, 0x00000064, /* 00000064 //C 0fd72 db	100 */
0x0000fd73, 0x00000064, /* 00000064 //C 0fd73 db	100 */
0x0000fd74, 0x00000072, /* 00000072 //C 0fd74 db	114 */
0x0000fd75, 0x00000065, /* 00000065 //C 0fd75 db	101 */
0x0000fd76, 0x00000073, /* 00000073 //C 0fd76 db	115 */
0x0000fd77, 0x00000073, /* 00000073 //C 0fd77 db	115 */
0x0000fd78, 0x0000000a, /* 0000000a //C 0fd78 db	10 */
0x0000fd79, 0x00000000, /* 00000000 //C 0fd79 db */
0x0000fd7a, 0x00000072, /* 00000072 //C 0fd7a db	114 */
0x0000fd7b, 0x0000005b, /* 0000005b //C 0fd7b db	91 */
0x0000fd7c, 0x00000065, /* 00000065 //C 0fd7c db	101 */
0x0000fd7d, 0x00000067, /* 00000067 //C 0fd7d db	103 */
0x0000fd7e, 0x0000005b, /* 0000005b //C 0fd7e db	91 */
0x0000fd7f, 0x00000073, /* 00000073 //C 0fd7f db	115 */
0x0000fd80, 0x0000005d, /* 0000005d //C 0fd80 db	93 */
0x0000fd81, 0x0000005d, /* 0000005d //C 0fd81 db	93 */
0x0000fd82, 0x00000020, /* 00000020 //C 0fd82 db	32 */
0x0000fd83, 0x00000020, /* 00000020 //C 0fd83 db	32 */
0x0000fd84, 0x00000020, /* 00000020 //C 0fd84 db	32 */
0x0000fd85, 0x00000020, /* 00000020 //C 0fd85 db	32 */
0x0000fd86, 0x00000020, /* 00000020 //C 0fd86 db	32 */
0x0000fd87, 0x00000020, /* 00000020 //C 0fd87 db	32 */
0x0000fd88, 0x00000020, /* 00000020 //C 0fd88 db	32 */
0x0000fd89, 0x00000020, /* 00000020 //C 0fd89 db	32 */
0x0000fd8a, 0x00000020, /* 00000020 //C 0fd8a db	32 */
0x0000fd8b, 0x00000020, /* 00000020 //C 0fd8b db	32 */
0x0000fd8c, 0x00000050, /* 00000050 //C 0fd8c db	80 */
0x0000fd8d, 0x00000072, /* 00000072 //C 0fd8d db	114 */
0x0000fd8e, 0x00000069, /* 00000069 //C 0fd8e db	105 */
0x0000fd8f, 0x0000006e, /* 0000006e //C 0fd8f db	110 */
0x0000fd90, 0x00000074, /* 00000074 //C 0fd90 db	116 */
0x0000fd91, 0x00000020, /* 00000020 //C 0fd91 db	32 */
0x0000fd92, 0x00000072, /* 00000072 //C 0fd92 db	114 */
0x0000fd93, 0x00000065, /* 00000065 //C 0fd93 db	101 */
0x0000fd94, 0x00000067, /* 00000067 //C 0fd94 db	103 */
0x0000fd95, 0x00000069, /* 00000069 //C 0fd95 db	105 */
0x0000fd96, 0x00000073, /* 00000073 //C 0fd96 db	115 */
0x0000fd97, 0x00000074, /* 00000074 //C 0fd97 db	116 */
0x0000fd98, 0x00000065, /* 00000065 //C 0fd98 db	101 */
0x0000fd99, 0x00000072, /* 00000072 //C 0fd99 db	114 */
0x0000fd9a, 0x00000073, /* 00000073 //C 0fd9a db	115 */
0x0000fd9b, 0x0000000a, /* 0000000a //C 0fd9b db	10 */
0x0000fd9c, 0x00000000, /* 00000000 //C 0fd9c db */
0x0000fd9d, 0x00000072, /* 00000072 //C 0fd9d db	114 */
0x0000fd9e, 0x00000058, /* 00000058 //C 0fd9e db	88 */
0x0000fd9f, 0x00000020, /* 00000020 //C 0fd9f db	32 */
0x0000fda0, 0x0000005b, /* 0000005b //C 0fda0 db	91 */
0x0000fda1, 0x00000076, /* 00000076 //C 0fda1 db	118 */
0x0000fda2, 0x00000061, /* 00000061 //C 0fda2 db	97 */
0x0000fda3, 0x0000006c, /* 0000006c //C 0fda3 db	108 */
0x0000fda4, 0x0000005d, /* 0000005d //C 0fda4 db	93 */
0x0000fda5, 0x00000020, /* 00000020 //C 0fda5 db	32 */
0x0000fda6, 0x00000020, /* 00000020 //C 0fda6 db	32 */
0x0000fda7, 0x00000020, /* 00000020 //C 0fda7 db	32 */
0x0000fda8, 0x00000020, /* 00000020 //C 0fda8 db	32 */
0x0000fda9, 0x00000020, /* 00000020 //C 0fda9 db	32 */
0x0000fdaa, 0x00000020, /* 00000020 //C 0fdaa db	32 */
0x0000fdab, 0x00000020, /* 00000020 //C 0fdab db	32 */
0x0000fdac, 0x00000020, /* 00000020 //C 0fdac db	32 */
0x0000fdad, 0x00000020, /* 00000020 //C 0fdad db	32 */
0x0000fdae, 0x00000020, /* 00000020 //C 0fdae db	32 */
0x0000fdaf, 0x00000047, /* 00000047 //C 0fdaf db	71 */
0x0000fdb0, 0x00000065, /* 00000065 //C 0fdb0 db	101 */
0x0000fdb1, 0x00000074, /* 00000074 //C 0fdb1 db	116 */
0x0000fdb2, 0x0000002f, /* 0000002f //C 0fdb2 db	47 */
0x0000fdb3, 0x00000073, /* 00000073 //C 0fdb3 db	115 */
0x0000fdb4, 0x00000065, /* 00000065 //C 0fdb4 db	101 */
0x0000fdb5, 0x00000074, /* 00000074 //C 0fdb5 db	116 */
0x0000fdb6, 0x00000020, /* 00000020 //C 0fdb6 db	32 */
0x0000fdb7, 0x00000052, /* 00000052 //C 0fdb7 db	82 */
0x0000fdb8, 0x00000058, /* 00000058 //C 0fdb8 db	88 */
0x0000fdb9, 0x0000000a, /* 0000000a //C 0fdb9 db	10 */
0x0000fdba, 0x00000000, /* 00000000 //C 0fdba db */
0x0000fdbb, 0x00000073, /* 00000073 //C 0fdbb db	115 */
0x0000fdbc, 0x00000070, /* 00000070 //C 0fdbc db	112 */
0x0000fdbd, 0x00000020, /* 00000020 //C 0fdbd db	32 */
0x0000fdbe, 0x0000005b, /* 0000005b //C 0fdbe db	91 */
0x0000fdbf, 0x00000076, /* 00000076 //C 0fdbf db	118 */
0x0000fdc0, 0x00000061, /* 00000061 //C 0fdc0 db	97 */
0x0000fdc1, 0x0000006c, /* 0000006c //C 0fdc1 db	108 */
0x0000fdc2, 0x0000005d, /* 0000005d //C 0fdc2 db	93 */
0x0000fdc3, 0x00000020, /* 00000020 //C 0fdc3 db	32 */
0x0000fdc4, 0x00000020, /* 00000020 //C 0fdc4 db	32 */
0x0000fdc5, 0x00000020, /* 00000020 //C 0fdc5 db	32 */
0x0000fdc6, 0x00000020, /* 00000020 //C 0fdc6 db	32 */
0x0000fdc7, 0x00000020, /* 00000020 //C 0fdc7 db	32 */
0x0000fdc8, 0x00000020, /* 00000020 //C 0fdc8 db	32 */
0x0000fdc9, 0x00000020, /* 00000020 //C 0fdc9 db	32 */
0x0000fdca, 0x00000020, /* 00000020 //C 0fdca db	32 */
0x0000fdcb, 0x00000020, /* 00000020 //C 0fdcb db	32 */
0x0000fdcc, 0x00000020, /* 00000020 //C 0fdcc db	32 */
0x0000fdcd, 0x00000047, /* 00000047 //C 0fdcd db	71 */
0x0000fdce, 0x00000065, /* 00000065 //C 0fdce db	101 */
0x0000fdcf, 0x00000074, /* 00000074 //C 0fdcf db	116 */
0x0000fdd0, 0x0000002f, /* 0000002f //C 0fdd0 db	47 */
0x0000fdd1, 0x00000073, /* 00000073 //C 0fdd1 db	115 */
0x0000fdd2, 0x00000065, /* 00000065 //C 0fdd2 db	101 */
0x0000fdd3, 0x00000074, /* 00000074 //C 0fdd3 db	116 */
0x0000fdd4, 0x00000020, /* 00000020 //C 0fdd4 db	32 */
0x0000fdd5, 0x00000052, /* 00000052 //C 0fdd5 db	82 */
0x0000fdd6, 0x00000031, /* 00000031 //C 0fdd6 db	49 */
0x0000fdd7, 0x00000033, /* 00000033 //C 0fdd7 db	51 */
0x0000fdd8, 0x0000000a, /* 0000000a //C 0fdd8 db	10 */
0x0000fdd9, 0x00000000, /* 00000000 //C 0fdd9 db */
0x0000fdda, 0x0000006c, /* 0000006c //C 0fdda db	108 */
0x0000fddb, 0x00000072, /* 00000072 //C 0fddb db	114 */
0x0000fddc, 0x00000020, /* 00000020 //C 0fddc db	32 */
0x0000fddd, 0x0000005b, /* 0000005b //C 0fddd db	91 */
0x0000fdde, 0x00000076, /* 00000076 //C 0fdde db	118 */
0x0000fddf, 0x00000061, /* 00000061 //C 0fddf db	97 */
0x0000fde0, 0x0000006c, /* 0000006c //C 0fde0 db	108 */
0x0000fde1, 0x0000005d, /* 0000005d //C 0fde1 db	93 */
0x0000fde2, 0x00000020, /* 00000020 //C 0fde2 db	32 */
0x0000fde3, 0x00000020, /* 00000020 //C 0fde3 db	32 */
0x0000fde4, 0x00000020, /* 00000020 //C 0fde4 db	32 */
0x0000fde5, 0x00000020, /* 00000020 //C 0fde5 db	32 */
0x0000fde6, 0x00000020, /* 00000020 //C 0fde6 db	32 */
0x0000fde7, 0x00000020, /* 00000020 //C 0fde7 db	32 */
0x0000fde8, 0x00000020, /* 00000020 //C 0fde8 db	32 */
0x0000fde9, 0x00000020, /* 00000020 //C 0fde9 db	32 */
0x0000fdea, 0x00000020, /* 00000020 //C 0fdea db	32 */
0x0000fdeb, 0x00000020, /* 00000020 //C 0fdeb db	32 */
0x0000fdec, 0x00000047, /* 00000047 //C 0fdec db	71 */
0x0000fded, 0x00000065, /* 00000065 //C 0fded db	101 */
0x0000fdee, 0x00000074, /* 00000074 //C 0fdee db	116 */
0x0000fdef, 0x0000002f, /* 0000002f //C 0fdef db	47 */
0x0000fdf0, 0x00000073, /* 00000073 //C 0fdf0 db	115 */
0x0000fdf1, 0x00000065, /* 00000065 //C 0fdf1 db	101 */
0x0000fdf2, 0x00000074, /* 00000074 //C 0fdf2 db	116 */
0x0000fdf3, 0x00000020, /* 00000020 //C 0fdf3 db	32 */
0x0000fdf4, 0x00000052, /* 00000052 //C 0fdf4 db	82 */
0x0000fdf5, 0x00000031, /* 00000031 //C 0fdf5 db	49 */
0x0000fdf6, 0x00000034, /* 00000034 //C 0fdf6 db	52 */
0x0000fdf7, 0x0000000a, /* 0000000a //C 0fdf7 db	10 */
0x0000fdf8, 0x00000000, /* 00000000 //C 0fdf8 db */
0x0000fdf9, 0x00000070, /* 00000070 //C 0fdf9 db	112 */
0x0000fdfa, 0x00000063, /* 00000063 //C 0fdfa db	99 */
0x0000fdfb, 0x00000020, /* 00000020 //C 0fdfb db	32 */
0x0000fdfc, 0x0000005b, /* 0000005b //C 0fdfc db	91 */
0x0000fdfd, 0x00000076, /* 00000076 //C 0fdfd db	118 */
0x0000fdfe, 0x00000061, /* 00000061 //C 0fdfe db	97 */
0x0000fdff, 0x0000006c, /* 0000006c //C 0fdff db	108 */
0x0000fe00, 0x0000005d, /* 0000005d //C 0fe00 db	93 */
0x0000fe01, 0x00000020, /* 00000020 //C 0fe01 db	32 */
0x0000fe02, 0x00000020, /* 00000020 //C 0fe02 db	32 */
0x0000fe03, 0x00000020, /* 00000020 //C 0fe03 db	32 */
0x0000fe04, 0x00000020, /* 00000020 //C 0fe04 db	32 */
0x0000fe05, 0x00000020, /* 00000020 //C 0fe05 db	32 */
0x0000fe06, 0x00000020, /* 00000020 //C 0fe06 db	32 */
0x0000fe07, 0x00000020, /* 00000020 //C 0fe07 db	32 */
0x0000fe08, 0x00000020, /* 00000020 //C 0fe08 db	32 */
0x0000fe09, 0x00000020, /* 00000020 //C 0fe09 db	32 */
0x0000fe0a, 0x00000020, /* 00000020 //C 0fe0a db	32 */
0x0000fe0b, 0x00000047, /* 00000047 //C 0fe0b db	71 */
0x0000fe0c, 0x00000065, /* 00000065 //C 0fe0c db	101 */
0x0000fe0d, 0x00000074, /* 00000074 //C 0fe0d db	116 */
0x0000fe0e, 0x0000002f, /* 0000002f //C 0fe0e db	47 */
0x0000fe0f, 0x00000073, /* 00000073 //C 0fe0f db	115 */
0x0000fe10, 0x00000065, /* 00000065 //C 0fe10 db	101 */
0x0000fe11, 0x00000074, /* 00000074 //C 0fe11 db	116 */
0x0000fe12, 0x00000020, /* 00000020 //C 0fe12 db	32 */
0x0000fe13, 0x00000052, /* 00000052 //C 0fe13 db	82 */
0x0000fe14, 0x00000031, /* 00000031 //C 0fe14 db	49 */
0x0000fe15, 0x00000035, /* 00000035 //C 0fe15 db	53 */
0x0000fe16, 0x0000000a, /* 0000000a //C 0fe16 db	10 */
0x0000fe17, 0x00000000, /* 00000000 //C 0fe17 db */
0x0000fe18, 0x00000066, /* 00000066 //C 0fe18 db	102 */
0x0000fe19, 0x00000020, /* 00000020 //C 0fe19 db	32 */
0x0000fe1a, 0x0000005b, /* 0000005b //C 0fe1a db	91 */
0x0000fe1b, 0x00000076, /* 00000076 //C 0fe1b db	118 */
0x0000fe1c, 0x00000061, /* 00000061 //C 0fe1c db	97 */
0x0000fe1d, 0x0000006c, /* 0000006c //C 0fe1d db	108 */
0x0000fe1e, 0x0000005d, /* 0000005d //C 0fe1e db	93 */
0x0000fe1f, 0x00000020, /* 00000020 //C 0fe1f db	32 */
0x0000fe20, 0x00000020, /* 00000020 //C 0fe20 db	32 */
0x0000fe21, 0x00000020, /* 00000020 //C 0fe21 db	32 */
0x0000fe22, 0x00000020, /* 00000020 //C 0fe22 db	32 */
0x0000fe23, 0x00000020, /* 00000020 //C 0fe23 db	32 */
0x0000fe24, 0x00000020, /* 00000020 //C 0fe24 db	32 */
0x0000fe25, 0x00000020, /* 00000020 //C 0fe25 db	32 */
0x0000fe26, 0x00000020, /* 00000020 //C 0fe26 db	32 */
0x0000fe27, 0x00000020, /* 00000020 //C 0fe27 db	32 */
0x0000fe28, 0x00000020, /* 00000020 //C 0fe28 db	32 */
0x0000fe29, 0x00000020, /* 00000020 //C 0fe29 db	32 */
0x0000fe2a, 0x00000047, /* 00000047 //C 0fe2a db	71 */
0x0000fe2b, 0x00000065, /* 00000065 //C 0fe2b db	101 */
0x0000fe2c, 0x00000074, /* 00000074 //C 0fe2c db	116 */
0x0000fe2d, 0x0000002f, /* 0000002f //C 0fe2d db	47 */
0x0000fe2e, 0x00000073, /* 00000073 //C 0fe2e db	115 */
0x0000fe2f, 0x00000065, /* 00000065 //C 0fe2f db	101 */
0x0000fe30, 0x00000074, /* 00000074 //C 0fe30 db	116 */
0x0000fe31, 0x00000020, /* 00000020 //C 0fe31 db	32 */
0x0000fe32, 0x00000066, /* 00000066 //C 0fe32 db	102 */
0x0000fe33, 0x0000006c, /* 0000006c //C 0fe33 db	108 */
0x0000fe34, 0x00000061, /* 00000061 //C 0fe34 db	97 */
0x0000fe35, 0x00000067, /* 00000067 //C 0fe35 db	103 */
0x0000fe36, 0x00000073, /* 00000073 //C 0fe36 db	115 */
0x0000fe37, 0x0000000a, /* 0000000a //C 0fe37 db	10 */
0x0000fe38, 0x00000000, /* 00000000 //C 0fe38 db */
0x0000fe39, 0x00000068, /* 00000068 //C 0fe39 db	104 */
0x0000fe3a, 0x0000002c, /* 0000002c //C 0fe3a db	44 */
0x0000fe3b, 0x0000003f, /* 0000003f //C 0fe3b db	63 */
0x0000fe3c, 0x00000020, /* 00000020 //C 0fe3c db	32 */
0x0000fe3d, 0x00000020, /* 00000020 //C 0fe3d db	32 */
0x0000fe3e, 0x00000020, /* 00000020 //C 0fe3e db	32 */
0x0000fe3f, 0x00000020, /* 00000020 //C 0fe3f db	32 */
0x0000fe40, 0x00000020, /* 00000020 //C 0fe40 db	32 */
0x0000fe41, 0x00000020, /* 00000020 //C 0fe41 db	32 */
0x0000fe42, 0x00000020, /* 00000020 //C 0fe42 db	32 */
0x0000fe43, 0x00000020, /* 00000020 //C 0fe43 db	32 */
0x0000fe44, 0x00000020, /* 00000020 //C 0fe44 db	32 */
0x0000fe45, 0x00000020, /* 00000020 //C 0fe45 db	32 */
0x0000fe46, 0x00000020, /* 00000020 //C 0fe46 db	32 */
0x0000fe47, 0x00000020, /* 00000020 //C 0fe47 db	32 */
0x0000fe48, 0x00000020, /* 00000020 //C 0fe48 db	32 */
0x0000fe49, 0x00000020, /* 00000020 //C 0fe49 db	32 */
0x0000fe4a, 0x00000020, /* 00000020 //C 0fe4a db	32 */
0x0000fe4b, 0x00000048, /* 00000048 //C 0fe4b db	72 */
0x0000fe4c, 0x00000065, /* 00000065 //C 0fe4c db	101 */
0x0000fe4d, 0x0000006c, /* 0000006c //C 0fe4d db	108 */
0x0000fe4e, 0x00000070, /* 00000070 //C 0fe4e db	112 */
0x0000fe4f, 0x0000000a, /* 0000000a //C 0fe4f db	10 */
0x0000fe50, 0x00000000, /* 00000000 //C 0fe50 db */
0x0000fe51, 0x00000000, /* 00000000 //C 0fe51 dd	0 */
0x0000ff53, 0x00000045, /* 00000045 //C 0ff53 db	69 */
0x0000ff54, 0x0000004f, /* 0000004f //C 0ff54 db	79 */
0x0000ff55, 0x00000046, /* 00000046 //C 0ff55 db	70 */
0x0000ff56, 0x00000020, /* 00000020 //C 0ff56 db	32 */
0x0000ff57, 0x00000050, /* 00000050 //C 0ff57 db	80 */
0x0000ff58, 0x0000004d, /* 0000004d //C 0ff58 db	77 */
0x0000ff59, 0x0000006f, /* 0000006f //C 0ff59 db	111 */
0x0000ff5a, 0x0000006e, /* 0000006e //C 0ff5a db	110 */
0x0000ff5b, 0x00000069, /* 00000069 //C 0ff5b db	105 */
0x0000ff5c, 0x00000074, /* 00000074 //C 0ff5c db	116 */
0x0000ff5d, 0x0000006f, /* 0000006f //C 0ff5d db	111 */
0x0000ff5e, 0x00000072, /* 00000072 //C 0ff5e db	114 */
0x0000ff5f, 0x0000000a, /* 0000000a //C 0ff5f db	10 */
0x0000ff60, 0x00000000, /* 00000000 //C 0ff60 db */
0xffffffff, 0xffffffff
};
