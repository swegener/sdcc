# Port specification for the xxxx port running with uCsim
#

# path to uCsim
UCZ80 = $(SDCC_DIR)/bin/sz80
#UCZ80 = $(SDCC_DIR)/bin/s51

SDCCFLAGS +=-mz80 --less-pedantic --profile -DREENTRANT=
#SDCCFLAGS +=--less-pedantic -DREENTRANT=reentrant

#OBJEXT = .o
EXEEXT = .ihx

EXTRAS = $(PORTS_DIR)/$(PORT)/testfwk$(OBJEXT) $(PORTS_DIR)/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
#%$(EXEEXT): %$(OBJEXT) $(EXTRAS)

%$(EXEEXT): %.c $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $< $(EXTRAS) -o $@

%$(OBJEXT): %.asm
	$(SDCC_DIR)/bin/as-z80 -plosgff $@ $<

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORTS_DIR)/$(PORT)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 10 seconds timeout
%.out: %$(EXEEXT) fwk/lib/timeout
	mkdir -p `dirname $@`
	-fwk/lib/timeout 10 $(UCZ80) -t32 $< < $(PORTS_DIR)/$(PORT)/uCsim.cmd > $@ \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(EXEEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	-grep -n FAIL $@ /dev/null || true

fwk/lib/timeout: fwk/lib/timeout.c

_clean:
	rm -f ports/$(PORT)/testfwk.asm ports/$(PORT)/*.lst ports/$(PORT)/*.o ports/$(PORT)/*.sym

