# path to uCsim
UCHC08A = $(SDCC_DIR)/sim/ucsim/hc08.src/shc08
UCHC08B = $(SDCC_DIR)/bin/shc08

UCHC08 = $(shell if [ -f $(UCHC08A) ]; then echo $(UCHC08A); else echo $(UCHC08B); fi)

SDCCFLAGS +=-mhc08 --nostdinc --less-pedantic --out-fmt-ihx -DREENTRANT=reentrant
LINKFLAGS = --nostdlib
LINKFLAGS += hc08.lib
LIBDIR = $(SDCC_DIR)/device/lib/build/hc08

OBJEXT = .rel
EXEEXT = .ihx

EXTRAS = $(PORTS_DIR)/$(PORT)/testfwk$(OBJEXT) $(PORTS_DIR)/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
#%$(EXEEXT): %$(OBJEXT) $(EXTRAS)

%$(EXEEXT): %.c $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) -L $(LIBDIR) $(EXTRAS) $< -o $@

%$(OBJEXT): %.asm
	$(SDCC_DIR)/bin/as-hc08 -plosgff $<

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORTS_DIR)/$(PORT)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 10 seconds timeout
%.out: %$(EXEEXT) fwk/lib/timeout
	mkdir -p `dirname $@`
	-fwk/lib/timeout 10 $(UCHC08) $< < $(PORTS_DIR)/$(PORT)/uCsim.cmd > $@ \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(EXEEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	python get_ticks.py < $@ >> $@
	-grep -n FAIL $@ /dev/null || true

fwk/lib/timeout: fwk/lib/timeout.c

_clean:
	rm -f ports/$(PORT)/testfwk.asm ports/$(PORT)/*.lst ports/$(PORT)/*.rel ports/$(PORT)/*.sym

