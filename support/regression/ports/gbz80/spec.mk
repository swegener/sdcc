# Port specification for the gbz80 port.

EMU = $(SDCC_EXTRA_DIR)/emu/rrgb/rrgb$(EXEEXT)
MAKEBIN = $(top_builddir)/bin/makebin$(EXEEXT)

SDCCFLAGS +=-mgbz80 --nostdinc --less-pedantic -DREENTRANT= -I$(top_srcdir)
LINKFLAGS = --nostdlib
LINKFLAGS += --out-fmt-ihx gbz80.lib
LIBDIR = $(top_builddir)/device/lib/build/gbz80

BINEXT = .gb

.PRECIOUS: gen/gbz80/support.rel gen/gbz80/testfwk.rel %.ihx

# Needs parts of gbdk-lib, namely the internal mul/div/mod functions.
EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)

# Rule to link into .ihx
%.ihx: %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) -L $(LIBDIR) $(EXTRAS) $< -o $@

# Rule to concert .ihx to .gb
%$(BINEXT): %.ihx
	$(MAKEBIN) -Z -yn ABS $< $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.asm
	@# TODO: sdas should place it\'s output in the current dir
	cp $< $(PORT_CASES_DIR)
	$(top_builddir)/bin/sdasgb -plosgff $(PORT_CASES_DIR)/$(notdir $<)
	rm $(PORT_CASES_DIR)/$(notdir $<)

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# PENDING: Path to sdcc-extra
%.out: %$(BINEXT)
	mkdir -p $(dir $@)
	$(EMU) -k $< > $@
	-grep -n FAIL $@ /dev/null || true

_clean:
