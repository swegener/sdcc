CC = sdcc

CFLAGS= -Wa-s 
MFLAGS= -mxa51
LFLAGS= --xram-loc 0x4000

all: hello.hex

OBJECTS = hello.rel hwinit.rel

hello.hex: $(OBJECTS)
	$(CC) $(MFLAGS) $(LFLAGS) $(OBJECTS)

clean:
	rm -f core *~ \#* *.asm *.cdb *.rel *.hex *.ihx *.lst *.map \
		*.xa *.rst *.sym *.lnk *.lib *.bin *.dump* *.log

%.rel: %.c hw.h
	$(CC) -c $(CFLAGS) $(MFLAGS) $<
