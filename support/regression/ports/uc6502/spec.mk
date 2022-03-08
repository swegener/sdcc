# Regression test specification for the mos6502 target running with uCsim

# simulation timeout in cycles
SIM_CYCLES = 1000000000
SIM_TIMEOUT = 30

ifdef SDCC_BIN_PATH
  AS = $(SDCC_BIN_PATH)/sdas6500$(EXEEXT)
  UC65 = $(SDCC_BIN_PATH)/ucsim_mos6502$(EXEEXT)
else
  ifdef UCSIM_DIR
    UC65 = $(UCSIM_DIR)/mos6502.src/ucsim_mos6502$(EXEEXT)
  else
    UC65 = $(top_builddir)/sim/ucsim/mos6502.src/ucsim_mos6502$(EXEEXT)
  endif
  AS = $(WINE) $(top_builddir)/bin/sdas6500$(EXEEXT)
ifndef CROSSCOMPILING
  SDCCFLAGS += --nostdinc -I$(top_srcdir)
  LINKFLAGS += --nostdlib -L$(top_builddir)/device/lib/build/mos6502
endif
endif

  EMU = $(WINE) $(UC65)

ifdef CROSSCOMPILING
  SDCCFLAGS += -I$(top_srcdir)
endif

SDCCFLAGS += -mmos6502 --less-pedantic --code-loc 0x200 --xram-loc 0xc000
LINKFLAGS += mos6502.lib

OBJEXT = .rel
BINEXT = .ihx

# otherwise `make` deletes testfwk.rel and `make -j` will fail
.PRECIOUS: $(PORT_CASES_DIR)/%$(OBJEXT)

# Required extras
EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)
include $(srcdir)/fwk/lib/spec.mk

# Rule to link into .ihx
%$(BINEXT): %$(OBJEXT) $(EXTRAS) $(FWKLIB) $(PORT_CASES_DIR)/fwk.lib
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) $(EXTRAS) $(PORT_CASES_DIR)/fwk.lib $< -o $@

%$(OBJEXT): %.asm
	$(AS) -plosgff $<

%$(OBJEXT): %.c
	$(VALGRIND) $(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(srcdir)/fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/fwk.lib: $(srcdir)/fwk/lib/fwk.lib
	cat < $(srcdir)/fwk/lib/fwk.lib > $@

# run simulator with SIM_TIMEOUT seconds timeout
%.out: %$(BINEXT) $(CASES_DIR)/timeout
	mkdir -p $(dir $@)
	-$(CASES_DIR)/timeout $(SIM_TIMEOUT) $(EMU) $(EMU_PORT_FLAG) $< < $(PORTS_DIR)/$(PORT)/uCsim.cmd > $@ \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(BINEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	$(PYTHON) $(srcdir)/get_ticks.py < $@ >> $@
	-grep -n FAIL $@ /dev/null || true

_clean:
