# Port specification for the z80 port running ontop of the Java based
# 'ConsoleZ80' emulator.

# PENDING: Path to gbdk-lib for stdarg
GBDK_LIB = ../../../gbdk-lib
RRZ80 = $(SDCC_EXTRA_DIR)/emu/rrz80/rrz80

SDCCFLAGS += -I$(GBDK_LIB)/include --lesspedantic

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
	../../bin/link-z80 -n -- -b_CODE=0x200 -b_DATA=0x8000 -j -i $@ $< $(EXTRAS)

%$(OBJEXT): %.c fwk/include/*.h
	$(SDCC) $(SDCCFLAGS) -c $<

%$(OBJEXT): %.asm
	../../bin/as-z80 -plosgff $@ $<

%$(OBJEXT): %.s
	../../bin/as-z80 -plosgff $@ $<

# PENDING: Path to sdcc-extra
%.out: %$(EXEEXT)
	mkdir -p `dirname $@`
	$(RRZ80) --maxruntime=3 --mapfile=$(<:.bin=.sym) $< > $@
	-grep -n FAIL $@ /dev/null || true

