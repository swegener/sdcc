#! /bin/bash

#
# repack_release.sh
#
# This script repacks sdcc Linux and Mac OS X snapshot build binary and doc
# packages into a sdcc release package.
#
# Example:
# ./repack_release.sh 20090314 5413 2.9.0-rc1
#


function fatal_error()
{
  echo "repack_release: $1" 1>&2
  exit 1;
}


function usage()
{
  echo "Usage: repack_release.sh <date> <revision> <version>" 1>&2
  exit 1;
}


function download()
{
  local date=$1 revision=$2

  mkdir -p dl

  if ! pushd dl
  then
    fatal_error "Can't cd to dl!"
  else
    wget http://sourceforge.net/projects/sdcc/files/snapshot_builds/sdcc-src/sdcc-src-${date}-${revision}.tar.bz2

    wget http://sourceforge.net/projects/sdcc/files/snapshot_builds/docs/sdcc-doc-${date}-${revision}.tar.bz2

    wget http://sourceforge.net/projects/sdcc/files/snapshot_builds/docs/sdcc-doc-${date}-${revision}.zip

    wget http://sourceforge.net/projects/sdcc/files/snapshot_builds/i386-unknown-linux2.5/sdcc-snapshot-i386-unknown-linux2.5-${date}-${revision}.tar.bz2

    wget http://sourceforge.net/projects/sdcc/files/snapshot_builds/i586-mingw32msvc/sdcc-snapshot-i586-mingw32msvc-${date}-${revision}.zip

    wget http://sourceforge.net/projects/sdcc/files/snapshot_builds/i586-mingw32msvc-setup/sdcc-${date}-${revision}-setup.exe

    wget http://sourceforge.net/projects/sdcc/files/snapshot_builds/i386_universal-apple-macosx/sdcc-snapshot-i386_universal-apple-macosx-${date}-${revision}.tar.bz2

    mv sdcc-snapshot-i386_universal-apple-macosx-${date}-${revision}.tar.bz2 sdcc-snapshot-universal-apple-macosx-${date}-${revision}.tar.bz2

    popd
  fi
}


function unpack()
{
  local bin_pkg=$1 doc_pkg=$2

  if [ -z "$(expr $(basename $bin_pkg) : 'sdcc-snapshot-\([^-]*-[^-]*-[^-]*\)-.*\.tar\.bz2')" ]
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
}


function pack()
{
  local arch=$1 ver=$2

  cp ./sdcc/share/sdcc/doc/INSTALL.txt ./sdcc
  cp ./sdcc/share/sdcc/doc/README.txt ./sdcc

  mkdir -p ul

  tar -cjvf ul/sdcc-${ver}-${arch}.tar.bz2 sdcc || fatal_error "Can't pack ul/sdcc-${ver}-${arch}.tar.bz2!"
  mv sdcc ${arch}
}


