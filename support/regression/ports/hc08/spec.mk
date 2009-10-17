# Regression test specification for the hc08 target running with uCsim
#

# path to uCsim
ifdef SDCC_BIN_PATH
  UCHC08 = $(SDCC_BIN_PATH)/shc08$(EXEEXT)

  AS_HC08 = $(SDCC_BIN_PATH)/sdas6808$(EXEEXT)
else
  UCHC08A = $(top_builddir)/sim/ucsim/hc08.src/shc08$(EXEEXT)
  UCHC08B = $(top_builddir)/bin/shc08$(EXEEXT)

  UCHC08 = $(shell if [ -f $(UCHC08A) ]; then echo $(UCHC08A); else echo $(UCHC08B); fi)

  AS_HC08 = $(top_builddir)/bin/sdas6808$(EXEEXT)

  SDCCFLAGS += --nostdinc -I$(top_srcdir)
  LINKFLAGS += --nostdlib -L$(top_builddir)/device/lib/build/hc08
endif

SDCCFLAGS +=-mhc08 --less-pedantic --out-fmt-ihx -DREENTRANT=reentrant
LINKFLAGS += hc08.lib

OBJEXT = .rel
BINEXT = .ihx

EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)

# Rule to link into .ihx
#%$(BINEXT): %$(OBJEXT) $(EXTRAS)

%$(BINEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) $(EXTRAS) $< -o $@

%$(OBJEXT): %.asm
	$(AS_HC08) -plosgff $<

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 10 seconds timeout
%.out: %$(BINEXT) $(CASES_DIR)/timeout$(EXEEXT)
	mkdir -p $(dir $@)
	-$(CASES_DIR)/timeout 10 $(UCHC08) $< < $(PORTS_DIR)/$(PORT)/uCsim.cmd > $@ \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(BINEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	python $(srcdir)/get_ticks.py < $@ >> $@
	-grep -n FAIL $@ /dev/null || true

$(CASES_DIR)/timeout$(EXEEXT): fwk/lib/timeout.c
	$(CC) $(CFLAGS) $< -o $@

_clean:
