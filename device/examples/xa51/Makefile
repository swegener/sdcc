CC = sdcc

#CFLAGS=
#MFLAGS=
#LFLAGS=

all: hello.hex


hello.hex: hello.c
	$(CC) -mxa51 hello.c

clean:
	rm -f core *~ \#* *.asm *.cdb *.rel *.hex *.ihx *.lst *.map \
		*.xa *.rst *.sym *.lnk *.lib *.bin *.dump*