function repack_win()
{
  local date=$1 revision=$2 ver=$3

  snapshot=~/svn_snapshots/sdcc/sdcc
  ver_maj=$(expr $ver : '\([0-9]*\)\.')
  ver_min=$(expr $ver : '[0-9]*\.\([0-9]*\)\.')
  ver_rev=$(expr $ver : '[0-9]*\.[0-9]*\.\([0-9]*\)')

  # - unpack WIN32 mingw daily snapshot sdcc-snapshot-i586-mingw32msvc-yyyymmdd-rrrr.zip
  #   to a clean directory (the option to create directories should be enabled).
  #   A sub directory sdcc is created (referenced as PKGDIR in continuation).
  unzip dl/sdcc-snapshot-i586-mingw32msvc-${date}-${revision}.zip

  if ! pushd sdcc
  then
    fatal_error "Can't cd to sdcc!"
  else
    # - remove the PKGDIR/doc/ directory
    rm -rf doc/

    # - unpack sdcc-doc-yyyymmdd-rrrr.zip to the PKGDIR/doc directory
    unzip ../dl/sdcc-doc-${date}-${revision}.zip

    # - copy files sdcc/support/scripts/sdcc.ico and sdcc/support/scripts/sdcc.nsi
    #   (this file) from the sdcc Subversion snapshot to the PKGDIR directory
    cp ${snapshot}/support/scripts/sdcc.nsi ${snapshot}/support/scripts/sdcc.ico .

    # - copy file COPYING and COPYING3 from the sdcc Subversion snapshot to the PKGDIR directory,
    #   rename it to COPYING.txt and COPYING3.txt and convert it to DOS format:
    cp ${snapshot}/COPYING COPYING.txt
    todos COPYING.txt
    cp ${snapshot}/sdas/COPYING3 COPYING3.txt
    todos COPYING3.txt
    cp ${snapshot}/ChangeLog doc/ChangeLog.txt
    todos doc/ChangeLog.txt
    cp ${snapshot}/README doc/README.txt
    todos doc/README.txt

    # - run NSIS installer from PKGDIR directory:
    #   Define -DWIN64 if createing a 64bit package.
    makensis -DFULL_DOC -DVER_MAJOR=${ver_maj} -DVER_MINOR=${ver_min} -DVER_REVISION=${ver_rev} -DVER_BUILD=${revision} sdcc.nsi

    # - A setup file setup.exe is created in PKGDIR directory.
    #   Rename it to sdcc-x.x.x-setup.exe and upload it
    #   to sdcc download repository at sourceforge.net
    cp setup.exe ../ul/sdcc-${ver}-setup.exe

    popd
  fi
}


function upload()
{
  local ver=$1

  raw_ver=$(expr $ver : '\([0-9]*\.[0-9]*\.[0-9]*\)')

  echo uploading ul/sdcc-src-${ver}.tar.bz2  sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc/${raw_ver}
  rsync -e ssh ul/sdcc-src-${ver}.tar.bz2  sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc/${raw_ver}

  echo uploading ul/sdcc-doc-${ver}.tar.bz2  sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-doc/${raw_ver}
  rsync -e ssh ul/sdcc-doc-${ver}.tar.bz2  sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-doc/${raw_ver}

  echo uploading ul/sdcc-doc-${ver}.zip  sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-doc/${raw_ver}
  rsync -e ssh ul/sdcc-doc-${ver}.zip  sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-doc/${raw_ver}

  echo uploading ul/sdcc-${ver}-setup.exe sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-win32/${raw_ver}
  rsync -e ssh ul/sdcc-${ver}-setup.exe sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-win32/${raw_ver}

  echo uploading ul/sdcc-${ver}-i386-unknown-linux2.5.tar.bz2 sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-linux-x86/${raw_ver}
  rsync -e ssh ul/sdcc-${ver}-i386-unknown-linux2.5.tar.bz2 sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-linux-x86/${raw_ver}

  echo uploading ul/sdcc-${ver}-universal-apple-macosx.tar.bz2 sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-macosx/${raw_ver}
  rsync -e ssh ul/sdcc-${ver}-universal-apple-macosx.tar.bz2 sdcc-builder@web.sourceforge.net:/home/pfs/project/sdcc/sdcc-macosx/${raw_ver}
}


{
  if [ $# != 3 ]
  then
    usage
  fi

  date=$1
  revision=$2
  ver=$3

  mkdir -p ul

  download $date $revision

  cp dl/sdcc-src-${date}-${revision}.tar.bz2 ul/sdcc-src-${ver}.tar.bz2
  cp dl/sdcc-doc-${date}-${revision}.tar.bz2 ul/sdcc-doc-${ver}.tar.bz2
  cp dl/sdcc-doc-${date}-${revision}.zip ul/sdcc-doc-${ver}.zip

  for arch in i386-unknown-linux2.5 universal-apple-macosx
  do
    unpack dl/sdcc-snapshot-${arch}-${date}-${revision}.tar.bz2 dl/sdcc-doc-${date}-${revision}.tar.bz2
    pack $arch $ver
  done

  repack_win $date $revision $ver

  upload $ver

  exit 0
}
