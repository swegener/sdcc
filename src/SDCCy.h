typedef union {
    symbol     *sym ;      /* symbol table pointer       */
    structdef  *sdef;      /* structure definition       */
    char       yychar[SDCC_NAME_MAX+1];
    link       *lnk ;      /* declarator  or specifier   */
    int        yyint;      /* integer value returned     */
    value      *val ;      /* for integer constant       */
    initList   *ilist;     /* initial list               */
    char       yyinline[MAX_INLINEASM]; /* inlined assembler code */
    ast       *asts;     /* expression tree            */
} YYSTYPE;
#define	IDENTIFIER	257
#define	TYPE_NAME	258
#define	CONSTANT	259
#define	STRING_LITERAL	260
#define	SIZEOF	261
#define	PTR_OP	262
#define	INC_OP	263
#define	DEC_OP	264
#define	LEFT_OP	265
#define	RIGHT_OP	266
#define	LE_OP	267
#define	GE_OP	268
#define	EQ_OP	269
#define	NE_OP	270
#define	AND_OP	271
#define	OR_OP	272
#define	MUL_ASSIGN	273
#define	DIV_ASSIGN	274
#define	MOD_ASSIGN	275
#define	ADD_ASSIGN	276
#define	SUB_ASSIGN	277
#define	LEFT_ASSIGN	278
#define	RIGHT_ASSIGN	279
#define	AND_ASSIGN	280
#define	XOR_ASSIGN	281
#define	OR_ASSIGN	282
#define	TYPEDEF	283
#define	EXTERN	284
#define	STATIC	285
#define	AUTO	286
#define	REGISTER	287
#define	CODE	288
#define	INTERRUPT	289
#define	SFR	290
#define	AT	291
#define	SBIT	292
#define	REENTRANT	293
#define	USING	294
#define	XDATA	295
#define	DATA	296
#define	IDATA	297
#define	PDATA	298
#define	VAR_ARGS	299
#define	CRITICAL	300
#define	CHAR	301
#define	SHORT	302
#define	INT	303
#define	LONG	304
#define	SIGNED	305
#define	UNSIGNED	306
#define	FLOAT	307
#define	DOUBLE	308
#define	CONST	309
#define	VOLATILE	310
#define	VOID	311
#define	BIT	312
#define	STRUCT	313
#define	UNION	314
#define	ENUM	315
#define	ELIPSIS	316
#define	RANGE	317
#define	FAR	318
#define	_XDATA	319
#define	_CODE	320
#define	_GENERIC	321
#define	_NEAR	322
#define	_PDATA	323
#define	_IDATA	324
#define	CASE	325
#define	DEFAULT	326
#define	IF	327
#define	ELSE	328
#define	SWITCH	329
#define	WHILE	330
#define	DO	331
#define	FOR	332
#define	GOTO	333
#define	CONTINUE	334
#define	BREAK	335
#define	RETURN	336
#define	INLINEASM	337
#define	IFX	338
#define	ADDRESS_OF	339
#define	GET_VALUE_AT_ADDRESS	340
#define	SPIL	341
#define	UNSPIL	342
#define	GETHBIT	343
#define	BITWISEAND	344
#define	UNARYMINUS	345
#define	IPUSH	346
#define	IPOP	347
#define	PCALL	348
#define	ENDFUNCTION	349
#define	JUMPTABLE	350
#define	RRC	351
#define	RLC	352
#define	CAST	353
#define	CALL	354
#define	PARAM	355
#define	NULLOP	356
#define	BLOCK	357
#define	LABEL	358
#define	RECEIVE	359
#define	SEND	360


extern YYSTYPE yylval;
