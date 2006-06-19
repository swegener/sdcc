# Port specification for the pic16 port running with gpsim

# path to gpsim
ifdef GPSIM_PATH
  GPSIM := $(GPSIM_PATH)/gpsim
else
  GPSIM := gpsim
endif

SDCCFLAGS += -mpic16 -pp18f452 -I$(top_srcdir)/device/include/pic16 --nostdinc --less-pedantic -Wl,-q -DREENTRANT=reentrant -I$(top_srcdir)
LINKFLAGS = --nostdlib
LINKFLAGS += libsdcc.lib libc18f.lib libm18f.lib
LIBDIR = $(top_builddir)device/lib/build/pic16

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

.PRECIOUS: gen/pic16/testfwk.o gen/pic16/support.o

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 25 seconds timeout
%.out: %$(EXEEXT) $(CASES_DIR)/timeout
	mkdir -p $(dir $@)
	-$(CASES_DIR)/timeout 25 "$(GPSIM)" -i -s $< -c $(PORTS_DIR)/pic16/gpsim.cmd > $@ || \
	  echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(EXEEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	python $(srcdir)/get_ticks.py < $@ >> $@
	-grep -n FAIL $@ /dev/null || true

$(CASES_DIR)/timeout: fwk/lib/timeout.c
	$(CC) $< -o $@

_clean:
