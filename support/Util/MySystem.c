/*-------------------------------------------------------------------------
  MySystem - SDCC Support function

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
#include "MySystem.h"
#include "newalloc.h"
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#define EXE_EXT ".exe"
#else
#define EXE_EXT ""
#endif


set *binPathSet = NULL; /* set of binary paths */


/*!
 * get command and arguments from command line
 */

static void
split_command(const char *cmd_line, char **command, char **params)
{
  const char *p, *cmd_start;
  char delim;
  char *str;
  unsigned len;

  /* skip leading spaces */
  for (p = cmd_line; isspace(*p); p++)
    ;

  /* get command */
  switch (*p) {
  case '\'':
  case '"':
    delim = *p;
    cmd_start = ++p;
    break;

  default:
    delim = ' ';
    cmd_start = p;
  }

  if (delim == ' ') {
    while (*p != '\0' && !isspace(*p))
      p++;
  }
  else {
    while (*p != '\0' && *p != delim)
      p++;
  }

  if (command != NULL) {
    len = p - cmd_start;
    str = Safe_alloc(len + 1);
    strncpy(str, cmd_start, len);
    str[len] = '\0';
    *command = str;
  }

  p++;

  /* skip spaces before parameters */
  while (isspace(*p))
    p++;

  /* get parameters */
  if (params != NULL)
    *params = Safe_strdup(p);
}


/*!
 * merge command and parameters to command line
 */

static char *
merge_command(const char *command, const char *params)
{
  char *cmd_line;
  size_t len;

  /* allocate extra space for ' ' and '\0' */
  len = strlen(command) + strlen(params) + 2;
  cmd_line = (char *)Safe_alloc(len);
  SNPRINTF(cmd_line, len, "%s %s", command, params);

  return cmd_line;
}


/*!
 * check if the path is absolute
 */

static int
has_path(const char *path)
{
  if (strrchr(path, DIR_SEPARATOR_CHAR) == NULL)
#ifdef _WIN32
    /* try *nix deir separator on WIN32 */
    if (strrchr(path, UNIX_DIR_SEPARATOR_CHAR) == NULL)
#endif
      return 0;

  return 1;
}


/*!
 * find the command:
 * 1) if the command is specified by absolute or relative path, try it
 * 2) try to find the command in predefined path's
 * 3) trust on $PATH
 */

static char *
get_path(const char *cmd)
{
  char *cmdLine = NULL;
  char *command;
  char *args;
  char *path;
  char cmdPath[PATH_MAX];


  /* get the command */
  split_command(cmd, &command, &args);

  if (!has_path(command)) {
    /* try to find the command in predefined binary paths */
    if ((path = (char *)setFirstItem(binPathSet)) != NULL) {
      do
      {
        SNPRINTF(cmdPath, sizeof cmdPath,
          "%s" DIR_SEPARATOR_STRING "%s", path, command);

#ifdef _WIN32
        /* Try if cmdPath or cmdPath.exe exist */
        if (0 == access(cmdPath, 0) ||
          0 == access(strncatz(cmdPath, EXE_EXT, sizeof cmdPath), 0)) {
#else
        /* Try if cmdPath */
        if (0 == access(cmdPath, X_OK)) {
#endif
          /* compose the command line */
          cmdLine = merge_command(cmdPath, args);
          break;
        }
      } while ((path = (char *)setNextItem(binPathSet)) != NULL);
    }
    if (cmdLine == NULL)
      cmdLine = merge_command(command, args);
  }
  else {
    /*
     * the command is defined with absolute path:
     * just return it
     */
    return Safe_strdup(cmd);
  }

  Safe_free(command);
  Safe_free(args);

  return cmdLine;
}


/*!
 * call an external program with arguements
 */

int
my_system(const char *cmd)
{
  int e;
  char *cmdLine = get_path(cmd);

  assert(cmdLine != NULL);

  if (options.verboseExec) {
      printf("+ %s\n", cmdLine);
  }

  e = system(cmdLine);
  Safe_free(cmdLine);

  return e;
}


/*!
 * pipe an external program with arguements
 */

#ifdef _WIN32
#define popen_read(cmd) _popen((cmd), "rt")
#else
#define popen_read(cmd) popen((cmd), "r")
#endif

FILE *
my_popen(const char *cmd)
{
  FILE *fp;
  char *cmdLine = get_path(cmd);

  assert(cmdLine != NULL);

  if (options.verboseExec) {
      printf("+ %s\n", cmdLine);
  }

  fp = popen_read(cmdLine);
  Safe_free(cmdLine);

  return fp;
}
