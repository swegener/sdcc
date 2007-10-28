# Port specification for the mcs51-large port running with uCsim
#
# model large

SDCCFLAGS +=--model-large

include $(PORTS_DIR)/mcs51-common/spec.mk

LIBDIR = $(top_builddir)/device/lib/build/large
