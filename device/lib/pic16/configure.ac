#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#
# The (generated) configure script should be called with
#     --build=<your-build-system> --target=pic16
# to force cross-compilation mode; rather use the wrapper
# ./configure.gnu instead of ./configure directly.
#

AC_PREREQ(2.61)
AC_INIT([pic16lib], [0.3], [tecodev AT users sourceforge net])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([debug/gstack/gstack.c])
AC_CONFIG_HEADER([config.h])


# SDCC setup
case $srcdir in
    [\\/]*|?:[\\/]*) abs_srcdir="$srcdir";
        ;;
    *) abs_srcdir="$ac_pwd/$srcdir";
        ;;
esac
case $ac_top_build_prefix in
    [\\/]*|?:[\\/]*)
        abs_top_builddir="$ac_top_build_prefix";
        ;;
    *) abs_top_builddir="$ac_pwd/$ac_top_build_prefix";
        ;;
esac

libdir=$libdir/pic16

# Checks for programs.

# The default architecture can be selected at configure time by setting the
# environment variable ARCH to the desired device (18fxxx).
AC_SUBST(ARCH, [${ARCH:-18f452}])

# We cannot use AC_PROG_CC(sdcc) as sdcc might not be built at configure-time...
AC_SUBST(CC, [\'$abs_top_builddir/../../../bin/sdcc\'])
AC_SUBST(CFLAGS, ["-mpic16 -p$ARCH"])
AC_PATH_PROG(CCAS, gpasm, :)
AC_SUBST(CCAS, [\'$CCAS\'])
AC_SUBST(CCASFLAGS, ["-p$ARCH"])
AC_PATH_PROG(LD, gplink, :)
AC_SUBST(LD, [\'$LD\'])
AC_PATH_PROG(AR, gplib, :)
AC_SUBST(AR, [\'$AR\'])
AC_SUBST(ARFLAGS, [-c])

# $RANLIB is called by the autotools but not provided nor required
AC_SUBST(RANLIB, [:])
AC_SUBST(OBJEXT, [o])

_AM_DEPENDENCIES(CC)
_AM_DEPENDENCIES(CCAS)

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

case "$CCAS:$LD:$AR" in
    *::*)
        AC_MSG_ERROR([gputils (gpasm, gplink, and gplib) are required but not found.])
        ;;
esac

AC_CONFIG_FILES([Makefile
                 debug/Makefile
                 libc/Makefile
                 libdev/Makefile
                 libio/Makefile
                 libm/Makefile
                 libsdcc/Makefile
                 startup/Makefile])
AC_OUTPUT

