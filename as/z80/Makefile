PRJDIR = ../..

include $(PRJDIR)/Makefile.common

SDC	= .
OBJS	= asdata.o asexpr.o aslex.o aslist.o asmain.o asout.o \
	  assubr.o assym.o z80adr.o z80ext.o z80mch.o z80pst.o
BINS	= as

CFLAGS	+= $(OPTS) -I. -DINDEXLIB -DMLH_MAP -DUNIX -DSDK
CFLAGS	+= -funsigned-char
LDFLAGS += -lm

all:	$(BINS)

$(BINS): $(OBJS)
	$(CC) $(LDFLAGS) -o $(BINS) $(OBJS)

%.o:	%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(BINS)
