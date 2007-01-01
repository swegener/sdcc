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
#include "internal.h"
#include "version.h"
#include "mkdeps.h"
#include "opts.h"
#include "intl.h"
#include "c-pretty-print.h"
#include "diagnostic.h"

#define CPP_FATAL_LIMIT 1000
/* True if we have seen a "fatal" error.  */
#define CPP_FATAL_ERRORS(PFILE) (cpp_errors (PFILE) >= CPP_FATAL_LIMIT)

const char *progname;		/* Needs to be global.  */

/* From laghooks-def.h */
/* The whole thing.  The structure is defined in langhooks.h.  */
#define LANG_HOOKS_INITIALIZER { \
  LANG_HOOKS_INIT_OPTIONS, \
  LANG_HOOKS_INITIALIZE_DIAGNOSTICS, \
  LANG_HOOKS_HANDLE_OPTION, \
  LANG_HOOKS_MISSING_ARGUMENT, \
  LANG_HOOKS_POST_OPTIONS, \
  LANG_HOOKS_INIT, \
  LANG_HOOKS_FINISH, \
  LANG_HOOKS_PRINT_ERROR_FUNCTION, \
}

/* From c-lang.c */
#define LANG_HOOKS_INIT_OPTIONS sdcpp_init_options
#define LANG_HOOKS_INITIALIZE_DIAGNOSTICS sdcpp_initialize_diagnostics
#define LANG_HOOKS_HANDLE_OPTION sdcpp_common_handle_option
#define LANG_HOOKS_MISSING_ARGUMENT sdcpp_common_missing_argument
#define LANG_HOOKS_POST_OPTIONS sdcpp_common_post_options
#define LANG_HOOKS_INIT sdcpp_common_init
#define LANG_HOOKS_FINISH sdcpp_common_finish
#define LANG_HOOKS_PRINT_ERROR_FUNCTION sdcpp_print_error_function

static unsigned int sdcpp_init_options (unsigned int argc, const char **argv);
static void sdcpp_initialize_diagnostics (diagnostic_context *context);
static void sdcpp_print_error_function (diagnostic_context *context, const char *file);

/* Each front end provides its own lang hook initializer.  */
const struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;

/* Name of top-level original source file (what was input to cpp).
   This comes from the #-command at the beginning of the actual input.
   If there isn't any there, then this is the cc1 input file name.  */

const char *main_input_filename;

#ifndef USE_MAPPED_LOCATION
location_t unknown_location = { NULL, 0 };
#endif

/* Current position in real source file.  */

location_t input_location;

struct line_maps line_table;

/* Stack of currently pending input files.  */

struct file_stack *input_file_stack;

/* Incremented on each change to input_file_stack.  */
int input_file_stack_tick;

/* Temporarily suppress certain warnings.
   This is set while reading code from a system header file.  */

int in_system_header = 0;

/* Nonzero means change certain warnings into errors.
   Usually these are warnings about failure to conform to some standard.  */

int flag_pedantic_errors = 0;

cpp_reader *parse_in;		/* Declared in c-pragma.h.  */

/* Nonzero means `char' should be signed.  */

int flag_signed_char;

/* Nonzero means don't output line number information.  */

char flag_no_line_commands;

/* Nonzero causes -E output not to be done, but directives such as
   #define that have side effects are still obeyed.  */

char flag_no_output;

/* Nonzero means dump macros in some fashion.  */

char flag_dump_macros;

/* Nonzero means pass #include lines through to the output.  */

char flag_dump_includes;

/* 0 means we want the preprocessor to not emit line directives for
   the current working directory.  1 means we want it to do it.  -1
   means we should decide depending on whether debugging information
   is being emitted or not.  */

int flag_working_directory = -1;

/* The current working directory of a translation.  It's generally the
   directory from which compilation was initiated, but a preprocessed
   file may specify the original directory in which it was
   created.  */

static const char *src_pwd;

/* Initialize src_pwd with the given string, and return true.  If it
   was already initialized, return false.  As a special case, it may
   be called with a NULL argument to test whether src_pwd has NOT been
   initialized yet.  */

/* From intl.c */
/* Opening quotation mark for diagnostics.  */
const char *open_quote = "'";

