# Regression test specification for the z80 target running with uCsim
#

# path to uCsim
ifdef SDCC_BIN_PATH
  UCZ80 = $(SDCC_BIN_PATH)/sz80$(EXEEXT)

  AS_Z80 = $(SDCC_BIN_PATH)/as-z80$(EXEEXT)
else
  SZ80A = $(top_builddir)/sim/ucsim/z80.src/sz80$(EXEEXT)
  SZ80B = $(top_builddir)/bin/sz80$(EXEEXT)

  UCZ80 = $(shell if [ -f $(SZ80A) ]; then echo $(SZ80A); else echo $(SZ80B); fi)

  AS_Z80 = $(top_builddir)/bin/as-z80$(EXEEXT)

  SDCCFLAGS += --nostdinc -I$(top_srcdir)
  LINKFLAGS += --nostdlib -L$(top_builddir)/device/lib/build/z80
endif

SDCCFLAGS +=-mz80 --less-pedantic --profile -DREENTRANT=
#SDCCFLAGS +=--less-pedantic -DREENTRANT=reentrant
LINKFLAGS += z80.lib

#OBJEXT = .o
BINEXT = .ihx

EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)

# Rule to link into .ihx
%.ihx: %.c $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) $(EXTRAS) $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.asm
	@# TODO: sdas should place it\'s output in the current dir
	cp $< $(PORT_CASES_DIR)
	$(AS_Z80) -plosgff $(PORT_CASES_DIR)/$(notdir $<)
	rm $(PORT_CASES_DIR)/$(notdir $<)

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 10 seconds timeout
%.out: %$(BINEXT) $(CASES_DIR)/timeout$(EXEEXT)
	mkdir -p $(dir $@)
	-$(CASES_DIR)/timeout 10 $(UCZ80) $< < $(PORTS_DIR)/$(PORT)/uCsim.cmd > $@ \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(BINEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	python $(srcdir)/get_ticks.py < $@ >> $@
	-grep -n FAIL $@ /dev/null || true

$(CASES_DIR)/timeout$(EXEEXT): fwk/lib/timeout.c
	$(CC) $(CFLAGS) $< -o $@

_clean:
