CC = sdcc

CFLAGS= -Wa-s -V --verbose
MFLAGS= -mxa51
#LFLAGS=

all: hello.hex


hello.hex: hello.c
	$(CC) $(CFLAGS) $(MFLAGS) hello.c

clean:
	rm -f core *~ \#* *.asm *.cdb *.rel *.hex *.ihx *.lst *.map \
		*.xa *.rst *.sym *.lnk *.lib *.bin *.dump* *.log

