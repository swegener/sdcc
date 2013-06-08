#include "common.h"
#include "SDCCicode.h"
#include "SDCCglobl.h"
#include "SDCCgen.h"

#define D(_s) { printf _s; fflush(stdout); }
/*#define D(_s)*/

#define EQUALS(l, i) (!strcmp((l), (i)))
#define ISINST(l, i) (!strncmp((l), (i), sizeof(i) - 1))

static int
readint(const char *str)
{
  int ret;
  if(str[0] == '#' || str[0] == '(')
    str++;
  if(sscanf(str, "0x%x", &ret))
    return(ret);
  if(!sscanf(str, "%d", &ret))
    {
      wassertl (0, "readint() got non-integer argument.");
      ret = -1;
    }
  return(ret);
}

static int
isreg(const char *what)
{
  if(what[0] == '(')
    what++;
  if(what[0] == 'a' || what[0] == 'x' || what[0] == 'y')
    return(TRUE);
  if(!strcmp(what, "sp"))
    return(TRUE);
  return(FALSE);
}

static char *
nextToken(char *p)
{
  /* strtok replacement */
  static char *str, *ret;
  if(p)
    str = p;
  ret = str;
  // Strip separators
  while(*str == ',' || isspace(*str))
    str++;
  if(*str == '(')
  {
    // Take an expression in brackets
    while(*str && *str != ')')
      str++;
    str++;
  } else {
    // Take until EOL or separator
    while(*str && *str != ',' && !isspace(*str))
      str++;
  }
  *str = '\0';
  str++;
  return(ret);
}

static int
isRelativeAddr(char *what, const char *mode)
{
  char buf[4];
  strcpy(buf, mode);
  strcat(buf, ")");
  return(what[0] == '(' && strstr(what, buf));
}

static int
isImmediate(char *what)
{
  return(what[0] == '#');
}

static int
isShortoff(char *what, const char *mode)
{
  return(isRelativeAddr(what, mode) && readint(what) <= 0xFF);
}

static int
isLongoff(char *what, const char *mode)
{
  return(isRelativeAddr(what, mode) && readint(what) > 0xFF);
}

static int
isSpIndexed(char *what)
{
  return isRelativeAddr(what, "sp");
}


