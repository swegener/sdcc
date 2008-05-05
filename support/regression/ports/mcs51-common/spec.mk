# Common regression test specification for the mcs51 targets running with uCsim

ifndef DEV_NULL
  DEV_NULL = /dev/null
endif

# path to uCsim
ifdef SDCC_BIN_PATH
  S51 = $(SDCC_BIN_PATH)/s51
else
  S51A = $(top_builddir)/sim/ucsim/s51.src/s51
  S51B = $(top_builddir)/bin/s51

  S51 = $(shell if [ -f $(S51A) ]; then echo $(S51A); else echo $(S51B); fi)

  SDCCFLAGS += --nostdinc -I$(INC_DIR)/mcs51 -I$(top_srcdir)
  LINKFLAGS += --nostdlib -L$(LIBDIR)
endif

SDCCFLAGS += --less-pedantic -DREENTRANT=reentrant
LINKFLAGS += mcs51.lib libsdcc.lib liblong.lib libint.lib libfloat.lib

OBJEXT = .rel
EXEEXT = .ihx

EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)
FWKLIB = $(PORT_CASES_DIR)/T2_isr$(OBJEXT)

# Rule to link into .ihx
%$(EXEEXT): %$(OBJEXT) $(EXTRAS) $(FWKLIB) $(PORT_CASES_DIR)/fwk.lib
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) $(EXTRAS) $(PORT_CASES_DIR)/fwk.lib $< -o $@

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/mcs51-common/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/fwk.lib:
	cp $(PORTS_DIR)/mcs51-common/fwk.lib $@

# run simulator with 30 seconds timeout
%.out: %$(EXEEXT) gen/timeout
	mkdir -p $(dir $@)
	-gen/timeout 30 "$(S51)" -t32 -S in=$(DEV_NULL),out=$@ $< < $(PORTS_DIR)/mcs51-common/uCsim.cmd > $(@:.out=.sim) \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(EXEEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	python $(srcdir)/get_ticks.py < $(@:.out=.sim) >> $@
	-grep -n FAIL $@ /dev/null || true


gen/timeout: $(srcdir)/fwk/lib/timeout.c
	$(CC) $(CFLAGS) $< -o $@

_clean:
