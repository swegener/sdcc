VPATH = $(srcdir)

CC		= sdcc -mz80

CPPFLAGS	=
CFLAGS		= --debug
LDFLAGS		=
LIBS		=

ALL		= $(MAIN) $(OTHERS)
OBJECTS		= $(MAIN).rel $(OTHERS:=.rel)

all: $(MAIN).hex | silent

silent:
	@echo -n

dep: $(MAIN).dep

$(MAIN).dep: $(addprefix $(srcdir)/,$(OBJECTS:.rel=.c)) $(srcdir)/*.h
	@>'$@'
	@for c in $(addprefix $(srcdir)/,$(OBJECTS:.rel=.c)); do \
		$(CC) -MM $(CPPFLAGS) $$c >>'$@' ;\
	done

include $(MAIN).dep

$(MAIN).ihx: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

.SUFFIXES: .rel .ihx .hex

.c.rel:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

.ihx.hex:
	packihx $< >$@

clean:
	rm -f $(ALL:=.rel) $(ALL:=.asm) $(ALL:=.lst) $(ALL:=.rst) $(ALL:=.sym) $(ALL:=.adb)
	rm -f $(MAIN).ihx $(MAIN).hex $(MAIN).lk $(MAIN).map $(MAIN).mem $(MAIN).cdb $(MAIN).omf $(MAIN).noi
	rm -f *~
	rm -f $(MAIN).dep


# End of sdcc.mk
