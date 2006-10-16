dnl Scott Dattalo
dnl
dnl  This is copied from the NcFTP distribution.
dnl  Author Mike Gleason mgleason@NcFTP.com
dnl
dnl This macro will check for the presence and version
dnl of the readline library. To get it into the aclocal.m4
dnl file, do this:
dnl   aclocal -I . --verbose
dnl
dnl The --verbose will show all of the files that are searched
dnl for .m4 macros.
AC_DEFUN([wi_LIB_READLINE], [
AC_MSG_CHECKING([for GNU Readline library, version 2.0 or newer])

wi_cv_lib_readline=no
wi_cv_lib_readline_result=no
ac_save_LIBS="$LIBS"
# Note: $LIBCURSES is permitted to be empty.
for LIBREADLINE in "-lreadline" "-lreadline $LIBCURSES" "-lreadline -ltermcap" "-lreadline -lncurses" "-lreadline -lcurses"
do
	LIBS="$ac_save_LIBS $LIBREADLINE"
	AC_TRY_RUN([
	/* program */
#include <stdio.h>
#include <stdlib.h>

main(int argc, char **argv)
{
	/* Note:  don't actually call readline, since it may block;
	 * We just want to see if it (dynamic) linked in okay.
	 */
	if (argc == 0)	/* never true */
		readline(0);
	exit(0);
}
],[
	# action if true
	wi_cv_lib_readline=yes
],[
	# action if false
	wi_cv_lib_readline=no
],[
	# action if cross compiling
	wi_cv_lib_readline=no
])

	if test "$wi_cv_lib_readline" = yes ; then break ; fi
done

# Now try it again, to be sure it is recent enough.
# rl_function_of_keyseq appeared in version 2.0
#
dnl AC_CHECK_FUNC(rl_function_of_keyseq, [wi_cv_lib_readline=yes],[
dnl 	wi_cv_lib_readline=no;wi_cv_lib_readline_result="no (it is present but too old to use)"
dnl ])
	AC_TRY_LINK([
		/* includes */
	],[
		/* function-body */
		readline(0);
		rl_function_of_keyseq(0);
	],[
		wi_cv_lib_readline=yes
	],[
		wi_cv_lib_readline=no
		wi_cv_lib_readline_result="no (it is present but too old to use)"
	])

if test "$wi_cv_lib_readline" = no ; then
	LIBREADLINE=""
	# restore LIBS
	LIBS="$ac_save_LIBS"
else
	/bin/rm -f readline.ver
	touch readline.ver

	AC_TRY_RUN([
	/* program */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

extern char *rl_library_version;

main()
{
	FILE *fp;
	double d;

	sscanf(rl_library_version, "%lf", &d);
	fp = fopen("readline.ver", "w");
	if (fp == NULL) exit(1);
	if (fprintf(fp, "%s\n", rl_library_version) < 0) exit(1);
	if (fprintf(fp, "%03d\n", (int) (d * 100.0)) < 0) exit(1);
	if (fclose(fp) < 0) exit(1);
	exit(0);
}
	],[
		# action if true
		rl_library_version=`sed -n 1,1p readline.ver 2>/dev/null`
		rlver=`sed -n 2,2p readline.ver 2>/dev/null`
		/bin/rm -f readline.ver
	],[
		# action if false
		rl_library_version=''
		rlver=''
		/bin/rm -f readline.ver
	],[
		# action if cross compiling
		rl_library_version=''
		rlver=''
		/bin/rm -f readline.ver
	])

	case "$rlver" in
		???)
			wi_cv_lib_readline_result="yes, installed version is $rl_library_version"
			;;
		*)
			# Test using current LIBS.
			AC_TRY_LINK([
				/* includes */
				extern int rl_completion_append_character;
			],[
				/* function-body */
				readline(0);
				rl_completion_append_character = 0;
			],[
				rlver="210"
			],[
				rlver="200"
			])

			if test "$rlver" = "210" ; then
				wi_cv_lib_readline_result="yes, version 2.1 or higher"
			else
				wi_cv_lib_readline_result="yes, version 2.0"
			fi
			;;
	esac

	wi_cv_lib_readline=yes
	# restore LIBS
	LIBS="$ac_save_LIBS"
fi
AC_MSG_RESULT($wi_cv_lib_readline_result)
AC_SUBST(LIBREADLINE)

if test "$wi_cv_lib_readline" = yes ; then
	# Now verify that all the headers are installed.
	#
	AC_REQUIRE_CPP()
	unset ac_cv_header_readline_chardefs_h
	unset ac_cv_header_readline_history_h
	unset ac_cv_header_readline_keymaps_h
	unset ac_cv_header_readline_readline_h
	unset ac_cv_header_readline_tilde_h
	AC_CHECK_HEADERS([readline/chardefs.h readline/history.h readline/keymaps.h readline/readline.h readline/tilde.h])

	for xxwi in \
		"$ac_cv_header_readline_chardefs_h" \
		"$ac_cv_header_readline_history_h" \
		"$ac_cv_header_readline_keymaps_h" \
		"$ac_cv_header_readline_readline_h" \
		"$ac_cv_header_readline_tilde_h"
	do
		if test "$xxwi" = no ; then
			break
		fi
	done

	if test "$xxwi" = no ; then
		AC_MSG_WARN([GNU Readline headers are not installed or could not be found -- GNU Readline will not be used.])
		wi_cv_lib_readline=no
		wi_cv_lib_readline_result="no (headers not installed)"
	else
		AC_DEFINE_UNQUOTED(HAVE_LIBREADLINE, $rlver,[Readline])
	fi
fi
	dnl Check to see if we have a namespace clean version of readline
	dnl or not. At least FreeBSD 4.9 ships with non-ns-clean readline.
	if test "$wi_cv_lib_readline" = "yes" ; then
		ac_save_LIBS="$LIBS"
		LIBS="$ac_save_LIBS $LIBREADLINE"
		AC_CHECK_FUNCS(rl_completion_matches completion_matches)
		# restore LIBS
		LIBS="$ac_save_LIBS"
	fi
	dnl Check to see if rl_callback_handler_install takes a cast
	AC_MSG_CHECKING([to see if readline callbacks take a cast])
	ac_save_LIBS="$LIBS"
	LIBS="$ac_save_LIBS $LIBREADLINE"
	ac_save_CC="$CC"
	CC="$CXX"
	AC_TRY_LINK([
#include <stdio.h>
#include <readline/readline.h>
void func(void) { return; }
	],[
		rl_callback_handler_install(">", (void(*)(char*))func);
	],[
		AC_DEFINE(READLINE_CB_TAKES_CAST, 1, [callbacks take a cast])
		AC_MSG_RESULT([yes])
	],[
		AC_MSG_RESULT([no])
	])
	# restore LIBS
	LIBS="$ac_save_LIBS"
	CC="$ac_save_CC"
])
dnl
dnl
