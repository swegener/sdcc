# Port specification for the mcs51 port running with uCsim
#
# model small stack-auto

include $(PORTS_DIR)/mcs51-stack-auto/spec.mk

LIBSDCCFLAGS+= --xstack
SDCCFLAGS   +=$(LIBSDCCFLAGS)
