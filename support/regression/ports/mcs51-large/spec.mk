# Port specification for the mcs51 port running with uCsim
#
# model large

SDCCFLAGS +=--model-large

include $(PORTS_DIR)/mcs51/spec.mk

# use C sources from mcs51
$(PORT_CASES_DIR)/%$(OBJEXT): $(PORTS_DIR)/mcs51/%.c
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

LIBDIR = $(top_builddir)device/lib/build/large
