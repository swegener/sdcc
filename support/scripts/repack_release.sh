#! /bin/bash

#
# repack_release.sh
#
# This script repacks sdcc Linux and Mac OS X snapshot build binary and doc packages
# into a sdcc release package.
#
# Repacking Linux package example:
# ./repack_release.sh sdcc-snapshot-i386-unknown-linux2.5-20090314-5413.tar.bz2 sdcc-doc-20090314-5413.tar.bz2 2.9.0
#
# Repacking MacOS X package example:
# ./repack_release.sh sdcc-snapshot-universal-apple-macosx-20090314-5413.tar.bz2 sdcc-doc-20090314-5413.tar.bz2 2.9.0
#


function fatal_error()
{
  echo "repack_release: $1" 1>&2
  exit 1;
}


function usage()
{
  echo "Usage: repack_release.sh <bin_package> <doc_package> <version>" 1>&2
  exit 1;
}


{
  if [ $# != 3 ]
  then
    usage
  fi

  bin_pkg=$1
  doc_pkg=$2
  ver=$3

  arch=$(expr $(basename $bin_pkg) : 'sdcc-snapshot-\([^-]*-[^-]*-[^-]*\)-.*\.tar\.bz2')
  if [ -z "$arch" ]
  then
    fatal_error "$bin_pkg is not a sdcc binary package!"
  fi

  if [ -d sdcc ]
  then
    fatal_error "Directory sdcc already exists!"
  fi

  tar -xjvf $bin_pkg || fatal_error "Can't unpack $bin_pkg!"
  
  # remove unneeded directories produced by sdbinutils
  rm -rf ./sdcc/include
  rm -rf ./sdcc/lib

  rm -rf ./sdcc/share/doc
  rm -rf ./sdcc/share/sdcc/doc

  tar -xjvf $doc_pkg -C ./sdcc/share/sdcc || fatal_error "Can't unpack $doc_pkg!"
  cp ./sdcc/share/sdcc/doc/INSTALL.txt ./sdcc
  cp ./sdcc/share/sdcc/doc/README.txt ./sdcc
  tar -cjvf sdcc-$ver-$arch.tar.bz2 sdcc || fatal_error "Can't pack sdcc-$ver-$arch.tar.bz2!"
  mv sdcc $arch

  exit 0
}
