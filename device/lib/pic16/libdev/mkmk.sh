#!/bin/sh

#
# Script to generate Makefile.am,
# execute in device/lib/pic16/libdev/ with no arguments.
#
# Written by Raphael Neider <tecodev AT users sourceforge net>
#
# Released under the terms of the GPL v2.
#

cat <<HERE
## Makefile.am -- Process this file with automake to produce Makefile.in
## This file has been automatically generated using $0.

lib_LIBRARIES =

HERE

ls -1 *.c | sort | while read file; do
    arch=${file%.c};
    arch=${arch#pic};
    cat <<HERE
lib_LIBRARIES += libdev${arch}.a
libdev${arch}_a_SOURCES = ${file}
libdev${arch}_a_CFLAGS = -p${arch} \$(AM_CFLAGS)

HERE
done

cat <<HERE
include \$(top_srcdir)/Makefile.common

HERE

