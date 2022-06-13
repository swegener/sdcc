#!/bin/sh

if [ -x /bin/test ]; then
	TEST=/bin/test
elif [ -x /usr/bin/test ]; then
	TEST=/usr/bin/test
else
	TEST=test
fi

echo "Make distclean of main directory..."
make -f clean.mk distclean

for pkg in cmd.src sim.src gui.src motorola.src \
	   s51.src avr.src z80.src tlcs.src xa.src \
	   stm8.src st7.src pdk.src p1516.src \
	   m68hc08.src m6809.src m6800.src m68hc11.src m68hc12.src \
	   mos6502.src rxk.src \
	   pblaze.src \
	   doc
do
    echo "Make distclean of package ${pkg} directory..."
    make -C $pkg -f clean.mk top_srcdir=$(pwd) distclean
    $TEST -L ${pkg}/dtest && rm -f ${pkg}/dtest
done

echo "Make clean of example directory..."
make -C example clean
echo "Make clean of test directory..."
make -C test clean

echo "Cleanup compiled files..."
find . -name '*.d' -exec rm {} \;
find . -name '*.lk' -exec rm {} \;
find . -name '*.lst' -exec rm {} \;
find . -name '*.map' -exec rm {} \;
find . -name '*.noi' -exec rm {} \;
find . -name '*.rel' -exec rm {} \;
find . -name '*.sym' -exec rm {} \;
find . -name '*.cdb' -exec rm {} \;
find . -name '*.rst' -exec rm {} \;