/* Closing quotation mark for diagnostics.  */
const char *close_quote = "'";
/* ----------- */

bool
set_src_pwd (const char *pwd)
{
  if (src_pwd)
    return false;

  src_pwd = xstrdup (pwd);
  return true;
}

/* Return the directory from which the translation unit was initiated,
   in case set_src_pwd() was not called before to assign it a
   different value.  */

const char *
get_src_pwd (void)
{
  if (! src_pwd)
    src_pwd = getpwd ();

   return src_pwd;
}

/* SDCPP specific pragmas */
/* SDCC specific
   sdcc_hash pragma */
static void
do_pragma_sdcc_hash (cpp_reader *pfile)
{
    const cpp_token *tok = _cpp_lex_token (pfile);

    if (tok->type == CPP_PLUS)
    {
	CPP_OPTION(pfile, allow_naked_hash)++;
    }
    else if (tok->type == CPP_MINUS)
    {
	CPP_OPTION(pfile, allow_naked_hash)--;
    }
    else
    {
	cpp_error (pfile, CPP_DL_ERROR,
		   "invalid #pragma sdcc_hash directive, need '+' or '-'");
    }
}

/* SDCC specific
   pedantic_parse_number pragma */
static void
do_pragma_pedantic_parse_number (cpp_reader *pfile)
{
    const cpp_token *tok = _cpp_lex_token (pfile);

  if (tok->type == CPP_PLUS)
    {
      CPP_OPTION(pfile, pedantic_parse_number)++;
    }
  else if (tok->type == CPP_MINUS)
    {
      CPP_OPTION(pfile, pedantic_parse_number)--;
    }
  else
    {
      cpp_error (pfile, CPP_DL_ERROR,
                 "invalid #pragma pedantic_parse_number directive, need '+' or '-'");
    }
}

/* SDCC _asm specific
   switch _asm block preprocessing on / off */
static void
do_pragma_preproc_asm (cpp_reader *pfile)
{
  const cpp_token *tok = _cpp_lex_token (pfile);

  if (tok->type == CPP_PLUS)
    {
      CPP_OPTION(pfile, preproc_asm)++;
    }
  else if (tok->type == CPP_MINUS)
    {
      CPP_OPTION(pfile, preproc_asm)--;
    }
  else
    {
      cpp_error (pfile, CPP_DL_ERROR,
		 "invalid #pragma preproc_asm directive, need '+' or '-'");
    }
}

/* SDCPP specific option initialization */
static unsigned int
sdcpp_init_options (unsigned int argc, const char **argv)
{
  unsigned int ret = sdcpp_common_init_options(argc, argv);

  CPP_OPTION (parse_in, preproc_asm) = 1;
  CPP_OPTION (parse_in, pedantic_parse_number) = 0;
  CPP_OPTION (parse_in, obj_ext) = NULL;

  /* Kevin abuse for SDCC. */
  cpp_register_pragma(parse_in, 0, "sdcc_hash", do_pragma_sdcc_hash, false);
  /* SDCC _asm specific */
  cpp_register_pragma(parse_in, 0, "preproc_asm", do_pragma_preproc_asm, false);
  /* SDCC specific */
  cpp_register_pragma(parse_in, 0, "pedantic_parse_number", do_pragma_pedantic_parse_number, false);

  /* SDCC _asm specific */
  parse_in->spec_nodes.n__asm = cpp_lookup (parse_in, DSC("_asm"));

  return ret;
}

static void
sdcpp_initialize_diagnostics (diagnostic_context *context)
{
  pretty_printer *base = context->printer;
  c_pretty_printer *pp = xmalloc (sizeof (c_pretty_printer));
  memcpy (pp_base (pp), base, sizeof (pretty_printer));
  pp_c_pretty_printer_init (pp);
  context->printer = (pretty_printer *) pp;

  /* It is safe to free this object because it was previously malloc()'d.  */
  free (base);
}

/* The default function to print out name of current function that caused
   an error.  */
static void
sdcpp_print_error_function (diagnostic_context *context, const char *file)
{
}

/* Initialize the PRETTY-PRINTER for handling C codes.  */

void
pp_c_pretty_printer_init (c_pretty_printer *pp)
{
}

