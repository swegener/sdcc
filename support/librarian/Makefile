PRJDIR          = ../..

include $(PRJDIR)/Makefile.common

CFLAGS          += -Wall

OBJECTS 	= sdcclib.o

SOURCES		= $(patsubst %.o,%.c,$(OBJECTS))

TARGET		= $(PRJDIR)/bin/sdcclib$(EXEEXT)

all: $(TARGET)

install: all installdirs
	$(INSTALL) $(TARGET) `echo $(bindir)/sdcclib$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(bindir)/sdcclib$(EXEEXT)|sed '$(transform)'`

# Deleting all the installed files
# --------------------------------
uninstall:
	rm -f $(bindir)/sdcclib$(EXEEXT)

# Performing self-test
# --------------------
check:


# Performing installation test
# ----------------------------
installcheck:


# Creating installation directories
# ---------------------------------
installdirs:
	$(INSTALL) -d $(bindir)


# Creating dependencies
# ---------------------
dep:

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@


checkconf:

include clean.mk
