# Makefile to get and build all the parts of GBDK

CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	else if [ -x /bin/bash ]; then echo /bin/bash; \
	else echo sh; fi ; fi)

TOPDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

BUILD = $(TOPDIR)/build/sdcc
SDCCLIB = $(BUILD)
CVSFLAGS = -z5
CVS = cvs
DIR = .
VER = 2.94
# Used as a branch name.
SHORTVER = 294

ROOT_GBDK = :pserver:anonymous@cvs.gbdk.sourceforge.net:/cvsroot/gbdk
ROOT_SDCC = :pserver:anonymous@cvs.sdcc.sourceforge.net:/cvsroot/sdcc

# For mingw32 hosted on Linux
# Source extension - what the gcc generated files have appended
#SE =
# Dest extenstion - what extension we want them to have.
#E = .exe
#SDCC_ROOT = \\\\gbdk

# For mingw32 on win32
# Source extension - what the gcc generated files have appended
#SE = .exe
# Dest extenstion - what extension we want them to have.
#E = .exe
#SDCC_ROOT = \\\\gbdk

# For Linux
SE = 
E =
SDCC_ROOT = /usr/lib/sdcc

#all: logged_in dist
all: dist

clean:
	for i in sdcc gbdk-lib gbdk-support/lcc; do make -C $$i clean; done
	rm -f *~
	rm -rf $(BUILD) gbdk-lib gbdk-support sdcc logged_in

update: logged_in
	cd $(DIR); cvs $(CVSFLAGS) -d$(ROOT_SDCC) co -r gbdk-$(SHORTVER) sdcc
	cd $(DIR); cvs $(CVSFLAGS) -d$(ROOT_GBDK) co -r gbdk-$(SHORTVER) gbdk-lib
	cd $(DIR); cvs $(CVSFLAGS) -d$(ROOT_GBDK) co -r gbdk-$(SHORTVER) gbdk-support

_sdcc: sdcc-bin sdcc-lib sdcc-doc

tidy:
	rm -rf `find $(BUILD) -name "CVS"`
	rm -rf `find $(BUILD)/lib -name "*.asm"`
	-strip $(BUILD)/bin/*

sdcc-bin: sdcc/sdccconf.h
	make -C sdcc sdcc-bin
	mkdir -p $(BUILD)/bin
	for i in \
	sdcc sdcpp link-gbz80 as-gbz80 aslink asx8051 sdcdb; \
	do cp sdcc/bin/$$i$(SE) $(BUILD)/bin/$$i$(E); done
	cp sdcc/sim/ucsim/s51.src/s51$(E) $(BUILD)/bin
	cp sdcc/sim/ucsim/z80.src/sz80$(E) $(BUILD)/bin
	cp sdcc/sim/ucsim/avr.src/savr$(E) $(BUILD)/bin
	cp sdcc/debugger/mcs51/*.el $(BUILD)/bin

sdcc-doc:
	(cd sdcc; tar cf - doc) | (cd $(BUILD); tar xf -)
	cp sdcc/README sdcc/COPYING $(BUILD)
	mkdir -p $(BUILD)/sim
	for i in COPYING INSTALL README TODO; \
	do cp sdcc/sim/ucsim/$$i $(BUILD)/sim; done
	(cd sdcc/sim/ucsim; tar cf - doc) | (cd $(BUILD)/sim; tar xf -)

sdcc-lib: sdcc-lib-z80 sdcc-lib-gen
	mkdir -p $(BUILD)/lib
	(cd sdcc/device/lib; tar cf - small large) | (cd $(BUILD)/lib; tar xf -)
	(cd sdcc/device; tar cf - examples include) | (cd $(BUILD); tar xf -)

sdcc-lib-z80:
	make -C gbdk-lib/libc SDCCLIB=$(BUILD) PORTS=z80 PLATFORMS=consolez80
	(cd gbdk-lib/build; tar cf - consolez80 z80) | (cd $(BUILD)/lib; tar xf -)

sdcc-lib-gen:
	make -C sdcc sdcc-device

sdcc/sdccconf.h: sdcc/configure
	cd sdcc; \
	./configure --datadir=$(SDCC_ROOT)

_gbdk-lib: _sdcc-min _gbdk-support
	cp -r gbdk-lib/include $(BUILD)
	make -C gbdk-lib SDCCLIB=$(SDCCLIB) PORTS=gbz80 PLATFORMS=gb

_gbdk-support:
	make -C gbdk-support/lcc clean
	make -C gbdk-support/lcc SDCCLIB=$(SDCC_ROOT)/
	mkdir -p $(BUILD)/bin
	cp gbdk-support/lcc/lcc$(SE) $(BUILD)/bin/lcc$(E)

dist: _sdcc-min _gbdk-lib _gbdk-support
	mkdir -p $(BUILD)/bin
	mkdir -p $(BUILD)/lib
	cp -r gbdk-lib/build/gbz80 $(BUILD)/lib
	cp -r gbdk-lib/build/gb $(BUILD)/lib
	make -C gbdk-lib/libc clean
	make -C gbdk-lib/examples/gb make.bat
	cp -r gbdk-lib/examples $(BUILD)
	cp -r gbdk-lib/libc $(BUILD)
	cp -r sdcc/doc $(BUILD)
	cp gbdk-support/README $(BUILD)
	strip $(BUILD)/bin/*
	rm -rf `find gbdk -name "CVS"`

zdist: dist
	tar czf gbdk-$(VER).tar.gz gbdk

logged_in:
	cvs -d$(ROOT_GBDK) login
	cvs -d$(ROOT_SDCC) login
	touch logged_in
	make -f update.mak update
