/*-------------------------------------------------------------------------
    sdcppmain.c - sdcpp: SDCC preprocessor main file, using cpplib.

    Written by Borut Razem, 2006.

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

#include "config.h"
#include "system.h"
#include "cpplib.h"
#include "cpphash.h"
#include "version.h"
#include "mkdeps.h"

#define _

#define CPP_FATAL_LIMIT 1000
/* True if we have seen a "fatal" error.  */
#define CPP_FATAL_ERRORS(PFILE) (cpp_errors (PFILE) >= CPP_FATAL_LIMIT)

const char *progname;		/* Needs to be global.  */


/* Set up dependency-file output.  On exit, if deps.style is non-zero
   then deps.file is not NULL; stdout is the empty string.  */
static void
init_dependency_output (cpp_reader *pfile)
{
  char *spec, *s, *output_file;

  /* Either of two environment variables can specify output of deps.
     Its value is either "OUTPUT_FILE" or "OUTPUT_FILE DEPS_TARGET",
     where OUTPUT_FILE is the file to write deps info to
     and DEPS_TARGET is the target to mention in the deps.  */

  if (CPP_OPTION (pfile, deps.style) == 0)
    {
      spec = getenv ("DEPENDENCIES_OUTPUT");
      if (spec)
	CPP_OPTION (pfile, deps.style) = 1;
      else
	{
	  spec = getenv ("SUNPRO_DEPENDENCIES");
	  if (spec)
	    {
	      CPP_OPTION (pfile, deps.style) = 2;
	      CPP_OPTION (pfile, deps.ignore_main_file) = 1;
	    }
	  else
	    return;
	}

      /* Find the space before the DEPS_TARGET, if there is one.  */
      s = strchr (spec, ' ');
      if (s)
	{
	  /* Let the caller perform MAKE quoting.  */
	  deps_add_target (pfile->deps, s + 1, 0);
	  output_file = (char *) xmalloc (s - spec + 1);
	  memcpy (output_file, spec, s - spec);
	  output_file[s - spec] = 0;
	}
      else
	output_file = spec;

      /* Command line -MF overrides environment variables and default.  */
      if (CPP_OPTION (pfile, deps.file) == 0)
	CPP_OPTION (pfile, deps.file) = output_file;

      CPP_OPTION (pfile, deps.append) = 1;
    }
  else if (CPP_OPTION (pfile, deps.file) == 0)
    /* If -M or -MM was seen without -MF, default output to wherever
       was specified with -o.  out_fname is non-NULL here.  */
    CPP_OPTION (pfile, deps.file) = CPP_OPTION (pfile, out_fname);
}

static void
sdcpp_post_options (cpp_reader *pfile)
{
  if (pfile->print_version)
    {
      fprintf (stderr, _("GNU CPP version %s (cpplib)"), version_string);
#ifdef TARGET_VERSION
      TARGET_VERSION;
#endif
      fputc ('\n', stderr);
    }

  /* Canonicalize in_fname and out_fname.  We guarantee they are not
     NULL, and that the empty string represents stdin / stdout.  */
  if (CPP_OPTION (pfile, in_fname) == NULL
      || !strcmp (CPP_OPTION (pfile, in_fname), "-"))
    CPP_OPTION (pfile, in_fname) = "";

  if (CPP_OPTION (pfile, out_fname) == NULL
      || !strcmp (CPP_OPTION (pfile, out_fname), "-"))
    CPP_OPTION (pfile, out_fname) = "";

  /* -dM makes no normal output.  This is set here so that -dM -dD
     works as expected.  */
  if (CPP_OPTION (pfile, dump_macros) == dump_only)
    CPP_OPTION (pfile, no_output) = 1;

  /* Disable -dD, -dN and -dI if we should make no normal output
     (such as with -M). Allow -M -dM since some software relies on
     this.  */
  if (CPP_OPTION (pfile, no_output))
    {
      if (CPP_OPTION (pfile, dump_macros) != dump_only)
	CPP_OPTION (pfile, dump_macros) = dump_none;
      CPP_OPTION (pfile, dump_includes) = 0;
    }

  /* We need to do this after option processing and before
     cpp_start_read, as cppmain.c relies on the options->no_output to
     set its callbacks correctly before calling cpp_start_read.  */
  init_dependency_output (pfile);

  /* After checking the environment variables, check if -M or -MM has
     not been specified, but other -M options have.  */
  if (CPP_OPTION (pfile, deps.style) == 0 &&
      (CPP_OPTION (pfile, deps.missing_files)
       || CPP_OPTION (pfile, deps.file)
       || CPP_OPTION (pfile, deps.phony_targets)))
    cpp_error (pfile, DL_ERROR, "you must additionally specify either -M or -MM");
}

/* Store the program name, and set the locale.  */
static void
general_init (const char *argv0)
{
  progname = argv0 + strlen (argv0);

  while (progname != argv0 && ! IS_DIR_SEPARATOR (progname[-1]))
    --progname;

  hex_init ();
}

/* Handle switches, preprocess and output.  */
static void
do_preprocessing (cpp_reader *pfile, int argc, char **argv)
{
  FILE *outf;
  int argi = 1;  /* Next argument to handle.  */

  argi += cpp_handle_options (pfile, argc - argi , argv + argi);
  if (CPP_FATAL_ERRORS (pfile))
    return;

  if (argi < argc)
    {
      cpp_error (pfile, DL_ERROR, "invalid option %s", argv[argi]);
      return;
    }

  sdcpp_post_options (pfile);
  if (CPP_FATAL_ERRORS (pfile))
    return;

  /* If cpp_handle_options saw --help or --version on the command
     line, it will have set pfile->help_only to indicate this.  Exit
     successfully.  [The library does not exit itself, because
     e.g. cc1 needs to print its own --help message at this point.]  */
  if (pfile->opts.help_only)
    return;

  /* Open the output now.  We must do so even if no_output is on,
     because there may be other output than from the actual
     preprocessing (e.g. from -dM).  */
  if (CPP_OPTION(pfile, out_fname)[0] == '\0')
    outf = stdout;
  else
    {
      outf = fopen (CPP_OPTION(pfile, out_fname), "w");
      if (outf == NULL)
	{
	  cpp_errno (pfile, DL_ERROR, CPP_OPTION(pfile, out_fname));
	  return;
	}
    }

  cpp_preprocess_file (pfile, pfile->opts.in_fname, outf);
}

int
main (int argc, char **argv)
{
  cpp_reader *pfile;

  general_init (argv[0]);

  /* Construct a reader with default language GNU C89.  */
  pfile = cpp_create_reader (CLK_GNUC89);

  do_preprocessing (pfile, argc, argv);

  cpp_destroy (pfile);

  return SUCCESS_EXIT_CODE;
}
