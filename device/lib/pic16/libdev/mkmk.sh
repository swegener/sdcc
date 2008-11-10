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

sed -e 's/\s*#.*$//' ../pics.all | grep -v '^\s*$' | sort | while read arch; do
    cat <<HERE
lib_LIBRARIES += libdev18f${arch}.a
libdev18f${arch}_a_SOURCES = pic18f${arch}.c
libdev18f${arch}_a_CFLAGS = -p18f${arch} \$(AM_CFLAGS)

HERE
done

cat <<HERE
include \$(top_srcdir)/Makefile.common

HERE

