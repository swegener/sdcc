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

#ifdef _WIN32
#undef DATADIR
#include <windows.h>
/* avoid DATADIR definition clash :-( */
#include <io.h>
#else
#include <unistd.h>
#endif
#include <ctype.h>
#include "SDCCglobl.h"
#include "SDCCutil.h"
#include "MySystem.h"
#include "newalloc.h"


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
 * find the command:
 * 1) if the command is specified by path, try it
 * 2) try to find the command in predefined path's
 * 3) trust on $PATH
 */

#ifdef _WIN32
/* WIN32 version */

/*
 * I don't like this solution, but unfortunately cmd.exe and command.com
 * don't accept something like this:
 * "program" "argument"
 * Cmd.exe accepts the following:
 * ""program" "argument""
 * but command.com doesn't.
 * The following is accepted by both:
 * program "argument"
 *
 * So the most portable WIN32 solution is to use GetShortPathName() for
 * program to get rid of spaces, so that quotes are not needed :-(
 * Using spawnvp() instead of system() is more portable cross platform approach,
 * but then also a substitute for _popen() should be developed...
 */

#define EXE_EXT ".exe"

/*!
 * merge command and parameters to command line
 */

static char *
merge_command(const char *command, const char *params)
{
  /* allocate extra space for ' ' and '\0' */
  char *cmd_line = (char *)Safe_alloc(strlen(command) + strlen(params) + 2);
  sprintf(cmd_line, "%s %s", command, params);

  return cmd_line;
}


/*!
 * check if path/command exist by converting it to short file name
 * if it exists, compose with args and return it
 */

static char *
compose_command_line(const char *path, const char *command, const char *args)
{
  unsigned len;
  char cmdPath[PATH_MAX];
  char shortPath[PATH_MAX];

  if (path != NULL)
    SNPRINTF(cmdPath, sizeof cmdPath,
      "%s" DIR_SEPARATOR_STRING "%s", path, command);
  else
    strncpyz(cmdPath, command, sizeof cmdPath);

  /* Try if cmdPath or cmdPath.exe exist by converting it to the short path name */
  len = GetShortPathName(cmdPath, shortPath, sizeof shortPath);
  assert(len < sizeof shortPath);
  if (0 == len) {
    len = GetShortPathName(strncatz(cmdPath, EXE_EXT, sizeof cmdPath), shortPath, sizeof shortPath);
    assert(len < sizeof shortPath);
  }
  if (0 != len) {
    /* compose the command line */
    return merge_command(shortPath, args);
  }
  else {
    /* path/command not found */
    return NULL;
  }
}


static char *
get_path(const char *cmd)
{
  char *cmdLine;
  char *command;
  char *args;
  char *path;

  /* get the command */
  split_command(cmd, &command, &args);

  if (NULL == (cmdLine = compose_command_line(NULL, command, args))) {
    /* not an absolute path: try to find the command in predefined binary paths */
    if (NULL != (path = (char *)setFirstItem(binPathSet))) {
      while (NULL == (cmdLine  = compose_command_line(path, command, args)) &&
        NULL != (path = (char *)setNextItem(binPathSet)))
        ;
    }

    if (NULL == cmdLine) {
      /* didn't found the command in predefined binary paths: try with PATH */
      char *envPath;

      if (NULL != (envPath = getenv("PATH"))) {
        /* make a local copy; strtok() will modify it */
        envPath = Safe_strdup(envPath);

        if (NULL != (path = strtok(envPath, ";"))) {
          while (NULL == (cmdLine = compose_command_line(path, command, args)) &&
           NULL != (path = strtok(NULL, ";")))
           ;
        }

        Safe_free(envPath);
      }
    }

    /* didn't found it; probably this won't help neither :-( */
    if (NULL == cmdLine)
      cmdLine = merge_command(command, args);
  }

  Safe_free(command);
  Safe_free(args);

  return cmdLine;
}

#else
/* *nix version */

/*!
 * merge command and parameters to command line
 */

static char *
merge_command(const char *command, const char *params)
{
  /* allocate extra space for 2x'"', ' ' and '\0' */
  char *cmd_line = (char *)Safe_alloc(strlen(command) + strlen(params) + 4);
  sprintf(cmd_line, "\"%s\" %s", command, params);

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
    /* try *nix dir separator on WIN32 */
    if (strrchr(path, UNIX_DIR_SEPARATOR_CHAR) == NULL)
#endif
      return 0;

  return 1;
}


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
    if (NULL != (path = (char *)setFirstItem(binPathSet))) {
      do
      {
        SNPRINTF(cmdPath, sizeof cmdPath,
          "%s" DIR_SEPARATOR_STRING "%s", path, command);

        /* Try if cmdPath */
        if (0 == access(cmdPath, X_OK)) {
          /* compose the command line */
          cmdLine = merge_command(cmdPath, args);
          break;
        }
      } while (NULL != (path = (char *)setNextItem(binPathSet)));
    }
    if (NULL == cmdLine)
      cmdLine = merge_command(command, args);

    Safe_free(command);
    Safe_free(args);

    return cmdLine;
  }
  else {
    /*
     * the command is defined with absolute path:
     * just return it
     */
    Safe_free(command);
    Safe_free(args);

    return Safe_strdup(cmd);
  }
}
#endif


/*!
 * call an external program with arguements
 */

int
my_system(const char *cmd)
{
  int e;
  char *cmdLine = get_path(cmd);

  assert(NULL != cmdLine);

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

  assert(NULL != cmdLine);

  if (options.verboseExec) {
      printf("+ %s\n", cmdLine);
  }

  fp = popen_read(cmdLine);
  Safe_free(cmdLine);

  return fp;
}
