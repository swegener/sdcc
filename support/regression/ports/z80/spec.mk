# Port specification for the z80 port running ontop of the Java based
# 'ConsoleZ80' emulator.

# PENDING: Path to gbdk-lib for stdarg
GBDK_LIB = ../../../gbdk-lib
SDCCFLAGS += -I$(GBDK_LIB)/include

EXEEXT = .bin

# Needs parts of gbdk-lib, namely the internal mul/div/mod functions.
EXTRAS = fwk/lib/testfwk$(OBJEXT) ports/$(PORT)/support$(OBJEXT) \
	$(GBDK_LIB)/libc/asm/z80/mul$(OBJEXT) \
	$(GBDK_LIB)/libc/asm/z80/div$(OBJEXT)

# Rule to generate a Emulator .bin file from the .ihx linker output.
%$(EXEEXT): %.ihx
	../makebin/makebin -s 32768 < $< > $@  # Must be 32768 due to a bug in ConsoleZ80

# Rule to link into .ihx
%.ihx: %$(OBJEXT) $(EXTRAS)
	../../bin/link-z80 -n -- -b_CODE=0x200 -b_DATA=0x8000 -i $@ $< $(EXTRAS)

%$(OBJEXT): %.c fwk/include/*.h
	$(SDCC) $(SDCCFLAGS) -c $<

%$(OBJEXT): %.asm
	../../bin/as-z80 -plosgff $@ $<

%$(OBJEXT): %.s
	../../bin/as-z80 -plosgff $@ $<

# PENDING: Path to sdcc-extra
%.out: %$(EXEEXT)
	mkdir -p `dirname $@`
	java -cp ../../../sdcc-extra/emu/rose ConsoleZ80 $< > $@
	if grep -q FAIL $@; then echo FAILURES in $@; fi
