# Port specification for the ds390 port running with uCsim

# path to uCsim
S51 = ../../sim/ucsim/s51.src/s51

SDCCFLAGS +=-mds390 --lesspedantic -DREENTRANT=reentrant --stack-after-data

OBJEXT = .rel
EXEEXT = .ihx

EXTRAS = fwk/lib/testfwk$(OBJEXT) $(PORTS_DIR)/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(EXTRAS) $<
	mv fwk/lib/testfwk.ihx $@
	mv fwk/lib/testfwk.map $(@:.ihx=.map)

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $<

# run simulator with 5 seconds timeout
%.out: %$(EXEEXT) fwk/lib/timeout
	mkdir -p `dirname $@`
	-fwk/lib/timeout 5 $(S51) -tds390f -S in=/dev/null,out=$@ $< < $(PORTS_DIR)/ds390/uCsim.cmd >/dev/null || \
          echo -e --- FAIL: \"timeout, simulation killed\" in $(<:.ihx=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	-grep -n FAIL $@ /dev/null || true

fwk/lib/timeout: fwk/lib/timeout.c
	gcc -o $@ $<

_clean:
	rm -f fwk/lib/timeout fwk/lib/timeout.exe $(PORTS_DIR)/$(PORT)/*.rel $(PORTS_DIR)/$(PORT)/*.rst \
	      $(PORTS_DIR)/$(PORT)/*.lst $(PORTS_DIR)/$(PORT)/*.sym $(PORTS_DIR)/$(PORT)/*.asm temp.lnk

