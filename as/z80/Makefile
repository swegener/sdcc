PRJDIR = ../..

include $(PRJDIR)/Makefile.common

SDC	= .
SRC	= asdata.c asexpr.c aslex.c aslist.c asmain.c asout.c \
	  assubr.c assym.c z80adr.c z80ext.c z80mch.c z80pst.c
OBJS	= $(SRC:%.c=%.o)
BINS	= $(BUILDDIR)/as$(EXT)

CFLAGS	+= $(OPTS) -I. -DINDEXLIB -DMLH_MAP -DUNIX -DSDK
CFLAGS	+= -funsigned-char
LDFLAGS += -lm

all:	dep $(BINS)

dep: Makefile.dep

Makefile.dep: $(SRC) *.h
	$(CPP) $(CPPFLAGS) $(M_OR_MM) $(SRC) > Makefile.dep

include Makefile.dep

$(BINS): $(OBJS)
	$(CC) $(LDFLAGS) -o $(BINS) $(OBJS)

# Hmm.  Should only rebuild if any of the source files are changed,
# but must do a clean build to make sure that there isnt a mix of
# z80 and gb stuff.
wary: $(OBJS)
	make clean
	make $(BINS)

%.o:	%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(BINS)
