PRJDIR = ../..

SOURCES = makebin.c
BIN = $(PRJDIR)/bin/makebin$(EXEEXT)

include $(PRJDIR)/Makefile.common

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJ)

install: all
	mkdir -p $(DESTDIR)$(bindir)
	$(INSTALL) $(BIN) `echo $(DESTDIR)$(bindir)/makebin$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(DESTDIR)$(bindir)/makebin$(EXEEXT)|sed '$(transform)'`

uninstall:
	rm -f $(DESTDIR)$(bindir)/makebin$(EXEEXT)

include clean.mk
