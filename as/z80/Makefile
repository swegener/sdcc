PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJDIR	= obj/$(EXT)

SDC	= .

SLIBSRC	= NewAlloc.c

SRC	= asdata.c asexpr.c aslex.c aslist.c asmain.c asout.c \
	  assubr.c assym.c z80adr.c z80ext.c z80mch.c z80pst.c

OBJS	= $(SRC:%.c=$(OBJDIR)/%.o)
SLIBOBJS	= $(SLIBSRC:%.c=$(OBJDIR)/%.o) 

BINS	= $(BUILDDIR)as$(EXT)$(EXEEXT)

CFLAGS	+= $(CPPFLAGS) $(OPTS) -I. -DINDEXLIB -DMLH_MAP -DUNIX -DSDK
CFLAGS	+= -funsigned-char
CFLAGS	+= -I$(SLIB) 

LDFLAGS += -lm $(EXTRALIBS)

all:	dep $(BINS)

dep: Makefile.dep

Makefile.dep: $(SRC) *.h
	$(CPP) $(CPPFLAGS) $(M_OR_MM) $(SRC) > Makefile.dep

include Makefile.dep

# We need a '.stamp'-file in $(OBJDIR), because the time stamp
# of $(OBJDIR) itself is always updated, when a file in $(OBJDIR)
# is updated. A rule like
#	$(OBJDIR)/anyfile: $(OBJDIR)
# will always force a remake (at least on Linux ext2).

$(OBJDIR)/.stamp:
	mkdir -p $(OBJDIR)
	touch $(OBJDIR)/.stamp

$(BINS): $(OBJS) $(SLIBOBJS)
	$(CC) -o $(BINS) $(OBJS) $(SLIBOBJS) $(LDFLAGS)


$(OBJDIR)/%.o:	%.c $(OBJDIR)/.stamp
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o:	$(SLIB)/%.c $(OBJDIR)/.stamp
	$(CC) -c $(CFLAGS) -o $@ $<

_as-z80:
	$(MAKE) EXT=-z80$(E)

_as-gbz80:
	$(MAKE) EXT=-gbz80$(E) OPTS=-DGAMEBOY

include clean.mk
