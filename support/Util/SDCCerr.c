/*
 * SDCCerr - Standard error handler
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>

#include "SDCCerr.h"


#define USE_STDOUT_FOR_ERRORS		0

#if USE_STDOUT_FOR_ERRORS
#define DEFAULT_ERROR_OUT	stdout
#else
#define DEFAULT_ERROR_OUT	stderr
#endif

static struct {
    ERROR_LOG_LEVEL logLevel;
    FILE *out;
} _SDCCERRG;

#define    PEDANTIC 0
#define    WARNING  1
#define    ERROR    2

extern char *filename ;
extern int lineno ;
extern int fatalError ;

/* Currently the errIndex field must match the position of the 
 * entry in the array. It is only included in order to make 
 * human error lookup easier.
 */
struct  
{
    int		errIndex;
    int		errType;
    char	*errText;
} ErrTab [] =
{
{ E_DUPLICATE, ERROR,
   "error *** Duplicate symbol '%s', symbol IGNORED\n" },
{ E_SYNTAX_ERROR, ERROR,
   "error *** Syntax Error Declaration ignored\n" },
{ E_CONST_EXPECTED, ERROR,
   "error *** Constant Expected Found Variable\n" },
{ E_OUT_OF_MEM, ERROR,
   "error *** 'malloc' failed file '%s' for size %ld\n" },
{ E_FILE_OPEN_ERR, ERROR,
   "error *** 'fopen' failed on file '%s'\n" },
{ E_INVALID_OCLASS, ERROR,
   "error *** Internal Error Oclass invalid '%s'\n" },
{ E_CANNOT_ALLOC, ERROR,
   "error *** Cannot allocate variable '%s'.\n" },
{ E_OLD_STYLE, ERROR,
   "error *** Old style C declaration. IGNORED '%s'\n" },
{ E_STACK_OUT, ERROR,
   "error *** Out of stack Space. '%s' not allocated\n" },
{ E_INTERNAL_ERROR, ERROR,
   "error *** FATAL Compiler Internal Error in file '%s' line number '%d' : %s \nContact Author with source code\n" },
{ E_LVALUE_REQUIRED, ERROR,
   "error *** 'lvalue' required for '%s' operation .\n" },
{ E_TMPFILE_FAILED, ERROR,
   "error *** Creation of temp file failed\n" },
{ E_FUNCTION_EXPECTED, ERROR,
   "error *** Function expected here '%s'\n" },
{ E_USING_ERROR, ERROR,
   "error *** 'using', 'interrupt' or 'reentrant' must follow a function definiton .'%s'\n" },
{ E_SFR_INIT, ERROR,
   "error *** Absolute address & initial value both cannot be specified for\n a 'sfr','sbit' storage class, initial value ignored '%s'\n" },
{ E_INIT_IGNORED, WARNING,
   "warning *** Variable in the storage class cannot be initialized.'%s'\n" },
{ E_AUTO_ASSUMED, WARNING,
   "warning *** storage class not allowed for automatic variable '%s' in reentrant function\n" },
{ E_AUTO_ABSA, ERROR,
   "error *** absolute address not allowed for automatic var '%s' in reentrant function \n" },
{ E_INIT_WRONG, WARNING,
   "warning *** Initializer different levels of indirections\n" },
{ E_FUNC_REDEF, ERROR,
   "error *** Function name '%s' redefined \n" },
{ E_ID_UNDEF, ERROR,
   "error *** Undefined identifier '%s'\n" },
{ W_STACK_OVERFLOW, WARNING,
   "warning *** stack exceeds 256 bytes for function '%s'\n" },
{ E_NEED_ARRAY_PTR, ERROR,
   "error *** Array or pointer required for '%s' operation \n" },
{ E_IDX_NOT_INT, ERROR,
   "error *** Array index not an integer\n" },
{ E_ARRAY_BOUND, ERROR,
   "error *** Array bound Exceeded, assuming zero\n" },
{ E_STRUCT_UNION, ERROR,
   "error *** Structure/Union expected left of '.%s'\n" },
{ E_NOT_MEMBER, ERROR,
   "error *** '%s' not a structure/union member\n" },
{ E_PTR_REQD, ERROR,
   "error *** Pointer required\n" },
{ E_UNARY_OP, ERROR,
   "error *** 'unary %c': illegal operand\n" },
{ E_CONV_ERR, ERROR,
   "error *** convertion error: integral promotion failed\n" },
{ E_INT_REQD, ERROR,
   "error *** type must be INT for bit field definition\n" },
{ E_BITFLD_SIZE, ERROR,
   "error *** bit field size greater than 16 . assuming 16\n" },
{ E_TRUNCATION, WARNING,
   "warning *** high order truncation might occur\n" },
{ E_CODE_WRITE, ERROR,
   "error *** Attempt to assign value to a constant variable %s\n" },
{ E_LVALUE_CONST, ERROR,
   "error *** Lvalue specifies constant object\n" },
{ E_ILLEGAL_ADDR, ERROR,
   "error *** '&' illegal operand , %s\n" },
{ E_CAST_ILLEGAL, ERROR,
   "error *** illegal cast (cast cannot be aggregate)\n" },
{ E_MULT_INTEGRAL, ERROR,
   "error *** '*' bad operand\n" },
{ E_ARG_ERROR, ERROR,
   "error *** Argument count error, argument ignored\n" },
{ E_ARG_COUNT, ERROR,
   "error *** Function was expecting more arguments\n" },
{ E_FUNC_EXPECTED, ERROR,
   "error *** Function name expected '%s'.ANSI style declaration REQUIRED\n" },
{ E_PLUS_INVALID, ERROR,
   "error *** invalid operand '%s'\n" },
{ E_PTR_PLUS_PTR, ERROR,
   "error *** pointer + pointer invalid\n" },
{ E_SHIFT_OP_INVALID, ERROR,
   "error *** invalid operand for shift operator\n" },
{ E_COMPARE_OP, ERROR,
   "error *** compare operand cannot be struct/union\n" },
{ E_BITWISE_OP, ERROR,
   "error *** operand invalid for bitwise operation\n" },
{ E_ANDOR_OP, ERROR,
   "error *** Invalid operand for '&&' or '||'\n" },
{ E_TYPE_MISMATCH, WARNING,
   "warning *** indirections to different types %s %s \n" },
{ E_AGGR_ASSIGN, ERROR,
   "error *** cannot assign values to aggregates\n" },
{ E_ARRAY_DIRECT, ERROR,
   "error *** bit Arrays can be accessed by literal index only\n" },
{ E_BIT_ARRAY, ERROR,
   "error *** Array or Pointer to bit|sbit|sfr not allowed.'%s'\n" },
{ E_DUPLICATE_TYPEDEF, ERROR,
   "error *** typedef/enum '%s' duplicate.Previous definiton Ignored\n" },
{ E_ARG_TYPE, ERROR,
   "error *** Actual Argument type different from declaration %d\n" },
{ E_RET_VALUE, ERROR,
   "error *** Function return value mismatch\n" },
{ E_FUNC_AGGR, ERROR,
   "error *** Function cannot return aggregate. Func body ignored\n" },
{ E_FUNC_DEF, ERROR,
   "error *** ANSI Style declaration needed\n" },
{ E_DUPLICATE_LABEL, ERROR,
   "error *** Label name redefined '%s'\n" },
{ E_LABEL_UNDEF, ERROR,
   "error *** Label undefined '%s'\n" },
{ E_FUNC_VOID, ERROR,
   "error *** void function returning value\n" },
{ E_VOID_FUNC, ERROR,
   "error *** function '%s' must return value\n" },
{ E_RETURN_MISMATCH, WARNING,
   "warning *** function return value mismatch\n" },
{ E_CASE_CONTEXT, ERROR,
   "error *** 'case/default' found without 'switch'.statement ignored\n" },
{ E_CASE_CONSTANT, ERROR,
   "error *** 'case' expression not constant. statement ignored\n" },
{ E_BREAK_CONTEXT, ERROR,
   "error *** 'break/continue' statement out of context\n" },
{ E_SWITCH_AGGR, ERROR,
   "error *** nonintegral used in switch expression\n" },
{ E_FUNC_BODY, ERROR,
   "error *** function '%s' already has body\n" },
{ E_UNKNOWN_SIZE, ERROR,
   "error *** attempt to allocate variable of unknown size '%s'\n" },
{ E_AUTO_AGGR_INIT, ERROR,
   "error *** aggregate 'auto' variable '%s' cannot be initialized\n" },
{ E_INIT_COUNT, ERROR,
   "error *** too many initializers\n" },
{ E_INIT_STRUCT, ERROR,
   "error *** struct/union/array '%s' :initialization needs curly braces\n" },
{ E_INIT_NON_ADDR, ERROR,
   "error *** non-address initialization expression\n" },
{ E_INT_DEFINED, ERROR,
   "error *** interrupt no '%d' already has a service routine '%s'\n" },
{ E_INT_ARGS, ERROR,
   "error *** interrupt routine cannot have arguments, arguments ingored\n" },
{ E_INCLUDE_MISSING, ERROR,
   "error *** critical compiler #include file missing.		\n" },
{ E_NO_MAIN, ERROR,
   "error *** function 'main' undefined\n" },
{ E_EXTERN_INIT, ERROR,
   "error *** 'extern' variable '%s' cannot be initialised	\n" },
{ E_PRE_PROC_FAILED, ERROR,
   "error *** Pre-Processor %s\n" },
{ E_DUP_FAILED, ERROR,
   "error *** _dup call failed\n" },
{ E_INCOMPAT_CAST, WARNING,
   "warning *** pointer being cast to incompatible type \n" },
{ E_LOOP_ELIMINATE, WARNING,
   "warning *** 'while' loop with 'zero' constant.loop eliminated\n" },
{ W_NO_SIDE_EFFECTS, WARNING,
   "warning *** %s expression has NO side effects.expr eliminated\n" },
{ E_CONST_TOO_LARGE, PEDANTIC,
   "warning *** constant value '%s', out of range.\n" },
{ W_BAD_COMPARE, WARNING,
   "warning *** comparison will either, ALWAYs succeed or ALWAYs fail\n" },
{ E_TERMINATING, ERROR,
   "error *** Compiler Terminating , contact author with source\n" },
{ W_LOCAL_NOINIT, WARNING,
   "warning *** 'auto' variable '%s' may be used before initialization at %s(%d)\n" },
{ W_NO_REFERENCE, WARNING,
   "warning *** in function %s unreferenced %s : '%s'\n" },
{ E_OP_UNKNOWN_SIZE, ERROR,
   "error *** unknown size for operand\n" },
{ W_LONG_UNSUPPORTED, WARNING,
   "warning *** '%s' 'long' not supported , declared as 'int' .\n" },
{ W_LITERAL_GENERIC, WARNING,
   "warning *** LITERAL value being cast to '_generic' pointer\n" },
{ E_SFR_ADDR_RANGE, ERROR,
   "error *** %s '%s' address out of range\n" },
{ E_BITVAR_STORAGE, ERROR,
   "error *** storage class CANNOT be specified for bit variable '%s'\n" },
{ W_EXTERN_MISMATCH, WARNING,
   "warning *** extern definition for '%s' mismatches with declaration.\n" },
{ E_NONRENT_ARGS, WARNING,
   "warning *** Functions called via pointers must be 'reentrant' to take arguments\n" },
{ W_DOUBLE_UNSUPPORTED, WARNING,
   "warning *** type 'double' not supported assuming 'float'\n" },
{ W_IF_NEVER_TRUE, WARNING,
   "warning *** if-statement condition always false.if-statement not generated\n" },
{ W_FUNC_NO_RETURN, WARNING,
   "warning *** no 'return' statement found for function '%s'\n" },
{ W_PRE_PROC_WARNING, WARNING,
   "warning *** Pre-Processor %s\n" },
{ W_STRUCT_AS_ARG, WARNING,
   "warning *** structure '%s' passed as function argument changed to pointer\n" },
{ E_PREV_DEF_CONFLICT, ERROR,
   "error *** conflict with previous definition of '%s' for attribute '%s'\n" },
{ E_CODE_NO_INIT, ERROR,
   "error *** variable '%s' declared in code space must have initialiser\n" },
{ E_OPS_INTEGRAL, ERROR,
   "error *** operands not integral for assignment operation\n" },
{ E_TOO_MANY_PARMS, ERROR,
   "error *** too many parameters \n" },
{ E_TOO_FEW_PARMS, ERROR,
   "error *** too few parameters\n" },
{ E_FUNC_NO_CODE, ERROR,
   "error *** code not generated for '%s' due to previous errors\n" },
{ E_TYPE_MISMATCH_PARM, WARNING,
   "warning *** type mismatch for parameter number %d\n" },
{ E_INVALID_FLOAT_CONST, ERROR,
   "error *** invalid float constant '%s'\n" },
{ E_INVALID_OP, ERROR,
   "error *** invalid operand for '%s' operation\n" },
{ E_SWITCH_NON_INTEGER, ERROR,
   "error *** switch value not an integer\n" },
{ E_CASE_NON_INTEGER, ERROR,
   "error *** case label not an integer\n" },
{ E_FUNC_TOO_LARGE, WARNING,
   "warning *** function '%s' too large for global optimization\n" },
{ W_CONTROL_FLOW, PEDANTIC,
   "warning *** conditional flow changed by optimizer '%s(%d)':so said EVELYN the modified DOG\n" },
{ W_PTR_TYPE_INVALID, WARNING,
   "warning *** invalid type specifier for pointer type specifier ignored\n" },
{ W_IMPLICIT_FUNC, WARNING,
   "warning *** function '%s' implicit declaration\n" },
{ E_CONTINUE, WARNING,
   "warning *** %s" },
{ W_TOOMANY_SPILS, WARNING,
   "info *** %s extended by %d bytes for compiler temp(s) :in function  '%s': %s \n" },
{ W_UNKNOWN_PRAGMA, WARNING,
   "warning *** unknown or unsupported #pragma directive '%s'\n" },
{ W_SHIFT_CHANGED, PEDANTIC,
   "warning *** %s shifting more than size of object changed to zero\n" },
{ W_UNKNOWN_OPTION, WARNING,
   "warning *** unknown compiler option '%s' ignored\n" },
{ W_UNSUPP_OPTION, WARNING,
   "warning *** option '%s' no longer supported  '%s' \n" },
{ W_UNKNOWN_FEXT, WARNING,
   "warning *** don't know what to do with file '%s'. file extension unsupported\n" },
{ W_TOO_MANY_SRC, WARNING,
   "warning *** cannot compile more than one source file . file '%s' ignored\n" },
{ I_CYCLOMATIC, WARNING,
   "info *** function '%s', # edges %d , # nodes %d , cyclomatic complexity %d\n" },
{ E_DIVIDE_BY_ZERO, ERROR,
   "error *** dividing by ZERO\n" },
{ E_FUNC_BIT, ERROR,
   "error *** function cannot return 'bit'\n" },
{ E_CAST_ZERO, ERROR,
   "error *** casting from to type 'void' is illegal\n" },
{ W_CONST_RANGE, WARNING,
   "warning *** constant is out of range %s\n" },
{ W_CODE_UNREACH, PEDANTIC,
   "warning *** unreachable code %s(%d)\n" },
{ W_NONPTR2_GENPTR, WARNING,
   "warning *** non-pointer type cast to _generic pointer\n" },
{ W_POSSBUG, WARNING,
   "warning *** possible code generation error at line %d,\n send source to sandeep.dutta@usa.net\n" },
{ W_PTR_ASSIGN, WARNING,
   "warning *** pointer types incompatible \n" },
{ W_UNKNOWN_MODEL, WARNING,
   "warning *** unknown memory model at %s : %d\n" },
{ E_UNKNOWN_TARGET, ERROR,
   "error   *** cannot generate code for target '%s'\n" },
{ W_INDIR_BANKED, WARNING,
   "warning *** Indirect call to a banked function not implemented.\n" },
{ W_UNSUPPORTED_MODEL, WARNING,
   "warning *** Model '%s' not supported for %s, ignored.\n" },
{ W_BANKED_WITH_NONBANKED, WARNING,
   "warning *** Both banked and nonbanked attributes used.  nonbanked wins.\n" },
{ W_BANKED_WITH_STATIC, WARNING,
   "warning *** Both banked and static used.  static wins.\n" },
{ W_INT_TO_GEN_PTR_CAST, WARNING,
   "warning *** converting integer type to generic pointer: assuming XDATA\n" },
{ W_ESC_SEQ_OOR_FOR_CHAR, WARNING,
   "warning *** escape sequence out of range for char.\n" },
{ E_INVALID_HEX, ERROR,
   "error   *** \\x used with no following hex digits.\n" },
};
/*
-------------------------------------------------------------------------------
SetErrorOut - Set the error output file

-------------------------------------------------------------------------------
*/

