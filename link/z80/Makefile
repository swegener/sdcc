TOPDIR 	= ../..

include $(TOPDIR)/Makefile.common

OBJS	= lkarea.o lkdata.o lkeval.o lkhead.o lkihx.o lklex.o \
	  lklibr.o lklist.o lkmain.o lkrloc.o lks19.o lksym.o \
	  lkgb.o lkgg.o
BINS	= link$E

CFLAGS	+= -funsigned-char -DUNIX

all:	$(BINS)

$(BINS): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o:	%.c
	$(CC) -c $(CFLAGS) -I$(TOPDIR)/as/$(PORT) -o $@ $<

clean:
	rm -f $(OBJS) $(BINS)
