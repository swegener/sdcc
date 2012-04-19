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
    if grep '^'"$arch"'$' newdevices.txt > /dev/null 2>&1; then
      cat <<HERE
if HAVE_NEW_PICS
lib_LIBRARIES += libdev18f${arch}.a
endif HAVE_NEW_PICS
HERE
    else
        echo "lib_LIBRARIES += libdev18f${arch}.a";
    fi;

    echo "libdev18f${arch}_a_SOURCES = pic18f${arch}.c";

    sdcc -mpic16 -p18f${arch} --use-non-free -o pic18f${arch}.inc -E pic18f${arch}.c;
    if grep '\bEEADRH\b' pic18f${arch}.inc >/dev/null 2>&1; then
        # 16 bit EEPROM address space
        for i in get put; do
            for j in 1 2 3 4; do
                echo "libdev18f${arch}_a_SOURCES += gptr/eeprom16_gptr${i}${j}_dispatch.S";
            done;
        done;
    elif grep '\bEEADR\b' pic18f${arch}.inc >/dev/null 2>&1; then
        # 8 bit EEPROM address space
        for i in get put; do
            for j in 1 2 3 4; do
                echo "libdev18f${arch}_a_SOURCES += gptr/eeprom8_gptr${i}${j}_dispatch.S";
            done;
        done;
    else
        # No EEPROM
        echo "libdev18f${arch}_a_SOURCES += gptr/dispatch.S";
    fi;

    if ! grep '\bSSPBUF\b' pic18f${arch}.inc >/dev/null 2>&1; then
        echo "libdev18f${arch}_a_SOURCES += fake_sspbuf.c";
    fi;
    rm pic18f${arch}.inc;

    echo "libdev18f${arch}_a_CFLAGS = -p18f${arch} \$(AM_CFLAGS)";
    echo "";
done;

cat <<HERE
include \$(top_srcdir)/Makefile.common

HERE

