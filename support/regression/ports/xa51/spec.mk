# Port specification for the xa51 port running with uCsim

# path to uCsim
SXA_A = $(SDCC_DIR)/sim/ucsim/xa.src/sxa
SXA_B = $(SDCC_DIR)/bin/sxa

SXA = $(shell if [ -f $(SXA_A) ]; then echo $(SXA_A); else echo $(SXA_B); fi)

SDCCFLAGS +=-mxa51 --lesspedantic -DREENTRANT=

OBJEXT = .rel
EXEEXT = .hex

EXTRAS = fwk/lib/testfwk$(OBJEXT) $(PORTS_DIR)/$(PORT)/support$(OBJEXT)

# Rule to link into .hex
%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(EXTRAS) $<
	mv fwk/lib/testfwk.hex $@
	mv fwk/lib/testfwk.map $(@:.hex=.map)

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $<

# run simulator with 10 seconds timeout
%.out: %$(EXEEXT) fwk/lib/timeout
	mkdir -p `dirname $@`
	-fwk/lib/timeout 1 $(SXA) -S in=/dev/null,out=$@ $< < $(PORTS_DIR)/xa51/uCsim.cmd >/dev/null || \
          echo -e --- FAIL: \"timeout, simulation killed\" in $(<:.ihx=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	-grep -n FAIL $@ /dev/null || true

fwk/lib/timeout: fwk/lib/timeout.c

_clean:
	rm -f fwk/lib/timeout fwk/lib/timeout.exe $(PORTS_DIR)/$(PORT)/*.rel $(PORTS_DIR)/$(PORT)/*.rst \
	      $(PORTS_DIR)/$(PORT)/*.lst $(PORTS_DIR)/$(PORT)/*.sym $(PORTS_DIR)/$(PORT)/*.xa temp.lnk


