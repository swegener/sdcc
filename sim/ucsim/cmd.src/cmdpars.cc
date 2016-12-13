/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "cmdpars.y"


#include "cmdlexcl.h"
#include "memcl.h"
#include "globals.h"
#include "stypes.h"

static void yyerror (const char *msg);


/* Line 268 of yacc.c  */
#line 82 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PTOK_PLUS = 258,
     PTOK_MINUS = 259,
     PTOK_ASTERIX = 260,
     PTOK_SLASH = 261,
     PTOK_EQUAL = 262,
     PTOK_LEFT_PAREN = 263,
     PTOK_RIGHT_PAREN = 264,
     PTOK_LEFT_BRACKET = 265,
     PTOK_RIGHT_BRACKET = 266,
     PTOK_DOT = 267,
     PTOK_AMPERSAND = 268,
     PTOK_PIPE = 269,
     PTOK_CIRCUM = 270,
     PTOK_PERCENT = 271,
     PTOK_TILDE = 272,
     PTOK_QUESTION = 273,
     PTOK_COLON = 274,
     PTOK_EXCLAMATION = 275,
     PTOK_LESS = 276,
     PTOK_GREATHER = 277,
     PTOK_COMMA = 278,
     PTOK_AND_OP = 279,
     PTOK_OR_OP = 280,
     PTOK_INC_OP = 281,
     PTOK_DEC_OP = 282,
     PTOK_EQ_OP = 283,
     PTOK_NE_OP = 284,
     PTOK_GE_OP = 285,
     PTOK_LE_OP = 286,
     PTOK_LEFT_OP = 287,
     PTOK_RIGHT_OP = 288,
     PTOK_MUL_ASSIGN = 289,
     PTOK_DIV_ASSIGN = 290,
     PTOK_MOD_ASSIGN = 291,
     PTOK_ADD_ASSIGN = 292,
     PTOK_SUB_ASSIGN = 293,
     PTOK_LEFT_ASSIGN = 294,
     PTOK_RIGHT_ASSIGN = 295,
     PTOK_AND_ASSIGN = 296,
     PTOK_XOR_ASSIGN = 297,
     PTOK_OR_ASSIGN = 298,
     PTOK_INT = 299,
     PTOK_MEMORY_OBJECT = 300,
     PTOK_MEMORY = 301,
     PTOK_NUMBER = 302,
     PTOK_BIT = 303
   };
#endif
/* Tokens.  */
#define PTOK_PLUS 258
#define PTOK_MINUS 259
#define PTOK_ASTERIX 260
#define PTOK_SLASH 261
#define PTOK_EQUAL 262
#define PTOK_LEFT_PAREN 263
#define PTOK_RIGHT_PAREN 264
#define PTOK_LEFT_BRACKET 265
#define PTOK_RIGHT_BRACKET 266
#define PTOK_DOT 267
#define PTOK_AMPERSAND 268
#define PTOK_PIPE 269
#define PTOK_CIRCUM 270
#define PTOK_PERCENT 271
#define PTOK_TILDE 272
#define PTOK_QUESTION 273
#define PTOK_COLON 274
#define PTOK_EXCLAMATION 275
#define PTOK_LESS 276
#define PTOK_GREATHER 277
#define PTOK_COMMA 278
#define PTOK_AND_OP 279
#define PTOK_OR_OP 280
#define PTOK_INC_OP 281
#define PTOK_DEC_OP 282
#define PTOK_EQ_OP 283
#define PTOK_NE_OP 284
#define PTOK_GE_OP 285
#define PTOK_LE_OP 286
#define PTOK_LEFT_OP 287
#define PTOK_RIGHT_OP 288
#define PTOK_MUL_ASSIGN 289
#define PTOK_DIV_ASSIGN 290
#define PTOK_MOD_ASSIGN 291
#define PTOK_ADD_ASSIGN 292
#define PTOK_SUB_ASSIGN 293
#define PTOK_LEFT_ASSIGN 294
#define PTOK_RIGHT_ASSIGN 295
#define PTOK_AND_ASSIGN 296
#define PTOK_XOR_ASSIGN 297
#define PTOK_OR_ASSIGN 298
#define PTOK_INT 299
#define PTOK_MEMORY_OBJECT 300
#define PTOK_MEMORY 301
#define PTOK_NUMBER 302
#define PTOK_BIT 303




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 57 "cmdpars.y"

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



