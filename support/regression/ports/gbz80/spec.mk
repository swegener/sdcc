# Port specification for the gbz80 port.

EMU = $(SDCC_EXTRA_DIR)/emu/rrgb/rrgb

SDCCFLAGS +=-mgbz80 --lesspedantic -DREENTRANT=

EXEEXT = .gb

# Needs parts of gbdk-lib, namely the internal mul/div/mod functions.
EXTRAS = ports/$(PORT)/testfwk$(OBJEXT) ports/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
%.gb: %.c $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $< $(EXTRAS) -o $@

%$(OBJEXT): %.asm
	../../bin/as-gbz80 -plosgff $@ $<

%$(OBJEXT): %.s
	../../bin/as-gbz80 -plosgff $@ $<

%$(OBJEXT): %.c
	echo $(OBJEXT)
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

ports/$(PORT)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# PENDING: Path to sdcc-extra
%.out: %$(EXEEXT)
	mkdir -p `dirname $@`
	$(EMU) -k -m $< > $@
	-grep -n FAIL $@ /dev/null || true

_clean:
	rm -f ports/$(PORT)/testfwk.asm ports/$(PORT)/*.lst ports/$(PORT)/*.o ports/$(PORT)/*.sym

