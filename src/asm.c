/** @file asm.c
    Provides output functions that modify the output string
    based on the input tokens and the assembler token mapping
    specification loaded.

    Note that the functions below only handle digit format modifiers.
    eg %02X is ok, but %lu and %.4u will fail.
*/
#include "common.h"
#include "asm.h"

/* A 'token' is like !blah or %24f and is under the programmers
   control. */
#define MAX_TOKEN_LEN		64

static hTab *_h;

char *
FileBaseName (char *fileFullName)
{
  char *p = fileFullName;

  if (!fileFullName) {
    return "unknown";
  }

  while (*fileFullName)
    {
      if ((*fileFullName == '/') || (*fileFullName == '\\') || (*fileFullName == ':'))
	{
	  p = fileFullName;
	  p++;
	}
      fileFullName++;
    }
  return p;
}

static const char *
_findMapping (const char *szKey)
{
  return shash_find (_h, szKey);
}

// Append a string onto another, and update the pointer to the end of
// the new string.
static char *
_appendAt (char *at, char *onto, const char *sz)
{
  wassert (at && onto && sz);
  strcpy (at, sz);
  return at + strlen (sz);
}

void 
tvsprintf (char *buffer, const char *format, va_list ap)
{
  // Under Linux PPC va_list is a structure instead of a primitive type,
  // and doesnt like being passed around.  This version turns everything
  // into one function.

  // Supports:
  //  !tokens
  //  %[CIFN] - special formats with no argument (ie list isnt touched)
  //  All of the system formats

  // This is acheived by expanding the tokens and zero arg formats into
  // one big format string, which is passed to the native printf.
  static int count;
  char noTokens[INITIAL_INLINEASM];
  char newFormat[INITIAL_INLINEASM];
  char *pInto = noTokens;
  char *p;
  char token[MAX_TOKEN_LEN];
  const char *sz = format;

  // NULL terminate it to let strlen work.
  *pInto = '\0';

  /* First pass: expand all of the macros */
  while (*sz)
    {
      if (*sz == '!')
	{
	  /* Start of a token.  Search until the first
	     [non alpha, *] and call it a token. */
	  const char *t;
	  p = token;
	  sz++;
	  while (isalpha (*sz) || *sz == '*')
	    {
	      *p++ = *sz++;
	    }
	  *p = '\0';
	  /* Now find the token in the token list */
	  if ((t = _findMapping (token)))
	    {
	      pInto = _appendAt (pInto, noTokens, t);
	    }
	  else
	    {
	      fprintf (stderr, "Cant find token \"%s\"\n", token);
	      wassert (0);
	    }
	}
      else
        {
          *pInto++ = *sz++;
        }
    }

  *pInto = '\0';

  /* Second pass: Expand any macros that we own */
  sz = noTokens;
  pInto = newFormat;

  while (*sz)
    {
      if (*sz == '%')
	{
	  // See if its one that we handle.
	  sz++;
	  switch (*sz)
	    {
	    case 'C':
	      // Code segment name.
	      pInto = _appendAt (pInto, newFormat, CODE_NAME);
              sz++;
	      break;
	    case 'F':
	      // Source file name.
	      pInto = _appendAt (pInto, newFormat, srcFileName);
              sz++;
	      break;
            case 'N':
              // Current function name.
              pInto = _appendAt (pInto, newFormat, currFunc->rname);
              sz++;
              break;
	    case 'I':
	      {
		// Unique ID.
		char id[20];
		sprintf (id, "%u", ++count);
		pInto = _appendAt (pInto, newFormat, id);
                sz++;
		break;
	      }
	    default:
	      // Not one of ours.  Copy until the end.
	      *pInto++ = '%';
	      while (!isalpha (*sz))
		{
		  *pInto++ = *sz++;
		}
	      *pInto++ = *sz++;
	    }
	}
      else
	{
	  *pInto++ = *sz++;
	}
    }

  *pInto = '\0';

  // Now do the actual printing
  vsprintf (buffer, newFormat, ap);
}

void 
tfprintf (FILE * fp, const char *szFormat,...)
{
  va_list ap;
  char buffer[INITIAL_INLINEASM];

  va_start (ap, szFormat);
  tvsprintf (buffer, szFormat, ap);
  fputs (buffer, fp);
}

void 
tsprintf (char *buffer, const char *szFormat,...)
{
  va_list ap;
  va_start (ap, szFormat);
  tvsprintf (buffer, szFormat, ap);
}

void 
asm_addTree (const ASM_MAPPINGS * pMappings)
{
  const ASM_MAPPING *pMap;

  /* Traverse down first */
  if (pMappings->pParent)
    asm_addTree (pMappings->pParent);
  pMap = pMappings->pMappings;
  while (pMap->szKey && pMap->szValue) {
      shash_add (&_h, pMap->szKey, pMap->szValue);
      pMap++;
  }
}

/*-----------------------------------------------------------------*/
/* printCLine - try to find the c-code for this lineno             */
/*-----------------------------------------------------------------*/
static FILE *inFile=NULL;
static char inLineString[1024];
static int inLineNo=0;
int rewinds=0;

char *printCLine (char *srcFile, int lineno) {
  char *ilsP=inLineString;
  if (!inFile) {
    inFile=fopen(srcFile, "r");
    if (!inFile) {
      perror ("printCLine");
      exit (1);
    }
  }
  if (lineno<inLineNo) {
    fseek (inFile, 0, SEEK_SET);
    inLineNo=0;
    rewinds++;
  }
  while (fgets (inLineString, 1024, inFile)) {
    inLineNo++;
    if (inLineNo==lineno) {
      // remove the trailing NL
      inLineString[strlen(inLineString)-1]='\0';
      break;
    }
  }
  while (isspace ((int)*ilsP))
    ilsP++;

  return ilsP;
}

static const ASM_MAPPING _asxxxx_mapping[] =
{
  {"labeldef", "%s::"},
  {"slabeldef", "%s:"},
  {"tlabeldef", "%05d$:"},
  {"tlabel", "%05d$"},
  {"immed", "#"},
  {"zero", "#0x00"},
  {"one", "#0x01"},
  {"area", ".area %s"},
  {"areacode", ".area %s"},
  {"areadata", ".area %s"},
  {"areahome", ".area %s"},
  {"ascii", ".ascii \"%s\""},
  {"ds", ".ds %d"},
  {"db", ".db"},
  {"dbs", ".db %s"},
  {"dw", ".dw"},
  {"dws", ".dw %s"},
  {"constbyte", "0x%02X"},
  {"constword", "0x%04X"},
  {"immedword", "#0x%04X"},
  {"immedbyte", "#0x%02X"},
  {"hashedstr", "#%s"},
  {"lsbimmeds", "#<%s"},
  {"msbimmeds", "#>%s"},
  {"module", ".module %s"},
  {"global", ".globl %s"},
  {"fileprelude", ""},
  {"functionheader",
   "; ---------------------------------\n"
   "; Function %s\n"
   "; ---------------------------------"
  },
  {"functionlabeldef", "%s:"},
  {"bankimmeds", "0	; PENDING: bank support"},
  {"los","(%s & 0xFF)"},
  {"his","(%s >> 8)"},
  {"hihis","(%s >> 16)"},
  {"hihihis","(%s >> 24)"},
  {"lod","(%d & 0xFF)"},
  {"hid","(%d >> 8)"},
  {"hihid","(%d >> 16)"},
  {"hihihid","(%d >> 24)"},
  {"lol","(%05d$ & 0xFF)"},
  {"hil","(%05d$ >> 8)"},
  {"hihil","(%05d$ >> 16)"},
  {"hihihil","(%05d$ >> 24)"},
  {"equ","="},
  {NULL, NULL}
};

static const ASM_MAPPING _gas_mapping[] =
{
  {"labeldef", "%s::"},
  {"slabeldef", "%s:"},
  {"tlabeldef", "%05d$:"},
  {"tlabel", "%05d$"},
  {"immed", "#"},
  {"zero", "#0x00"},
  {"one", "#0x01"},
  {"area", ".section %s"},
  {"areacode", ".section %s"},
  {"areadata", ".section %s"},
  {"areahome", ".section %s"},
  {"ascii", ".ascii \"%s\""},
  {"ds", ".ds %d"},
  {"db", ".db"},
  {"dbs", ".db %s"},
  {"dw", ".dw"},
  {"dws", ".dw %s"},
  {"constbyte", "0x%02X"},
  {"constword", "0x%04X"},
  {"immedword", "#0x%04X"},
  {"immedbyte", "#0x%02X"},
  {"hashedstr", "#%s"},
  {"lsbimmeds", "#<%s"},
  {"msbimmeds", "#>%s"},
  {"module", ".file \"%s.c\""},
  {"global", ".globl %s"},
  {"extern", ".globl %s"},
  {"fileprelude", ""},
  {"functionheader",
   "; ---------------------------------\n"
   "; Function %s\n"
   "; ---------------------------------"
  },
  {"functionlabeldef", "%s:"},
  {"bankimmeds", "0	; PENDING: bank support"},  
  {NULL, NULL}
};

