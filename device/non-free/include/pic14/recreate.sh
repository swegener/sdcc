#!/bin/sh

# This script can be used to recreate the device library files from
# gputils' .inc files.
# Usage:
#     mkdir temp && cd temp && ../recreate.sh
#
# You will need to adjust the paths to SDCC and gputils before running!

#GPUTILS=$HOME/svn/gputils
#SDCC=$HOME/svn/plain

GPUTILS=$HOME/svn_snapshots/gputils/gputils
SDCC=$HOME/svn_snapshots/sdcc/sdcc

is_in()
{
  local f l j
  f=$1
  shift
  l=$*

  for j in $l; do
    if [ $f = $j ]
    then
      return 0
    fi
  done

  return 1
}

NO_LEGACY_NAMES=

for i in ../pic*.h
do
  if ! is_in $i ../pic14regs.h ../pic16fam.h ]
  then
    test -e $i && grep -q NO_LEGACY_NAMES $i && NO_LEGACY_NAMES="$NO_LEGACY_NAMES $i"
  fi
done

#for i in ../pic*.h; do
for i in ../pic16f946.h \
../pic16f747.h \
../pic16f1508.h \
../pic16f1823.h \
../pic16c781.h \
../pic12f675.h \
../pic16c926.h \
../pic12f629.h \
../pic16f684.h \
../pic16f688.h \
../pic16f1509.h \
../pic16f630.h \
../pic16c782.h \
../pic16f676.h \
../pic16c770.h \
../pic16f1782.h \
../pic16f716.h \
../pic16c771.h \
../pic16f1503.h \
../pic16f1507.h \
../pic16f1783.h \
../pic16c925.h \
../pic16c432.h \
../pic16c717.h
do
  if ! is_in $i ../pic14regs.h ../pic16fam.h ]
  then
    if is_in $i $NO_LEGACY_NAMES
    then
      emit_legacy_names=1
    else
      emit_legacy_names=
    fi
    DEV=`echo "$i" | sed -e "s:../pic::;s/\.h//"`;
    echo "Creating ${DEV} ${emit_legacy_names}...";
    "${SDCC}/support/scripts/inc2h.pl" "${DEV}" "${GPUTILS}" "${emit_legacy_names}";
  fi
done
