# Port specification for compiling on the host machines version of gcc
SDCC = $(shell which gcc > /dev/null || which cc)
SDCCFLAGS = -DPORT_HOST=1 -Wall -fsigned-char -DREENTRANT=

EXEEXT = .bin

# Required extras
EXTRAS = ports/$(PORT)/testfwk$(OBJEXT) ports/$(PORT)/support$(OBJEXT)

%.out: %$(EXEEXT)
	mkdir -p `dirname $@`
	-$< > $@
	-grep -n FAIL $@ /dev/null || true	

%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) -o $@ $< $(EXTRAS)

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

ports/$(PORT)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

_clean:
	rm -f ports/$(PORT)/support.o ports/$(PORT)/testfwk$(OBJEXT)

