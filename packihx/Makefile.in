PRJDIR          = ..

include $(PRJDIR)/Makefile.common

CFLAGS          += -Wall

OBJECTS 	= packihx.o

SOURCES		= $(patsubst %.o,%.c,$(OBJECTS))

TARGET		= $(PRJDIR)/bin/packihx$(EXEEXT)

all: $(TARGET)

install: all installdirs
	$(INSTALL) $(TARGET) `echo $(bindir)/packihx$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(bindir)/packihx$(EXEEXT)|sed '$(transform)'`

# Deleting all the installed files
# --------------------------------
uninstall:
	rm -f $(bindir)/packihx$(EXEEXT)

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
