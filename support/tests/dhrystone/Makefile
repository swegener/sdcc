# Simple Makefile for dhrystone and sdcc

PROC = gbz80
PORT = gb

CC = /home/michaelh/projects/gbdk-support/lcc/lcc
# -DNOENUM is here to make the results more predictable
CFLAGS =
CFLAGS += -DREG= -DNOSTRUCTASSIGN -DNOENUM
LIBDIR = /home/michaelh/projects/gbdk-lib/build/
LD = link-$(PROC)
LDFLAGS = -v
AS = as-$(PROC)

OBJ = dhry.o

all: dhry

dhry: $(OBJ)
	$(CC) -o dhry.gb dhry.o

native:
	gcc -g -O2 -DREG= -DNOSTRUCTASSIGN -DNOENUM -o dhry dhry.c

dhry.c:

clean:
	rm -r *~ dhry
