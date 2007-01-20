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

GPUTILS=$HOME/svn/gputils
SDCC=$HOME/svn/sdcc

mkdir build;
cd build;
cp $SDCC/device/lib/pic/libdev/pic14ports.txt .

cat $SDCC/device/lib/pic/libdev/devices.txt | while read PROC; do
    echo $PROC;
    $SDCC/support/scripts/inc2h.pl $PROC $GPUTILS;
done

cd ..;
mv build/*.c .
mv build/*.h $SDCC/device/include/pic/
