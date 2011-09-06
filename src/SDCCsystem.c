/*-------------------------------------------------------------------------
  SDCCsystem - SDCC system & pipe functions

             Written By - Sandeep Dutta . sandeep.dutta@usa.net (1999)

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
/* avoid DATADIR definition clash :-( */
#undef DATADIR
#include <windows.h>
#undef TRUE
#undef FALSE
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif
#include <string.h>
#include <ctype.h>
#include "SDCCglobl.h"
#include "SDCCutil.h"
#include "dbuf_string.h"
#include "SDCCsystem.h"
#include "newalloc.h"


set *binPathSet = NULL; /* set of binary paths */


/*!
 * get command and arguments from command line
 */

static void
split_command (const char *cmd_line, char **command, char **params)
{
  const char *p, *cmd_start;
  char delim;
  char *str;
  unsigned len;

  /* skip leading spaces */
  for (p = cmd_line; isspace (*p); p++)
    ;

  /* get command */
  switch (*p)
    {
    case '\'':
    case '"':
      delim = *p;
      cmd_start = ++p;
      break;

    default:
      delim = ' ';
      cmd_start = p;
    }

  if (delim == ' ')
    {
      while (*p != '\0' && !isspace (*p))
        p++;
    }
  else
    {
      while (*p != '\0' && *p != delim)
        p++;
    }

  if (command != NULL)
    {
      len = p - cmd_start;
      str = Safe_alloc (len + 1);
      strncpy (str, cmd_start, len);
      str[len] = '\0';
      *command = str;
    }

  p++;

  /* skip spaces before parameters */
  while (isspace (*p))
    p++;

  /* get parameters */
  if (params != NULL)
    *params = Safe_strdup (p);
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

static const char *
merge_command (const char *command, const char *params)
{
  struct dbuf_s dbuf;

  /* allocate extra space for ' ' and '\0' */
  dbuf_init (&dbuf, strlen (command) + strlen (params) + 2);

  dbuf_append_str (&dbuf, command);
  dbuf_append (&dbuf, " ", 1);
  dbuf_append_str (&dbuf, params);

  return dbuf_detach_c_str (&dbuf);
}


/*!
 * check if path/command exist by converting it to short file name
 * if it exists, compose with args and return it
 */

static const char *
compose_command_line (const char *path, const char *command, const char *args)
{
  unsigned len;
  struct dbuf_s cmdPath;
  char shortPath[PATH_MAX];

  dbuf_init (&cmdPath, PATH_MAX);

  if (path != NULL)
    dbuf_makePath (&cmdPath, path, command);
  else
    dbuf_append_str (&cmdPath, command);

  /* Try if cmdPath or cmdPath.exe exist by converting it to the short path name */
  len = GetShortPathName (dbuf_c_str (&cmdPath), shortPath, sizeof shortPath);
  assert (len < sizeof shortPath);
  if (0 == len)
    {
      dbuf_append_str (&cmdPath, EXE_EXT);
      len = GetShortPathName (dbuf_c_str (&cmdPath), shortPath, sizeof shortPath);
      assert (len < sizeof shortPath);
    }
  if (0 != len)
    {
      /* compose the command line */
      return merge_command (shortPath, args);
    }
  else
    {
      /* path/command not found */
      return NULL;
    }
}


static const char *
get_path (const char *cmd)
{
  const char *cmdLine;
  char *command;
  char *args;
  char *path;

  /* get the command */
  split_command (cmd, &command, &args);

  if (NULL == (cmdLine = compose_command_line(NULL, command, args)))
    {
      /* not an absolute path: try to find the command in predefined binary paths */
      if (NULL != (path = (char *)setFirstItem (binPathSet)))
        {
          while (NULL == (cmdLine  = compose_command_line (path, command, args)) &&
            NULL != (path = (char *)setNextItem (binPathSet)))
            ;
        }

      if (NULL == cmdLine)
        {
          /* didn't found the command in predefined binary paths: try with PATH */
          char *envPath;

          if (NULL != (envPath = getenv("PATH")))
            {
              /* make a local copy; strtok() will modify it */
              envPath = Safe_strdup (envPath);

              if (NULL != (path = strtok (envPath, ";")))
                {
                  while (NULL == (cmdLine = compose_command_line (path, command, args)) &&
                    NULL != (path = strtok (NULL, ";")))
                    ;
                }

              Safe_free (envPath);
          }
    }

    /* didn't found it; probably this won't help neither :-( */
    if (NULL == cmdLine)
      cmdLine = merge_command (command, args);
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

static const char *
merge_command (const char *command, const char *params)
{
  struct dbuf_s dbuf;

  /* allocate extra space for 2x'"', ' ' and '\0' */
  dbuf_init (&dbuf, strlen (command) + strlen (params) + 4);
  dbuf_append (&dbuf, "\"", 1);
  dbuf_append_str (&dbuf, command);
  dbuf_append (&dbuf, "\" ", 2);
  dbuf_append_str (&dbuf, params);

  return dbuf_detach_c_str (&dbuf);
}


/*!
 * check if the path is relative or absolute (if contains the dir separator)
 */

static int
has_path (const char *path)
{
  return dbuf_splitPath (path, NULL, NULL);
}


static const char *
get_path (const char *cmd)
{
  const char *cmdLine = NULL;
  char *command;
  char *args;
  char *path;

  /* get the command */
  split_command (cmd, &command, &args);

  if (!has_path (command))
    {
      /* try to find the command in predefined binary paths */
      if (NULL != (path = (char *)setFirstItem (binPathSet)))
        {
          do
            {
              struct dbuf_s dbuf;
              const char *cmdPath;

              dbuf_init (&dbuf, PATH_MAX);
              dbuf_makePath (&dbuf, path, command);
              cmdPath = dbuf_detach (&dbuf);

              /* Try if cmdPath */
              if (0 == access (cmdPath, X_OK))
                {
                  /* compose the command line */
                  cmdLine = merge_command (cmdPath, args);
                  break;
                }
            } while (NULL != (path = (char *)setNextItem (binPathSet)));
        }
      if (NULL == cmdLine)
        cmdLine = merge_command (command, args);

      Safe_free (command);
      Safe_free (args);

      return cmdLine;
    }
  else
    {
      /*
       * the command is defined with absolute path:
       * just return it
       */
      Safe_free (command);
      Safe_free (args);

      return Safe_strdup (cmd);
    }
}
#endif


/*!
 * call an external program with arguements
 */

int
sdcc_system (const char *cmd)
{
  int e;
  const char *cmdLine = get_path (cmd);

  assert (NULL != cmdLine);

  if (options.verboseExec)
    printf ("+ %s\n", cmdLine);

  e = system (cmdLine);

  if (options.verboseExec && e)
    printf ("+ %s returned errorcode %d\n", cmdLine, e);

  dbuf_free (cmdLine);

  return e;
}


/*!
 * pipe an external program with arguements
 */

#ifdef _WIN32
static HANDLE hProcess = NULL;
/*
 * use native WIN32 solution due to a bug in wine msvrct.dll popen implementation
 * (see http://bugs.winehq.org/show_bug.cgi?id=25062)
 * and due to incorrect wine msvrct.dll pclose implementation
 * (see http://bugs.winehq.org/show_bug.cgi?id=25063)
 */
static FILE *
sdcc_popen_read (const char *cmd)
{ 
  HANDLE childIn, childOut;
  SECURITY_ATTRIBUTES sa;
  PROCESS_INFORMATION pi;
  STARTUPINFO si;
  char *lcmd;

  assert (hProcess == NULL);

  sa.nLength = sizeof (SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = NULL;

  if (!CreatePipe (&childIn, &childOut, &sa, 0))
    {
      _doserrno = GetLastError ();
      return NULL;
    }

  if (!SetHandleInformation (childIn, HANDLE_FLAG_INHERIT, 0))
    {
      _doserrno = GetLastError ();
      return NULL;
    }

  memset (&si, 0, sizeof (STARTUPINFO));
  si.cb = sizeof (STARTUPINFO); 
  si.hStdError = GetStdHandle (STD_ERROR_HANDLE);
  si.hStdOutput = childOut;
  si.hStdInput = GetStdHandle (STD_INPUT_HANDLE);
  si.dwFlags |= STARTF_USESTDHANDLES;

  lcmd = Safe_strdup(cmd);
  if (!CreateProcess (NULL, lcmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
      _doserrno = GetLastError ();
      Safe_free(lcmd);
      return NULL;
    }
  Safe_free(lcmd);

  CloseHandle (childOut);
  CloseHandle (pi.hThread);
  hProcess = pi.hProcess;

  return fdopen (_open_osfhandle ((intptr_t)childIn, _O_RDONLY | _O_TEXT), "rt");
}

/*
 * use native WIN32 solution due to incorrect wine msvrct.dll pclose implementation
 * (see http://bugs.winehq.org/show_bug.cgi?id=25063)
 */
int
sdcc_pclose (FILE *fp)
{
  int ret = -1;

  if (fp != NULL)
    {
      DWORD exitStatus;

      assert (hProcess != NULL);

      if ((WaitForSingleObject (hProcess, (DWORD)(-1L)) == 0) &&
        GetExitCodeProcess (hProcess, &exitStatus))
        ret = exitStatus;

      CloseHandle (hProcess);

      fclose (fp);

      hProcess = NULL;
    }

    return ret;
}
#else
#define sdcc_popen_read(cmd)  popen ((cmd), "r")
int
sdcc_pclose (FILE *fp)
{
  return pclose (fp);
}
#endif

FILE *
sdcc_popen (const char *cmd)
{
  FILE *fp;
  const char *cmdLine = get_path (cmd);

  assert (NULL != cmdLine);

  if (options.verboseExec)
    {
      printf ("+ %s\n", cmdLine);
    }

  fp = sdcc_popen_read (cmdLine);
  dbuf_free (cmdLine);

  return fp;
}
