/* SDCCerr.c - error handling routines */

#include "common.h"

#define USE_STDOUT_FOR_ERRORS		0

#if USE_STDOUT_FOR_ERRORS
#define ERRSINK		stdout
#else
#define ERRSINK		stderr
#endif

#define ERROR		0
#define WARNING		1

struct  {
	int		errType ;
	char	*errText ;
} ErrTab [] =
{
{  ERROR  ,"error *** Duplicate symbol '%s', symbol IGNORED\n"		      },
{ ERROR  ,"error *** Syntax Error Declaration ingonerd\n"			      },
{ ERROR  ,"error *** Constant Expected Found Variable\n"                          },
{ ERROR  ,"error *** 'malloc' failed file '%s' for size %ld\n"	              },
{ ERROR  ,"error *** 'fopen' failed on file '%s'\n"		                      },
{ ERROR  ,"error *** Internal Error Oclass invalid '%s'\n"			      },
{ ERROR  ,"error *** Cannot allocate variable '%s'.\n"			      },
{ ERROR  ,"error *** Old style C declaration. IGNORED '%s'\n"		      },
{ ERROR  ,"error *** Out of stack Space. '%s' not allocted\n"		      },
{ ERROR  ,"error *** FATAL Compiler Internal Error in file '%s' line number '%d' : %s \nContact Author with source code\n" },
{ ERROR  ,"error *** 'lvalue' required for '%s' operation .\n"		      },
{ ERROR  ,"error *** Creation of temp file failed\n"			      },
{ ERROR  ,"error *** Function expected here '%s'\n"				      },
{ ERROR  ,"error *** 'using', 'interrupt' or 'reentrant' must follow a function definiton .'%s'\n"},
{ ERROR  ,"error *** Absolute address & initial value both cannot be specified for\n a 'sfr','sbit' storage class, initial value ignored '%s'\n"  },
{ WARNING,"warning *** Variable in the storage class cannot be initialized.'%s'\n"},
{ WARNING,"warning *** storage class not allowed for automatic variable '%s' in reentrant function\n"   },
{ ERROR  ,"error *** absolute address not allowed for automatic var '%s' in reentrant function \n"	      },
{ WARNING,"warning *** Initializer different levels of indirections\n"	      },
{ ERROR  ,"error *** Function name '%s' redefined \n"			      },
{ ERROR  ,"error *** Undefined identifier '%s'\n"                                 },
{ WARNING,"warning *** stack exceeds 256 bytes for function '%s'\n"},
{ ERROR  ,"error *** Array or pointer required for '%s' operation \n"	      },
{ ERROR  ,"error *** Array index not an integer\n"                                },
{ ERROR  ,"error *** Array bound Exceeded, assuming zero\n"		              },
{ ERROR  ,"error *** Structure/Union expected left of '.%s'\n"                    },
{ ERROR  ,"error *** '%s' not a structure/union member\n"                         },
{ ERROR  ,"error *** Pointer required\n"	                                      },
{ ERROR  ,"error *** 'unary %c': illegal operand\n"	                              },
{ ERROR  ,"error *** convertion error: integral promotion failed\n"	              },
{ ERROR  ,"error *** type must be INT for bit field definition\n"	              },
{ ERROR  ,"error *** bit field size greater than 16 . assuming 16\n"              },
{ WARNING,"warning *** high order truncation might occur\n"	                      },
{ ERROR  ,"error *** Attempt to assign value to a constant variable %s\n"         },
{ ERROR  ,"error *** Lvalue specifies constant object\n"                          },
{ ERROR  ,"error *** '&' illegal operand , %s\n"                                  },
{ ERROR  ,"error *** illegal cast (cast cannot be aggregate)\n"                   },
{ ERROR  ,"error *** '*' bad operand\n"                                           },
{ ERROR  ,"error *** Argument count error, argument ignored\n"                    },
{ ERROR  ,"error *** Function was expecting more arguments\n"                     },
{ ERROR  ,"error *** Function name expected '%s'.ANSI style declaration REQUIRED\n"},
{ ERROR  ,"error *** invalid operand '%s'\n"					      },
{ ERROR  ,"error *** pointer + pointer invalid\n"				      },
{ ERROR  ,"error *** invalid operand for shift operator\n"			      },
{ ERROR  ,"error *** compare operand cannot be struct/union\n"		      },
{ ERROR  ,"error *** operand invalid for bitwise operation\n"		      },
{ ERROR  ,"error *** Invalid operand for '&&' or '||'\n"			      },
{ WARNING,"warning *** indirections to different types %s %s \n"		      },
{ ERROR  ,"error *** cannot assign values to aggregates\n"			      },
{ ERROR  ,"error *** bit Arrays can be accessed by literal index only\n"	      },
{ ERROR  ,"error *** Array or Pointer to bit|sbit|sfr not allowed.'%s'\n"	      },
{ ERROR  ,"error *** typedef/enum '%s' duplicate.Previous definiton Ignored\n"    },
{ ERROR  ,"error *** Actual Argument type different from declaration %d\n"	      },
{ ERROR  ,"error *** Function return value mismatch\n"			      },
{ ERROR  ,"error *** Function cannot return aggregate. Func body ignored\n"	      },
{ ERROR  ,"error *** ANSI Style declaration needed\n"			      },
{ ERROR  ,"error *** Label name redefined '%s'\n"				      },
{ ERROR  ,"error *** Label undefined '%s'\n"				      },
{ ERROR  ,"error *** void function returning value\n"			      },
{ ERROR  ,"error *** function '%s' must return value\n"			      },
{ WARNING,"warning *** function return value mismatch\n"			      },
{ ERROR  ,"error *** 'case/default' found without 'switch'.statement ignored\n"   },
{ ERROR  ,"error *** 'case' expression not constant. statement ignored\n"	      },
{ ERROR  ,"error *** 'break/continue' statement out of context\n"		      },
{ ERROR  ,"error *** nonintegral used in switch expression\n"		      },
{ ERROR  ,"error *** function '%s' already has body\n"			      },
{ ERROR  ,"error *** attempt to allocate variable of unknown size '%s'\n"	      },
{ ERROR  ,"error *** aggregate 'auto' variable '%s' cannot be initialized\n"      },
{ ERROR  ,"error *** too many initializers\n"				      },
{ ERROR  ,"error *** struct/union/array '%s' :initialization needs curly braces\n"},
{ ERROR  ,"error *** non-address initialization expression\n"		      },
{ ERROR  ,"error *** interrupt no '%d' already has a service routine '%s'\n"	      },
{ ERROR  ,"error *** interrupt routine cannot have arguments, arguments ingored\n"},
{ ERROR  ,"error *** critical compiler #include file missing.		\n"   },
{ ERROR  ,"error *** function 'main' undefined\n"                                 },
{ ERROR  ,"error *** 'extern' variable '%s' cannot be initialised	\n"	      },
{ ERROR  ,"error *** Pre-Processor %s\n"					      },
{ ERROR  ,"error *** _dup call failed\n"                                          },
{ WARNING,"warning *** pointer being cast to incompatible type \n"                        },
{ WARNING,"warning *** 'while' loop with 'zero' constant.loop eliminated\n"	      },
{ WARNING,"warning *** %s expression has NO side effects.expr eliminated\n"	      },
{ WARNING,"warning *** constant value '%s', out of range.\n"		      },
{ WARNING,"warning *** comparison will either, ALWAYs succeed or ALWAYs fail\n"   },
{ ERROR  ,"error *** Compiler Terminating , contact author with source\n"	      },
{ WARNING,"warning *** 'auto' variable '%s' may be used before initialization at %s(%d)\n"      },
{ WARNING,"warning *** in function %s unreferenced %s : '%s'\n"			      },
{ ERROR  ,"error *** unknown size for operand\n"				      },
{ WARNING,"warning *** '%s' 'long' not supported , declared as 'int' .\n"	      },
{ WARNING,"warning *** LITERAL value being cast to '_generic' pointer\n"	      },
{ ERROR  ,"error *** %s '%s' address out of range\n"				      },
{ ERROR  ,"error *** storage class CANNOT be specified for bit variable '%s'\n"    },
{ WARNING,"warning *** extern definition for '%s' mismatches with declaration.\n"  },
{ WARNING,"warning *** Functions called via pointers must be 'reentrant' to take arguments\n"	},
{ WARNING,"warning *** type 'double' not supported assuming 'float'\n"		      },
{ WARNING,"warning *** if-statement condition always false.if-statement not generated\n"},
{ WARNING,"warning *** no 'return' statement found for function '%s'\n"	      },
{ WARNING,"warning *** Pre-Processor %s\n"					      },
{ WARNING,"warning *** structure '%s' passed as function argument changed to pointer\n"},
{ ERROR  ,"error *** conflict with previous definition of '%s' for attribute '%s'\n" },
{ ERROR  ,"error *** variable '%s' declared in code space must have initialiser\n" },
{ ERROR  ,"error *** operands not integral for assignment operation\n"            },
{ ERROR  ,"error *** too many parameters \n"                                      },
{ ERROR  ,"error *** to few parameters\n"                                         },
{ ERROR  ,"error *** code not generated for '%s' due to previous errors\n"},
{ WARNING,"warning *** type mismatch for parameter number %d\n"},
{ ERROR  ,"error *** invalid float constant '%s'\n"},
{ ERROR  ,"error *** invalid operand for '%s' operation\n"},
{ ERROR  ,"error *** switch value not an integer\n"},
{ ERROR  ,"error *** case label not an integer\n"},
{ WARNING,"warning *** function '%s' too large for global optimization\n"},
{ WARNING,"warning *** conditional flow changed by optimizer '%s(%d)':so said EVELYN the modified DOG\n"},
{ WARNING,"warning *** invalid type specifier for pointer type specifier ignored\n"},
{ WARNING,"warning *** function '%s' implicit declaration\n"},
{ WARNING,"warning *** %s"},
{ WARNING,"info *** %s extended by %d bytes for compiler temp(s) :in function  '%s': %s \n"},
{ WARNING,"warning *** unknown or unsupported #pragma directive '%s'\n"},
{ WARNING,"warning *** %s shifting more than size of object changed to zero\n"},
{ WARNING,"warning *** unknown compiler option '%s' ignored\n"},
{ WARNING,"warning *** option '%s' no longer supported  '%s' \n"},
{ WARNING,"warning *** don't know what to do with file '%s'. file extension unsupported\n"},
{ WARNING,"warning *** cannot compile more than one source file . file '%s' ignored\n"},
{ WARNING,"info *** function '%s', # edges %d , # nodes %d , cyclomatic complexity %d\n"},
{ ERROR  ,"error *** dividing by ZERO\n"},
{ ERROR  ,"error *** function cannot return 'bit'\n"},
{ ERROR  ,"error *** casting from to type 'void' is illegal\n"},
{ WARNING,"warning *** constant is out of range %s\n" },
{ WARNING,"warning *** unreachable code %s(%d)\n"},
{ WARNING,"warning *** non-pointer type cast to _generic pointer\n"},
{ WARNING,"warning *** possible code generation error at line %d,\n send source to sandeep.dutta@usa.net\n"},
{ WARNING,"warning *** pointer types incompatible \n" },
{ WARNING,"warning *** unknown memory model at %s : %d\n" },
{ ERROR  ,"error   *** cannot generate code for target '%s'\n"},
{ WARNING,"warning *** Indirect call to a banked function not implemented.\n"},
{ WARNING,"warning *** Model '%s' not supported for %s, ignored.\n"},
{ WARNING,"warning *** Both banked and nonbanked attributes used.  nonbanked wins.\n"},
{ WARNING,"warning *** Both banked and static used.  static wins.\n"},
{ WARNING,"warning *** converting integer type to generic pointer: assuming XDATA\n"}
};

void	vwerror (int errNum, va_list marker)
{
    if ( ErrTab[errNum].errType== ERROR )
	fatalError++ ;
    
    if ( filename && lineno ) {
	fprintf(ERRSINK, "%s(%d):",filename,lineno);
    } else {
        fprintf(ERRSINK, "at %d:",lineno);
    }
    vfprintf(ERRSINK, ErrTab[errNum].errText,marker);
}

/****************************************************************************/
/* werror - writes an error to the listing file & to standarderr	    */
/****************************************************************************/
void	werror (int errNum, ... )
{
    va_list	marker;
    va_start(marker,errNum);
    vwerror(errNum, marker);
    va_end( marker );
}

