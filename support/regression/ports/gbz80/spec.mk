# Port specification for the gbz80 port.

EMU = $(SDCC_EXTRA_DIR)/emu/rrgb/rrgb$(EXEEXT)

SDCCFLAGS +=-mgbz80 --nostdinc --less-pedantic -DREENTRANT=
LINKFLAGS = --nostdlib
LINKFLAGS += gbz80.lib
LIBDIR = $(top_builddir)/device/lib/build/gbz80

BINEXT = .gb

# Needs parts of gbdk-lib, namely the internal mul/div/mod functions.
EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)

# Rule to link into .ihx
%.gb: %.c $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) -L $(LIBDIR) $(EXTRAS) $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.asm
	@# TODO: sdas should place it\'s output in the current dir
	test $(srcdir) = . || cp $< $(PORT_CASES_DIR)
	$(top_builddir)/bin/as-gbz80 -plosgff $(PORT_CASES_DIR)/$(notdir $<)
	test $(srcdir) = . || rm $(PORT_CASES_DIR)/$(notdir $<)

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# PENDING: Path to sdcc-extra
%.out: %$(BINEXT)
	mkdir -p $(dir $@)
	$(EMU) -k -m $< > $@
	-grep -n FAIL $@ /dev/null || true

_clean:
