#Generic rules and options for building with Borland C++

.cacheautodepend

YACC = bison
LEX = flex
CC = bcc32

SLIB = $(PRJDIR)/support/Util

OFLAGS = -6 -O2 -w-proto -g0

.c.obj:
        $(CC) -c -o$@ $(OFLAGS) $(CFLAGS) $(CPPFLAGS) -I$(SLIB) $<
