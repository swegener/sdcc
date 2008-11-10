#!/bin/sh

#
# Script to generate Makefile.am,
# execute in device/lib/pic16/libio/ with no arguments.
#
# Written by Raphael Neider <tecodev AT users sourceforge net>
#
# Released under the terms of the GPL v2.
#

cat <<HEREDOC
## Makefile.am -- Process this file with automake to produce Makefile.in
## This file was automatically generated using $0.

lib_LIBRARIES = 

HEREDOC

sed -e 's/\s*#.*$//' ../pics.all | grep -v "^\s*$" | sort | while read arch; do
    echo "Generating for device >>${arch}<<" >&2;
    cat <<HERE
lib_LIBRARIES += libio18f${arch}.a
libio18f${arch}_a_SOURCES = dummy.c
HERE
    sed -e 's/\s*\(#.*\)\{0,1\}$//' adc.ignore | grep -x $arch > /dev/null 2>&1 && { echo "No adc." >&2 ; } || cat <<HERE
libio18f${arch}_a_SOURCES += adc/adcbusy.c adc/adcclose.c adc/adcconv.c
libio18f${arch}_a_SOURCES += adc/adcopen.c adc/adcread.c adc/adcsetch.c
HERE
    sed -e 's/\s*\(#.*\)\{0,1\}$//' i2c.ignore | grep -x $arch > /dev/null 2>&1 && { echo "No i2c." >&2 ; } || cat <<HERE
libio18f${arch}_a_SOURCES += i2c/i2cack.c i2c/i2cclose.c i2c/i2cdrdy.c
libio18f${arch}_a_SOURCES += i2c/i2cidle.c i2c/i2cnack.c i2c/i2copen.c
libio18f${arch}_a_SOURCES += i2c/i2creadc.c i2c/i2creads.c i2c/i2crestart.c
libio18f${arch}_a_SOURCES += i2c/i2cstart.c i2c/i2cstop.c i2c/i2cwritec.c
libio18f${arch}_a_SOURCES += i2c/i2cwrites.c
HERE
    sed -e 's/\s*\(#.*\)\{0,1\}$//' usart.ignore | grep -x $arch > /dev/null 2>&1 && { echo "No usart." >&2 ; } || cat <<HERE
libio18f${arch}_a_SOURCES += usart/ubaud.c usart/ubusy.c usart/uclose.c
libio18f${arch}_a_SOURCES += usart/udrdy.c usart/ugetc.c usart/ugets.c
libio18f${arch}_a_SOURCES += usart/uopen.c usart/uputc.c usart/uputs.c
libio18f${arch}_a_SOURCES += usart/usartd.c
HERE
    cat <<HERE
libio18f${arch}_a_CFLAGS = -p18f${arch} \$(AM_CFLAGS)

HERE
done

cat <<HEREDOC
include \$(top_srcdir)/Makefile.common

HEREDOC

