PRJDIR = ../..

SOURCES = makebin.c
BIN = $(PRJDIR)/bin/makebin$(EXEEXT)

include $(PRJDIR)/Makefile.common

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJ)

install: all
	mkdir -p $(bindir)
	$(INSTALL) $(BIN) `echo $(bindir)/makebin$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(bindir)/makebin$(EXEEXT)|sed '$(transform)'`

uninstall:
	rm -f $(bindir)/makebin$(EXEEXT)

include clean.mk
