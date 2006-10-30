dnl aclocal.m4 generated automatically by aclocal 1.4-p6

dnl Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl This macro will check for the presence of the readline library.
dnl To get it into the aclocal.m4 dnl file, do this:
dnl   aclocal -I . --verbose
dnl
dnl The --verbose will show all of the files that are searched
dnl for .m4 macros.

AC_DEFUN([wi_LIB_READLINE], [
  dnl check for the readline.h header file
  AC_CHECK_HEADER(readline/readline.h)

  if test "$ac_cv_header_readline_readline_h" = yes; then
    dnl check for the readline library

    ac_save_LIBS="$LIBS"
    # Note: $LIBCURSES is permitted to be empty.

    for LIBREADLINE in "-lreadline.dll" "-lreadline" "-lreadline $LIBCURSES" "-lreadline -ltermcap" "-lreadline -lncurses" "-lreadline -lcurses"
    do
      AC_MSG_CHECKING([for GNU Readline library])

      LIBS="$ac_save_LIBS $LIBREADLINE"

      AC_TRY_LINK([
        /* includes */
        #include <stdio.h>
        #include <readline/readline.h>
      ],[
        /* function-body */
        readline(NULL);
        rl_function_of_keyseq(NULL, NULL, NULL);
      ],[
        wi_cv_lib_readline=yes
        wi_cv_lib_readline_result="$LIBREADLINE"
      ],[
        wi_cv_lib_readline=no
        wi_cv_lib_readline_result=no
      ])

      if test "$wi_cv_lib_readline" = yes; then
        AC_MSG_RESULT($wi_cv_lib_readline_result)
        AC_SUBST(LIBREADLINE)
        AC_DEFINE_UNQUOTED(HAVE_LIBREADLINE, 1, [Readline])
        break
      fi
    done

    LIBS="$ac_save_LIBS"
  fi
])

