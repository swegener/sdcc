# Port specification for the pic14 port running with gpsim

# path to gpsim
ifdef GPSIM_PATH
  GPSIM := $(GPSIM_PATH)/gpsim
else
  GPSIM := gpsim
endif

SDCCFLAGS += -mpic14 -pp16f877 -I$(top_srcdir)/device/include/pic --nostdinc --less-pedantic -Wl,-q -DREENTRANT=reentrant -I$(top_srcdir)
LINKFLAGS = --nostdlib
LINKFLAGS += libsdcc.lib libm.lib
LIBDIR = $(top_builddir)/device/lib/build/pic

OBJEXT = .o
EXEEXT = .cod

EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)

# Rule to link into .ihx
%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	-$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) -L $(LIBDIR) $(EXTRAS) $< -o $@

%$(OBJEXT): %.c
	-$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.c
	-$(SDCC) $(SDCCFLAGS) -c $< -o $@

.PRECIOUS: gen/pic14/testfwk.o gen/pic14/support.o

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 25 seconds timeout
%.out: %$(EXEEXT) $(CASES_DIR)/timeout
	mkdir -p $(dir $@)
	-$(CASES_DIR)/timeout 25 "$(GPSIM)" -i -s $< -c $(PORTS_DIR)/pic14/gpsim.cmd > $@ || \
	  echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(EXEEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	python $(srcdir)/get_ticks.py < $@ >> $@
	-grep -n FAIL $@ /dev/null || true

$(CASES_DIR)/timeout: fwk/lib/timeout.c
	$(CC) $(CFLAGS) $< -o $@

_clean:
