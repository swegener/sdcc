/*
 * Simulator of microcontrollers (sim51.cc)
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

#include "ddconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "i_string.h"

#include "globals.h"
#include "utils.h"

#include "sim51cl.h"
#include "cmd51cl.h"
#include "uc51cl.h"
#include "uc52cl.h"
#include "uc51rcl.h"
#include "uc89c51rcl.h"
#include "uc251cl.h"
#include "glob.h"


cl_sim51::cl_sim51(int iargc, char *iargv[]):
  cl_sim("t:s:S:hH", iargc, iargv)
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
     "  -t CPU       Type of CPU: 51, C52, 251, etc.\n"
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

enum {
  SOPT_IN= 0,
  SOPT_OUT
};

static const char *S_opts[]= {
  /*[SOPT_IN]=*/ "in",
  /*[SOPT_OUT]=*/ "out",
  NULL
};

int
cl_sim51::proc_arg(char optopt, char *optarg)
{
  char *cpu_type= NULL, *cp;
  int i;
  char *subopts, *value;

  switch (optopt)
    {

    case 't':

      if (cpu_type)
	free(cpu_type);
      cpu_type= strdup(optarg);
      for (cp= cpu_type; *cp; *cp= toupper(*cp), cp++);
      arguments->add(new cl_prg_arg('t', 0, cpu_type));
      break;

    case 's':
      {
	FILE *Ser_in, *Ser_out;
	if (arg_avail('s'))
	  {
	    fprintf(stderr, "-s option can not be used more than once.\n");
	    break;
	  }
	arguments->add(new cl_prg_arg('s', 0, (long long)1));
	if ((Ser_in= fopen(optarg, "r")) == NULL)
	  {
	    fprintf(stderr,
		    "Can't open `%s': %s\n", optarg, strerror(errno));
	    return(4);
	  }
	arguments->add(new cl_prg_arg(0, "Ser_in", Ser_in));
	if ((Ser_out= fopen(optarg, "w")) == NULL)
	  {
	    fprintf(stderr,
		    "Can't open `%s': %s\n", optarg, strerror(errno));
	    return(4);
	  }
	arguments->add(new cl_prg_arg(0, "Ser_out", Ser_out));
	break;
      }

    case 'S':

      subopts= optarg;
      while (*subopts != '\0')
	switch (get_sub_opt(&subopts, S_opts, &value))
	  {
	    FILE *Ser_in, *Ser_out;
	  case SOPT_IN:
	    if (value == NULL) {
	      fprintf(stderr, "No value for -S in\n");
	      exit(1);
	    }
	    if (arg_avail("Ser_in"))
	      {
		fprintf(stderr, "Serial input specified more than once.\n");
		break;
	      }
	    if ((Ser_in= fopen(value, "r")) == NULL)
	      {
		fprintf(stderr,
			"Can't open `%s': %s\n", value, strerror(errno));
		exit(4);
	      }
	    arguments->add(new cl_prg_arg(0, "Ser_in", Ser_in));
	    break;
	  case SOPT_OUT:
	    if (value == NULL) {
	      fprintf(stderr, "No value for -S out\n");
	      exit(1);
	    }
	    if (arg_avail("Ser_out"))
	      {
		fprintf(stderr, "Serial output specified more than once.\n");
		break;
	      }
	    if ((Ser_out= fopen(value, "w")) == NULL)
	      {
		fprintf(stderr,
			"Can't open `%s': %s\n", value, strerror(errno));
		exit(4);
	      }
	    arguments->add(new cl_prg_arg(0, "Ser_out", Ser_out));
	    break;
	  default:
	    /* Unknown suboption. */
	    fprintf(stderr, "Unknown suboption `%s' for -S\n", value);
	    exit(1);
	    break;
	  }
      break;

    case 'h':

      print_help("s51");
      exit(0);
      break;
      
    case 'H':
      i= 0;
      while (cpus_51[i].type_str != NULL)
	{
	  printf("%s\n", cpus_51[i].type_str);
	  i++;
	}
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
  return(0);
}

class cl_commander *
cl_sim51::mk_commander(void)
{
  class cl_commander *cmd= new cl_51cmd(this);
  return(cmd);
}

class cl_uc *
cl_sim51::mk_controller(void)
{
  int i;

  i= 0;
  if (get_sarg('t', 0) == NULL)
    simulator->arguments->add(new cl_prg_arg('t', 0, "C51"));
  while ((cpus_51[i].type_str != NULL) &&
	 (strcmp(get_sarg('t', 0), cpus_51[i].type_str) != 0))
    i++;
  if (cpus_51[i].type_str == NULL)
    {
      fprintf(stderr, "Unknown processor type. "
	      "Use -H option to see known types.\n");
      return(NULL);
    }
  switch (cpus_51[i].type)
    {
    case CPU_51: case CPU_31:
      return(new t_uc51(cpus_51[i].type, cpus_51[i].technology, this));
    case CPU_52: case CPU_32:
      return(new t_uc52(cpus_51[i].type, cpus_51[i].technology, this));
    case CPU_51R:
      return(new t_uc51r(cpus_51[i].type, cpus_51[i].technology, this));
    case CPU_89C51R:
      return(new t_uc89c51r(cpus_51[i].type, cpus_51[i].technology, this));
    case CPU_251:
      return(new t_uc251(cpus_51[i].type, cpus_51[i].technology, this));
    }
  return(NULL);
}

void
cl_sim51::build_cmd_set(void)
{
  cl_sim::build_cmd_set();
  cmdset->del("ds");
}


/* End of s51.src/sim51.cc */
