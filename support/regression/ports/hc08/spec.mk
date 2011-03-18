# Regression test specification for the hc08 target running with uCsim

CC_FOR_BUILD = $(CC)

# path to uCsim
ifdef SDCC_BIN_PATH
  UCHC08 = $(SDCC_BIN_PATH)/shc08$(EXEEXT)

  AS_HC08 = $(SDCC_BIN_PATH)/sdas6808$(EXEEXT)
else
  UCHC08A = $(top_builddir)/sim/ucsim/hc08.src/shc08$(EXEEXT)
  UCHC08B = $(top_builddir)/bin/shc08$(EXEEXT)

  UCHC08C = $(shell if [ -f $(UCHC08A) ]; then echo $(UCHC08A); else echo $(UCHC08B); fi)

  AS_HC08C = $(top_builddir)/bin/sdas6808$(EXEEXT)

ifndef CROSSCOMPILING
  SDCCFLAGS += --nostdinc -I$(top_srcdir)
  LINKFLAGS += --nostdlib -L$(top_builddir)/device/lib/build/hc08
endif
endif

ifdef CROSSCOMPILING
  SDCCFLAGS += -I$(top_srcdir)

  UCHC08 = wine $(UCHC08C)
  AS_HC08 = wine $(AS_HC08C)
else
  UCHC08 = $(UCHC08C)
  AS_HC08 = $(AS_HC08C)
endif

SDCCFLAGS +=-mhc08 --less-pedantic --out-fmt-ihx -DREENTRANT=__reentrant
LINKFLAGS += hc08.lib

OBJEXT = .rel
BINEXT = .ihx

# otherwise `make` deletes testfwk.o and `make -j` will fail
.PRECIOUS: $(PORT_CASES_DIR)/%$(OBJEXT)

# Required extras
EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)
FWKLIB = $(PORT_CASES_DIR)/statics$(OBJEXT)

# Rule to link into .ihx
%$(BINEXT): %$(OBJEXT) $(EXTRAS) $(FWKLIB) $(PORT_CASES_DIR)/fwk.lib
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) $(EXTRAS) $(PORT_CASES_DIR)/fwk.lib $< -o $@

%$(OBJEXT): %.asm
	$(AS_HC08) -plosgff $<

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/fwk.lib:
	cp $(PORTS_DIR)/$(PORT)/fwk.lib $@

# run simulator with 10 seconds timeout
%.out: %$(BINEXT) $(CASES_DIR)/timeout
	mkdir -p $(dir $@)
	-$(CASES_DIR)/timeout 10 $(UCHC08) $< < $(PORTS_DIR)/$(PORT)/uCsim.cmd > $(@:.out=.sim) \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(BINEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	python $(srcdir)/get_ticks.py < $(@:.out=.sim) >> $@
	-grep -n FAIL $@ /dev/null || true

$(CASES_DIR)/timeout: $(srcdir)/fwk/lib/timeout.c
	$(CC_FOR_BUILD) $(CFLAGS) $< -o $@

_clean:
