%{
#include "cmdlexcl.h"
#include "cmdpars.h"

static YY_cl_ucsim_parser_STYPE *yylval;

int yywrap(void);

#define tok(x) (YY_cl_ucsim_parser_CLASS::x)
#define lexer_obj (dynamic_cast<class cl_ucsim_lexer *>(this))

%}

%%

[ \t]*			;

"+"			return(tok(PTOK_PLUS));
"-"			return(tok(PTOK_MINUS));
"*"			return(tok(PTOK_ASTERIX));
"/"			return(tok(PTOK_SLASH));
"("			return(tok(PTOK_LEFT_PAREN));
")"			return(tok(PTOK_RIGHT_PAREN));
"["			return(tok(PTOK_LEFT_BRACKET));
"]"			return(tok(PTOK_RIGHT_BRACKET));
"="			return(tok(PTOK_EQUAL));
"."			return(tok(PTOK_DOT));
"&"			return(tok(PTOK_AMPERSAND));

([0-9]+)|(0x[0-9a-fA-F]+) {
  yylval->number= strtol(yytext, 0, 0);
  return(YY_cl_ucsim_parser_CLASS::PTOK_NUMBER);
}

[a-zA-Z_][0-9a-zA-Z_]*	return(lexer_obj->check_id(yytext));

.			;

%%

#undef lexer_obj

int
yywrap(void)
{
  return(1);
}

#include "cmdlexcl.h"
#include "globals.h"

int
cl_ucsim_lexer::LexerInput(char *buf, int max_size)
{
  if (!string_to_parse)
    return(yyFlexLexer::LexerInput(buf, max_size));
  int lrem= strlen(string_ptr);
  int n= max_size;
  if (lrem < max_size)
    n= lrem;
  strncpy(buf, string_ptr, n);
  string_ptr+= n;
  return(n);
}

void
cl_ucsim_lexer::activate_lexer_to_parse_into(void *yylv)
{
  yylval= (YY_cl_ucsim_parser_STYPE *)yylv;
}

int
cl_ucsim_lexer::check_id(char *token)
{
  class cl_uc *uc= application->get_uc();
  //printf("checking id=\"%s\"\n",token);

  if (uc)
    {
      class cl_memory *mem= uc->memory(token);
      if (mem)
	{
	  yylval->memory_object= mem;
	  return(tok(PTOK_MEMORY_OBJECT));
	}
      
      t_addr addr;
      bool found= uc->symbol2address(yytext, uc->sfr_tbl(), &addr);
      if (found)
	{
	  /*yylval->number= addr;
	    return(tok(PTOK_NUMBER));*/
	  yylval->memory.memory= uc->address_space(MEM_SFR_ID);
	  yylval->memory.address= addr;
	  return(tok(PTOK_MEMORY));
	}

      found= uc->symbol2address(yytext, uc->bit_tbl(), &addr);
      if (found)
	{
	  t_addr memaddr;
	  t_mem mask;
	  yylval->bit.memory= uc->bit2mem(addr, &memaddr, &mask);
	  yylval->bit.mem_address= memaddr;
	  yylval->bit.bit_address= addr;
	  yylval->bit.mask= mask;
	  return(tok(PTOK_BIT));
	}
    }

  return(0);
}
