# Port specification for the mcs51-stack-auto port running with uCsim
#
# model small stack-auto

SDCCFLAGS +=--stack-auto

include $(PORTS_DIR)/mcs51-common/spec.mk

LIBDIR = $(top_builddir)/device/lib/build/small-stack-auto
