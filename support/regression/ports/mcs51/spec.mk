# Port specification for the mcs51 port running with uCsim
#
# model small

# path to uCsim
S51A = $(SDCC_DIR)/sim/ucsim/s51.src/s51
S51B = $(SDCC_DIR)/bin/s51

S51 = $(shell if [ -f $(S51A) ]; then echo $(S51A); else echo $(S51B); fi)

SDCCFLAGS +=--nostdinc --less-pedantic -DREENTRANT=reentrant
LINKFLAGS = --nostdlib
LINKFLAGS += mcs51.lib libsdcc.lib liblong.lib libint.lib libfloat.lib
LIBDIR = $(SDCC_DIR)/device/lib/build/small

OBJEXT = .rel
EXEEXT = .ihx

EXTRAS = $(PORTS_DIR)/$(PORT)/testfwk$(OBJEXT) $(PORTS_DIR)/$(PORT)/support$(OBJEXT)

# Rule to link into .ihx
%$(EXEEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) $(LINKFLAGS) -L $(LIBDIR) $(EXTRAS) $< -o $@

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORTS_DIR)/$(PORT)/testfwk$(OBJEXT): fwk/lib/testfwk.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

# run simulator with 30 seconds timeout
%.out: %$(EXEEXT) fwk/lib/timeout
	mkdir -p `dirname $@`
	-fwk/lib/timeout 30 $(S51) -t32 -S in=/dev/null,out=$@ $< < $(PORTS_DIR)/mcs51/uCsim.cmd >/dev/null \
	  || echo -e --- FAIL: \"timeout, simulation killed\" in $(<:$(EXEEXT)=.c)"\n"--- Summary: 1/1/1: timeout >> $@
	-grep -n FAIL $@ /dev/null || true

fwk/lib/timeout: fwk/lib/timeout.c


_clean:
	rm -f fwk/lib/timeout fwk/lib/timeout.exe $(PORTS_DIR)/$(PORT)/*.rel $(PORTS_DIR)/$(PORT)/*.rst \
	      $(PORTS_DIR)/$(PORT)/*.lst $(PORTS_DIR)/$(PORT)/*.sym $(PORTS_DIR)/$(PORT)/*.asm \
	      $(PORTS_DIR)/$(PORT)/*.lnk $(PORTS_DIR)/$(PORT)/*.map $(PORTS_DIR)/$(PORT)/*.mem
