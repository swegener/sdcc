/*-------------------------------------------------------------------------
  SDCCutil.c - Small utility functions.

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

#include <math.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <sys/stat.h>
#include "dbuf.h"
#include "SDCCglobl.h"
#include "SDCCmacro.h"
#include "SDCCutil.h"
#include "newalloc.h"
#ifndef _WIN32
#include "findme.h"
#endif

#include "version.h"

/** Given an array of name, value string pairs creates a new hash
    containing all of the pairs.
*/
hTab *
populateStringHash(const char **pin)
{
  hTab *pret = NULL;

  while (*pin)
    {
      shash_add (&pret, pin[0], pin[1]);
      pin += 2;
    }

  return pret;
}

/** Prints elements of the set to the file, each element on new line
*/
void
fputStrSet(FILE *fp, set *list)
{
  const char *s;

  for (s = setFirstItem(list); s != NULL; s = setNextItem(list)) {
    fputs(s, fp);
    fputc('\n', fp);
  }
}

/** Prepend / append given strings to each item of string set. The result is in a
    new string set.
*/
set *
appendStrSet(set *list, const char *pre, const char *post)
{
  set *new_list = NULL;
  const char *item;
  struct dbuf_s dbuf;

  for (item = setFirstItem(list); item != NULL; item = setNextItem(list)) {
    dbuf_init(&dbuf, PATH_MAX);
    if (pre != NULL)
      dbuf_append(&dbuf, pre, strlen(pre));
    dbuf_append(&dbuf, item, strlen(item));
    if (post != NULL)
      dbuf_append(&dbuf, post, strlen(post));
    addSet(&new_list, (void *)dbuf_c_str(&dbuf));
    dbuf_detach(&dbuf);
  }

  return new_list;
}

/** Given a set returns a string containing all of the strings seperated
    by spaces. The returned string is on the heap.
*/
const char *
joinStrSet(set *list)
{
  const char *s;
  struct dbuf_s dbuf;

  dbuf_init(&dbuf, PATH_MAX);

  for (s = setFirstItem(list); s != NULL; s = setNextItem(list))
    {
      dbuf_append(&dbuf, s, strlen(s));
      dbuf_append(&dbuf, " ", 1);
    }

  s = dbuf_c_str(&dbuf);
  dbuf_detach(&dbuf);
  return s;
}

/** Given a file with path information in the binary files directory,
    returns the directory component. Used for discovery of bin
    directory of SDCC installation. Returns NULL if the path is
    impossible.
*/
#ifdef _WIN32
char *
getBinPath(const char *prel)
{
  char *p;
  size_t len;
  static char path[PATH_MAX];
    
  /* try DOS and *nix dir separator on WIN32 */
  if (NULL != (p = strrchr(prel, DIR_SEPARATOR_CHAR)) ||
    NULL != (p = strrchr(prel, UNIX_DIR_SEPARATOR_CHAR))) {
    len = min((sizeof path) - 1, p - prel);
    strncpy(path, prel, len);
    path[len] = '\0';
    return path;
  }
  /* not enough info in prel; do it with module name */
  else if (0 != GetModuleFileName(NULL, path, sizeof path) &&
    NULL != (p = strrchr(path, DIR_SEPARATOR_CHAR))) {
    *p = '\0';
    return path;
  }
  else
    return NULL;
}
#else
char *
getBinPath(const char *prel)
{
  static char path[PATH_MAX];
  const char *ret_path;

  if (NULL != (ret_path = findProgramPath(prel))) {
    char *p;
    size_t len;

    if (NULL != (p = strrchr(ret_path, DIR_SEPARATOR_CHAR)) &&
      PATH_MAX > (len = p - ret_path)) {
      memcpy(path, ret_path, len);
      path[len] = '\0';
      free((void *)ret_path);

      return path;
    }
    else {
      free((void *)ret_path);

      return NULL;
    }
  }
  else
    return NULL;
}
#endif

/** Returns true if the given path exists.
 */
bool
pathExists (const char *ppath)
{
  struct stat s;

  return stat (ppath, &s) == 0;
}

static hTab *_mainValues;

void
setMainValue (const char *pname, const char *pvalue)
{
  assert(pname);

  shash_add (&_mainValues, pname, pvalue);
}

