# Port specification for the ds390 port running with uCsim

# path to uCsim
S51A = $(SDCC_DIR)/sim/ucsim/s51.src/s51
S51B = $(SDCC_DIR)/bin/s51

S51 = $(shell if [ -f $(S51A) ]; then echo $(S51A); else echo $(S51B); fi)

SDCCFLAGS +=-mds390 --lesspedantic -DREENTRANT=reentrant --stack-after-data

OBJEXT = .rel
EXEEXT = .ihx

EXTRAS = fwk/lib/testfwk$(OBJEXT) $(PORTS_DIR)/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(EXTRAS) $< -o $@

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 10 seconds timeout
%.out: %$(EXEEXT) fwk/lib/timeout
	mkdir -p `dirname $@`
	-fwk/lib/timeout 10 $(S51) -tds390f -S in=/dev/null,out=$@ $< < $(PORTS_DIR)/ds390/uCsim.cmd >/dev/null || \
          echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(EXEEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	-grep -n FAIL $@ /dev/null || true

fwk/lib/timeout: fwk/lib/timeout.c

_clean:
	rm -f fwk/lib/timeout fwk/lib/timeout.exe $(PORTS_DIR)/$(PORT)/*.rel $(PORTS_DIR)/$(PORT)/*.rst \
	      $(PORTS_DIR)/$(PORT)/*.lst $(PORTS_DIR)/$(PORT)/*.sym $(PORTS_DIR)/$(PORT)/*.asm temp.lnk