/* Line 293 of yacc.c  */
#line 230 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 242 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   148

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNRULES -- Number of states.  */
#define YYNSTATES  115

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   303

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    15,    17,    20,
      23,    25,    28,    31,    34,    37,    40,    43,    46,    49,
      51,    56,    58,    60,    64,    68,    72,    74,    78,    82,
      84,    88,    92,    94,    98,   102,   106,   110,   112,   116,
     120,   122,   126,   128,   132,   134,   138,   140,   144,   146,
     150,   152,   158,   160,   164,   168,   170,   172,   174,   176,
     178,   180,   182,   184,   186,   188,   190,   192,   196,   198,
     203,   205
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      50,     0,    -1,    69,    -1,    70,    -1,    71,    -1,    47,
      -1,     8,    69,     9,    -1,    51,    -1,    70,    26,    -1,
      70,    27,    -1,    52,    -1,    26,    70,    -1,    27,    70,
      -1,    13,    70,    -1,    13,    71,    -1,     4,    53,    -1,
       3,    53,    -1,    17,    53,    -1,    20,    53,    -1,    53,
      -1,     8,    55,     9,    70,    -1,    44,    -1,    54,    -1,
      56,     5,    54,    -1,    56,     6,    54,    -1,    56,    16,
      54,    -1,    56,    -1,    57,     3,    56,    -1,    57,     4,
      56,    -1,    57,    -1,    58,    32,    57,    -1,    58,    33,
      57,    -1,    58,    -1,    59,    21,    58,    -1,    59,    22,
      58,    -1,    59,    31,    58,    -1,    59,    30,    58,    -1,
      59,    -1,    60,    28,    59,    -1,    60,    29,    59,    -1,
      60,    -1,    61,    13,    60,    -1,    61,    -1,    62,    15,
      61,    -1,    62,    -1,    63,    14,    62,    -1,    63,    -1,
      64,    24,    63,    -1,    64,    -1,    65,    25,    64,    -1,
      65,    -1,    65,    18,    69,    19,    66,    -1,    66,    -1,
      70,    68,    67,    -1,    71,     7,    67,    -1,     7,    -1,
      34,    -1,    35,    -1,    36,    -1,    37,    -1,    38,    -1,
      39,    -1,    40,    -1,    41,    -1,    42,    -1,    43,    -1,
      67,    -1,    69,    23,    67,    -1,    46,    -1,    45,    10,
      69,    11,    -1,    48,    -1,    70,    12,    69,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    74,    74,   140,   141,   142,   144,   149,   156,   162,
     176,   178,   185,   193,   194,   203,   204,   205,   206,   226,
     227,   245,   249,   250,   251,   252,   256,   257,   258,   262,
     263,   264,   268,   269,   270,   271,   272,   276,   277,   278,
     282,   283,   287,   288,   292,   293,   297,   298,   302,   303,
     307,   308,   312,   315,   357,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   389,   390,   394,   395,
     402,   403
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PTOK_PLUS", "PTOK_MINUS",
  "PTOK_ASTERIX", "PTOK_SLASH", "PTOK_EQUAL", "PTOK_LEFT_PAREN",
  "PTOK_RIGHT_PAREN", "PTOK_LEFT_BRACKET", "PTOK_RIGHT_BRACKET",
  "PTOK_DOT", "PTOK_AMPERSAND", "PTOK_PIPE", "PTOK_CIRCUM", "PTOK_PERCENT",
  "PTOK_TILDE", "PTOK_QUESTION", "PTOK_COLON", "PTOK_EXCLAMATION",
  "PTOK_LESS", "PTOK_GREATHER", "PTOK_COMMA", "PTOK_AND_OP", "PTOK_OR_OP",
  "PTOK_INC_OP", "PTOK_DEC_OP", "PTOK_EQ_OP", "PTOK_NE_OP", "PTOK_GE_OP",
  "PTOK_LE_OP", "PTOK_LEFT_OP", "PTOK_RIGHT_OP", "PTOK_MUL_ASSIGN",
  "PTOK_DIV_ASSIGN", "PTOK_MOD_ASSIGN", "PTOK_ADD_ASSIGN",
  "PTOK_SUB_ASSIGN", "PTOK_LEFT_ASSIGN", "PTOK_RIGHT_ASSIGN",
  "PTOK_AND_ASSIGN", "PTOK_XOR_ASSIGN", "PTOK_OR_ASSIGN", "PTOK_INT",
  "PTOK_MEMORY_OBJECT", "PTOK_MEMORY", "PTOK_NUMBER", "PTOK_BIT",
  "$accept", "ucsim_grammar", "primary_expr", "postfix_expr", "unary_expr",
  "cast_expr", "type_name", "multiplicative_expr", "additive_expr",
  "shift_expr", "relational_expr", "equality_expr", "and_expr",
  "exclusive_or_expr", "inclusive_or_expr", "logical_and_expr",
  "logical_or_expr", "conditional_expr", "assignment_expr",
  "assignment_operator", "expr", "memory", "bit", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    51,    51,    51,    51,    52,    52,    52,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    54,
      54,    55,    56,    56,    56,    56,    57,    57,    57,    58,
      58,    58,    59,    59,    59,    59,    59,    60,    60,    60,
      61,    61,    62,    62,    63,    63,    64,    64,    65,    65,
      66,    66,    67,    67,    67,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    69,    69,    70,    70,
      71,    71
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     3,     1,     2,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       4,     1,     1,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     5,     1,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     4,
       1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     5,    70,     0,     7,    10,    19,    22,    26,    29,
      32,    37,    40,    42,    44,    46,    48,    50,    52,    66,
       2,     3,     4,     0,    16,     3,     4,    15,    21,     0,
       0,    13,    14,    17,    18,    11,    12,     0,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     8,     9,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,     0,     0,     0,     6,     0,    23,    24,
      25,    27,    28,    30,    31,    33,    34,    36,    35,    38,
      39,    41,    43,    45,    47,     0,    49,    67,    71,    53,
      54,    20,    69,     0,    51
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    39,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    83,
      40,    35,    36
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -64
static const yytype_int8 yypact[] =
{
      74,    89,    89,    10,    -2,    89,    89,    16,    16,    22,
     -64,   -64,   -64,    12,   -64,   -64,   -64,   -64,    29,    48,
      63,    19,    70,    13,    14,    24,    18,    -3,   -64,   -64,
      42,   105,    62,    74,   -64,   -10,   -64,   -64,   -64,    64,
       2,    76,   -64,   -64,   -64,   -64,   -64,    74,   -64,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,   -64,
      74,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,    74,    74,    16,   -64,     8,   -64,   -64,
     -64,    29,    29,    48,    48,    63,    63,    63,    63,    19,
      19,    70,    13,    14,    24,     5,    18,   -64,    42,   -64,
     -64,   -64,   -64,    74,   -64
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -64,   -64,   -64,   -64,    58,    30,   -64,    51,    53,    67,
      50,    43,    23,    11,    49,    46,   -64,    15,   -63,   -64,
       1,     0,     6
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      31,    30,    70,    31,    41,   107,    32,    45,    46,    32,
      42,    86,    48,     1,     2,    66,    71,    72,     3,   112,
     109,   110,    67,     4,   113,    68,    62,     5,    68,    63,
       6,    68,    47,    31,    49,    50,     7,     8,    64,    32,
      56,    57,    65,     9,    10,    51,    12,    31,    87,    58,
      59,    52,    53,    32,    38,     9,    10,    11,    12,    34,
      37,     9,    10,    43,    44,    68,    31,   105,    31,    84,
      31,   108,    32,    85,    32,   103,    32,     1,     2,    88,
      89,    90,     3,    31,    31,   111,   102,     4,    70,    32,
      32,     5,     1,     2,     6,    54,    55,    33,    60,    61,
       7,     8,     4,    91,    92,   101,     5,    93,    94,     6,
      99,   100,    69,   106,   104,     7,     8,    70,     0,     9,
      10,    11,    12,    95,    96,    97,    98,     0,   114,     0,
       0,    71,    72,     0,     9,    10,    11,    12,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-64))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
       0,     0,    12,     3,     4,    68,     0,     7,     8,     3,
       4,     9,     0,     3,     4,    18,    26,    27,     8,    11,
      83,    84,    25,    13,    19,    23,    13,    17,    23,    15,
      20,    23,    10,    33,     5,     6,    26,    27,    14,    33,
      21,    22,    24,    45,    46,    16,    48,    47,    47,    30,
      31,     3,     4,    47,    44,    45,    46,    47,    48,     1,
       2,    45,    46,     5,     6,    23,    66,    66,    68,     7,
      70,    70,    66,     9,    68,    64,    70,     3,     4,    49,
      50,    51,     8,    83,    84,    85,    63,    13,    12,    83,
      84,    17,     3,     4,    20,    32,    33,     8,    28,    29,
      26,    27,    13,    52,    53,    62,    17,    54,    55,    20,
      60,    61,     7,    67,    65,    26,    27,    12,    -1,    45,
      46,    47,    48,    56,    57,    58,    59,    -1,   113,    -1,
      -1,    26,    27,    -1,    45,    46,    47,    48,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     8,    13,    17,    20,    26,    27,    45,
      46,    47,    48,    50,    51,    52,    53,    54,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      69,    70,    71,     8,    53,    70,    71,    53,    44,    55,
      69,    70,    71,    53,    53,    70,    70,    10,     0,     5,
       6,    16,     3,     4,    32,    33,    21,    22,    30,    31,
      28,    29,    13,    15,    14,    24,    18,    25,    23,     7,
      12,    26,    27,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    68,     7,     9,     9,    69,    54,    54,
      54,    56,    56,    57,    57,    58,    58,    58,    58,    59,
      59,    60,    61,    62,    63,    69,    64,    67,    69,    67,
      67,    70,    11,    19,    66
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 74 "cmdpars.y"
    { application->dd_printf("%d\n", (yyvsp[(1) - (1)].number)); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 140 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].memory).memory->read((yyvsp[(1) - (1)].memory).address); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 141 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (1)].bit).memory->read((yyvsp[(1) - (1)].bit).mem_address) & (yyvsp[(1) - (1)].bit).mask)?1:0; }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 142 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 144 "cmdpars.y"
    { (yyval.number)= (yyvsp[(2) - (3)].number); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 149 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 157 "cmdpars.y"
    {
	  (yyval.number)= (yyvsp[(1) - (2)].memory).memory->read((yyvsp[(1) - (2)].memory).address);
	  (yyvsp[(1) - (2)].memory).memory->write((yyvsp[(1) - (2)].memory).address, (yyval.number)+1);
	}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 163 "cmdpars.y"
    {
	  (yyval.number)= (yyvsp[(1) - (2)].memory).memory->read((yyvsp[(1) - (2)].memory).address);
	  (yyvsp[(1) - (2)].memory).memory->write((yyvsp[(1) - (2)].memory).address, (yyval.number)-1);
	}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 176 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 179 "cmdpars.y"
    {
	  (yyval.number)= (yyvsp[(2) - (2)].memory).memory->read((yyvsp[(2) - (2)].memory).address);
	  (yyvsp[(2) - (2)].memory).memory->write((yyvsp[(2) - (2)].memory).address, (yyval.number)+1);
	  (yyval.number)= (yyvsp[(2) - (2)].memory).memory->read((yyvsp[(2) - (2)].memory).address);
	}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 186 "cmdpars.y"
    {
	  (yyval.number)= (yyvsp[(2) - (2)].memory).memory->read((yyvsp[(2) - (2)].memory).address);
	  (yyvsp[(2) - (2)].memory).memory->write((yyvsp[(2) - (2)].memory).address, (yyval.number)-1);
	  (yyval.number)= (yyvsp[(2) - (2)].memory).memory->read((yyvsp[(2) - (2)].memory).address);
	}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 193 "cmdpars.y"
    { (yyval.number)= (yyvsp[(2) - (2)].memory).address; }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 195 "cmdpars.y"
    {
	  (yyval.number)= (yyvsp[(2) - (2)].bit).bit_address;
	  if ((yyval.number) < 0)
	    {
	      yyerror("Bit has no address.");
	      (yyval.number)= 0;
	    }
	}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 203 "cmdpars.y"
    { (yyval.number)= -(yyvsp[(2) - (2)].number); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 204 "cmdpars.y"
    { (yyval.number)= +(yyvsp[(2) - (2)].number); }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 205 "cmdpars.y"
    { (yyval.number)= ~(yyvsp[(2) - (2)].number); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 206 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(2) - (2)].number))?0:1; }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 226 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 228 "cmdpars.y"
    {
	  (yyval.number)= (yyvsp[(4) - (4)].memory).memory->read((yyvsp[(4) - (4)].memory).address);
	  if ((yyvsp[(2) - (4)].number) == PTOK_INT)
	    {
	      u32_t smask;
	      smask= 1 << ((yyvsp[(4) - (4)].memory).memory->width - 1);
	      if ((yyval.number) & smask)
		{
		  u32_t mask;
		  mask= -1 & ~((yyvsp[(4) - (4)].memory).memory->data_mask);
		  (yyval.number)= (yyval.number) | mask;
		}
	    }
	}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 245 "cmdpars.y"
    { (yyval.number)= PTOK_INT; }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 249 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 250 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) * (yyvsp[(3) - (3)].number); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 251 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) / (yyvsp[(3) - (3)].number); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 252 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) % (yyvsp[(3) - (3)].number); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 256 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 257 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) + (yyvsp[(3) - (3)].number); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 258 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) - (yyvsp[(3) - (3)].number); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 262 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 263 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) << (yyvsp[(3) - (3)].number); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 264 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) >> (yyvsp[(3) - (3)].number); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 268 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 269 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (3)].number) < (yyvsp[(3) - (3)].number))?1:0; }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 270 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (3)].number) > (yyvsp[(3) - (3)].number))?1:0; }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 271 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (3)].number) <= (yyvsp[(3) - (3)].number))?1:0; }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 272 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (3)].number) >= (yyvsp[(3) - (3)].number))?1:0; }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 276 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 277 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (3)].number)==(yyvsp[(3) - (3)].number))?1:0; }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 278 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (3)].number)!=(yyvsp[(3) - (3)].number))?1:0; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 282 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 283 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) & (yyvsp[(3) - (3)].number); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 287 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 288 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) ^ (yyvsp[(3) - (3)].number); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 292 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 293 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (3)].number) | (yyvsp[(3) - (3)].number); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 297 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 298 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (3)].number) && (yyvsp[(3) - (3)].number))?1:0; }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 302 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 303 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (3)].number) || (yyvsp[(3) - (3)].number))?1:0; }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 307 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 308 "cmdpars.y"
    { (yyval.number)= ((yyvsp[(1) - (5)].number))?((yyvsp[(3) - (5)].number)):((yyvsp[(5) - (5)].number)); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 312 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 316 "cmdpars.y"
    {
	  t_mem org= (yyvsp[(1) - (3)].memory).memory->read((yyvsp[(1) - (3)].memory).address);
	  (yyval.number)= (yyvsp[(3) - (3)].number);
	  switch ((yyvsp[(2) - (3)].number))
	    {
	    case PTOK_EQUAL:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_MUL_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org *= (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_DIV_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org /= (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_MOD_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org %= (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_ADD_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org += (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_SUB_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org -= (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_LEFT_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org <<= (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_RIGHT_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org >>= (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_AND_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org &= (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_XOR_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org ^= (yyvsp[(3) - (3)].number));
	      break;
	    case PTOK_OR_ASSIGN:
	      (yyvsp[(1) - (3)].memory).memory->write((yyvsp[(1) - (3)].memory).address, org |= (yyvsp[(3) - (3)].number));
	      break;
	    }
	  (yyval.number)= (yyvsp[(1) - (3)].memory).memory->read((yyvsp[(1) - (3)].memory).address);
	}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 358 "cmdpars.y"
    {
	  if ((yyvsp[(3) - (3)].number))
	    {
	      (yyvsp[(1) - (3)].bit).memory->write((yyvsp[(1) - (3)].bit).mem_address,
			       (yyvsp[(1) - (3)].bit).memory->read((yyvsp[(1) - (3)].bit).mem_address) | (yyvsp[(1) - (3)].bit).mask);
	      (yyval.number)= 1;
	    }
	  else
	    {
	      (yyvsp[(1) - (3)].bit).memory->write((yyvsp[(1) - (3)].bit).mem_address,
			       (yyvsp[(1) - (3)].bit).memory->read((yyvsp[(1) - (3)].bit).mem_address) & ~((yyvsp[(1) - (3)].bit).mask));
	      (yyval.number)= 0;
	    }
	}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 375 "cmdpars.y"
    { (yyval.number)= PTOK_EQUAL; }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 376 "cmdpars.y"
    { (yyval.number)= PTOK_MUL_ASSIGN; }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 377 "cmdpars.y"
    { (yyval.number)= PTOK_DIV_ASSIGN; }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 378 "cmdpars.y"
    { (yyval.number)= PTOK_MOD_ASSIGN; }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 379 "cmdpars.y"
    { (yyval.number)= PTOK_ADD_ASSIGN; }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 380 "cmdpars.y"
    { (yyval.number)= PTOK_SUB_ASSIGN; }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 381 "cmdpars.y"
    { (yyval.number)= PTOK_LEFT_ASSIGN; }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 382 "cmdpars.y"
    { (yyval.number)= PTOK_RIGHT_ASSIGN; }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 383 "cmdpars.y"
    { (yyval.number)= PTOK_AND_ASSIGN; }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 384 "cmdpars.y"
    { (yyval.number)= PTOK_XOR_ASSIGN; }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 385 "cmdpars.y"
    { (yyval.number)= PTOK_OR_ASSIGN; }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 389 "cmdpars.y"
    { (yyval.number)= (yyvsp[(1) - (1)].number); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 390 "cmdpars.y"
    { (yyval.number)= (yyvsp[(3) - (3)].number); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 396 "cmdpars.y"
    {
	    (yyval.memory).memory= (yyvsp[(1) - (4)].memory_object);
	    (yyval.memory).address= (yyvsp[(3) - (4)].number);
	  }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 404 "cmdpars.y"
    {
	    (yyval.bit).memory= (yyvsp[(1) - (3)].memory).memory;
	    (yyval.bit).mem_address= (yyvsp[(1) - (3)].memory).address;
	    (yyval.bit).mask= 1 << (yyvsp[(3) - (3)].number);
	    (yyval.bit).bit_address= -1;
	    class cl_uc *uc= application->get_uc();
	    if (uc)
	      (yyval.bit).bit_address= uc->bit_address((yyvsp[(1) - (3)].memory).memory, (yyvsp[(1) - (3)].memory).address, (yyvsp[(3) - (3)].number));
	  }
    break;



/* Line 1806 of yacc.c  */
#line 2163 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 415 "cmdpars.y"


static void
yyerror (const char *msg)
{
  application->dd_printf ("Parser error: %s\n", msg);
}