void
print_version (FILE *file, const char *indent)
{
  fprintf (file, _("GNU CPP version %s (cpplib)"), version_string);
#ifdef TARGET_VERSION
  TARGET_VERSION;
#endif
  fputc ('\n', file);
}

/* Initialization of the front end environment, before command line
   options are parsed.  Signal handlers, internationalization etc.
   ARGV0 is main's argv[0].  */
static void
general_init (const char *argv0)
{
  const char *p;

  p = argv0 + strlen (argv0);
  while (p != argv0 && !IS_DIR_SEPARATOR (p[-1]))
    --p;
  progname = p;

  xmalloc_set_program_name (progname);

  hex_init ();

  gcc_init_libintl ();

  /* Initialize the diagnostics reporting machinery, so option parsing
     can give warnings and errors.  */
  diagnostic_initialize (global_dc);
}

/* Process the options that have been parsed.  */
static void
process_options (void)
{
  /* Allow the front end to perform consistency checks and do further
     initialization based on the command line options.  This hook also
     sets the original filename if appropriate (e.g. foo.i -> foo.c)
     so we can correctly initialize debug output.  */
  /*no_backend =*/ (*lang_hooks.post_options) (&main_input_filename);
  input_filename = main_input_filename;
}

#if 0
/* A warning.  Use this for code which is correct according to the
   relevant language specification but is likely to be buggy anyway.  */
void
warning (const char *msgid, ...)
{
  va_list ap;

  va_start (ap, msgid);
  fprintf (stderr, "%s: error: ", progname);
  vfprintf (stderr, msgid, ap);
  va_end (ap);
}

/* A hard error: the code is definitely ill-formed, and an object file
   will not be produced.  */
void
error (const char *msgid, ...)
{
  va_list ap;

  va_start (ap, msgid);
  fprintf (stderr, "%s: warning: ", progname);
  vfprintf (stderr, msgid, ap);
  va_end (ap);
}

/* Print a fatal I/O error message.  Argument are like printf.
   Also include a system error message based on `errno'.  */
void
fatal_io_error (const char *msgid, ...)
{
  va_list ap;

  va_start (ap, msgid);
  fprintf (stderr, "%s: %s: ", progname, xstrerror (errno));
  vfprintf(stderr, msgid, ap);
  va_end (ap);
  exit (FATAL_EXIT_CODE);
}
#endif

/* Parse a -d... command line switch.  */

void
decode_d_option (const char *arg)
{
  int c;

  while (*arg)
    switch (c = *arg++)
      {
      case 'D':	/* These are handled by the preprocessor.  */
      case 'I':
      case 'M':
      case 'N':
	break;

      default:
	warning (0, "unrecognized gcc debugging option: %c", c);
	break;
      }
}

/* Language-dependent initialization.  Returns nonzero on success.  */
static int
lang_dependent_init (const char *name)
{
  /* Other front-end initialization.  */
  if ((*lang_hooks.init) () == 0)
    return 0;

  return 1;
}

/* Clean up: close opened files, etc.  */

static void
finalize (void)
{
  /* Language-specific end of compilation actions.  */
  (*lang_hooks.finish) ();
}

/* Initialize the compiler, and compile the input file.  */
static void
do_compile (void)
{
  process_options ();

  /* Don't do any more if an error has already occurred.  */
  if (!errorcount)
    {
      /* Language-dependent initialization.  Returns true on success.  */
      lang_dependent_init (main_input_filename);

      finalize ();
    }
}

/* Entry point of cc1, cc1plus, jc1, f771, etc.
   Exit code is FATAL_EXIT_CODE if can't open files or if there were
   any errors, or SUCCESS_EXIT_CODE if compilation succeeded.

   It is not safe to call this function more than once.  */

int
main (unsigned int argc, const char **argv)
{
  /* Initialization of GCC's environment, and diagnostics.  */
  general_init (argv[0]);

  /* Parse the options and do minimal processing; basically just
     enough to default flags appropriately.  */
  decode_options (argc, argv);

  /* Exit early if we can (e.g. -help).  */
  if (!exit_after_options)
    do_compile ();

  if (errorcount || sorrycount)
    return (FATAL_EXIT_CODE);

  return (SUCCESS_EXIT_CODE);
}