void
buildCmdLine2 (char *pbuffer, size_t len, const char *pcmd, ...)
{
  va_list ap;
  char *poutcmd;

  assert(pbuffer && pcmd);
  assert(_mainValues);

  va_start(ap, pcmd);

  poutcmd = mvsprintf(_mainValues, pcmd, ap);

  va_end(ap);

  strncpyz(pbuffer, poutcmd, len);
  Safe_free(poutcmd);
}

void
populateMainValues (const char **ppin)
{
  _mainValues = populateStringHash(ppin);
}

/** Returns true if sz starts with the string given in key.
 */
bool
startsWith (const char *sz, const char *key)
{
  return !strncmp (sz, key, strlen (key));
}

/** Removes any newline characters from the string.  Not strictly the
    same as perl's chomp.
*/
void
chomp (char *sz)
{
  char *nl;
  while ((nl = strrchr (sz, '\n')))
    *nl = '\0';
}

hTab *
getRuntimeVariables(void)
{
  return _mainValues;
}


/* strncpy() with guaranteed NULL termination. */
char *strncpyz(char *dest, const char *src, size_t n)
{
    assert(n > 0);

    --n;
    /* paranoia... */
    if (strlen(src) > n)
    {
	fprintf(stderr, "strncpyz prevented buffer overrun!\n");
    }
    
    strncpy(dest, src, n);
    dest[n] = 0;
    return dest;
}

/* like strncat() with guaranteed NULL termination
 * The passed size should be the size of the dest buffer, not the number of 
 * bytes to copy.
 */
char *strncatz(char *dest, const char *src, size_t n)
{
    size_t maxToCopy;
    size_t destLen = strlen(dest);
    
    assert(n > 0);
    assert(n > destLen);
    
    maxToCopy = n - destLen - 1;
    
    /* paranoia... */
    if (strlen(src) + destLen >= n)
    {
	fprintf(stderr, "strncatz prevented buffer overrun!\n");
    }
    
    strncat(dest, src, maxToCopy);
    dest[n - 1] = 0;
    return dest;
}


/*-----------------------------------------------------------------*/
/* getBuildNumber - return build number                            */
/*-----------------------------------------------------------------*/
const char *getBuildNumber(void)
{
  return (SDCC_BUILD_NUMBER);
}

#if defined(HAVE_VSNPRINTF) || defined(HAVE_VSPRINTF)
size_t SDCCsnprintf(char *dst, size_t n, const char *fmt, ...)
{
  va_list args;
  int len;

  va_start(args, fmt);

# if defined(HAVE_VSNPRINTF)
  len = vsnprintf(dst, n, fmt, args);
# else
  vsprintf(dst, fmt, args);
  len = strlen(dst) + 1;
# endif

  va_end(args);

  /* on some gnu systems, vsnprintf returns -1 if output is truncated.
   * In the C99 spec, vsnprintf returns the number of characters that 
   * would have been written, were space available.
   */
  if ((len < 0) || (size_t) len >= n) {
    fprintf(stderr, "internal error: sprintf truncated.\n");
  }

  return len;
}
#endif

/** Pragma tokenizer
 */
void
init_pragma_token(struct pragma_token_s *token)
{
  dbuf_init(&token->dbuf, 16);
  token->type = TOKEN_UNKNOWN;
}

char *
get_pragma_token(const char *s, struct pragma_token_s *token)
{
  dbuf_set_size(&token->dbuf, 0);

  /* skip leading spaces */
  while (*s != '\n' && isspace(*s))
    ++s;

  if ('\0' == *s || '\n' == *s)
    {
      token->type = TOKEN_EOL;
    }
  else if (isdigit(*s))
    {
      char *end;

      long val = strtol(s, &end, 0);

      if (end != s && ('\0' == *end || isspace(*s)))
        {
          token->val.int_val = val;
          token->type = TOKEN_INT;
          dbuf_append(&token->dbuf, s, end - s);
        }
      s = end;
    }
  else
    {
      while ('\0' != *s && !isspace(*s))
        {
          dbuf_append(&token->dbuf, s, 1);
          ++s;
        }

      token->type = TOKEN_STR;
    }

    return (char *)s;
}

const char *
get_pragma_string(struct pragma_token_s *token)
{
  return dbuf_c_str(&token->dbuf);
}

void
free_pragma_token(struct pragma_token_s *token)
{
  dbuf_destroy(&token->dbuf);
}
