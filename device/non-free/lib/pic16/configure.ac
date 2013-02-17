#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([pic16lib], [0.3], [tecodev AT users sourceforge net])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([libdev/pic18f13k50.c])
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

AC_SUBST([USE_FLOATS], ["-DUSE_FLOATS=0"])
AC_ARG_ENABLE(
    [floats],
    AS_HELP_STRING([--enable-floats], [Enable output of float-values via printf().]),
    [if test "yes" = $enableval; then USE_FLOATS="-DUSE_FLOATS=1"; fi]
)

NEWPICS=no;
AC_ARG_ENABLE(
    [new-pics],
    AS_HELP_STRING([--enable-new-pics], [Enable use of PIC devices added to gputils after the 0.13.7 release. Requires custom-built gputils obtained from their SVN repository!]),
    [ case "x$enableval" in
      xyes) NEWPICS=yes;
            AC_MSG_NOTICE([Enabling support for PICs from unreleased gputils -- make sure you have up-to-date gputils from SVN.]);;
      xno) NEWPICS=no;;
      *) AC_MSG_ERROR([bad value "${enableval}" for --enable-new-pics]);;
      esac],
    [ NEWPICS=no ]
)

AM_CONDITIONAL([HAVE_NEW_PICS], [ test x$NEWPICS = xyes ])

# Checks for programs.

# The default architecture can be selected at configure time by setting the
# environment variable ARCH to the desired device (18fxxx).
AC_SUBST(ARCH, [${ARCH:-18f452}])

# We cannot use AC_PROG_CC(sdcc) as sdcc might not be built at configure-time...
AC_SUBST(CC, [\'$abs_top_builddir/../../../../bin/sdcc\'])
AC_PATH_PROG(CCAS, gpasm, :)
AC_PATH_PROG(LD, gplink, :)
AC_PATH_PROG(AR, gplib, :)

case ":$CCAS:$LD:$AR:" in
    *:::*)
        AC_MSG_ERROR([gputils (gpasm, gplink, and gplib) are required but not found.
Either install gputils or reconfigure with --disable-pic14-port and --disable-pic16-port.])
        ;;
esac;

mCCAS=$CCAS;
AC_SUBST(CFLAGS, ["-mpic16 -p$ARCH"])
AC_SUBST(CCASFLAGS, ["-p$ARCH"])
AC_SUBST(ARFLAGS, [-c])
AC_SUBST(CCAS, [\'$CCAS\'])
AC_SUBST(LD, [\'$LD\'])
AC_SUBST(AR, [\'$AR\'])

# $RANLIB is called by the autotools but not provided nor required
AC_SUBST(RANLIB, [:])
AC_SUBST(OBJEXT, [o])

_AM_DEPENDENCIES(CC)
_AM_DEPENDENCIES(CCAS)

AC_MSG_CHECKING([devices supported by gputils])
good_pics="";
bad_pics="";
CHECK=".checkdevices/check.asm";
mkdir -p ".checkdevices";
echo "  END" > "$CHECK";
for i in "${srcdir}/libdev/pic1"*.c; do
  p="${i##*pic}";
  p="${p%.c}";
  if $mCCAS -p "$p" "$CHECK" >/dev/null 2>&1; then
    good_pics="$good_pics $p";
  else
    bad_pics="$bad_pics $p";
  fi;
done;
AC_MSG_RESULT([[$good_pics]])
if test -n "$bad_pics"; then
  AC_MSG_WARN([[
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!
!!! WARNING: The installed gputils do not support all
!!! PIC devices currently supported by SDCC.
!!! If you continue to build SDCC, library files for the
!!! following devices will not be built, and you will
!!! not be able to compile any projects for these devices:
!!!

$bad_pics

!!!
!!! Please update your gputils to a recent snapshot and
!!! run configure again using the updated gputils. Make
!!! sure to have them in PATH prior to the previously
!!! found ones (or remove the older version completely).
!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

]])
  for d in 5 4 3 2 1; do
    echo "Continuing in $d seconds ...";
    sleep 1;
  done;
fi;

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 libdev/Makefile])
AC_OUTPUT