static const ASM_MAPPING _a390_mapping[] =
{
  {"labeldef", "%s:"},
  {"slabeldef", "%s:"},
  {"tlabeldef", "L%05d:"},
  {"tlabel", "L%05d"},
  {"immed", "#"},
  {"zero", "#0"},
  {"one", "#1"},
  {"area", "; SECTION NOT SUPPORTED"},
  {"areacode", "; SECTION NOT SUPPORTED"},
  {"areadata", "; SECTION NOT SUPPORTED"},
  {"areahome", "; SECTION NOT SUPPORTED"},
  {"ascii", "db \"%s\""},
  {"ds", "; STORAGE NOT SUPPORTED"},
  {"db", "db"},
  {"dbs", "db \"%s\""},
  {"dw", "dw"},
  {"dws", "dw %s"},
  {"constbyte", "0%02xh"},
  {"constword", "0%04xh"},
  {"immedword", "#0%04Xh"},
  {"immedbyte", "#0%02Xh"},
  {"hashedstr", "#%s"},
  {"lsbimmeds", "#<%s"},
  {"msbimmeds", "#>%s"},
  {"module", "; .file \"%s.c\""},
  {"global", "; .globl %s"},
  {"fileprelude", ""},
  {"functionheader",
   "; ---------------------------------\n"
   "; Function %s\n"
   "; ---------------------------------"
  },
  {"functionlabeldef", "%s:"},
  {"bankimmeds", "0	; PENDING: bank support"},  
  {"los","(%s & 0FFh)"},
  {"his","((%s / 256) & 0FFh)"},
  {"hihis","((%s / 65536) & 0FFh)"},
  {"hihihis","((%s / 16777216) & 0FFh)"},
  {"lod","(%d & 0FFh)"},
  {"hid","((%d / 256) & 0FFh)"},
  {"hihid","((%d / 65536) & 0FFh)"},
  {"hihihid","((%d / 16777216) & 0FFh)"},
  {"lol","(L%05d & 0FFh)"},
  {"hil","((L%05d / 256) & 0FFh)"},
  {"hihil","((L%05d / 65536) & 0FFh)"},
  {"hihihil","((L%09d / 16777216) & 0FFh)"},
  {"equ"," equ"},
  {NULL, NULL}
};

static const ASM_MAPPING _xa_asm_mapping[] =
{
  {"labeldef", "%s:"},
  {"slabeldef", "%s:"},
  {"tlabeldef", "L%05d:"},
  {"tlabel", "L%05d"},
  {"immed", "#"},
  {"zero", "#0"},
  {"one", "#1"},
  {"area", ".area %s"},
  {"areacode", ".area %s"},
  {"areadata", ".area %s"},
  {"areahome", ".area %s"},
  {"ascii", ".db \"%s\""},
  {"ds", ".ds %d"},
  {"db", ".db"},
  {"dbs", ".db \"%s\""},
  {"dw", ".dw"},
  {"dws", ".dw %s"},
  {"constbyte", "0%02xh"},
  {"constword", "0%04xh"},
  {"immedword", "#0%04Xh"},
  {"immedbyte", "#0%02Xh"},
  {"hashedstr", "#%s"},
  {"lsbimmeds", "#<%s"},
  {"msbimmeds", "#>%s"},
  {"module", ".module %s"},
  {"global", ".globl %s"},
  {"fileprelude", ""},
  {"functionheader",
   "; ---------------------------------\n"
   "; Function %s\n"
   "; ---------------------------------"
  },
  {"functionlabeldef", "%s:"},
  {"bankimmeds", "0	; PENDING: bank support"},  
  {"los","(%s & 0FFh)"},
  {"his","((%s / 256) & 0FFh)"},
  {"hihis","((%s / 65536) & 0FFh)"},
  {"hihihis","((%s / 16777216) & 0FFh)"},
  {"lod","(%d & 0FFh)"},
  {"hid","((%d / 256) & 0FFh)"},
  {"hihid","((%d / 65536) & 0FFh)"},
  {"hihihid","((%d / 16777216) & 0FFh)"},
  {"lol","(L%05d & 0FFh)"},
  {"hil","((L%05d / 256) & 0FFh)"},
  {"hihil","((L%05d / 65536) & 0FFh)"},
  {"hihihil","((L%09d / 16777216) & 0FFh)"},
  {"equ"," equ"},
  {NULL, NULL}
};

const ASM_MAPPINGS asm_asxxxx_mapping =
{
  NULL,
  _asxxxx_mapping
};

const ASM_MAPPINGS asm_gas_mapping =
{
  NULL,
  _gas_mapping
};

const ASM_MAPPINGS asm_a390_mapping =
{
  NULL,
  _a390_mapping
};

const ASM_MAPPINGS asm_xa_asm_mapping =
{
  NULL,
  _xa_asm_mapping
};
