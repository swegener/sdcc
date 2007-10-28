# Port specification for the mcs51 port running with uCsim
#
# model small

SDCCFLAGS +=--model-small

include $(PORTS_DIR)/mcs51-common/spec.mk

LIBDIR = $(top_builddir)/device/lib/build/small
