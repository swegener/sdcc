SDCC = gcc
SDCCFLAGS = -Wall

EXEEXT = .bin

EXTRAS = fwk/lib/testfwk$(OBJEXT) ports/$(PORT)/support$(OBJEXT)

$(PORT_SUBRESULTS_DIR)/%.out: $(PORT_CASES_DIR)/%$(EXEEXT)
	mkdir -p `dirname $@`
	-$< > $@
	if grep -q FAIL $@; then echo FAILURES in $@; fi

%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) -o $@ $< $(EXTRAS)

%$(OBJEXT): %.c fwk/include/*.h
	$(SDCC) $(SDCCFLAGS) -c $< -o $@
