CC = gcc
CFLAGS = -O2
YACC = bison -y -d
LEX = flex -i
LEXLIB = 

xa_asm: xa_main.o xa_asm.tab.o xa_asm.lex.o xa_dasm.o
	$(CC) -o xa_asm xa_main.o xa_asm.tab.o xa_asm.lex.o xa_dasm.o $(LEXLIB)
	strip xa_asm

xa_asm.lex.o: xa_asm.lex.c xa_asm.tab.h xa_main.h
	$(CC) $(CFLAGS) -c xa_asm.lex.c

xa_asm.tab.o: xa_asm.tab.c xa_asm.tab.h xa_main.h
	$(CC) $(CFLAGS) -c xa_asm.tab.c

xa_main.o: xa_main.c xa_main.h
	$(CC) $(CFLAGS) -Wall -c xa_main.c

xa_dasm.o: xa_dasm.c
	$(CC) $(CFLAGS) -Wall -c xa_dasm.c

xa_asm.tab.c xa_asm.tab.h: xa_asm.y
	$(YACC) xa_asm.y
	mv y.tab.c xa_asm.tab.c
	mv y.tab.h xa_asm.tab.h

xa_asm.lex.c: xa_asm.l
	$(LEX) xa_asm.l
	mv lex.yy.c xa_asm.lex.c

all: touch xa_asm

touch:
	touch xa_asm.y xa_asm.l xa_dasm.c

clean:
	rm -f *.tab.c *.tab.h *.lex.c *.o *.bak y.output *.hex *.lst *.obj
	rm -f xa_asm core *~

