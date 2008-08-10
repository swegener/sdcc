#!/bin/sh

#
# Wrapper for the configure-script to force cross-compilation mode.
#

SRCDIR=`dirname "$0"`
"$SRCDIR/configure" "$@" --build=`"$SRCDIR/config.guess"` --host=pic16

