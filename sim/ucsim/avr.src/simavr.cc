/*
 * Simulator of microcontrollers (simavr.cc)
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
 * 
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 */

/* This file is part of microcontroller simulator: ucsim.

UCSIM is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

UCSIM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UCSIM; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA. */
/*@1@*/


#include <ctype.h>

// sim.src
#include "appcl.h"

// local
#include "simavrcl.h"
#include "avrcl.h"


cl_simavr::cl_simavr(class cl_app *the_app, int iargc, char *iargv[]):
  cl_sim(the_app, "h", iargc, iargv)
{}


static void
print_help(char *name)
{
  printf("%s: %s\n", name, VERSIONSTR);
  printf("Usage: %s [-hHVvP] [-p prompt] [-t CPU] [-X freq[k|M]]\n"
	 "       [-c file] [-s file] [-S optionlist]"
#ifdef SOCKET_AVAIL
	 " [-Z portnum]"
#endif
	 "\n"
	 "       [files...]\n", name);
  printf
    (
     "Options:\n"
     "  -t CPU       Type of CPU: etc.\n"
     "  -X freq[k|M] XTAL frequency\n"
     "  -c file      Open command console on `file'\n"
#ifdef SOCKET_AVAIL
     "  -Z portnum   Use localhost:portnumber for command console\n"
#endif
     "  -s file      Connect serial interface to `file'\n"
     "  -S options   `options' is a comma separated list of options\n"
     "               according to serial interface. Know options are:\n"
     "                  in=file   serial input will be read from file named `file'\n"
     "                  out=file  serial output will be written to `file'\n"
     "  -p prompt    Specify string for prompt\n"
     "  -P           Prompt is a null ('\\0') character\n"
     "  -V           Verbose mode\n"
     "  -v           Print out version number\n"
     "  -H           Print out types of known CPUs\n"
     "  -h           Print out this help\n"
     );
}


int
cl_simavr::proc_arg(char optopt, char *optarg)
{
  switch (optopt)
    {
    
    case 'h':
      
      print_help("savr");
      exit(0);
      break;
    
    case '?':

      if (isprint(optopt))
	fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
	fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return(1);
      break;
      
    default:
      // should never happen...
      abort();
    }
}


class cl_uc *
cl_simavr::mk_controller(void)
{
  return(new cl_avr(this));
}


/* End of avr.src/simavr.cc */
