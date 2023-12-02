# Regression test specification for the pic16 target running with gpsim

# simulation timeout in seconds
SIM_TIMEOUT = 25

# path to gpsim
ifdef GPSIM_PATH
  GPSIM := $(WINE) $(GPSIM_PATH)/gpsim$(EXEEXT)
else
  GPSIM := $(WINE) gpsim$(EXEEXT)
endif

EMU = $(GPSIM)
EMU_FLAGS = -i -s
EMU_INPUT = -c $(PORTS_DIR)/pic16/gpsim.cmd

ifndef SDCC_BIN_PATH
  ifndef CROSSCOMPILING
    SDCCFLAGS += --nostdinc -I$(top_srcdir)/device/include/pic16 -I$(top_srcdir)/device/non-free/include/pic16 -I$(top_srcdir)
    LINKFLAGS += --nostdlib -L$(top_builddir)/device/lib/build/pic16 -L$(top_builddir)/device/non-free/lib/build/pic16
  else
    SDCCFLAGS += --use-non-free
  endif
else
  SDCCFLAGS += --use-non-free
endif

ifdef CROSSCOMPILING
  SDCCFLAGS += -I$(top_srcdir)
endif

SDCCFLAGS += -mpic16 -pp18f452 --less-pedantic -Wl,-q
SDCCFLAGS += --no-peep
SDCCFLAGS += --no-warn-non-free
# The default crt0iz.o does not work in the simulator.  It clears all memory
# from 0xeff to 0x0, many devices don't have that much memory.
SDCCFLAGS += --use-crt=crt0i.o
LINKFLAGS += libsdcc.lib libc18f.lib libm18f.lib

OBJEXT = .o
BINEXT = .cod

EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)

# Rule to link into .ihx
%$(BINEXT): %$(OBJEXT) $(EXTRAS)
	-$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) $(EXTRAS) $< -o $@

%$(OBJEXT): %.c
	-$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.c
	-$(SDCC) $(SDCCFLAGS) -c $< -o $@

.PRECIOUS: gen/pic16/testfwk.o gen/pic16/support.o

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

_clean:
