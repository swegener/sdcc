VPATH = $(srcdir)


TARGET		= stm8

CC		= sdcc -m$(TARGET) --std-c99

CPPFLAGS	=
CFLAGS		= --debug
LDFLAGS		=
LIBS		=

-include	$(srcdir)/$(MAIN).mk

DEVICES		?= S208

ALL		= $(MAIN) $(OTHERS)

OBJECTS		= $(MAIN).rel $(OTHERS:=.rel)

CPPFLAGS	= -DDEVICE=DEV_STM8$(DEVICE) -I$(VPATH)

.SUFFIXES: .rel .ihx .hex

.PHONY: $(DEVICES)

all: $(DEVICES) | silent

silent:
	@echo -n

$(DEVICES):
	@test -d $@ || mkdir $@
	@$(MAKE) --no-print-directory -C $@ DEVICE=$@ REAL=yes MAIN=$(MAIN) -f$(VPATH)/sdcc.mk compile

compile: dep $(MAIN).hex | silent

.c.rel:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

.ihx.hex:
	packihx $< >$@

$(MAIN).ihx: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAAGS) $(OBJECTS) -o $@

$(MAIN).cdb: $(MAIN).hex

dep: $(MAIN).dep

$(MAIN).dep: $(addprefix $(srcdir)/,$(OBJECTS:.rel=.c)) $(srcdir)/*.h
	@>'$@'
	@for c in $(addprefix $(srcdir)/,$(OBJECTS:.rel=.c)); do \
		$(CC) -MM $(CPPFLAGS) $$c >>'$@'; \
	done

clean:
	rm -rf $(DEVICES)

ifeq ($(REAL),yes)
include $(MAIN).dep
endif
