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

static const char *_findMapping(const char *szKey)
{
    return shash_find(_h, szKey);
}

static va_list _iprintf(char *pInto, const char *sz, va_list ap)
{
    char format[MAX_TOKEN_LEN];
    char *pStart = pInto;
    static int count;

    while (*sz) {
	if (*sz == '%') {
	    switch (*++sz) {
		/* See if it's a special emitter */
	    case 'r':
		wassert(0);
		break;
	    /* Name of the code segment */
	    case 'C':
		strcpy(pInto, CODE_NAME);
		pInto = pStart + strlen(pStart);
		sz++;
		break;
	    case 'F':
		strcpy(pInto, srcFileName);
		pInto = pStart + strlen(pStart);	
		sz++;
		break;
	    case 'I':
		sprintf(pInto, "%u", ++count);
		pInto = pStart + strlen(pStart);
		sz++;
		break;
	    default:
		{
		    /* Scan out the arg and pass it on to sprintf */
		    char *p = format;
		    *p++ = '%';
		    while (isdigit(*sz))
			*p++ = *sz++;
		    *p++ = *sz++;
		    *p = '\0';
		    vsprintf(pInto, format, ap);
		    /* PENDING: Assume that the arg length was an int */
		    va_arg(ap, int);
		}
	    }
	    pInto = pStart + strlen(pStart);
	}
	else {
	    *pInto++ = *sz++;
	}
    }
    *pInto = '\0';

    return ap;
}

void tvsprintf(char *buffer, const char *sz, va_list ap)
{
    char *pInto = buffer;
    char *p;
    char token[MAX_TOKEN_LEN];

    buffer[0] = '\0';
    
    while (*sz) {
	if (*sz == '!') {
	    /* Start of a token.  Search until the first
	       [non alplha, *] and call it a token. */
	    const char *t;
	    p = token;
	    sz++;
	    while (isalpha(*sz) || *sz == '*') {
		*p++ = *sz++;
	    }
	    *p = '\0';
	    /* Now find the token in the token list */
	    if ((t = _findMapping(token))) {
		ap = _iprintf(pInto, t, ap);
		pInto = buffer + strlen(buffer);
	    }
	    else {
		fprintf(stderr, "Cant find token \"%s\"\n", token);
		wassert(0);
	    }
	}
	else if (*sz == '%') {
	    p = token;
	    *p++ = *sz++;
	    while (!isalpha(*sz)) {
		*p++ = *sz++;
	    }
	    *p++ = *sz++;
	    *p = '\0';
	    vsprintf(pInto, token, ap);
	    pInto = buffer + strlen(buffer);
	    va_arg(ap, int);
	}
	else {
	    *pInto++ = *sz++;
	}
    }
    *pInto = '\0';
}

void tfprintf(FILE *fp, const char *szFormat, ...)
{
    va_list ap;
    char buffer[MAX_INLINEASM];

    va_start(ap, szFormat);
    tvsprintf(buffer, szFormat, ap);
    fputs(buffer, fp);
}

void tsprintf(char *buffer, const char *szFormat, ...)
{
    va_list ap;
    va_start(ap, szFormat);
    tvsprintf(buffer, szFormat, ap);
}

void asm_addTree(const ASM_MAPPINGS *pMappings)
{
    const ASM_MAPPING *pMap;
    /* Traverse down first */
    if (pMappings->pParent)
	asm_addTree(pMappings->pParent);
    pMap = pMappings->pMappings;
    while (pMap->szKey && pMap->szValue) {
	shash_add(&_h, pMap->szKey, pMap->szValue);
	pMap++;
    }
}

static const ASM_MAPPING _asxxxx_mapping[] = {
    { "labeldef", "%s::" },
    { "slabeldef", "%s:" },
    { "tlabeldef", "%05d$:" },
    { "tlabel", "%05d$" },
    { "immed", "#" },
    { "zero", "#0x00" },
    { "one", "#0x01" },
    { "area", ".area %s" },
    { "areacode", ".area %s" },
    { "areadata", ".area %s" },
    { "areahome", ".area %s" },
    { "ascii", ".ascii \"%s\"" },
    { "ds", ".ds %d" },
    { "db", ".db" },
    { "dbs", ".db %s" },
    { "dw", ".dw" },
    { "dws", ".dw %s" },
    { "constbyte", "0x%02X" },
    { "constword", "0x%04X" },
    { "immedword", "#0x%04X" },
    { "immedbyte", "#0x%02X" },
    { "hashedstr", "#%s" },
    { "lsbimmeds", "#<%s" },
    { "msbimmeds", "#>%s" },
    { "module", ".module %s" },
    { "global", ".globl %s" },
    { "fileprelude", "" },
    { "functionheader", 
      "; ---------------------------------\n"
      "; Function %s\n"
      "; ---------------------------------"
    },
    { "functionlabeldef", "%s:" },
    { "bankimmeds", "0	; PENDING: bank support" },
    { NULL, NULL }
};

const ASM_MAPPINGS asm_asxxxx_mapping = {
    NULL,
    _asxxxx_mapping
};

