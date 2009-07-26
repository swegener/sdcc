# Port specification for the z80 port running ontop of the Java based
# 'ConsoleZ80' emulator.

RRZ80 = $(SDCC_EXTRA_DIR)/emu/rrz80/rrz80$(EXEEXT)

SDCCFLAGS +=-mz80 --nostdinc --less-pedantic --profile -DREENTRANT= -I$(top_srcdir)
LINKFLAGS = --nostdlib
LINKFLAGS += z80.lib
LIBDIR = $(top_builddir)/device/lib/build/z80

BINEXT = .bin

# Needs parts of gbdk-lib, namely the internal mul/div/mod functions.
EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)

# Rule to generate a Emulator .bin file from the .ihx linker output.
%$(BINEXT): %.ihx
	$(top_builddir)/bin/makebin -s 32768 < $< > $@

# Rule to link into .ihx
%.ihx: %.c $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) -L$(LIBDIR) $(EXTRAS) $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.asm
	$(top_builddir)/bin/as-z80 -plosgff $@ $<

%$(OBJEXT): %.s
	$(top_builddir)/bin/as-z80 -plosgff $@ $<

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# PENDING: Path to sdcc-extra
%.out: %$(BINEXT)
	mkdir -p $(dir $@)
	$(RRZ80) --maxruntime=3 --mapfile=$(<:.bin=.sym) $< > $@
	-grep -n FAIL $@ /dev/null || true

_clean:
