/** @file altlex.c
    An alternate lexer to SDCC.lex.
    In development - ie messy and just plain wrong.
*/
#include "common.h"
#include <assert.h>

FILE *yyin;

int yylineno;
int column;
char *currFname;
char *yytext;

/* Right.  What are the parts of the C stream?  From SDCC.lex:
   D = [0..9]		digits
   L = [a..z A..Z _]	alphanumerics and _
   H = [a..f A..F 0-9]	Hex digits
   E = [eE+-0-9]	Digits in a float
   FS = [fFlL]		Specifiers for a float
   IS = [uUlL]		Specifiers for a int

   L[LD]*      		A 'token' - cant think of a good name
   			Check tokens against the reserved words.
			If match 
				return the token id.
			else 
				If in the typedef table, do stuff...
				Blah.  See check_type()
   0[xX]{H}+		Hex number - PENDING: specifiers
   0{D}+		Octal number - PENDING: specifiers
   {D}+			Decimal - PENDING: specifiers
   Floats		PENDING
   
   Exceptions:
   Comment start	Strip until end of comment.
   ...			Ellipses

   So the inputs are:
      Skip whitespace
      switch class
	 L	Try to read a token
	 D	Try to read a number
	 Punct	Try to read punct
*/	

char linebuf[10000];
int linepos, linelen;

#define INLINE	inline

static int underflow(void)
{
    linelen = fread(linebuf, 1, sizeof(linebuf), yyin);
    if (linelen <= 0)
	return EOF;
    linepos = 0;
    return linebuf[linepos++];
}

static int INLINE ygetc(void)
{
    if (linepos < linelen)
	return linebuf[linepos++];
    else
	return underflow();
};

static int INLINE yungetc(int c)
{
    linebuf[--linepos] = c;
    return 0;
}

#define GETC()		ygetc()
#define UNGETC(_a)	yungetc(_a)

//#define GETC()		fgetc(yyin);
//#define UNGETC(_a)	ungetc(_a, yyin)
#define ISL(_a)		(isalnum(_a) || _a == '_')
#define ISALNUM(_a)	isalnum(_a)
#define ISHEX(_a)	isalnum(_a)

char *stringLiteral (void)
{
    static char line[1000];
    int ch;
    char *str = line;
       
    *str++ = '\"'			;
    /* put into the buffer till we hit the */
    /* first \" */
    while (1) {

	ch = GETC();
	if (!ch) 	    break	; /* end of input */
	/* if it is a \ then everything allowed */
	if (ch == '\\') {
	    *str++ = ch     ; /* backslash in place */
	    *str++ = GETC()		; /* following char in place */
	    continue			;      /* carry on */
	}
	     
	/* if new line we have a new line break */
	if (ch == '\n') break		;
	 
	/* if this is a quote then we have work to do */
	/* find the next non whitespace character     */
	/* if that is a double quote then carry on    */
	if (ch == '\"') {
	 
	    while ((ch = GETC()) && isspace(ch)) ;
	    if (!ch) break		; 
	    if (ch != '\"') {
		UNGETC(ch)			;
		break			;
	    }
		  
	    continue		;
        }
	*str++  = ch;	  
    }  
    *str++ = '\"'			;
    *str = '\0';
    return line;
}

void discard_comments(int type)
{
    int c;
    if (type == '*') {
	do {
	    c = GETC();
	    if (c == '*') {
		c = GETC();
		if (c == '/')
		    return;
	    }
	    else if (c == EOF)
		return;
	} while (1);
    }
    else if (type == '/') {
	while (c != '\n' && c != EOF) {
	    c = GETC();
	}
    }
    else {
	assert(0);
    }
}

#define TKEYWORD(_a)	return _a

