PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJDIR	= obj/$(EXT)
SRC	= lkarea.c lkdata.c lkeval.c lkhead.c lkihx.c lklex.c \
	  lklibr.c lklist.c lkmain.c lkrloc.c lks19.c lksym.c \
	  lkgb.c lkgg.c
OBJS	= $(SRC:%.c=$(OBJDIR)/%.o)
BINS	= $(BUILDDIR)link$(EXT)

CFLAGS	+= $(OPTS) -DINDEXLIB -DMLH_MAP -DUNIX -DSDK
CFLAGS	+= -funsigned-char -DUNIX

all:	$(BINS)

$(BINS): $(OBJDIR) $(OBJS)
	$(CC) $(LDFLAGS) -o $(BINS) $(OBJS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o:	%.c
	$(CC) -c $(CFLAGS) -I$(PRJDIR)/as/$(PORT) -o $@ $<

clean:
	rm -rf $(OBJS) $(BINS) $(OBJDIR)

_link-z80:
	$(MAKE) EXT=-z80$(E)

_link-gbz80:
	$(MAKE) EXT=-gbz80$(E)
