# Port specification for the xa51 port running with uCsim

ifndef DEV_NULL
  DEV_NULL = /dev/null
endif

# path to uCsim
SXA_A = $(top_builddir)sim/ucsim/xa.src/sxa
SXA_B = $(top_builddir)bin/sxa

SXA = $(shell if [ -f $(SXA_A) ]; then echo $(SXA_A); else echo $(SXA_B); fi)

SDCCFLAGS +=-mxa51 --less-pedantic -DREENTRANT= -I$(top_srcdir)

OBJEXT = .rel
EXEEXT = .hex

EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)

# Rule to link into .hex
%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(EXTRAS) $< -o $@

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 1 second timeout
%.out: %$(EXEEXT) $(CASES_DIR)/timeout
	mkdir -p $(dir $@)
	-$(CASES_DIR)/timeout 1 $(SXA) -S in=$(DEV_NULL),out=$@ $< < $(PORTS_DIR)/xa51/uCsim.cmd >/dev/null || \
          echo -e --- FAIL: \"timeout, simulation killed\" in $(<:.ihx=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	-grep -n FAIL $@ /dev/null || true

$(CASES_DIR)/timeout: fwk/lib/timeout.c
	$(CC) $< -o $@

_clean:
