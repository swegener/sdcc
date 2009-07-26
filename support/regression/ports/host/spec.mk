# Port specification for compiling on the host machines version of gcc
SDCC = $(shell ( sh -c "gcc --version" 2>&1 ) > /dev/null  && echo gcc || echo cc)
SDCCFLAGS = $(CPPFLAGS) -DPORT_HOST=1 -fsigned-char -fpack-struct -DREENTRANT= -I$(top_builddir) -I$(top_srcdir)
# disable all warnings:
SDCCFLAGS+= -w
# enable all warnings:
#SDCCFLAGS+= -Wall

BINEXT = .bin
OBJEXT = .o
INC_DIR = .

# otherwise `make` deletes testfwk.o and `make -j` will fail
.PRECIOUS: $(PORT_CASES_DIR)/%$(OBJEXT)

# Required extras
EXTRAS = $(PORT_CASES_DIR)/testfwk$(OBJEXT) $(PORT_CASES_DIR)/support$(OBJEXT)

%.out: %$(BINEXT)
	mkdir -p $(dir $@)
	-$< > $@
	-grep -n FAIL $@ /dev/null || true

%$(BINEXT): %$(OBJEXT) $(EXTRAS)
	$(SDCC) $(SDCCFLAGS) -o $@ $< $(EXTRAS) -lm

%$(OBJEXT): %.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/$(PORT)/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(PORT_CASES_DIR)/%$(OBJEXT): fwk/lib/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

_clean:
