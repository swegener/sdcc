/*-----------------------------------------------------------------------
  SDCC.lex - lexical analyser for use with sdcc ( a freeware compiler for
  8/16 bit microcontrollers)
  Written by : Sandeep Dutta . sandeep.dutta@usa.net (1997)

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

D       [0-9]
L       [a-zA-Z_]
H       [a-fA-F0-9]
E       [Ee][+-]?{D}+
FS      (f|F|l|L)
IS      (u|U|l|L)*

%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "newalloc.h"
#include "dbuf.h"

#define TKEYWORD(token) return (isTargetKeyword(yytext) ? token :\
                                check_type())

#define TKEYWORDSDCC(token) return (options.std_sdcc && isTargetKeyword(yytext)\
                                    ? token : check_type())

#define TKEYWORD99(token) return (options.std_c99 ? token : check_type())

extern int lineno, column;
extern char *filename;

/* global definitions */
char *currFname;
int mylineno = 1;

/* local definitions */
static struct dbuf_s asmbuff;

/* forward declarations */
static char *stringLiteral(void);
static void count(void);
static int process_pragma(const char *);
static int check_type(void);
static int isTargetKeyword(char *s);
static int checkCurrFile(char *s);
%}

%x asm
%%
_?"_asm"         {
  count();
  if (!options.std_sdcc && yytext[1] != '_')
    return check_type();
  assert(asmbuff.alloc == 0 && asmbuff.len == 0 && asmbuff.buf == NULL);
  dbuf_init(&asmbuff, INITIAL_INLINEASM);
  BEGIN(asm);
}
<asm>_?"_endasm" {
  count();
  if (!options.std_sdcc && yytext[1] != '_')
    {
      dbuf_append(&asmbuff, yytext, strlen(yytext));
    }
  else
    {
      yylval.yyinline = dbuf_c_str(&asmbuff);
      dbuf_detach(&asmbuff);
      BEGIN(INITIAL);
      return (INLINEASM);
    }
}
<asm>\n        {
  count();
  dbuf_append(&asmbuff, yytext, 1);
}
<asm>.         {
  dbuf_append(&asmbuff, yytext, 1);
}
"at"           { count(); TKEYWORDSDCC(AT); }
"__at"         { count(); TKEYWORD(AT); }
"auto"         { count(); return(AUTO); }
"bit"          { count(); TKEYWORDSDCC(BIT); }
"__bit"        { count(); TKEYWORD(BIT); }
"break"        { count(); return(BREAK); }
"case"         { count(); return(CASE); }
"char"         { count(); return(CHAR); }
"code"         { count(); TKEYWORDSDCC(CODE); }
"__code"       { count(); TKEYWORD(CODE); }
"const"        { count(); return(CONST); }
"continue"     { count(); return(CONTINUE); }
"critical"     { count(); TKEYWORDSDCC(CRITICAL); }
"__critical"   { count(); TKEYWORD(CRITICAL); }
"data"         { count(); TKEYWORDSDCC(DATA); }
"__data"       { count(); TKEYWORD(DATA); }
"default"      { count(); return(DEFAULT); }
"do"           { count(); return(DO); }
"double"       { count(); werror(W_DOUBLE_UNSUPPORTED);return(FLOAT); }
"else"         { count(); return(ELSE); }
"enum"         { count(); return(ENUM); }
"extern"       { count(); return(EXTERN); }
"far"          { count(); TKEYWORDSDCC(XDATA); }
"__far"        { count(); TKEYWORD(XDATA); }
"eeprom"       { count(); TKEYWORDSDCC(EEPROM); }
"__eeprom"     { count(); TKEYWORD(EEPROM); }
"float"        { count(); return(FLOAT); }
"fixed16x16"   { count(); TKEYWORDSDCC(FIXED16X16); }
"__fixed16x16"   { count(); TKEYWORD(FIXED16X16); }
"flash"        { count(); TKEYWORDSDCC(CODE); }
"__flash"      { count(); TKEYWORD(CODE); }
"for"          { count(); return(FOR); }
"goto"         { count(); return(GOTO); }
"idata"        { count(); TKEYWORDSDCC(IDATA); }
"__idata"      { count(); TKEYWORD(IDATA); }
"if"           { count(); return(IF); }
"int"          { count(); return(INT); }
"interrupt"    { count(); TKEYWORDSDCC(INTERRUPT); }
"__interrupt"  { count(); TKEYWORD(INTERRUPT); }
"nonbanked"    { count(); TKEYWORDSDCC(NONBANKED); }
"__nonbanked"  { count(); TKEYWORD(NONBANKED); }
"banked"       { count(); TKEYWORDSDCC(BANKED); }
"__banked"     { count(); TKEYWORD(BANKED); }
"long"         { count(); return(LONG); }
"near"         { count(); TKEYWORDSDCC(DATA); }
"__near"       { count(); TKEYWORD(DATA); }
"pdata"        { count(); TKEYWORDSDCC(PDATA); }
"__pdata"      { count(); TKEYWORD(PDATA); }
"reentrant"    { count(); TKEYWORDSDCC(REENTRANT); }
"__reentrant"  { count(); TKEYWORD(REENTRANT); }
"shadowregs"   { count(); TKEYWORDSDCC(SHADOWREGS); }
"__shadowregs" { count(); TKEYWORD(SHADOWREGS); }
"wparam"       { count(); TKEYWORDSDCC(WPARAM); }
"__wparam"     { count(); TKEYWORD(WPARAM); }
"register"     { count(); return(REGISTER); }
"return"       { count(); return(RETURN); }
"sfr"          { count(); TKEYWORDSDCC(SFR); }
"__sfr"        { count(); TKEYWORD(SFR); }
"sfr16"        { count(); TKEYWORDSDCC(SFR16); }
"__sfr16"      { count(); TKEYWORD(SFR16); }
"sfr32"        { count(); TKEYWORDSDCC(SFR32); }
"__sfr32"      { count(); TKEYWORD(SFR32); }
"sbit"         { count(); TKEYWORDSDCC(SBIT); }
"__sbit"       { count(); TKEYWORD(SBIT); }
"short"        { count(); return(SHORT); }
"signed"       { count(); return(SIGNED); }
"sizeof"       { count(); return(SIZEOF); }
"sram"         { count(); TKEYWORDSDCC(XDATA); }
"__sram"       { count(); TKEYWORD(XDATA); }
"static"       { count(); return(STATIC); }
"struct"       { count(); return(STRUCT); }
"switch"       { count(); return(SWITCH); }
"typedef"      { count(); return(TYPEDEF); }
"union"        { count(); return(UNION); }
"unsigned"     { count(); return(UNSIGNED); }
"void"         { count(); return(VOID); }
"volatile"     { count(); return(VOLATILE); }
"using"        { count(); TKEYWORDSDCC(USING); }
"__using"      { count(); TKEYWORD(USING); }
"_naked"       { count(); TKEYWORDSDCC(NAKED); }
"__naked"      { count(); TKEYWORD(NAKED); }
"while"        { count(); return(WHILE); }
"xdata"        { count(); TKEYWORDSDCC(XDATA); }
"__xdata"      { count(); TKEYWORD(XDATA); }
"..."          { count(); return(VAR_ARGS); }
"__typeof"     { count(); return TYPEOF; }
"_JavaNative"  { count(); TKEYWORD(JAVANATIVE); }
"_overlay"     { count(); TKEYWORDSDCC(OVERLAY); }
"__overlay"    { count(); TKEYWORD(OVERLAY); }
"inline"       { count(); TKEYWORD99(INLINE); }
"restrict"     { count(); TKEYWORD99(RESTRICT); }
{L}({L}|{D})*  { count(); return(check_type()); }
0[xX]{H}+{IS}? { count(); yylval.val = constVal(yytext); return(CONSTANT); }
0[0-7]*{IS}?     { count(); yylval.val = constVal(yytext); return(CONSTANT); }
[1-9]{D}*{IS}?      { count(); yylval.val = constVal(yytext); return(CONSTANT); }
'(\\.|[^\\'])+' { count();yylval.val = charVal (yytext); return(CONSTANT); /* ' make syntax highliter happy */ }
{D}+{E}{FS}?   { count(); yylval.val = constFloatVal(yytext);return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?  { count(); yylval.val = constFloatVal(yytext);return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?  { count(); yylval.val = constFloatVal(yytext);return(CONSTANT); }
\"             { count(); yylval.val=strVal(stringLiteral()); return(STRING_LITERAL); }
">>=" { count(); yylval.yyint = RIGHT_ASSIGN ; return(RIGHT_ASSIGN); }
"<<=" { count(); yylval.yyint = LEFT_ASSIGN  ; return(LEFT_ASSIGN); }
"+="  { count(); yylval.yyint = ADD_ASSIGN   ; return(ADD_ASSIGN); }
"-="  { count(); yylval.yyint = SUB_ASSIGN   ; return(SUB_ASSIGN); }
"*="  { count(); yylval.yyint = MUL_ASSIGN   ; return(MUL_ASSIGN); }
"/="  { count(); yylval.yyint = DIV_ASSIGN   ; return(DIV_ASSIGN); }
"%="  { count(); yylval.yyint = MOD_ASSIGN   ; return(MOD_ASSIGN); }
"&="  { count(); yylval.yyint = AND_ASSIGN   ; return(AND_ASSIGN); }
"^="  { count(); yylval.yyint = XOR_ASSIGN   ; return(XOR_ASSIGN); }
"|="  { count(); yylval.yyint = OR_ASSIGN    ; return(OR_ASSIGN); }
">>"           { count(); return(RIGHT_OP); }
"<<"           { count(); return(LEFT_OP); }
"++"           { count(); return(INC_OP); }
"--"           { count(); return(DEC_OP); }
"->"           { count(); return(PTR_OP); }
"&&"           { count(); return(AND_OP); }
"||"           { count(); return(OR_OP); }
"<="           { count(); return(LE_OP); }
">="           { count(); return(GE_OP); }
"=="           { count(); return(EQ_OP); }
"!="           { count(); return(NE_OP); }
";"            { count(); return(';'); }
"{"            { count(); NestLevel++ ; ignoreTypedefType = 0; return('{'); }
"}"            { count(); NestLevel--; return('}'); }
","            { count(); return(','); }
":"            { count(); return(':'); }
"="            { count(); return('='); }
"("            { count(); ignoreTypedefType = 0; return('('); }
")"            { count(); return(')'); }
"["            { count(); return('['); }
"]"            { count(); return(']'); }
"."            { count(); return('.'); }
"&"            { count(); return('&'); }
"!"            { count(); return('!'); }
"~"            { count(); return('~'); }
"-"            { count(); return('-'); }
"+"            { count(); return('+'); }
"*"            { count(); return('*'); }
"/"            { count(); return('/'); }
"%"            { count(); return('%'); }
"<"            { count(); return('<'); }
">"            { count(); return('>'); }
"^"            { count(); return('^'); }
"|"            { count(); return('|'); }
"?"            { count(); return('?'); }
^#pragma.*$    { count(); process_pragma(yytext); }
^(#line.*"\n")|(#.*"\n") { count(); checkCurrFile(yytext); }

^[^(]+"("[0-9]+") : error"[^\n]+ { werror(E_PRE_PROC_FAILED, yytext); count(); }
^[^(]+"("[0-9]+") : warning"[^\n]+ { werror(W_PRE_PROC_WARNING, yytext); count(); }
"\r\n"         { count(); }
"\n"           { count(); }
[ \t\v\f]      { count(); }
\\ {
  int ch = input();
  if (ch != '\n') {
    /* that could have been removed by the preprocessor anyway */
    werror (W_STRAY_BACKSLASH, column);
    unput(ch);
  }
}
.              { count(); }
%%

/* flex 2.5.31 undefines yytext_ptr, so we have to define it again */
#ifndef yytext_ptr
#define yytext_ptr yytext
#endif


static int checkCurrFile (char *s)
{
    int  lNum;
    char *tptr;

    /* skip '#' character */
    if (*s++ != '#')
      return 0;

    /* check if this is a #line
       this is not standard and can be removed in the future */
#define LINE_STR  "line"
#define LINE_LEN  ((sizeof LINE_STR) - 1)

    if (strncmp(s, LINE_STR, LINE_LEN) == 0)
      s += LINE_LEN;

    /* get the line number */
    lNum = strtol(s, &tptr, 10);
    if (tptr == s || !isspace((unsigned char)*tptr))
      return 0;
    s = tptr;

    /* now see if we have a file name */
    while (*s != '\"' && *s)
      s++;

    /* if we don't have a filename then */
    /* set the current line number to   */
    /* line number if printFlag is on   */
    if (!*s) {
      lineno = mylineno = lNum;
      return 0;
    }

    /* if we have a filename then check */
    /* if it is "standard in" if yes then */
    /* get the currentfile name info    */
    s++ ;

    /* in c1mode fullSrcFileName is NULL */
    if (fullSrcFileName &&
         strncmp(s, fullSrcFileName, strlen(fullSrcFileName)) == 0) {
      lineno = mylineno = lNum;
      currFname = fullSrcFileName;
    } else {
        char *sb = s;
        /* mark the end of the filename */
        while (*s != '"') s++;
        *s = '\0';
        currFname = strdup (sb);
        lineno = mylineno = lNum;
    }
    filename = currFname ;
    return 0;
}

int column = 0;
int plineIdx =0;

static void count(void)
{
  int i;
  for (i = 0; yytext[i] != '\0'; i++) {
    if (yytext[i] == '\n') {
      column = 0;
      lineno = ++mylineno;
    }
    else
      if (yytext[i] == '\t')
        column += 8 - (column % 8);
      else
        column++;
  }
  /* ECHO; */
}

static int check_type(void)
{
  symbol *sym = findSym(SymbolTab, NULL, yytext);

  /* check if it is in the table as a typedef */
  if (!ignoreTypedefType && sym && IS_SPEC (sym->etype)
      && SPEC_TYPEDEF (sym->etype)) {
    strncpyz(yylval.yychar, yytext, SDCC_NAME_MAX);
    return (TYPE_NAME);
  }
  else {
    strncpyz (yylval.yychar, yytext, SDCC_NAME_MAX);
    return(IDENTIFIER);
  }
}

/*
 * Change by JTV 2001-05-19 to not concantenate strings
 * to support ANSI hex and octal escape sequences in string literals
 */

static char *stringLiteral(void)
{
#define STR_BUF_CHUNCK_LEN  1024
  int ch;
  static struct dbuf_s dbuf;
  char buf[2];

  if (dbuf.alloc == 0)
    dbuf_init(&dbuf, STR_BUF_CHUNCK_LEN);
  else
    dbuf_set_size(&dbuf, 0);

  dbuf_append(&dbuf, "\"", 1);
  /* put into the buffer till we hit the first \" */

  while ((ch = input()) != 0) {
    switch (ch) {
    case '\\':
      /* if it is a \ then escape char's are allowed */
      ch = input();
      if (ch == '\n') {
        /* \<newline> is a continuator */
        lineno = ++mylineno;
        column = 0;
      }
      else {
        buf[0] = '\\';
        buf[1] = ch;
        dbuf_append(&dbuf, buf, 2); /* get the escape char, no further check */
      }
      break; /* carry on */

    case '\n':
      /* if new line we have a new line break, which is illegal */
      werror(W_NEWLINE_IN_STRING);
      dbuf_append(&dbuf, "\n", 1);
      lineno = ++mylineno;
      column = 0;
      break;

    case '"':
      /* if this is a quote then we have work to do */
      /* find the next non whitespace character     */
      /* if that is a double quote then carry on    */
      dbuf_append(&dbuf, "\"", 1);  /* Pass end of this string or substring to evaluator */
      while ((ch = input()) && (isspace(ch) || ch == '\\')) {
        switch (ch) {
        case '\\':
          if ((ch = input()) != '\n') {
            werror(W_STRAY_BACKSLASH, column);
            unput(ch);
          }
          else {
            lineno = ++mylineno;
            column = 0;
          }
          break;

        case '\n':
          mylineno++;
          break;
        }
      }

      if (!ch)
        goto out;

      if (ch != '\"') {
        unput(ch);
        goto out;
      }
      break;

    default:
      buf[0] = ch;
      dbuf_append(&dbuf, buf, 1);  /* Put next substring introducer into output string */
    }
  }

out:
  return (char *)dbuf_c_str(&dbuf);
}


enum {
     P_SAVE = 1,
     P_RESTORE,
     P_NOINDUCTION,
     P_NOINVARIANT,
     P_INDUCTION,
     P_STACKAUTO,
     P_NOJTBOUND,
     P_NOOVERLAY,
     P_LESSPEDANTIC,
     P_NOGCSE,
     P_CALLEE_SAVES,
     P_EXCLUDE,
     P_NOIV,
     P_LOOPREV,
     P_OVERLAY_,     /* I had a strange conflict with P_OVERLAY while */
                     /* cross-compiling for MINGW32 with gcc 3.2 */
     P_DISABLEWARN,
     P_OPTCODESPEED,
     P_OPTCODESIZE,
     P_OPTCODEBALANCED,
     P_STD_C89,
     P_STD_C99,
     P_STD_SDCC89,
     P_STD_SDCC99,
     P_CODESEG,
     P_CONSTSEG
};


/* SAVE/RESTORE stack */
#define SAVE_RESTORE_SIZE 128

STACK_DCL(options_stack, struct options *, SAVE_RESTORE_SIZE)
STACK_DCL(optimize_stack, struct optimize *, SAVE_RESTORE_SIZE)
STACK_DCL(SDCCERRG_stack, struct SDCCERRG *, SAVE_RESTORE_SIZE)

/*
 * cloneXxx functions should be updated every time a new set is
 * added to the options or optimize structure!
 */

static struct options *cloneOptions(struct options *opt)
{
  struct options *new_opt;

  new_opt = Safe_malloc(sizeof (struct options));

  /* clone scalar values */
  *new_opt = *opt;

  /* clone sets */
  new_opt->calleeSavesSet = setFromSetNonRev(opt->calleeSavesSet);
  new_opt->excludeRegsSet = setFromSetNonRev(opt->excludeRegsSet);
  /* not implemented yet: */
  /* new_opt->olaysSet = setFromSetNonRev(opt->olaysSet); */

  return new_opt;
}

static struct optimize *cloneOptimize(struct optimize *opt)
{
  struct optimize *new_opt;

  new_opt = Safe_malloc(sizeof (struct optimize));

  /* clone scalar values */
  *new_opt = *opt;

  return new_opt;
}

static struct SDCCERRG *cloneSDCCERRG (struct SDCCERRG *val)
{
  struct SDCCERRG *new_val;

  new_val = Safe_malloc(sizeof (struct SDCCERRG));

  /* clone scalar values */
  *new_val = *val;

  return new_val;
}

static void copyAndFreeOptions(struct options *dest, struct options *src)
{
  /* delete dest sets */
  deleteSet(&dest->calleeSavesSet);
  deleteSet(&dest->excludeRegsSet);
  /* not implemented yet: */
  /* deleteSet(&dest->olaysSet); */

  /* copy src to dest */
  *dest = *src;

  Safe_free(src);
}

static void copyAndFreeOptimize(struct optimize *dest, struct optimize *src)
{
  /* copy src to dest */
  *dest = *src;

  Safe_free(src);
}

static void copyAndFreeSDCCERRG(struct SDCCERRG *dest, struct SDCCERRG *src)
{
  /* copy src to dest */
  *dest = *src;

  Safe_free(src);
}

/*
 * returns 1 if the pragma was processed, 0 if not
 */
static int doPragma(int id, const char *name, const char *cp)
{
  struct pragma_token_s token;
  int err = 0;
  int processed = 1;

  init_pragma_token(&token);

  switch (id) 
    {
    case P_SAVE:
      {
        cp = get_pragma_token(cp, &token);
        if (TOKEN_EOL != token.type)
          {
            err = 1;
            break;
          }

        STACK_PUSH(options_stack, cloneOptions(&options));
        STACK_PUSH(optimize_stack, cloneOptimize(&optimize));
        STACK_PUSH(SDCCERRG_stack, cloneSDCCERRG(&_SDCCERRG));
      }
      break;

    case P_RESTORE:
      {
        struct options *optionsp;
        struct optimize *optimizep;
        struct SDCCERRG *sdccerrgp;

        cp = get_pragma_token(cp, &token);
        if (TOKEN_EOL != token.type)
          {
            err = 1;
            break;
          }

        optionsp = STACK_POP(options_stack);
        copyAndFreeOptions(&options, optionsp);

        optimizep = STACK_POP(optimize_stack);
        copyAndFreeOptimize(&optimize, optimizep);

        sdccerrgp = STACK_POP(SDCCERRG_stack);
        copyAndFreeSDCCERRG(&_SDCCERRG, sdccerrgp);
      }
      break;

    case P_NOINDUCTION:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      optimize.loopInduction = 0;
      break;

    case P_NOINVARIANT:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      optimize.loopInvariant = 0;
      break;

    case P_INDUCTION:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      optimize.loopInduction = 1;
      break;

    case P_STACKAUTO:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      options.stackAuto = 1;
      break;

    case P_NOJTBOUND:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      optimize.noJTabBoundary = 1;
      break;

    case P_NOGCSE:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      optimize.global_cse = 0;
      break;

    case P_NOOVERLAY:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      options.noOverlay = 1;
      break;

    case P_LESSPEDANTIC:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      options.lessPedantic = 1;
      setErrorLogLevel(ERROR_LEVEL_WARNING);
      break;

    case P_CALLEE_SAVES:
      /* append to the functions already listed
         in callee-saves */
      setParseWithComma(&options.calleeSavesSet, cp);
      err = -1;
      break;

    case P_EXCLUDE:
      {
        deleteSet(&options.excludeRegsSet);
        setParseWithComma(&options.excludeRegsSet, cp);
        err = -1;
      }
      break;

    case P_NOIV:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      options.noiv = 1;
      break;

    case P_LOOPREV:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      optimize.noLoopReverse = 1;
      break;

    case P_OVERLAY_:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      break; /* notyet */

    case P_DISABLEWARN:
      {
        int warn;

        cp = get_pragma_token(cp, &token);

        if (token.type != TOKEN_INT)
          {
            err = 1;
            break;
          }
        warn = token.val.int_val;

        cp = get_pragma_token(cp, &token);
        if (TOKEN_EOL != token.type)
          {
            err = 1;
            break;
          }

        if (warn < MAX_ERROR_WARNING)
          setWarningDisabled(warn);
      }
      break;

    case P_OPTCODESPEED:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      optimize.codeSpeed = 1;
      optimize.codeSize = 0;
      break;

    case P_OPTCODESIZE:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      optimize.codeSpeed = 0;
      optimize.codeSize = 1;
      break;

    case P_OPTCODEBALANCED:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      optimize.codeSpeed = 0;
      optimize.codeSize = 0;
      break;

    case P_STD_C89:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      options.std_c99 = 0;
      options.std_sdcc = 0;
      break;

    case P_STD_C99:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      options.std_c99 = 1;
      options.std_sdcc = 0;
      break;

    case P_STD_SDCC89:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      options.std_c99 = 0;
      options.std_sdcc = 1;
      break;

    case P_STD_SDCC99:
      cp = get_pragma_token(cp, &token);
      if (TOKEN_EOL != token.type)
        {
          err = 1;
          break;
        }

      options.std_c99 = 1;
      options.std_sdcc = 1;
      break;

    case P_CODESEG:
      {
        const char *segname;

        cp = get_pragma_token(cp, &token);
        if (token.type == TOKEN_EOL)
          {
            err = 1;
            break;
          }
        segname = get_pragma_string(&token);

        cp = get_pragma_token(cp, &token);
        if (token.type != TOKEN_EOL)
          {
            err = 1;
            break;
          }

        if (strlen(segname) > 8)
          {
            err = 1;
            break;
          }
        else
          {
            dbuf_append(&token.dbuf, "(CODE)", (sizeof "(CODE)") - 1);
            options.code_seg = Safe_strdup(get_pragma_string(&token));
          }
      }
      break;

    case P_CONSTSEG:
      {
        const char *segname;

        cp = get_pragma_token(cp, &token);
        if (token.type == TOKEN_EOL)
          {
            err = 1;
            break;
          }
        segname = get_pragma_string(&token);

        cp = get_pragma_token(cp, &token);
        if (token.type != TOKEN_EOL)
          {
            err = 1;
            break;
          }

        if (strlen(segname) > 8)
          {
            err = 1;
            break;
          }
        else
          {
            dbuf_append(&token.dbuf, "(CODE)", (sizeof "(CODE)") - 1);
            options.code_seg = Safe_strdup(get_pragma_string(&token));
          }
      }
      break;

    default:
      processed = 0;
      break;
    }

  get_pragma_token(cp, &token);

  if (1 == err || (0 == err && token.type != TOKEN_EOL))
    werror(W_BAD_PRAGMA_ARGUMENTS, name);

  free_pragma_token(&token);
  return processed;
}

static struct pragma_s pragma_tbl[] = {
  { "save",           P_SAVE,         0, doPragma },
  { "restore",        P_RESTORE,      0, doPragma },
  { "noinduction",    P_NOINDUCTION,  0, doPragma },
  { "noinvariant",    P_NOINVARIANT,  0, doPragma },
  { "noloopreverse",  P_LOOPREV,      0, doPragma },
  { "induction",      P_INDUCTION,    0, doPragma },
  { "stackauto",      P_STACKAUTO,    0, doPragma },
  { "nojtbound",      P_NOJTBOUND,    0, doPragma },
  { "nogcse",         P_NOGCSE,       0, doPragma },
  { "nooverlay",      P_NOOVERLAY,    0, doPragma },
  { "callee_saves",   P_CALLEE_SAVES, 0, doPragma },
  { "exclude",        P_EXCLUDE,      0, doPragma },
  { "noiv",           P_NOIV,         0, doPragma },
  { "overlay",        P_OVERLAY_,     0, doPragma },
  { "less_pedantic",  P_LESSPEDANTIC, 0, doPragma },
  { "disable_warning",P_DISABLEWARN,  0, doPragma },
  { "opt_code_speed", P_OPTCODESPEED, 0, doPragma },
  { "opt_code_size",  P_OPTCODESIZE,  0, doPragma },
  { "opt_code_balanced", P_OPTCODEBALANCED, 0, doPragma },
  { "std_c89",        P_STD_C89,      0, doPragma },
  { "std_c99",        P_STD_C99,      0, doPragma },
  { "std_sdcc89",     P_STD_SDCC89,   0, doPragma },
  { "std_sdcc99",     P_STD_SDCC99,   0, doPragma },
  { "codeseg",        P_CODESEG,      0, doPragma },
  { "constseg",       P_CONSTSEG,     0, doPragma },
  { NULL,             0,              0, NULL },
};

/*
 * returns 1 if the pragma was processed, 0 if not
 */
int
process_pragma_tbl(const struct pragma_s *pragma_tbl, const char *s)
{
  struct pragma_token_s token;
  int i;
  int ret = 0;

  init_pragma_token(&token);

  s = get_pragma_token(s, &token);

  /* skip separating whitespace */
  while ('\n' != *s && isspace((unsigned char)*s))
    s++;

  for (i = 0; NULL != pragma_tbl[i].name; ++i)
    {
      /* now compare and do what needs to be done */
      if (strcmp(get_pragma_string(&token), pragma_tbl[i].name) == 0)
        {
          if (pragma_tbl[i].deprecated != 0)
            werror(W_DEPRECATED_PRAGMA, pragma_tbl[i].name);

          ret = (*pragma_tbl[i].func)(pragma_tbl[i].id, pragma_tbl[i].name, s);
          break;
        }
    }

  free_pragma_token(&token);
  return ret;
}

static int process_pragma(const char *s)
{
  struct pragma_token_s token;

  init_pragma_token(&token);

  s = get_pragma_token(s, &token);
  if (0 != strcmp("#pragma", get_pragma_string(&token)))
    {
      /* Oops, womething went totally wrong - internal error */
      wassertl(0, "pragma parser internal error");
    }

  /* skip spaces */
  while ('\n' != *s && isspace((unsigned char)*s))
    ++s;

  /* First give the port a chance */
  if (port->process_pragma && port->process_pragma(s))
    return 1;

  if (process_pragma_tbl(pragma_tbl, s))
    {
      return 1;
    }
  else
    {
      werror(W_UNKNOWN_PRAGMA, s);
      return 0;
    }
}

/* will return 1 if the string is a part
   of a target specific keyword */
static int isTargetKeyword(char *s)
{
  int i;

  if (port->keywords == NULL)
    return 0;

  if (s[0] == '_' && s[1] == '_')
    {
      /* Keywords in the port's array have either 0 or 1 underscore, */
      /* so skip over the appropriate number of chars when comparing */
      for (i = 0 ; port->keywords[i] ; i++ )
        {
          if (port->keywords[i][0] == '_' &&
              strcmp(port->keywords[i],s+1) == 0)
            return 1;
          else if (strcmp(port->keywords[i],s+2) == 0)
            return 1;
        }
    }
  else
    {
      for (i = 0 ; port->keywords[i] ; i++ )
        {
          if (strcmp(port->keywords[i],s) == 0)
            return 1;
        }
    }

  return 0;
}

int yywrap(void)
{
  if (!STACK_EMPTY(options_stack) || !STACK_EMPTY(optimize_stack))
    werror(W_SAVE_RESTORE);

  return 1;
}

int yyerror(char *s)
{
  fflush(stdout);

  if(options.vc_err_style)
    fprintf(stderr, "\n%s(%d) : %s: token -> '%s' ; column %d\n",
      filename, mylineno, s, yytext, column);
  else
    fprintf(stderr, "\n%s:%d: %s: token -> '%s' ; column %d\n",
      filename, mylineno, s ,yytext, column);
  fatalError++;

  return 0;
}
