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

D        [0-9]
L        [a-zA-Z_]
H        [a-fA-F0-9]
E        [Ee][+-]?{D}+
FS       (f|F|l|L)
IS       (u|U|l|L)*
%{

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "newalloc.h"
#include "dbuf.h"

char *stringLiteral();
char *currFname;

extern int lineno, column;
extern char *filename ;
int   yylineno = 1               ;
void count()                     ;
int process_pragma(char *);
#undef yywrap

int yywrap YY_PROTO((void))
{
   return(1);
}
#define TKEYWORD(token) return (isTargetKeyword(yytext) ? token :\
			        check_type(yytext))
char *asmbuff=NULL;
int asmbuffSize=0;
char *asmp ;
extern int check_type		();
 extern int isTargetKeyword     ();
extern int checkCurrFile	(char *);
extern int processPragma	(char *);
extern int printListing		(int   );
struct optimize save_optimize ;
struct options  save_options  ;
%}
%x asm
%%
"_asm"         {  
  count(); 
  asmp = asmbuff = realloc (asmbuff, INITIAL_INLINEASM);
  asmbuffSize=INITIAL_INLINEASM;
  BEGIN(asm) ;
}
<asm>"_endasm" { 
  count();
  *asmp = '\0';
  yylval.yyinline = strdup (asmbuff);
  BEGIN(INITIAL);
  return (INLINEASM);
}
<asm>.         { 
  if (asmp-asmbuff >= asmbuffSize-2) {
    // increase the buffersize with 50%
    int size=asmp-asmbuff;
    asmbuffSize=asmbuffSize*3/2;
    asmbuff = realloc (asmbuff, asmbuffSize); 
    asmp=asmbuff+size;
  }
  *asmp++ = yytext[0];
}
<asm>\n        { 
  count(); 
  if (asmp-asmbuff >= asmbuffSize-3) {
    // increase the buffersize with 50%
    int size=asmp-asmbuff;
    asmbuffSize=asmbuffSize*3/2;
    asmbuff = realloc (asmbuff, asmbuffSize); 
    asmp=asmbuff+size;
  }
  *asmp++ = '\n' ;
}
"at"	       { count(); TKEYWORD(AT)  ; }
"auto"	       { count(); return(AUTO); }
"bit"	       { count(); TKEYWORD(BIT) ; }
"break"        { count(); return(BREAK); }
"case"         { count(); return(CASE); }
"char"         { count(); return(CHAR); }
"code"         { count(); TKEYWORD(CODE); }
"const"        { count(); return(CONST); }
"continue"     { count(); return(CONTINUE); }
"critical"     { count(); TKEYWORD(CRITICAL); } 
"data"	       { count(); TKEYWORD(DATA);   }
"default"      { count(); return(DEFAULT); }
"do"           { count(); return(DO); }
"double"       { count(); werror(W_DOUBLE_UNSUPPORTED);return(FLOAT); }
"else"         { count(); return(ELSE); }
"enum"         { count(); return(ENUM); }
"extern"       { count(); return(EXTERN); }
"far"          { count(); TKEYWORD(XDATA);  }
"eeprom"       { count(); TKEYWORD(EEPROM);  }
"float"        { count(); return(FLOAT); }
"flash"        { count(); TKEYWORD(CODE);}
"for"          { count(); return(FOR); }
"goto"	       { count(); return(GOTO); }
"idata"        { count(); TKEYWORD(IDATA);}
"if"           { count(); return(IF); }
"int"          { count(); return(INT); }
"interrupt"    { count(); return(INTERRUPT);}
"nonbanked"    { count(); TKEYWORD(NONBANKED);}
"banked"       { count(); TKEYWORD(BANKED);}
"long"	       { count(); return(LONG); }
"near"	       { count(); TKEYWORD(DATA);}
"pdata"        { count(); TKEYWORD(PDATA); }
"reentrant"    { count(); TKEYWORD(REENTRANT);}
"register"     { count(); return(REGISTER); }
"return"       { count(); return(RETURN); }
"sfr"	       { count(); TKEYWORD(SFR)	; }
"sbit"	       { count(); TKEYWORD(SBIT)	; }
"short"        { count(); return(SHORT); }
"signed"       { count(); return(SIGNED); }
"sizeof"       { count(); return(SIZEOF); }
"sram"         { count(); TKEYWORD(XDATA);}
"static"       { count(); return(STATIC); }
"struct"       { count(); return(STRUCT); }
"switch"       { count(); return(SWITCH); }
"typedef"      { count(); return(TYPEDEF); }
"union"        { count(); return(UNION); }
"unsigned"     { count(); return(UNSIGNED); }
"void"         { count(); return(VOID); }
"volatile"     { count(); return(VOLATILE); }
"using"        { count(); TKEYWORD(USING); }
"_naked"       { count(); TKEYWORD(NAKED); }
"while"        { count(); return(WHILE); }
"xdata"        { count(); TKEYWORD(XDATA); }
"..."	       { count(); return(VAR_ARGS);}
"__typeof"     { count(); return TYPEOF;}
"_JavaNative"  { count(); TKEYWORD(JAVANATIVE);}
"_overlay"     { count(); TKEYWORD(OVERLAY);}
{L}({L}|{D})*  { count(); return(check_type()); }
0[xX]{H}+{IS}? { count(); yylval.val = constVal(yytext); return(CONSTANT); }
0{D}+{IS}?     { count(); yylval.val = constVal(yytext); return(CONSTANT); }
{D}+{IS}?      { count(); yylval.val = constVal(yytext); return(CONSTANT); }
'(\\.|[^\\'])+' { count();yylval.val = charVal (yytext); return(CONSTANT); /* ' make syntax highliter happy */}
{D}+{E}{FS}?   { count(); yylval.val = constFloatVal(yytext);return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?  { count(); yylval.val = constFloatVal(yytext);return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ count(); yylval.val = constFloatVal(yytext);return(CONSTANT); }
\"             { count(); yylval.val=strVal(stringLiteral()); return(STRING_LITERAL);}
">>=" { count(); yylval.yyint = RIGHT_ASSIGN ; return(RIGHT_ASSIGN); }
"<<=" { count(); yylval.yyint = LEFT_ASSIGN  ; return(LEFT_ASSIGN) ; }
"+="  { count(); yylval.yyint = ADD_ASSIGN   ; return(ADD_ASSIGN)  ; }
"-="  { count(); yylval.yyint = SUB_ASSIGN   ; return(SUB_ASSIGN)  ; }
"*="  { count(); yylval.yyint = MUL_ASSIGN   ; return(MUL_ASSIGN)  ; }
"/="  { count(); yylval.yyint = DIV_ASSIGN   ; return(DIV_ASSIGN)  ; }
"%="  { count(); yylval.yyint = MOD_ASSIGN   ; return(MOD_ASSIGN)  ; }
"&="  { count(); yylval.yyint = AND_ASSIGN   ; return(AND_ASSIGN)  ; }
"^="  { count(); yylval.yyint = XOR_ASSIGN   ; return(XOR_ASSIGN)  ; }
"|="  { count(); yylval.yyint = OR_ASSIGN    ; return(OR_ASSIGN)   ; }
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
"{"	       { count(); NestLevel++ ;  return('{'); }
"}"	       { count(); NestLevel--; return('}'); }
","            { count(); return(','); }
":"            { count(); return(':'); }
"="            { count(); return('='); }
"("            { count(); return('('); }
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
^#line.*"\n"	   { count(); checkCurrFile(yytext); }
^#pragma.*"\n"   { count(); process_pragma(yytext); }

^[^(]+"("[0-9]+") : error"[^\n]+ { werror(E_PRE_PROC_FAILED,yytext);count(); }
^[^(]+"("[0-9]+") : warning"[^\n]+ { werror(W_PRE_PROC_WARNING,yytext);count(); }
"\r\n"		   { count(); }
"\n"		   { count(); }
[ \t\v\f]      { count(); }
\\ {
  char ch=input();
  if (ch!='\n') {
    // that could have been removed by the preprocessor anyway
    werror (W_STRAY_BACKSLASH, column);
    unput(ch);
  }
}
.			   { count()	; }
%%

int checkCurrFile ( char *s)
{
    char lineNum[10]			;
    int  lNum				;
    char *tptr				;
       
    /* first check if this is a #line */
    if ( strncmp(s,"#line",5) )
	return  0				;
    
    /* get to the line number */
    while (!isdigit(*s))
	s++ ;
    tptr = lineNum ;
    while (isdigit(*s))
	*tptr++ = *s++ ;
    *tptr = '\0'; 
    sscanf(lineNum,"%d",&lNum);
    
    /* now see if we have a file name */
    while (*s != '\"' && *s) 
	s++ ;
    
    /* if we don't have a filename then */
    /* set the current line number to   */
    /* line number if printFlag is on   */
    if (!*s) {		
      lineno = yylineno = lNum ;
      return 0;
    }
    
    /* if we have a filename then check */
    /* if it is "standard in" if yes then */
    /* get the currentfile name info    */
    s++ ;

    /* in c1mode fullSrcFileName is NULL */
    if ( fullSrcFileName &&
         strncmp(s,fullSrcFileName,strlen(fullSrcFileName)) == 0) {
      lineno = yylineno = lNum;					
      currFname = fullSrcFileName ;
    }  else {
	char *sb = s;
	/* mark the end of the filename */
	while (*s != '"') s++;
	*s = '\0';
	currFname = strdup (sb);
	lineno = yylineno = lNum;
    }
    filename = currFname ;
    return 0;
}
    
int column = 0;
int plineIdx=0;

void count()
{
  int i;
  for (i = 0; yytext[i] != '\0'; i++)   {				
    if (yytext[i] == '\n')      {         
      column = 0;
      lineno = ++yylineno ;
    }
    else 
      if (yytext[i] == '\t')
	column += 8 - (column % 8);
      else
	column++;
  }
  /* ECHO; */
}

int check_type()
{
	/* check if it is in the typedef table */
	if (findSym(TypedefTab,NULL,yytext)) {
		strncpyz(yylval.yychar,yytext, SDCC_NAME_MAX);
		return (TYPE_NAME) ;
	}
	else   {
		strncpyz (yylval.yychar,yytext, SDCC_NAME_MAX);
		return(IDENTIFIER);
	}
}

/*
 * Change by JTV 2001-05-19 to not concantenate strings
 * to support ANSI hex and octal escape sequences in string liteals 
 */

char *stringLiteral()
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
        lineno = ++yylineno;
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
      lineno = ++yylineno;
      column = 0;
      break;

    case '"':
      /* if this is a quote then we have work to do */
      /* find the next non whitespace character     */
      /* if that is a double quote then carry on    */
      dbuf_append(&dbuf, "\"", 1);  /* Pass end of this string or substring to evaluator */
      while ((ch = input()) && (isspace(ch) || ch=='\\')) {
        switch (ch) {
        case '\\':
          if ((ch = input()) != '\n') {
            werror(W_STRAY_BACKSLASH, column);
            unput(ch);
          }
          else {
            lineno = ++yylineno;
            column = 0;
          }
          break;

        case '\n':
          yylineno++;
          break;
        }
      }

      if (!ch) 
        goto out;

      if (ch != '\"') {
        unput(ch) ;
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


enum pragma_id {
     P_SAVE = 1,
     P_RESTORE ,
     P_NOINDUCTION,
     P_NOINVARIANT,
     P_INDUCTION ,
     P_STACKAUTO ,
     P_NOJTBOUND ,
     P_NOOVERLAY ,
     P_LESSPEDANTIC,
     P_NOGCSE    ,
     P_CALLEE_SAVES,
     P_EXCLUDE   ,
     P_NOIV      ,
     P_LOOPREV   ,
     P_OVERLAY_      /* I had a strange conflict with P_OVERLAY while */
                     /* cross-compiling for MINGW32 with gcc 3.2 */
};


/* SAVE/RESTORE stack */
#define SAVE_RESTORE_SIZE 128

STACK_DCL(options_stack, struct options *, SAVE_RESTORE_SIZE)
STACK_DCL(optimize_stack, struct optimize *, SAVE_RESTORE_SIZE)


void doPragma (int op, char *cp)
{
  switch (op) {
  case P_SAVE:
    {
      struct options *optionsp;
      struct optimize *optimizep;

      optionsp = Safe_malloc(sizeof (struct options));
      *optionsp = options;
      STACK_PUSH(options_stack, optionsp);

      optimizep = Safe_malloc(sizeof (struct optimize));
      *optimizep = optimize;
      STACK_PUSH(optimize_stack, optimizep);
    }
    break;

  case P_RESTORE:
    {
      struct options *optionsp;
      struct optimize *optimizep;

      optionsp = STACK_POP(options_stack);
      options = *optionsp;
      Safe_free(optionsp);

      optimizep = STACK_POP(optimize_stack);
      optimize = *optimizep;
      Safe_free(optimizep);
    }
    break;

  case P_NOINDUCTION:
    optimize.loopInduction = 0 ;
    break;

  case P_NOINVARIANT:
    optimize.loopInvariant = 0 ;
    break;

  case P_INDUCTION:
    optimize.loopInduction = 1 ;
    break;

  case P_STACKAUTO:
    options.stackAuto = 1;
    break;

  case P_NOJTBOUND:
    optimize.noJTabBoundary = 1;
    break;

  case P_NOGCSE:
    optimize.global_cse = 0;
    break;

  case P_NOOVERLAY:
    options.noOverlay = 1;
    break;

  case P_LESSPEDANTIC:
    options.lessPedantic = 1;
    break;

  case P_CALLEE_SAVES:
    {
      int i=0;
      /* append to the functions already listed
         in callee-saves */
      for (; options.calleeSaves[i] ;i++);
        parseWithComma(&options.calleeSaves[i], Safe_strdup(cp));
    }
    break;

  case P_EXCLUDE:
    parseWithComma(options.excludeRegs, Safe_strdup(cp));
    break;

  case P_NOIV:
    options.noiv = 1;
    break;

  case P_LOOPREV:
    optimize.noLoopReverse = 1;
    break;

  case P_OVERLAY_:
    break; /* notyet */
  }
}

int process_pragma(char *s)
{
#define NELEM(x)  (sizeof (x) / sizeof (x)[0])
#define PRAGMA    "#pragma"

  static struct pragma_s {
    const char *name;
    enum pragma_id id;
  } pragma_tbl[] = {
    { "SAVE",           P_SAVE },
    { "RESTORE",        P_RESTORE },
    { "NOINDUCTION",    P_NOINDUCTION },
    { "NOINVARIANT",    P_NOINVARIANT },
    { "NOLOOPREVERSE",  P_LOOPREV },
    { "INDUCTION",      P_INDUCTION },
    { "STACKAUTO",      P_STACKAUTO },
    { "NOJTBOUND",      P_NOJTBOUND },
    { "NOGCSE",         P_NOGCSE },
    { "NOOVERLAY",      P_NOOVERLAY },
    { "CALLEE-SAVES",   P_CALLEE_SAVES },
    { "EXCLUDE",        P_EXCLUDE },
    { "NOIV",           P_NOIV },
    { "OVERLAY",        P_OVERLAY_ },
    { "LESS_PEDANTIC",  P_LESSPEDANTIC },
  };
  char *cp ;
  int i;

  /* find the pragma */
  while (strncmp(s, PRAGMA, (sizeof PRAGMA) - 1))
    s++;
  s += (sizeof PRAGMA) - 1;
    
  /* look for the directive */
  while(isspace(*s))
    s++;

  cp = s;
  /* look for the end of the directive */
  while ((!isspace(*s)) && (*s != '\n')) 
    s++ ;    

  /* First give the port a chance */
  if (port->process_pragma && !port->process_pragma(cp))
    return 0;

  for (i = 0; i < NELEM(pragma_tbl); i++) {
    /* now compare and do what needs to be done */
    size_t len = strlen(pragma_tbl[i].name);

    if (strncmp(cp, pragma_tbl[i].name, len) == 0) {
      doPragma(pragma_tbl[i].id, cp + len);
      return 0;
    }
  }

  werror(W_UNKNOWN_PRAGMA,cp);
  return 0;
}

/* will return 1 if the string is a part
   of a target specific keyword */
int isTargetKeyword(char *s)
{
    int i;
    
    if (port->keywords == NULL)
	return 0;
    for ( i = 0 ; port->keywords[i] ; i++ ) {
	if (strcmp(port->keywords[i],s) == 0)
	    return 1;
    }
    
    return 0;
}

extern int fatalError;

int yyerror(char *s)
{
   fflush(stdout);

   if (yylineno && filename) {
     fprintf(stdout,"\n%s:%d: %s: token -> '%s' ; column %d\n",
	     filename,yylineno,
	     s,yytext,column);
     fatalError++;
   } else {
     // this comes from an empy file, no problem
   }
   return 0;
}
