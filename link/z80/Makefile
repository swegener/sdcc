PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJS	= lkarea.o lkdata.o lkeval.o lkhead.o lkihx.o lklex.o \
	  lklibr.o lklist.o lkmain.o lkrloc.o lks19.o lksym.o \
	  lkgb.o lkgg.o
BINS	= $(BUILDDIR)/link$(EXT)

CFLAGS	+= $(OPTS) -DINDEXLIB -DMLH_MAP -DUNIX -DSDK
CFLAGS	+= -funsigned-char -DUNIX

all:	$(BINS)

$(BINS): $(OBJS)
	$(CC) $(LDFLAGS) -o $(BINS) $(OBJS)

%.o:	%.c
	$(CC) -c $(CFLAGS) -I$(PRJDIR)/as/$(PORT) -o $@ $<

clean:
	rm -f $(OBJS) $(BINS)
