#ifndef YY_cl_ucsim_parser_h_included
#define YY_cl_ucsim_parser_h_included
#define YY_USE_CLASS

#line 1 "/usr/share/bison++/bison.h"
/* before anything */
#ifdef c_plusplus
 #ifndef __cplusplus
  #define __cplusplus
 #endif
#endif


 #line 8 "/usr/share/bison++/bison.h"
#define YY_cl_ucsim_parser_INHERIT  : public cl_base
#define YY_cl_ucsim_parser_MEMBERS  class cl_ucsim_lexer *lexer_object;\
virtual ~YY_cl_ucsim_parser_CLASS(void) { delete lexer_object; }
#define YY_cl_ucsim_parser_CONSTRUCTOR_PARAM  \
class cl_ucsim_lexer *the_lexer
#define YY_cl_ucsim_parser_CONSTRUCTOR_CODE  \
lexer_object= the_lexer;

#line 43 "cmdpars.y"
typedef union {
  long number;
  class cl_memory *memory_object;
  struct {
    class cl_memory *memory;
    long address;
  } memory;
  struct {
    class cl_memory *memory;
    long mem_address, bit_address;
    long mask;
  } bit;
} yy_cl_ucsim_parser_stype;
#define YY_cl_ucsim_parser_STYPE yy_cl_ucsim_parser_stype

#line 21 "/usr/share/bison++/bison.h"
 /* %{ and %header{ and %union, during decl */
#ifndef YY_cl_ucsim_parser_COMPATIBILITY
 #ifndef YY_USE_CLASS
  #define  YY_cl_ucsim_parser_COMPATIBILITY 1
 #else
  #define  YY_cl_ucsim_parser_COMPATIBILITY 0
 #endif
#endif

#if YY_cl_ucsim_parser_COMPATIBILITY != 0
/* backward compatibility */
 #ifdef YYLTYPE
  #ifndef YY_cl_ucsim_parser_LTYPE
   #define YY_cl_ucsim_parser_LTYPE YYLTYPE
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
  #endif
 #endif
/*#ifdef YYSTYPE*/
  #ifndef YY_cl_ucsim_parser_STYPE
   #define YY_cl_ucsim_parser_STYPE YYSTYPE
  /* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
   /* use %define STYPE */
  #endif
/*#endif*/
 #ifdef YYDEBUG
  #ifndef YY_cl_ucsim_parser_DEBUG
   #define  YY_cl_ucsim_parser_DEBUG YYDEBUG
   /* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
   /* use %define DEBUG */
  #endif
 #endif 
 /* use goto to be compatible */
 #ifndef YY_cl_ucsim_parser_USE_GOTO
  #define YY_cl_ucsim_parser_USE_GOTO 1
 #endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_cl_ucsim_parser_USE_GOTO
 #define YY_cl_ucsim_parser_USE_GOTO 0
#endif

#ifndef YY_cl_ucsim_parser_PURE

 #line 65 "/usr/share/bison++/bison.h"
#define YY_cl_ucsim_parser_PURE 1


#line 65 "/usr/share/bison++/bison.h"
/* YY_cl_ucsim_parser_PURE */
#endif


 #line 68 "/usr/share/bison++/bison.h"

#line 68 "/usr/share/bison++/bison.h"
/* prefix */

#ifndef YY_cl_ucsim_parser_DEBUG

 #line 71 "/usr/share/bison++/bison.h"

#line 71 "/usr/share/bison++/bison.h"
/* YY_cl_ucsim_parser_DEBUG */
#endif

#ifndef YY_cl_ucsim_parser_LSP_NEEDED

 #line 75 "/usr/share/bison++/bison.h"

#line 75 "/usr/share/bison++/bison.h"
 /* YY_cl_ucsim_parser_LSP_NEEDED*/
#endif

/* DEFAULT LTYPE*/
#ifdef YY_cl_ucsim_parser_LSP_NEEDED
 #ifndef YY_cl_ucsim_parser_LTYPE
  #ifndef BISON_YYLTYPE_ISDECLARED
   #define BISON_YYLTYPE_ISDECLARED
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;
  #endif

  #define YY_cl_ucsim_parser_LTYPE yyltype
 #endif
#endif

/* DEFAULT STYPE*/
#ifndef YY_cl_ucsim_parser_STYPE
 #define YY_cl_ucsim_parser_STYPE int
#endif

/* DEFAULT MISCELANEOUS */
#ifndef YY_cl_ucsim_parser_PARSE
 #define YY_cl_ucsim_parser_PARSE yyparse
