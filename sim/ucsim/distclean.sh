#!/bin/sh

make -f clean.mk distclean
for pkg in cmd.src sim.src gui.src motorola.src \
	   s51.src avr.src z80.src tlcs.src xa.src \
	   hc08.src stm8.src st7.src pdk.src p1516.src \
	   m6809.src m6800.src mcs6502.src rxk.src \
	   pblaze.src \
	   doc
do
    make -C $pkg -f clean.mk distclean
    test -L ${pkg}/dtest && rm -f ${pkg}/dtest
done
make -C example clean
make -C test clean

for d in $(ls ../test); do
    make -C ../test/$d clean
done
