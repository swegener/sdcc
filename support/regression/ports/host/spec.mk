# Port specification for compiling on the host machines version of gcc
SDCC = gcc
SDCCFLAGS = -Wall

EXEEXT = .bin

# Required extras
EXTRAS = fwk/lib/testfwk$(OBJEXT) ports/$(PORT)/support$(OBJEXT)

%.out: %$(EXEEXT)
	mkdir -p `dirname $@`
	-$< > $@
	if grep -q FAIL $@; then echo FAILURES in $@; fi

%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) -o $@ $< $(EXTRAS)

%$(OBJEXT): %.c fwk/include/*.h
	$(SDCC) $(SDCCFLAGS) -c $< -o $@