#endif

#ifndef YY_cl_ucsim_parser_LEX
 #define YY_cl_ucsim_parser_LEX yylex
#endif

#ifndef YY_cl_ucsim_parser_LVAL
 #define YY_cl_ucsim_parser_LVAL yylval
#endif

#ifndef YY_cl_ucsim_parser_LLOC
 #define YY_cl_ucsim_parser_LLOC yylloc
#endif

#ifndef YY_cl_ucsim_parser_CHAR
 #define YY_cl_ucsim_parser_CHAR yychar
#endif

#ifndef YY_cl_ucsim_parser_NERRS
 #define YY_cl_ucsim_parser_NERRS yynerrs
#endif

#ifndef YY_cl_ucsim_parser_DEBUG_FLAG
 #define YY_cl_ucsim_parser_DEBUG_FLAG yydebug
#endif

#ifndef YY_cl_ucsim_parser_ERROR
 #define YY_cl_ucsim_parser_ERROR yyerror
#endif

#ifndef YY_cl_ucsim_parser_PARSE_PARAM
 #ifndef __STDC__
  #ifndef __cplusplus
   #ifndef YY_USE_CLASS
    #define YY_cl_ucsim_parser_PARSE_PARAM
    #ifndef YY_cl_ucsim_parser_PARSE_PARAM_DEF
     #define YY_cl_ucsim_parser_PARSE_PARAM_DEF
    #endif
   #endif
  #endif
 #endif
 #ifndef YY_cl_ucsim_parser_PARSE_PARAM
  #define YY_cl_ucsim_parser_PARSE_PARAM void
 #endif
#endif

/* TOKEN C */
#ifndef YY_USE_CLASS

 #ifndef YY_cl_ucsim_parser_PURE
  #ifndef yylval
   extern YY_cl_ucsim_parser_STYPE YY_cl_ucsim_parser_LVAL;
  #else
   #if yylval != YY_cl_ucsim_parser_LVAL
    extern YY_cl_ucsim_parser_STYPE YY_cl_ucsim_parser_LVAL;
   #else
    #warning "Namespace conflict, disabling some functionality (bison++ only)"
   #endif
  #endif
 #endif


 #line 169 "/usr/share/bison++/bison.h"
#define	PTOK_PLUS	258
#define	PTOK_MINUS	259
#define	PTOK_ASTERIX	260
#define	PTOK_SLASH	261
#define	PTOK_EQUAL	262
#define	PTOK_LEFT_PAREN	263
#define	PTOK_RIGHT_PAREN	264
#define	PTOK_LEFT_BRACKET	265
#define	PTOK_RIGHT_BRACKET	266
#define	PTOK_DOT	267
#define	PTOK_AMPERSAND	268
#define	PTOK_MEMORY_OBJECT	269
#define	PTOK_MEMORY	270
#define	PTOK_NUMBER	271
#define	PTOK_BIT	272
#define	UNARYMINUS	273


#line 169 "/usr/share/bison++/bison.h"
 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
 #ifndef YY_cl_ucsim_parser_CLASS
  #define YY_cl_ucsim_parser_CLASS cl_ucsim_parser
 #endif

 #ifndef YY_cl_ucsim_parser_INHERIT
  #define YY_cl_ucsim_parser_INHERIT
 #endif

 #ifndef YY_cl_ucsim_parser_MEMBERS
  #define YY_cl_ucsim_parser_MEMBERS 
 #endif

 #ifndef YY_cl_ucsim_parser_LEX_BODY
  #define YY_cl_ucsim_parser_LEX_BODY  
 #endif

 #ifndef YY_cl_ucsim_parser_ERROR_BODY
  #define YY_cl_ucsim_parser_ERROR_BODY  
 #endif

 #ifndef YY_cl_ucsim_parser_CONSTRUCTOR_PARAM
  #define YY_cl_ucsim_parser_CONSTRUCTOR_PARAM
 #endif
 /* choose between enum and const */
 #ifndef YY_cl_ucsim_parser_USE_CONST_TOKEN
  #define YY_cl_ucsim_parser_USE_CONST_TOKEN 0
  /* yes enum is more compatible with flex,  */
  /* so by default we use it */ 
 #endif
 #if YY_cl_ucsim_parser_USE_CONST_TOKEN != 0
  #ifndef YY_cl_ucsim_parser_ENUM_TOKEN
   #define YY_cl_ucsim_parser_ENUM_TOKEN yy_cl_ucsim_parser_enum_token
  #endif
 #endif

