/*-------------------------------------------------------------------------
  SDCCmain.c - Macro support code.

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

#include "common.h"

enum 
  {
    MAX_STRING_LENGTH	  = 2048,
    MAX_MACRO_NAME_LENGTH = 128
  };

void
_evalMacros(char *apinto, hTab *pvals, const char *pfrom, size_t alen)
{
  bool  fdidsomething = FALSE;
  char  *pinto = apinto;
  size_t plen = alen;

  assert(pinto);
  assert(pvals);
  assert(pfrom);

  while (plen && *pfrom)
    {
      if (*pfrom == '{')
        {
          const char *pend = ++pfrom;
          char name[MAX_MACRO_NAME_LENGTH];
          const char *pval;

          while (*pend && *pend != '}')
            {
              pend++;
            }
          if (*pend != '}')
            {
              wassertl(0, "Unterminated macro expansion");
            }
          /* Pull out the macro name */
	  if (pend - pfrom >= MAX_MACRO_NAME_LENGTH)
	  {
	      wassertl(0, "macro name too long");
	  }
	    
          strncpy(name, pfrom, pend-pfrom);
          name[pend-pfrom] = '\0';

          /* Look up the value in the hash table */
          pval = shash_find (pvals, name);
          
          if (pval == NULL)
            {
	      fprintf (stderr, "Cant find macro \"%s\"\n", name);
	      wassertl (0, "Invalid macro name");
            }
	    else
	    {
		/* Replace */
		strncpy(pinto, pval, plen);
		pinto += strlen(pval);
		plen -= plen > strlen(pval) ? strlen(pval) : plen;
		fdidsomething = TRUE;
	    }

          pfrom = pend+1;
        }
      else
        {
          /* Pass through */
          *pinto++ = *pfrom++;
	  plen--;
        }
    }

  if (!plen)
  {
      wassertl(0, "macro expansion too long");
  }
    
  *pinto = '\0';

  /* If we did something then recursivly expand any expanded macros */
  if (fdidsomething)
    {
      char ainto[MAX_STRING_LENGTH];
      _evalMacros(ainto, pvals, apinto, MAX_STRING_LENGTH);
      strncpyz(apinto, ainto, alen);
    }
}

char *
mvsprintf(hTab *pvals, const char *pformat, va_list ap)
{
  char ainto[MAX_STRING_LENGTH];
  char atmp[MAX_STRING_LENGTH];

  /* Recursivly evaluate all the macros in the string */
  _evalMacros(ainto, pvals, pformat, MAX_STRING_LENGTH);
  /* Evaluate all the arguments */
#if defined(HAVE_VSNPRINTF)
    if (vsnprintf(atmp, MAX_STRING_LENGTH, ainto, ap) >= MAX_STRING_LENGTH)
    {
	fprintf(stderr, "Internal error: mvsprintf output truncated.\n");
    }
#else    
    {	
	int wlen; 
	
	wlen = vsprintf(atmp, ainto, ap);
	
	if (wlen < 0 || wlen >= MAX_STRING_LENGTH)
	{
	    wassertl(0, "mvsprintf overflowed.");
	}
    }
#endif    
    
  /* Recursivly evaluate any macros that were used as arguments */
  _evalMacros(ainto, pvals, atmp, MAX_STRING_LENGTH);

  /* Return a copy of the evaluated string. */
  return Safe_strdup(ainto);
}

char *msprintf(hTab *pvals, const char *pformat, ...)
{
  va_list ap;
  char *pret;

  va_start(ap, pformat);

  pret = mvsprintf(pvals, pformat, ap);

  va_end(ap);

  return pret;
}

void
mfprintf(FILE *fp, hTab *pvals, const char *pformat, ...)
{
  va_list ap;
  char *p;

  va_start(ap, pformat);

  p = mvsprintf(pvals, pformat, ap);

  va_end(ap);

  fputs(p, fp);
  Safe_free(p);
}
