all: p.hex

p.hex: p.bin
	srec_cat -Output p.hex -Intel p.bin -Binary

p.bin: p.asm
	z80asm -o p.bin p.asm