int
stm8instructionSize(lineNode *pl)
{
  char buf[32];
  strncpy(buf, pl->line, 32);
  buf[31] = 0;
  char *operand = nextToken(buf);
  char *op1start = nextToken(NULL);
  char *op2start = nextToken(NULL);
  //fprintf(stderr, "op1start=%s op2start=%s\n", op1start, op2start);
  while(op2start && isspace(*op2start)) op2start++;
  int i = 0;
  //printf("operand=%s op1start=%s op2start=%s\n", operand, op1start, op2start);

  /* arity=1 */
  if(EQUALS(operand, "clr")
                || EQUALS(operand, "dec")
                || EQUALS(operand, "inc")
                || EQUALS(operand, "push")
                || EQUALS(operand, "swap")
                || EQUALS(operand, "jp")
                || EQUALS(operand, "cpl")
                || EQUALS(operand, "tnz"))
  {
    if(!op1start)
      return(3);
    if(op1start[0] == 'a' || op1start[1] == 'x')
      return(1);
    if(op1start[0] == '(')
      op1start++;
    if(strstr(op1start, ",y)"))
      i++; // costs extra byte for operating with y
    if(readint(op1start) <= 0xFF)
      return(2+i);
    /* op1 > 0xFF */
    if(EQUALS(operand, "jp") && !strchr(op1start, 'y'))
      return(3);
    return(4);
  }
  if(EQUALS(operand, "exg"))
  {
    if(isreg(op2start))
      return(1);
    else
      return(3);
  }
  if(EQUALS(operand, "addw") || EQUALS(operand, "subw"))
  {
    if(isImmediate(op2start) && op1start[0] == 'y')
      return(4);
    if(isImmediate(op2start))
      return(3);
    if(isSpIndexed(op2start))
      return(3);
    if(isLongoff(op2start, "x"))
      return(4);
  }
  if(EQUALS(operand, "cplw"))
  {
    if(op1start[0] == 'y')
      return(2);
    else
      return(1);
  }
  if(EQUALS(operand, "ldf"))
  {
    if(isRelativeAddr(op1start, "y") || isRelativeAddr(op2start, "y"))
      return(5);
    else
      return(4);
  }
  /* Operations that costs 2 or 3 bytes for immediate */
  if(ISINST(operand, "ld")
                || EQUALS(operand, "cp")
                || EQUALS(operand, "cpw")
                || ISINST(operand, "adc")
                || EQUALS(operand, "add")
                || ISINST(operand, "and")
                || ISINST(operand, "bcp")
                || ISINST(operand, "call")
                || ISINST(operand, "callr")
                || ISINST(operand, "jr")
                || ISINST(operand, "or")
                || ISINST(operand, "sbc")
                || EQUALS(operand, "sub")
                || ISINST(operand, "xor"))
  {
    if(!op1start || !op2start)
      return(4);
    char suffix = operand[strlen(operand)-1];
    if(suffix == 'w' && isImmediate(op2start))
      i++; // costs extra byte
    if(isSpIndexed(op1start) || isSpIndexed(op2start))
      return(2);
    if(!strcmp(op1start, "(x)") || !strcmp(op2start, "(x)"))
      return(1);
    if(!strcmp(op1start, "(y)") || !strcmp(op2start, "(y)"))
      return(2);
    if(isShortoff(op1start, "x") || isShortoff(op2start, "x"))
      return(2);
    if(isShortoff(op1start, "y") || isShortoff(op2start, "y"))
      return(3);
    if(isLongoff(op1start, "x") || isLongoff(op2start, "x"))
      return(3);
    if(isRelativeAddr(op1start, "y"))
      return(4);
    if(strchr(op1start, 'y') || (strchr(op2start, 'y') && !EQUALS(operand, "ldw")))
      i++; // costs extra byte for operating with y
    if(isreg(op1start) && isreg(op2start))
      return(1+i);
    if(op2start[0] == '#')
      return(2+i); // ld reg, #immd
    if(readint(op2start) <= 0xFF)
      return(2+i);
    return(3+i);
  }

  /* mov costs 3, 4 or 5 bytes depending on it's addressing mode */
  if(EQUALS(operand, "mov")) {
    if(op2start && op2start[0] == '#')
      return(4);
    if(op2start && readint(op2start) <= 0xFF)
      return(3);
    if(op2start && readint(op2start) > 0xFF)
      return(5);
  }

  /* Operations that always costs 1 byte */
  if(EQUALS(operand, "ccf")
                || EQUALS(operand, "divw")
                || EQUALS(operand, "exgw")
                || EQUALS(operand, "iret")
                || EQUALS(operand, "nop")
                || EQUALS(operand, "rcf")
                || EQUALS(operand, "ret")
                || EQUALS(operand, "retf")
                || EQUALS(operand, "rvf")
                || EQUALS(operand, "scf"))
  {
    return(1);
  }
  /* Operations that costs 2 or 1 bytes depending on 
     is the Y or X register used */
  if(EQUALS(operand, "clrw")
                || EQUALS(operand, "decw")
                || EQUALS(operand, "div")
                || EQUALS(operand, "incw")
                || EQUALS(operand, "mul")
                || EQUALS(operand, "negw")
                || EQUALS(operand, "popw")
                || EQUALS(operand, "pushw")
                || EQUALS(operand, "rlcw")
                || EQUALS(operand, "rlwa")
                || EQUALS(operand, "rrcw")
                || EQUALS(operand, "rrwa")
                || EQUALS(operand, "sllw")
                || EQUALS(operand, "slaw")
                || EQUALS(operand, "sraw")
                || EQUALS(operand, "srlw")
                || EQUALS(operand, "swapw")
                || EQUALS(operand, "tnzw"))
  {
    if((op1start && !strcmp(op1start, "y")) || (op2start && !strcmp(op2start, "y")))
      return(2);
    else
      return(1);
  }
  return(4);
}
