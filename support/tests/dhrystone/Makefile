# Simple Makefile for dhrystone and sdcc

CC = /home/michaelh/projects/sdcc/bin/sdcc
# -DNOENUM is here to make the results more predictable
CFLAGS = -mz80 -v --dumpall
CFLAGS += -DREG= -DNOSTRUCTASSIGN -DNOENUM -DBROKEN_SDCC=0 -DHZ=100
LIBDIR = /home/michaelh/projects/sdcc/device/lib/z80/
LD = link-z80

OBJ = dhry.o

all: dhry

dhry: $(OBJ)
	$(LD) -n -- -i -m -k$(LIBDIR) -lz80.lib \
	-b_CODE=0x200 dhry.ihx $(LIBDIR)crt0.o $(OBJ)
	cat dhry.ihx | ../../makebin/makebin > dhry.rom

native:
	gcc -g -O2 -DREG= -DNOSTRUCTASSIGN -DNOENUM -o dhry dhry.c

.c.o:
	$(CC) $(CFLAGS) $<

dhry.c: $(CC)

clean:
	rm -r *~ dhry

