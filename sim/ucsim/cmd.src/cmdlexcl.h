#ifndef CMDLEXCL_HEADER
#define CMDLEXCL_HEADER

#include <stdlib.h>
//#ifndef __FLEX_LEXER_H
//#include <FlexLexer.h>
//#endif
#include "flex_lexer.h"
#include "i_string.h"

#include "pobjcl.h"


class cl_ucsim_lexer: public yyFlexLexer
{
protected:
  char *string_to_parse;
  char *string_ptr;
public:
  cl_ucsim_lexer(void): yyFlexLexer()
  {
    string_to_parse= string_ptr= 0;
  }
  cl_ucsim_lexer(char *str): yyFlexLexer()
  {
    string_to_parse= string_ptr= strdup(str);
  }
  virtual ~cl_ucsim_lexer(void)
  {
    if (string_to_parse)
      {
	free(string_to_parse);
	string_to_parse= 0;
      }
  }
  void activate_lexer_to_parse_into(void *yylv);
protected:
  int LexerInput(char *buf, int max_size);
public:
  int check_id(char *token);
};


#endif
