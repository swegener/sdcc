# Port specification for compiling on the host machines version of gcc
SDCC = $(shell ( sh -c "gcc --version" 2>&1 ) > /dev/null  && echo gcc || echo cc)
SDCCFLAGS = -DPORT_HOST=1 -Wall -fsigned-char -fpack-struct -DREENTRANT=

EXEEXT = .bin
OBJEXT = .o

# otherwise `make` deletes it and `make -j`will fail
.PRECIOUS: ports/$(PORT)/testfwk$(OBJEXT)

# Required extras
EXTRAS = ports/$(PORT)/testfwk$(OBJEXT) ports/$(PORT)/support$(OBJEXT)

%.out: %$(EXEEXT)
	mkdir -p `dirname $@`
	-$< > $@
	-grep -n FAIL $@ /dev/null || true	

%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) -o $@ $< $(EXTRAS) -lm

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

ports/$(PORT)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

_clean:
	rm -f ports/$(PORT)/support.o ports/$(PORT)/testfwk$(OBJEXT)

