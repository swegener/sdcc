# Port specification for the pic16 port running with gpsim

# path to gpsim
ifdef GPSIM_PATH
  GPSIM := $(GPSIM_PATH)/gpsim
else
  GPSIM := gpsim
endif

SDCCFLAGS += -mpic16 -pp18f452 -I$(SDCC_DIR)/device/include/pic16 --nostdinc --less-pedantic -Wl,-t255 -Wl,-q -DREENTRANT=reentrant
LINKFLAGS = --nostdlib
LINKFLAGS += libsdcc.lib libc18f.lib libm18f.lib
LIBDIR = $(SDCC_DIR)/device/lib/build/pic16

OBJEXT = .o
EXEEXT = .cod

EXTRAS = $(PORTS_DIR)/$(PORT)/testfwk$(OBJEXT) $(PORTS_DIR)/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) -L $(LIBDIR) $(EXTRAS) $< -o $@

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORTS_DIR)/$(PORT)/testfwk$(OBJEXT): fwk/lib/testfwk.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 25 seconds timeout
%.out: %$(EXEEXT) fwk/lib/timeout
	mkdir -p `dirname $@`
	-fwk/lib/timeout 25 $(GPSIM) -i -s $< -c $(PORTS_DIR)/pic16/gpsim.cmd > $@ || \
          echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(EXEEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	python get_ticks.py < $@ >> $@
	-grep -n FAIL $@ /dev/null || true

fwk/lib/timeout: fwk/lib/timeout.c

_clean:
	rm -f fwk/lib/timeout fwk/lib/timeout.exe $(PORTS_DIR)/$(PORT)/*.rel $(PORTS_DIR)/$(PORT)/*.rst \
	      $(PORTS_DIR)/$(PORT)/*.lst $(PORTS_DIR)/$(PORT)/*.sym $(PORTS_DIR)/$(PORT)/*.asm \
	      $(PORTS_DIR)/$(PORT)/*.lnk $(PORTS_DIR)/$(PORT)/*.map $(PORTS_DIR)/$(PORT)/*.mem