int check_token(const char *sz)
{
    if (!strcmp(sz, "at")) {
	TKEYWORD(AT)  ; }
    
    else if (!strcmp(sz, "auto")) {
	 return(AUTO); }
    
    else if (!strcmp(sz, "bit")) {
	 TKEYWORD(BIT) ; }
    
    else if (!strcmp(sz, "break")) {
	 return(BREAK); }

    else if (!strcmp(sz, "case")) {
	 return(CASE); }

    else if (!strcmp(sz, "char")) {
	 return(CHAR); }

    else if (!strcmp(sz, "code")) {
	 TKEYWORD(CODE); }

    else if (!strcmp(sz, "const")) {
	 return(CONST); }

    else if (!strcmp(sz, "continue")) {
	 return(CONTINUE); }

    else if (!strcmp(sz, "critical")) {
	 TKEYWORD(CRITICAL); } 

    else if (!strcmp(sz, "data")) {
	 TKEYWORD(DATA);   }

    else if (!strcmp(sz, "default")) {
	 return(DEFAULT); }

    else if (!strcmp(sz, "do")) {
	 return(DO); }

    else if (!strcmp(sz, "double")) {
	 werror(W_DOUBLE_UNSUPPORTED);return(FLOAT); }

    else if (!strcmp(sz, "else")) {
	 return(ELSE); }

    else if (!strcmp(sz, "enum")) {
	 return(ENUM); }

    else if (!strcmp(sz, "extern")) {
	 return(EXTERN); }

    else if (!strcmp(sz, "far")) {
	 TKEYWORD(XDATA);  }

    else if (!strcmp(sz, "eeprom")) {
	 TKEYWORD(EEPROM);  }

    else if (!strcmp(sz, "float")) {
	 return(FLOAT); }

    else if (!strcmp(sz, "flash")) {
	 TKEYWORD(CODE);}

    else if (!strcmp(sz, "for")) {
	 return(FOR); }

    else if (!strcmp(sz, "goto")) {
	 return(GOTO); }

    else if (!strcmp(sz, "idata")) {
	 TKEYWORD(IDATA);}

    else if (!strcmp(sz, "if")) {
	 return(IF); }

    else if (!strcmp(sz, "int")) {
	 return(INT); }

    else if (!strcmp(sz, "interrupt")) {
	 return(INTERRUPT);}

    else if (!strcmp(sz, "nonbanked")) {
	 TKEYWORD(NONBANKED);}

    else if (!strcmp(sz, "banked")) {
	 TKEYWORD(BANKED);}

    else if (!strcmp(sz, "long")) {
	 return(LONG); }

    else if (!strcmp(sz, "near")) {
	 TKEYWORD(DATA);}

    else if (!strcmp(sz, "pdata")) {
	 TKEYWORD(PDATA); }

    else if (!strcmp(sz, "reentrant")) {
	 TKEYWORD(REENTRANT);}

    else if (!strcmp(sz, "register")) {
	 return(REGISTER); }

    else if (!strcmp(sz, "return")) {
	 return(RETURN); }

    else if (!strcmp(sz, "sfr")) {
	 TKEYWORD(SFR)	; }

    else if (!strcmp(sz, "sbit")) {
	 TKEYWORD(SBIT)	; }

    else if (!strcmp(sz, "short")) {
	 return(SHORT); }

    else if (!strcmp(sz, "signed")) {
	 return(SIGNED); }

    else if (!strcmp(sz, "sizeof")) {
	 return(SIZEOF); }

    else if (!strcmp(sz, "sram")) {
	 TKEYWORD(XDATA);}

    else if (!strcmp(sz, "static")) {
	 return(STATIC); }

    else if (!strcmp(sz, "struct")) {
	 return(STRUCT); }

    else if (!strcmp(sz, "switch")) {
	 return(SWITCH); }

    else if (!strcmp(sz, "typedef")) {
	 return(TYPEDEF); }

    else if (!strcmp(sz, "union")) {
	 return(UNION); }

    else if (!strcmp(sz, "unsigned")) {
	 return(UNSIGNED); }

    else if (!strcmp(sz, "void")) {
	 return(VOID); }

    else if (!strcmp(sz, "volatile")) {
	 return(VOLATILE); }

    else if (!strcmp(sz, "using")) {
	 TKEYWORD(USING); }

    else if (!strcmp(sz, "while")) {
	 return(WHILE); }

    else if (!strcmp(sz, "xdata")) {
	 TKEYWORD(XDATA); }

    else if (!strcmp(sz, "_data")) {
	 TKEYWORD(_NEAR); }

    else if (!strcmp(sz, "_code")) {
	 TKEYWORD(_CODE); }

    else if (!strcmp(sz, "_eeprom")) {
	 TKEYWORD(_EEPROM); }

    else if (!strcmp(sz, "_flash")) {
	 TKEYWORD(_CODE); }

    else if (!strcmp(sz, "_generic")) {
	 TKEYWORD(_GENERIC); }

    else if (!strcmp(sz, "_near")) {
	 TKEYWORD(_NEAR); }

    else if (!strcmp(sz, "_sram")) {
	 TKEYWORD(_XDATA);}

    else if (!strcmp(sz, "_xdata")) {
	 TKEYWORD(_XDATA);}

    else if (!strcmp(sz, "_pdata")) {
	 TKEYWORD(_PDATA); }

    else if (!strcmp(sz, "_idata")) {
	 TKEYWORD(_IDATA); }

    /* check if it is in the typedef table */
    if (findSym(TypedefTab,NULL,sz)) {
	strcpy(yylval.yychar,sz);
	return (TYPE_NAME) ;
    }
    else   {
	strcpy (yylval.yychar,sz);
	return(IDENTIFIER);
    }
}

int yylex(void)
{
    int c;
    char line[128];
    char *p;

    c = GETC();
    while (1) {
	/* Handle comments first */
	if (c == '/') {
	    int c2 = GETC();
	    if (c2 == '*' || c2 == '/') {
		discard_comments(c2);
		c = GETC();
		continue;
	    }
	    else
		UNGETC(c2);
	}
	switch (c) {
	case EOF:
	    return 0;
	case ' ':
	case '\t':
	case '\r':
	case '\n':
	    /* Skip whitespace */
	    break;
	case 'a': case 'b': case 'c': case 'd':
	case 'e': case 'f': case 'g': case 'h':
	case 'i': case 'j': case 'k': case 'l':
	case 'm': case 'n': case 'o': case 'p':
	case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x':
	case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D':
	case 'E': case 'F': case 'G': case 'H':
	case 'I': case 'J': case 'K': case 'L':
	case 'M': case 'N': case 'O': case 'P':
	case 'Q': case 'R': case 'S': case 'T':
	case 'U': case 'V': case 'W': case 'X':
	case 'Y': case 'Z':
	case '_':
	    /* Start of a token.  Parse. */
	    p = line;
	    *p++ = c;
	    c = GETC();
	    while (ISL(c)) {
		*p++ = c;
		c = GETC();
	    }
	    *p = '\0';
	    UNGETC(c);
	    return check_token(line);
	case '0': case '1':
	case '2': case '3': case '4': case '5':
	case '6': case '7': case '8': case '9':
	    p = line;
	    *p++ = c;
	    c = GETC();
	    if (c == 'x' || c == 'X') {
		*p++ = c;
		c = GETC();
	    }
	    while (ISHEX(c)) {
		*p++ = c;
		c = GETC();
	    }
	    *p = '\0';
	    UNGETC(c);
	    yylval.val = constVal(line);
	    return CONSTANT;
	case '\"':
	    /* A string */
	    p = stringLiteral();
	    yylval.val = strVal(p);
	    return(STRING_LITERAL);
	case '\'':
	    /* ie '\n' */
	    break;
	case '#':
	    /* Assume a pragma and toast the rest of the line. */
	    c = GETC();
	    while (c != '\n') {
		c = GETC();
	    }
	    break;
	case '=':
	case '&':
	case '!':
	case '-':
	case '+':
	case '*':
	case '/':
	case '%':
	case '<':
	case '>':
	case '^':
	case '|':
	    /* Cases which can be compounds */
	    return c;
	case '{':
	    NestLevel++;
	    return c;
	case '}':
	    NestLevel--;
	    return c;
	case '.':
	    c = GETC();
	    if (c == '.') {
		c = GETC();
		if (c == '.') {
		    return VAR_ARGS;
		}
	    }
	    UNGETC(c);
	    /* Fall through */
	case ',':
	case ':':
	case '(': case ')':
	case '[': case ']':
	case '~':
	case '?':
	    /* Special characters that cant be part of a composite */
	    return c;
	default:
	    printf("Unhandled char %c\n", c);
	}
	c = GETC();
    }
    return 0;
}
