# Port specification for the xxxx port running with uCsim
#

# path to uCsim
UCZ80 = $(SDCC_DIR)/bin/sz80
#UCZ80 = $(SDCC_DIR)/bin/s51

SDCCFLAGS +=-mz80 --lesspedantic --profile -DREENTRANT=
#SDCCFLAGS +=--lesspedantic -DREENTRANT=reentrant --stack-after-data

#OBJEXT = .o
EXEEXT = .ihx

EXTRAS = fwk/lib/testfwk$(OBJEXT) $(PORTS_DIR)/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
#%$(EXEEXT): %$(OBJEXT) $(EXTRAS)

%$(EXEEXT): %.c $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $< $(EXTRAS)
#	mv fwk/lib/testfwk.ihx $@
#	mv fwk/lib/testfwk.map $(@:.ihx=.map)

%$(OBJEXT): %.asm
	$(SDCC_DIR)/bin/as-z80 -plosgff $@ $<

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $<

# run simulator with 10 seconds timeout
%.out: %$(EXEEXT) fwk/lib/timeout
	mkdir -p `dirname $@`
	-fwk/lib/timeout 10 $(UCZ80) -t32 $< < $(PORTS_DIR)/$(PORT)/uCsim.cmd > $@ \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:.ihx=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	-grep -n FAIL $@ /dev/null || true

fwk/lib/timeout: fwk/lib/timeout.c

_clean:
	rm -f fwk/lib/timeout fwk/lib/timeout.exe $(PORTS_DIR)/$(PORT)/*.rel $(PORTS_DIR)/$(PORT)/*.rst \
	      $(PORTS_DIR)/$(PORT)/*.lst $(PORTS_DIR)/$(PORT)/*.sym $(PORTS_DIR)/$(PORT)/*.asm temp.lnk