class YY_cl_ucsim_parser_CLASS YY_cl_ucsim_parser_INHERIT
{
public: 
 #if YY_cl_ucsim_parser_USE_CONST_TOKEN != 0
  /* static const int token ... */
  
 #line 212 "/usr/share/bison++/bison.h"
static const int PTOK_PLUS;
static const int PTOK_MINUS;
static const int PTOK_ASTERIX;
static const int PTOK_SLASH;
static const int PTOK_EQUAL;
static const int PTOK_LEFT_PAREN;
static const int PTOK_RIGHT_PAREN;
static const int PTOK_LEFT_BRACKET;
static const int PTOK_RIGHT_BRACKET;
static const int PTOK_DOT;
static const int PTOK_AMPERSAND;
static const int PTOK_MEMORY_OBJECT;
static const int PTOK_MEMORY;
static const int PTOK_NUMBER;
static const int PTOK_BIT;
static const int UNARYMINUS;


#line 212 "/usr/share/bison++/bison.h"
 /* decl const */
 #else
  enum YY_cl_ucsim_parser_ENUM_TOKEN { YY_cl_ucsim_parser_NULL_TOKEN=0
  
 #line 215 "/usr/share/bison++/bison.h"
	,PTOK_PLUS=258
	,PTOK_MINUS=259
	,PTOK_ASTERIX=260
	,PTOK_SLASH=261
	,PTOK_EQUAL=262
	,PTOK_LEFT_PAREN=263
	,PTOK_RIGHT_PAREN=264
	,PTOK_LEFT_BRACKET=265
	,PTOK_RIGHT_BRACKET=266
	,PTOK_DOT=267
	,PTOK_AMPERSAND=268
	,PTOK_MEMORY_OBJECT=269
	,PTOK_MEMORY=270
	,PTOK_NUMBER=271
	,PTOK_BIT=272
	,UNARYMINUS=273


#line 215 "/usr/share/bison++/bison.h"
 /* enum token */
     }; /* end of enum declaration */
 #endif
public:
 int YY_cl_ucsim_parser_PARSE(YY_cl_ucsim_parser_PARSE_PARAM);
 virtual void YY_cl_ucsim_parser_ERROR(char *msg) YY_cl_ucsim_parser_ERROR_BODY;
 #ifdef YY_cl_ucsim_parser_PURE
  #ifdef YY_cl_ucsim_parser_LSP_NEEDED
   virtual int  YY_cl_ucsim_parser_LEX(YY_cl_ucsim_parser_STYPE *YY_cl_ucsim_parser_LVAL,YY_cl_ucsim_parser_LTYPE *YY_cl_ucsim_parser_LLOC) YY_cl_ucsim_parser_LEX_BODY;
  #else
   virtual int  YY_cl_ucsim_parser_LEX(YY_cl_ucsim_parser_STYPE *YY_cl_ucsim_parser_LVAL) YY_cl_ucsim_parser_LEX_BODY;
  #endif
 #else
  virtual int YY_cl_ucsim_parser_LEX() YY_cl_ucsim_parser_LEX_BODY;
  YY_cl_ucsim_parser_STYPE YY_cl_ucsim_parser_LVAL;
  #ifdef YY_cl_ucsim_parser_LSP_NEEDED
   YY_cl_ucsim_parser_LTYPE YY_cl_ucsim_parser_LLOC;
  #endif
  int YY_cl_ucsim_parser_NERRS;
  int YY_cl_ucsim_parser_CHAR;
 #endif
 #if YY_cl_ucsim_parser_DEBUG != 0
  public:
   int YY_cl_ucsim_parser_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
 #endif
public:
 YY_cl_ucsim_parser_CLASS(YY_cl_ucsim_parser_CONSTRUCTOR_PARAM);
public:
 YY_cl_ucsim_parser_MEMBERS 
};
/* other declare folow */
#endif


#if YY_cl_ucsim_parser_COMPATIBILITY != 0
 /* backward compatibility */
 /* Removed due to bison problems
 /#ifndef YYSTYPE
 / #define YYSTYPE YY_cl_ucsim_parser_STYPE
 /#endif*/

 #ifndef YYLTYPE
  #define YYLTYPE YY_cl_ucsim_parser_LTYPE
 #endif
 #ifndef YYDEBUG
  #ifdef YY_cl_ucsim_parser_DEBUG 
   #define YYDEBUG YY_cl_ucsim_parser_DEBUG
  #endif
 #endif

#endif
/* END */

 #line 267 "/usr/share/bison++/bison.h"
#endif
