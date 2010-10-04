#!/bin/sh

# This script can be used to recreate the device library files from
# gputils' .inc files.
# Usage:
#     mkdir temp && cd temp && ../recreate.sh
#
# You will need to adjust the paths to SDCC and gputils before running!

GPUTILS=$HOME/svn/gputils
SDCC=$HOME/svn/plain

for i in ../pic*.h; do
    if [ ! "$i" = "../pic14regs.h" ]; then
	DEV=`echo "$i" | sed -e "s:../pic::;s/\.h//"`;
	echo "Creating ${DEV}...";
	"${SDCC}/support/scripts/inc2h.pl" "${DEV}" "${GPUTILS}";
    fi
done

