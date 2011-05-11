# Common regression test specification for the mcs51 targets running with uCsim

CC_FOR_BUILD = $(CC)

# path to uCsim
ifdef SDCC_BIN_PATH
  S51 = $(SDCC_BIN_PATH)/s51$(EXEEXT)
else
  S51A = $(top_builddir)/sim/ucsim/s51.src/s51$(EXEEXT)
  S51B = $(top_builddir)/bin/s51$(EXEEXT)

  S51C = $(shell if [ -f $(S51A) ]; then echo $(S51A); else echo $(S51B); fi)

ifndef CROSSCOMPILING
  SDCCFLAGS += --nostdinc -I$(INC_DIR)/mcs51 -I$(top_srcdir)
  LINKFLAGS += --nostdlib -L$(LIBDIR)
endif
endif

ifdef CROSSCOMPILING
  DEV_NULL ?= NUL
  SDCCFLAGS += -I$(top_srcdir)

  EMU = wine $(S51C)
else
  DEV_NULL ?= /dev/null

  EMU = $(S51C)
endif

SDCCFLAGS += --less-pedantic -DREENTRANT=__reentrant
LINKFLAGS += mcs51.lib libsdcc.lib liblong.lib libint.lib libfloat.lib

OBJEXT = .rel
BINEXT = .ihx

# otherwise `make` deletes testfwk.rel and `make -j` will fail
.PRECIOUS: $(PORT_CASES_DIR)/%$(OBJEXT)

# Required extras
EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)
include $(srcdir)/fwk/lib/spec.mk
FWKLIB += $(PORT_CASES_DIR)/T2_isr$(OBJEXT)

# Rule to link into .ihx
%$(BINEXT): %$(OBJEXT) $(EXTRAS) $(FWKLIB) $(PORT_CASES_DIR)/fwk.lib
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) $(EXTRAS) $(PORT_CASES_DIR)/fwk.lib $< -o $@

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/mcs51-common/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(srcdir)/fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/fwk.lib: $(srcdir)/fwk/lib/fwk.lib $(PORTS_DIR)/mcs51-common/fwk.lib
	cat < $(srcdir)/fwk/lib/fwk.lib > $@
	cat < $(PORTS_DIR)/mcs51-common/fwk.lib >> $@

# run simulator with 30 seconds timeout
%.out: %$(BINEXT) $(CASES_DIR)/timeout
	mkdir -p $(dir $@)
	-$(CASES_DIR)/timeout 30 $(EMU) -t32 -S in=$(DEV_NULL),out=$@ $< < $(PORTS_DIR)/mcs51-common/uCsim.cmd > $(@:.out=.sim) \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(BINEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	python $(srcdir)/get_ticks.py < $(@:.out=.sim) >> $@
	-grep -n FAIL $@ /dev/null || true

$(CASES_DIR)/timeout: $(srcdir)/fwk/lib/timeout.c
	$(CC_FOR_BUILD) $(CFLAGS) $< -o $@

_clean:
