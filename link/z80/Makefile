TOPDIR 	= ../..

include $(TOPDIR)/Makefile.common

OBJS	= lkarea.o lkdata.o lkeval.o lkhead.o lkihx.o lklex.o \
	  lklibr.o lklist.o lkmain.o lkrloc.o lks19.o lksym.o \
	  lkgb.o lkgg.o
BINS	= $(TOPDIR)/bin/link-gbz80$E 

CFLAGS	+= -DINDEXLIB -DMLH_MAP -DGAMEBOY -DUNIX -DSDK -DSDK_VERSION_STRING="\"3.0.0\"" -DTARGET_STRING="\"gbz80\""
CFLAGS	+= -funsigned-char -DUNIX

all:	$(BINS)

$(BINS): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o:	%.c
	$(CC) -c $(CFLAGS) -I$(TOPDIR)/as/$(PORT) -o $@ $<

clean:
	rm -f $(OBJS) $(BINS)
