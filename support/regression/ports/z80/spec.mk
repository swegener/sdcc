SDCCFLAGS += -I/home/michaelh/projects/gbdk-lib/include

EXEEXT = .bin

EXTRAS = fwk/lib/testfwk$(OBJEXT) ports/$(PORT)/support$(OBJEXT) \
	/home/michaelh/projects/gbdk-lib/libc/asm/z80/mul$(OBJEXT) \
	/home/michaelh/projects/gbdk-lib/libc/asm/z80/div$(OBJEXT)

%$(EXEEXT): %.ihx
	../makebin/makebin -s 32768 < $< > $@

%.ihx: %$(OBJEXT) $(EXTRAS)
	../../bin/link-z80 -n -- -b_CODE=0x200 -b_DATA=0x8000 -i $@ $< $(EXTRAS)

%$(OBJEXT): %.c fwk/include/*.h
	$(SDCC) $(SDCCFLAGS) -c $<

%$(OBJEXT): %.asm
	../../bin/as-z80 -plosgff $@ $<

%$(OBJEXT): %.s
	../../bin/as-z80 -plosgff $@ $<

$(PORT_SUBRESULTS_DIR)/%.out: $(PORT_CASES_DIR)/%$(EXEEXT)
	mkdir -p `dirname $@`
	java -cp /home/michaelh/projects/rose ConsoleZ80 $< > $@
	if grep -q FAIL $@; then echo FAILURES in $@; fi

#$(PORT_SUBRESULTS_DIR)/%.out: $(PORT_CASES_DIR)/%$(EXEEXT)
#	mkdir -p `dirname $@`
#	-$< > $@
#	if grep -q FAIL $@; then echo FAILURES in $@; fi


#%$(EXEEXT): %$(OBJEXT) fwk/lib/testfwk$(OBJEXT)
#	$(SDCC) $(SDCCFLAGS) -o $@ $< fwk/lib/testfwk$(OBJEXT)

#%$(OBJEXT): %.c fwk/include/*.h
#	$(SDCC) $(SDCCFLAGS) -c $< -o $@
