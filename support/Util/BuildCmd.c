/*-------------------------------------------------------------------------
  BuildCmd - SDCC Support function

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

/*! Build a command line with parameter substitution
*/

#include <string.h>
#include <assert.h>
#include "SDCCset.h"
#include "BuildCmd.h"

void
buildCmdLine (char *into, const char **cmds,
	      const char *p1, const char *p2,
	      const char *p3, set *list)
{
  int first = 1;

  assert(cmds != NULL);
  assert(into != NULL);

  *into = '\0';

  while (*cmds) {
    const char *p, *from, *par;
    int sep = 1;

    from = *cmds;
    cmds++;

    /* See if it has a '$' anywhere - if not, just copy */
    if ((p = strchr (from, '$'))) {
      /* include first part of cmd */
      if (p != from) {
        if (!first && sep)
          strcat(into, " ");
        strncat(into, from, p - from);
        sep = 0;
      }
      from = p + 2;

      /* include parameter */
      p++;
      switch (*p) {
      case '1':
        par = p1;
	break;

      case '2':
        par = p2;
	break;

      case '3':
        par = p3;
	break;

      case 'l':
        {
          const char *tmp;
          par = NULL;

          if (list != NULL && (tmp = (const char *)setFirstItem(list)) != NULL) {
            do
            {
              if (*tmp != '\0') {
                if (sep)
		  strcat(into, " ");  /* seperate it */
                strcat(into, tmp);
                tmp++;
                sep = 1;
              }
            } while ((tmp = (const char *)setNextItem(list)) != NULL);
          }
        }
	break;

      default:
        par = NULL;
	assert(0);
      }

      if (par && *par != '\0') {
        if (!first && sep)
          strcat(into, " ");   /* seperate it */
        strcat(into, par);
        sep = 0;
      }
    }

    /* include the rest of cmd, e.g. ".asm" from "$1.asm" */
    if (*from != '\0') {
      if (!first && sep)
        strcat(into, " ");   /* seperate it */
      strcat(into, from);
      sep = 0;
    }

    first = 0;
  }
}
