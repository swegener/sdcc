# Port specification for the mcs51 port running with uCsim

S51 = ../../sim/ucsim/s51.src/s51

SDCCFLAGS += --lesspedantic -DREENTRANT=reentrant -DGENERIC=_generic --stack-after-data

OBJEXT = .rel
EXEEXT = .ihx

# Needs parts of gbdk-lib, namely the internal mul/div/mod functions.
EXTRAS = fwk/lib/testfwk$(OBJEXT) ports/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
%.ihx: %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(EXTRAS) $<
	mv fwk/lib/testfwk.ihx $@
	mv fwk/lib/testfwk.map $(@:.ihx=.map)

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $<

# PENDING: Path to sdcc-extra
%.out: %$(EXEEXT)
	mkdir -p `dirname $@`
	$(S51) -t32 -S in=$(shell tty),out=$@ $< < ports/mcs51/uCsim.cmd >/dev/null 2>&1
	-grep -n FAIL $@ /dev/null || true
