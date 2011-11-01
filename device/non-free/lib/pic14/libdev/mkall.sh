#!/bin/sh

# This script is supposed to recreate all device libraries and their
# accompanying header files from the gputils' header/*.inc files
# using sdcc/support/scripts/inc2h.pl.
#
# The devices to be created are listed in ./devices.txt, the output
# files will be located in ./build/, which is created before use.
#
# (c) 2007 by Raphael Neider <rneider @ web.de>
# This file is published under the terms of the GPLv2.

GPUTILS=$HOME/gputils.svn
SDCC=$HOME/sdcc/sdcc.svn

HEADERS=$SDCC/device/non-free/include/pic14

if [ ! -d "$GPUTILS" ]; then
    echo "gputils not found in '$GPUTILS' -- exiting.";
    exit 1;
fi;

if [ ! -d "$SDCC" ]; then
    echo "sdcc not found in '$SDCC' -- exiting.";
    exit 1;
fi;

mkdir build;
cd build;
cp ../pic14ports.txt .

if true; then
    sed -e 's/\s*#.*$//' ../devices.txt | grep -v '^\s*$' | while read PROC; do
        echo $PROC;
        $SDCC/support/scripts/inc2h.pl $PROC $GPUTILS;
    done;
fi;

for i in *.c; do
    if [ -f "$i" ]; then
        h="${i%.c}.h";
        echo "";
        echo "### $i";
        diff -up "../$i" "$i";
        diff -up "$HEADERS/$h" "$h";
        ok=no;
        while [ ! xyes = "x$ok" ]; do
            echo "Replace? [y/n]";
            read ans;
            case "$ans" in
                y|Y)
                    echo "Replacing ...";
                    mv -f "$i" ..;
                    mv -f "$h" "$HEADERS/$h";
                    ok=yes;
                    ;;
                n|N)
                    echo "Keeping ...";
                    rm -f "$i" "$h";
                    ok=yes;
                    ;;
                *)
                    ok=no;
                    ;;
            esac;
        done;
    else
        echo "NEW: $i";
        mv -f "$i" ..;
        mv -f "$h" "$HEADERS/$h";
    fi;
done;

cd ..;
