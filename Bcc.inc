#Generic rules and options for building with Borland C++

OFLAGS = -6 -O2 -g0 -w-proto

.c.obj:
        BCC32 -c $(OFLAGS) $(CFLAGS) $(CPPFLAGS) $<