FILE *SetErrorOut(FILE *NewErrorOut)

{
    _SDCCERRG.out = NewErrorOut ;

return NewErrorOut ;
}

void
setErrorLogLevel (ERROR_LOG_LEVEL level)
{
    _SDCCERRG.logLevel = level;
}

/*
-------------------------------------------------------------------------------
vwerror - Output a standard eror message with variable number of arguements

-------------------------------------------------------------------------------
*/

void vwerror (int errNum, va_list marker)
{
    if (_SDCCERRG.out == NULL) {
        _SDCCERRG.out = DEFAULT_ERROR_OUT;
    }

    if (ErrTab[errNum].errIndex != errNum)
    {
        fprintf(_SDCCERRG.out, 
        	"*** Internal error: error table entry for %d inconsistent.", errNum);
    }

    if (ErrTab[errNum].errType >= _SDCCERRG.logLevel) {
        if ( ErrTab[errNum].errType == ERROR )
            fatalError++ ;
  
        if ( filename && lineno ) {
            fprintf(_SDCCERRG.out, "%s(%d):",filename,lineno);
        } else if (lineno) {
            fprintf(_SDCCERRG.out, "at %d:", lineno);
        }
    
        vfprintf(_SDCCERRG.out, ErrTab[errNum].errText,marker);
    }
    else {
        // Below the logging level, drop.
    }
}
/*
-------------------------------------------------------------------------------
werror - Output a standard eror message with variable number of arguements

-------------------------------------------------------------------------------
*/

void werror (int errNum, ... )
{
    va_list	marker;
    va_start(marker,errNum);
    vwerror(errNum, marker);
    va_end( marker );
}

