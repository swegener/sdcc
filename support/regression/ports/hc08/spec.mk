# Port specification for the xxxx port running with uCsim
#

# path to uCsim
UCHC08 = $(SDCC_DIR)/bin/shc08

SDCCFLAGS +=-mhc08 --less-pedantic --out-fmt-ihx -DREENTRANT=
#SDCCFLAGS +=--less-pedantic -DREENTRANT=reentrant

OBJEXT = .rel
EXEEXT = .ihx

EXTRAS = $(PORTS_DIR)/$(PORT)/testfwk$(OBJEXT) $(PORTS_DIR)/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
#%$(EXEEXT): %$(OBJEXT) $(EXTRAS)

%$(EXEEXT): %.c $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $< $(EXTRAS) -o $@

%$(OBJEXT): %.asm
	$(SDCC_DIR)/bin/as-hc08 -plosgff $<

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORTS_DIR)/$(PORT)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 10 seconds timeout
%.out: %$(EXEEXT) fwk/lib/timeout
	mkdir -p `dirname $@`
	-fwk/lib/timeout 10 $(UCHC08) -t32 $< < $(PORTS_DIR)/$(PORT)/uCsim.cmd > $@ \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(EXEEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	-grep -n FAIL $@ /dev/null || true

fwk/lib/timeout: fwk/lib/timeout.c

_clean:
	rm -f ports/$(PORT)/testfwk.asm ports/$(PORT)/*.lst ports/$(PORT)/*.rel ports/$(PORT)/*.sym

